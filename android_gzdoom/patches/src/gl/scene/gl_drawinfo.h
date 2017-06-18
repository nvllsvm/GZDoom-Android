*** doom/src/main/jni/gzdoom/src/gl/scene/gl_drawinfo.h	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_drawinfo.h	2017-06-18 23:34:13.310581678 -0400
***************
*** 13,38 ****
  
  enum DrawListType
  {
! 	GLDL_PLAINWALLS,
! 	GLDL_PLAINFLATS,
! 	GLDL_MASKEDWALLS,
! 	GLDL_MASKEDFLATS,
! 	GLDL_MASKEDWALLSOFS,
! 	GLDL_MODELS,
  
  	GLDL_TRANSLUCENT,
  	GLDL_TRANSLUCENTBORDER,
  
  	GLDL_TYPES,
  };
  
  enum Drawpasses
  {
! 	GLPASS_ALL,			// Main pass with dynamic lights
! 	GLPASS_LIGHTSONLY,	// only collect dynamic lights
! 	GLPASS_PLAIN,		// Main pass without dynamic lights
  	GLPASS_DECALS,		// Draws a decal
  	GLPASS_TRANSLUCENT,	// Draws translucent objects
  };
  
  //==========================================================================
--- 13,54 ----
  
  enum DrawListType
  {
! 	// These are organized so that the various multipass rendering modes
! 	// have to be set as few times as possible
! 	GLDL_LIGHT,	
! 	GLDL_LIGHTBRIGHT,
! 	GLDL_LIGHTMASKED,
! 	GLDL_LIGHTFOG,
! 	GLDL_LIGHTFOGMASKED,
! 
! 	GLDL_PLAIN,
! 	GLDL_MASKED,
! 	GLDL_FOG,
! 	GLDL_FOGMASKED,
  
  	GLDL_TRANSLUCENT,
  	GLDL_TRANSLUCENTBORDER,
  
  	GLDL_TYPES,
+ 
+ 	GLDL_FIRSTLIGHT = GLDL_LIGHT,
+ 	GLDL_LASTLIGHT = GLDL_LIGHTFOGMASKED,
+ 	GLDL_FIRSTNOLIGHT = GLDL_PLAIN,
+ 	GLDL_LASTNOLIGHT = GLDL_FOGMASKED,
  };
  
  enum Drawpasses
  {
! 	GLPASS_BASE,		// Draws the untextured surface only
! 	GLPASS_BASE_MASKED,	// Draws an untextured surface that is masked by the texture
! 	GLPASS_PLAIN,		// Draws a texture that isn't affected by dynamic lights with sector light settings
! 	GLPASS_LIGHT,		// Draws dynamic lights
! 	GLPASS_LIGHT_ADDITIVE,	// Draws additive dynamic lights
! 	GLPASS_TEXTURE,		// Draws the texture to be modulated with the light information on the base surface
  	GLPASS_DECALS,		// Draws a decal
+ 	GLPASS_DECALS_NOFOG,// Draws a decal without setting the fog (used for passes that need a fog layer)
  	GLPASS_TRANSLUCENT,	// Draws translucent objects
+ 	GLPASS_ALL			// Everything at once, using shaders for dynamic lights
  };
  
  //==========================================================================
***************
*** 98,114 ****
  		Reset();
  	}
  
- 	unsigned int Size()
- 	{
- 		return drawitems.Size();
- 	}
- 
  	void AddWall(GLWall * wall);
  	void AddFlat(GLFlat * flat);
  	void AddSprite(GLSprite * sprite);
  	void Reset();
! 	void SortWalls();
! 	void SortFlats();
  
  
  	void MakeSortList();
--- 114,124 ----
  		Reset();
  	}
  
  	void AddWall(GLWall * wall);
  	void AddFlat(GLFlat * flat);
  	void AddSprite(GLSprite * sprite);
  	void Reset();
! 	void Sort();
  
  
  	void MakeSortList();
***************
*** 123,135 ****
  	SortNode * SortSpriteList(SortNode * head);
  	SortNode * DoSort(SortNode * head);
  	
! 	void DoDraw(int pass, int index, bool trans);
  	void DoDrawSorted(SortNode * node);
  	void DrawSorted();
  	void Draw(int pass);
- 	void DrawWalls(int pass);
- 	void DrawFlats(int pass);
- 	void DrawDecals();
  	
  	GLDrawList * next;
  } ;
--- 133,142 ----
  	SortNode * SortSpriteList(SortNode * head);
  	SortNode * DoSort(SortNode * head);
  	
! 	void DoDraw(int pass, int index);
  	void DoDrawSorted(SortNode * node);
  	void DrawSorted();
  	void Draw(int pass);
  	
  	GLDrawList * next;
  } ;
***************
*** 266,272 ****
  
  extern FDrawInfo * gl_drawinfo;
  
! void gl_SetPlaneTextureRotation(const GLSectorPlane * secplane, FMaterial * gltexture);
  void gl_SetRenderStyle(FRenderStyle style, bool drawopaque, bool allowcolorblending);
  
  #endif
\ No newline at end of file
--- 273,279 ----
  
  extern FDrawInfo * gl_drawinfo;
  
! bool gl_SetPlaneTextureRotation(const GLSectorPlane * secplane, FMaterial * gltexture);
  void gl_SetRenderStyle(FRenderStyle style, bool drawopaque, bool allowcolorblending);
  
  #endif
\ No newline at end of file
