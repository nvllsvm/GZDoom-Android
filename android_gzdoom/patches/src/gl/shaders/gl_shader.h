*** doom/src/main/jni/gzdoom/src/gl/shaders/gl_shader.h	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/shaders/gl_shader.h	2017-06-18 23:34:13.423916060 -0400
***************
*** 7,228 ****
  
  extern bool gl_shaderactive;
  
! enum
! {
! 	VATTR_VERTEX = 0,
! 	VATTR_TEXCOORD = 1,
! 	VATTR_COLOR = 2,
! 	VATTR_VERTEX2 = 3
! };
! 
  
  //==========================================================================
  //
  //
  //==========================================================================
  
- class FUniform1i
- {
- 	int mIndex;
- 
- public:
- 	void Init(GLuint hShader, const GLchar *name)
- 	{
- 		mIndex = glGetUniformLocation(hShader, name);
- 	}
- 
- 	void Set(int newvalue)
- 	{
- 		glUniform1i(mIndex, newvalue);
- 	}
- };
- 
- class FBufferedUniform1i
- {
- 	int mBuffer;
- 	int mIndex;
- 
- public:
- 	void Init(GLuint hShader, const GLchar *name)
- 	{
- 		mIndex = glGetUniformLocation(hShader, name);
- 		mBuffer = 0;
- 	}
- 
- 	void Set(int newvalue)
- 	{
- 		if (newvalue != mBuffer)
- 		{
- 			mBuffer = newvalue;
- 			glUniform1i(mIndex, newvalue);
- 		}
- 	}
- };
- 
- class FBufferedUniform4i
- {
- 	int mBuffer[4];
- 	int mIndex;
- 
- public:
- 	void Init(GLuint hShader, const GLchar *name)
- 	{
- 		mIndex = glGetUniformLocation(hShader, name);
- 		memset(mBuffer, 0, sizeof(mBuffer));
- 	}
- 
- 	void Set(const int *newvalue)
- 	{
- 		if (memcmp(newvalue, mBuffer, sizeof(mBuffer)))
- 		{
- 			memcpy(mBuffer, newvalue, sizeof(mBuffer));
- 			glUniform4iv(mIndex, 1, newvalue);
- 		}
- 	}
- };
- 
- class FBufferedUniform1f
- {
- 	float mBuffer;
- 	int mIndex;
- 
- public:
- 	void Init(GLuint hShader, const GLchar *name)
- 	{
- 		mIndex = glGetUniformLocation(hShader, name);
- 		mBuffer = 0;
- 	}
- 
- 	void Set(float newvalue)
- 	{
- 		if (newvalue != mBuffer)
- 		{
- 			mBuffer = newvalue;
- 			glUniform1f(mIndex, newvalue);
- 		}
- 	}
- };
- 
- class FBufferedUniform4f
- {
- 	float mBuffer[4];
- 	int mIndex;
- 
- public:
- 	void Init(GLuint hShader, const GLchar *name)
- 	{
- 		mIndex = glGetUniformLocation(hShader, name);
- 		memset(mBuffer, 0, sizeof(mBuffer));
- 	}
- 
- 	void Set(const float *newvalue)
- 	{
- 		if (memcmp(newvalue, mBuffer, sizeof(mBuffer)))
- 		{
- 			memcpy(mBuffer, newvalue, sizeof(mBuffer));
- 			glUniform4fv(mIndex, 1, newvalue);
- 		}
- 	}
- };
- 
- class FUniform4f
- {
- 	int mIndex;
- 
- public:
- 	void Init(GLuint hShader, const GLchar *name)
- 	{
- 		mIndex = glGetUniformLocation(hShader, name);
- 	}
- 
- 	void Set(const float *newvalue)
- 	{
- 		glUniform4fv(mIndex, 1, newvalue);
- 	}
- 
- 	void Set(float a, float b, float c, float d)
- 	{
- 		glUniform4f(mIndex, a, b, c, d);
- 	}
- };
- 
- class FBufferedUniformPE
- {
- 	PalEntry mBuffer;
- 	int mIndex;
- 
- public:
- 	void Init(GLuint hShader, const GLchar *name)
- 	{
- 		mIndex = glGetUniformLocation(hShader, name);
- 		mBuffer = 0;
- 	}
- 
- 	void Set(PalEntry newvalue)
- 	{
- 		if (newvalue != mBuffer)
- 		{
- 			mBuffer = newvalue;
- 			glUniform4f(mIndex, newvalue.r/255.f, newvalue.g/255.f, newvalue.b/255.f, newvalue.a/255.f);
- 		}
- 	}
- };
- 
- 
  class FShader
  {
! 	friend class FShaderManager;
  	friend class FRenderState;
  
  	unsigned int hShader;
  	unsigned int hVertProg;
  	unsigned int hFragProg;
- 	FName mName;
  
! 	FBufferedUniform1f muDesaturation;
! 	FBufferedUniform1i muFogEnabled;
! 	FBufferedUniform1i muTextureMode;
! 	FBufferedUniform4f muCameraPos;
! 	FBufferedUniform4f muLightParms;
! 	FUniform1i muFixedColormap;
! 	FUniform4f muColormapStart;
! 	FUniform4f muColormapRange;
! 	FBufferedUniform1i muLightIndex;
! 	FBufferedUniformPE muFogColor;
! 	FBufferedUniform4f muDynLightColor;
! 	FBufferedUniformPE muObjectColor;
! 	FUniform4f muGlowBottomColor;
! 	FUniform4f muGlowTopColor;
! 	FUniform4f muGlowBottomPlane;
! 	FUniform4f muGlowTopPlane;
! 	FBufferedUniform1f muInterpolationFactor;
! 	FBufferedUniform1f muClipHeightTop;
! 	FBufferedUniform1f muClipHeightBottom;
! 	FBufferedUniform1f muAlphaThreshold;
! 	FBufferedUniform1f muTimer;
! 	
  	int lights_index;
! 	int projectionmatrix_index;
! 	int viewmatrix_index;
! 	int modelmatrix_index;
! 	int texturematrix_index;
! public:
! 	int fakevb_index;
! private:
! 	int currentglowstate;
! 	int currentfixedcolormap;
! 	bool currentTextureMatrixState;
! 	bool currentModelMatrixState;
  
  public:
! 	FShader(const char *name)
! 		: mName(name)
  	{
  		hShader = hVertProg = hFragProg = 0;
! 		currentglowstate = 0;
! 		currentfixedcolormap = 0;
! 		currentTextureMatrixState = true;	// by setting the matrix state to 'true' it is guaranteed to be set the first time the render state gets applied.
! 		currentModelMatrixState = true;
  	}
  
  	~FShader();
--- 7,78 ----
  
  extern bool gl_shaderactive;
  
! const int VATTR_GLOWDISTANCE = 15;
! const int VATTR_FOGPARAMS = 14;
! const int VATTR_LIGHTLEVEL = 13; // Korshun.
  
  //==========================================================================
  //
  //
  //==========================================================================
  
  class FShader
  {
! 	friend class FShaderContainer;
  	friend class FRenderState;
  
  	unsigned int hShader;
  	unsigned int hVertProg;
  	unsigned int hFragProg;
  
! 	int timer_index;
! 	int desaturation_index;
! 	int fogenabled_index;
! 	int texturemode_index;
! 	int camerapos_index;
! 	int lightparms_index;
! 	int colormapstart_index;
! 	int colormaprange_index;
! 	int lightrange_index;
! 	int fogcolor_index;
  	int lights_index;
! 	int dlightcolor_index;
! 
! 	int glowbottomcolor_index;
! 	int glowtopcolor_index;
! 
! 	int currentfogenabled;
! 	int currenttexturemode;
! 	float currentlightfactor;
! 	float currentlightdist;
! 
! 	PalEntry currentfogcolor;
! 	float currentfogdensity;
! 
! 	FStateVec3 currentcamerapos;
  
  public:
! 	FShader()
  	{
  		hShader = hVertProg = hFragProg = 0;
! 		currentfogenabled = currenttexturemode = 0;
! 		currentlightfactor = currentlightdist = 0.0f;
! 		currentfogdensity = -1;
! 		currentfogcolor = 0;
! 
! 		timer_index = -1;
! 		desaturation_index = -1;
! 		fogenabled_index = -1;
! 		texturemode_index = -1;
! 		camerapos_index = -1;
! 		lightparms_index = -1;
! 		colormapstart_index = -1;
! 		colormaprange_index = -1;
! 		lightrange_index = -1;
! 		fogcolor_index = -1;
! 		lights_index = -1;
! 		dlightcolor_index = -1;
! 
  	}
  
  	~FShader();
***************
*** 233,243 ****
  	void SetGlowParams(float *topcolors, float topheight, float *bottomcolors, float bottomheight);
  	void SetLightRange(int start, int end, int forceadd);
  
! 	bool Bind();
  	unsigned int GetHandle() const { return hShader; }
  
! 	void ApplyMatrices(VSMatrix *proj, VSMatrix *view);
  
  };
  
  
--- 83,116 ----
  	void SetGlowParams(float *topcolors, float topheight, float *bottomcolors, float bottomheight);
  	void SetLightRange(int start, int end, int forceadd);
  
! 	bool Bind(float Speed);
  	unsigned int GetHandle() const { return hShader; }
  
! };
  
+ //==========================================================================
+ //
+ // This class contains the shaders for the different lighting modes
+ // that are required (e.g. special colormaps etc.)
+ //
+ //==========================================================================
+ 
+ class FShaderContainer
+ {
+ 	friend class FShaderManager;
+ 
+ 	FName Name;
+ 
+ 	enum { NUM_SHADERS = 16 };
+ 
+ 	FShader *shader[NUM_SHADERS];
+ 	FShader *shader_cm;	// the shader for fullscreen colormaps
+ 	FShader *shader_fl;	// the shader for the fog layer
+ 
+ public:
+ 	FShaderContainer(const char *ShaderName, const char *ShaderPath);
+ 	~FShaderContainer();
+ 	FShader *Bind(int cm, bool glowing, float Speed, bool lights);
  };
  
  
***************
*** 248,257 ****
  //==========================================================================
  class FShaderManager
  {
! 	TArray<FShader*> mTextureEffects;
! 	TArray<FShader*> mTextureEffectsNAT;
  	FShader *mActiveShader;
! 	FShader *mEffectShaders[MAX_EFFECTS];
  
  	void Clean();
  	void CompileShaders();
--- 121,134 ----
  //==========================================================================
  class FShaderManager
  {
! 	enum 
! 	{ 
! 		NUM_EFFECTS = 2 
! 	};
! 
! 	TArray<FShaderContainer*> mTextureEffects;
  	FShader *mActiveShader;
! 	FShader *mEffectShaders[NUM_EFFECTS];
  
  	void Clean();
  	void CompileShaders();
***************
*** 259,295 ****
  public:
  	FShaderManager();
  	~FShaderManager();
- 	FShader *Compile(const char *ShaderName, const char *ShaderPath, bool usediscard);
  	int Find(const char *mame);
  	FShader *BindEffect(int effect);
  	void SetActiveShader(FShader *sh);
- 	void ApplyMatrices(VSMatrix *proj, VSMatrix *view);
- 	FShader *GetActiveShader() const
- 	{
- 		return mActiveShader;
- 	}
  
! 	FShader *Get(unsigned int eff, bool alphateston)
  	{
  		// indices 0-2 match the warping modes, 3 is brightmap, 4 no texture, the following are custom
- 		if (!alphateston && eff <= 3)
- 		{
- 			return mTextureEffectsNAT[eff];	// Non-alphatest shaders are only created for default, warp1+2 and brightmap. The rest won't get used anyway
- 		}
  		if (eff < mTextureEffects.Size())
  		{
  			return mTextureEffects[eff];
  		}
  		return NULL;
  	}
  };
  
  #define FIRST_USER_SHADER 12
  
- enum
- {
- 	LIGHTBUF_BINDINGPOINT = 1
- };
  
  #endif
  
--- 136,160 ----
  public:
  	FShaderManager();
  	~FShaderManager();
  	int Find(const char *mame);
  	FShader *BindEffect(int effect);
  	void SetActiveShader(FShader *sh);
  
! 	FShaderContainer *Get(unsigned int eff)
  	{
  		// indices 0-2 match the warping modes, 3 is brightmap, 4 no texture, the following are custom
  		if (eff < mTextureEffects.Size())
  		{
  			return mTextureEffects[eff];
  		}
  		return NULL;
  	}
+ 
+ 	void Recompile();
  };
  
  #define FIRST_USER_SHADER 12
  
  
  #endif
  
