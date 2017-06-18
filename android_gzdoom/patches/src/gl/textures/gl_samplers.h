*** /home/draje/Code/gzdoom/src/gl/textures/gl_samplers.h	2017-06-18 07:52:50.696346991 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/gl/textures/gl_samplers.h	2017-06-18 09:09:24.168397311 -0400
***************
*** 1,28 ****
! #ifndef __GL_SAMPLERS_H
! #define __GL_SAMPLERS_H
! 
! #include "gl_hwtexture.h"
! 
! class FSamplerManager
! {
! 	// We need 6 different samplers: 4 for the different clamping modes,
! 	// one for 2D-textures and one for voxel textures
! 	unsigned int mSamplers[6];
! 	unsigned int mLastBound[FHardwareTexture::MAX_TEXTURES];
! 
! 	void UnbindAll();
! 
! public:
! 
! 	FSamplerManager();
! 	~FSamplerManager();
! 
! 	void Bind(int texunit, int num);
! 	void SetTextureFilterMode();
! 
! 
! };
! 
! 
! #endif
! 
--- 1,28 ----
! #ifndef __GL_SAMPLERS_H
! #define __GL_SAMPLERS_H
! 
! #include "gl_hwtexture.h"
! 
! class FSamplerManager
! {
! 	// We need 6 different samplers: 4 for the different clamping modes,
! 	// one for 2D-textures and one for voxel textures
! 	unsigned int mSamplers[7];
! 	unsigned int mLastBound[FHardwareTexture::MAX_TEXTURES];
! 
! 	void UnbindAll();
! 
! public:
! 
! 	FSamplerManager();
! 	~FSamplerManager();
! 
! 	void Bind(int texunit, int num);
! 	void SetTextureFilterMode();
! 
! 
! };
! 
! 
! #endif
! 
