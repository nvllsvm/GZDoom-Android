*** /home/draje/Code/gzdoom/src/gl/utility/gl_convert.h	2017-06-18 07:52:50.696346991 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/gl/utility/gl_convert.h	2017-06-18 09:09:24.168397311 -0400
***************
*** 1,9 ****
! 
! #ifndef __GLC_CONVERT
! #define __GLC_CONVERT
! 
! #include "m_fixed.h"
! #define ANGLE_TO_FLOAT(ang) ((float)((ang) * 180. / ANGLE_180))
! #define FLOAT_TO_ANGLE(ang) xs_RoundToUInt((ang) / 180. * ANGLE_180)
! 
  #endif
\ No newline at end of file
--- 1,9 ----
! 
! #ifndef __GLC_CONVERT
! #define __GLC_CONVERT
! 
! #include "m_fixed.h"
! #define ANGLE_TO_FLOAT(ang) ((float)((ang) * 180. / ANGLE_180))
! #define FLOAT_TO_ANGLE(ang) xs_RoundToUInt((ang) / 180. * ANGLE_180)
! 
  #endif
\ No newline at end of file
