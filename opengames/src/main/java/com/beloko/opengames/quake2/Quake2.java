/*
 * Copyright (C) 2009 jeyries@yahoo.fr
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.beloko.opengames.quake2;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.EGLConfigChooser;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.os.Vibrator;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;

import com.bda.controller.Controller;
import com.bda.controller.ControllerListener;
import com.bda.controller.StateEvent;
import com.beloko.opengames.AppSettings;
import com.beloko.opengames.CDAudioPlayer;
import com.beloko.opengames.FPSLimit;
import com.beloko.opengames.GD;
import com.beloko.opengames.Utils;
import com.beloko.touchcontrols.ControlInterpreter;
import com.beloko.touchcontrols.CustomCommands;
import com.beloko.touchcontrols.MogaHack;
import com.beloko.touchcontrols.TouchSettings;
import com.beloko.touchcontrols.TouchControlsSettings;

import java.io.IOException;
import java.nio.ByteBuffer;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

public class Quake2 extends Activity 
{
	String LOG = "Quake2";

	private ControlInterpreter controlInterp;

	private final MogaControllerListener mogaListener = new MogaControllerListener();
	Controller mogaController = null;

	private String args;


	private QuakeView mGLSurfaceView = null;
	private QuakeRenderer mRenderer = new QuakeRenderer();
	Activity act;

	private Vibrator vibrator;

	private boolean please_exit = false;

	//Audio thread locking
	private static final Object threadLock = new Object();
	private static boolean doThreadLock;
	
	
	// android settings - saved as preferences
	private boolean debug = false,	
			invert_roll = false,
			enable_audio = true,
			enable_sensor = true,
			enable_vibrator = false,
			enable_ecomode = false;

	private long tstart;
	private int timelimit = 0; //4*60000;


	public static final String version = "1.91" ;


	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);           

		Log.i("Quake2.java", "onCreate " + version);

		act = this;

		AppSettings.setGame(GD.IDGame.Quake2);//Always Quake
		AppSettings.reloadSettings(getApplication());

		args = getIntent().getStringExtra("args");


		mogaController = Controller.getInstance(this);
		MogaHack.init(mogaController,this);
		mogaController.setListener(mogaListener, new Handler());

		load_preferences();              

		//Log.i( "Quake2", "version : " + getVersion());

		// fullscreen
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		// keep screen on 
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
				WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		Utils.setImmersionMode(this);


		GD.init(getApplicationContext());


		Quake2Lib.loadLibraries();
		start_quake2();   

	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		Utils.onWindowFocusChanged(this, hasFocus);
	}

	public void load_preferences(){

		// Restore preferences
		SharedPreferences settings = getPreferences( MODE_PRIVATE );
		debug = settings.getBoolean("debug", debug);
		invert_roll = settings.getBoolean("invert_roll", invert_roll);
		enable_audio = settings.getBoolean("enable_audio", enable_audio );
		enable_sensor = settings.getBoolean("enable_sensor", enable_sensor);
		enable_vibrator = settings.getBoolean("enable_vibrator", enable_vibrator);
		enable_ecomode = settings.getBoolean("enable_ecomode", enable_ecomode);   

	}

	public void save_preferences(){
		// Save user preferences. We need an Editor object to
		// make changes. All objects are from android.context.Context
		SharedPreferences settings = getPreferences( MODE_PRIVATE );
		SharedPreferences.Editor editor = settings.edit();

		editor.putBoolean("debug", debug);
		editor.putBoolean("invert_roll", invert_roll);
		editor.putBoolean("enable_audio", enable_audio);
		editor.putBoolean("enable_sensor", enable_sensor);
		editor.putBoolean("enable_vibrator", enable_vibrator);
		editor.putBoolean("enable_ecomode", enable_ecomode); 

		// Don't forget to commit your edits!!!
		editor.commit();
	}




	public void start_quake2() {

		Quake2Lib engine = new Quake2Lib();
		controlInterp = new ControlInterpreter(engine,Utils.getGameGamepadConfig(AppSettings.game), TouchSettings.gamePadControlsFile, TouchSettings.gamePadEnabled);

		TouchControlsSettings.setup(act, engine);
		TouchControlsSettings.loadSettings(act);
		TouchControlsSettings.sendToQuake();

		CustomCommands.setup(act, engine, getIntent().getStringExtra("main_qc"), getIntent().getStringExtra("mod_qc"));


		mRenderer.speed_limit = enable_ecomode ? 40 : 0;


		vibrator = (Vibrator)getSystemService(Context.VIBRATOR_SERVICE);


		// Create our Preview view and set it as the content of our
		// Activity
		mGLSurfaceView = new QuakeView(this);
		//mGLSurfaceView.setGLWrapper( new MyWrapper());
		//mGLSurfaceView.setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR | GLSurfaceView.DEBUG_LOG_GL_CALLS);
		//setEGLConfigChooser  (int redSize, int greenSize, int blueSize, int alphaSize, int depthSize, int stencilSize)
		//mGLSurfaceView.setEGLConfigChooser(8,8,8,0,16,0);
		mGLSurfaceView.setEGLConfigChooser( new QuakeEGLConfigChooser() );

		mGLSurfaceView.setPreserveEGLContextOnPause( true);

		mGLSurfaceView.setRenderer(mRenderer);

		// This will keep the screen on, while your view is visible. 
		mGLSurfaceView.setKeepScreenOn(true);

		setContentView(mGLSurfaceView);
		mGLSurfaceView.requestFocus();
		mGLSurfaceView.setFocusableInTouchMode(true);

	}

	class QuakeEGLConfigChooser implements EGLConfigChooser {


		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

			Log.i( "Quake2.java", "chooseConfig");


			int[] mConfigSpec  = {
					EGL10.EGL_RED_SIZE, 8,
					EGL10.EGL_GREEN_SIZE, 8,
					EGL10.EGL_BLUE_SIZE, 8,
					EGL10.EGL_ALPHA_SIZE, 8,
					EGL10.EGL_DEPTH_SIZE, 16,
					//EGL10.EGL_STENCIL_SIZE, 0,
					EGL10.EGL_NONE};


			int[] num_config = new int[1];
			egl.eglChooseConfig(display, mConfigSpec, null, 0, num_config);

			int numConfigs = num_config[0];

			Log.i( "Quake2.java", "numConfigs="+numConfigs);

			if (numConfigs <= 0) {
				throw new IllegalArgumentException(
						"No EGL configs match configSpec");
			}

			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, mConfigSpec, configs, numConfigs,
					num_config);


			if (debug)
				for(EGLConfig config : configs) {
					Log.i( "Quake2.java", "found EGL config : " + printConfig(egl,display,config));       	
				}


			// best choice : select first config
			Log.i( "Quake2.java", "selected EGL config : " + printConfig(egl,display,configs[0]));

			return configs[0];
		}


		private  String printConfig(EGL10 egl, EGLDisplay display,
				EGLConfig config) {

			int r = findConfigAttrib(egl, display, config,
					EGL10.EGL_RED_SIZE, 0);
			int g = findConfigAttrib(egl, display, config,
					EGL10.EGL_GREEN_SIZE, 0);
			int b = findConfigAttrib(egl, display, config,
					EGL10.EGL_BLUE_SIZE, 0);
			int a = findConfigAttrib(egl, display, config,
					EGL10.EGL_ALPHA_SIZE, 0);
			int d = findConfigAttrib(egl, display, config,
					EGL10.EGL_DEPTH_SIZE, 0);
			int s = findConfigAttrib(egl, display, config,
					EGL10.EGL_STENCIL_SIZE, 0);

			/*
			 * 
			 * EGL_CONFIG_CAVEAT value 

         #define EGL_NONE		       0x3038	
         #define EGL_SLOW_CONFIG		       0x3050	
         #define EGL_NON_CONFORMANT_CONFIG      0x3051	
			 */

			return String.format("EGLConfig rgba=%d%d%d%d depth=%d stencil=%d", r,g,b,a,d,s)
					+ " native=" + findConfigAttrib(egl, display, config, EGL10.EGL_NATIVE_RENDERABLE, 0)
					+ " buffer=" + findConfigAttrib(egl, display, config, EGL10.EGL_BUFFER_SIZE, 0)
					+ String.format(" caveat=0x%04x" , findConfigAttrib(egl, display, config, EGL10.EGL_CONFIG_CAVEAT, 0))
					;
		}

		private int findConfigAttrib(EGL10 egl, EGLDisplay display,
				EGLConfig config, int attribute, int defaultValue) {

			int[] mValue = new int[1];
			if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
				return mValue[0];
			}
			return defaultValue;
		}

	} // end of QuakeEGLConfigChooser

	//protected void onStart();




	@Override
	protected void onPause() {
		Log.i( "Quake2.java", "onPause" );
		CDAudioPlayer.onPause();
		mogaController.onPause();
		mGLSurfaceView.onPause();
		doThreadLock = true;
		super.onPause();
	}

	@Override
	protected void onResume() {

		Log.i( "Quake2.java", "onResume" );
		CDAudioPlayer.onResume();
		mogaController.onResume();
		super.onResume();

		mGLSurfaceView.onResume();
		
		doThreadLock = false;
		synchronized (threadLock){
			threadLock.notifyAll();
		}	
	}

	@Override
	protected void onRestart() {
		Log.i( "Quake2.java", "onRestart" );
		super.onRestart();
	}


	@Override
	protected void onStop() {
		Log.i( "Quake2.java", "onStop" );
		super.onStop();	
	}

	@Override
	protected void onDestroy() {
		Log.i( "Quake2.java", "onDestroy" ); 
		super.onDestroy();
		mogaController.exit();
		System.exit(0);
	}





	class MogaControllerListener implements ControllerListener {


		@Override
		public void onKeyEvent(com.bda.controller.KeyEvent event) {
			//Log.d(LOG,"onKeyEvent " + event.getKeyCode());
			controlInterp.onMogaKeyEvent(event,mogaController.getState(Controller.STATE_CURRENT_PRODUCT_VERSION));
		}

		@Override
		public void onMotionEvent(com.bda.controller.MotionEvent event) {
			controlInterp.onGenericMotionEvent(event);
		}

		@Override
		public void onStateEvent(StateEvent event) {
			Log.d(LOG,"onStateEvent " + event.getState());
		}
	}


	class QuakeView extends GLSurfaceView {


		public QuakeView(Context context) {
			super(context);

		}


		@Override
		public boolean onGenericMotionEvent(MotionEvent event) {
			return controlInterp.onGenericMotionEvent(event);
		}

		@Override
		public boolean onTouchEvent(MotionEvent event)
		{
			return controlInterp.onTouchEvent(event);
		}

		@Override
		public boolean onKeyDown(int keyCode, KeyEvent event)
		{
			return controlInterp.onKeyDown(keyCode, event);
		}

		@Override
		public boolean onKeyUp(int keyCode, KeyEvent event)
		{
			return controlInterp.onKeyUp(keyCode, event);
		} 

	}  // end of QuakeView

	///////////// GLSurfaceView.Renderer implementation ///////////

	class QuakeRenderer implements GLSurfaceView.Renderer {



		public void onSurfaceCreated(GL10 gl, EGLConfig config) {


			Log.d("Renderer", "onSurfaceCreated");

			gl.glDisable(GL10.GL_DITHER);

			gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT,
					//GL10.GL_FASTEST);
					GL10.GL_NICEST);

		}





		private void init( int width, int height ){

			Log.i( "Quake2", "screen size : " + width + "x"+ height);

			Quake2Lib.Quake2SetScreenSize(width, height);

			if (AppSettings.getIntOption(getApplicationContext(), "quake2_hud_size", 0) == 0) //Large
			{
				Quake2Lib.Quake2SetWidth(500);
				Quake2Lib.Quake2SetHeight(280);
			}
			else if (AppSettings.getIntOption(getApplicationContext(), "quake2_hud_size", 0) == 1) //Medium
			{
				Quake2Lib.Quake2SetWidth(700);
				Quake2Lib.Quake2SetHeight(400);

			}
			else if (AppSettings.getIntOption(getApplicationContext(), "quake2_hud_size", 0) == 2) //Small
			{
				Quake2Lib.Quake2SetWidth(1000);
				Quake2Lib.Quake2SetHeight(800);
			}

			Utils.copyPNGAssets(getApplicationContext(),AppSettings.graphicsDir);   

			fpsLimit = new FPSLimit(getApplicationContext());

			Log.i("Quake2", "Quake2Init start");

			//Quake2Lib.setLibraryPath(getFilesDir().getPath() + "/../lib/");
			Quake2Lib.setLibraryPath(getApplicationInfo().nativeLibraryDir); //Updated 01/03/14


			String[] args_array = Utils.creatArgs(args);

			int gamedll = getIntent().getIntExtra("game_dll", Quake2Lib.Q2DLL_GAME);

			int ret =Quake2Lib.Quake2Init(AppSettings.graphicsDir, 64, args_array, gamedll);

			Log.i("Quake2", "Quake2Init done");

			tstart = SystemClock.uptimeMillis();
		}




		private int counter_fps=0;
		private long tprint_fps= 0;
		private int framenum=0;

		// speed limit : 10 FPS
		private int speed_limit = 0; 
		//40;
		//100;
		//200;

		private int vibration_duration = //0;
				100;

		private boolean vibration_running = false;
		private long vibration_end;

		private long tprev = 0;
		private boolean paused = false;

		private boolean audio_initalised = false;
		//// new Renderer interface
		int notifiedflags;

		FPSLimit fpsLimit;

		public void onDrawFrame(GL10 gl) {

			fpsLimit.tick();

			long tnow = SystemClock.uptimeMillis(); 
			int tdelta = (int)(tnow-tprev) ;
			if ( tprev == 0 ) tdelta = 0;
			tprev = tnow;

			if ( timelimit!=0 && (tnow-tstart)>= timelimit){
				Log.i( "Quake2.java", "Timer expired. exiting");
				finish();
				timelimit = 0;
			}

			// compute FPS
			if ( (tnow-tprint_fps) >= 1000){
				if (debug)
					Log.i("Quake2",String.format( "FPS= %d",counter_fps));

				tprint_fps = tnow;
				counter_fps = 0;        	
			}
			counter_fps ++;



			if (Quake2Lib.Quake2GetDisableScreen()==0){

				// start audio thread
				if (!audio_initalised)
				{
					Log.d(LOG,"Starting audio");
					new Thread( new Runnable(){
						public void run() {
							try {
								audio_thread();
							} catch (IOException e) {
								e.printStackTrace();
							}		
						}}).start();
					audio_initalised = true;	
				}
			}



			/*
			 * Usually, the first thing one might want to do is to clear
			 * the screen. The most efficient way of doing this is to use
			 * glClear().
			 */

			int vibration = 0;

			//Log.i("Quake2", "Quake2Frame start");

			//if (framenum < 30)
			//	Log.i("Quake2", String.format("frame %d",framenum));


			//while( sQuake2Frame()==0 );   	
			int flags =  sQuake2Frame();

			if (flags != notifiedflags)
			{
				if (((flags ^ notifiedflags) & 1) != 0)
				{
					final int fl = flags;
					Runnable r = new Runnable() 
					{	//doing this on the ui thread because android sucks.
						public void run()
						{
							InputMethodManager im = (InputMethodManager) act.getSystemService(Context.INPUT_METHOD_SERVICE);
							if (im != null)
							{
								if ((fl & 1) != 0)
								{
									//								getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
									im.showSoftInput(mGLSurfaceView, 0);//InputMethodManager.SHOW_FORCED);
								}
								else
								{
									//								getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
									im.hideSoftInputFromWindow(mGLSurfaceView.getWindowToken(), 0);
								}
							}
							else
								android.util.Log.i("FTEDroid", "IMM failed");
						}
					};
					act.runOnUiThread(r);
				}
				notifiedflags = flags;
			}



			framenum ++;

			if (enable_vibrator)
				vibration = Quake2Lib.Quake2GetVibration();


			long tafter = SystemClock.uptimeMillis(); 



			if (vibration_running
					&& (tafter -vibration_end)> 0)
				vibration_running = false;    

			if (!vibration_running
					&& vibration == 1 
					&& vibration_duration > 0 ){
				// Start the vibration
				vibrator.vibrate(vibration_duration);
				vibration_running = true;
				vibration_end = tafter + vibration_duration;
			}



			// speed limit : 10 FPS
			// probably a bad idea, because Android will try to run 
			// other processes in the background if we go to sleep ..

			if (speed_limit>0){
				long tsleep =  speed_limit - (tafter - tnow);
				if ( tsleep > 0 )
					SystemClock.sleep( tsleep );
			}

		}




		public void onSurfaceChanged(GL10 gl, int width, int height) {
			Log.d("Renderer", String.format("onSurfaceChanged %dx%d", width,height) );
			controlInterp.setScreenSize(width, height);
			gl.glViewport(0, 0, width, height);
			init( width, height );
		}


	} // end of QuakeRenderer



	private static Object quake2Lock = new Object();



	private static int sQuake2Frame(){
		int ret;
		synchronized(quake2Lock) { 	
			ret = Quake2Lib.Quake2Frame();
		}
		return ret;
	}

	private static int sQuake2PaintAudio( ByteBuffer buf ){
		int ret;
		synchronized(quake2Lock) { 	
			ret = Quake2Lib.Quake2PaintAudio(buf);
		}
		return ret;
	}

	/*----------------------------
	 * Audio
	 *----------------------------*/

	public void audio_thread() throws IOException{


		int audioSize = (2048*4); 

		ByteBuffer audioBuffer = ByteBuffer.allocateDirect(audioSize);

		byte[] audioData = new byte[audioSize];

		AudioTrack oTrack = new AudioTrack(AudioManager.STREAM_MUSIC, 22050, //44100,
				AudioFormat.CHANNEL_CONFIGURATION_STEREO,
				AudioFormat.ENCODING_PCM_16BIT, 
				2*(22050/5), // 100 millisecond buffer
				// => impact on audio latency
				AudioTrack.MODE_STREAM);

		Log.i("Quake2", "start audio");

		// Start playing data that is written
		oTrack.play();

		long tstart = SystemClock.uptimeMillis();

		while (!please_exit){

			if (doThreadLock)
				synchronized (threadLock) {
					try {
						if (oTrack != null)
							oTrack.pause();
						threadLock.wait();
						if (oTrack != null)
							oTrack.play();
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			
			long tnow = SystemClock.uptimeMillis() ;

			// timelimit
			if ( timelimit!=0 && (tnow-tstart) > timelimit)
				break;


			sQuake2PaintAudio( audioBuffer );          	    


			audioBuffer.position(0);
			audioBuffer.get(audioData);

			// Write the byte array to the track
			oTrack.write(audioData, 0, audioData.length);    
		}

		Log.i("Quake2", "stop audio");

		// Done writting to the track
		oTrack.stop();	    

	}

}


