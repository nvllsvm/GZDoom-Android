*** /home/draje/Code/gzdoom/src/g_shared/a_pickups.h	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_shared/a_pickups.h	2017-06-18 09:09:24.168397311 -0400
***************
*** 135,140 ****
--- 135,141 ----
  	IF_NEVERRESPAWN		= 1<<20,	// Never, ever respawns
  	IF_NOSCREENFLASH	= 1<<21,	// No pickup flash on the player's screen
  	IF_TOSSED			= 1<<22,	// Was spawned by P_DropItem (i.e. as a monster drop)
+ 	IF_ALWAYSRESPAWN	= 1<<23,	// Always respawn, regardless of dmflag
  
  };
  
