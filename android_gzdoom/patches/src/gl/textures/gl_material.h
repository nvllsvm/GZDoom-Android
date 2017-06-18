*** doom/src/main/jni/gzdoom/src/gl/textures/gl_material.h	2017-06-18 23:15:10.669973853 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/textures/gl_material.h	2017-06-18 23:34:13.253914489 -0400
***************
*** 13,29 ****
  struct FRemapTable;
  class FTextureShader;
  
- enum
- {
- 	CLAMP_NONE = 0,
- 	CLAMP_X = 1,
- 	CLAMP_Y = 2,
- 	CLAMP_XY = 3,
- 	CLAMP_XY_NOMIP = 4,
- 	CLAMP_NOFILTER = 5,
- 	CLAMP_CAMTEX = 6,
- };
- 
  
  
  struct FTexCoordInfo
--- 13,18 ----
***************
*** 51,58 ****
  //===========================================================================
  class FMaterial;
  
  
! class FGLTexture
  {
  	friend class FMaterial;
  public:
--- 40,54 ----
  //===========================================================================
  class FMaterial;
  
+ enum ETexUse
+ {
+ 	GLUSE_PATCH,
+ 	GLUSE_TEXTURE,
+ 	GLUSE_SPRITE,
+ };
+ 
  
! class FGLTexture //: protected WorldTextureInfo, protected PatchTextureInfo
  {
  	friend class FMaterial;
  public:
***************
*** 62,83 ****
  	int HiresLump;
  
  private:
! 	FHardwareTexture *mHwTexture;
  
  	bool bHasColorkey;		// only for hires
  	bool bExpand;
  
! 	unsigned char * LoadHiresTexture(FTexture *hirescheck, int *width, int *height);
  
! 	FHardwareTexture *CreateHwTexture();
  
- 	const FHardwareTexture *Bind(int texunit, int clamp, int translation, FTexture *hirescheck);
- 	
  public:
  	FGLTexture(FTexture * tx, bool expandpatches);
  	~FGLTexture();
  
! 	unsigned char * CreateTexBuffer(int translation, int & w, int & h, FTexture *hirescheck);
  
  	void Clean(bool all);
  	int Dump(int i);
--- 58,88 ----
  	int HiresLump;
  
  private:
! 	FHardwareTexture *gltexture[5];
! 	FHardwareTexture *glpatch;
! 
! 	int currentwarp;
! 	int currentwarptime;
  
  	bool bHasColorkey;		// only for hires
  	bool bExpand;
+ 	float AlphaThreshold;
  
! 	unsigned char * LoadHiresTexture(FTexture *hirescheck, int *width, int *height, int cm);
! 	BYTE *WarpBuffer(BYTE *buffer, int Width, int Height, int warp);
  
! 	FHardwareTexture *CreateTexture(int clampmode);
! 	//bool CreateTexture();
! 	bool CreatePatch();
! 
! 	const FHardwareTexture *Bind(int texunit, int cm, int clamp, int translation, FTexture *hirescheck, int warp);
! 	const FHardwareTexture *BindPatch(int texunit, int cm, int translation, int warp);
  
  public:
  	FGLTexture(FTexture * tx, bool expandpatches);
  	~FGLTexture();
  
! 	unsigned char * CreateTexBuffer(int cm, int translation, int & w, int & h, bool expand, FTexture *hirescheck, int warp);
  
  	void Clean(bool all);
  	int Dump(int i);
***************
*** 92,99 ****
  
  class FMaterial
  {
- 	friend class FRenderState;
- 
  	struct FTextureLayer
  	{
  		FTexture *texture;
--- 97,102 ----
***************
*** 107,123 ****
  	TArray<FTextureLayer> mTextureLayers;
  	int mShaderIndex;
  
! 	short mLeftOffset;
! 	short mTopOffset;
! 	short mWidth;
! 	short mHeight;
! 	short mRenderWidth;
! 	short mRenderHeight;
! 	bool mExpanded;
  
! 	float mSpriteU[2], mSpriteV[2];
! 	FloatRect mSpriteRect;
  
  	FGLTexture * ValidateSysTexture(FTexture * tex, bool expand);
  	bool TrimBorders(int *rect);
  
--- 110,126 ----
  	TArray<FTextureLayer> mTextureLayers;
  	int mShaderIndex;
  
! 	short LeftOffset[3];
! 	short TopOffset[3];
! 	short Width[3];
! 	short Height[3];
! 	short RenderWidth[2];
! 	short RenderHeight[2];
  
! 	float SpriteU[2], SpriteV[2];
! 	float spriteright, spritebottom;
  
+ 	void SetupShader(int shaderindex, int &cm);
  	FGLTexture * ValidateSysTexture(FTexture * tex, bool expand);
  	bool TrimBorders(int *rect);
  
***************
*** 132,147 ****
  		return !!mBaseLayer->tex->bMasked;
  	}
  
! 	int GetLayers() const
! 	{
! 		return mTextureLayers.Size() + 1;
! 	}
! 
! 	void Bind(int clamp, int translation);
  
! 	unsigned char * CreateTexBuffer(int translation, int & w, int & h, bool allowhires=true) const
  	{
! 		return mBaseLayer->CreateTexBuffer(translation, w, h, allowhires? tex : NULL);
  	}
  
  	void Clean(bool f)
--- 135,146 ----
  		return !!mBaseLayer->tex->bMasked;
  	}
  
! 	void Bind(int cm, int clamp = 0, int translation = 0, int overrideshader = 0);
! 	void BindPatch(int cm, int translation = 0, int overrideshader = 0);
  
! 	unsigned char * CreateTexBuffer(int cm, int translation, int & w, int & h, bool expand = false, bool allowhires=true) const
  	{
! 		return mBaseLayer->CreateTexBuffer(cm, translation, w, h, expand, allowhires? tex:NULL, 0);
  	}
  
  	void Clean(bool f)
***************
*** 152,233 ****
  	void BindToFrameBuffer();
  	// Patch drawing utilities
  
! 	void GetSpriteRect(FloatRect * r) const
! 	{
! 		*r = mSpriteRect;
! 	}
! 
  	void GetTexCoordInfo(FTexCoordInfo *tci, fixed_t x, fixed_t y) const;
  
  	// This is scaled size in integer units as needed by walls and flats
! 	int TextureHeight() const { return mRenderHeight; }
! 	int TextureWidth() const { return mRenderWidth; }
  
  	int GetAreas(FloatRect **pAreas) const;
  
! 	int GetWidth() const
  	{
! 		return mWidth;
  	}
  
! 	int GetHeight() const
  	{
! 		return mHeight;
  	}
  
! 	int GetLeftOffset() const
  	{
! 		return mLeftOffset;
  	}
  
! 	int GetTopOffset() const
  	{
! 		return mTopOffset;
  	}
  
! 	int GetScaledLeftOffset() const
  	{
! 		return DivScale16(mLeftOffset, tex->xScale);
  	}
  
! 	int GetScaledTopOffset() const
  	{
! 		return DivScale16(mTopOffset, tex->yScale);
  	}
  
! 	float GetScaledLeftOffsetFloat() const
  	{
! 		return mLeftOffset / FIXED2FLOAT(tex->xScale);
  	}
  
! 	float GetScaledTopOffsetFloat() const
  	{
! 		return mTopOffset/ FIXED2FLOAT(tex->yScale);
  	}
  
  	// This is scaled size in floating point as needed by sprites
! 	float GetScaledWidthFloat() const
  	{
! 		return mWidth / FIXED2FLOAT(tex->xScale);
  	}
  
! 	float GetScaledHeightFloat() const
  	{
! 		return mHeight / FIXED2FLOAT(tex->yScale);
  	}
  
  	// Get right/bottom UV coordinates for patch drawing
  	float GetUL() const { return 0; }
  	float GetVT() const { return 0; }
! 	float GetUR() const { return 1; }
! 	float GetVB() const { return 1; }
! 	float GetU(float upix) const { return upix/(float)mWidth; }
! 	float GetV(float vpix) const { return vpix/(float)mHeight; }
! 
! 	float GetSpriteUL() const { return mSpriteU[0]; }
! 	float GetSpriteVT() const { return mSpriteV[0]; }
! 	float GetSpriteUR() const { return mSpriteU[1]; }
! 	float GetSpriteVB() const { return mSpriteV[1]; }
  
  
  
--- 151,228 ----
  	void BindToFrameBuffer();
  	// Patch drawing utilities
  
! 	void GetRect(FloatRect *r, ETexUse i) const;
  	void GetTexCoordInfo(FTexCoordInfo *tci, fixed_t x, fixed_t y) const;
  
  	// This is scaled size in integer units as needed by walls and flats
! 	int TextureHeight(ETexUse i) const { return RenderHeight[i]; }
! 	int TextureWidth(ETexUse i) const { return RenderWidth[i]; }
  
  	int GetAreas(FloatRect **pAreas) const;
  
! 	int GetWidth(ETexUse i) const
  	{
! 		return Width[i];
  	}
  
! 	int GetHeight(ETexUse i) const
  	{
! 		return Height[i];
  	}
  
! 	int GetLeftOffset(ETexUse i) const
  	{
! 		return LeftOffset[i];
  	}
  
! 	int GetTopOffset(ETexUse i) const
  	{
! 		return TopOffset[i];
  	}
  
! 	int GetScaledLeftOffset(ETexUse i) const
  	{
! 		return DivScale16(LeftOffset[i], tex->xScale);
  	}
  
! 	int GetScaledTopOffset(ETexUse i) const
  	{
! 		return DivScale16(TopOffset[i], tex->yScale);
  	}
  
! 	float GetScaledLeftOffsetFloat(ETexUse i) const
  	{
! 		return LeftOffset[i] / FIXED2FLOAT(tex->xScale);
  	}
  
! 	float GetScaledTopOffsetFloat(ETexUse i) const
  	{
! 		return TopOffset[i] / FIXED2FLOAT(tex->yScale);
  	}
  
  	// This is scaled size in floating point as needed by sprites
! 	float GetScaledWidthFloat(ETexUse i) const
  	{
! 		return Width[i] / FIXED2FLOAT(tex->xScale);
  	}
  
! 	float GetScaledHeightFloat(ETexUse i) const
  	{
! 		return Height[i] / FIXED2FLOAT(tex->yScale);
  	}
  
  	// Get right/bottom UV coordinates for patch drawing
  	float GetUL() const { return 0; }
  	float GetVT() const { return 0; }
! 	float GetUR() const { return spriteright; }
! 	float GetVB() const { return spritebottom; }
! 	float GetU(float upix) const { return upix/(float)Width[GLUSE_PATCH] * spriteright; }
! 	float GetV(float vpix) const { return vpix/(float)Height[GLUSE_PATCH] * spritebottom; }
! 
! 	float GetSpriteUL() const { return SpriteU[0]; }
! 	float GetSpriteVT() const { return SpriteV[0]; }
! 	float GetSpriteUR() const { return SpriteU[1]; }
! 	float GetSpriteVB() const { return SpriteV[1]; }
  
  
  
***************
*** 238,244 ****
  			if (!mBaseLayer->tex->bHasCanvas)
  			{
  				int w, h;
! 				unsigned char *buffer = CreateTexBuffer(0, w, h);
  				delete [] buffer;
  			}
  			else
--- 233,239 ----
  			if (!mBaseLayer->tex->bHasCanvas)
  			{
  				int w, h;
! 				unsigned char *buffer = CreateTexBuffer(CM_DEFAULT, 0, w, h);
  				delete [] buffer;
  			}
  			else
***************
*** 251,259 ****
  
  	static void DeleteAll();
  	static void FlushAll();
! 	static FMaterial *ValidateTexture(FTexture * tex, bool expand);
! 	static FMaterial *ValidateTexture(FTextureID no, bool expand, bool trans);
! 	static void ClearLastTexture();
  
  };
  
--- 246,253 ----
  
  	static void DeleteAll();
  	static void FlushAll();
! 	static FMaterial *ValidateTexture(FTexture * tex);
! 	static FMaterial *ValidateTexture(FTextureID no, bool trans);
  
  };
  
