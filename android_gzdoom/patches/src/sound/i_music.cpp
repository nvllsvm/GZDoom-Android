*** doom/src/main/jni/gzdoom/src/sound/i_music.cpp	2017-06-20 19:11:53.559650382 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/sound/i_music.cpp	2017-06-23 22:19:48.929897200 -0400
***************
*** 44,50 ****
--- 44,52 ----
  #include <fcntl.h>
  #include <signal.h>
  #include <unistd.h>
+ #ifndef __ANDROID__
  #include <wordexp.h>
+ #endif
  #include <stdio.h>
  #include "mus2midi.h"
  #define FALSE 0
