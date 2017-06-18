*** doom/src/main/jni/gzdoom/src/gl/renderer/gl_lightdata.h	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/renderer/gl_lightdata.h	2017-06-18 23:34:13.280581402 -0400
***************
*** 5,10 ****
--- 5,13 ----
  #include "r_data/renderstyle.h"
  #include "gl/renderer/gl_colormap.h"
  
+ bool gl_BrightmapsActive();
+ bool gl_GlowActive();
+ 
  inline int gl_ClampLight(int lightlevel)
  {
  	return clamp(lightlevel, 0, 255);
***************
*** 15,21 ****
  void gl_SetFogParams(int _fogdensity, PalEntry _outsidefogcolor, int _outsidefogdensity, int _skyfog);
  
  int gl_CalcLightLevel(int lightlevel, int rellight, bool weapon);
! void gl_SetColor(int light, int rellight, const FColormap &cm, float alpha, bool weapon=false);
  
  float gl_GetFogDensity(int lightlevel, PalEntry fogcolor);
  struct sector_t;
--- 18,27 ----
  void gl_SetFogParams(int _fogdensity, PalEntry _outsidefogcolor, int _outsidefogdensity, int _skyfog);
  
  int gl_CalcLightLevel(int lightlevel, int rellight, bool weapon);
! PalEntry gl_CalcLightColor(int light, PalEntry pe, int blendfactor, bool force = false);
! void gl_GetLightColor(int lightlevel, int rellight, const FColormap * cm, float * pred, float * pgreen, float * pblue, bool weapon=false);
! void gl_SetColor(int light, int rellight, const FColormap * cm, float alpha, PalEntry ThingColor = 0xffffff, bool weapon=false);
! void gl_SetColor(int light, int rellight, const FColormap * cm, float *red, float *green, float *blue, PalEntry ThingColor=0xffffff, bool weapon=false);
  
  float gl_GetFogDensity(int lightlevel, PalEntry fogcolor);
  struct sector_t;
***************
*** 41,46 ****
--- 47,60 ----
  	return gl_fixedcolormap || (gl_isWhite(color) && lightlevel==255);
  }
  
+ __forceinline void gl_Desaturate(int gray, int ired, int igreen, int iblue, BYTE & red, BYTE & green, BYTE & blue, int fac)
+ {
+ 	red = (ired*(31-fac) + gray*fac)/31;
+ 	green = (igreen*(31-fac) + gray*fac)/31;
+ 	blue = (iblue*(31-fac) + gray*fac)/31;
+ }
+ 
+ void gl_ModifyColor(BYTE & red, BYTE & green, BYTE & blue, int cm);
  void gl_DeleteAllAttachedLights();
  void gl_RecreateAllAttachedLights();
  
