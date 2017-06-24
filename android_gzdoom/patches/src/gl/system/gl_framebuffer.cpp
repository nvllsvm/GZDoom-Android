*** doom/src/main/jni/gzdoom/src/gl/system/gl_framebuffer.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/system/gl_framebuffer.cpp	2017-06-18 23:34:13.210580755 -0400
***************
*** 74,82 ****
  
  FGLRenderer *GLRenderer;
  
  void gl_LoadExtensions();
  void gl_PrintStartupLog();
- void gl_SetupMenu();
  
  //==========================================================================
  //
--- 73,81 ----
  
  FGLRenderer *GLRenderer;
  
+ void gl_SetupMenu();
  void gl_LoadExtensions();
  void gl_PrintStartupLog();
  
  //==========================================================================
  //
***************
*** 119,132 ****
  {
  	static bool first=true;
  
- 	if (first)
- 	{
- 		ogl_LoadFunctions();
- 	}
- 
  	gl_LoadExtensions();
  	Super::InitializeState();
- 
  	if (first)
  	{
  		first=false;
--- 118,125 ----
***************
*** 135,155 ****
  		gl_PrintStartupLog();
  #endif
  
  	}
  	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  	glClearDepth(1.0f);
  	glDepthFunc(GL_LESS);
  
  	glEnable(GL_DITHER);
  	glDisable(GL_CULL_FACE);
  	glDisable(GL_POLYGON_OFFSET_FILL);
  	glEnable(GL_POLYGON_OFFSET_LINE);
  	glEnable(GL_BLEND);
! 	glEnable(GL_DEPTH_CLAMP);
  	glDisable(GL_DEPTH_TEST);
  	glEnable(GL_TEXTURE_2D);
  	glDisable(GL_LINE_SMOOTH);
  	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
--- 128,170 ----
  		gl_PrintStartupLog();
  #endif
  
+ 		if (gl.flags&RFL_NPOT_TEXTURE)
+ 		{
+ 			Printf("Support for non power 2 textures enabled.\n");
+ 		}
+ 		if (gl.flags&RFL_OCCLUSION_QUERY)
+ 		{
+ 			Printf("Occlusion query enabled.\n");
+ 		}
  	}
  	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  	glClearDepth(1.0f);
  	glDepthFunc(GL_LESS);
+ 	glShadeModel(GL_SMOOTH);
  
  	glEnable(GL_DITHER);
+ 	glEnable(GL_ALPHA_TEST);
  	glDisable(GL_CULL_FACE);
  	glDisable(GL_POLYGON_OFFSET_FILL);
  	glEnable(GL_POLYGON_OFFSET_LINE);
  	glEnable(GL_BLEND);
! 	glEnable(GL_DEPTH_CLAMP_NV);
  	glDisable(GL_DEPTH_TEST);
  	glEnable(GL_TEXTURE_2D);
  	glDisable(GL_LINE_SMOOTH);
  	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
+ 	glAlphaFunc(GL_GEQUAL,0.5f);
+ 	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
+ 	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
+ 	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
+ 
+ 	// This was to work around a bug in some older driver. Probably doesn't make sense anymore.
+ 	glEnable(GL_FOG);
+ 	glDisable(GL_FOG);
+ 
+ 	glHint(GL_FOG_HINT, GL_FASTEST);
+ 	glFogi(GL_FOG_MODE, GL_EXP);
+ 
  
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
***************
*** 374,383 ****
  //==========================================================================
  bool OpenGLFrameBuffer::Begin2D(bool)
  {
! 	gl_RenderState.mViewMatrix.loadIdentity();
! 	gl_RenderState.mProjectionMatrix.ortho(0, GetWidth(), GetHeight(), 0, -1.0f, 1.0f);
! 	gl_RenderState.ApplyMatrices();
! 
  	glDisable(GL_DEPTH_TEST);
  
  	// Korshun: ENABLE AUTOMAP ANTIALIASING!!!
--- 389,406 ----
  //==========================================================================
  bool OpenGLFrameBuffer::Begin2D(bool)
  {
! 	glMatrixMode(GL_MODELVIEW);
! 	glLoadIdentity();
! 	glMatrixMode(GL_PROJECTION);
! 	glLoadIdentity();
! 	glOrtho(
! 		(GLdouble) 0,
! 		(GLdouble) GetWidth(), 
! 		(GLdouble) GetHeight(), 
! 		(GLdouble) 0,
! 		(GLdouble) -1.0, 
! 		(GLdouble) 1.0 
! 		);
  	glDisable(GL_DEPTH_TEST);
  
  	// Korshun: ENABLE AUTOMAP ANTIALIASING!!!
***************
*** 536,540 ****
  	ScreenshotBuffer = NULL;
  	LastCamera = NULL;
  	gl_GenerateGlobalBrightmapFromColormap();
! }
! 
--- 559,562 ----
  	ScreenshotBuffer = NULL;
  	LastCamera = NULL;
  	gl_GenerateGlobalBrightmapFromColormap();
! }
\ No newline at end of file
