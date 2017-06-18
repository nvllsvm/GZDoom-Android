*** /home/draje/Code/gzdoom/src/g_game.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_game.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 76,81 ****
--- 76,82 ----
  #include "d_net.h"
  #include "d_event.h"
  #include "p_acs.h"
+ #include "p_effect.h"
  #include "m_joy.h"
  #include "farchive.h"
  #include "r_renderer.h"
***************
*** 115,120 ****
--- 116,122 ----
  CVAR (Bool, storesavepic, true, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  CVAR (Bool, longsavemessages, true, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  CVAR (String, save_dir, "", CVAR_ARCHIVE|CVAR_GLOBALCONFIG);
+ CVAR (Bool, cl_waitforsave, true, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
  EXTERN_CVAR (Float, con_midtime);
  
  //==========================================================================
***************
*** 140,145 ****
--- 142,148 ----
  gamestate_t 	gamestate = GS_STARTUP;
  
  int 			paused;
+ bool			pauseext;
  bool 			sendpause;				// send a pause event next tic 
  bool			sendsave;				// send a save event next tic 
  bool			sendturn180;			// [RH] send a 180 degree turn next tic
***************
*** 161,166 ****
--- 164,171 ----
  int 			gametic;
  
  CVAR(Bool, demo_compress, true, CVAR_ARCHIVE|CVAR_GLOBALCONFIG);
+ FString			newdemoname;
+ FString			newdemomap;
  FString			demoname;
  bool 			demorecording;
  bool 			demoplayback;
***************
*** 518,523 ****
--- 523,533 ----
  	}
  }
  
+ 
+ #ifdef __ANDROID__
+ extern void Android_IN_Move(ticcmd_t* cmd );
+ 
+ #endif
  //
  // G_BuildTiccmd
  // Builds a ticcmd from all of the available inputs
***************
*** 681,686 ****
--- 691,700 ----
  		forward += (int)((float)mousey * m_forward);
  	}
  
+ #ifdef __ANDROID__
+ 	Android_IN_Move(cmd);
+ #endif
+ 
  	cmd->ucmd.pitch = LocalViewPitch >> 16;
  
  	if (SendLand)
***************
*** 874,880 ****
  			pnum &= MAXPLAYERS-1;
  			if (playeringame[pnum] &&
  				(!checkTeam || players[pnum].mo->IsTeammate (players[consoleplayer].mo) ||
! 				(bot_allowspy && players[pnum].isbot)))
  			{
  				break;
  			}
--- 888,894 ----
  			pnum &= MAXPLAYERS-1;
  			if (playeringame[pnum] &&
  				(!checkTeam || players[pnum].mo->IsTeammate (players[consoleplayer].mo) ||
! 				(bot_allowspy && players[pnum].Bot != NULL)))
  			{
  				break;
  			}
***************
*** 1013,1022 ****
  	// do player reborns if needed
  	for (i = 0; i < MAXPLAYERS; i++)
  	{
! 		if (playeringame[i] &&
! 			(players[i].playerstate == PST_REBORN || players[i].playerstate == PST_ENTER))
  		{
! 			G_DoReborn (i, false);
  		}
  	}
  
--- 1027,1042 ----
  	// do player reborns if needed
  	for (i = 0; i < MAXPLAYERS; i++)
  	{
! 		if (playeringame[i])
  		{
! 			if ((players[i].playerstate == PST_GONE))
! 			{
! 				G_DoPlayerPop(i);
! 			}
! 			if ((players[i].playerstate == PST_REBORN || players[i].playerstate == PST_ENTER))
! 			{
! 				G_DoReborn(i, false);
! 			}
  		}
  	}
  
***************
*** 1041,1046 ****
--- 1061,1070 ----
  		case ga_loadlevel:
  			G_DoLoadLevel (-1, false);
  			break;
+ 		case ga_recordgame:
+ 			G_CheckDemoStatus();
+ 			G_RecordDemo(newdemoname);
+ 			G_BeginRecording(newdemomap);
  		case ga_newgame2:	// Silence GCC (see above)
  		case ga_newgame:
  			G_DoNewGame ();
***************
*** 1114,1119 ****
--- 1138,1146 ----
  	// check, not just the player's x position like BOOM.
  	DWORD rngsum = FRandom::StaticSumSeeds ();
  
+ 	//Added by MC: For some of that bot stuff. The main bot function.
+ 	bglobal.Main ();
+ 
  	for (i = 0; i < MAXPLAYERS; i++)
  	{
  		if (playeringame[i])
***************
*** 1133,1145 ****
  			// If the user alt-tabbed away, paused gets set to -1. In this case,
  			// we do not want to read more demo commands until paused is no
  			// longer negative.
! 			if (demoplayback && paused >= 0)
  			{
  				G_ReadDemoTiccmd (cmd, i);
  			}
  			else
  			{
! 				memcpy (cmd, newcmd, sizeof(ticcmd_t));
  			}
  
  			// check for turbo cheats
--- 1160,1172 ----
  			// If the user alt-tabbed away, paused gets set to -1. In this case,
  			// we do not want to read more demo commands until paused is no
  			// longer negative.
! 			if (demoplayback)
  			{
  				G_ReadDemoTiccmd (cmd, i);
  			}
  			else
  			{
! 				memcpy(cmd, newcmd, sizeof(ticcmd_t));
  			}
  
  			// check for turbo cheats
***************
*** 1149,1155 ****
  				Printf ("%s is turbo!\n", players[i].userinfo.GetName());
  			}
  
! 			if (netgame && !players[i].isbot && !demoplayback && (gametic%ticdup) == 0)
  			{
  				//players[i].inconsistant = 0;
  				if (gametic > BACKUPTICS*ticdup && consistancy[i][buf] != cmd->consistancy)
--- 1176,1182 ----
  				Printf ("%s is turbo!\n", players[i].userinfo.GetName());
  			}
  
! 			if (netgame && players[i].Bot == NULL && !demoplayback && (gametic%ticdup) == 0)
  			{
  				//players[i].inconsistant = 0;
  				if (gametic > BACKUPTICS*ticdup && consistancy[i][buf] != cmd->consistancy)
***************
*** 1331,1340 ****
  	int			chasecam;
  	BYTE		currclass;
  	userinfo_t  userinfo;	// [RH] Save userinfo
- 	botskill_t  b_skill;	//Added by MC:
  	APlayerPawn *actor;
  	const PClass *cls;
  	FString		log;
  
  	p = &players[player];
  
--- 1358,1367 ----
  	int			chasecam;
  	BYTE		currclass;
  	userinfo_t  userinfo;	// [RH] Save userinfo
  	APlayerPawn *actor;
  	const PClass *cls;
  	FString		log;
+ 	DBot		*Bot;		//Added by MC:
  
  	p = &players[player];
  
***************
*** 1344,1361 ****
  	itemcount = p->itemcount;
  	secretcount = p->secretcount;
  	currclass = p->CurrentPlayerClass;
-     b_skill = p->skill;    //Added by MC:
  	userinfo.TransferFrom(p->userinfo);
  	actor = p->mo;
  	cls = p->cls;
  	log = p->LogText;
  	chasecam = p->cheats & CF_CHASECAM;
  
  	// Reset player structure to its defaults
  	p->~player_t();
  	::new(p) player_t;
  
  	memcpy (p->frags, frags, sizeof(p->frags));
  	p->fragcount = fragcount;
  	p->killcount = killcount;
  	p->itemcount = itemcount;
--- 1371,1389 ----
  	itemcount = p->itemcount;
  	secretcount = p->secretcount;
  	currclass = p->CurrentPlayerClass;
  	userinfo.TransferFrom(p->userinfo);
  	actor = p->mo;
  	cls = p->cls;
  	log = p->LogText;
  	chasecam = p->cheats & CF_CHASECAM;
+ 	Bot = p->Bot;			//Added by MC:
  
  	// Reset player structure to its defaults
  	p->~player_t();
  	::new(p) player_t;
  
  	memcpy (p->frags, frags, sizeof(p->frags));
+ 	p->health = actor->health;
  	p->fragcount = fragcount;
  	p->killcount = killcount;
  	p->itemcount = itemcount;
***************
*** 1366,1373 ****
  	p->cls = cls;
  	p->LogText = log;
  	p->cheats |= chasecam;
! 
!     p->skill = b_skill;	//Added by MC:
  
  	p->oldbuttons = ~0, p->attackdown = true; p->usedown = true;	// don't do anything immediately
  	p->original_oldbuttons = ~0;
--- 1394,1400 ----
  	p->cls = cls;
  	p->LogText = log;
  	p->cheats |= chasecam;
! 	p->Bot = Bot;			//Added by MC:
  
  	p->oldbuttons = ~0, p->attackdown = true; p->usedown = true;	// don't do anything immediately
  	p->original_oldbuttons = ~0;
***************
*** 1375,1389 ****
  
  	if (gamestate != GS_TITLELEVEL)
  	{
  		actor->GiveDefaultInventory ();
  		p->ReadyWeapon = p->PendingWeapon;
  	}
  
!     //Added by MC: Init bot structure.
!     if (bglobal.botingame[player])
!         bglobal.CleanBotstuff (p);
!     else
! 		p->isbot = false;
  }
  
  //
--- 1402,1420 ----
  
  	if (gamestate != GS_TITLELEVEL)
  	{
+ 		// [GRB] Give inventory specified in DECORATE
  		actor->GiveDefaultInventory ();
  		p->ReadyWeapon = p->PendingWeapon;
  	}
  
! 	//Added by MC: Init bot structure.
! 	if (p->Bot != NULL)
! 	{
! 		botskill_t skill = p->Bot->skill;
! 		p->Bot->Clear ();
! 		p->Bot->player = p;
! 		p->Bot->skill = skill;
! 	}
  }
  
  //
***************
*** 1658,1663 ****
--- 1689,1744 ----
  	}
  }
  
+ //
+ // G_DoReborn
+ //
+ void G_DoPlayerPop(int playernum)
+ {
+ 	playeringame[playernum] = false;
+ 
+ 	if (deathmatch)
+ 	{
+ 		Printf("%s left the game with %d frags\n",
+ 			players[playernum].userinfo.GetName(),
+ 			players[playernum].fragcount);
+ 	}
+ 	else
+ 	{
+ 		Printf("%s left the game\n", players[playernum].userinfo.GetName());
+ 	}
+ 
+ 	// [RH] Revert each player to their own view if spying through the player who left
+ 	for (int ii = 0; ii < MAXPLAYERS; ++ii)
+ 	{
+ 		if (playeringame[ii] && players[ii].camera == players[playernum].mo)
+ 		{
+ 			players[ii].camera = players[ii].mo;
+ 			if (ii == consoleplayer && StatusBar != NULL)
+ 			{
+ 				StatusBar->AttachToPlayer(&players[ii]);
+ 			}
+ 		}
+ 	}
+ 
+ 	// [RH] Make the player disappear
+ 	FBehavior::StaticStopMyScripts(players[playernum].mo);
+ 	if (players[playernum].mo != NULL)
+ 	{
+ 		P_DisconnectEffect(players[playernum].mo);
+ 		players[playernum].mo->player = NULL;
+ 		players[playernum].mo->Destroy();
+ 		if (!(players[playernum].mo->ObjectFlags & OF_EuthanizeMe))
+ 		{ // We just destroyed a morphed player, so now the original player
+ 			// has taken their place. Destroy that one too.
+ 			players[playernum].mo->Destroy();
+ 		}
+ 		players[playernum].mo = NULL;
+ 		players[playernum].camera = NULL;
+ 	}
+ 	// [RH] Let the scripts know the player left
+ 	FBehavior::StaticStartTypedScripts(SCRIPT_Disconnect, NULL, true, playernum);
+ }
+ 
  void G_ScreenShot (char *filename)
  {
  	shotfile = filename;
***************
*** 2076,2081 ****
--- 2157,2165 ----
  		filename = G_BuildSaveName ("demosave.zds", -1);
  	}
  
+ 	if (cl_waitforsave)
+ 		I_FreezeTime(true);
+ 
  	insave = true;
  	G_SnapshotLevel ();
  
***************
*** 2085,2090 ****
--- 2169,2175 ----
  	{
  		Printf ("Could not create savegame '%s'\n", filename.GetChars());
  		insave = false;
+ 		I_FreezeTime(false);
  		return;
  	}
  
***************
*** 2161,2166 ****
--- 2246,2252 ----
  	}
  		
  	insave = false;
+ 	I_FreezeTime(false);
  }
  
  
***************
*** 2373,2378 ****
--- 2459,2474 ----
  
  CCMD (playdemo)
  {
+ 	if (netgame)
+ 	{
+ 		Printf("End your current netgame first!");
+ 		return;
+ 	}
+ 	if (demorecording)
+ 	{
+ 		Printf("End your current demo first!");
+ 		return;
+ 	}
  	if (argv.argc() > 1)
  	{
  		G_DeferedPlayDemo (argv[1]);
***************
*** 2555,2561 ****
  	{
  		FixPathSeperator (defdemoname);
  		DefaultExtension (defdemoname, ".lmp");
! 		M_ReadFile (defdemoname, &demobuffer);
  	}
  	demo_p = demobuffer;
  
--- 2651,2657 ----
  	{
  		FixPathSeperator (defdemoname);
  		DefaultExtension (defdemoname, ".lmp");
! 		M_ReadFileMalloc (defdemoname, &demobuffer);
  	}
  	demo_p = demobuffer;
  
