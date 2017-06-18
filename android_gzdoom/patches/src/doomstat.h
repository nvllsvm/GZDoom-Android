*** /home/draje/Code/gzdoom/src/doomstat.h	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/doomstat.h	2017-06-18 09:09:24.161730580 -0400
***************
*** 109,114 ****
--- 109,115 ----
  extern	bool			automapactive;	// In AutoMap mode?
  extern	EMenuState		menuactive; 	// Menu overlayed?
  extern	int				paused; 		// Game Pause?
+ extern	bool			pauseext;
  
  
  extern	bool			viewactive;
***************
*** 136,141 ****
--- 137,144 ----
  // Disable save/end game?
  extern	bool			usergame;
  
+ extern	FString			newdemoname;
+ extern	FString			newdemomap;
  extern	bool			demoplayback;
  extern	bool			demorecording;
  extern	int				demover;
