*** doom/src/main/jni/gzdoom/src/gl/textures/gl_texture.h	2017-06-18 23:15:10.669973853 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/textures/gl_texture.h	2017-06-18 23:34:13.237247668 -0400
***************
*** 56,61 ****
--- 56,78 ----
  	//Span **Spans;
  };
  
+ class FCloneTexture : public FTexture
+ {
+ public:
+ 	FCloneTexture (FTexture *source, int usetype);
+ 	~FCloneTexture ();
+ 
+ 	const BYTE *GetColumn (unsigned int column, const Span **spans_out);
+ 	const BYTE *GetPixels ();
+ 	void Unload ();
+ 
+ 	int CopyTrueColorPixels(FBitmap *bmp, int x, int y, int rotate, FCopyInfo *inf);
+ 	bool UseBasePalette() { return false; }
+ 
+ protected:
+ 	FTexture *SourcePic;
+ };
+ 
  
  void gl_GenerateGlobalBrightmapFromColormap();
  PalEntry averageColor(const DWORD *data, int size, fixed_t maxout);
