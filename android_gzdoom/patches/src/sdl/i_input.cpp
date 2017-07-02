*** doom/src/main/jni/gzdoom/src/sdl/i_input.cpp	2017-07-01 18:06:20.415950933 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/sdl/i_input.cpp	2017-07-02 13:17:24.572082505 -0400
***************
*** 41,47 ****
  
  static BYTE KeySymToDIK[SDLK_LAST], DownState[SDLK_LAST];
  
! static WORD DIKToKeySym[256] =
  {
  	0, SDLK_ESCAPE, '1', '2', '3', '4', '5', '6',
  	'7', '8', '9', '0', '-', '=', SDLK_BACKSPACE, SDLK_TAB,
--- 41,47 ----
  
  static BYTE KeySymToDIK[SDLK_LAST], DownState[SDLK_LAST];
  
! static uint32_t DIKToKeySym[256*2] =
  {
  		0, SDLK_ESCAPE, '1', '2', '3', '4', '5', '6',
  		'7', '8', '9', '0', '-', '=', SDLK_BACKSPACE, SDLK_TAB,
***************
*** 83,98 ****
  
  static void InitKeySymMap ()
  {
  	for (int i = 0; i < 256; ++i)
  	{
! 		KeySymToDIK[DIKToKeySym[i]] = i;
  	}
  	KeySymToDIK[0] = 0;
  	KeySymToDIK[SDLK_RSHIFT] = DIK_LSHIFT;
  	KeySymToDIK[SDLK_RCTRL] = DIK_LCONTROL;
  	KeySymToDIK[SDLK_RALT] = DIK_LMENU;
  	// Depending on your Linux flavor, you may get SDLK_PRINT or SDLK_SYSREQ
  	KeySymToDIK[SDLK_PRINT] = DIK_SYSRQ;
  }
  
  static void I_CheckGUICapture ()
--- 83,115 ----
  
  static void InitKeySymMap ()
  {
+ 	LOGI("InitKeySymMap %d   %d",SDLK_LAST);
  	for (int i = 0; i < 256; ++i)
  	{
! 		//LOGI("InitKeySymMap %d",DIKToKeySym[i]);
! 		uint32_t key = DIKToKeySym[i];
! 		if (key & SDLK_SCANCODE_MASK) //if bit 30 is set, set bit 8 instead
! 		{
! 			key &= 0xFF;
! 			key |= 0x100;
! 		}
! 		KeySymToDIK[key] = i;
! 		//LOGI("InitKeySymMap %d",KeySymToDIK[97]);
! 	}
! 	for (int i = 0; i < 256; ++i)
! 	{
! 		//LOGI("KeySymToDIK[%d] =  %d",i,KeySymToDIK[i]);
  	}
+ 	LOGI("InitKeySymMap done %d %d %d %d",SDLK_RSHIFT,SDLK_RCTRL,SDLK_RALT,SDLK_PRINT);
  	KeySymToDIK[0] = 0;
+ 	/*
  	KeySymToDIK[SDLK_RSHIFT] = DIK_LSHIFT;
  	KeySymToDIK[SDLK_RCTRL] = DIK_LCONTROL;
  	KeySymToDIK[SDLK_RALT] = DIK_LMENU;
  	// Depending on your Linux flavor, you may get SDLK_PRINT or SDLK_SYSREQ
  	KeySymToDIK[SDLK_PRINT] = DIK_SYSRQ;
+ 
+ 	 */
  }
  
  static void I_CheckGUICapture ()
***************
*** 370,387 ****
  
  	case SDL_KEYDOWN:
  	case SDL_KEYUP:
! 		if (sev.key.keysym.sym >= SDLK_LAST)
  			break;
  
  		if (!GUICapture)
  		{
  			event.type = sev.type == SDL_KEYDOWN ? EV_KeyDown : EV_KeyUp;
! 			event.data1 = KeySymToDIK[sev.key.keysym.sym];
  			if (event.data1)
  			{
! 				if (sev.key.keysym.sym < 256)
  				{
! 					event.data2 = sev.key.keysym.sym;
  				}
  				D_PostEvent (&event);
  			}
--- 387,412 ----
  
  	case SDL_KEYDOWN:
  	case SDL_KEYUP:
! 		if ((sev.key.keysym.sym & 0x1FF) >= SDLK_LAST)
  			break;
  
  		if (!GUICapture)
  		{
  			event.type = sev.type == SDL_KEYDOWN ? EV_KeyDown : EV_KeyUp;
! 			uint32_t key = sev.key.keysym.sym;
! 			if (key & SDLK_SCANCODE_MASK) //if bit 30 is set, set bit 8 instead
! 			{
! 				key &= 0xFF;
! 				key |= 0x100;
! 			}
! 
! 			event.data1 = KeySymToDIK[key];
! 			LOGI("keysym = %d, DIK = %d",sev.key.keysym.sym ,event.data1);
  			if (event.data1)
  			{
! 				if ((sev.key.keysym.sym & 0x1FF) < 256)
  				{
! 					event.data2 = sev.key.keysym.sym & 0x1FF;
  				}
  				D_PostEvent (&event);
  			}
***************
*** 394,447 ****
  						  ((sev.key.keysym.mod & KMOD_CTRL) ? GKM_CTRL : 0) |
  						  ((sev.key.keysym.mod & KMOD_ALT) ? GKM_ALT : 0);
  
! 			if (sev.key.keysym.sym < SDLK_LAST)
  			{
  				if (event.subtype == EV_GUI_KeyDown)
  				{
! 					if (DownState[sev.key.keysym.sym])
  					{
  						event.subtype = EV_GUI_KeyRepeat;
  					}
! 					DownState[sev.key.keysym.sym] = 1;
  				}
  				else
  				{
! 					DownState[sev.key.keysym.sym] = 0;
  				}
  			}
  
! 			switch (sev.key.keysym.sym)
! 			{
! 			case SDLK_KP_ENTER:	event.data1 = GK_RETURN;	break;
! 			case SDLK_PAGEUP:	event.data1 = GK_PGUP;		break;
! 			case SDLK_PAGEDOWN:	event.data1 = GK_PGDN;		break;
! 			case SDLK_END:		event.data1 = GK_END;		break;
! 			case SDLK_HOME:		event.data1 = GK_HOME;		break;
! 			case SDLK_LEFT:		event.data1 = GK_LEFT;		break;
! 			case SDLK_RIGHT:	event.data1 = GK_RIGHT;		break;
! 			case SDLK_UP:		event.data1 = GK_UP;		break;
! 			case SDLK_DOWN:		event.data1 = GK_DOWN;		break;
! 			case SDLK_DELETE:	event.data1 = GK_DEL;		break;
! 			case SDLK_ESCAPE:	event.data1 = GK_ESCAPE;	break;
! 			case SDLK_F1:		event.data1 = GK_F1;		break;
! 			case SDLK_F2:		event.data1 = GK_F2;		break;
! 			case SDLK_F3:		event.data1 = GK_F3;		break;
! 			case SDLK_F4:		event.data1 = GK_F4;		break;
! 			case SDLK_F5:		event.data1 = GK_F5;		break;
! 			case SDLK_F6:		event.data1 = GK_F6;		break;
! 			case SDLK_F7:		event.data1 = GK_F7;		break;
! 			case SDLK_F8:		event.data1 = GK_F8;		break;
! 			case SDLK_F9:		event.data1 = GK_F9;		break;
! 			case SDLK_F10:		event.data1 = GK_F10;		break;
! 			case SDLK_F11:		event.data1 = GK_F11;		break;
! 			case SDLK_F12:		event.data1 = GK_F12;		break;
  			default:
! 				if (sev.key.keysym.sym < 256)
  				{
! 					event.data1 = sev.key.keysym.sym;
  				}
  				break;
  			}
  			event.data2 = sev.key.keysym.unicode & 0xff;
  			if (event.data1 < 128)
  			{
--- 419,475 ----
  					((sev.key.keysym.mod & KMOD_CTRL) ? GKM_CTRL : 0) |
  					((sev.key.keysym.mod & KMOD_ALT) ? GKM_ALT : 0);
  
! 			if ((sev.key.keysym.sym & 0x1FF) < SDLK_LAST)
  			{
  				if (event.subtype == EV_GUI_KeyDown)
  				{
! 					if (DownState[sev.key.keysym.sym & 0x1FF])
  					{
  						event.subtype = EV_GUI_KeyRepeat;
  					}
! 					DownState[sev.key.keysym.sym & 0x1FF] = 1;
  				}
  				else
  				{
! 					DownState[sev.key.keysym.sym & 0x1FF] = 0;
  				}
  			}
  
! 			switch (sev.key.keysym.sym & 0x1FF)
! 			{
! 			case SDLK_KP_ENTER & 0x1FF:	event.data1 = GK_RETURN;	break;
! 			case SDLK_RETURN & 0x1FF:	event.data1 = GK_RETURN;	break;
! 			case SDLK_PAGEUP & 0x1FF:	event.data1 = GK_PGUP;		break;
! 			case SDLK_PAGEDOWN & 0x1FF:	event.data1 = GK_PGDN;		break;
! 			case SDLK_END & 0x1FF:		event.data1 = GK_END;		break;
! 			case SDLK_HOME & 0x1FF:		event.data1 = GK_HOME;		break;
! 			case SDLK_LEFT & 0x1FF:		event.data1 = GK_LEFT;		break;
! 			case SDLK_RIGHT & 0x1FF:	event.data1 = GK_RIGHT;		break;
! 			case SDLK_UP & 0x1FF:		event.data1 = GK_UP;		break;
! 			case SDLK_DOWN & 0x1FF:		event.data1 = GK_DOWN;		break;
! 			case SDLK_DELETE & 0x1FF:	event.data1 = GK_DEL;		break;
! 			case SDLK_BACKSPACE & 0x1FF:	event.data1 = GK_BACKSPACE;		break;
! 			case SDLK_ESCAPE & 0x1FF:	event.data1 = GK_ESCAPE;	break;
! 			case SDLK_F1 & 0x1FF:		event.data1 = GK_F1;		break;
! 			case SDLK_F2 & 0x1FF:		event.data1 = GK_F2;		break;
! 			case SDLK_F3 & 0x1FF:		event.data1 = GK_F3;		break;
! 			case SDLK_F4 & 0x1FF:		event.data1 = GK_F4;		break;
! 			case SDLK_F5 & 0x1FF:		event.data1 = GK_F5;		break;
! 			case SDLK_F6 & 0x1FF:		event.data1 = GK_F6;		break;
! 			case SDLK_F7 & 0x1FF:		event.data1 = GK_F7;		break;
! 			case SDLK_F8 & 0x1FF:		event.data1 = GK_F8;		break;
! 			case SDLK_F9 & 0x1FF:		event.data1 = GK_F9;		break;
! 			case SDLK_F10 & 0x1FF:		event.data1 = GK_F10;		break;
! 			case SDLK_F11 & 0x1FF:		event.data1 = GK_F11;		break;
! 			case SDLK_F12 & 0x1FF:		event.data1 = GK_F12;		break;
  			default:
! 				if (sev.key.keysym.sym & 0x1FF < 256)
  				{
! 					event.data1 = sev.key.keysym.sym & 0x1FF;
  				}
  				break;
  			}
+ 			LOGI("keysym = %d, DIK = %d",sev.key.keysym.sym ,event.data1);
  			event.data2 = sev.key.keysym.unicode & 0xff;
  			if (event.data1 < 128)
  			{
