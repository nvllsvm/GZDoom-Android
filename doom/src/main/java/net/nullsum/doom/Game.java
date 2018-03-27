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
package net.nullsum.doom;

import android.app.Activity;
import android.content.Context;
import android.graphics.PixelFormat;
import android.media.AudioTrack;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;

import com.bda.controller.Controller;
import com.bda.controller.ControllerListener;
import com.bda.controller.StateEvent;
import com.beloko.libsdl.SDLLib;
import com.beloko.touchcontrols.ControlInterpreter;
import com.beloko.touchcontrols.MogaHack;
import com.beloko.touchcontrols.ShowKeyboard;
import com.beloko.touchcontrols.TouchControlsEditing;
import com.beloko.touchcontrols.TouchControlsSettings;
import com.beloko.touchcontrols.TouchSettings;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class Game extends Activity implements Handler.Callback {
    private final MogaControllerListener mogaListener = new MogaControllerListener();
    private final String LOG = "Game";
    private final QuakeRenderer mRenderer = new QuakeRenderer();
    private Controller mogaController = null;
    private Activity act;
    private int surfaceWidth = -1;
    private int surfaceHeight;
    private int resDiv = 1;
    private ControlInterpreter controlInterp;
    private String args;
    private String gamePath;
    private GameView mGLSurfaceView = null;
    private Handler handlerUI;

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        act = this;

        handlerUI = new Handler(this);

        AppSettings.reloadSettings(getApplication());

        args = getIntent().getStringExtra("args");
        gamePath = getIntent().getStringExtra("game_path");
        resDiv = getIntent().getIntExtra("res_div", 1);

        mogaController = Controller.getInstance(this);
        MogaHack.init(mogaController, this);
        mogaController.setListener(mogaListener, new Handler());

        // fullscreen
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        // keep screen on
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        Utils.setImmersionMode(this);


        start_game();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        Utils.onWindowFocusChanged(this, hasFocus);
    }


    private void start_game() {

        NativeLib.loadLibraries();

        NativeLib engine = new NativeLib();


        controlInterp = new ControlInterpreter(engine, Utils.getGameGamepadConfig(), TouchSettings.gamePadControlsFile, TouchSettings.gamePadEnabled);

        TouchControlsSettings.setup(act, engine);
        TouchControlsSettings.loadSettings(act);
        TouchControlsSettings.sendToQuake();

        TouchControlsEditing.setup(act);

        mGLSurfaceView = new GameView(this);

        NativeLib.gv = mGLSurfaceView;

        ShowKeyboard.setup(act, mGLSurfaceView);

        mGLSurfaceView.setEGLConfigChooser(new BestEglChooser(getApplicationContext()));

        mGLSurfaceView.setRenderer(mRenderer);

        // This will keep the screen on, while your view is visible.
        mGLSurfaceView.setKeepScreenOn(true);

        setContentView(mGLSurfaceView);
        mGLSurfaceView.requestFocus();
        mGLSurfaceView.setFocusableInTouchMode(true);
    }


    @Override
    protected void onPause() {
        Log.i(LOG, "onPause");
        SDLLib.nativePause();
        SDLLib.onPause();
        mogaController.onPause();
        super.onPause();
    }

    @Override
    protected void onResume() {

        Log.i(LOG, "onResume");
        SDLLib.nativeResume();
        SDLLib.onResume();
        mogaController.onResume();
        super.onResume();
        mGLSurfaceView.onResume();
    }


    @Override
    protected void onDestroy() {
        Log.i(LOG, "onDestroy");
        super.onDestroy();
        mogaController.exit();
        System.exit(0);
    }

    @Override
    public boolean handleMessage(Message msg) {
        // TODO Auto-generated method stub
        return false;
    }

    class MogaControllerListener implements ControllerListener {


        @Override
        public void onKeyEvent(com.bda.controller.KeyEvent event) {
            //Log.d(LOG,"onKeyEvent " + event.getKeyCode());
            controlInterp.onMogaKeyEvent(event, mogaController.getState(Controller.STATE_CURRENT_PRODUCT_VERSION));
        }

        @Override
        public void onMotionEvent(com.bda.controller.MotionEvent event) {
            controlInterp.onGenericMotionEvent(event);
        }

        @Override
        public void onStateEvent(StateEvent event) {
            Log.d(LOG, "onStateEvent " + event.getState());
        }
    }


    ///////////// GLSurfaceView.Renderer implementation ///////////

    class GameView extends MyGLSurfaceView {

		/*--------------------
         * Event handling
		 *--------------------*/


        public GameView(Context context) {
            super(context);

        }

        @Override
        public boolean onGenericMotionEvent(MotionEvent event) {
            return controlInterp.onGenericMotionEvent(event);
        }

        @Override
        public boolean onTouchEvent(MotionEvent event) {
            return controlInterp.onTouchEvent(event);
        }

        @Override
        public boolean onKeyDown(int keyCode, KeyEvent event) {
            return controlInterp.onKeyDown(keyCode, event);
        }

        @Override
        public boolean onKeyUp(int keyCode, KeyEvent event) {
            return controlInterp.onKeyUp(keyCode, event);
        }

    }  // end of QuakeView

    class QuakeRenderer implements MyGLSurfaceView.Renderer {


        boolean divDone = false;
        //// new Renderer interface
        boolean SDLinited = false;

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            Log.d("Renderer", "onSurfaceCreated");
        }

        private void init(int width, int height) {

            Log.i(LOG, "screen size : " + width + "x" + height);

            NativeLib.setScreenSize(width, height);

            Utils.copyPNGAssets(getApplicationContext(), AppSettings.graphicsDir);

            Log.i(LOG, "Quake2Init start");

            //args = "-width 1280 -height 736 +set vid_renderer 1 -iwad tnt.wad -file brutal19.pk3 +set fluid_patchset /sdcard/WeedsGM3.sf2";
            //args = "+set vid_renderer 1 ";
            String gzdoom_args = "-width " + surfaceWidth / resDiv + " -height " + surfaceHeight / resDiv + " +set vid_renderer 1 ";
            String[] args_array = Utils.creatArgs(args + gzdoom_args);

            int audioSameple = AudioTrack.getNativeOutputSampleRate(AudioTrack.MODE_STREAM);
            Log.d(LOG, "audioSample = " + audioSameple);

            if ((audioSameple != 48000) && (audioSameple != 44100)) //Just in case
                audioSameple = 48000;

            NativeLib.init(AppSettings.graphicsDir, audioSameple, args_array, 0, gamePath);

            Log.i(LOG, "Quake2Init done");

        }

        public void onDrawFrame(GL10 gl) {

            Log.d("Renderer", "onDrawFrame");

            if (!divDone)
                handlerUI.post(new Runnable() {
                    @Override
                    public void run() {
                        mGLSurfaceView.getHolder().setFixedSize(surfaceWidth / resDiv, surfaceHeight / resDiv);
                        divDone = true;
                    }
                });

            if (divDone)
                init(surfaceWidth / resDiv, surfaceHeight / resDiv);
            else {
                try {
                    Thread.sleep(200);
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }

            Log.d("Renderer", "onDrawFrame END");

        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            Log.d("Renderer", String.format("onSurfaceChanged %dx%d", width, height));

            if (surfaceWidth == -1) {
                surfaceWidth = width;
                surfaceHeight = height;
            }

            if (!SDLinited) {
                SDLLib.nativeInit(false);
                SDLLib.surfaceChanged(PixelFormat.RGBA_8888, surfaceWidth / resDiv, surfaceHeight / resDiv);
                SDLinited = true;
            }

            //Display display = ((WindowManager) act.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
            //Point size = new Point();
            //display.getSize(size);
            controlInterp.setScreenSize(surfaceWidth, surfaceHeight);

            //controlInterp.setScreenSize(width, height);


        }
    } // end of QuakeRenderer
}


