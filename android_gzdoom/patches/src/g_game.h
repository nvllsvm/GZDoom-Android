*** /home/draje/Code/gzdoom/src/g_game.h	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_game.h	2017-06-18 09:09:24.161730580 -0400
***************
*** 81,86 ****
--- 81,87 ----
  void G_PlayerFinishLevel (int player, EFinishLevelType mode, int flags);
  
  void G_DoReborn (int playernum, bool freshbot);
+ void G_DoPlayerPop(int playernum);
  
  // Adds pitch to consoleplayer's viewpitch and clamps it
  void G_AddViewPitch (int look);
