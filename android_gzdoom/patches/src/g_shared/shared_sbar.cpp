*** /home/draje/Code/gzdoom/src/g_shared/shared_sbar.cpp	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_shared/shared_sbar.cpp	2017-06-18 09:09:24.168397311 -0400
***************
*** 1123,1129 ****
  	ST_LoadCrosshair();
  
  	// Don't draw the crosshair if there is none
! 	if (CrosshairImage == NULL || gamestate == GS_TITLELEVEL)
  	{
  		return;
  	}
--- 1123,1129 ----
  	ST_LoadCrosshair();
  
  	// Don't draw the crosshair if there is none
! 	if (CrosshairImage == NULL || gamestate == GS_TITLELEVEL || camera->health <= 0)
  	{
  		return;
  	}
