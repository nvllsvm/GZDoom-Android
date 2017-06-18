*** /home/draje/Code/gzdoom/src/tempfiles.h	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/tempfiles.h	2017-06-18 11:04:59.829467002 -0400
***************
*** 38,43 ****
--- 38,45 ----
  #pragma once
  #endif
  
+ #include <stdlib.h>
+ 
  // Returns a file name suitable for use as a temp file.
  // If you create a file with this name (and presumably you
  // will), it will be deleted automatically by this class's
