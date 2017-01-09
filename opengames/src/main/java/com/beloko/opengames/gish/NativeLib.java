package com.beloko.opengames.gish;

import android.util.Log;
import android.view.KeyEvent;

import com.beloko.libsdl.SDLLib;
import com.beloko.opengames.MyGLSurfaceView;
import com.beloko.touchcontrols.ControlInterface;

public class NativeLib implements ControlInterface {


	public static void loadLibraries(boolean setup)
	{

		try {
			Log.i("JNI", "Trying to load libraries");

			SDLLib.loadSDL();
			System.loadLibrary("touchcontrols");

			//System.loadLibrary("openal");
			//System.loadLibrary("vorbis");
			//System.loadLibrary("ogg");
			//System.loadLibrary("gish");
			System.loadLibrary("abuse");
		}

		catch (UnsatisfiedLinkError ule) {
			Log.e("JNI", "WARNING: Could not load shared library: " + ule.toString());
		}

	}

	public static native int init(String graphics_dir,int mem,String[] args,int game,String path);

	public static native void setScreenSize( int width, int height );

	public static native int frame();

	public static native boolean touchEvent( int action, int pid, float x, float y);
	public static native void keypress(int down, int qkey, int unicode);
	public static native void doAction(int state, int action);
	public static native void analogFwd(float v);
	public static native void analogSide(float v);
	public static native void analogPitch(int mode,float v);
	public static native void analogYaw(int mode,float v);
	public static native void setTouchSettings(float alpha,float strafe,float fwd,float pitch,float yaw,int other);

	public static native void quickCommand(String command);

	@Override
	public void quickCommand_if(String command)
	{
		quickCommand(command);
	}

	@Override
	public void initTouchControls_if(String pngPath, int width, int height) {

	}

	@Override
	public boolean touchEvent_if(int action, int pid, float x, float y) {
		return touchEvent(  action,  pid,  x,  y);
	}
	@Override
	public void keyPress_if(int down, int qkey, int unicode) {
		keypress(down,qkey,unicode);

	}
	@Override
	public void doAction_if(int state, int action) {
		doAction(state,action);
	} 

	@Override
	public void analogFwd_if(float v) {
		analogFwd(v);
	}
	@Override
	public void analogSide_if(float v) {
		analogSide(v);
	}
	@Override
	public void  analogPitch_if(int mode,float v)
	{
		analogPitch(mode,v);
	}
	@Override
	public void  analogYaw_if(int mode,float v)
	{
		analogYaw(mode,v);
	}

	@Override
	public void setTouchSettings_if(float alpha,float strafe, float fwd, float pitch,
			float yaw, int other) {
		setTouchSettings(alpha,strafe, fwd, pitch, yaw, other);

	}

	public static final int KEY_PRESS = 1;
	public static final int KEY_RELEASE = 0;



		 public static final int SCAN_ESC = 27;
		 public static final int SCAN_0 = 48;
		 public static final int SCAN_1 = 49;
		 public static final int SCAN_2 = 50;
		 public static final int SCAN_3 = 51;
		 public static final int SCAN_4 = 52;
		 public static final int SCAN_5 = 53;
		 public static final int SCAN_6 = 54;
		 public static final int SCAN_7 = 55;
		 public static final int SCAN_8 = 56;
		 public static final int SCAN_9 = 57;
		 public static final int SCAN_MINUS = 45;
		 public static final int SCAN_EQUALS = 61;
		 public static final int SCAN_BKSP = 8;
		 public static final int SCAN_TAB = 9;
		 public static final int SCAN_LFT_BRACKET = 91;
		 public static final int SCAN_RGT_BRACKET = 93;
		 public static final int SCAN_ENTER = 13;
		 public static final int SCAN_CTRL = 306;
		 public static final int SCAN_RCTRL = 305;
		 public static final int SCAN_SEMI = 59;
		 public static final int SCAN_APOS = 39;
		 public static final int SCAN_TILDE = 96;
		 public static final int SCAN_SHIFT = 304;
		 public static final int SCAN_BACK_SLASH = 92;
		 public static final int SCAN_COMMA = 44;
		 public static final int SCAN_PERIOD = 46;
		 public static final int SCAN_FOWARD_SLASH = 47;
		 public static final int SCAN_RSHIFT = 303;
		 public static final int SCAN_PRT_SCRN = 316;
		 public static final int SCAN_ALT = 308;
		 public static final int SCAN_SPACE = 32;
		 public static final int SCAN_CAPS_LOCK = 301;
		 public static final int SCAN_F1 = 282;
		 public static final int SCAN_F2 = 283;
		 public static final int SCAN_F3 = 284;
		 public static final int SCAN_F4 = 285;
		 public static final int SCAN_F5 = 286;
		 public static final int SCAN_F6 = 287;
		 public static final int SCAN_F7 = 288;
		 public static final int SCAN_F8 = 289;
		 public static final int SCAN_F9 = 290;
		 public static final int SCAN_F10 = 291;
		 public static final int SCAN_F11 = 292;
		 public static final int SCAN_F12 = 293;
		 public static final int SCAN_NUM_LOCK = 300;
		 public static final int SCAN_SCROLL_LOCK = 302;
		 public static final int SCAN_NUMHOME = 263;
		 public static final int SCAN_UP = 273;
		 public static final int SCAN_NUMPGUP = 265;
		 public static final int SCAN_NUM_MINUS = 269;
		 public static final int SCAN_LEFT = 276;
		 public static final int SCAN_CENTER = 261;
		 public static final int SCAN_RIGHT = 275;
		 public static final int SCAN_NUMPLUS = 270;
		 public static final int SCAN_NUMEND = 257;
		 public static final int SCAN_DOWN = 274;
		 public static final int SCAN_NUMPGDWN = 259;
		 public static final int SCAN_NUMINS = 256;
		 public static final int SCAN_NUMDEL = 266;
		 public static final int SCAN_LWIN = 311;
		 public static final int SCAN_RWIN = 312;
		 public static final int SCAN_APPS = 313;
		 public static final int SCAN_NUMENTER = 271;
		 public static final int SCAN_NUMLEFT = 260;
		 public static final int SCAN_NUMRIGHT = 262;
		 public static final int SCAN_NUMUP = 264;
		 public static final int SCAN_NUMDOWN = 258;
		 public static final int SCAN_LEFTARROW = 276;
		 public static final int SCAN_RIGHTARROW = 275;
		 public static final int SCAN_UPARROW = 273;
		 public static final int SCAN_DOWNARROW = 274;
		 public static final int SCAN_INSERT = 277;
		 public static final int SCAN_DELETE = 127;
		 public static final int SCAN_HOME = 278;
		 public static final int SCAN_END = 279;
		 public static final int SCAN_PAGEUP = 280;
		 public static final int SCAN_PAGEDOWN = 281;
		 public static final int SCAN_A = 97;
		 public static final int SCAN_B = 98;
		 public static final int SCAN_C = 99;
		 public static final int SCAN_D = 100;
		 public static final int SCAN_E = 101;
		 public static final int SCAN_F = 102;
		 public static final int SCAN_G = 103;
		 public static final int SCAN_H = 104;
		 public static final int SCAN_I = 105;
		 public static final int SCAN_J = 106;
		 public static final int SCAN_K = 107;
		 public static final int SCAN_L = 108;
		 public static final int SCAN_M = 109;
		 public static final int SCAN_N = 110;
		 public static final int SCAN_O = 111;
		 public static final int SCAN_P = 112;
		 public static final int SCAN_Q = 113;
		 public static final int SCAN_R = 114;
		 public static final int SCAN_S = 115;
		 public static final int SCAN_T = 116;
		 public static final int SCAN_U = 117;
		 public static final int SCAN_V = 118;
		 public static final int SCAN_W = 119;
		 public static final int SCAN_X = 120;
		 public static final int SCAN_Y = 121;
		 public static final int SCAN_Z = 122;



	public  int mapKey(int acode,  int unicode)
	{
		if ((acode >= KeyEvent.KEYCODE_A) && (acode <= KeyEvent.KEYCODE_Z))
		{
			return SCAN_A + (acode - KeyEvent.KEYCODE_A);
		}
		else
		{
			switch(acode)
			{
			case KeyEvent.KEYCODE_TAB:
				return SCAN_TAB;
			case KeyEvent.KEYCODE_DPAD_CENTER:
			case KeyEvent.KEYCODE_ENTER:
				return SCAN_ENTER;
			case KeyEvent.KEYCODE_ESCAPE:
			case KeyEvent.KEYCODE_BACK:
				return 41;
			case KeyEvent.KEYCODE_SPACE:
				return SCAN_SPACE;
			case KeyEvent.KEYCODE_DEL:
				return SCAN_BKSP;
			case KeyEvent.KEYCODE_DPAD_UP:
				return SCAN_UP;
			case KeyEvent.KEYCODE_DPAD_DOWN:
				return SCAN_DOWN;
			case KeyEvent.KEYCODE_DPAD_LEFT:
				return SCAN_LEFT;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
				return SCAN_RIGHT;
			case KeyEvent.KEYCODE_ALT_LEFT:
				return SCAN_A;
			case KeyEvent.KEYCODE_ALT_RIGHT:
				return SCAN_ALT;
			case KeyEvent.KEYCODE_CTRL_LEFT:
				return SCAN_CTRL;
			case KeyEvent.KEYCODE_CTRL_RIGHT:
				return SCAN_RCTRL;
			case KeyEvent.KEYCODE_SHIFT_LEFT:
				return SCAN_SHIFT;
			case KeyEvent.KEYCODE_SHIFT_RIGHT:
				return SCAN_RSHIFT;
			case KeyEvent.KEYCODE_F1:
				return SCAN_F1;
			case KeyEvent.KEYCODE_F2:
				return SCAN_F2;
			case KeyEvent.KEYCODE_F3:
				return SCAN_F3;
			case KeyEvent.KEYCODE_F4:
				return SCAN_F4;
			case KeyEvent.KEYCODE_F5:
				return SCAN_F5;
			case KeyEvent.KEYCODE_F6:
				return SCAN_F6;
			case KeyEvent.KEYCODE_F7:
				return SCAN_F7;
			case KeyEvent.KEYCODE_F8:
				return SCAN_F8;
			case KeyEvent.KEYCODE_F9:
				return SCAN_F9;
			case KeyEvent.KEYCODE_F10:
				return SCAN_F10;
			case KeyEvent.KEYCODE_F11:
				return SCAN_F11;
			case KeyEvent.KEYCODE_F12:
				return SCAN_F12;
			case KeyEvent.KEYCODE_FORWARD_DEL:
				return SCAN_DELETE;
			case KeyEvent.KEYCODE_INSERT:
				return SCAN_INSERT;
			case KeyEvent.KEYCODE_PAGE_UP:
				return SCAN_PAGEUP;
			case KeyEvent.KEYCODE_PAGE_DOWN:
				return SCAN_PAGEDOWN;
			case KeyEvent.KEYCODE_MOVE_HOME:
				return SCAN_HOME;
			case KeyEvent.KEYCODE_MOVE_END:
				return SCAN_END;
			case KeyEvent.KEYCODE_BREAK:
				return SCAN_PRT_SCRN;
			case KeyEvent.KEYCODE_PERIOD:
				return SCAN_PERIOD;
			case KeyEvent.KEYCODE_COMMA:
				return SCAN_COMMA;
			case KeyEvent.KEYCODE_SLASH:
				return SCAN_BACK_SLASH;
			case KeyEvent.KEYCODE_0:
				return SCAN_0;
			case KeyEvent.KEYCODE_1:
				return SCAN_1;
			case KeyEvent.KEYCODE_2:
				return SCAN_2;
			case KeyEvent.KEYCODE_3:
				return SCAN_3;
			case KeyEvent.KEYCODE_4:
				return SCAN_4;
			case KeyEvent.KEYCODE_5:
				return SCAN_5;
			case KeyEvent.KEYCODE_6:
				return SCAN_6;
			case KeyEvent.KEYCODE_7:
				return SCAN_7;
			case KeyEvent.KEYCODE_8:
				return SCAN_8;
			case KeyEvent.KEYCODE_9:
				return SCAN_9;
			default:
				if (unicode < 128)
					return Character.toLowerCase(unicode);
			}
		}
		return 0;
	}

	public static MyGLSurfaceView gv;


	static void swapBuffers()
	{
		boolean canDraw = false;
		do
		{
			gv.swapBuffers();
			canDraw = gv.setupSurface();
		}while (!canDraw);
	}
}
