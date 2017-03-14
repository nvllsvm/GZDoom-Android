#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <unistd.h>


#include "SDL_android_extra.h"

#include "TouchControlsContainer.h"
#include "JNITouchControlsUtils.h"



extern "C"
{
#define DEFAULT_FADE_FRAMES 10

#include "../GL/gl.h"
#include "../GL/nano_gl.h"

#include "in_android.h"
#include "SDL_keycode.h"

#define JAVA_FUNC(x) Java_net_nullsum_doom_NativeLib_##x

int android_screen_width;
int android_screen_height;


touchscreemode_t currentScreenMode = TS_BLANK;

#define KEY_SHOW_WEAPONS 0x1000
#define KEY_SHOOT        0x1001

#define KEY_SHOW_INV     0x1006
#define KEY_QUICK_CMD    0x1007
#define KEY_SHOW_KBRD    0x1008
#define KEY_SHOW_CUSTOM  0x1009



float gameControlsAlpha = 0.5;
bool turnMouseMode = true;
bool invertLook = false;
bool precisionShoot = false;
bool showSticks = true;
bool hideTouchControls = true;
bool enableWeaponWheel = true;

bool shooting = false;

static int controlsCreated = 0;
touchcontrols::TouchControlsContainer controlsContainer;

touchcontrols::TouchControls *tcMenuMain=0;
touchcontrols::TouchControls *tcGameMain=0;
touchcontrols::TouchControls *tcGameWeapons=0;
touchcontrols::TouchControls *tcWeaponWheel=0;
touchcontrols::TouchControls *tcInventory=0;
touchcontrols::TouchControls *tcAutomap=0;
touchcontrols::TouchControls *tcBlank=0;
touchcontrols::TouchControls *tcCutomButtons=0;



//So can hide and show these buttons
touchcontrols::TouchJoy *touchJoyLeft;
touchcontrols::TouchJoy *touchJoyRight;

extern JNIEnv* env_;

GLint     matrixMode;
GLfloat   projection[16];
GLfloat   model[16];

void openGLStart()
{
	nanoPushState();

	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	glGetFloatv(GL_MODELVIEW_MATRIX, model);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, android_screen_width, android_screen_height);
	glOrthof(0.0f, android_screen_width, android_screen_height, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_DEPTH_TEST);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY );


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
}

void openGLEnd()
{
	nanoPopState();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(model);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projection);


	if (matrixMode == GL_MODELVIEW)
	{
		glMatrixMode(GL_MODELVIEW);
	}
	else if (matrixMode == GL_TEXTURE)
	{
		glMatrixMode(GL_TEXTURE);
	}
}

void gameSettingsButton(int state)
{
	if (state == 1)
	{
		showTouchSettings();
	}
}

void customSettingsButton(int state)
{
	if (state == 1)
	{
		showEditButtons();
	}
}


static jclass NativeLibClass = 0;
static jmethodID swapBuffersMethod = 0;
void swapBuffers()
{
	if (NativeLibClass == 0)
	{
		NativeLibClass = env_->FindClass("net/nullsum/doom/NativeLib");
		swapBuffersMethod = env_->GetStaticMethodID(NativeLibClass, "swapBuffers", "()V");
	}
	env_->CallStaticVoidMethod(NativeLibClass, swapBuffersMethod);
}

extern unsigned int Sys_Milliseconds(void);

static unsigned int reload_time_down;
void gameButton(int state,int code)
{

	if (code == KEY_SHOOT)
	{
		shooting = state;
		PortableAction(state,PORT_ACT_ATTACK);
	}
	else if (code == KEY_SHOW_WEAPONS)
	{
		if (state == 1)
			if (!tcGameWeapons->enabled)
			{

				tcGameWeapons->animateIn(5);
			}
	}
	else if (code == KEY_SHOW_INV)
	{
		if (state == 1)
		{
			if (!tcInventory->enabled)
			{
				tcInventory->animateIn(5);
			}
			else
				tcInventory->animateOut(5);
		}
	}
	else if (code == KEY_SHOW_CUSTOM)
	{
		if (state == 1)
		{
			if (!tcCutomButtons->enabled)
			{
				tcCutomButtons->setEnabled(true);
				tcCutomButtons->setAlpha(gameControlsAlpha);
				tcCutomButtons->fade(touchcontrols::FADE_IN,5);
			}
			else
				tcCutomButtons->fade(touchcontrols::FADE_OUT,5);
		}
	}
	else if  (code == KEY_SHOW_KBRD)
	{
		if (state)
			showKeyboard(true);
	}
	else
	{
		PortableAction(state, code);
	}
}

void blankButton(int state,int code)
{
	PortableAction(state, PORT_ACT_USE);
	PortableKeyEvent(state, SDL_SCANCODE_RETURN, 0);
}

void customButton(int state,int code)
{
	PortableKeyEvent(state, code, 0);
}

//Weapon wheel callbacks
void weaponWheelSelected(int enabled)
{
	if (enabled)
		tcWeaponWheel->fade(touchcontrols::FADE_IN,5); //fade in
}

void weaponWheel(int segment)
{
	LOGI("weaponWheel %d",segment);
	PortableAction(1,PORT_ACT_WEAP1 + segment);
}

void menuButton(int state,int code)
{
	if (code == KEY_SHOW_KBRD)
	{
		if (state)
			showKeyboard(true);
	}
	else
		PortableAction(state, code);
}



int left_double_action;
int right_double_action;

void left_double_tap(int state)
{
	if (left_double_action)
		PortableAction(state,left_double_action);
}

void right_double_tap(int state)
{
	if (right_double_action)
		PortableAction(state,right_double_action);
}



//To be set by android
float strafe_sens,forward_sens;
float pitch_sens,yaw_sens;

void left_stick(float joy_x, float joy_y,float mouse_x, float mouse_y)
{
	joy_x *=10;
	float strafe = joy_x;

	PortableMove(joy_y * 15 * forward_sens,-strafe * strafe_sens);
}
void right_stick(float joy_x, float joy_y,float mouse_x, float mouse_y)
{
	int invert = invertLook?-1:1;

	float scale;

	scale = (shooting && precisionShoot)?0.3:1;

	PortableLookPitch(LOOK_MODE_MOUSE,mouse_y  * pitch_sens * invert * scale);

	if (turnMouseMode)
		PortableLookYaw(LOOK_MODE_MOUSE,mouse_x*2*yaw_sens * scale);
	else
		PortableLookYaw(LOOK_MODE_JOYSTICK,joy_x*6*yaw_sens * scale);

}

void inventoryButton(int state,int code)
{
	PortableAction(state,code);
}

//Weapon select callbacks
void selectWeaponButton(int state, int code)
{
	PortableKeyEvent(state, code, 0);
	if (state == 0)
		tcGameWeapons->animateOut(5);
}

void automap_multitouch_mouse_move(int action,float x, float y,float dx, float dy)
{

	if (action == MULTITOUCHMOUSE_MOVE)
	{
		PortableAutomapControl(0,dx*4000000,dy*4000000);
	}
	else if (action == MULTITOUCHMOUSE_ZOOM)
	{
		PortableAutomapControl(x*5,0,0);
	}
}


void setHideSticks(bool v)
{
	if (touchJoyLeft) touchJoyLeft->setHideGraphics(v);
	if (touchJoyRight) touchJoyRight->setHideGraphics(v);
}


void initControls(int width, int height,const char * graphics_path)
{
	touchcontrols::GLScaleWidth = (float)width;
	touchcontrols::GLScaleHeight = (float)height;

	LOGI("initControls %d x %d,x path = %s",width,height,graphics_path);

	if (!controlsCreated)
	{
		LOGI("creating controls");

		touchcontrols::setGraphicsBasePath(graphics_path);
		setControlsContainer(&controlsContainer);

		controlsContainer.openGL_start.connect( sigc::ptr_fun(&openGLStart));
		controlsContainer.openGL_end.connect( sigc::ptr_fun(&openGLEnd));


		tcMenuMain = new touchcontrols::TouchControls("menu",false,false);
		tcGameMain = new touchcontrols::TouchControls("game",false,true,1,true);
		tcGameWeapons = new touchcontrols::TouchControls("weapons",false,true,1,false);
		tcInventory  = new touchcontrols::TouchControls("inventory",false,true,1,false);
		tcWeaponWheel = new touchcontrols::TouchControls("weapon_wheel",false,true,1,false);
		tcAutomap = new touchcontrols::TouchControls("automap",false,false);
		tcBlank = new touchcontrols::TouchControls("blank",true,false);
		tcCutomButtons = new touchcontrols::TouchControls("custom_buttons",false,true,1,true);

		//Menu
		tcMenuMain->addControl(new touchcontrols::Button("down_arrow",touchcontrols::RectF(20,13,23,16),"arrow_down",PORT_ACT_MENU_DOWN));
		tcMenuMain->addControl(new touchcontrols::Button("up_arrow",touchcontrols::RectF(20,10,23,13),"arrow_up",PORT_ACT_MENU_UP));
		tcMenuMain->addControl(new touchcontrols::Button("left_arrow",touchcontrols::RectF(17,13,20,16),"arrow_left",PORT_ACT_MENU_LEFT));
		tcMenuMain->addControl(new touchcontrols::Button("right_arrow",touchcontrols::RectF(23,13,26,16),"arrow_right",PORT_ACT_MENU_RIGHT));
		tcMenuMain->addControl(new touchcontrols::Button("enter",touchcontrols::RectF(0,10,6,16),"enter",PORT_ACT_MENU_SELECT));
		tcMenuMain->addControl(new touchcontrols::Button("keyboard",touchcontrols::RectF(0,0,3,3),"keyboard",KEY_SHOW_KBRD));
		tcMenuMain->signal_button.connect(  sigc::ptr_fun(&menuButton) );
		tcMenuMain->setAlpha(0.8);


		//Game
		tcGameMain->setAlpha(gameControlsAlpha);
		tcGameMain->addControl(new touchcontrols::Button("attack",touchcontrols::RectF(20,7,23,10),"shoot",KEY_SHOOT));

		tcGameMain->addControl(new touchcontrols::Button("use",touchcontrols::RectF(23,6,26,9),"use",PORT_ACT_USE));
		tcGameMain->addControl(new touchcontrols::Button("quick_save",touchcontrols::RectF(24,0,26,2),"save",PORT_ACT_QUICKSAVE));
		tcGameMain->addControl(new touchcontrols::Button("quick_load",touchcontrols::RectF(20,0,22,2),"load",PORT_ACT_QUICKLOAD));
		tcGameMain->addControl(new touchcontrols::Button("map",touchcontrols::RectF(2,0,4,2),"map",PORT_ACT_MAP));
		tcGameMain->addControl(new touchcontrols::Button("keyboard",touchcontrols::RectF(8,0,10,2),"keyboard",KEY_SHOW_KBRD));


		tcGameMain->addControl(new touchcontrols::Button("jump",touchcontrols::RectF(24,3,26,5),"jump",PORT_ACT_JUMP,false,true));
		tcGameMain->addControl(new touchcontrols::Button("crouch",touchcontrols::RectF(24,14,26,16),"crouch",PORT_ACT_DOWN,false,true));
		tcGameMain->addControl(new touchcontrols::Button("attack_alt",touchcontrols::RectF(21,5,23,7),"shoot_alt",PORT_ACT_ALT_ATTACK,false,true));
		tcGameMain->addControl(new touchcontrols::Button("show_custom",touchcontrols::RectF(0,7,2,9),"Custom_Show",KEY_SHOW_CUSTOM,false,true));


		tcGameMain->addControl(new touchcontrols::Button("show_weapons",touchcontrols::RectF(12,14,14,16),"show_weapons",KEY_SHOW_WEAPONS));
		tcGameMain->addControl(new touchcontrols::Button("next_weapon",touchcontrols::RectF(0,3,3,5),"next_weap",PORT_ACT_NEXT_WEP));
		tcGameMain->addControl(new touchcontrols::Button("prev_weapon",touchcontrols::RectF(0,5,3,7),"prev_weap",PORT_ACT_PREV_WEP));


		touchJoyRight = new touchcontrols::TouchJoy("touch",touchcontrols::RectF(17,4,26,16),"look_arrow");
		tcGameMain->addControl(touchJoyRight);
		touchJoyRight->signal_move.connect(sigc::ptr_fun(&right_stick) );
		touchJoyRight->signal_double_tap.connect(sigc::ptr_fun(&right_double_tap) );

		touchJoyLeft = new touchcontrols::TouchJoy("stick",touchcontrols::RectF(0,7,8,16),"strafe_arrow");
		tcGameMain->addControl(touchJoyLeft);
		touchJoyLeft->signal_move.connect(sigc::ptr_fun(&left_stick) );
		touchJoyLeft->signal_double_tap.connect(sigc::ptr_fun(&left_double_tap) );

		tcGameMain->signal_button.connect(  sigc::ptr_fun(&gameButton) );
		tcGameMain->signal_settingsButton.connect(  sigc::ptr_fun(&gameSettingsButton) );


		//Weapons
		tcGameWeapons->addControl(new touchcontrols::Button("weapon1",touchcontrols::RectF(1,14,3,16),"key_1",SDL_SCANCODE_1));
		tcGameWeapons->addControl(new touchcontrols::Button("weapon2",touchcontrols::RectF(4,14,6,16),"key_2",SDL_SCANCODE_2));
		tcGameWeapons->addControl(new touchcontrols::Button("weapon3",touchcontrols::RectF(7,14,9,16),"key_3",SDL_SCANCODE_3));
		tcGameWeapons->addControl(new touchcontrols::Button("weapon4",touchcontrols::RectF(10,14,12,16),"key_4",SDL_SCANCODE_4));

		tcGameWeapons->addControl(new touchcontrols::Button("weapon5",touchcontrols::RectF(14,14,16,16),"key_5",SDL_SCANCODE_5));
		tcGameWeapons->addControl(new touchcontrols::Button("weapon6",touchcontrols::RectF(17,14,19,16),"key_6",SDL_SCANCODE_6));
		tcGameWeapons->addControl(new touchcontrols::Button("weapon7",touchcontrols::RectF(20,14,22,16),"key_7",SDL_SCANCODE_7));
		tcGameWeapons->addControl(new touchcontrols::Button("weapon8",touchcontrols::RectF(23,14,25,16),"key_8",SDL_SCANCODE_8));
		tcGameWeapons->signal_button.connect(  sigc::ptr_fun(&selectWeaponButton) );
		tcGameWeapons->setAlpha(0.8);

		//Weapon wheel
		touchcontrols::WheelSelect *wheel = new touchcontrols::WheelSelect("weapon_wheel",touchcontrols::RectF(7,2,19,14),"weapon_wheel",8);
		wheel->signal_selected.connect(sigc::ptr_fun(&weaponWheel) );
		wheel->signal_enabled.connect(sigc::ptr_fun(&weaponWheelSelected));
		tcWeaponWheel->addControl(wheel);
		tcWeaponWheel->setAlpha(0.8);
		tcInventory->signal_button.connect(  sigc::ptr_fun(&inventoryButton) );
		tcInventory->setAlpha(0.5);


		//Auto Map
		touchcontrols::MultitouchMouse *multimouse = new touchcontrols::MultitouchMouse("gamemouse",touchcontrols::RectF(0,0,26,16),"");
		multimouse->setHideGraphics(true);
		tcAutomap->addControl(multimouse);
		multimouse->signal_action.connect(sigc::ptr_fun(&automap_multitouch_mouse_move) );
		tcAutomap->addControl(new touchcontrols::Button("map",touchcontrols::RectF(2,0,4,2),"map",PORT_ACT_MAP));
		tcAutomap->signal_button.connect(  sigc::ptr_fun(&gameButton) );


		//Blank
		tcBlank->addControl(new touchcontrols::Button("enter",touchcontrols::RectF(0,0,26,16),"",0x123));
		tcBlank->signal_button.connect(  sigc::ptr_fun(&blankButton) );

		//Custom Controls
		tcCutomButtons->addControl(new touchcontrols::Button("A",touchcontrols::RectF(5,5,7,7),"Custom_1",SDL_SCANCODE_A,false,false));
		tcCutomButtons->addControl(new touchcontrols::Button("B",touchcontrols::RectF(7,5,9,7),"Custom_2",SDL_SCANCODE_B,false,false));
		tcCutomButtons->addControl(new touchcontrols::Button("C",touchcontrols::RectF(5,7,7,9),"Custom_3",SDL_SCANCODE_C,false,false));

		tcCutomButtons->addControl(new touchcontrols::Button("D",touchcontrols::RectF(7,7,9,9),"Custom_4",SDL_SCANCODE_D,false,false));
		tcCutomButtons->addControl(new touchcontrols::Button("E",touchcontrols::RectF(5,9,7,11),"Custom_5",SDL_SCANCODE_E,false,false));
		tcCutomButtons->addControl(new touchcontrols::Button("F",touchcontrols::RectF(7,9,9,11),"Custom_6",SDL_SCANCODE_F,false,false));
		tcCutomButtons->setColor(0.7,0.7,1.f);

		tcCutomButtons->signal_button.connect(sigc::ptr_fun(&customButton));
		tcCutomButtons->signal_settingsButton.connect(  sigc::ptr_fun(&customSettingsButton) );
		tcCutomButtons->setAlpha(0.8);


		controlsContainer.addControlGroup(tcGameMain);
		controlsContainer.addControlGroup(tcGameWeapons);
		controlsContainer.addControlGroup(tcCutomButtons);
		controlsContainer.addControlGroup(tcMenuMain);
		controlsContainer.addControlGroup(tcWeaponWheel);
		controlsContainer.addControlGroup(tcAutomap);
		controlsContainer.addControlGroup(tcBlank);
		controlsCreated = 1;

		tcGameMain->setXMLFile((std::string)graphics_path +  "/game_gz.xml");
		tcInventory->setXMLFile((std::string)graphics_path +  "/inventory_gz.xml");
		tcWeaponWheel->setXMLFile((std::string)graphics_path +  "/weaponwheel_gz.xml");
		tcGameWeapons->setXMLFile((std::string)graphics_path +  "/weapons_gz.xml");
		tcCutomButtons->setXMLFile((std::string)graphics_path +  "/custom_buttons_0_gz.xml");
	}
	else
		LOGI("NOT creating controls");

	controlsContainer.initGL();
}

void updateTouchScreenMode(touchscreemode_t mode)
{
	if (mode != currentScreenMode){

		//first disable the last screen and fade out is necessary
		switch(currentScreenMode){
		case TS_BLANK:
			tcBlank->resetOutput();
			tcBlank->setEnabled(false);
			break;
		case TS_MENU:
			tcMenuMain->resetOutput();
			tcMenuMain->fade(touchcontrols::FADE_OUT,DEFAULT_FADE_FRAMES);
			break;
		case TS_GAME:
			tcGameMain->resetOutput();
			tcGameMain->fade(touchcontrols::FADE_OUT,DEFAULT_FADE_FRAMES);
			tcGameWeapons->setEnabled(false);
			tcWeaponWheel->setEnabled(false);
			tcCutomButtons->resetOutput();
			tcCutomButtons->setEnabled(false);
			break;
		case TS_MAP:
			tcAutomap->resetOutput();
			tcAutomap->fade(touchcontrols::FADE_OUT,DEFAULT_FADE_FRAMES);
			break;
		case TS_CUSTOM:
			tcCutomButtons->resetOutput();
			tcCutomButtons->fade(touchcontrols::FADE_OUT,DEFAULT_FADE_FRAMES);
			break;
		}

		//Enable the current new screen
		switch(mode){
		case TS_BLANK:
			tcBlank->setEnabled(true);
			break;
		case TS_MENU:
			tcMenuMain->setEnabled(true);
			tcMenuMain->fade(touchcontrols::FADE_IN,DEFAULT_FADE_FRAMES);
			break;
		case TS_GAME:
			tcGameMain->setEnabled(true);
			tcGameMain->fade(touchcontrols::FADE_IN,DEFAULT_FADE_FRAMES);
			tcWeaponWheel->setEnabled(true);
			break;
		case TS_MAP:
			tcAutomap->setEnabled(true);
			tcAutomap->fade(touchcontrols::FADE_IN,DEFAULT_FADE_FRAMES);
			break;
		case TS_CUSTOM:
			tcCutomButtons->setEnabled(true);
			tcCutomButtons->setAlpha(1.0);
			tcCutomButtons->fade(touchcontrols::FADE_IN,DEFAULT_FADE_FRAMES);
			break;
		}

		currentScreenMode = mode;
	}

}
void frameControls()
{
	updateTouchScreenMode(PortableGetScreenMode());

	setHideSticks(!showSticks);
	controlsContainer.draw();

	swapBuffers();
}


void setTouchSettings(float alpha,float strafe,float fwd,float pitch,float yaw,int other)
{

	gameControlsAlpha = alpha;
	if (tcGameMain)
		tcGameMain->setAlpha(gameControlsAlpha);

	turnMouseMode   = other & 0x2?true:false;
	invertLook      = other & 0x4?true:false;
	precisionShoot  = other & 0x8?true:false;
	showSticks      = other & 0x1000?true:false;
	enableWeaponWheel  = other & 0x2000?true:false;

	if (tcWeaponWheel)
		tcWeaponWheel->setEnabled(enableWeaponWheel);


	hideTouchControls = other & 0x80000000?true:false;


	switch ((other>>4) & 0xF)
	{
	case 1:
		left_double_action = PORT_ACT_ATTACK;
		break;
	case 2:
		left_double_action = PORT_ACT_JUMP;
		break;
	default:
		left_double_action = 0;
	}

	switch ((other>>8) & 0xF)
	{
	case 1:
		right_double_action = PORT_ACT_ATTACK;
		break;
	case 2:
		right_double_action = PORT_ACT_JUMP;
		break;
	default:
		right_double_action = 0;
	}

	strafe_sens = strafe;
	forward_sens = fwd;
	pitch_sens = pitch;
	yaw_sens = yaw;

}

int quit_now = 0;

#define EXPORT_ME __attribute__ ((visibility("default")))

JNIEnv* env_;

int argc=1;
const char * argv[32];
std::string graphicpath;

const char * getFilesPath()
{
	return graphicpath.c_str(); //graphics path is the same as files path
}

std::string game_path;

const char * getGamePath()
{
	return game_path.c_str();
}

std::string home_env;


extern void Android_SetGameResolution(int width, int height);
int android_audio_rate;

jint EXPORT_ME
JAVA_FUNC(init) ( JNIEnv* env,	jobject thiz,jstring graphics_dir,jint audio_rate,jobjectArray argsArray,jint lowRes,jstring game_path_ )
{
	env_ = env;
	android_audio_rate = audio_rate;

	argv[0] = "quake";
	int argCount = (env)->GetArrayLength( argsArray);
	LOGI("argCount = %d",argCount);
	for (int i=0; i<argCount; i++) {
		jstring string = (jstring) (env)->GetObjectArrayElement( argsArray, i);
		argv[argc] = (char *)(env)->GetStringUTFChars( string, 0);
		LOGI("arg = %s",argv[argc]);
		argc++;
	}


	game_path = (char *)(env)->GetStringUTFChars( game_path_, 0);

	LOGI("game_path = %s",getGamePath());

	setenv("HOME", getGamePath(),1);

	putenv("TIMIDITY_CFG=../timidity.cfg");

	chdir(getGamePath());


	const char * p = env->GetStringUTFChars(graphics_dir,NULL);
	graphicpath =  std::string(p);



	initControls(android_screen_width,-android_screen_height,graphicpath.c_str());

	SDL_SetSwapBufferCallBack(frameControls);

	//Now done in java to keep context etc
	SDL_SwapBufferPerformsSwap(false);

	PortableInit(argc,argv); //Never returns!!

	return 0;
}


jint EXPORT_ME
JAVA_FUNC(frame) ( JNIEnv* env,	jobject thiz )
{
}

__attribute__((visibility("default"))) jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGI("JNI_OnLoad");
	setTCJNIEnv(vm);
	return JNI_VERSION_1_4;
}


void EXPORT_ME
JAVA_FUNC(keypress) (JNIEnv *env, jobject obj,jint down, jint keycode, jint unicode)
{
	LOGI("keypress %d",keycode);
	if (controlsContainer.isEditing())
	{
		if (down && (keycode == SDL_SCANCODE_ESCAPE ))
			controlsContainer.finishEditing();
		return;
	}
	PortableKeyEvent(down,keycode,unicode);

}


void EXPORT_ME
JAVA_FUNC(touchEvent) (JNIEnv *env, jobject obj,jint action, jint pid, jfloat x, jfloat y)
{
	controlsContainer.processPointer(action,pid,x,y);
}


void EXPORT_ME
JAVA_FUNC(doAction) (JNIEnv *env, jobject obj,	jint state, jint action)
{
	LOGI("doAction %d %d",state,action);
	if (hideTouchControls)
		if (tcGameMain)
		{
			if (tcGameMain->isEnabled())
				tcGameMain->animateOut(30);

			tcWeaponWheel->animateOut(1);
		}
	PortableAction(state,action);
}

void EXPORT_ME
JAVA_FUNC(analogFwd) (JNIEnv *env, jobject obj,	jfloat v)
{
	PortableMoveFwd(v);
}

void EXPORT_ME
JAVA_FUNC(analogSide) (JNIEnv *env, jobject obj,jfloat v)
{
	PortableMoveSide(v);
}

void EXPORT_ME
JAVA_FUNC(analogPitch) (JNIEnv *env, jobject obj,
		jint mode,jfloat v)
		{
	PortableLookPitch(mode, v);
		}

void EXPORT_ME
JAVA_FUNC(analogYaw) (JNIEnv *env, jobject obj,	jint mode,jfloat v)
{
	PortableLookYaw(mode, v);
}

void EXPORT_ME
JAVA_FUNC(setTouchSettings) (JNIEnv *env, jobject obj,	jfloat alpha,jfloat strafe,jfloat fwd,jfloat pitch,jfloat yaw,int other)
{
	setTouchSettings(alpha,strafe,fwd,pitch,yaw,other);
}

std::string quickCommandString;
jint EXPORT_ME
JAVA_FUNC(quickCommand) (JNIEnv *env, jobject obj,	jstring command)
{

	return 0;
}




void EXPORT_ME
JAVA_FUNC(setScreenSize) ( JNIEnv* env,	jobject thiz, jint width, jint height)
{
	android_screen_width = width;
	android_screen_height = height;
}


}
