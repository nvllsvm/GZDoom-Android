*** /home/draje/Code/gzdoom/src/statnums.h	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/statnums.h	2017-06-18 09:39:35.678101881 -0400
***************
*** 63,68 ****
--- 63,69 ----
  	STAT_SECTOREFFECT,						// All sector effects that cause floor and ceiling movement
  	STAT_ACTORMOVER,						// actor movers
  	STAT_SCRIPTS,							// The ACS thinker. This is to ensure that it can't tick before all actors called PostBeginPlay
+ 	STAT_BOT,								// Bot thinker
  };
  
  #endif
\ No newline at end of file
