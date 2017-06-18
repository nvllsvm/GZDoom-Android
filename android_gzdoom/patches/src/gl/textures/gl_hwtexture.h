*** doom/src/main/jni/gzdoom/src/gl/textures/gl_hwtexture.h	2017-06-18 23:15:10.669973853 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/textures/gl_hwtexture.h	2017-06-18 23:34:13.260581217 -0400
***************
*** 2,11 ****
  #ifndef __GLTEXTURE_H
  #define __GLTEXTURE_H
  
- #ifdef LoadImage
- #undef LoadImage
- #endif
- 
  #define SHADED_TEXTURE -1
  #define DIRECT_PALETTE -2
  
--- 2,7 ----
***************
*** 14,25 ****
  class FCanvasTexture;
  class AActor;
  
- // For error catching while changing parameters.
- enum EInvalid
- {
- 	Invalid = 0
- };
- 
  enum
  {
  	GLT_CLAMPX=1,
--- 10,15 ----
***************
*** 28,53 ****
  
  class FHardwareTexture
  {
- public:
  	enum
  	{
  		MAX_TEXTURES = 16
  	};
  
- private:
  	struct TranslatedTexture
  	{
  		unsigned int glTexID;
  		int translation;
! 		bool mipmapped;
! 
! 		void Delete();
  	};
  
  public:
  
  	static unsigned int lastbound[MAX_TEXTURES];
  	static int lastactivetexture;
  	static int max_texturesize;
  
  	static int GetTexDimension(int value);
--- 18,40 ----
  
  class FHardwareTexture
  {
  	enum
  	{
  		MAX_TEXTURES = 16
  	};
  
  	struct TranslatedTexture
  	{
  		unsigned int glTexID;
  		int translation;
! 		int cm;
  	};
  
  public:
  
  	static unsigned int lastbound[MAX_TEXTURES];
  	static int lastactivetexture;
+ 	static bool supportsNonPower2;
  	static int max_texturesize;
  
  	static int GetTexDimension(int value);
***************
*** 55,73 ****
  private:
  
  	short texwidth, texheight;
  	bool forcenocompression;
  
! 	TranslatedTexture glDefTex;
! 	TArray<TranslatedTexture> glTex_Translated;
  	unsigned int glDepthID;	// only used by camera textures
  
! 	TranslatedTexture * GetTexID(int translation);
  
  	int GetDepthBuffer();
! 	void Resize(int width, int height, unsigned char *src_data, unsigned char *dst_data);
  
  public:
! 	FHardwareTexture(int w, int h, bool nocompress);
  	~FHardwareTexture();
  
  	static void Unbind(int texunit);
--- 42,65 ----
  private:
  
  	short texwidth, texheight;
+ 	//float scalexfac, scaleyfac;
+ 	bool mipmap;
+ 	BYTE clampmode;
+ 	bool forcenofiltering;
  	bool forcenocompression;
  
! 	unsigned int * glTexID;
! 	TArray<TranslatedTexture> glTexID_Translated;
  	unsigned int glDepthID;	// only used by camera textures
  
! 	void LoadImage(unsigned char * buffer,int w, int h, unsigned int & glTexID,int wrapparam, bool alphatexture, int texunit);
! 	unsigned * GetTexID(int cm, int translation);
  
  	int GetDepthBuffer();
! 	void DeleteTexture(unsigned int texid);
  
  public:
! 	FHardwareTexture(int w, int h, bool mip, bool wrap, bool nofilter, bool nocompress);
  	~FHardwareTexture();
  
  	static void Unbind(int texunit);
***************
*** 75,82 ****
  
  	void BindToFrameBuffer();
  
! 	unsigned int Bind(int texunit, int translation, bool needmipmap);
! 	unsigned int CreateTexture(unsigned char * buffer, int w, int h, int texunit, bool mipmap, int translation);
  
  	void Clean(bool all);
  };
--- 67,75 ----
  
  	void BindToFrameBuffer();
  
! 	unsigned int Bind(int texunit, int cm, int translation=0);
! 	unsigned int CreateTexture(unsigned char * buffer, int w, int h,bool wrap, int texunit, int cm, int translation=0);
! 	void Resize(int _width, int _height) ;
  
  	void Clean(bool all);
  };
