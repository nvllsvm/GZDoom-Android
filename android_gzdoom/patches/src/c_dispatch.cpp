*** /home/draje/Code/gzdoom/src/c_dispatch.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/c_dispatch.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 185,190 ****
--- 185,193 ----
  	"clearplayerclasses"
  };
  
+ static TArray<FString> StoredStartupSets;
+ static bool RunningStoredStartups;
+ 
  // CODE --------------------------------------------------------------------
  
  IMPLEMENT_CLASS (DWaitingCommand)
***************
*** 537,542 ****
--- 540,557 ----
  	}
  }
  
+ void C_ExecStoredSets()
+ {
+ 	assert(!RunningStoredStartups);
+ 	RunningStoredStartups = true;
+ 	for (unsigned i = 0; i < StoredStartupSets.Size(); ++i)
+ 	{
+ 		C_DoCommand(StoredStartupSets[i]);
+ 	}
+ 	StoredStartupSets.Clear();
+ 	RunningStoredStartups = false;
+ }
+ 
  void C_DoCommand (const char *cmd, int keynum)
  {
  	FConsoleCommand *com;
***************
*** 612,618 ****
  
  	if ( (com = FindNameInHashTable (Commands, beg, len)) )
  	{
! 		if (gamestate != GS_STARTUP || ParsingKeyConf ||
  			(len == 3 && strnicmp (beg, "set", 3) == 0) ||
  			(len == 7 && strnicmp (beg, "logfile", 7) == 0) ||
  			(len == 9 && strnicmp (beg, "unbindall", 9) == 0) ||
--- 627,648 ----
  
  	if ( (com = FindNameInHashTable (Commands, beg, len)) )
  	{
! 		if (gamestate == GS_STARTUP && !RunningStoredStartups &&
! 			len == 3 && strnicmp(beg, "set", 3) == 0)
! 		{
! 			// Save setting of unknown cvars for later, in case a loaded wad has a
! 			// CVARINFO that defines it.
! 			FCommandLine args(beg);
! 			if (args.argc() > 1 && FindCVar(args[1], NULL) == NULL)
! 			{
! 				StoredStartupSets.Push(beg);
! 			}
! 			else
! 			{
! 				com->Run(args, players[consoleplayer].mo, keynum);
! 			}
! 		}
! 		else if (gamestate != GS_STARTUP || ParsingKeyConf ||
  			(len == 3 && strnicmp (beg, "set", 3) == 0) ||
  			(len == 7 && strnicmp (beg, "logfile", 7) == 0) ||
  			(len == 9 && strnicmp (beg, "unbindall", 9) == 0) ||
***************
*** 657,668 ****
  		}
  		else
  		{ // We don't know how to handle this command
! 			char cmdname[64];
! 			size_t minlen = MIN<size_t> (len, 63);
! 
! 			memcpy (cmdname, beg, minlen);
! 			cmdname[len] = 0;
! 			Printf ("Unknown command \"%s\"\n", cmdname);
  		}
  	}
  }
--- 687,701 ----
  		}
  		else
  		{ // We don't know how to handle this command
! 			if (gamestate == GS_STARTUP && !RunningStoredStartups)
! 			{
! 				// Save it for later, in case a CVARINFO defines it.
! 				StoredStartupSets.Push(beg);
! 			}
! 			else
! 			{
! 				Printf ("Unknown command \"%.*s\"\n", len, beg);
! 			}
  		}
  	}
  }
