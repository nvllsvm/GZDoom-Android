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
package com.beloko.opengames.chocdoom;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;

import com.bda.controller.Controller;
import com.beloko.libsdl.SDLLib;
import com.beloko.opengames.AppSettings;
import com.beloko.opengames.BestEglChooser;
import com.beloko.opengames.CDAudioPlayer;
import com.beloko.opengames.GD;
import com.beloko.opengames.MyGLSurfaceView;
import com.beloko.opengames.Utils;
import com.beloko.touchcontrols.ControlInterpreter;
import com.beloko.touchcontrols.TouchSettings;
import com.beloko.touchcontrols.ShowKeyboard;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class Setup extends Activity
{
	static String LOG = "Setup";

	private ControlInterpreter controlInterp;


	Controller mogaController = null;

	static private String args;
	static private String gamePath;
	static private String game;

	private GameView mGLSurfaceView = null;
	private QuakeRenderer mRenderer = new QuakeRenderer();
	static Activity act;

	int surfaceWidth,surfaceHeight;

	//This is called by the setup C code
	static public void launchGame()
	{
		Intent intent = new Intent(act, Game.class);
		intent.setAction(Intent.ACTION_MAIN);
		intent.addCategory(Intent.CATEGORY_LAUNCHER);

		intent.putExtra("game_path",gamePath);    
		intent.putExtra("game",game);    
		
		intent.putExtra("args",args);
		intent.putExtra("setup_launch",true); 
		
		Log.d(LOG,"launchGame args = " + args);
		act.startActivity(intent);
	}
	
	
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);           

		act = this;

		game = getIntent().getStringExtra("game");
		AppSettings.setGame(GD.IDGame.valueOf(game));
		AppSettings.reloadSettings(getApplication());

		args = getIntent().getStringExtra("args");
		gamePath  = getIntent().getStringExtra("game_path");
		// fullscreen
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		// keep screen on 
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
				WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		start_quake2();   

	}



	public void start_quake2() {

		NativeLib.loadLibraries(true);

		NativeLib engine = new NativeLib();

		controlInterp = new ControlInterpreter(engine,Utils.getGameGamepadConfig(AppSettings.game), TouchSettings.gamePadControlsFile, TouchSettings.gamePadEnabled);


		// Create our Preview view and set it as the content of our
		// Activity
		mGLSurfaceView = new GameView(this);

		NativeLib.gv = mGLSurfaceView;

		ShowKeyboard.setup(act, mGLSurfaceView);

		mGLSurfaceView.setEGLConfigChooser( new BestEglChooser(getApplicationContext()) );
		mGLSurfaceView.setRenderer(mRenderer);

		// This will keep the screen on, while your view is visible. 
		mGLSurfaceView.setKeepScreenOn(true);

		setContentView(mGLSurfaceView);
		mGLSurfaceView.requestFocus();
		mGLSurfaceView.setFocusableInTouchMode(true);
	}




	@Override
	protected void onPause() {
		Log.i( LOG, "onPause" );
		SDLLib.onPause();
		mGLSurfaceView.onPause();
		super.onPause();
	}

	@Override
	protected void onResume() {
		Log.i( LOG, "onResume" );
		CDAudioPlayer.onResume();
		SDLLib.onResume();
		mGLSurfaceView.onResume();
		super.onResume();
	}




	@Override
	protected void onDestroy() {
		Log.i( "Quake2.java", "onDestroy" ); 
		super.onDestroy();
		System.exit(0);
	}


	class GameView extends MyGLSurfaceView {



		public GameView(Context context) {
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

	} 



	///////////// GLSurfaceView.Renderer implementation ///////////

	class QuakeRenderer implements MyGLSurfaceView.Renderer {



		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			Log.d(LOG, "onSurfaceCreated");
		}


		private void init( int width, int height ){

			Log.i( LOG, "screen size : " + width + "x"+ height);

			NativeLib.setScreenSize(width,height);

			Utils.copyPNGAssets(getApplicationContext(), AppSettings.graphicsDir);

			Log.i("Quake2", "Quake2Init start");

			//args = "";

			String[] args_array = Utils.creatArgs(args);

			int ret = NativeLib.init(AppSettings.graphicsDir,64,args_array,0,gamePath);

			Log.i("Quake2", "Quake2Init done");
		}



		//// new Renderer interface

		boolean inited = false;

		public void onDrawFrame(GL10 gl) {
			//Does not return
			init( surfaceWidth, surfaceHeight );

		}




		public void onSurfaceChanged(GL10 gl, int width, int height) {


			Log.d("Renderer", String.format("onSurfaceChanged %dx%d", width,height) );

			SDLLib.nativeInit(false);

			SDLLib.surfaceChanged(PixelFormat.RGBA_8888, width, height);
			
			controlInterp.setScreenSize(width, height);

			surfaceWidth = width;
			surfaceHeight = height;
		}
	}

}


