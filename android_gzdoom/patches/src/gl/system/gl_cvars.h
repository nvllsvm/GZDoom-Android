*** doom/src/main/jni/gzdoom/src/gl/system/gl_cvars.h	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/system/gl_cvars.h	2017-06-18 23:34:13.217247482 -0400
***************
*** 10,15 ****
--- 10,21 ----
  #pragma warning(disable:4244)
  #endif
  
+ EXTERN_CVAR(Bool, gl_warp_shader)
+ EXTERN_CVAR(Bool, gl_fog_shader)
+ EXTERN_CVAR(Bool, gl_colormap_shader)
+ EXTERN_CVAR(Bool, gl_brightmap_shader)
+ EXTERN_CVAR(Bool, gl_glow_shader)
+ 
  EXTERN_CVAR(Bool,gl_enhanced_nightvision)
  EXTERN_CVAR(Int, screenblocks);
  EXTERN_CVAR(Bool, gl_texture)
***************
*** 21,26 ****
--- 27,34 ----
  
  EXTERN_CVAR(Int, gl_weaponlight)
  
+ EXTERN_CVAR(Bool, gl_forcemultipass)
+ 
  EXTERN_CVAR (Bool, gl_lights);
  EXTERN_CVAR (Bool, gl_attachedlights);
  EXTERN_CVAR (Bool, gl_lights_checkside);
***************
*** 36,44 ****
--- 44,58 ----
  
  EXTERN_CVAR(Bool,gl_mirrors)
  EXTERN_CVAR(Bool,gl_mirror_envmap)
+ EXTERN_CVAR(Bool, gl_render_segs)
  EXTERN_CVAR(Bool, gl_seamless)
+ EXTERN_CVAR(Bool, gl_dynlight_shader)
  
  EXTERN_CVAR(Float, gl_mask_threshold)
  EXTERN_CVAR(Float, gl_mask_sprite_threshold)
  
+ EXTERN_CVAR(Int, gl_usevbo)
+ 
+ 
+ 
  #endif // _GL_INTERN_H
