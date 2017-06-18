*** /home/draje/Code/gzdoom/src/gl/dynlights/gl_glow.h	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/gl/dynlights/gl_glow.h	2017-06-18 09:09:24.168397311 -0400
***************
*** 1,10 ****
! 
! #ifndef __GL_GLOW
! #define __GL_GLOW
! 
! struct sector_t;
! 
! void gl_InitGlow(const char * lumpnm);
! int gl_CheckSpriteGlow(sector_t *sec, int lightlevel, int x, int y, int z);
! 
! #endif
--- 1,10 ----
! 
! #ifndef __GL_GLOW
! #define __GL_GLOW
! 
! struct sector_t;
! 
! void gl_InitGlow(const char * lumpnm);
! int gl_CheckSpriteGlow(sector_t *sec, int lightlevel, int x, int y, int z);
! 
! #endif
