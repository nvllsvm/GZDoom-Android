*** doom/src/main/jni/gzdoom/src/sdl/i_system.h	2017-07-01 18:06:20.415950933 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/android/i_system.h	2017-07-02 01:33:06.065396558 -0400
***************
*** 23,28 ****
--- 23,36 ----
  #ifndef __I_SYSTEM__
  #define __I_SYSTEM__
  
+ #include <android/log.h>
+ 
+ #ifndef LOGI
+ #define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"JNI", __VA_ARGS__))
+ #define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "JNI", __VA_ARGS__))
+ #define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"JNI", __VA_ARGS__))
+ #endif
+ 
  #include <dirent.h>
  #include <ctype.h>
  
