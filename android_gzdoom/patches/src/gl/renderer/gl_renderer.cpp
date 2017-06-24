*** doom/src/main/jni/gzdoom/src/gl/renderer/gl_renderer.cpp	2017-06-20 19:11:53.529649767 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/renderer/gl_renderer.cpp	2017-06-24 08:07:14.138255754 -0400
***************
*** 52,73 ****
  
  #include "gl/system/gl_interface.h"
  #include "gl/system/gl_framebuffer.h"
- #include "gl/system/gl_cvars.h"
  #include "gl/renderer/gl_renderer.h"
  #include "gl/renderer/gl_lightdata.h"
  #include "gl/renderer/gl_renderstate.h"
  #include "gl/data/gl_data.h"
  #include "gl/data/gl_vertexbuffer.h"
  #include "gl/scene/gl_drawinfo.h"
  #include "gl/shaders/gl_shader.h"
  #include "gl/textures/gl_texture.h"
  #include "gl/textures/gl_translate.h"
  #include "gl/textures/gl_material.h"
- #include "gl/textures/gl_samplers.h"
  #include "gl/utility/gl_clock.h"
  #include "gl/utility/gl_templates.h"
  #include "gl/models/gl_models.h"
- #include "gl/dynlights/gl_lightbuffer.h"
  
  //===========================================================================
  // 
--- 52,71 ----
  
  #include "gl/system/gl_interface.h"
  #include "gl/system/gl_framebuffer.h"
  #include "gl/renderer/gl_renderer.h"
  #include "gl/renderer/gl_lightdata.h"
  #include "gl/renderer/gl_renderstate.h"
  #include "gl/data/gl_data.h"
  #include "gl/data/gl_vertexbuffer.h"
+ #include "gl/dynlights/gl_lightbuffer.h"
  #include "gl/scene/gl_drawinfo.h"
  #include "gl/shaders/gl_shader.h"
  #include "gl/textures/gl_texture.h"
  #include "gl/textures/gl_translate.h"
  #include "gl/textures/gl_material.h"
  #include "gl/utility/gl_clock.h"
  #include "gl/utility/gl_templates.h"
  #include "gl/models/gl_models.h"
  
  //===========================================================================
  // 
***************
*** 75,80 ****
--- 73,80 ----
  //
  //===========================================================================
  
+ EXTERN_CVAR(Bool, gl_render_segs)
+ 
  //-----------------------------------------------------------------------------
  //
  // Initialize
***************
*** 92,102 ****
  	mViewVector = FVector2(0,0);
  	mCameraPos = FVector3(0,0,0);
  	mVBO = NULL;
- 	mSkyVBO = NULL;
  	gl_spriteindex = 0;
  	mShaderManager = NULL;
! 	glpart2 = glpart = mirrortexture = NULL;
! 	mLights = NULL;
  }
  
  void FGLRenderer::Initialize()
--- 92,100 ----
  	mViewVector = FVector2(0,0);
  	mCameraPos = FVector3(0,0,0);
  	mVBO = NULL;
  	gl_spriteindex = 0;
  	mShaderManager = NULL;
! 	glpart2 = glpart = gllight = mirrortexture = NULL;
  }
  
  void FGLRenderer::Initialize()
***************
*** 104,134 ****
  	glpart2 = FTexture::CreateTexture(Wads.GetNumForFullName("glstuff/glpart2.png"), FTexture::TEX_MiscPatch);
  	glpart = FTexture::CreateTexture(Wads.GetNumForFullName("glstuff/glpart.png"), FTexture::TEX_MiscPatch);
  	mirrortexture = FTexture::CreateTexture(Wads.GetNumForFullName("glstuff/mirror.png"), FTexture::TEX_MiscPatch);
  
  	mVBO = new FFlatVertexBuffer;
- 	mSkyVBO = new FSkyVertexBuffer;
- 	mModelVBO = new FModelVertexBuffer;
- 	mLights = new FLightBuffer();
- 	gl_RenderState.SetVertexBuffer(mVBO);
  	mFBID = 0;
  	SetupLevel();
  	mShaderManager = new FShaderManager;
! 	mSamplerManager = new FSamplerManager;
  }
  
  FGLRenderer::~FGLRenderer() 
  {
  	gl_DeleteAllAttachedLights();
  	FMaterial::FlushAll();
  	if (mShaderManager != NULL) delete mShaderManager;
- 	if (mSamplerManager != NULL) delete mSamplerManager;
  	if (mVBO != NULL) delete mVBO;
- 	if (mModelVBO) delete mModelVBO;
- 	if (mSkyVBO != NULL) delete mSkyVBO;
- 	if (mLights != NULL) delete mLights;
  	if (glpart2) delete glpart2;
  	if (glpart) delete glpart;
  	if (mirrortexture) delete mirrortexture;
  	if (mFBID != 0) glDeleteFramebuffers(1, &mFBID);
  }
  
--- 102,129 ----
  	glpart2 = FTexture::CreateTexture(Wads.GetNumForFullName("glstuff/glpart2.png"), FTexture::TEX_MiscPatch);
  	glpart = FTexture::CreateTexture(Wads.GetNumForFullName("glstuff/glpart.png"), FTexture::TEX_MiscPatch);
  	mirrortexture = FTexture::CreateTexture(Wads.GetNumForFullName("glstuff/mirror.png"), FTexture::TEX_MiscPatch);
+ 	gllight = FTexture::CreateTexture(Wads.GetNumForFullName("glstuff/gllight.png"), FTexture::TEX_MiscPatch);
+ 	//gllight = FTexture::CreateTexture(Wads.GetNumForFullName("glstuff/mirror.png"), FTexture::TEX_MiscPatch);
  
  	mVBO = new FFlatVertexBuffer;
  	mFBID = 0;
  	SetupLevel();
  	mShaderManager = new FShaderManager;
! 	//mThreadManager = new FGLThreadManager;
  }
  
  FGLRenderer::~FGLRenderer() 
  {
+ 	gl_CleanModelData();
  	gl_DeleteAllAttachedLights();
  	FMaterial::FlushAll();
+ 	//if (mThreadManager != NULL) delete mThreadManager;
  	if (mShaderManager != NULL) delete mShaderManager;
  	if (mVBO != NULL) delete mVBO;
  	if (glpart2) delete glpart2;
  	if (glpart) delete glpart;
  	if (mirrortexture) delete mirrortexture;
+ 	if (gllight) delete gllight;
  	if (mFBID != 0) glDeleteFramebuffers(1, &mFBID);
  }
  
***************
*** 217,225 ****
  
  bool FGLRenderer::StartOffscreen()
  {
! 	if (mFBID == 0) glGenFramebuffers(1, &mFBID);
! 	glBindFramebuffer(GL_FRAMEBUFFER, mFBID);
! 	return true;
  }
  
  //===========================================================================
--- 212,224 ----
  
  bool FGLRenderer::StartOffscreen()
  {
! 	if (gl.flags & RFL_FRAMEBUFFER)
! 	{
! 		if (mFBID == 0) glGenFramebuffers(1, &mFBID);
! 		glBindFramebuffer(GL_FRAMEBUFFER, mFBID);
! 		return true;
! 	}
! 	return false;
  }
  
  //===========================================================================
***************
*** 230,236 ****
  
  void FGLRenderer::EndOffscreen()
  {
! 	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
  }
  
  //===========================================================================
--- 229,238 ----
  
  void FGLRenderer::EndOffscreen()
  {
! 	if (gl.flags & RFL_FRAMEBUFFER)
! 	{
! 		glBindFramebuffer(GL_FRAMEBUFFER, 0); 
! 	}
  }
  
  //===========================================================================
***************
*** 241,250 ****
  
  unsigned char *FGLRenderer::GetTextureBuffer(FTexture *tex, int &w, int &h)
  {
! 	FMaterial * gltex = FMaterial::ValidateTexture(tex, false);
  	if (gltex)
  	{
! 		return gltex->CreateTexBuffer(0, w, h);
  	}
  	return NULL;
  }
--- 243,252 ----
  
  unsigned char *FGLRenderer::GetTextureBuffer(FTexture *tex, int &w, int &h)
  {
! 	FMaterial * gltex = FMaterial::ValidateTexture(tex);
  	if (gltex)
  	{
! 		return gltex->CreateTexBuffer(CM_DEFAULT, 0, w, h);
  	}
  	return NULL;
  }
***************
*** 267,291 ****
  	int borderHeight = (trueHeight - height) / 2;
  
  	glViewport(0, 0, width, trueHeight);
! 	gl_RenderState.mProjectionMatrix.loadIdentity();
! 	gl_RenderState.mProjectionMatrix.ortho(0.0f, width * 1.0f, 0.0f, trueHeight, -1.0f, 1.0f);
! 	gl_RenderState.SetColor(0.f ,0.f ,0.f ,1.f);
  	gl_RenderState.Set2DMode(true);
  	gl_RenderState.EnableTexture(false);
! 	gl_RenderState.Apply();
! 	gl_RenderState.ApplyMatrices(); 
  
- 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
- 	ptr->Set(0, borderHeight, 0, 0, 0); ptr++;
- 	ptr->Set(0, 0, 0, 0, 0); ptr++;
- 	ptr->Set(width, 0, 0, 0, 0); ptr++;
- 	ptr->Set(width, borderHeight, 0, 0, 0); ptr++;
- 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
- 	ptr->Set(0, trueHeight, 0, 0, 0); ptr++;
- 	ptr->Set(0, trueHeight - borderHeight, 0, 0, 0); ptr++;
- 	ptr->Set(width, trueHeight - borderHeight, 0, 0, 0); ptr++;
- 	ptr->Set(width, trueHeight, 0, 0, 0); ptr++;
- 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  	gl_RenderState.EnableTexture(true);
  
  	glViewport(0, (trueHeight - height) / 2, width, height); 
--- 269,297 ----
  	int borderHeight = (trueHeight - height) / 2;
  
  	glViewport(0, 0, width, trueHeight);
! 	glMatrixMode(GL_PROJECTION);
! 	glLoadIdentity();
! 	glOrtho(0.0, width * 1.0, 0.0, trueHeight, -1.0, 1.0);
! 	glMatrixMode(GL_MODELVIEW);
! 	glColor3f(0.f, 0.f, 0.f);
  	gl_RenderState.Set2DMode(true);
  	gl_RenderState.EnableTexture(false);
! 	gl_RenderState.Apply(true);
! 
! 	glBegin(GL_QUADS);
! 	// upper quad
! 	glVertex2i(0, borderHeight);
! 	glVertex2i(0, 0);
! 	glVertex2i(width, 0);
! 	glVertex2i(width, borderHeight);
! 
! 	// lower quad
! 	glVertex2i(0, trueHeight);
! 	glVertex2i(0, trueHeight - borderHeight);
! 	glVertex2i(width, trueHeight - borderHeight);
! 	glVertex2i(width, trueHeight);
! 	glEnd();
  
  	gl_RenderState.EnableTexture(true);
  
  	glViewport(0, (trueHeight - height) / 2, width, height); 
***************
*** 305,349 ****
  	double y = parms.y - parms.top * yscale;
  	double w = parms.destwidth;
  	double h = parms.destheight;
! 	float u1, v1, u2, v2;
! 	int light = 255;
  
! 	FMaterial * gltex = FMaterial::ValidateTexture(img, false);
  
  	if (parms.colorOverlay && (parms.colorOverlay & 0xffffff) == 0)
  	{
  		// Right now there's only black. Should be implemented properly later
! 		light = 255 - APART(parms.colorOverlay);
  		parms.colorOverlay = 0;
  	}
  
  	if (!img->bHasCanvas)
  	{
! 		int translation = 0;
! 		if (!parms.alphaChannel)
  		{
  			if (parms.remap != NULL && !parms.remap->Inactive)
  			{
  				GLTranslationPalette * pal = static_cast<GLTranslationPalette*>(parms.remap->GetNative());
  				if (pal) translation = -pal->GetIndex();
  			}
  		}
- 		gl_SetRenderStyle(parms.style, !parms.masked, false);
- 		gl_RenderState.SetMaterial(gltex, CLAMP_XY_NOMIP, translation, 0, !!(parms.style.Flags & STYLEF_RedIsAlpha));
  
  		u1 = gltex->GetUL();
  		v1 = gltex->GetVT();
  		u2 = gltex->GetUR();
  		v2 = gltex->GetVB();
- 
  	}
  	else
  	{
! 		gl_RenderState.SetMaterial(gltex, CLAMP_XY_NOMIP, 0, -1, false);
! 		u1 = 0.f;
! 		v1 = 1.f;
! 		u2 = 1.f;
! 		v2 = 0.f;
  		gl_RenderState.SetTextureMode(TM_OPAQUE);
  	}
  	
--- 311,357 ----
  	double y = parms.y - parms.top * yscale;
  	double w = parms.destwidth;
  	double h = parms.destheight;
! 	float u1, v1, u2, v2, r, g, b;
! 	float light = 1.f;
  
! 	FMaterial * gltex = FMaterial::ValidateTexture(img);
  
  	if (parms.colorOverlay && (parms.colorOverlay & 0xffffff) == 0)
  	{
  		// Right now there's only black. Should be implemented properly later
! 		light = 1.f - APART(parms.colorOverlay)/255.f;
  		parms.colorOverlay = 0;
  	}
  
  	if (!img->bHasCanvas)
  	{
! 		if (!parms.alphaChannel) 
  		{
+ 			int translation = 0;
  			if (parms.remap != NULL && !parms.remap->Inactive)
  			{
  				GLTranslationPalette * pal = static_cast<GLTranslationPalette*>(parms.remap->GetNative());
  				if (pal) translation = -pal->GetIndex();
  			}
+ 			gltex->BindPatch(CM_DEFAULT, translation);
+ 		}
+ 		else 
+ 		{
+ 			// This is an alpha texture
+ 			gltex->BindPatch(CM_SHADE, 0);
  		}
  
  		u1 = gltex->GetUL();
  		v1 = gltex->GetVT();
  		u2 = gltex->GetUR();
  		v2 = gltex->GetVB();
  	}
  	else
  	{
! 		gltex->Bind(CM_DEFAULT, 0, 0);
! 		u2=1.f;
! 		v2=-1.f;
! 		u1 = v1 = 0.f;
  		gl_RenderState.SetTextureMode(TM_OPAQUE);
  	}
  	
***************
*** 364,380 ****
  		u2 = float(u2 - (parms.texwidth - parms.windowright) / parms.texwidth);
  	}
  
- 	PalEntry color;
  	if (parms.style.Flags & STYLEF_ColorIsFixed)
  	{
! 		color = parms.fillcolor;
  	}
  	else
  	{
! 		color = PalEntry(light, light, light);
  	}
! 	color.a = Scale(parms.alpha, 255, FRACUNIT);
! 
  	// scissor test doesn't use the current viewport for the coordinates, so use real screen coordinates
  	int btm = (SCREENHEIGHT - screen->GetHeight()) / 2;
  	btm = SCREENHEIGHT - btm;
--- 372,388 ----
  		u2 = float(u2 - (parms.texwidth - parms.windowright) / parms.texwidth);
  	}
  
  	if (parms.style.Flags & STYLEF_ColorIsFixed)
  	{
! 		r = RPART(parms.fillcolor)/255.0f;
! 		g = GPART(parms.fillcolor)/255.0f;
! 		b = BPART(parms.fillcolor)/255.0f;
  	}
  	else
  	{
! 		r = g = b = light;
  	}
! 	
  	// scissor test doesn't use the current viewport for the coordinates, so use real screen coordinates
  	int btm = (SCREENHEIGHT - screen->GetHeight()) / 2;
  	btm = SCREENHEIGHT - btm;
***************
*** 383,415 ****
  	int space = (static_cast<OpenGLFrameBuffer*>(screen)->GetTrueHeight()-screen->GetHeight())/2;
  	glScissor(parms.lclip, btm - parms.dclip + space, parms.rclip - parms.lclip, parms.dclip - parms.uclip);
  	
! 	gl_RenderState.SetColor(color);
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, 0.f);
! 	gl_RenderState.Apply();
  
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(x, y, 0, u1, v1); ptr++;
! 	ptr->Set(x, y + h, 0, u1, v2); ptr++;
! 	ptr->Set(x + w, y, 0, u2, v1); ptr++;
! 	ptr->Set(x + w, y + h, 0, u2, v2); ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  
  	if (parms.colorOverlay)
  	{
  		gl_RenderState.SetTextureMode(TM_MASK);
  		gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  		gl_RenderState.BlendEquation(GL_FUNC_ADD);
- 		gl_RenderState.SetColor(PalEntry(parms.colorOverlay));
  		gl_RenderState.Apply();
! 
! 		FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 		ptr->Set(x, y, 0, u1, v1); ptr++;
! 		ptr->Set(x, y + h, 0, u1, v2); ptr++;
! 		ptr->Set(x + w, y, 0, u2, v1); ptr++;
! 		ptr->Set(x + w, y + h, 0, u2, v2); ptr++;
! 		GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  	}
  
  	glScissor(0, 0, screen->GetWidth(), screen->GetHeight());
  	glDisable(GL_SCISSOR_TEST);
  	gl_RenderState.SetTextureMode(TM_MODULATE);
--- 391,438 ----
  	int space = (static_cast<OpenGLFrameBuffer*>(screen)->GetTrueHeight()-screen->GetHeight())/2;
  	glScissor(parms.lclip, btm - parms.dclip + space, parms.rclip - parms.lclip, parms.dclip - parms.uclip);
  	
! 	gl_SetRenderStyle(parms.style, !parms.masked, false);
! 	if (img->bHasCanvas)
! 	{
! 		gl_RenderState.SetTextureMode(TM_OPAQUE);
! 	}
  
! 	glColor4f(r, g, b, FIXED2FLOAT(parms.alpha));
! 	
! 	gl_RenderState.EnableAlphaTest(false);
! 	gl_RenderState.Apply();
! 	glBegin(GL_TRIANGLE_STRIP);
! 	glTexCoord2f(u1, v1);
! 	glVertex2d(x, y);
! 	glTexCoord2f(u1, v2);
! 	glVertex2d(x, y + h);
! 	glTexCoord2f(u2, v1);
! 	glVertex2d(x + w, y);
! 	glTexCoord2f(u2, v2);
! 	glVertex2d(x + w, y + h);
! 	glEnd();
  
  	if (parms.colorOverlay)
  	{
  		gl_RenderState.SetTextureMode(TM_MASK);
  		gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  		gl_RenderState.BlendEquation(GL_FUNC_ADD);
  		gl_RenderState.Apply();
! 		glColor4ub(RPART(parms.colorOverlay),GPART(parms.colorOverlay),BPART(parms.colorOverlay),APART(parms.colorOverlay));
! 		glBegin(GL_TRIANGLE_STRIP);
! 		glTexCoord2f(u1, v1);
! 		glVertex2d(x, y);
! 		glTexCoord2f(u1, v2);
! 		glVertex2d(x, y + h);
! 		glTexCoord2f(u2, v1);
! 		glVertex2d(x + w, y);
! 		glTexCoord2f(u2, v2);
! 		glVertex2d(x + w, y + h);
! 		glEnd();
  	}
  
+ 	gl_RenderState.EnableAlphaTest(true);
+ 	
  	glScissor(0, 0, screen->GetWidth(), screen->GetHeight());
  	glDisable(GL_SCISSOR_TEST);
  	gl_RenderState.SetTextureMode(TM_MODULATE);
***************
*** 426,439 ****
  {
  	PalEntry p = color? (PalEntry)color : GPalette.BaseColors[palcolor];
  	gl_RenderState.EnableTexture(false);
! 	gl_RenderState.SetColorAlpha(p, 1.f);
! 	gl_RenderState.Apply();
! 
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(x1, y1, 0, 0, 0); ptr++;
! 	ptr->Set(x2, y2, 0, 0, 0); ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_LINES);
! 	
  	gl_RenderState.EnableTexture(true);
  }
  
--- 449,460 ----
  {
  	PalEntry p = color? (PalEntry)color : GPalette.BaseColors[palcolor];
  	gl_RenderState.EnableTexture(false);
! 	gl_RenderState.Apply(true);
! 	glColor3ub(p.r, p.g, p.b);
! 	glBegin(GL_LINES);
! 	glVertex2i(x1, y1);
! 	glVertex2i(x2, y2);
! 	glEnd();
  	gl_RenderState.EnableTexture(true);
  }
  
***************
*** 446,458 ****
  {
  	PalEntry p = color? (PalEntry)color : GPalette.BaseColors[palcolor];
  	gl_RenderState.EnableTexture(false);
! 	gl_RenderState.SetColorAlpha(p, 1.f);
! 	gl_RenderState.Apply();
! 
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(x1, y1, 0, 0, 0); ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_POINTS);
! 
  	gl_RenderState.EnableTexture(true);
  }
  
--- 467,477 ----
  {
  	PalEntry p = color? (PalEntry)color : GPalette.BaseColors[palcolor];
  	gl_RenderState.EnableTexture(false);
! 	gl_RenderState.Apply(true);
! 	glColor3ub(p.r, p.g, p.b);
! 	glBegin(GL_POINTS);
! 	glVertex2i(x1, y1);
! 	glEnd();
  	gl_RenderState.EnableTexture(true);
  }
  
***************
*** 464,482 ****
  
  void FGLRenderer::Dim(PalEntry color, float damount, int x1, int y1, int w, int h)
  {
  	gl_RenderState.EnableTexture(false);
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  	gl_RenderState.AlphaFunc(GL_GREATER,0);
! 	gl_RenderState.SetColorAlpha(color, damount);
! 	gl_RenderState.Apply();
  	
- 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
- 	ptr->Set(x1, y1, 0, 0, 0); ptr++;
- 	ptr->Set(x1, y1+h, 0, 0, 0); ptr++;
- 	ptr->Set(x1+w, y1+h, 0, 0, 0); ptr++;
- 	ptr->Set(x1+w, y1, 0, 0, 0); ptr++;
- 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_FAN);
- 
  	gl_RenderState.EnableTexture(true);
  }
  
--- 483,507 ----
  
  void FGLRenderer::Dim(PalEntry color, float damount, int x1, int y1, int w, int h)
  {
+ 	float r, g, b;
+ 	
  	gl_RenderState.EnableTexture(false);
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  	gl_RenderState.AlphaFunc(GL_GREATER,0);
! 	gl_RenderState.Apply(true);
! 	
! 	r = color.r/255.0f;
! 	g = color.g/255.0f;
! 	b = color.b/255.0f;
! 	
! 	glBegin(GL_TRIANGLE_FAN);
! 	glColor4f(r, g, b, damount);
! 	glVertex2i(x1, y1);
! 	glVertex2i(x1, y1 + h);
! 	glVertex2i(x1 + w, y1 + h);
! 	glVertex2i(x1 + w, y1);
! 	glEnd();
  	
  	gl_RenderState.EnableTexture(true);
  }
  
***************
*** 489,499 ****
  {
  	float fU1,fU2,fV1,fV2;
  
! 	FMaterial *gltexture=FMaterial::ValidateTexture(src, false);
  	
  	if (!gltexture) return;
  
! 	gl_RenderState.SetMaterial(gltexture, CLAMP_NONE, 0, -1, false);
  	
  	// scaling is not used here.
  	if (!local_origin)
--- 514,524 ----
  {
  	float fU1,fU2,fV1,fV2;
  
! 	FMaterial *gltexture=FMaterial::ValidateTexture(src);
  	
  	if (!gltexture) return;
  
! 	gltexture->Bind(CM_DEFAULT, 0, 0);
  	
  	// scaling is not used here.
  	if (!local_origin)
***************
*** 510,524 ****
  		fU2 = float(right-left) / src->GetWidth();
  		fV2 = float(bottom-top) / src->GetHeight();
  	}
- 	gl_RenderState.ResetColor();
  	gl_RenderState.Apply();
! 
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(left, top, 0, fU1, fV1); ptr++;
! 	ptr->Set(left, bottom, 0, fU1, fV2); ptr++;
! 	ptr->Set(right, top, 0, fU2, fV1); ptr++;
! 	ptr->Set(right, bottom, 0, fU2, fV2); ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  }
  
  //==========================================================================
--- 535,548 ----
  		fU2 = float(right-left) / src->GetWidth();
  		fV2 = float(bottom-top) / src->GetHeight();
  	}
  	gl_RenderState.Apply();
! 	glBegin(GL_TRIANGLE_STRIP);
! 	glColor4f(1, 1, 1, 1);
! 	glTexCoord2f(fU1, fV1); glVertex2f(left, top);
! 	glTexCoord2f(fU1, fV2); glVertex2f(left, bottom);
! 	glTexCoord2f(fU2, fV1); glVertex2f(right, top);
! 	glTexCoord2f(fU2, fV2); glVertex2f(right, bottom);
! 	glEnd();
  }
  
  //==========================================================================
***************
*** 574,580 ****
  		return;
  	}
  
! 	FMaterial *gltexture = FMaterial::ValidateTexture(texture, false);
  
  	if (gltexture == NULL)
  	{
--- 598,604 ----
  		return;
  	}
  
! 	FMaterial *gltexture = FMaterial::ValidateTexture(texture);
  
  	if (gltexture == NULL)
  	{
***************
*** 584,592 ****
  	FColormap cm;
  	cm = colormap;
  
! 	gl_SetColor(lightlevel, 0, cm, 1.f);
  
! 	gl_RenderState.SetMaterial(gltexture, CLAMP_NONE, 0, -1, false);
  
  	int i;
  	float rot = float(rotation * M_PI / float(1u << 31));
--- 608,618 ----
  	FColormap cm;
  	cm = colormap;
  
! 	lightlevel = gl_CalcLightLevel(lightlevel, 0, true);
! 	PalEntry pe = gl_CalcLightColor(lightlevel, cm.LightColor, cm.blendfactor, true);
! 	glColor3ub(pe.r, pe.g, pe.b);
  
! 	gltexture->Bind(cm.colormap);
  
  	int i;
  	float rot = float(rotation * M_PI / float(1u << 31));
***************
*** 606,613 ****
  	float oy = float(originy);
  
  	gl_RenderState.Apply();
! 
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
  	for (i = 0; i < npoints; ++i)
  	{
  		float u = points[i].X - 0.5f - ox;
--- 632,638 ----
  	float oy = float(originy);
  
  	gl_RenderState.Apply();
! 	glBegin(GL_TRIANGLE_FAN);
  	for (i = 0; i < npoints; ++i)
  	{
  		float u = points[i].X - 0.5f - ox;
***************
*** 618,626 ****
  			u = t * cosrot - v * sinrot;
  			v = v * cosrot + t * sinrot;
  		}
! 		ptr->Set(points[i].X, points[i].Y, 0, u*uscale, v*vscale);
! 		ptr++;
  	}
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_FAN);
  }
  
--- 643,651 ----
  			u = t * cosrot - v * sinrot;
  			v = v * cosrot + t * sinrot;
  		}
! 		glTexCoord2f(u * uscale, v * vscale);
! 		glVertex3f(points[i].X, points[i].Y /* + yoffs */, 0);
  	}
! 	glEnd();
  }
  
