*** doom/src/main/jni/gzdoom/src/gl/textures/gl_bitmap.h	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/textures/gl_bitmap.h	2017-06-18 23:34:13.267247946 -0400
***************
*** 5,27 ****
  #include "gl/textures/gl_material.h"
  
  
  class FGLBitmap : public FBitmap
  {
  	int translation;
  public:
  
! 	FGLBitmap()
! 	{
! 		translation = 0;
! 	}
  	FGLBitmap(BYTE *buffer, int pitch, int width, int height)
  		: FBitmap(buffer, pitch, width, height)
! 	{
! 		translation = 0;
! 	}
  
! 	void SetTranslationInfo(int _trans)
  	{
  		if (_trans != -1337) translation = _trans;
  
  	}
--- 5,26 ----
  #include "gl/textures/gl_material.h"
  
  
+ void ModifyPalette(PalEntry * pout, PalEntry * pin, int cm, int count);
+ 
  class FGLBitmap : public FBitmap
  {
+ 	int cm;
  	int translation;
  public:
  
! 	FGLBitmap() { cm = CM_DEFAULT; translation = 0; }
  	FGLBitmap(BYTE *buffer, int pitch, int width, int height)
  		: FBitmap(buffer, pitch, width, height)
! 	{ cm = CM_DEFAULT; translation = 0; }
  
! 	void SetTranslationInfo(int _cm, int _trans=-1337)
  	{
+ 		if (_cm != -1) cm = _cm;
  		if (_trans != -1337) translation = _trans;
  
  	}
