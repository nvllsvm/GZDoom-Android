*** doom/src/main/jni/gzdoom/src/gl/system/gl_interface.h	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/system/gl_interface.h	2017-06-18 23:34:13.230580940 -0400
***************
*** 5,35 ****
  
  enum RenderFlags
  {
  	// [BB] Added texture compression flags.
! 	RFL_TEXTURE_COMPRESSION=1,
! 	RFL_TEXTURE_COMPRESSION_S3TC=2,
  
! 	RFL_SHADER_STORAGE_BUFFER = 4,
! 	RFL_BUFFER_STORAGE = 8
  };
  
  enum TexMode
  {
! 	TM_MODULATE = 0,	// (r, g, b, a)
! 	TM_MASK,			// (1, 1, 1, a)
! 	TM_OPAQUE,			// (r, g, b, 1)
! 	TM_INVERSE,			// (1-r, 1-g, 1-b, a)
! 	TM_REDTOALPHA,		// (1, 1, 1, r)
  };
  
  struct RenderContext
  {
  	unsigned int flags;
  	unsigned int maxuniforms;
- 	unsigned int maxuniformblock;
- 	unsigned int uniformblockalignment;
- 	float version;
- 	float glslversion;
  	int max_texturesize;
  	char * vendorstring;
  
--- 5,48 ----
  
  enum RenderFlags
  {
+ 	RFL_NPOT_TEXTURE=1,
+ 	RFL_OCCLUSION_QUERY=4,
  	// [BB] Added texture compression flags.
! 	RFL_TEXTURE_COMPRESSION=8,
! 	RFL_TEXTURE_COMPRESSION_S3TC=16,
  
! 	RFL_VBO = 32,
! 	RFL_MAP_BUFFER_RANGE = 64,
! 	RFL_FRAMEBUFFER = 128,
! 	RFL_TEXTUREBUFFER = 256,
! 	RFL_NVIDIA = 512,
! 	RFL_ATI = 1024,
! 
! 
! 	RFL_GL_20 = 0x10000000,
! 	RFL_GL_21 = 0x20000000,
! 	RFL_GL_30 = 0x40000000,
  };
  
  enum TexMode
  {
! 	TMF_MASKBIT = 1,
! 	TMF_OPAQUEBIT = 2,
! 	TMF_INVERTBIT = 4,
! 
! 	TM_MODULATE = 0,
! 	TM_MASK = TMF_MASKBIT,
! 	TM_OPAQUE = TMF_OPAQUEBIT,
! 	TM_INVERT = TMF_INVERTBIT,
! 	//TM_INVERTMASK = TMF_MASKBIT | TMF_INVERTBIT
! 	TM_INVERTOPAQUE = TMF_INVERTBIT | TMF_OPAQUEBIT,
  };
  
  struct RenderContext
  {
  	unsigned int flags;
+ 	unsigned int shadermodel;
  	unsigned int maxuniforms;
  	int max_texturesize;
  	char * vendorstring;
  
