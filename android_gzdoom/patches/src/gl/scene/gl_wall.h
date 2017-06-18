*** doom/src/main/jni/gzdoom/src/gl/scene/gl_wall.h	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_wall.h	2017-06-18 23:34:13.290581495 -0400
***************
*** 20,26 ****
  struct GLSkyInfo;
  struct FTexCoordInfo;
  struct FPortal;
- struct FFlatVertex;
  
  
  enum WallTypes
--- 20,25 ----
***************
*** 39,44 ****
--- 38,44 ----
  	RENDERWALL_MIRROR,
  	RENDERWALL_MIRRORSURFACE,
  	RENDERWALL_M2SNF,
+ 	RENDERWALL_M2SFOG,
  	RENDERWALL_COLOR,
  	RENDERWALL_FFBLOCK,
  	RENDERWALL_COLORLAYER,
***************
*** 96,117 ****
  		//GLWF_CLAMPX=1, use GLT_* for these!
  		//GLWF_CLAMPY=2,
  		GLWF_SKYHACK=4,
! 		GLWF_GLOW=8,		// illuminated by glowing flats
! 		GLWF_NOSPLITUPPER=16,
! 		GLWF_NOSPLITLOWER=32,
! 		GLWF_NOSPLIT=64,
  	};
  
- 	enum
- 	{
- 		RWF_BLANK = 0,
- 		RWF_TEXTURED = 1,	// actually not being used anymore because with buffers it's even less efficient not writing the texture coordinates - but leave it here
- 		RWF_GLOW = 2,
- 		RWF_NOSPLIT = 4,
- 		RWF_NORENDER = 8,
- 	};
- 
- 
  	friend struct GLDrawList;
  	friend class GLPortal;
  
--- 96,108 ----
  		//GLWF_CLAMPX=1, use GLT_* for these!
  		//GLWF_CLAMPY=2,
  		GLWF_SKYHACK=4,
! 		GLWF_FOGGY=8,
! 		GLWF_GLOW=16,		// illuminated by glowing flats
! 		GLWF_NOSHADER=32,	// cannot be drawn with shaders.
! 		GLWF_NOSPLITUPPER=64,
! 		GLWF_NOSPLITLOWER=128,
  	};
  
  	friend struct GLDrawList;
  	friend class GLPortal;
  
***************
*** 135,141 ****
  	float topglowcolor[4];
  	float bottomglowcolor[4];
  
! 	int dynlightindex;
  	int firstwall, numwalls;	// splitting info.
  
  	union
--- 126,132 ----
  	float topglowcolor[4];
  	float bottomglowcolor[4];
  
! 	int firstdynlight, lastdynlight;
  	int firstwall, numwalls;	// splitting info.
  
  	union
***************
*** 150,156 ****
  
  
  	FTextureID topflat,bottomflat;
- 	secplane_t topplane, bottomplane;	// we need to save these to pass them to the shader for calculating glows.
  
  	// these are not the same as ytop and ybottom!!!
  	float zceil[2];
--- 141,146 ----
***************
*** 167,173 ****
  
  	void SetupLights();
  	bool PrepareLight(texcoord * tcs, ADynamicLight * light);
! 	void RenderWall(int textured, unsigned int *store = NULL);
  
  	void FloodPlane(int pass);
  
--- 157,165 ----
  
  	void SetupLights();
  	bool PrepareLight(texcoord * tcs, ADynamicLight * light);
! 	void RenderWall(int textured, float * color2, ADynamicLight * light=NULL);
! 	void RenderGlowingPoly(int textured, ADynamicLight * light=NULL);
! 	int Intersection(FloatRect * rc,GLWall * result);
  
  	void FloodPlane(int pass);
  
***************
*** 219,228 ****
  	void RenderMirrorSurface();
  	void RenderTranslucentWall();
  
! 	void SplitLeftEdge(texcoord * tcs, FFlatVertex *&ptr);
! 	void SplitRightEdge(texcoord * tcs, FFlatVertex *&ptr);
! 	void SplitUpperEdge(texcoord * tcs, FFlatVertex *&ptr);
! 	void SplitLowerEdge(texcoord * tcs, FFlatVertex *&ptr);
  
  public:
  
--- 211,220 ----
  	void RenderMirrorSurface();
  	void RenderTranslucentWall();
  
! 	void SplitLeftEdge(texcoord * tcs, bool glow);
! 	void SplitRightEdge(texcoord * tcs, bool glow);
! 	void SplitUpperEdge(texcoord * tcs, bool glow);
! 	void SplitLowerEdge(texcoord * tcs, bool glow);
  
  public:
  
***************
*** 279,295 ****
  
  	int dynlightindex;
  
! 	void SetupSubsectorLights(int pass, subsector_t * sub, int *dli = NULL);
  	void DrawSubsector(subsector_t * sub);
  	void DrawSubsectorLights(subsector_t * sub, int pass);
! 	void DrawSubsectors(int pass, bool processlights, bool istrans);
! 	void ProcessLights(bool istrans);
  
  	void PutFlat(bool fog = false);
  	void Process(sector_t * model, int whichplane, bool notexture);
  	void SetFrom3DFloor(F3DFloor *rover, bool top, bool underside);
  	void ProcessSector(sector_t * frontsector);
! 	void Draw(int pass, bool trans);
  };
  
  
--- 271,286 ----
  
  	int dynlightindex;
  
! 	bool SetupSubsectorLights(bool lightsapplied, subsector_t * sub);
  	void DrawSubsector(subsector_t * sub);
  	void DrawSubsectorLights(subsector_t * sub, int pass);
! 	void DrawSubsectors(int pass, bool istrans);
  
  	void PutFlat(bool fog = false);
  	void Process(sector_t * model, int whichplane, bool notexture);
  	void SetFrom3DFloor(F3DFloor *rover, bool top, bool underside);
  	void ProcessSector(sector_t * frontsector);
! 	void Draw(int pass);
  };
  
  
***************
*** 334,340 ****
  
  	void SplitSprite(sector_t * frontsector, bool translucent);
  	void SetLowerParam();
- 	void PerformSpriteClipAdjustment(AActor *thing, fixed_t thingx, fixed_t thingy, float spriteheight);
  
  public:
  
--- 325,330 ----
***************
*** 356,362 ****
  
  // Light + color
  
! void gl_SetDynSpriteLight(AActor *self, fixed_t x, fixed_t y, fixed_t z, subsector_t *subsec);
! void gl_SetDynSpriteLight(AActor *actor, particle_t *particle);
  
  #endif
--- 346,365 ----
  
  // Light + color
  
! bool gl_GetSpriteLight(AActor *Self, fixed_t x, fixed_t y, fixed_t z, subsector_t * subsec, int desaturation, float * out, line_t *line = NULL, int side = 0);
! int gl_SetSpriteLight(AActor * thing, int lightlevel, int rellight, FColormap * cm, float alpha, PalEntry ThingColor = 0xffffff, bool weapon=false);
! 
! void gl_GetSpriteLight(AActor * thing, int lightlevel, int rellight, FColormap * cm,
! 					   float *red, float *green, float *blue,
! 					   PalEntry ThingColor, bool weapon);
! 
! int gl_SetSpriteLighting(FRenderStyle style, AActor *thing, int lightlevel, int rellight, FColormap *cm, 
! 						  PalEntry ThingColor, float alpha, bool fullbright, bool weapon);
! 
! int gl_SetSpriteLight(particle_t * thing, int lightlevel, int rellight, FColormap *cm, float alpha, PalEntry ThingColor = 0xffffff);
! void gl_GetLightForThing(AActor * thing, float upper, float lower, float & r, float & g, float & b);
! 
! 
! 
  
  #endif
