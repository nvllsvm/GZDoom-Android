*** /home/draje/Code/gzdoom/src/menu/loadsavemenu.cpp	2017-06-18 07:23:17.300140515 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/menu/loadsavemenu.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 965,970 ****
--- 965,976 ----
  	V_SetBorderNeedRefresh();
  }
  
+ #ifdef __ANDROID__
+ extern "C"
+ {
+ extern void showKeyboard(int val);
+ }
+ #endif
  //=============================================================================
  //
  //
***************
*** 995,1008 ****
--- 1001,1023 ----
  		DMenu *input = new DTextEnterMenu(this, savegamestring, SAVESTRINGSIZE, 1, fromcontroller);
  		M_ActivateMenu(input);
  		mEntering = true;
+ #ifdef __ANDROID__
+ 		showKeyboard(1);
+ #endif
  	}
  	else if (mkey == MKEY_Input)
  	{
+ #ifdef __ANDROID__
+ 		showKeyboard(0);
+ #endif
  		mEntering = false;
  		DoSave(SaveGames[Selected]);
  	}
  	else if (mkey == MKEY_Abort)
  	{
+ #ifdef __ANDROID__
+ 		showKeyboard(0);
+ #endif
  		mEntering = false;
  	}
  	return false;
