

extern int main_android (int argc, char **argv);



#include "in_android.h"

#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "SDL.h"
#include "SDL_keycode.h"

#include "templates.h"
#include "version.h"
#include "doomdef.h"
#include "doomstat.h"
#include "d_protocol.h"
#include "d_netinf.h"
#include "intermission/intermission.h"
#include "m_argv.h"
#include "m_misc.h"
#include "menu/menu.h"
#include "m_random.h"
#include "m_crc32.h"
#include "i_system.h"
#include "i_input.h"
#include "p_saveg.h"
#include "p_tick.h"
#include "d_main.h"
#include "wi_stuff.h"
#include "hu_stuff.h"
#include "st_stuff.h"
#include "am_map.h"
#include "c_console.h"
#include "c_cvars.h"
#include "c_bind.h"
#include "c_dispatch.h"
#include "v_video.h"
#include "w_wad.h"
#include "p_local.h"
#include "s_sound.h"
#include "gstrings.h"
#include "r_sky.h"
#include "g_game.h"
#include "g_level.h"
#include "b_bot.h"			//Added by MC:
#include "sbar.h"
#include "m_swap.h"
#include "m_png.h"
#include "gi.h"
#include "a_keys.h"
#include "a_artifacts.h"
#include "r_data/r_translate.h"
#include "cmdlib.h"
#include "d_net.h"
#include "d_event.h"
#include "p_acs.h"
#include "m_joy.h"
#include "farchive.h"
#include "r_renderer.h"
#include "r_data/colormaps.h"

#include <android/log.h>
//#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"JNI", __VA_ARGS__))
//#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "JNI", __VA_ARGS__))
//#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"JNI", __VA_ARGS__))


/*
// FIFO STUFF ////////////////////
// Copied from FTEQW, I don't know if this is thread safe, but it's safe enough for a game :)
#define EVENTQUEUELENGTH 128

//This event_t is from choc Doom d_event.h
event_t  eventlist[EVENTQUEUELENGTH];

volatile int events_avail;
volatile int events_used;

static  event_t *in_newevent(void)
{
	if (events_avail >= events_used + EVENTQUEUELENGTH)
		return 0;
	return &eventlist[events_avail & (EVENTQUEUELENGTH-1)];
}

static void in_finishevent(void)
{
	events_avail++;
}

int add_choc_event(evtype_t type, int data1, int data2, int data3){

	//LOGI("PortableKeyEvent %d %d",state,code);
	event_t *ev = in_newevent();
	if (!ev)
		return 0;

	ev->type = type;
	ev->data1 = data1;
	ev->data2 = data2;
	ev->data3 = data3;

	in_finishevent();
	return 0;
}
///////////////////////

 */
extern "C"
{
extern int SDL_SendKeyboardKey(Uint8 state, SDL_Scancode scancode);
}

int PortableKeyEvent(int state, int code, int unicode){
	LOGI("PortableKeyEvent %d %d %d",state,code,unicode);

	if (state)
		SDL_SendKeyboardKey(SDL_PRESSED, (SDL_Scancode)code);
	else
		SDL_SendKeyboardKey(SDL_RELEASED, (SDL_Scancode) code);

	return 0;

}

void buttonChange(int state,FButtonStatus *button)
{
	if (state)
	{
		button->bDown = true;
		button->bWentDown  = true;
	}
	else
	{
		button->bWentUp = true;
		button->bDown = false;
	}
}

void PortableAction(int state, int action)
{
	LOGI("PortableAction %d   %d",state,action);

	if (PortableGetScreenMode() == TS_MENU)
	{
		if (action >= PORT_ACT_MENU_UP && action <= PORT_ACT_MENU_BACK)
		{

			int sdl_code [] = { SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT,
					SDL_SCANCODE_RIGHT, SDL_SCANCODE_RETURN, SDL_SCANCODE_ESCAPE };
			PortableKeyEvent(state, sdl_code[action-PORT_ACT_MENU_UP], 0);
			return;

		}
	}
	else
	{

		if ((action >= PORT_ACT_CUSTOM_0) && (action <= PORT_ACT_CUSTOM_7))
		{
			PortableKeyEvent(state, SDL_SCANCODE_A + action - PORT_ACT_CUSTOM_0, 0);
		}
		else
		{
			switch (action)
			{
			case PORT_ACT_LEFT:
				buttonChange(state,&Button_Left);
				break;
			case PORT_ACT_RIGHT:
				buttonChange(state,&Button_Right);
				break;
			case PORT_ACT_FWD:
				buttonChange(state,&Button_Forward);
				break;
			case PORT_ACT_BACK:
				buttonChange(state,&Button_Back);
				break;
			case PORT_ACT_MOVE_LEFT:
				buttonChange(state,&Button_MoveLeft);
				break;
			case PORT_ACT_MOVE_RIGHT:
				buttonChange(state,&Button_MoveRight);
				break;
			case PORT_ACT_USE:
				buttonChange(state,&Button_Use);
				break;
			case PORT_ACT_ATTACK:
				buttonChange(state,&Button_Attack);
				break;
			case PORT_ACT_ALT_ATTACK:
				buttonChange(state,&Button_AltAttack);
				break;
			case PORT_ACT_JUMP:
				buttonChange(state,&Button_Jump);
				break;
			case PORT_ACT_DOWN:
				buttonChange(state,&Button_Crouch);
				break;
			case PORT_ACT_NEXT_WEP:
				if (state)
					PortableCommand("weapnext");
				break;
			case PORT_ACT_PREV_WEP:
				if (state)
					PortableCommand("weapprev");
				break;
			case PORT_ACT_MAP:
				if (state)
					PortableCommand("togglemap");
				break;
			case PORT_ACT_QUICKLOAD:
				if (state)
					PortableCommand("quickload");
				break;
			case PORT_ACT_QUICKSAVE:
				if (state)
					PortableCommand("quicksave");
				break;
			case PORT_ACT_WEAP0:
				if (state)
					PortableCommand("slot 0");
				break;
			case PORT_ACT_WEAP1:
				if (state)
					PortableCommand("slot 1");
				break;
			case PORT_ACT_WEAP2:
				if (state)
					PortableCommand("slot 2");
				break;
			case PORT_ACT_WEAP3:
				if (state)
					PortableCommand("slot 3");
				break;
			case PORT_ACT_WEAP4:
				if (state)
					PortableCommand("slot 4");
				break;
			case PORT_ACT_WEAP5:
				if (state)
					PortableCommand("slot 5");
				break;
			case PORT_ACT_WEAP6:
				if (state)
					PortableCommand("slot 6");
				break;
			case PORT_ACT_WEAP7:
				if (state)
					PortableCommand("slot 7");
				break;
			case PORT_ACT_WEAP8:
				if (state)
					PortableCommand("slot 8");
				break;
			case PORT_ACT_WEAP9:
				if (state)
					PortableCommand("slot 9");
				break;
			}
		}
	}
}

int mdx=0,mdy=0;
void PortableMouse(float dx,float dy)
{
	dx *= 1500;
	dy *= 1200;

	mdx += dx;
	mdy += dy;
}

int absx=0,absy=0;
void PortableMouseAbs(float x,float y)
{
	absx = x;
	absy = y;
}


// =================== FORWARD and SIDE MOVMENT ==============

float forwardmove_android, sidemove_android; //Joystick mode

void PortableMoveFwd(float fwd)
{
	if (fwd > 1)
		fwd = 1;
	else if (fwd < -1)
		fwd = -1;

	forwardmove_android = fwd;
}

void PortableMoveSide(float strafe)
{
	if (strafe > 1)
		strafe = 1;
	else if (strafe < -1)
		strafe = -1;

	sidemove_android = strafe;
}

void PortableMove(float fwd, float strafe)
{
	PortableMoveFwd(fwd);
	PortableMoveSide(strafe);
}

//======================================================================

//Look up and down
int look_pitch_mode;
float look_pitch_mouse,look_pitch_abs,look_pitch_joy;
void PortableLookPitch(int mode, float pitch)
{
	look_pitch_mode = mode;
	switch(mode)
	{
	case LOOK_MODE_MOUSE:
		look_pitch_mouse += pitch;
		break;
	case LOOK_MODE_ABSOLUTE:
		look_pitch_abs = pitch;
		break;
	case LOOK_MODE_JOYSTICK:
		look_pitch_joy = pitch;
		break;
	}
}

//left right
int look_yaw_mode;
float look_yaw_mouse,look_yaw_joy;
void PortableLookYaw(int mode, float yaw)
{
	look_yaw_mode = mode;
	switch(mode)
	{
	case LOOK_MODE_MOUSE:
		look_yaw_mouse += yaw;
		break;
	case LOOK_MODE_JOYSTICK:
		look_yaw_joy = yaw;
		break;
	}
}




void PortableInit(int argc,const char ** argv){
	main_android(argc,(char **)argv);
}

extern bool		automapactive;
bool bindingbutton = false;

touchscreemode_t PortableGetScreenMode()
{

	if (menuactive != MENU_Off)
	{
		if (bindingbutton)
			return TS_CUSTOM;
		else
			return TS_MENU;
	}
	else if (gamestate == GS_LEVEL) // In a game
	{
		if (automapactive)
			return TS_MAP;
		else
			return TS_GAME;
	}
	else
		return TS_BLANK;

}


int PortableShowKeyboard(void){

	return 0;
}

const char *cmd_to_run = NULL;
void PortableCommand(const char * cmd)
{
	cmd_to_run = cmd;
}

float am_zoom = 0, am_pan_x = 0,am_pan_y = 0;
void PortableAutomapControl(float zoom, float x, float y)
{
	am_zoom += zoom;
	am_pan_x += x;
	am_pan_y += y;
	LOGI("am_pan_x = %f",am_pan_x);
}


void Android_AM_controls(float *zoom, fixed_t *pan_x,fixed_t *pan_y)
{
	if (am_zoom)
	{
		if (am_zoom > 0)
			*zoom = 1 + am_zoom;
		if (am_zoom < 0)
			*zoom = -1 + am_zoom;

		am_zoom = 0;
	}

	*pan_x += (fixed_t)am_pan_x;
	*pan_y += -(fixed_t)am_pan_y;
	am_pan_x = am_pan_y = 0;
	//LOGI("zoom = %f",*zoom);
}

extern fixed_t			forwardmove[2], sidemove[2];
extern void G_AddViewAngle (int yaw);
extern void G_AddViewPitch (int look);
void AddCommandString (char *cmd, int keynum);
void Android_IN_Move(ticcmd_t* cmd )
{

	cmd->ucmd.forwardmove  += forwardmove_android * forwardmove[1];
	cmd->ucmd.sidemove  += sidemove_android   * sidemove[1];

	//LOGI("Side: %d   %d",(int)(sidemove_android  * sidemove[1]),(int)(-look_yaw_joy * 1000));

	switch(look_pitch_mode)
	{
	case LOOK_MODE_MOUSE:
		G_AddViewPitch(look_pitch_mouse * 30000);
		look_pitch_mouse = 0;
		break;
	case LOOK_MODE_JOYSTICK:
		G_AddViewPitch(-look_pitch_joy * 1000);
		break;
	}


	switch(look_yaw_mode)
	{
	case LOOK_MODE_MOUSE:
		G_AddViewAngle(-look_yaw_mouse * 50000);
		look_yaw_mouse = 0;
		break;
	case LOOK_MODE_JOYSTICK:
		G_AddViewAngle(-look_yaw_joy * 1000);
		break;
	}

	if (cmd_to_run)
	{
		AddCommandString(cmd_to_run,0);
		cmd_to_run = NULL;
	}
}



