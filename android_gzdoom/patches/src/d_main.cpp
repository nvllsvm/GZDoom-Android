*** /home/draje/Code/gzdoom/src/d_main.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/d_main.cpp	2017-06-18 10:53:16.673098915 -0400
***************
*** 2263,2272 ****
--- 2254,2265 ----
  		gameinfo.ConfigName = iwad_info->Configname;
  		lastIWAD = iwad;
  
+ #ifndef __ANDROID__
  		if ((gameinfo.flags & GI_SHAREWARE) && pwads.Size() > 0)
  		{
  			I_FatalError ("You cannot -file with the shareware version. Register!");
  		}
+ #endif
  
  		FBaseCVar::DisableCallbacks();
  		GameConfig->DoGameSetup (gameinfo.ConfigName);
