*** /home/draje/Code/gzdoom/src/d_iwad.cpp	2017-06-18 09:55:08.420031453 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/d_iwad.cpp	2017-06-18 09:56:47.020990068 -0400
***************
*** 451,456 ****
--- 451,464 ----
  			}
  		}
  #endif
+ 
+ #ifndef __ANDROID__
+         TArray<FString> steam_path = I_GetSteamPath();
+         for (i = 0; i < steam_path.Size(); ++i)
+         {
+             CheckIWAD (steam_path[i], &wads[0]);
+         }
+ #endif
  	}
  
  	if (iwadparm != NULL && !wads[0].Path.IsEmpty())
