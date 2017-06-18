*** doom/src/main/jni/gzdoom/src/gl/system/gl_system.h	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/system/gl_system.h	2017-06-18 23:34:13.207247391 -0400
***************
*** 67,81 ****
  #include <fcntl.h>
  
  //GL headers
- #include "gl_load.h"
- 
  #if defined(__APPLE__)
! 	#include <OpenGL/OpenGL.h>
  #endif
  
  #ifdef _WIN32
  #define DWORD WINDOWS_DWORD	// I don't want to depend on this throughout the GL code!
! //#include "gl/api/wglext.h"
  #ifndef __WINE__
  #undef DWORD
  #endif
--- 67,93 ----
  #include <fcntl.h>
  
  //GL headers
  #if defined(__APPLE__)
! #include <GL/glew.h>
! #include <OpenGL/OpenGL.h>
! #elif defined(__ANDROID__)
! #include "gl.h"
! #include "glu.h"
! #include "glext.h"
! #include "qgl_linked.h"
! #elif defined(__unix__)
! #include <GL/glew.h>
! #include "gl/api/glext.h"
! #else // !__APPLE__ && !__unix__
! #include <GL/gl.h>
! #include <GL/glu.h>
! #include "gl/api/glext.h"
  #endif
+ #include "gl/api/gl_api.h"
  
  #ifdef _WIN32
  #define DWORD WINDOWS_DWORD	// I don't want to depend on this throughout the GL code!
! #include "gl/api/wglext.h"
  #ifndef __WINE__
  #undef DWORD
  #endif
