*** doom/src/main/jni/gzdoom/src/gl/renderer/gl_renderstate.cpp	2017-06-20 19:11:53.529649767 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/renderer/gl_renderstate.cpp	2017-06-24 18:30:22.320334094 -0400
***************
*** 53,65 ****
  void gl_SetTextureMode(int type);
  
  FRenderState gl_RenderState;
  
! CVAR(Bool, gl_direct_state_change, true, 0)
  
  
- static VSMatrix identityMatrix(1);
- TArray<VSMatrix> gl_MatrixStack;
- 
  //==========================================================================
  //
  //
--- 53,63 ----
  void gl_SetTextureMode(int type);
  
  FRenderState gl_RenderState;
+ int FStateAttr::ChangeCounter;
  
! CVAR(Bool, gl_direct_state_change, false, 0)
  
  
  //==========================================================================
  //
  //
***************
*** 69,233 ****
  void FRenderState::Reset()
  {
  	mTextureEnabled = true;
! 	mBrightmapEnabled = mFogEnabled = mGlowEnabled = false;
! 	mFogColor.d = -1;
! 	mTextureMode = -1;
! 	mLightIndex = -1;
! 	mDesaturation = 0;
  	mSrcBlend = GL_SRC_ALPHA;
  	mDstBlend = GL_ONE_MINUS_SRC_ALPHA;
  	mAlphaThreshold = 0.5f;
  	mBlendEquation = GL_FUNC_ADD;
! 	mObjectColor = 0xffffffff;
  	m2D = true;
- 	mVertexBuffer = mCurrentVertexBuffer = NULL;
- 	mColormapState = CM_DEFAULT;
- 	mLightParms[3] = -1.f;
- 	mSpecialEffect = EFF_NONE;
- 	mClipHeightTop = 65536.f;
- 	mClipHeightBottom = -65536.f;
- 
- 	stSrcBlend = stDstBlend = -1;
- 	stBlendEquation = -1;
- 	stAlphaThreshold = -1.f;
  }
  
  //==========================================================================
  //
! // Apply shader settings
  //
  //==========================================================================
  
! bool FRenderState::ApplyShader()
  {
! 	if (mSpecialEffect > EFF_NONE)
  	{
! 		activeShader = GLRenderer->mShaderManager->BindEffect(mSpecialEffect);
  	}
! 	else
  	{
! 		activeShader = GLRenderer->mShaderManager->Get(mTextureEnabled ? mEffectState : 4, mAlphaThreshold >= 0.f);
! 		activeShader->Bind();
  	}
! 
! 	int fogset = 0;
! 
! 	if (mFogEnabled)
  	{
! 		if ((mFogColor & 0xffffff) == 0)
  		{
! 			fogset = gl_fogmode;
  		}
! 		else
  		{
! 			fogset = -gl_fogmode;
  		}
  	}
  
! 	glVertexAttrib4fv(VATTR_COLOR, mColor.vec);
  
- 	activeShader->muDesaturation.Set(mDesaturation / 255.f);
- 	activeShader->muFogEnabled.Set(fogset);
- 	activeShader->muTextureMode.Set(mTextureMode);
- 	activeShader->muCameraPos.Set(mCameraPos.vec);
- 	activeShader->muLightParms.Set(mLightParms);
- 	activeShader->muFogColor.Set(mFogColor);
- 	activeShader->muObjectColor.Set(mObjectColor);
- 	activeShader->muDynLightColor.Set(mDynColor.vec);
- 	activeShader->muInterpolationFactor.Set(mInterpolationFactor);
- 	activeShader->muClipHeightTop.Set(mClipHeightTop);
- 	activeShader->muClipHeightBottom.Set(mClipHeightBottom);
- 	activeShader->muTimer.Set(gl_frameMS * mShaderTimer / 1000.f);
- 	activeShader->muAlphaThreshold.Set(mAlphaThreshold);
- 	activeShader->muLightIndex.Set(mLightIndex);	// will always be -1 for now
  
! 	if (mGlowEnabled)
! 	{
! 		activeShader->muGlowTopColor.Set(mGlowTop.vec);
! 		activeShader->muGlowBottomColor.Set(mGlowBottom.vec);
! 		activeShader->muGlowTopPlane.Set(mGlowTopPlane.vec);
! 		activeShader->muGlowBottomPlane.Set(mGlowBottomPlane.vec);
! 		activeShader->currentglowstate = 1;
! 	}
! 	else if (activeShader->currentglowstate)
  	{
! 		// if glowing is on, disable it.
! 		static const float nulvec[] = { 0.f, 0.f, 0.f, 0.f };
! 		activeShader->muGlowTopColor.Set(nulvec);
! 		activeShader->muGlowBottomColor.Set(nulvec);
! 		activeShader->muGlowTopPlane.Set(nulvec);
! 		activeShader->muGlowBottomPlane.Set(nulvec);
! 		activeShader->currentglowstate = 0;
  	}
! 
! 	if (mColormapState != activeShader->currentfixedcolormap)
  	{
! 		float r, g, b;
! 		activeShader->currentfixedcolormap = mColormapState;
! 		if (mColormapState == CM_DEFAULT)
  		{
! 			activeShader->muFixedColormap.Set(0);
! 		}
! 		else if (mColormapState < CM_MAXCOLORMAP)
! 		{
! 			FSpecialColormap *scm = &SpecialColormaps[gl_fixedcolormap - CM_FIRSTSPECIALCOLORMAP];
! 			float m[] = { scm->ColorizeEnd[0] - scm->ColorizeStart[0],
! 				scm->ColorizeEnd[1] - scm->ColorizeStart[1], scm->ColorizeEnd[2] - scm->ColorizeStart[2], 0.f };
  
! 			activeShader->muFixedColormap.Set(1);
! 			activeShader->muColormapStart.Set(scm->ColorizeStart[0], scm->ColorizeStart[1], scm->ColorizeStart[2], 0.f);
! 			activeShader->muColormapRange.Set(m);
  		}
! 		else if (mColormapState == CM_FOGLAYER)
  		{
! 			activeShader->muFixedColormap.Set(3);
  		}
! 		else if (mColormapState == CM_LITE)
  		{
! 			if (gl_enhanced_nightvision)
  			{
! 				r = 0.375f, g = 1.0f, b = 0.375f;
  			}
  			else
  			{
! 				r = g = b = 1.f;
  			}
- 			activeShader->muFixedColormap.Set(2);
- 			activeShader->muColormapStart.Set(r, g, b, 1.f);
  		}
! 		else if (mColormapState >= CM_TORCH)
  		{
! 			int flicker = mColormapState - CM_TORCH;
! 			r = (0.8f + (7 - flicker) / 70.0f);
! 			if (r > 1.0f) r = 1.0f;
! 			b = g = r;
! 			if (gl_enhanced_nightvision) b = g * 0.75f;
! 			activeShader->muFixedColormap.Set(2);
! 			activeShader->muColormapStart.Set(r, g, b, 1.f);
  		}
! 	}
! 	if (mTextureMatrixEnabled)
! 	{
! 		mTextureMatrix.matrixToGL(activeShader->texturematrix_index);
! 		activeShader->currentTextureMatrixState = true;
! 	}
! 	else if (activeShader->currentTextureMatrixState)
! 	{
! 		activeShader->currentTextureMatrixState = false;
! 		identityMatrix.matrixToGL(activeShader->texturematrix_index);
! 	}
  
! 	if (mModelMatrixEnabled)
! 	{
! 		mModelMatrix.matrixToGL(activeShader->modelmatrix_index);
! 		activeShader->currentModelMatrixState = true;
! 	}
! 	else if (activeShader->currentModelMatrixState)
! 	{
! 		activeShader->currentModelMatrixState = false;
! 		identityMatrix.matrixToGL(activeShader->modelmatrix_index);
  	}
! 	return true;
  }
  
  
--- 67,259 ----
  void FRenderState::Reset()
  {
  	mTextureEnabled = true;
! 	mBrightmapEnabled = mFogEnabled = mGlowEnabled = mLightEnabled = false;
! 	ffTextureEnabled = ffFogEnabled = false;
! 	mSpecialEffect = ffSpecialEffect = EFF_NONE;
! 	mFogColor.d = ffFogColor.d = -1;
! 	mFogDensity = ffFogDensity = 0;
! 	mTextureMode = ffTextureMode = -1;
  	mSrcBlend = GL_SRC_ALPHA;
  	mDstBlend = GL_ONE_MINUS_SRC_ALPHA;
+ 	glSrcBlend = glDstBlend = -1;
+ 	glAlphaFunc = -1;
+ 	mAlphaFunc = GL_GEQUAL;
  	mAlphaThreshold = 0.5f;
  	mBlendEquation = GL_FUNC_ADD;
! 	glBlendEquation = -1;
  	m2D = true;
  }
  
+ 
  //==========================================================================
  //
! // Set texture shader info
  //
  //==========================================================================
  
! int FRenderState::SetupShader(bool cameratexture, int &shaderindex, int &cm, float warptime)
  {
! 	bool usecmshader;
! 	int softwarewarp = 0;
! 
! 	if (shaderindex == 3)
  	{
! 		// Brightmap should not be used.
! 		if (!mBrightmapEnabled || cm >= CM_FIRSTSPECIALCOLORMAP)
! 		{
! 			shaderindex = 0;
! 		}
  	}
! 
! 	if (gl.shadermodel == 4)
  	{
! 		usecmshader = cm > CM_DEFAULT && cm < CM_MAXCOLORMAP && mTextureMode != TM_MASK;
  	}
! 	else if (gl.shadermodel == 3)
  	{
! 		usecmshader = (cameratexture || gl_colormap_shader) && 
! 			cm > CM_DEFAULT && cm < CM_MAXCOLORMAP && mTextureMode != TM_MASK;
! 
! 		if (!gl_brightmap_shader && shaderindex == 3) 
  		{
! 			shaderindex = 0;
  		}
! 		else if (!gl_warp_shader && shaderindex !=3)
  		{
! 			if (shaderindex <= 2) softwarewarp = shaderindex;
! 			shaderindex = 0;
  		}
  	}
+ 	else
+ 	{
+ 		usecmshader = cameratexture;
+ 		softwarewarp = shaderindex > 0 && shaderindex < 3? shaderindex : 0;
+ 		shaderindex = 0;
+ 	}
  
! 	mEffectState = shaderindex;
! 	mColormapState = usecmshader? cm : CM_DEFAULT;
! 	if (usecmshader) cm = CM_DEFAULT;
! 	mWarpTime = warptime;
! 	return softwarewarp;
! }
  
  
! //==========================================================================
! //
! // Apply shader settings
! //
! //==========================================================================
! 
! bool FRenderState::ApplyShader()
! {
! 	bool useshaders = false;
! 	FShader *activeShader = NULL;
! 
! 	if (mSpecialEffect > 0 && gl.shadermodel > 2)
  	{
! 		activeShader = GLRenderer->mShaderManager->BindEffect(mSpecialEffect);
  	}
! 	else
  	{
! 		switch (gl.shadermodel)
  		{
! 		case 2:
! 			useshaders = (mTextureEnabled && mColormapState != CM_DEFAULT);
! 			break;
! 
! 		case 3:
! 			useshaders = (
! 				mEffectState != 0 ||	// special shaders
! 				(mFogEnabled && (gl_fogmode == 2 || gl_fog_shader) && gl_fogmode != 0) || // fog requires a shader
! 				(mTextureEnabled && (mEffectState != 0 || mColormapState)) ||		// colormap
! 				mGlowEnabled		// glow requires a shader
! 				);
! 			break;
! 
! 		case 4:
! 			useshaders = (!m2D || mEffectState != 0 || mColormapState); // all 3D rendering and 2D with texture effects.
! 			break;
  
! 		default:
! 			break;
  		}
! 
! 		if (useshaders)
  		{
! 			FShaderContainer *shd = GLRenderer->mShaderManager->Get(mTextureEnabled? mEffectState : 4);
! 
! 			if (shd != NULL)
! 			{
! 				activeShader = shd->Bind(mColormapState, mGlowEnabled, mWarpTime, mLightEnabled);
! 			}
  		}
! 	}
! 
! 	if (activeShader)
! 	{
! 		int fogset = 0;
! 		if (mFogEnabled)
  		{
! 			if ((mFogColor & 0xffffff) == 0)
  			{
! 				fogset = gl_fogmode;
  			}
  			else
  			{
! 				fogset = -gl_fogmode;
  			}
  		}
! 
! 		if (fogset != activeShader->currentfogenabled)
  		{
! 			glUniform1i(activeShader->fogenabled_index, (activeShader->currentfogenabled = fogset)); 
  		}
! 		if (mTextureMode != activeShader->currenttexturemode)
! 		{
! 			glUniform1i(activeShader->texturemode_index, (activeShader->currenttexturemode = mTextureMode)); 
! 		}
! 		if (activeShader->currentcamerapos.Update(&mCameraPos))
! 		{
! 			glUniform3fv(activeShader->camerapos_index, 1, mCameraPos.vec); 
! 		}
! 		/*if (mLightParms[0] != activeShader->currentlightfactor || 
! 			mLightParms[1] != activeShader->currentlightdist ||
! 			mFogDensity != activeShader->currentfogdensity)*/
! 		{
! 			const float LOG2E = 1.442692f;	// = 1/log(2)
! 			//activeShader->currentlightdist = mLightParms[1];
! 			//activeShader->currentlightfactor = mLightParms[0];
! 			//activeShader->currentfogdensity = mFogDensity;
! 			// premultiply the density with as much as possible here to reduce shader
! 			// execution time.
! 			glVertexAttrib4f(VATTR_FOGPARAMS, mLightParms[0], mLightParms[1], mFogDensity * (-LOG2E / 64000.f), 0);
! 		}
! 		if (mFogColor != activeShader->currentfogcolor)
! 		{
! 			activeShader->currentfogcolor = mFogColor;
  
! 			glUniform4f (activeShader->fogcolor_index, mFogColor.r/255.f, mFogColor.g/255.f, 
! 							mFogColor.b/255.f, 0);
! 		}
! 		if (mGlowEnabled)
! 		{
! 			glUniform4fv(activeShader->glowtopcolor_index, 1, mGlowTop.vec);
! 			glUniform4fv(activeShader->glowbottomcolor_index, 1, mGlowBottom.vec);
! 		}
! 		if (mLightEnabled)
! 		{
! 			glUniform3iv(activeShader->lightrange_index, 1, mNumLights);
! 			glUniform4fv(activeShader->lights_index, mNumLights[2], mLightData);
! 		}
! 		if (glset.lightmode == 8)
! 		{
! 			glUniform3fv(activeShader->dlightcolor_index, 1, mDynLight);
! 		}
! 
! 		return true;
  	}
! 	return false;
  }
  
  
***************
*** 237,283 ****
  //
  //==========================================================================
  
! void FRenderState::Apply()
  {
  	if (!gl_direct_state_change)
  	{
! 		if (mSrcBlend != stSrcBlend || mDstBlend != stDstBlend)
  		{
! 			stSrcBlend = mSrcBlend;
! 			stDstBlend = mDstBlend;
  			glBlendFunc(mSrcBlend, mDstBlend);
  		}
! 		if (mBlendEquation != stBlendEquation)
  		{
! 			stBlendEquation = mBlendEquation;
! 			glBlendEquation(mBlendEquation);
  		}
  	}
  
! 	if (mVertexBuffer != mCurrentVertexBuffer)
  	{
! 		if (mVertexBuffer == NULL) glBindBuffer(GL_ARRAY_BUFFER, 0);
! 		else mVertexBuffer->BindVBO();
! 		mCurrentVertexBuffer = mVertexBuffer;
! 	}
! 	ApplyShader();
! }
! 
  
  
! void FRenderState::ApplyMatrices()
! {
! 	if (GLRenderer->mShaderManager != NULL)
! 	{
! 		GLRenderer->mShaderManager->ApplyMatrices(&mProjectionMatrix, &mViewMatrix);
  	}
  }
  
- void FRenderState::ApplyLightIndex(int index)
- {
- 	if (GLRenderer->mLights->GetBufferType() == GL_UNIFORM_BUFFER && index > -1)
- 	{
- 		index = GLRenderer->mLights->BindUBO(index);
- 	}
- 	activeShader->muLightIndex.Set(index);
- }
\ No newline at end of file
--- 263,362 ----
  //
  //==========================================================================
  
! void FRenderState::Apply(bool forcenoshader)
  {
  	if (!gl_direct_state_change)
  	{
! 		if (mSrcBlend != glSrcBlend || mDstBlend != glDstBlend)
  		{
! 			glSrcBlend = mSrcBlend;
! 			glDstBlend = mDstBlend;
  			glBlendFunc(mSrcBlend, mDstBlend);
  		}
! 		if (mAlphaFunc != glAlphaFunc || mAlphaThreshold != glAlphaThreshold)
! 		{
! 			glAlphaFunc = mAlphaFunc;
! 			glAlphaThreshold = mAlphaThreshold;
! #ifdef __ANDROID__ //WTF is this a bug?
! 			::glAlphaFunc(mAlphaFunc, mAlphaThreshold);
! #else
! 			::glBlendFunc(mAlphaFunc, mAlphaThreshold);
! #endif
! 		}
! 		if (mAlphaTest != glAlphaTest)
! 		{
! 			glAlphaTest = mAlphaTest;
! 			if (mAlphaTest) glEnable(GL_ALPHA_TEST);
! 			else glDisable(GL_ALPHA_TEST);
! 		}
! 		if (mBlendEquation != glBlendEquation)
  		{
! 			glBlendEquation = mBlendEquation;
! 			::glBlendEquation(mBlendEquation);
  		}
  	}
  
! 	if (forcenoshader || !ApplyShader())
  	{
! 		GLRenderer->mShaderManager->SetActiveShader(NULL);
! 		if (mTextureMode != ffTextureMode)
! 		{
! 			gl_SetTextureMode((ffTextureMode = mTextureMode));
! 		}
! 		if (mTextureEnabled != ffTextureEnabled)
! 		{
! 			if ((ffTextureEnabled = mTextureEnabled)) glEnable(GL_TEXTURE_2D);
! 			else glDisable(GL_TEXTURE_2D);
! 		}
! 		if (mFogEnabled != ffFogEnabled)
! 		{
! 			if ((ffFogEnabled = mFogEnabled)) 
! 			{
! 				glEnable(GL_FOG);
! 			}
! 			else glDisable(GL_FOG);
! 		}
! 		if (mFogEnabled)
! 		{
! 			if (ffFogColor != mFogColor)
! 			{
! 				ffFogColor = mFogColor;
! 				GLfloat FogColor[4]={mFogColor.r/255.0f,mFogColor.g/255.0f,mFogColor.b/255.0f,0.0f};
! 				glFogfv(GL_FOG_COLOR, FogColor);
! 			}
! 			if (ffFogDensity != mFogDensity)
! 			{
! 				glFogf(GL_FOG_DENSITY, mFogDensity/64000.f);
! 				ffFogDensity=mFogDensity;
! 			}
! 		}
! 		if (mSpecialEffect != ffSpecialEffect)
! 		{
! 			switch (ffSpecialEffect)
! 			{
! 			case EFF_SPHEREMAP:
! 				glDisable(GL_TEXTURE_GEN_T);
! 				glDisable(GL_TEXTURE_GEN_S);
  
+ 			default:
+ 				break;
+ 			}
+ 			switch (mSpecialEffect)
+ 			{
+ 			case EFF_SPHEREMAP:
+ 				// Use sphere mapping for this
+ 				glEnable(GL_TEXTURE_GEN_T);
+ 				glEnable(GL_TEXTURE_GEN_S);
+ 				glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
+ 				glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
+ 				break;
  
! 			default:
! 				break;
! 			}
! 			ffSpecialEffect = mSpecialEffect;
! 		}
  	}
+ 
  }
  
