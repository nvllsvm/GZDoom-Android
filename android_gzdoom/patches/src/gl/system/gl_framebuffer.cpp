*** doom/src/main/jni/gzdoom/src/gl/system/gl_framebuffer.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/system/gl_framebuffer.cpp	2017-06-18 23:34:13.210580755 -0400
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
