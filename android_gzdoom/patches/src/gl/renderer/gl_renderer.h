*** doom/src/main/jni/gzdoom/src/gl/renderer/gl_renderer.h	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/renderer/gl_renderer.h	2017-06-18 23:34:13.277248038 -0400
***************
*** 9,23 ****
  struct particle_t;
  class FCanvasTexture;
  class FFlatVertexBuffer;
- class FSkyVertexBuffer;
- class FModelVertexBuffer;
  class OpenGLFrameBuffer;
  struct FDrawInfo;
  struct pspdef_t;
  class FShaderManager;
  class GLPortal;
! class FLightBuffer;
! class FSamplerManager;
  
  enum SectorRenderFlags
  {
--- 9,20 ----
  struct particle_t;
  class FCanvasTexture;
  class FFlatVertexBuffer;
  class OpenGLFrameBuffer;
  struct FDrawInfo;
  struct pspdef_t;
  class FShaderManager;
  class GLPortal;
! class FGLThreadManager;
  
  enum SectorRenderFlags
  {
***************
*** 56,69 ****
  	float mCurrentFoV;
  	AActor *mViewActor;
  	FShaderManager *mShaderManager;
! 	FSamplerManager *mSamplerManager;
  	int gl_spriteindex;
  	unsigned int mFBID;
  
  	FTexture *glpart2;
  	FTexture *glpart;
  	FTexture *mirrortexture;
! 	
  	float mSky1Pos, mSky2Pos;
  
  	FRotator mAngles;
--- 53,67 ----
  	float mCurrentFoV;
  	AActor *mViewActor;
  	FShaderManager *mShaderManager;
! 	FGLThreadManager *mThreadManager;
  	int gl_spriteindex;
  	unsigned int mFBID;
  
  	FTexture *glpart2;
  	FTexture *glpart;
  	FTexture *mirrortexture;
! 	FTexture *gllight;
! 
  	float mSky1Pos, mSky2Pos;
  
  	FRotator mAngles;
***************
*** 71,79 ****
  	FVector3 mCameraPos;
  
  	FFlatVertexBuffer *mVBO;
- 	FSkyVertexBuffer *mSkyVBO;
- 	FModelVertexBuffer *mModelVBO;
- 	FLightBuffer *mLights;
  
  
  	FGLRenderer(OpenGLFrameBuffer *fb);
--- 69,74 ----
***************
*** 96,102 ****
  	void DrawScene(bool toscreen = false);
  	void DrawBlend(sector_t * viewsector);
  
! 	void DrawPSprite (player_t * player,pspdef_t *psp,fixed_t sx, fixed_t sy, bool hudModelStep, int OverrideShader, bool alphatexture);
  	void DrawPlayerSprites(sector_t * viewsector, bool hudModelStep);
  	void DrawTargeterSprites();
  
--- 91,97 ----
  	void DrawScene(bool toscreen = false);
  	void DrawBlend(sector_t * viewsector);
  
! 	void DrawPSprite (player_t * player,pspdef_t *psp,fixed_t sx, fixed_t sy, int cm_index, bool hudModelStep, int OverrideShader);
  	void DrawPlayerSprites(sector_t * viewsector, bool hudModelStep);
  	void DrawTargeterSprites();
  
