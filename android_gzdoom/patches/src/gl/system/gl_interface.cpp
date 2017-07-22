*** doom/src/main/jni/gzdoom/src/gl/system/gl_interface.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/system/gl_interface.cpp	2017-06-18 23:34:13.213914119 -0400
***************
*** 46,57 ****
--- 46,75 ----
  #include "gl/system/gl_interface.h"
  #include "gl/system/gl_cvars.h"
  
+ #if defined (__unix__) || defined (__APPLE__)
+ #define PROC void*
+ #define LPCSTR const char*
+ 
+ #include <SDL.h>
+ #define wglGetProcAddress(x) (*SDL_GL_GetProcAddress)(x)
+ #endif
+ static void APIENTRY glBlendEquationDummy (GLenum mode);
+ 
+ 
  static TArray<FString>  m_Extensions;
  
  RenderContext gl;
  
  int occlusion_type=0;
  
+ PROC myGetProcAddress(LPCSTR proc)
+ {
+ 	PROC p = wglGetProcAddress(proc);
+ 	if (p == NULL) I_Error("Fatal: GL function '%s' not found.", proc);
+ 	return p;
+ }
+ 
+ 
  //==========================================================================
  //
  // 
***************
*** 60,74 ****
  
  static void CollectExtensions()
  {
! 	const char *extension;
  
! 	int max = 0;
! 	glGetIntegerv(GL_NUM_EXTENSIONS, &max);
  
! 	for(int i = 0; i < max; i++)
  	{
! 		extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
! 		m_Extensions.Push(FString(extension));
  	}
  }
  
--- 78,101 ----
  
  static void CollectExtensions()
  {
! 	const char *supported = NULL;
! 	char *extensions, *extension;
  
! 	supported = (char *)glGetString(GL_EXTENSIONS);
  
! 	if (supported)
  	{
! 		extensions = new char[strlen(supported) + 1];
! 		strcpy(extensions, supported);
! 
! 		extension = strtok(extensions, " ");
! 		while(extension)
! 		{
! 			m_Extensions.Push(FString(extension));
! 			extension = strtok(NULL, " ");
! 		}
! 
! 		delete [] extensions;
  	}
  }
  
***************
*** 99,152 ****
  static void InitContext()
  {
  	gl.flags=0;
  }
  
  //==========================================================================
  //
  // 
  //
  //==========================================================================
! 
  void gl_LoadExtensions()
  {
  	InitContext();
  	CollectExtensions();
  
! 	const char *version = Args->CheckValue("-glversion");
! 	if (version == NULL) version = (const char*)glGetString(GL_VERSION);
! 	else Printf("Emulating OpenGL v %s\n", version);
  
! 	// Don't even start if it's lower than 3.0
! 	if (strcmp(version, "3.0") < 0)
  	{
! 		I_FatalError("Unsupported OpenGL version.\nAt least OpenGL 3.0 is required to run " GAMENAME ".\n");
  	}
  
! 	// add 0.01 to account for roundoff errors making the number a tad smaller than the actual version
! 	gl.version = strtod(version, NULL) + 0.01f;
! 	gl.glslversion = strtod((char*)glGetString(GL_SHADING_LANGUAGE_VERSION), NULL) + 0.01f;
  
! 	gl.vendorstring = (char*)glGetString(GL_VENDOR);
  
  	if (CheckExtension("GL_ARB_texture_compression")) gl.flags|=RFL_TEXTURE_COMPRESSION;
  	if (CheckExtension("GL_EXT_texture_compression_s3tc")) gl.flags|=RFL_TEXTURE_COMPRESSION_S3TC;
! 	if (!Args->CheckParm("-gl3"))
! 	{
! 		// don't use GL 4.x features when running in GL 3 emulation mode.
! 		if (CheckExtension("GL_ARB_shader_storage_buffer_object")) gl.flags |= RFL_SHADER_STORAGE_BUFFER;
! 		if (CheckExtension("GL_ARB_buffer_storage")) gl.flags |= RFL_BUFFER_STORAGE;
! 	}
! 	
! 	int v;
! 	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &v);
! 	gl.maxuniforms = v;
! 	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &v);
! 	gl.maxuniformblock = v;
! 	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &v);
! 	gl.uniformblockalignment = v;
! 	
  	glGetIntegerv(GL_MAX_TEXTURE_SIZE,&gl.max_texturesize);
  	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  }
  
  //==========================================================================
--- 126,369 ----
  static void InitContext()
  {
  	gl.flags=0;
+ 	glBlendEquation = glBlendEquationDummy;
  }
  
+ 
  //==========================================================================
  //
  // 
  //
  //==========================================================================
! extern "C"
! {
! 	#include "GL/nano_gl.h"
! }
  void gl_LoadExtensions()
  {
+ 
+ #ifdef __ANDROID__
+ 	LOGI("gl_LoadExtensions");
+ 	QGL_Init( 0 );
+ #endif
  	InitContext();
  	CollectExtensions();
  
! 	const char *version = (const char*)glGetString(GL_VERSION);
  
! 	// Don't even start if it's lower than 1.3
! 	if (strcmp(version, "1.3") < 0) 
! 	{
! 		I_FatalError("Unsupported OpenGL version.\nAt least GL 1.3 is required to run " GAMENAME ".\n");
! 	}
! 	else if (strcmp(version, "1.4") < 0) 
  	{
! 		// The engine will still assume 1.4 but the only 1.4 feature being used is GL_GENERATE_MIPMAP which should be supported as an extension
! 		// on most 1.3 cards this is present but let's print a warning that not everything may work as intended.
! 		Printf(TEXTCOLOR_RED "The current graphics driver implements a OpenGL version lower than 1.4 and may not support all features " GAMENAME " requires.\n");
  	}
  
! 	// This loads any function pointers and flags that require a vaild render context to
! 	// initialize properly
! 
! 	gl.shadermodel = 0;	// assume no shader support
! 	gl.vendorstring=(char*)glGetString(GL_VENDOR);
! 
! #ifdef __ANDROID__
! 	gl.flags = 0;
! 
! 
! 	//glBindBuffer			= nanoglBindBuffer;
! 	//glDeleteBuffers			= nanoglDeleteBuffers;
! 	//glGenBuffers			= nanoglGenBuffers;
! 	//glBufferData			= nanoglBufferData;
! 	//glBufferSubData			= nanoglBufferSubData;
! 	//glMapBuffer				= nanoglMapBuffer;
! 	//glUnmapBuffer			= nanoglUnmapBuffer;
! 
! 	//gl.flags |= RFL_VBO;
! 
! 	//gl.flags|=RFL_NPOT_TEXTURE;
! 	if (CheckExtension("GL_ARB_texture_non_power_of_two")) gl.flags|=RFL_NPOT_TEXTURE;
! 	qglGetIntegerv(GL_MAX_TEXTURE_SIZE,&gl.max_texturesize);
! 	qglPixelStorei(GL_UNPACK_ALIGNMENT, 1);
! 
! 	glDisable(GL_CLIP_PLANE0);
! 	glEnable(GL_CLIP_PLANE0);
! 
! 	return;
! #endif
  
! 	// First try the regular function
! 	glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation");
! 	// If that fails try the EXT version
! 	if (!glBlendEquation) glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquationEXT");
! 	// If that fails use a no-op dummy
! 	if (!glBlendEquation) glBlendEquation = glBlendEquationDummy;
  
+ 	if (CheckExtension("GL_ARB_texture_non_power_of_two")) gl.flags|=RFL_NPOT_TEXTURE;
  	if (CheckExtension("GL_ARB_texture_compression")) gl.flags|=RFL_TEXTURE_COMPRESSION;
  	if (CheckExtension("GL_EXT_texture_compression_s3tc")) gl.flags|=RFL_TEXTURE_COMPRESSION_S3TC;
! 	if (strstr(gl.vendorstring, "NVIDIA")) gl.flags|=RFL_NVIDIA;
! 	else if (strstr(gl.vendorstring, "ATI Technologies")) gl.flags|=RFL_ATI;
! 
! 	if (strcmp(version, "2.0") >= 0) gl.flags|=RFL_GL_20;
! 	if (strcmp(version, "2.1") >= 0) gl.flags|=RFL_GL_21;
! 	if (strcmp(version, "3.0") >= 0) gl.flags|=RFL_GL_30;
! 
  	glGetIntegerv(GL_MAX_TEXTURE_SIZE,&gl.max_texturesize);
  	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
+ 
+ 	if (gl.flags & RFL_GL_20)
+ 	{
+ 		glDeleteShader = (PFNGLDELETESHADERPROC)myGetProcAddress("glDeleteShader");
+ 		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)myGetProcAddress("glDeleteProgram");
+ 		glDetachShader = (PFNGLDETACHSHADERPROC)myGetProcAddress("glDetachShader");
+ 		glCreateShader = (PFNGLCREATESHADERPROC)myGetProcAddress("glCreateShader");
+ 		glShaderSource = (PFNGLSHADERSOURCEPROC)myGetProcAddress("glShaderSource");
+ 		glCompileShader = (PFNGLCOMPILESHADERPROC)myGetProcAddress("glCompileShader");
+ 		glCreateProgram = (PFNGLCREATEPROGRAMPROC)myGetProcAddress("glCreateProgram");
+ 		glAttachShader = (PFNGLATTACHSHADERPROC)myGetProcAddress("glAttachShader");
+ 		glLinkProgram = (PFNGLLINKPROGRAMPROC)myGetProcAddress("glLinkProgram");
+ 		glUseProgram = (PFNGLUSEPROGRAMPROC)myGetProcAddress("glUseProgram");
+ 		glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)myGetProcAddress("glValidateProgram");
+ 
+ 		glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)myGetProcAddress("glVertexAttrib1f");
+ 		glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)myGetProcAddress("glVertexAttrib2f");
+ 		glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)myGetProcAddress("glVertexAttrib4f");
+ 		glVertexAttrib2fv = (PFNGLVERTEXATTRIB4FVPROC)myGetProcAddress("glVertexAttrib2fv");
+ 		glVertexAttrib3fv = (PFNGLVERTEXATTRIB4FVPROC)myGetProcAddress("glVertexAttrib3fv");
+ 		glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)myGetProcAddress("glVertexAttrib4fv");
+ 		glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC)myGetProcAddress("glVertexAttrib4ubv");
+ 		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)myGetProcAddress("glGetAttribLocation");
+ 		glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)myGetProcAddress("glBindAttribLocation");
+ 
+ 
+ 		glUniform1f = (PFNGLUNIFORM1FPROC)myGetProcAddress("glUniform1f");
+ 		glUniform2f = (PFNGLUNIFORM2FPROC)myGetProcAddress("glUniform2f");
+ 		glUniform3f = (PFNGLUNIFORM3FPROC)myGetProcAddress("glUniform3f");
+ 		glUniform4f = (PFNGLUNIFORM4FPROC)myGetProcAddress("glUniform4f");
+ 		glUniform1i = (PFNGLUNIFORM1IPROC)myGetProcAddress("glUniform1i");
+ 		glUniform2i = (PFNGLUNIFORM2IPROC)myGetProcAddress("glUniform2i");
+ 		glUniform3i = (PFNGLUNIFORM3IPROC)myGetProcAddress("glUniform3i");
+ 		glUniform4i = (PFNGLUNIFORM4IPROC)myGetProcAddress("glUniform4i");
+ 		glUniform1fv = (PFNGLUNIFORM1FVPROC)myGetProcAddress("glUniform1fv");
+ 		glUniform2fv = (PFNGLUNIFORM2FVPROC)myGetProcAddress("glUniform2fv");
+ 		glUniform3fv = (PFNGLUNIFORM3FVPROC)myGetProcAddress("glUniform3fv");
+ 		glUniform4fv = (PFNGLUNIFORM4FVPROC)myGetProcAddress("glUniform4fv");
+ 		glUniform1iv = (PFNGLUNIFORM1IVPROC)myGetProcAddress("glUniform1iv");
+ 		glUniform2iv = (PFNGLUNIFORM2IVPROC)myGetProcAddress("glUniform2iv");
+ 		glUniform3iv = (PFNGLUNIFORM3IVPROC)myGetProcAddress("glUniform3iv");
+ 		glUniform4iv = (PFNGLUNIFORM4IVPROC)myGetProcAddress("glUniform4iv");
+ 
+ 		glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)myGetProcAddress("glUniformMatrix2fv");
+ 		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)myGetProcAddress("glUniformMatrix3fv");
+ 		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)myGetProcAddress("glUniformMatrix4fv");
+ 
+ 		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)myGetProcAddress("glGetProgramInfoLog");
+ 		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)myGetProcAddress("glGetShaderInfoLog");
+ 		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)myGetProcAddress("glGetUniformLocation");
+ 		glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)myGetProcAddress("glGetActiveUniform");
+ 		glGetUniformfv = (PFNGLGETUNIFORMFVPROC)myGetProcAddress("glGetUniformfv");
+ 		glGetUniformiv = (PFNGLGETUNIFORMIVPROC)myGetProcAddress("glGetUniformiv");
+ 		glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)myGetProcAddress("glGetShaderSource");
+ 
+ 		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)myGetProcAddress("glEnableVertexAttribArray");
+ 		glDisableVertexAttribArray= (PFNGLDISABLEVERTEXATTRIBARRAYPROC)myGetProcAddress("glDisableVertexAttribArray");
+ 		glVertexAttribPointer		= (PFNGLVERTEXATTRIBPOINTERPROC)myGetProcAddress("glVertexAttribPointer");
+ 
+ 		// what'S the equivalent of this in GL 2.0???
+ 		glGetObjectParameteriv = (PFNGLGETOBJECTPARAMETERIVARBPROC)myGetProcAddress("glGetObjectParameterivARB");
+ 
+ 		// Rules:
+ 		// SM4 will always use shaders. No option to switch them off is needed here.
+ 		// SM3 has shaders optional but they are off by default (they will have a performance impact
+ 		// SM2 only uses shaders for colormaps on camera textures and has no option to use them in general.
+ 		//     On SM2 cards the shaders will be too slow and show visual bugs (at least on GF 6800.)
+ 		if (strcmp((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION), "1.3") >= 0) gl.shadermodel = 4;
+ 		else if (CheckExtension("GL_NV_GPU_shader4")) gl.shadermodel = 4;	// for pre-3.0 drivers that support GF8xxx.
+ 		else if (CheckExtension("GL_EXT_GPU_shader4")) gl.shadermodel = 4;	// for pre-3.0 drivers that support GF8xxx.
+ 		else if (CheckExtension("GL_NV_vertex_program3")) gl.shadermodel = 3;
+ 		else if (!strstr(gl.vendorstring, "NVIDIA")) gl.shadermodel = 3;
+ 		else gl.shadermodel = 2;	// Only for older NVidia cards which had notoriously bad shader support.
+ 
+ 		// Command line overrides for testing and problem cases.
+ 		if (Args->CheckParm("-sm2") && gl.shadermodel > 2) gl.shadermodel = 2;
+ 		else if (Args->CheckParm("-sm3") && gl.shadermodel > 3) gl.shadermodel = 3;
+ 	}
+ 
+ 	if (CheckExtension("GL_ARB_occlusion_query"))
+ 	{
+ 		glGenQueries         = (PFNGLGENQUERIESARBPROC)myGetProcAddress("glGenQueriesARB");
+ 		glDeleteQueries      = (PFNGLDELETEQUERIESARBPROC)myGetProcAddress("glDeleteQueriesARB");
+ 		glGetQueryObjectuiv  = (PFNGLGETQUERYOBJECTUIVARBPROC)myGetProcAddress("glGetQueryObjectuivARB");
+ 		glBeginQuery         = (PFNGLBEGINQUERYARBPROC)myGetProcAddress("glBeginQueryARB");
+ 		glEndQuery           = (PFNGLENDQUERYPROC)myGetProcAddress("glEndQueryARB");
+ 		gl.flags|=RFL_OCCLUSION_QUERY;
+ 	}
+ 
+ 	if (gl.flags & RFL_GL_21)
+ 	{
+ 		glBindBuffer			= (PFNGLBINDBUFFERPROC)myGetProcAddress("glBindBuffer");
+ 		glDeleteBuffers			= (PFNGLDELETEBUFFERSPROC)myGetProcAddress("glDeleteBuffers");
+ 		glGenBuffers			= (PFNGLGENBUFFERSPROC)myGetProcAddress("glGenBuffers");
+ 		glBufferData			= (PFNGLBUFFERDATAPROC)myGetProcAddress("glBufferData");
+ 		glBufferSubData			= (PFNGLBUFFERSUBDATAPROC)myGetProcAddress("glBufferSubData");
+ 		glMapBuffer				= (PFNGLMAPBUFFERPROC)myGetProcAddress("glMapBuffer");
+ 		glUnmapBuffer			= (PFNGLUNMAPBUFFERPROC)myGetProcAddress("glUnmapBuffer");
+ 		gl.flags |= RFL_VBO;
+ 	}
+ 	else if (CheckExtension("GL_ARB_vertex_buffer_object"))
+ 	{
+ 		glBindBuffer			= (PFNGLBINDBUFFERPROC)myGetProcAddress("glBindBufferARB");
+ 		glDeleteBuffers			= (PFNGLDELETEBUFFERSPROC)myGetProcAddress("glDeleteBuffersARB");
+ 		glGenBuffers			= (PFNGLGENBUFFERSPROC)myGetProcAddress("glGenBuffersARB");
+ 		glBufferData			= (PFNGLBUFFERDATAPROC)myGetProcAddress("glBufferDataARB");
+ 		glBufferSubData			= (PFNGLBUFFERSUBDATAPROC)myGetProcAddress("glBufferSubDataARB");
+ 		glMapBuffer				= (PFNGLMAPBUFFERPROC)myGetProcAddress("glMapBufferARB");
+ 		glUnmapBuffer			= (PFNGLUNMAPBUFFERPROC)myGetProcAddress("glUnmapBufferARB");
+ 		gl.flags |= RFL_VBO;
+ 	}
+ 
+ 	if (CheckExtension("GL_ARB_map_buffer_range")) 
+ 	{
+ 		glMapBufferRange			= (PFNGLMAPBUFFERRANGEPROC)myGetProcAddress("glMapBufferRange");
+ 		glFlushMappedBufferRange	= (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)myGetProcAddress("glFlushMappedBufferRange");
+ 		gl.flags|=RFL_MAP_BUFFER_RANGE;
+ 	}
+ 
+ 	if (CheckExtension("GL_ARB_framebuffer_object"))
+ 	{
+ 		glGenFramebuffers			= (PFNGLGENFRAMEBUFFERSPROC)myGetProcAddress("glGenFramebuffers");
+ 		glDeleteFramebuffers		= (PFNGLDELETEFRAMEBUFFERSPROC)myGetProcAddress("glDeleteFramebuffers");
+ 		glBindFramebuffer			= (PFNGLBINDFRAMEBUFFERPROC)myGetProcAddress("glBindFramebuffer");
+ 		glFramebufferTexture2D		= (PFNGLFRAMEBUFFERTEXTURE2DPROC)myGetProcAddress("glFramebufferTexture2D");
+ 		glGenRenderbuffers			= (PFNGLGENRENDERBUFFERSPROC)myGetProcAddress("glGenRenderbuffers");
+ 		glDeleteRenderbuffers		= (PFNGLDELETERENDERBUFFERSPROC)myGetProcAddress("glDeleteRenderbuffers");
+ 		glBindRenderbuffer			= (PFNGLBINDRENDERBUFFERPROC)myGetProcAddress("glBindRenderbuffer");
+ 		glRenderbufferStorage		= (PFNGLRENDERBUFFERSTORAGEPROC)myGetProcAddress("glRenderbufferStorage");
+ 		glFramebufferRenderbuffer	= (PFNGLFRAMEBUFFERRENDERBUFFERPROC)myGetProcAddress("glFramebufferRenderbuffer");
+ 
+ 		gl.flags|=RFL_FRAMEBUFFER;
+ 	}
+ 
+ #if 0
+ 	if (CheckExtension("GL_ARB_texture_buffer_object") && 
+ 			CheckExtension("GL_ARB_texture_float") &&
+ 			CheckExtension("GL_EXT_GPU_Shader4") &&
+ 			CheckExtension("GL_ARB_texture_rg") &&
+ 			gl.shadermodel == 4)
+ 	{
+ 		glTexBufferARB = (PFNGLTEXBUFFERARBPROC)myGetProcAddress("glTexBufferARB");
+ 		gl.flags|=RFL_TEXTUREBUFFER;
+ 	}
+ #endif
+ 
+ 
+ 
+ 	glActiveTexture = (PFNGLACTIVETEXTUREPROC)myGetProcAddress("glActiveTextureARB");
+ 	glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC) myGetProcAddress("glMultiTexCoord2fARB");
+ 	glMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC) myGetProcAddress("glMultiTexCoord2fvARB");
  }
  
  //==========================================================================
***************
*** 157,195 ****
  
  void gl_PrintStartupLog()
  {
- 	int v;
- 	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &v);
- 
  	Printf ("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
  	Printf ("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
! 	Printf ("GL_VERSION: %s (%s profile)\n", glGetString(GL_VERSION), (v & GL_CONTEXT_CORE_PROFILE_BIT)? "Core" : "Compatibility");
  	Printf ("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
! 	Printf ("GL_EXTENSIONS:");
! 	for (unsigned i = 0; i < m_Extensions.Size(); i++)
! 	{
! 		Printf(" %s", m_Extensions[i].GetChars());
! 	}
  
- 	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &v);
- 	Printf("\nMax. texture size: %d\n", v);
  	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &v);
  	Printf ("Max. texture units: %d\n", v);
  	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &v);
  	Printf ("Max. fragment uniforms: %d\n", v);
  	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &v);
  	Printf ("Max. vertex uniforms: %d\n", v);
- 	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &v);
- 	Printf ("Max. uniform block size: %d\n", v);
- 	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &v);
- 	Printf ("Uniform block alignment: %d\n", v);
- 
  	glGetIntegerv(GL_MAX_VARYING_FLOATS, &v);
  	Printf ("Max. varying: %d\n", v);
! 	glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &v);
! 	Printf("Max. combined shader storage blocks: %d\n", v);
! 	glGetIntegerv(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, &v);
! 	Printf("Max. vertex shader storage blocks: %d\n", v);
  
  
  }
  
--- 374,500 ----
  
  void gl_PrintStartupLog()
  {
  	Printf ("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
  	Printf ("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
! 	Printf ("GL_VERSION: %s\n", glGetString(GL_VERSION));
  	Printf ("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
! 	Printf ("GL_EXTENSIONS: %s\n", glGetString(GL_EXTENSIONS));
! 	int v;
  
  	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &v);
  	Printf ("Max. texture units: %d\n", v);
  	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &v);
  	Printf ("Max. fragment uniforms: %d\n", v);
+ 	if (gl.shadermodel == 4) gl.maxuniforms = v;
  	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &v);
  	Printf ("Max. vertex uniforms: %d\n", v);
  	glGetIntegerv(GL_MAX_VARYING_FLOATS, &v);
  	Printf ("Max. varying: %d\n", v);
! 	glGetIntegerv(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, &v);
! 	Printf ("Max. combined uniforms: %d\n", v);
! 	glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &v);
! 	Printf ("Max. combined uniform blocks: %d\n", v);
  
+ }
  
+ //==========================================================================
+ //
+ // 
+ //
+ //==========================================================================
+ 
+ static void APIENTRY glBlendEquationDummy (GLenum mode)
+ {
+ 	// If this is not supported all non-existent modes are
+ 	// made to draw nothing.
+ 	if (mode == GL_FUNC_ADD)
+ 	{
+ 		glColorMask(true, true, true, true);
+ 	}
+ 	else
+ 	{
+ 		glColorMask(false, false, false, false);
+ 	}
+ }
+ 
+ //==========================================================================
+ //
+ // 
+ //
+ //==========================================================================
+ #ifdef __ANDROID__
+ #define GL_SOURCE0_RGB                                      GL_SRC0_RGB
+ #define GL_SOURCE1_RGB                                      GL_SRC1_RGB
+ #define GL_SOURCE2_RGB                                      GL_SRC2_RGB
+ #define GL_SOURCE0_ALPHA                                    GL_SRC0_ALPHA
+ #define GL_SOURCE1_ALPHA                                    GL_SRC1_ALPHA
+ #define GL_SOURCE2_ALPHA                                    GL_SRC2_ALPHA
+ #endif
+ void gl_SetTextureMode(int type)
+ {
+ 	static float white[] = {1.f,1.f,1.f,1.f};
+ 
+ 	if (type == TM_MASK)
+ 	{
+ #ifdef __ANDROID__
+ 		return; //Causes textures to screw up for some reason...
+ #endif
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PRIMARY_COLOR);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
+ 
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE); 
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PRIMARY_COLOR);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE0);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
+ 	}
+ 	else if (type == TM_OPAQUE)
+ 	{
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE0);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
+ 
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE); 
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PRIMARY_COLOR);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
+ 	}
+ 	else if (type == TM_INVERT)
+ 	{
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE0);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_ONE_MINUS_SRC_COLOR);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
+ 
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE); 
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PRIMARY_COLOR);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE0);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
+ 	}
+ 	else if (type == TM_INVERTOPAQUE)
+ 	{
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE0);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_ONE_MINUS_SRC_COLOR);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
+ 
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE); 
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PRIMARY_COLOR);
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
+ 	}
+ 	else // if (type == TM_MODULATE)
+ 	{
+ 		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
+ 	}
  }
  
+ //} // extern "C"
