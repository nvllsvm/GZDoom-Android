*** /home/draje/Code/gzdoom/src/p_tick.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_tick.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 120,126 ****
  
  	for (i = 0; i<MAXPLAYERS; i++)
  		if (playeringame[i] &&
! 			/*Added by MC: Freeze mode.*/!(bglobal.freeze && players[i].isbot))
  			P_PlayerThink (&players[i]);
  
  	StatusBar->Tick ();		// [RH] moved this here
--- 120,126 ----
  
  	for (i = 0; i<MAXPLAYERS; i++)
  		if (playeringame[i] &&
! 			/*Added by MC: Freeze mode.*/!(bglobal.freeze && players[i].Bot != NULL))
  			P_PlayerThink (&players[i]);
  
  	StatusBar->Tick ();		// [RH] moved this here
