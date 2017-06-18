*** doom/src/main/jni/gzdoom/src/gl/textures/gl_skyboxtexture.h	2017-06-18 23:15:10.669973853 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/textures/gl_skyboxtexture.h	2017-06-18 23:34:13.253914489 -0400
***************
*** 21,27 ****
  	int CopyTrueColorPixels(FBitmap *bmp, int x, int y, int rotate, FCopyInfo *inf);
  	bool UseBasePalette();
  	void Unload ();
! 	void PrecacheGL(int cache);
  
  	void SetSize()
  	{
--- 21,27 ----
  	int CopyTrueColorPixels(FBitmap *bmp, int x, int y, int rotate, FCopyInfo *inf);
  	bool UseBasePalette();
  	void Unload ();
! 	void PrecacheGL();
  
  	void SetSize()
  	{
