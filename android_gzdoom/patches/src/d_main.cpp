*** /home/draje/Code/gzdoom/src/d_main.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/d_main.cpp	2017-06-18 10:53:16.673098915 -0400
***************
*** 466,472 ****
  		}
  
  		// Come out of chasecam mode if we're not allowed to use chasecam.
! 		if (!(dmflags2 & DF2_CHASECAM) && !G_SkillProperty (SKILLP_DisableCheats) && !sv_cheats)
  		{
  			// Take us out of chasecam mode only.
  			if (p->cheats & CF_CHASECAM)
--- 466,472 ----
  		}
  
  		// Come out of chasecam mode if we're not allowed to use chasecam.
! 		if (!(dmflags2 & DF2_CHASECAM) && CheckCheatmode(false))
  		{
  			// Take us out of chasecam mode only.
  			if (p->cheats & CF_CHASECAM)
***************
*** 620,625 ****
--- 620,626 ----
  CVAR (Flag, compat_maskedmidtex,		compatflags,  COMPATF_MASKEDMIDTEX);
  CVAR (Flag, compat_badangles,			compatflags2, COMPATF2_BADANGLES);
  CVAR (Flag, compat_floormove,			compatflags2, COMPATF2_FLOORMOVE);
+ CVAR (Flag, compat_soundcutoff,			compatflags2, COMPATF2_SOUNDCUTOFF);
  
  //==========================================================================
  //
***************
*** 756,764 ****
  			}
  			screen->SetBlendingRect(viewwindowx, viewwindowy,
  				viewwindowx + viewwidth, viewwindowy + viewheight);
! 			P_PredictPlayer(&players[consoleplayer]);
  			Renderer->RenderView(&players[consoleplayer]);
! 			P_UnPredictPlayer();
  			if ((hw2d = screen->Begin2D(viewactive)))
  			{
  				// Redraw everything every frame when using 2D accel
--- 757,765 ----
  			}
  			screen->SetBlendingRect(viewwindowx, viewwindowy,
  				viewwindowx + viewwidth, viewwindowy + viewheight);
! 
  			Renderer->RenderView(&players[consoleplayer]);
! 
  			if ((hw2d = screen->Begin2D(viewactive)))
  			{
  				// Redraw everything every frame when using 2D accel
***************
*** 832,846 ****
  		}
  	}
  	// draw pause pic
! 	if (paused && menuactive == MENU_Off)
  	{
  		FTexture *tex;
  		int x;
  
  		tex = TexMan(gameinfo.PauseSign);
  		x = (SCREENWIDTH - tex->GetScaledWidth() * CleanXfac)/2 +
  			tex->GetScaledLeftOffset() * CleanXfac;
  		screen->DrawTexture (tex, x, 4, DTA_CleanNoMove, true, TAG_DONE);
  	}
  
  	// [RH] Draw icon, if any
--- 833,855 ----
  		}
  	}
  	// draw pause pic
! 	if ((paused || pauseext) && menuactive == MENU_Off)
  	{
  		FTexture *tex;
  		int x;
+ 		FString pstring = "By ";
  
  		tex = TexMan(gameinfo.PauseSign);
  		x = (SCREENWIDTH - tex->GetScaledWidth() * CleanXfac)/2 +
  			tex->GetScaledLeftOffset() * CleanXfac;
  		screen->DrawTexture (tex, x, 4, DTA_CleanNoMove, true, TAG_DONE);
+ 		if (paused && multiplayer)
+ 		{
+ 			pstring += players[paused - 1].userinfo.GetName();
+ 			screen->DrawText(SmallFont, CR_RED,
+ 				(screen->GetWidth() - SmallFont->StringWidth(pstring)*CleanXfac) / 2,
+ 				(tex->GetScaledHeight() * CleanYfac) + 4, pstring, DTA_CleanNoMove, true, TAG_DONE);
+ 		}
  	}
  
  	// [RH] Draw icon, if any
***************
*** 974,998 ****
  				I_StartTic ();
  				D_ProcessEvents ();
  				G_BuildTiccmd (&netcmds[consoleplayer][maketic%BACKUPTICS]);
- 				//Added by MC: For some of that bot stuff. The main bot function.
- 				int i;
- 				for (i = 0; i < MAXPLAYERS; i++)
- 				{
- 					if (playeringame[i] && players[i].isbot && players[i].mo)
- 					{
- 						players[i].savedyaw = players[i].mo->angle;
- 						players[i].savedpitch = players[i].mo->pitch;
- 					}
- 				}
- 				bglobal.Main (maketic%BACKUPTICS);
- 				for (i = 0; i < MAXPLAYERS; i++)
- 				{
- 					if (playeringame[i] && players[i].isbot && players[i].mo)
- 					{
- 						players[i].mo->angle = players[i].savedyaw;
- 						players[i].mo->pitch = players[i].savedpitch;
- 					}
- 				}
  				if (advancedemo)
  					D_DoAdvanceDemo ();
  				C_Ticker ();
--- 983,988 ----
***************
*** 1337,1342 ****
--- 1327,1333 ----
  	{
  		gameaction = ga_fullconsole;
  		demosequence = -1;
+ 		G_CheckDemoStatus();
  	}
  }
  
***************
*** 2263,2272 ****
--- 2254,2265 ----
  		gameinfo.ConfigName = iwad_info->Configname;
  		lastIWAD = iwad;
  
+ #ifndef __ANDROID__
  		if ((gameinfo.flags & GI_SHAREWARE) && pwads.Size() > 0)
  		{
  			I_FatalError ("You cannot -file with the shareware version. Register!");
  		}
+ #endif
  
  		FBaseCVar::DisableCallbacks();
  		GameConfig->DoGameSetup (gameinfo.ConfigName);
***************
*** 2282,2287 ****
--- 2275,2282 ----
  		execFiles = Args->GatherFiles ("-exec");
  		D_MultiExec (execFiles, true);
  
+ 		C_ExecCmdLineParams ();		// [RH] do all +set commands on the command line
+ 
  		CopyFiles(allwads, pwads);
  
  		// Since this function will never leave we must delete this array here manually.
***************
*** 2297,2303 ****
  		// Now that wads are loaded, define mod-specific cvars.
  		ParseCVarInfo();
  
! 		C_ExecCmdLineParams ();		// [RH] do all +set commands on the command line
  
  		// [RH] Initialize localizable strings.
  		GStrings.LoadStrings (false);
--- 2292,2299 ----
  		// Now that wads are loaded, define mod-specific cvars.
  		ParseCVarInfo();
  
! 		// Try setting previously unknown cvars again, as a CVARINFO may have made them known.
! 		C_ExecStoredSets();
  
  		// [RH] Initialize localizable strings.
  		GStrings.LoadStrings (false);
***************
*** 2587,2592 ****
--- 2583,2589 ----
  			new (&gameinfo) gameinfo_t;		// Reset gameinfo
  			S_Shutdown();					// free all channels and delete playlist
  			C_ClearAliases();				// CCMDs won't be reinitialized so these need to be deleted here
+ 			DestroyCVarsFlagged(CVAR_MOD);	// Delete any cvar left by mods
  
  			GC::FullGC();					// perform one final garbage collection before deleting the class data
  			PClass::ClearRuntimeData();		// clear all runtime generated class data
