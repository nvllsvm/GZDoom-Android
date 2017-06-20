package net.nullsum.doom;

import android.util.Log;
import android.view.KeyEvent;

import com.beloko.libsdl.SDLLib;
import net.nullsum.doom.MyGLSurfaceView;
import com.beloko.touchcontrols.ControlInterface;

public class NativeLib implements ControlInterface {

	public static void loadLibraries()
	{

		try {
			Log.i("JNI", "Trying to load libraries");

			SDLLib.loadSDL();
			System.loadLibrary("touchcontrols");
			
				//System.loadLibrary("fmodL");
			
			System.loadLibrary("fmod");
			System.loadLibrary("openal");
			//System.loadLibrary("output_sdl");
            System.loadLibrary("gzdoom");

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

	public static final int  SDL_SCANCODE_A = 4;
	public static final int  SDL_SCANCODE_B = 5;
	public static final int  SDL_SCANCODE_C = 6;
	public static final int  SDL_SCANCODE_D = 7;
	public static final int  SDL_SCANCODE_E = 8;
	public static final int  SDL_SCANCODE_F = 9;
	public static final int  SDL_SCANCODE_G = 10;
	public static final int  SDL_SCANCODE_H = 11;
	public static final int  SDL_SCANCODE_I = 12;
	public static final int  SDL_SCANCODE_J = 13;
	public static final int  SDL_SCANCODE_K = 14;
	public static final int  SDL_SCANCODE_L = 15;
	public static final int  SDL_SCANCODE_M = 16;
	public static final int  SDL_SCANCODE_N = 17;
	public static final int  SDL_SCANCODE_O = 18;
	public static final int  SDL_SCANCODE_P = 19;
	public static final int  SDL_SCANCODE_Q = 20;
	public static final int  SDL_SCANCODE_R = 21;
	public static final int  SDL_SCANCODE_S = 22;
	public static final int  SDL_SCANCODE_T = 23;
	public static final int  SDL_SCANCODE_U = 24;
	public static final int  SDL_SCANCODE_V = 25;
	public static final int  SDL_SCANCODE_W = 26;
	public static final int  SDL_SCANCODE_X = 27;
	public static final int  SDL_SCANCODE_Y = 28;
	public static final int  SDL_SCANCODE_Z = 29;

	public static final int  SDL_SCANCODE_1 = 30;
	public static final int  SDL_SCANCODE_2 = 31;
	public static final int  SDL_SCANCODE_3 = 32;
	public static final int  SDL_SCANCODE_4 = 33;
	public static final int  SDL_SCANCODE_5 = 34;
	public static final int  SDL_SCANCODE_6 = 35;
	public static final int  SDL_SCANCODE_7 = 36;
	public static final int  SDL_SCANCODE_8 = 37;
	public static final int  SDL_SCANCODE_9 = 38;
	public static final int  SDL_SCANCODE_0 = 39;

	public static final int  SDL_SCANCODE_RETURN = 40;
	public static final int  SDL_SCANCODE_ESCAPE = 41;
	public static final int  SDL_SCANCODE_BACKSPACE = 42;
	public static final int  SDL_SCANCODE_TAB = 43;
	public static final int  SDL_SCANCODE_SPACE = 44;

	public static final int  SDL_SCANCODE_MINUS = 45;
	public static final int  SDL_SCANCODE_EQUALS = 46;
	public static final int  SDL_SCANCODE_LEFTBRACKET = 47;
	public static final int  SDL_SCANCODE_RIGHTBRACKET = 48;

	public static final int  SDL_SCANCODE_COMMA = 54;
	public static final int  SDL_SCANCODE_PERIOD = 55;
	public static final int  SDL_SCANCODE_SLASH = 56;

	public static final int  SDL_SCANCODE_CAPSLOCK = 57;

	public static final int  SDL_SCANCODE_F1 = 58;
	public static final int  SDL_SCANCODE_F2 = 59;
	public static final int  SDL_SCANCODE_F3 = 60;
	public static final int  SDL_SCANCODE_F4 = 61;
	public static final int  SDL_SCANCODE_F5 = 62;
	public static final int  SDL_SCANCODE_F6 = 63;
	public static final int  SDL_SCANCODE_F7 = 64;
	public static final int  SDL_SCANCODE_F8 = 65;
	public static final int  SDL_SCANCODE_F9 = 66;
	public static final int  SDL_SCANCODE_F10 = 67;
	public static final int  SDL_SCANCODE_F11 = 68;
	public static final int  SDL_SCANCODE_F12 = 69;

	public static final int  SDL_SCANCODE_PRINTSCREEN = 70;
	public static final int  SDL_SCANCODE_SCROLLLOCK = 71;
	public static final int  SDL_SCANCODE_PAUSE = 72;
	public static final int  SDL_SCANCODE_INSERT = 73; 
	public static final int  SDL_SCANCODE_HOME = 74;
	public static final int  SDL_SCANCODE_PAGEUP = 75;
	public static final int  SDL_SCANCODE_DELETE = 76;
	public static final int  SDL_SCANCODE_END = 77;
	public static final int  SDL_SCANCODE_PAGEDOWN = 78;
	public static final int  SDL_SCANCODE_RIGHT = 79;
	public static final int  SDL_SCANCODE_LEFT = 80;
	public static final int  SDL_SCANCODE_DOWN = 81;
	public static final int  SDL_SCANCODE_UP = 82;

	public static final int  SDL_SCANCODE_POWER = 102;// Hack this is overridden to give an underscore instead

	public static final int  SDL_SCANCODE_LCTRL = 224;
	public static final int  SDL_SCANCODE_LSHIFT = 225;
	public static final int  SDL_SCANCODE_LALT = 226; /**< alt; option */
	public static final int  SDL_SCANCODE_LGUI = 227; /**< windows; command (apple); meta */
	public static final int  SDL_SCANCODE_RCTRL = 228;
	public static final int  SDL_SCANCODE_RSHIFT = 229;
	public static final int  SDL_SCANCODE_RALT = 230; /**< alt gr; option */
	public  int mapKey(int acode,  int unicode)
	{
		Log.d("TEST","key = " + acode + " " + unicode);
		if (unicode == 95) //Hack to make underscore work
		{
			return SDL_SCANCODE_POWER;
		}
		else if ((acode >= KeyEvent.KEYCODE_A) && (acode <= KeyEvent.KEYCODE_Z))
		{
			return SDL_SCANCODE_A + (acode - KeyEvent.KEYCODE_A);
		}
		else
		{
			switch(acode)
			{
			case KeyEvent.KEYCODE_TAB:
				return SDL_SCANCODE_TAB;
			case KeyEvent.KEYCODE_DPAD_CENTER:
			case KeyEvent.KEYCODE_ENTER:
				return SDL_SCANCODE_RETURN;
			case KeyEvent.KEYCODE_ESCAPE:
			case KeyEvent.KEYCODE_BACK:
				return SDL_SCANCODE_ESCAPE;
			case KeyEvent.KEYCODE_SPACE:
				return SDL_SCANCODE_SPACE;
			case KeyEvent.KEYCODE_DEL:
				return SDL_SCANCODE_BACKSPACE;
			case KeyEvent.KEYCODE_DPAD_UP:
				return SDL_SCANCODE_UP;
			case KeyEvent.KEYCODE_DPAD_DOWN:
				return SDL_SCANCODE_DOWN;
			case KeyEvent.KEYCODE_DPAD_LEFT:
				return SDL_SCANCODE_LEFT;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
				return SDL_SCANCODE_RIGHT;
			case KeyEvent.KEYCODE_ALT_LEFT:
				return SDL_SCANCODE_A;
			case KeyEvent.KEYCODE_ALT_RIGHT:
				return SDL_SCANCODE_RALT;
			case KeyEvent.KEYCODE_CTRL_LEFT:
				return SDL_SCANCODE_LCTRL;
			case KeyEvent.KEYCODE_CTRL_RIGHT:
				return SDL_SCANCODE_RCTRL;
			case KeyEvent.KEYCODE_SHIFT_LEFT:
				return SDL_SCANCODE_LSHIFT;
			case KeyEvent.KEYCODE_SHIFT_RIGHT:
				return SDL_SCANCODE_RSHIFT;
			case KeyEvent.KEYCODE_F1:
				return SDL_SCANCODE_F1;
			case KeyEvent.KEYCODE_F2:
				return SDL_SCANCODE_F2;
			case KeyEvent.KEYCODE_F3:
				return SDL_SCANCODE_F3;
			case KeyEvent.KEYCODE_F4:
				return SDL_SCANCODE_F4;
			case KeyEvent.KEYCODE_F5:
				return SDL_SCANCODE_F5;
			case KeyEvent.KEYCODE_F6:
				return SDL_SCANCODE_F6;
			case KeyEvent.KEYCODE_F7:
				return SDL_SCANCODE_F7;
			case KeyEvent.KEYCODE_F8:
				return SDL_SCANCODE_F8;
			case KeyEvent.KEYCODE_F9:
				return SDL_SCANCODE_F9;
			case KeyEvent.KEYCODE_F10:
				return SDL_SCANCODE_F10;
			case KeyEvent.KEYCODE_F11:
				return SDL_SCANCODE_F11;
			case KeyEvent.KEYCODE_F12:
				return SDL_SCANCODE_F12;	
			case KeyEvent.KEYCODE_FORWARD_DEL:
				return SDL_SCANCODE_DELETE;
			case KeyEvent.KEYCODE_INSERT:
				return SDL_SCANCODE_INSERT;
			case KeyEvent.KEYCODE_PAGE_UP:
				return SDL_SCANCODE_PAGEUP;
			case KeyEvent.KEYCODE_PAGE_DOWN:
				return SDL_SCANCODE_PAGEDOWN;
			case KeyEvent.KEYCODE_MOVE_HOME:
				return SDL_SCANCODE_HOME;
			case KeyEvent.KEYCODE_MOVE_END:
				return SDL_SCANCODE_END;
			case KeyEvent.KEYCODE_BREAK:
				return SDL_SCANCODE_PRINTSCREEN;
			case KeyEvent.KEYCODE_PERIOD:
				return SDL_SCANCODE_PERIOD;
			case KeyEvent.KEYCODE_COMMA:
				return SDL_SCANCODE_COMMA;
			case KeyEvent.KEYCODE_SLASH:
				return SDL_SCANCODE_SLASH;
			case KeyEvent.KEYCODE_0:
				return SDL_SCANCODE_0;
			case KeyEvent.KEYCODE_1:
				return SDL_SCANCODE_1;
			case KeyEvent.KEYCODE_2:
				return SDL_SCANCODE_2;
			case KeyEvent.KEYCODE_3:
				return SDL_SCANCODE_3;
			case KeyEvent.KEYCODE_4:
				return SDL_SCANCODE_4;
			case KeyEvent.KEYCODE_5:
				return SDL_SCANCODE_5;
			case KeyEvent.KEYCODE_6:
				return SDL_SCANCODE_6;
			case KeyEvent.KEYCODE_7:
				return SDL_SCANCODE_7;
			case KeyEvent.KEYCODE_8:
				return SDL_SCANCODE_8;
			case KeyEvent.KEYCODE_9:
				return SDL_SCANCODE_9;
			case KeyEvent.KEYCODE_MINUS:
				return SDL_SCANCODE_MINUS;
			case KeyEvent.KEYCODE_EQUALS:
				return SDL_SCANCODE_EQUALS;
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
