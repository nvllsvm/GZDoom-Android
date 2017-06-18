*** /home/draje/Code/gzdoom/src/v_draw.cpp	2017-06-18 07:23:17.323474086 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/v_draw.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 34,39 ****
--- 34,43 ----
  
  // #define NO_SWRENDER 	// set this if you want to exclude the software renderer. Without software renderer the base implementations of DrawTextureV and FillSimplePoly need to be disabled because they depend on it.
  
+ #ifdef ONLY_GPL
+ #define NO_SWRENDER
+ #endif
+ 
  #include <stdio.h>
  #include <stdarg.h>
  
***************
*** 75,81 ****
  // FillSimplePoly uses this
  extern "C" short spanend[MAXHEIGHT];
  
! CVAR (Bool, hud_scale, false, CVAR_ARCHIVE);
  
  // For routines that take RGB colors, cache the previous lookup in case there
  // are several repetitions with the same color.
--- 79,85 ----
  // FillSimplePoly uses this
  extern "C" short spanend[MAXHEIGHT];
  
! CVAR (Bool, hud_scale, true, CVAR_ARCHIVE);
  
  // For routines that take RGB colors, cache the previous lookup in case there
  // are several repetitions with the same color.
