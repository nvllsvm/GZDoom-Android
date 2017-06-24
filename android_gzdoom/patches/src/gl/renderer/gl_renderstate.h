*** doom/src/main/jni/gzdoom/src/gl/renderer/gl_renderstate.h	2017-06-20 19:11:53.529649767 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/renderer/gl_renderstate.h	2017-06-24 18:34:00.772732083 -0400
***************
*** 10,44 ****
  #include "r_defs.h"
  #include "r_data/r_translate.h"
  
- class FVertexBuffer;
- class FShader;
- extern TArray<VSMatrix> gl_MatrixStack;
- 
  EXTERN_CVAR(Bool, gl_direct_state_change)
  
! struct FStateVec4
  {
  	float vec[4];
  
  	void Set(float r, float g, float b, float a)
  	{
  		vec[0] = r;
  		vec[1] = g;
  		vec[2] = b;
  		vec[3] = a;
  	}
  };
  
  
  enum EEffect
  {
! 	EFF_NONE=-1,
  	EFF_FOGBOUNDARY,
  	EFF_SPHEREMAP,
- 	EFF_BURN,
- 	EFF_STENCIL,
- 
- 	MAX_EFFECTS
  };
  
  class FRenderState
--- 10,92 ----
  #include "r_defs.h"
  #include "r_data/r_translate.h"
  
  EXTERN_CVAR(Bool, gl_direct_state_change)
  
! struct FStateAttr
! {
! 	static int ChangeCounter;
! 	int mLastChange;
! 
! 	FStateAttr()
! 	{
! 		mLastChange = -1;
! 	}
! 
! 	bool operator == (const FStateAttr &other)
! 	{
! 		return mLastChange == other.mLastChange;
! 	}
! 
! 	bool operator != (const FStateAttr &other)
! 	{
! 		return mLastChange != other.mLastChange;
! 	}
! 
! };
! 
! struct FStateVec3 : public FStateAttr
! {
! 	float vec[3];
! 
! 	bool Update(FStateVec3 *other)
! 	{
! 		if (mLastChange != other->mLastChange)
! 		{
! 			*this = *other;
! 			return true;
! 		}
! 		return false;
! 	}
! 
! 	void Set(float x, float y, float z)
! 	{
! 		vec[0] = x;
! 		vec[1] = z;
! 		vec[2] = y;
! 		mLastChange = ++ChangeCounter;
! 	}
! };
! 
! struct FStateVec4 : public FStateAttr
  {
  	float vec[4];
  
+ 	bool Update(FStateVec4 *other)
+ 	{
+ 		if (mLastChange != other->mLastChange)
+ 		{
+ 			*this = *other;
+ 			return true;
+ 		}
+ 		return false;
+ 	}
+ 
  	void Set(float r, float g, float b, float a)
  	{
  		vec[0] = r;
  		vec[1] = g;
  		vec[2] = b;
  		vec[3] = a;
+ 		mLastChange = ++ChangeCounter;
  	}
  };
  
  
  enum EEffect
  {
! 	EFF_NONE,
  	EFF_FOGBOUNDARY,
  	EFF_SPHEREMAP,
  };
  
  class FRenderState
***************
*** 46,97 ****
  	bool mTextureEnabled;
  	bool mFogEnabled;
  	bool mGlowEnabled;
  	bool mBrightmapEnabled;
- 	int mLightIndex;
  	int mSpecialEffect;
  	int mTextureMode;
! 	int mDesaturation;
! 	int mSoftLight;
! 	float mLightParms[4];
  	int mSrcBlend, mDstBlend;
  	float mAlphaThreshold;
- 	int mBlendEquation;
  	bool mAlphaTest;
  	bool m2D;
! 	bool mModelMatrixEnabled;
! 	bool mTextureMatrixEnabled;
! 	float mInterpolationFactor;
! 	float mClipHeightTop, mClipHeightBottom;
! 	float mShaderTimer;
! 
! 	FVertexBuffer *mVertexBuffer, *mCurrentVertexBuffer;
! 	FStateVec4 mColor;
! 	FStateVec4 mCameraPos;
  	FStateVec4 mGlowTop, mGlowBottom;
- 	FStateVec4 mGlowTopPlane, mGlowBottomPlane;
  	PalEntry mFogColor;
! 	PalEntry mObjectColor;
! 	FStateVec4 mDynColor;
  
  	int mEffectState;
  	int mColormapState;
  
! 	float stAlphaThreshold;
! 	int stSrcBlend, stDstBlend;
! 	bool stAlphaTest;
! 	int stBlendEquation;
! 
! 	FShader *activeShader;
  
  	bool ApplyShader();
  
  public:
- 
- 	VSMatrix mProjectionMatrix;
- 	VSMatrix mViewMatrix;
- 	VSMatrix mModelMatrix;
- 	VSMatrix mTextureMatrix;
- 
  	FRenderState()
  	{
  		Reset();
--- 94,139 ----
  	bool mTextureEnabled;
  	bool mFogEnabled;
  	bool mGlowEnabled;
+ 	bool mLightEnabled;
  	bool mBrightmapEnabled;
  	int mSpecialEffect;
  	int mTextureMode;
! 	float mDynLight[3];
! 	float mLightParms[2];
! 	int mNumLights[3];
! 	float *mLightData;
  	int mSrcBlend, mDstBlend;
+ 	int mAlphaFunc;
  	float mAlphaThreshold;
  	bool mAlphaTest;
+ 	int mBlendEquation;
  	bool m2D;
! 
! 	FStateVec3 mCameraPos;
  	FStateVec4 mGlowTop, mGlowBottom;
  	PalEntry mFogColor;
! 	float mFogDensity;
  
  	int mEffectState;
  	int mColormapState;
+ 	float mWarpTime;
  
! 	int glSrcBlend, glDstBlend;
! 	int glAlphaFunc;
! 	float glAlphaThreshold;
! 	bool glAlphaTest;
! 	int glBlendEquation;
! 
! 	bool ffTextureEnabled;
! 	bool ffFogEnabled;
! 	int ffTextureMode;
! 	int ffSpecialEffect;
! 	PalEntry ffFogColor;
! 	float ffFogDensity;
  
  	bool ApplyShader();
  
  public:
  	FRenderState()
  	{
  		Reset();
***************
*** 99,176 ****
  
  	void Reset();
  
! 	void SetMaterial(FMaterial *mat, int clampmode, int translation, int overrideshader, bool alphatexture)
! 	{
! 		// textures without their own palette are a special case for use as an alpha texture:
! 		// They use the color index directly as an alpha value instead of using the palette's red.
! 		// To handle this case, we need to set a special translation for such textures.
! 		if (alphatexture)
! 		{
! 			if (mat->tex->UseBasePalette()) translation = TRANSLATION(TRANSLATION_Standard, 8);
! 		}
! 		mEffectState = overrideshader >= 0? overrideshader : mat->mShaderIndex;
! 		mShaderTimer = mat->tex->gl_info.shaderspeed;
! 		mat->Bind(clampmode, translation);
! 	}
! 
! 	void Apply();
! 	void ApplyMatrices();
! 	void ApplyLightIndex(int index);
! 
! 	void SetVertexBuffer(FVertexBuffer *vb)
! 	{
! 		mVertexBuffer = vb;
! 	}
! 
! 	void ResetVertexBuffer()
! 	{
! 		// forces rebinding with the next 'apply' call.
! 		mCurrentVertexBuffer = NULL;
! 	}
! 
! 	void SetClipHeightTop(float clip)
! 	{
! 		mClipHeightTop = clip;
! 	}
! 
! 	float GetClipHeightTop()
! 	{
! 		return mClipHeightTop;
! 	}
! 
! 	void SetClipHeightBottom(float clip)
! 	{
! 		mClipHeightBottom = clip;
! 	}
! 
! 	float GetClipHeightBottom()
! 	{
! 		return mClipHeightBottom;
! 	}
! 
! 	void SetColor(float r, float g, float b, float a = 1.f, int desat = 0)
! 	{
! 		mColor.Set(r, g, b, a);
! 		mDesaturation = desat;
! 	}
! 
! 	void SetColor(PalEntry pe, int desat = 0)
! 	{
! 		mColor.Set(pe.r/255.f, pe.g/255.f, pe.b/255.f, pe.a/255.f);
! 		mDesaturation = desat;
! 	}
! 
! 	void SetColorAlpha(PalEntry pe, float alpha = 1.f, int desat = 0)
! 	{
! 		mColor.Set(pe.r/255.f, pe.g/255.f, pe.b/255.f, alpha);
! 		mDesaturation = desat;
! 	}
! 
! 	void ResetColor()
! 	{
! 		mColor.Set(1,1,1,1);
! 		mDesaturation = 0;
! 	}
  
  	void SetTextureMode(int mode)
  	{
--- 141,148 ----
  
  	void Reset();
  
! 	int SetupShader(bool cameratexture, int &shaderindex, int &cm, float warptime);
! 	void Apply(bool forcenoshader = false);
  
  	void SetTextureMode(int mode)
  	{
***************
*** 197,205 ****
  		mGlowEnabled = on;
  	}
  
! 	void SetLightIndex(int n)
  	{
! 		mLightIndex = n;
  	}
  
  	void EnableBrightmap(bool on)
--- 169,177 ----
  		mGlowEnabled = on;
  	}
  
! 	void EnableLight(bool on)
  	{
! 		mLightEnabled = on;
  	}
  
  	void EnableBrightmap(bool on)
***************
*** 207,225 ****
  		mBrightmapEnabled = on;
  	}
  
- 	void EnableModelMatrix(bool on)
- 	{
- 		mModelMatrixEnabled = on;
- 	}
- 
- 	void EnableTextureMatrix(bool on)
- 	{
- 		mTextureMatrixEnabled = on;
- 	}
- 
  	void SetCameraPos(float x, float y, float z)
  	{
! 		mCameraPos.Set(x, z, y, 0);
  	}
  
  	void SetGlowParams(float *t, float *b)
--- 179,187 ----
  		mBrightmapEnabled = on;
  	}
  
  	void SetCameraPos(float x, float y, float z)
  	{
! 		mCameraPos.Set(x,y,z);
  	}
  
  	void SetGlowParams(float *t, float *b)
***************
*** 228,266 ****
  		mGlowBottom.Set(b[0], b[1], b[2], b[3]);
  	}
  
! 	void SetSoftLightLevel(int level)
! 	{
! 		if (glset.lightmode == 8) mLightParms[3] = level / 255.f;
! 		else mLightParms[3] = -1.f;
! 	}
! 
! 	void SetGlowPlanes(const secplane_t &top, const secplane_t &bottom)
  	{
! 		mGlowTopPlane.Set(FIXED2FLOAT(top.a), FIXED2FLOAT(top.b), FIXED2FLOAT(top.ic), FIXED2FLOAT(top.d));
! 		mGlowBottomPlane.Set(FIXED2FLOAT(bottom.a), FIXED2FLOAT(bottom.b), FIXED2FLOAT(bottom.ic), FIXED2FLOAT(bottom.d));
! 	}
! 
! 	void SetDynLight(float r, float g, float b)
! 	{
! 		mDynColor.Set(r, g, b, 0);
! 	}
! 
! 	void SetObjectColor(PalEntry pe)
! 	{
! 		mObjectColor = pe;
  	}
  
  	void SetFog(PalEntry c, float d)
  	{
- 		const float LOG2E = 1.442692f;	// = 1/log(2)
  		mFogColor = c;
! 		if (d >= 0.0f) mLightParms[2] = d * (-LOG2E / 64000.f);
  	}
  
  	void SetLightParms(float f, float d)
  	{
! 		mLightParms[1] = f;
! 		mLightParms[0] = d;
  	}
  
  	void SetFixedColormap(int cm)
--- 190,220 ----
  		mGlowBottom.Set(b[0], b[1], b[2], b[3]);
  	}
  
! 	void SetDynLight(float r,float g, float b)
  	{
! 		mDynLight[0] = r;
! 		mDynLight[1] = g;
! 		mDynLight[2] = b;
  	}
  
  	void SetFog(PalEntry c, float d)
  	{
  		mFogColor = c;
! 		if (d >= 0.0f) mFogDensity = d;
  	}
  
  	void SetLightParms(float f, float d)
  	{
! 		mLightParms[0] = f;
! 		mLightParms[1] = d;
! 	}
! 
! 	void SetLights(int *numlights, float *lightdata)
! 	{
! 		mNumLights[0] = numlights[0];
! 		mNumLights[1] = numlights[1];
! 		mNumLights[2] = numlights[2];
! 		mLightData = lightdata;	// caution: the data must be preserved by the caller until the 'apply' call!
  	}
  
  	void SetFixedColormap(int cm)
***************
*** 288,295 ****
  
  	void AlphaFunc(int func, float thresh)
  	{
! 		if (func == GL_GREATER) mAlphaThreshold = thresh;
! 		else mAlphaThreshold = thresh - 0.001f;
  	}
  
  	void BlendEquation(int eq)
--- 242,269 ----
  
  	void AlphaFunc(int func, float thresh)
  	{
! 		if (!gl_direct_state_change)
! 		{
! 			mAlphaFunc = func;
! 			mAlphaThreshold = thresh;
! 		}
! 		else
! 		{
! 			::glAlphaFunc(func, thresh);
! 		}
! 	}
! 
! 	void EnableAlphaTest(bool on)
! 	{
! 		if (!gl_direct_state_change)
! 		{
! 			mAlphaTest = on;
! 		}
! 		else
! 		{
! 			if (on) glEnable(GL_ALPHA_TEST);
! 			else glDisable(GL_ALPHA_TEST);
! 		}
  	}
  
  	void BlendEquation(int eq)
***************
*** 300,306 ****
  		}
  		else
  		{
! 			glBlendEquation(eq);
  		}
  	}
  
--- 274,280 ----
  		}
  		else
  		{
! 			::glBlendEquation(eq);
  		}
  	}
  
***************
*** 308,318 ****
  	{
  		m2D = on;
  	}
- 
- 	void SetInterpolationFactor(float fac)
- 	{
- 		mInterpolationFactor = fac;
- 	}
  };
  
  extern FRenderState gl_RenderState;
--- 282,287 ----
