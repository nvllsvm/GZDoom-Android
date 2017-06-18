*** /home/draje/Code/gzdoom/src/gl/hqnx/mystdint.h	2017-06-18 07:52:50.693013627 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/gl/hqnx/mystdint.h	2017-06-18 09:09:24.168397311 -0400
***************
*** 1,19 ****
! #ifndef __MYSTDINT_H
! #define __MYSTDINT_H
! 
! #ifndef _MSC_VER
! #include <stdint.h>
! #else
! typedef unsigned __int64 uint64_t;
! typedef signed __int64 int64_t;
! typedef unsigned __int32 uint32_t;
! typedef signed __int32 int32_t;
! typedef unsigned __int16 uint16_t;
! typedef signed __int16 int16_t;
! typedef unsigned __int8 uint8_t;
! typedef signed __int8 int8_t;
! #endif
! 
! 
! 
  #endif
\ No newline at end of file
--- 1,19 ----
! #ifndef __MYSTDINT_H
! #define __MYSTDINT_H
! 
! #ifndef _MSC_VER
! #include <stdint.h>
! #else
! typedef unsigned __int64 uint64_t;
! typedef signed __int64 int64_t;
! typedef unsigned __int32 uint32_t;
! typedef signed __int32 int32_t;
! typedef unsigned __int16 uint16_t;
! typedef signed __int16 int16_t;
! typedef unsigned __int8 uint8_t;
! typedef signed __int8 int8_t;
! #endif
! 
! 
! 
  #endif
\ No newline at end of file
