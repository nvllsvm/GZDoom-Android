*** /home/draje/Code/gzdoom/src/g_shared/sbar_mugshot.cpp	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_shared/sbar_mugshot.cpp	2017-06-18 09:09:24.168397311 -0400
***************
*** 444,450 ****
  		if (bNormal)
  		{
  			bool good;
! 			if ((player->cheats & CF_GODMODE) || (player->mo != NULL && player->mo->flags2 & MF2_INVULNERABLE))
  			{
  				good = SetState((stateflags & ANIMATEDGODMODE) ? "godanimated" : "god");
  			}
--- 444,450 ----
  		if (bNormal)
  		{
  			bool good;
! 			if ((player->cheats & CF_GODMODE) || (player->cheats & CF_GODMODE2) || (player->mo != NULL && player->mo->flags2 & MF2_INVULNERABLE))
  			{
  				good = SetState((stateflags & ANIMATEDGODMODE) ? "godanimated" : "god");
  			}
