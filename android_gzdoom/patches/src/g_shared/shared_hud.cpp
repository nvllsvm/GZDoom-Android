*** /home/draje/Code/gzdoom/src/g_shared/shared_hud.cpp	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_shared/shared_hud.cpp	2017-06-18 09:09:24.168397311 -0400
***************
*** 37,42 ****
--- 37,43 ----
  // copy would be.
  
  #include "doomtype.h"
+ #include "doomdef.h"
  #include "v_video.h"
  #include "gi.h"
  #include "c_cvars.h"
***************
*** 48,53 ****
--- 49,55 ----
  #include "p_local.h"
  #include "doomstat.h"
  #include "g_level.h"
+ #include "d_net.h"
  
  #include <time.h>
  
***************
*** 73,78 ****
--- 75,81 ----
  CVAR (Bool,  hud_showweapons,	true, CVAR_ARCHIVE);	// Show weapons collected
  CVAR (Int ,  hud_showtime,		0,	    CVAR_ARCHIVE);	// Show time on HUD
  CVAR (Int ,  hud_timecolor,		CR_GOLD,CVAR_ARCHIVE);	// Color of in-game time on HUD
+ CVAR (Int ,  hud_showlag,		0, CVAR_ARCHIVE);		// Show input latency (maketic - gametic difference)
  
  CVAR (Int, hud_ammo_red, 25, CVAR_ARCHIVE)					// ammo percent less than which status is red    
  CVAR (Int, hud_ammo_yellow, 50, CVAR_ARCHIVE)				// ammo percent less is yellow more green        
***************
*** 917,922 ****
--- 920,970 ----
  	DrawHudText(SmallFont, hud_timecolor, timeString, hudwidth - width, height, FRACUNIT);
  }
  
+ //---------------------------------------------------------------------------
+ //
+ // Draw in-game latency
+ //
+ //---------------------------------------------------------------------------
+ 
+ static void DrawLatency()
+ {
+ 	if (hud_showlag <= 0 ||
+ 		(hud_showlag == 1 && !netgame) ||
+ 		hud_showlag > 2)
+ 	{
+ 		return;
+ 	}
+ 	int i, localdelay = 0, arbitratordelay = 0;
+ 	for (i = 0; i < BACKUPTICS; i++) localdelay += netdelay[0][i];
+ 	for (i = 0; i < BACKUPTICS; i++) arbitratordelay += netdelay[nodeforplayer[Net_Arbitrator]][i];
+ 	localdelay = ((localdelay / BACKUPTICS) * ticdup) * (1000 / TICRATE);
+ 	arbitratordelay = ((arbitratordelay / BACKUPTICS) * ticdup) * (1000 / TICRATE);
+ 	int color = CR_GREEN;
+ 	if (MAX(localdelay, arbitratordelay) > 200)
+ 	{
+ 		color = CR_YELLOW;
+ 	}
+ 	if (MAX(localdelay, arbitratordelay) > 400)
+ 	{
+ 		color = CR_ORANGE;
+ 	}
+ 	if (MAX(localdelay, arbitratordelay) >= ((BACKUPTICS / 2 - 1) * ticdup) * (1000 / TICRATE))
+ 	{
+ 		color = CR_RED;
+ 	}
+ 
+ 	char tempstr[32];
+ 
+ 	const int millis = (level.time % TICRATE) * (1000 / TICRATE);
+ 	mysnprintf(tempstr, sizeof(tempstr), "a:%dms - l:%dms", arbitratordelay, localdelay);
+ 
+ 	const int characterCount = strlen(tempstr);
+ 	const int width = SmallFont->GetCharWidth('0') * characterCount + 2; // small offset from screen's border
+ 	const int height = SmallFont->GetHeight() * 2;
+ 
+ 	DrawHudText(SmallFont, color, tempstr, hudwidth - width, height, FRACUNIT);
+ }
+ 
  
  //---------------------------------------------------------------------------
  //
***************
*** 982,987 ****
--- 1030,1036 ----
  		if (idmypos) DrawCoordinates(CPlayer);
  
  		DrawTime();
+ 		DrawLatency();
  	}
  	else
  	{
