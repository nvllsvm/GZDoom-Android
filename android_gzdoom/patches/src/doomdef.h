*** /home/draje/Code/gzdoom/src/doomdef.h	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/doomdef.h	2017-06-18 09:09:24.161730580 -0400
***************
*** 339,344 ****
--- 339,345 ----
  
  	COMPATF2_BADANGLES		= 1 << 0,	// It is impossible to face directly NSEW.
  	COMPATF2_FLOORMOVE		= 1 << 1,	// Use the same floor motion behavior as Doom.
+ 	COMPATF2_SOUNDCUTOFF	= 1 << 2,	// Cut off sounds when an actor vanishes instead of making it owner-less
  };
  
  // Emulate old bugs for select maps. These are not exposed by a cvar
