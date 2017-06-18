*** /home/draje/Code/gzdoom/src/tempfiles.cpp	2017-06-18 07:23:17.320140719 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/tempfiles.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 42,49 ****
--- 42,54 ----
  // mkstemp should be used instead. However, there is no mkstemp
  // under VC++, and even if there was, I still need to know the
  // file name so that it can be used as input to Timidity.
+ #ifdef __ANDROID__
+ 	Name = malloc(256);
+ 	sprintf(Name,"./gzdoom_cache/%s",prefix);
  
+ #else
  	Name = tempnam (NULL, prefix);
+ #endif
  }
  
  FTempFileName::~FTempFileName ()
