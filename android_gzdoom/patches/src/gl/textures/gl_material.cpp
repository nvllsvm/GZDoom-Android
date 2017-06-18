*** doom/src/main/jni/gzdoom/src/gl/textures/gl_material.cpp	2017-06-18 23:15:10.669973853 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/textures/gl_material.cpp	2017-06-18 23:34:13.243914397 -0400
***************
*** 53,65 ****
  #include "gl/system/gl_interface.h"
  #include "gl/system/gl_framebuffer.h"
  #include "gl/renderer/gl_lightdata.h"
- #include "gl/renderer/gl_renderer.h"
  #include "gl/data/gl_data.h"
  #include "gl/textures/gl_texture.h"
  #include "gl/textures/gl_translate.h"
  #include "gl/textures/gl_bitmap.h"
  #include "gl/textures/gl_material.h"
- #include "gl/textures/gl_samplers.h"
  #include "gl/shaders/gl_shader.h"
  
  EXTERN_CVAR(Bool, gl_render_precise)
--- 53,63 ----
***************
*** 80,95 ****
  //===========================================================================
  FGLTexture::FGLTexture(FTexture * tx, bool expandpatches)
  {
! 	assert(tx->gl_info.SystemTexture[expandpatches] == NULL);
  	tex = tx;
  
! 	mHwTexture = NULL;
! 	HiresLump = -1;
  	hirestexture = NULL;
  	bHasColorkey = false;
  	bIsTransparent = -1;
  	bExpand = expandpatches;
! 	tex->gl_info.SystemTexture[expandpatches] = this;
  }
  
  //===========================================================================
--- 78,95 ----
  //===========================================================================
  FGLTexture::FGLTexture(FTexture * tx, bool expandpatches)
  {
! 	assert(tx->gl_info.SystemTexture == NULL);
  	tex = tx;
  
! 	glpatch=NULL;
! 	for(int i=0;i<5;i++) gltexture[i]=NULL;
! 	HiresLump=-1;
  	hirestexture = NULL;
+ 	currentwarp = 0;
  	bHasColorkey = false;
  	bIsTransparent = -1;
  	bExpand = expandpatches;
! 	tex->gl_info.SystemTexture = this;
  }
  
  //===========================================================================
***************
*** 109,115 ****
  // Checks for the presence of a hires texture replacement and loads it
  //
  //==========================================================================
! unsigned char *FGLTexture::LoadHiresTexture(FTexture *tex, int *width, int *height)
  {
  	if (HiresLump==-1) 
  	{
--- 109,115 ----
  // Checks for the presence of a hires texture replacement and loads it
  //
  //==========================================================================
! unsigned char *FGLTexture::LoadHiresTexture(FTexture *tex, int *width, int *height, int cm)
  {
  	if (HiresLump==-1) 
  	{
***************
*** 131,136 ****
--- 131,138 ----
  		memset(buffer, 0, w * (h+1) * 4);
  
  		FGLBitmap bmp(buffer, w*4, w, h);
+ 		bmp.SetTranslationInfo(cm);
+ 
  		
  		int trans = hirestexture->CopyTrueColorPixels(&bmp, 0, 0);
  		hirestexture->CheckTrans(buffer, w*h, trans);
***************
*** 161,177 ****
  
  void FGLTexture::Clean(bool all)
  {
! 	if (mHwTexture) 
  	{
! 		if (!all) mHwTexture->Clean(false);
  		else
  		{
! 			delete mHwTexture;
! 			mHwTexture = NULL;
  		}
  	}
  }
  
  
  //===========================================================================
  // 
--- 163,269 ----
  
  void FGLTexture::Clean(bool all)
  {
! 	for(int i=0;i<5;i++)
! 	{
! 		if (gltexture[i]) 
! 		{
! 			if (!all) gltexture[i]->Clean(false);
! 			else
! 			{
! 				delete gltexture[i];
! 				gltexture[i]=NULL;
! 			}
! 		}
! 	}
! 	if (glpatch) 
  	{
! 		if (!all) glpatch->Clean(false);
  		else
  		{
! 			delete glpatch;
! 			glpatch=NULL;
  		}
  	}
  }
  
+ //===========================================================================
+ //
+ // FGLTex::WarpBuffer
+ //
+ //===========================================================================
+ 
+ BYTE *FGLTexture::WarpBuffer(BYTE *buffer, int Width, int Height, int warp)
+ {
+ 	if (Width > 256 || Height > 256) return buffer;
+ 
+ 	DWORD *in = (DWORD*)buffer;
+ 	DWORD *out = (DWORD*)new BYTE[4*Width*Height];
+ 	float Speed = static_cast<FWarpTexture*>(tex)->GetSpeed();
+ 
+ 	static_cast<FWarpTexture*>(tex)->GenTime = r_FrameTime;
+ 
+ 	static DWORD linebuffer[256];	// anything larger will bring down performance so it is excluded above.
+ 	DWORD timebase = DWORD(r_FrameTime*Speed*23/28);
+ 	int xsize = Width;
+ 	int ysize = Height;
+ 	int xmask = xsize - 1;
+ 	int ymask = ysize - 1;
+ 	int ds_xbits;
+ 	int i,x;
+ 
+ 	if (warp == 1)
+ 	{
+ 		for(ds_xbits=-1,i=Width; i; i>>=1, ds_xbits++);
+ 
+ 		for (x = xsize-1; x >= 0; x--)
+ 		{
+ 			int yt, yf = (finesine[(timebase+(x+17)*128)&FINEMASK]>>13) & ymask;
+ 			const DWORD *source = in + x;
+ 			DWORD *dest = out + x;
+ 			for (yt = ysize; yt; yt--, yf = (yf+1)&ymask, dest += xsize)
+ 			{
+ 				*dest = *(source+(yf<<ds_xbits));
+ 			}
+ 		}
+ 		timebase = DWORD(r_FrameTime*Speed*32/28);
+ 		int y;
+ 		for (y = ysize-1; y >= 0; y--)
+ 		{
+ 			int xt, xf = (finesine[(timebase+y*128)&FINEMASK]>>13) & xmask;
+ 			DWORD *source = out + (y<<ds_xbits);
+ 			DWORD *dest = linebuffer;
+ 			for (xt = xsize; xt; xt--, xf = (xf+1)&xmask)
+ 			{
+ 				*dest++ = *(source+xf);
+ 			}
+ 			memcpy (out+y*xsize, linebuffer, xsize*sizeof(DWORD));
+ 		}
+ 	}
+ 	else
+ 	{
+ 		int ybits;
+ 		for(ybits=-1,i=ysize; i; i>>=1, ybits++);
+ 
+ 		DWORD timebase = (r_FrameTime * Speed * 40 / 28);
+ 		for (x = xsize-1; x >= 0; x--)
+ 		{
+ 			for (int y = ysize-1; y >= 0; y--)
+ 			{
+ 				int xt = (x + 128
+ 					+ ((finesine[(y*128 + timebase*5 + 900) & 8191]*2)>>FRACBITS)
+ 					+ ((finesine[(x*256 + timebase*4 + 300) & 8191]*2)>>FRACBITS)) & xmask;
+ 				int yt = (y + 128
+ 					+ ((finesine[(y*128 + timebase*3 + 700) & 8191]*2)>>FRACBITS)
+ 					+ ((finesine[(x*256 + timebase*4 + 1200) & 8191]*2)>>FRACBITS)) & ymask;
+ 				const DWORD *source = in + (xt << ybits) + yt;
+ 				DWORD *dest = out + (x << ybits) + y;
+ 				*dest = *source;
+ 			}
+ 		}
+ 	}
+ 	delete [] buffer;
+ 	return (BYTE*)out;
+ }
  
  //===========================================================================
  // 
***************
*** 179,185 ****
  //
  //===========================================================================
  
! unsigned char * FGLTexture::CreateTexBuffer(int translation, int & w, int & h, FTexture *hirescheck)
  {
  	unsigned char * buffer;
  	int W, H;
--- 271,277 ----
  //
  //===========================================================================
  
! unsigned char * FGLTexture::CreateTexBuffer(int cm, int translation, int & w, int & h, bool expand, FTexture *hirescheck, int warp)
  {
  	unsigned char * buffer;
  	int W, H;
***************
*** 189,210 ****
  	// by hires textures
  	if (gl_texture_usehires && hirescheck != NULL)
  	{
! 		buffer = LoadHiresTexture (hirescheck, &w, &h);
  		if (buffer)
  		{
  			return buffer;
  		}
  	}
  
! 	W = w = tex->GetWidth() + bExpand*2;
! 	H = h = tex->GetHeight() + bExpand*2;
  
  
  	buffer=new unsigned char[W*(H+1)*4];
  	memset(buffer, 0, W * (H+1) * 4);
  
  	FGLBitmap bmp(buffer, W*4, W, H);
! 	bmp.SetTranslationInfo(translation);
  
  	if (tex->bComplex)
  	{
--- 281,302 ----
  	// by hires textures
  	if (gl_texture_usehires && hirescheck != NULL)
  	{
! 		buffer = LoadHiresTexture (hirescheck, &w, &h, cm);
  		if (buffer)
  		{
  			return buffer;
  		}
  	}
  
! 	W = w = tex->GetWidth() + expand*2;
! 	H = h = tex->GetHeight() + expand*2;
  
  
  	buffer=new unsigned char[W*(H+1)*4];
  	memset(buffer, 0, W * (H+1) * 4);
  
  	FGLBitmap bmp(buffer, W*4, W, H);
! 	bmp.SetTranslationInfo(cm, translation);
  
  	if (tex->bComplex)
  	{
***************
*** 215,221 ****
  		if (imgCreate.Create(W, H))
  		{
  			memset(imgCreate.GetPixels(), 0, W * H * 4);
! 			int trans = tex->CopyTrueColorPixels(&imgCreate, bExpand, bExpand);
  			bmp.CopyPixelDataRGB(0, 0, imgCreate.GetPixels(), W, H, 4, W * 4, 0, CF_BGRA);
  			tex->CheckTrans(buffer, W*H, trans);
  			bIsTransparent = tex->gl_info.mIsTransparent;
--- 307,313 ----
  		if (imgCreate.Create(W, H))
  		{
  			memset(imgCreate.GetPixels(), 0, W * H * 4);
! 			int trans = tex->CopyTrueColorPixels(&imgCreate, expand, expand);
  			bmp.CopyPixelDataRGB(0, 0, imgCreate.GetPixels(), W, H, 4, W * 4, 0, CF_BGRA);
  			tex->CheckTrans(buffer, W*H, trans);
  			bIsTransparent = tex->gl_info.mIsTransparent;
***************
*** 223,229 ****
  	}
  	else if (translation<=0)
  	{
! 		int trans = tex->CopyTrueColorPixels(&bmp, bExpand, bExpand);
  		tex->CheckTrans(buffer, W*H, trans);
  		bIsTransparent = tex->gl_info.mIsTransparent;
  	}
--- 315,321 ----
  	}
  	else if (translation<=0)
  	{
! 		int trans = tex->CopyTrueColorPixels(&bmp, expand, expand);
  		tex->CheckTrans(buffer, W*H, trans);
  		bIsTransparent = tex->gl_info.mIsTransparent;
  	}
***************
*** 232,244 ****
  		// When using translations everything must be mapped to the base palette.
  		// Since FTexture's method is doing exactly that by calling GetPixels let's use that here
  		// to do all the dirty work for us. ;)
! 		tex->FTexture::CopyTrueColorPixels(&bmp, bExpand, bExpand);
  		bIsTransparent = 0;
  	}
  
  	// [BB] The hqnx upsampling (not the scaleN one) destroys partial transparency, don't upsamle textures using it.
! 	// [BB] Potentially upsample the buffer.
! 	return gl_CreateUpsampledTextureBuffer ( tex, buffer, W, H, w, h, !!bIsTransparent);
  }
  
  
--- 324,348 ----
  		// When using translations everything must be mapped to the base palette.
  		// Since FTexture's method is doing exactly that by calling GetPixels let's use that here
  		// to do all the dirty work for us. ;)
! 		tex->FTexture::CopyTrueColorPixels(&bmp, expand, expand);
  		bIsTransparent = 0;
  	}
  
+ 	if (warp != 0)
+ 	{
+ 		buffer = WarpBuffer(buffer, W, H, warp);
+ 	}
  	// [BB] The hqnx upsampling (not the scaleN one) destroys partial transparency, don't upsamle textures using it.
! 	// Also don't upsample warped textures.
! 	else //if (bIsTransparent != 1)
! 	{
! 		// [BB] Potentially upsample the buffer.
! 		buffer = gl_CreateUpsampledTextureBuffer ( tex, buffer, W, H, w, h, bIsTransparent || cm == CM_SHADE );
! 	}
! 	currentwarp = warp;
! 	currentwarptime = gl_frameMS;
! 
! 	return buffer;
  }
  
  
***************
*** 248,291 ****
  //
  //===========================================================================
  
! FHardwareTexture *FGLTexture::CreateHwTexture()
  {
  	if (tex->UseType==FTexture::TEX_Null) return NULL;		// Cannot register a NULL texture
! 	if (mHwTexture == NULL)
  	{
! 		mHwTexture = new FHardwareTexture(tex->GetWidth() + bExpand*2, tex->GetHeight() + bExpand*2, tex->gl_info.bNoCompress);
  	}
! 	return mHwTexture; 
  }
  
  //===========================================================================
  // 
  //	Binds a texture to the renderer
  //
  //===========================================================================
  
! const FHardwareTexture *FGLTexture::Bind(int texunit, int clampmode, int translation, FTexture *hirescheck)
  {
  	int usebright = false;
  
  	if (translation <= 0) translation = -translation;
  	else translation = GLTranslationPalette::GetInternalTranslation(translation);
  
! 	bool needmipmap = (clampmode <= CLAMP_XY);
  
! 	FHardwareTexture *hwtex = CreateHwTexture();
  
  	if (hwtex)
  	{
! 		// Texture has become invalid
! 		if ((!tex->bHasCanvas && !tex->bWarped) && tex->CheckModified())
  		{
  			Clean(true);
! 			hwtex = CreateHwTexture();
  		}
  
  		// Bind it to the system.
! 		if (!hwtex->Bind(texunit, translation, needmipmap))
  		{
  			
  			int w=0, h=0;
--- 352,434 ----
  //
  //===========================================================================
  
! FHardwareTexture *FGLTexture::CreateTexture(int clampmode)
  {
  	if (tex->UseType==FTexture::TEX_Null) return NULL;		// Cannot register a NULL texture
! 	if (!gltexture[clampmode]) 
  	{
! 		gltexture[clampmode] = new FHardwareTexture(tex->GetWidth(), tex->GetHeight(), true, true, false, tex->gl_info.bNoCompress);
  	}
! 	return gltexture[clampmode]; 
  }
  
  //===========================================================================
  // 
+ //	Create Hardware texture for patch use
+ //
+ //===========================================================================
+ 
+ bool FGLTexture::CreatePatch()
+ {
+ 	if (tex->UseType==FTexture::TEX_Null) return false;		// Cannot register a NULL texture
+ 	if (!glpatch) 
+ 	{
+ 		glpatch=new FHardwareTexture(tex->GetWidth() + bExpand, tex->GetHeight() + bExpand, false, false, tex->gl_info.bNoFilter, tex->gl_info.bNoCompress);
+ 	}
+ 	if (glpatch) return true; 	
+ 	return false;
+ }
+ 
+ 
+ //===========================================================================
+ // 
  //	Binds a texture to the renderer
  //
  //===========================================================================
  
! const FHardwareTexture *FGLTexture::Bind(int texunit, int cm, int clampmode, int translation, FTexture *hirescheck, int warp)
  {
  	int usebright = false;
  
  	if (translation <= 0) translation = -translation;
  	else translation = GLTranslationPalette::GetInternalTranslation(translation);
  
! 	FHardwareTexture *hwtex;
! 	
! 	if (gltexture[4] != NULL && clampmode < 4 && gltexture[clampmode] == NULL)
! 	{
! 		hwtex = gltexture[clampmode] = gltexture[4];
! 		gltexture[4] = NULL;
  
! 		if (hwtex->Bind(texunit, cm, translation))
! 		{
! 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (clampmode & GLT_CLAMPX)? GL_CLAMP_TO_EDGE : GL_REPEAT);
! 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (clampmode & GLT_CLAMPY)? GL_CLAMP_TO_EDGE : GL_REPEAT);
! 		}
! 	}
! 	else
! 	{
! 		hwtex = CreateTexture(clampmode);
! 	}
  
  	if (hwtex)
  	{
! 		if ((warp != 0 || currentwarp != warp) && currentwarptime != gl_frameMS)
  		{
+ 			// must recreate the texture
  			Clean(true);
! 			hwtex = CreateTexture(clampmode);
! 		}
! 
! 		// Texture has become invalid - this is only for special textures, not the regular warping, which is handled above
! 		else if ((warp == 0 && !tex->bHasCanvas && !tex->bWarped) && tex->CheckModified())
! 		{
! 			Clean(true);
! 			hwtex = CreateTexture(clampmode);
  		}
  
  		// Bind it to the system.
! 		if (!hwtex->Bind(texunit, cm, translation))
  		{
  			
  			int w=0, h=0;
***************
*** 295,320 ****
  			
  			if (!tex->bHasCanvas)
  			{
! 				buffer = CreateTexBuffer(translation, w, h, hirescheck);
  				tex->ProcessData(buffer, w, h, false);
  			}
! 			if (!hwtex->CreateTexture(buffer, w, h, texunit, needmipmap, translation)) 
  			{
  				// could not create texture
  				delete[] buffer;
  				return NULL;
  			}
  			delete[] buffer;
  		}
  
  		if (tex->bHasCanvas) static_cast<FCanvasTexture*>(tex)->NeedUpdate();
- 		GLRenderer->mSamplerManager->Bind(texunit, clampmode);
  		return hwtex; 
  	}
  	return NULL;
  }
  
  //===========================================================================
  //
  //
  //
--- 438,518 ----
  			
  			if (!tex->bHasCanvas)
  			{
! 				buffer = CreateTexBuffer(cm, translation, w, h, false, hirescheck, warp);
  				tex->ProcessData(buffer, w, h, false);
  			}
! 			if (!hwtex->CreateTexture(buffer, w, h, true, texunit, cm, translation)) 
  			{
  				// could not create texture
  				delete[] buffer;
  				return NULL;
  			}
+ 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (clampmode & GLT_CLAMPX)? GL_CLAMP_TO_EDGE : GL_REPEAT);
+ 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (clampmode & GLT_CLAMPY)? GL_CLAMP_TO_EDGE : GL_REPEAT);
  			delete[] buffer;
  		}
  
  		if (tex->bHasCanvas) static_cast<FCanvasTexture*>(tex)->NeedUpdate();
  		return hwtex; 
  	}
  	return NULL;
  }
  
  //===========================================================================
+ // 
+ //	Binds a sprite to the renderer
+ //
+ //===========================================================================
+ const FHardwareTexture * FGLTexture::BindPatch(int texunit, int cm, int translation, int warp)
+ {
+ 	bool usebright = false;
+ 	int transparm = translation;
+ 
+ 	if (translation <= 0) translation = -translation;
+ 	else translation = GLTranslationPalette::GetInternalTranslation(translation);
+ 
+ 	if (CreatePatch())
+ 	{
+ 		if (warp != 0 || currentwarp != warp)
+ 		{
+ 			// must recreate the texture
+ 			Clean(true);
+ 			CreatePatch();
+ 		}
+ 
+ 		// Texture has become invalid - this is only for special textures, not the regular warping, which is handled above
+ 		else if ((warp == 0 && !tex->bHasCanvas && !tex->bWarped) && tex->CheckModified())
+ 		{
+ 			Clean(true);
+ 			CreatePatch();
+ 		}
+ 
+ 
+ 		// Bind it to the system. 
+ 		if (!glpatch->Bind(texunit, cm, translation))
+ 		{
+ 			int w, h;
+ 
+ 			// Create this texture
+ 			unsigned char * buffer = CreateTexBuffer(cm, translation, w, h, bExpand, NULL, warp);
+ 			tex->ProcessData(buffer, w, h, true);
+ 			if (!glpatch->CreateTexture(buffer, w, h, false, texunit, cm, translation)) 
+ 			{
+ 				// could not create texture
+ 				delete[] buffer;
+ 				return NULL;
+ 			}
+ 			delete[] buffer;
+ 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
+ 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
+ 		}
+ 		return glpatch; 	
+ 	}
+ 	return NULL;
+ }
+ 
+ 
+ //===========================================================================
  //
  //
  //
***************
*** 381,387 ****
  {
  	if (tex	&& tex->UseType!=FTexture::TEX_Null)
  	{
! 		FGLTexture *gltex = tex->gl_info.SystemTexture[expand];
  		if (gltex == NULL) 
  		{
  			gltex = new FGLTexture(tex, expand);
--- 579,585 ----
  {
  	if (tex	&& tex->UseType!=FTexture::TEX_Null)
  	{
! 		FGLTexture *gltex = tex->gl_info.SystemTexture;
  		if (gltex == NULL) 
  		{
  			gltex = new FGLTexture(tex, expand);
***************
*** 399,408 ****
  TArray<FMaterial *> FMaterial::mMaterials;
  int FMaterial::mMaxBound;
  
! FMaterial::FMaterial(FTexture * tx, bool expanded)
  {
  	mShaderIndex = 0;
- 	tex = tx;
  
  	// TODO: apply custom shader object here
  	/* if (tx->CustomShaderDefinition)
--- 597,612 ----
  TArray<FMaterial *> FMaterial::mMaterials;
  int FMaterial::mMaxBound;
  
! FMaterial::FMaterial(FTexture * tx, bool forceexpand)
  {
+ 	assert(tx->gl_info.Material == NULL);
+ 
+ 	bool expanded = tx->UseType == FTexture::TEX_Sprite || 
+ 					tx->UseType == FTexture::TEX_SkinSprite || 
+ 					tx->UseType == FTexture::TEX_Decal ||
+ 					forceexpand;
+ 
  	mShaderIndex = 0;
  
  	// TODO: apply custom shader object here
  	/* if (tx->CustomShaderDefinition)
***************
*** 420,426 ****
  	{
  		expanded = false;
  	}
! 	else
  	{
  		if (tx->gl_info.shaderindex >= FIRST_USER_SHADER)
  		{
--- 624,630 ----
  	{
  		expanded = false;
  	}
! 	else if (gl.shadermodel > 2) 
  	{
  		if (tx->gl_info.shaderindex >= FIRST_USER_SHADER)
  		{
***************
*** 439,499 ****
  			}
  		}
  	}
- 	assert(tx->gl_info.Material[expanded] == NULL);
- 	mBaseLayer = ValidateSysTexture(tx, true);
- 
  
- 	mWidth = tx->GetWidth();
- 	mHeight = tx->GetHeight();
- 	mLeftOffset = tx->LeftOffset;
- 	mTopOffset = tx->TopOffset;
- 	mRenderWidth = tx->GetScaledWidth();
- 	mRenderHeight = tx->GetScaledHeight();
- 	mSpriteU[0] = mSpriteV[0] = 0.f;
- 	mSpriteU[1] = mSpriteV[1] = 1.f;
- 
- 	FTexture *basetex = tx->GetRedirect(false);
- 	mBaseLayer = ValidateSysTexture(basetex, expanded);
- 
- 	// mSpriteRect is for positioning the sprite in the scene.
- 	mSpriteRect.left = -mLeftOffset / FIXED2FLOAT(tx->xScale);
- 	mSpriteRect.top = -mTopOffset / FIXED2FLOAT(tx->yScale);
- 	mSpriteRect.width = mWidth / FIXED2FLOAT(tx->xScale);
- 	mSpriteRect.height = mHeight / FIXED2FLOAT(tx->yScale);
  
! 	if (expanded)
  	{
! 		// a little adjustment to make sprites look better with texture filtering:
! 		// create a 1 pixel wide empty frame around them.
! 		mWidth+=2;
! 		mHeight+=2;
! 		mLeftOffset+=1;
! 		mTopOffset+=1;
! 		mRenderWidth = mRenderWidth * mWidth / (mWidth-2);
! 		mRenderHeight = mRenderHeight * mHeight / (mHeight-2);
! 
! 		int trim[4];
! 
! 		if (TrimBorders(trim))
! 		{
! 			mSpriteRect.left = -(mLeftOffset - trim[0]) / FIXED2FLOAT(tx->xScale);
! 			mSpriteRect.top = -(mTopOffset - trim[1]) / FIXED2FLOAT(tx->yScale);
! 			mSpriteRect.width = (trim[2] + 2) / FIXED2FLOAT(tx->xScale);
! 			mSpriteRect.height = (trim[3] + 2) / FIXED2FLOAT(tx->yScale);
! 
! 			mSpriteU[0] = trim[0] / (float)mWidth;
! 			mSpriteV[0] = trim[1] / (float)mHeight;
! 			mSpriteU[1] *= (trim[0]+trim[2]+2) / (float)mWidth; 
! 			mSpriteV[1] *= (trim[1]+trim[3]+2) / (float)mHeight; 
! 		}
! 	}
  
  	mTextureLayers.ShrinkToFit();
  	mMaxBound = -1;
  	mMaterials.Push(this);
! 	tx->gl_info.Material[expanded] = this;
  	if (tx->bHasCanvas) tx->gl_info.mIsTransparent = 0;
! 	mExpanded = expanded;
  }
  
  //===========================================================================
--- 643,728 ----
  			}
  		}
  	}
  
  
! 	for (int i=GLUSE_PATCH; i<=GLUSE_TEXTURE; i++)
  	{
! 		Width[i] = tx->GetWidth();
! 		Height[i] = tx->GetHeight();
! 		LeftOffset[i] = tx->LeftOffset;
! 		TopOffset[i] = tx->TopOffset;
! 		RenderWidth[i] = tx->GetScaledWidth();
! 		RenderHeight[i] = tx->GetScaledHeight();
! 	}
! 	Width[GLUSE_SPRITE] = Width[GLUSE_PATCH];
! 	Height[GLUSE_SPRITE] = Height[GLUSE_PATCH];
! 	LeftOffset[GLUSE_SPRITE] = LeftOffset[GLUSE_PATCH];
! 	TopOffset[GLUSE_SPRITE] = TopOffset[GLUSE_PATCH];
! 	SpriteU[0] = SpriteV[0] = 0;
! 	spriteright = SpriteU[1] = Width[GLUSE_PATCH] / (float)FHardwareTexture::GetTexDimension(Width[GLUSE_PATCH]);
! 	spritebottom = SpriteV[1] = Height[GLUSE_PATCH] / (float)FHardwareTexture::GetTexDimension(Height[GLUSE_PATCH]);
  
  	mTextureLayers.ShrinkToFit();
  	mMaxBound = -1;
  	mMaterials.Push(this);
! 	tx->gl_info.Material = this;
  	if (tx->bHasCanvas) tx->gl_info.mIsTransparent = 0;
! 	tex = tx;
! 
! 	tx->gl_info.mExpanded = expanded;
! 	FTexture *basetex = tx->GetRedirect(gl.shadermodel < 4);
! 	if (!expanded && !basetex->gl_info.mExpanded && basetex->UseType != FTexture::TEX_Sprite)
! 	{
! 		// check if the texture is just a simple redirect to a patch
! 		// If so we should use the patch for texture creation to
! 		// avoid eventual redundancies. 
! 		// This may only be done if both textures use the same expansion mode
! 		// Redirects to sprites are not permitted because sprites get expanded, however, this won't have been set
! 		// if the sprite hadn't been used yet.
! 		mBaseLayer = ValidateSysTexture(basetex, false);
! 	}
! 	else if (!expanded)
! 	{
! 		// if we got a non-expanded texture that redirects to an expanded one
! 		mBaseLayer = ValidateSysTexture(tx, false);
! 	}
! 	else
! 	{
! 		// a little adjustment to make sprites look better with texture filtering:
! 		// create a 1 pixel wide empty frame around them.
! 		RenderWidth[GLUSE_PATCH]+=2;
! 		RenderHeight[GLUSE_PATCH]+=2;
! 		Width[GLUSE_PATCH]+=2;
! 		Height[GLUSE_PATCH]+=2;
! 		LeftOffset[GLUSE_PATCH]+=1;
! 		TopOffset[GLUSE_PATCH]+=1;
! 		Width[GLUSE_SPRITE] += 2;
! 		Height[GLUSE_SPRITE] += 2;
! 		LeftOffset[GLUSE_SPRITE] += 1;
! 		TopOffset[GLUSE_SPRITE] += 1;
! 		spriteright = SpriteU[1] = Width[GLUSE_PATCH] / (float)FHardwareTexture::GetTexDimension(Width[GLUSE_PATCH]);
! 		spritebottom = SpriteV[1] = Height[GLUSE_PATCH] / (float)FHardwareTexture::GetTexDimension(Height[GLUSE_PATCH]);
! 
! 		mBaseLayer = ValidateSysTexture(tx, true);
! 
! 		if (gl.flags & RFL_NPOT_TEXTURE)	// trimming only works if non-power-of-2 textures are supported
! 		{
! 			int trim[4];
! 
! 			if (TrimBorders(trim))
! 			{
! 				Width[GLUSE_SPRITE] = trim[2] + 2;
! 				Height[GLUSE_SPRITE] = trim[3] + 2;
! 				LeftOffset[GLUSE_SPRITE] -= trim[0];
! 				TopOffset[GLUSE_SPRITE] -= trim[1];
! 
! 				SpriteU[0] = SpriteU[1] * (trim[0] / (float)Width[GLUSE_PATCH]);
! 				SpriteV[0] = SpriteV[1] * (trim[1] / (float)Height[GLUSE_PATCH]);
! 				SpriteU[1] *= (trim[0]+trim[2]+2) / (float)Width[GLUSE_PATCH]; 
! 				SpriteV[1] *= (trim[1]+trim[3]+2) / (float)Height[GLUSE_PATCH]; 
! 			}
! 		}
! 	}
  }
  
  //===========================================================================
***************
*** 529,541 ****
  	int w;
  	int h;
  
! 	unsigned char *buffer = CreateTexBuffer(0, w, h);
  
  	if (buffer == NULL) 
  	{
  		return false;
  	}
! 	if (w != mWidth || h != mHeight)
  	{
  		// external Hires replacements cannot be trimmed.
  		delete [] buffer;
--- 758,770 ----
  	int w;
  	int h;
  
! 	unsigned char *buffer = CreateTexBuffer(CM_DEFAULT, 0, w, h);
  
  	if (buffer == NULL) 
  	{
  		return false;
  	}
! 	if (w != Width[GLUSE_TEXTURE] || h != Height[GLUSE_TEXTURE])
  	{
  		// external Hires replacements cannot be trimmed.
  		delete [] buffer;
***************
*** 608,635 ****
  //
  //===========================================================================
  
! static FMaterial *last;
! static int lastclamp;
! static int lasttrans;
! 
! 
! void FMaterial::Bind(int clampmode, int translation)
! {
! 	// avoid rebinding the same texture multiple times.
! 	if (this == last && lastclamp == clampmode && translation == lasttrans) return;
! 	last = this;
! 	lastclamp = clampmode;
! 	lasttrans = translation;
! 
  	int usebright = false;
  	int maxbound = 0;
! 	bool allowhires = tex->xScale == FRACUNIT && tex->yScale == FRACUNIT && clampmode <= CLAMP_XY && !mExpanded;
  
! 	if (tex->bHasCanvas) clampmode = CLAMP_CAMTEX;
! 	else if (tex->bWarped && clampmode <= CLAMP_XY) clampmode = CLAMP_NONE;
  
! 	const FHardwareTexture *gltexture = mBaseLayer->Bind(0, clampmode, translation, allowhires? tex:NULL);
! 	if (gltexture != NULL)
  	{
  		for(unsigned i=0;i<mTextureLayers.Size();i++)
  		{
--- 837,857 ----
  //
  //===========================================================================
  
! void FMaterial::Bind(int cm, int clampmode, int translation, int overrideshader)
! {
  	int usebright = false;
+ 	int shaderindex = overrideshader > 0? overrideshader : mShaderIndex;
  	int maxbound = 0;
! 	bool allowhires = tex->xScale == FRACUNIT && tex->yScale == FRACUNIT;
! 
! 	int softwarewarp = gl_RenderState.SetupShader(tex->bHasCanvas, shaderindex, cm, tex->gl_info.shaderspeed);
  
! 	if (tex->bHasCanvas || tex->bWarped) clampmode = 0;
! 	else if (clampmode != -1) clampmode &= 3;
! 	else clampmode = 4;
  
! 	const FHardwareTexture *gltexture = mBaseLayer->Bind(0, cm, clampmode, translation, allowhires? tex:NULL, softwarewarp);
! 	if (gltexture != NULL && shaderindex > 0 && overrideshader == 0)
  	{
  		for(unsigned i=0;i<mTextureLayers.Size();i++)
  		{
***************
*** 638,650 ****
  			{
  				FTextureID id = mTextureLayers[i].texture->id;
  				layer = TexMan(id);
! 				ValidateSysTexture(layer, mExpanded);
  			}
  			else
  			{
  				layer = mTextureLayers[i].texture;
  			}
! 			layer->gl_info.SystemTexture[mExpanded]->Bind(i+1, clampmode, 0, NULL);
  			maxbound = i+1;
  		}
  	}
--- 860,872 ----
  			{
  				FTextureID id = mTextureLayers[i].texture->id;
  				layer = TexMan(id);
! 				ValidateSysTexture(layer, false);
  			}
  			else
  			{
  				layer = mTextureLayers[i].texture;
  			}
! 			layer->gl_info.SystemTexture->Bind(i+1, CM_DEFAULT, clampmode, 0, NULL, false);
  			maxbound = i+1;
  		}
  	}
***************
*** 658,675 ****
  
  
  //===========================================================================
  //
  //
  //
  //===========================================================================
  void FMaterial::Precache()
  {
! 	Bind(0, 0);
  }
  
  //===========================================================================
  //
! // Retrieve texture coordinate info for per-wall scaling
  //
  //===========================================================================
  
--- 880,946 ----
  
  
  //===========================================================================
+ // 
+ //	Binds a texture to the renderer
+ //
+ //===========================================================================
+ 
+ void FMaterial::BindPatch(int cm, int translation, int overrideshader)
+ {
+ 	int usebright = false;
+ 	int shaderindex = overrideshader > 0? overrideshader : mShaderIndex;
+ 	int maxbound = 0;
+ 
+ 	int softwarewarp = gl_RenderState.SetupShader(tex->bHasCanvas, shaderindex, cm, tex->gl_info.shaderspeed);
+ 
+ 	const FHardwareTexture *glpatch = mBaseLayer->BindPatch(0, cm, translation, softwarewarp);
+ 	// The only multitexture effect usable on sprites is the brightmap.
+ 	if (glpatch != NULL && shaderindex == 3)
+ 	{
+ 		mTextureLayers[0].texture->gl_info.SystemTexture->BindPatch(1, CM_DEFAULT, 0, 0);
+ 		maxbound = 1;
+ 	}
+ 	// unbind everything from the last texture that's still active
+ 	for(int i=maxbound+1; i<=mMaxBound;i++)
+ 	{
+ 		FHardwareTexture::Unbind(i);
+ 		mMaxBound = maxbound;
+ 	}
+ }
+ 
+ 
+ //===========================================================================
  //
  //
  //
  //===========================================================================
  void FMaterial::Precache()
  {
! 	if (tex->UseType==FTexture::TEX_Sprite) 
! 	{
! 		BindPatch(CM_DEFAULT, 0);
! 	}
! 	else 
! 	{
! 		int cached = 0;
! 		for(int i=0;i<4;i++)
! 		{
! 			if (mBaseLayer->gltexture[i] != 0)
! 			{
! 				Bind (CM_DEFAULT, i, 0);
! 				cached++;
! 			}
! 			if (cached == 0) Bind(CM_DEFAULT, -1, 0);
! 		}
! 	}
  }
  
  //===========================================================================
  //
! // This function is needed here to temporarily manipulate the texture
! // for per-wall scaling so that the coordinate functions return proper
! // results. Doing this here is much easier than having the calling code
! // make these calculations.
  //
  //===========================================================================
  
***************
*** 677,690 ****
  {
  	if (x == FRACUNIT)
  	{
! 		tci->mRenderWidth = mRenderWidth;
  		tci->mScaleX = tex->xScale;
  		tci->mTempScaleX = FRACUNIT;
  	}
  	else
  	{
  		fixed_t scale_x = FixedMul(x, tex->xScale);
! 		int foo = (mWidth << 17) / scale_x; 
  		tci->mRenderWidth = (foo >> 1) + (foo & 1); 
  		tci->mScaleX = scale_x;
  		tci->mTempScaleX = x;
--- 948,961 ----
  {
  	if (x == FRACUNIT)
  	{
! 		tci->mRenderWidth = RenderWidth[GLUSE_TEXTURE];
  		tci->mScaleX = tex->xScale;
  		tci->mTempScaleX = FRACUNIT;
  	}
  	else
  	{
  		fixed_t scale_x = FixedMul(x, tex->xScale);
! 		int foo = (Width[GLUSE_TEXTURE] << 17) / scale_x; 
  		tci->mRenderWidth = (foo >> 1) + (foo & 1); 
  		tci->mScaleX = scale_x;
  		tci->mTempScaleX = x;
***************
*** 692,705 ****
  
  	if (y == FRACUNIT)
  	{
! 		tci->mRenderHeight = mRenderHeight;
  		tci->mScaleY = tex->yScale;
  		tci->mTempScaleY = FRACUNIT;
  	}
  	else
  	{
  		fixed_t scale_y = FixedMul(y, tex->yScale);
! 		int foo = (mHeight << 17) / scale_y; 
  		tci->mRenderHeight = (foo >> 1) + (foo & 1); 
  		tci->mScaleY = scale_y;
  		tci->mTempScaleY = y;
--- 963,976 ----
  
  	if (y == FRACUNIT)
  	{
! 		tci->mRenderHeight = RenderHeight[GLUSE_TEXTURE];
  		tci->mScaleY = tex->yScale;
  		tci->mTempScaleY = FRACUNIT;
  	}
  	else
  	{
  		fixed_t scale_y = FixedMul(y, tex->yScale);
! 		int foo = (Height[GLUSE_TEXTURE] << 17) / scale_y; 
  		tci->mRenderHeight = (foo >> 1) + (foo & 1); 
  		tci->mScaleY = scale_y;
  		tci->mTempScaleY = y;
***************
*** 710,716 ****
  		tci->mRenderHeight = -tci->mRenderHeight;
  	}
  	tci->mWorldPanning = tex->bWorldPanning;
! 	tci->mWidth = mWidth;
  }
  
  //===========================================================================
--- 981,987 ----
  		tci->mRenderHeight = -tci->mRenderHeight;
  	}
  	tci->mWorldPanning = tex->bWorldPanning;
! 	tci->mWidth = Width[GLUSE_TEXTURE];
  }
  
  //===========================================================================
***************
*** 741,756 ****
  
  void FMaterial::BindToFrameBuffer()
  {
! 	if (mBaseLayer->mHwTexture == NULL)
  	{
  		// must create the hardware texture first
! 		mBaseLayer->Bind(0, 0, 0, NULL);
  		FHardwareTexture::Unbind(0);
- 		ClearLastTexture();
  	}
! 	mBaseLayer->mHwTexture->BindToFrameBuffer();
  }
  
  //==========================================================================
  //
  // Gets a texture from the texture manager and checks its validity for
--- 1012,1041 ----
  
  void FMaterial::BindToFrameBuffer()
  {
! 	if (mBaseLayer->gltexture == NULL)
  	{
  		// must create the hardware texture first
! 		mBaseLayer->Bind(0, CM_DEFAULT, 0, 0, NULL, 0);
  		FHardwareTexture::Unbind(0);
  	}
! 	mBaseLayer->gltexture[0]->BindToFrameBuffer();
  }
  
+ //===========================================================================
+ //
+ // GetRect
+ //
+ //===========================================================================
+ 
+ void FMaterial::GetRect(FloatRect * r, ETexUse i) const
+ {
+ 	r->left = -GetScaledLeftOffsetFloat(i);
+ 	r->top = -GetScaledTopOffsetFloat(i);
+ 	r->width = GetScaledWidthFloat(i);
+ 	r->height = GetScaledHeightFloat(i);
+ }
+ 
+ 
  //==========================================================================
  //
  // Gets a texture from the texture manager and checks its validity for
***************
*** 758,780 ****
  //
  //==========================================================================
  
! FMaterial * FMaterial::ValidateTexture(FTexture * tex, bool expand)
  {
  	if (tex	&& tex->UseType!=FTexture::TEX_Null)
  	{
! 		FMaterial *gltex = tex->gl_info.Material[expand];
  		if (gltex == NULL) 
  		{
! 			gltex = new FMaterial(tex, expand);
  		}
  		return gltex;
  	}
  	return NULL;
  }
  
! FMaterial * FMaterial::ValidateTexture(FTextureID no, bool expand, bool translate)
  {
! 	return ValidateTexture(translate? TexMan(no) : TexMan[no], expand);
  }
  
  
--- 1043,1066 ----
  //
  //==========================================================================
  
! FMaterial * FMaterial::ValidateTexture(FTexture * tex)
  {
  	if (tex	&& tex->UseType!=FTexture::TEX_Null)
  	{
! 		FMaterial *gltex = tex->gl_info.Material;
  		if (gltex == NULL) 
  		{
! 			//@sync-tex
! 			gltex = new FMaterial(tex, false);
  		}
  		return gltex;
  	}
  	return NULL;
  }
  
! FMaterial * FMaterial::ValidateTexture(FTextureID no, bool translate)
  {
! 	return ValidateTexture(translate? TexMan(no) : TexMan[no]);
  }
  
  
***************
*** 794,808 ****
  	// so this will catch everything.
  	for(int i=TexMan.NumTextures()-1;i>=0;i--)
  	{
! 		for (int j = 0; j < 2; j++)
! 		{
! 			FGLTexture *gltex = TexMan.ByIndex(i)->gl_info.SystemTexture[j];
! 			if (gltex != NULL) gltex->Clean(true);
! 		}
  	}
  }
  
! void FMaterial::ClearLastTexture()
  {
! 	last = NULL;
  }
--- 1080,1126 ----
  	// so this will catch everything.
  	for(int i=TexMan.NumTextures()-1;i>=0;i--)
  	{
! 		FGLTexture *gltex = TexMan.ByIndex(i)->gl_info.SystemTexture;
! 		if (gltex != NULL) gltex->Clean(true);
  	}
  }
  
! //==========================================================================
! //
! // Prints some texture info
! //
! //==========================================================================
! 
! int FGLTexture::Dump(int i)
  {
! 	int cnt = 0;
! 	int lump = tex->GetSourceLump();
! 	Printf(PRINT_LOG, "Texture '%s' (Index %d, Lump %d, Name '%s'):\n", tex->Name.GetChars(), i, lump, Wads.GetLumpFullName(lump));
! 	if (hirestexture) Printf(PRINT_LOG, "\tHirestexture\n");
! 	if (glpatch) Printf(PRINT_LOG, "\tPatch\n"),cnt++;
! 	if (gltexture[0]) Printf(PRINT_LOG, "\tTexture (x:no,  y:no )\n"),cnt++;
! 	if (gltexture[1]) Printf(PRINT_LOG, "\tTexture (x:yes, y:no )\n"),cnt++;
! 	if (gltexture[2]) Printf(PRINT_LOG, "\tTexture (x:no,  y:yes)\n"),cnt++;
! 	if (gltexture[3]) Printf(PRINT_LOG, "\tTexture (x:yes, y:yes)\n"),cnt++;
! 	if (gltexture[4]) Printf(PRINT_LOG, "\tTexture precache\n"),cnt++;
! 	return cnt;
! }
! 
! CCMD(textureinfo)
! {
! 	int cnth = 0, cntt = 0, pix = 0;
! 	for(int i=0; i<TexMan.NumTextures(); i++)
! 	{
! 		FTexture *tex = TexMan.ByIndex(i);
! 		FGLTexture *systex = tex->gl_info.SystemTexture;
! 		if (systex != NULL) 
! 		{
! 			int cnt = systex->Dump(i);
! 			cnth+=cnt;
! 			cntt++;
! 			pix += cnt * tex->GetWidth() * tex->GetHeight();
! 		}
! 	}
! 	Printf(PRINT_LOG, "%d system textures, %d hardware textures, %d pixels\n", cntt, cnth, pix);
  }
+ 
