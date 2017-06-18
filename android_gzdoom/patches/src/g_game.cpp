*** /home/draje/Code/gzdoom/src/g_game.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_game.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 518,523 ****
--- 523,533 ----
  	}
  }
  
+ 
+ #ifdef __ANDROID__
+ extern void Android_IN_Move(ticcmd_t* cmd );
+ 
+ #endif
  //
  // G_BuildTiccmd
  // Builds a ticcmd from all of the available inputs
***************
*** 681,686 ****
--- 691,700 ----
  		forward += (int)((float)mousey * m_forward);
  	}
  
+ #ifdef __ANDROID__
+ 	Android_IN_Move(cmd);
+ #endif
+ 
  	cmd->ucmd.pitch = LocalViewPitch >> 16;
  
  	if (SendLand)
