*** /home/draje/Code/gzdoom/src/g_level.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_level.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 199,204 ****
--- 199,244 ----
  //
  //==========================================================================
  
+ CCMD(recordmap)
+ {
+ 	if (netgame)
+ 	{
+ 		Printf("You cannot record a new game while in a netgame.");
+ 		return;
+ 	}
+ 	if (argv.argc() > 2)
+ 	{
+ 		try
+ 		{
+ 			if (!P_CheckMapData(argv[2]))
+ 			{
+ 				Printf("No map %s\n", argv[2]);
+ 			}
+ 			else
+ 			{
+ 				G_DeferedInitNew(argv[2]);
+ 				gameaction = ga_recordgame;
+ 				newdemoname = argv[1];
+ 				newdemomap = argv[2];
+ 			}
+ 		}
+ 		catch (CRecoverableError &error)
+ 		{
+ 			if (error.GetMessage())
+ 				Printf("%s", error.GetMessage());
+ 		}
+ 	}
+ 	else
+ 	{
+ 		Printf("Usage: recordmap <filename> <map name>\n");
+ 	}
+ }
+ 
+ //==========================================================================
+ //
+ //
+ //==========================================================================
+ 
  CCMD (open)
  {
  	if (netgame)
***************
*** 236,241 ****
--- 276,293 ----
  {
  	int i;
  
+ 	// Destory all old player refrences that may still exist
+ 	TThinkerIterator<APlayerPawn> it(STAT_TRAVELLING);
+ 	APlayerPawn *pawn, *next;
+ 
+ 	next = it.Next();
+ 	while ((pawn = next) != NULL)
+ 	{
+ 		next = it.Next();
+ 		pawn->flags |= MF_NOSECTOR | MF_NOBLOCKMAP;
+ 		pawn->Destroy();
+ 	}
+ 
  	G_ClearSnapshots ();
  	ST_SetNeedRefresh();
  	netgame = false;
***************
*** 1112,1117 ****
--- 1164,1171 ----
  			}
  		}
  	}
+ 
+ 	bglobal.StartTravel ();
  }
  
  //==========================================================================
***************
*** 1183,1188 ****
--- 1237,1251 ----
  			pawn->AddToHash ();
  			pawn->SetState(pawn->SpawnState);
  			pawn->player->SendPitchLimits();
+ 			// Sync the FLY flags.
+ 			if (pawn->flags2 & MF2_FLY)
+ 			{
+ 				pawn->player->cheats |= CF_FLY;
+ 			}
+ 			else
+ 			{
+ 				pawn->player->cheats &= ~CF_FLY;
+ 			}
  
  			for (inv = pawn->Inventory; inv != NULL; inv = inv->Inventory)
  			{
***************
*** 1200,1205 ****
--- 1263,1270 ----
  			}
  		}
  	}
+ 
+ 	bglobal.FinishTravel ();
  }
   
  //==========================================================================
***************
*** 1222,1227 ****
--- 1287,1293 ----
  	level.teamdamage = teamdamage;
  	level.flags = 0;
  	level.flags2 = 0;
+ 	level.flags3 = 0;
  
  	info = FindLevelInfo (level.MapName);
  
***************
*** 1275,1280 ****
--- 1341,1347 ----
  	level.clusterflags = clus ? clus->flags : 0;
  	level.flags |= info->flags;
  	level.flags2 |= info->flags2;
+ 	level.flags3 |= info->flags3;
  	level.levelnum = info->levelnum;
  	level.Music = info->Music;
  	level.musicorder = info->musicorder;
***************
*** 1378,1393 ****
  //
  //
  //==========================================================================
  
  void G_SerializeLevel (FArchive &arc, bool hubLoad)
  {
! 	int i = level.totaltime;
! 	
! 	Renderer->StartSerialize(arc);
! 
! 	arc << level.flags
! 		<< level.flags2
! 		<< level.fadeto
  		<< level.found_secrets
  		<< level.found_items
  		<< level.killed_monsters
--- 1445,1463 ----
  //
  //
  //==========================================================================
+ void gl_SerializeGlobals(FArchive &arc);
  
  void G_SerializeLevel (FArchive &arc, bool hubLoad)
  {
! 		int i = level.totaltime;
! 		
! 		Renderer->StartSerialize(arc);
! 	#ifndef __ANDROID__
! 		gl_SerializeGlobals(arc);
! 	#endif
! 		arc << level.flags
! 			<< level.flags2
! 			<< level.fadeto
  		<< level.found_secrets
  		<< level.found_items
  		<< level.killed_monsters
