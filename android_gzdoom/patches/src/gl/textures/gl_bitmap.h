*** /home/draje/Code/gzdoom/src/gl/textures/gl_bitmap.h	2017-06-18 07:52:50.696346991 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/gl/textures/gl_bitmap.h	2017-06-18 09:09:24.168397311 -0400
***************
*** 1,35 ****
! #ifndef __GL_BITMAP_H
! #define __GL_BITMAP_H
! 
! #include "textures/bitmap.h"
! #include "gl/textures/gl_material.h"
! 
! 
! class FGLBitmap : public FBitmap
! {
! 	int translation;
! public:
! 
! 	FGLBitmap()
! 	{
! 		translation = 0;
! 	}
! 	FGLBitmap(BYTE *buffer, int pitch, int width, int height)
! 		: FBitmap(buffer, pitch, width, height)
! 	{
! 		translation = 0;
! 	}
! 
! 	void SetTranslationInfo(int _trans)
! 	{
! 		if (_trans != -1337) translation = _trans;
! 
! 	}
! 
! 	virtual void CopyPixelDataRGB(int originx, int originy, const BYTE *patch, int srcwidth, 
! 								int srcheight, int step_x, int step_y, int rotate, int ct, FCopyInfo *inf = NULL);
! 	virtual void CopyPixelData(int originx, int originy, const BYTE * patch, int srcwidth, int srcheight, 
! 								int step_x, int step_y, int rotate, PalEntry * palette, FCopyInfo *inf = NULL);
! };
! 
! #endif
--- 1,35 ----
! #ifndef __GL_BITMAP_H
! #define __GL_BITMAP_H
! 
! #include "textures/bitmap.h"
! #include "gl/textures/gl_material.h"
! 
! 
! class FGLBitmap : public FBitmap
! {
! 	int translation;
! public:
! 
! 	FGLBitmap()
! 	{
! 		translation = 0;
! 	}
! 	FGLBitmap(BYTE *buffer, int pitch, int width, int height)
! 		: FBitmap(buffer, pitch, width, height)
! 	{
! 		translation = 0;
! 	}
! 
! 	void SetTranslationInfo(int _trans)
! 	{
! 		if (_trans != -1337) translation = _trans;
! 
! 	}
! 
! 	virtual void CopyPixelDataRGB(int originx, int originy, const BYTE *patch, int srcwidth, 
! 								int srcheight, int step_x, int step_y, int rotate, int ct, FCopyInfo *inf = NULL);
! 	virtual void CopyPixelData(int originx, int originy, const BYTE * patch, int srcwidth, int srcheight, 
! 								int step_x, int step_y, int rotate, PalEntry * palette, FCopyInfo *inf = NULL);
! };
! 
! #endif
