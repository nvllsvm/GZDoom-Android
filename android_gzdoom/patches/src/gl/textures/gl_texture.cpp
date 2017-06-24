*** doom/src/main/jni/gzdoom/src/gl/textures/gl_texture.cpp	2017-06-20 19:11:53.539649973 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/textures/gl_texture.cpp	2017-06-23 21:24:51.665462769 -0400
***************
*** 52,58 ****
  #include "gl/renderer/gl_renderer.h"
  #include "gl/textures/gl_texture.h"
  #include "gl/textures/gl_material.h"
- #include "gl/textures/gl_samplers.h"
  
  //==========================================================================
  //
--- 52,57 ----
***************
*** 61,67 ****
  //==========================================================================
  CUSTOM_CVAR(Float,gl_texture_filter_anisotropic,8.0f,CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOINITCALL)
  {
! 	if (GLRenderer != NULL && GLRenderer->mSamplerManager != NULL) GLRenderer->mSamplerManager->SetTextureFilterMode();
  }
  
  CCMD(gl_flush)
--- 60,66 ----
  //==========================================================================
  CUSTOM_CVAR(Float,gl_texture_filter_anisotropic,8.0f,CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOINITCALL)
  {
! 	if (GLRenderer != NULL) GLRenderer->FlushTextures();
  }
  
  CCMD(gl_flush)
***************
*** 72,78 ****
  CUSTOM_CVAR(Int, gl_texture_filter, 4, CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOINITCALL)
  {
  	if (self < 0 || self > 5) self=4;
! 	if (GLRenderer != NULL && GLRenderer->mSamplerManager != NULL) GLRenderer->mSamplerManager->SetTextureFilterMode();
  }
  
  CUSTOM_CVAR(Int, gl_texture_format, 0, CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOINITCALL)
--- 71,77 ----
  CUSTOM_CVAR(Int, gl_texture_filter, 4, CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOINITCALL)
  {
  	if (self < 0 || self > 5) self=4;
! 	if (GLRenderer != NULL) GLRenderer->FlushTextures();
  }
  
  CUSTOM_CVAR(Int, gl_texture_format, 0, CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOINITCALL)
***************
*** 95,100 ****
--- 94,100 ----
  }
  
  CVAR(Bool, gl_precache, false, CVAR_ARCHIVE)
+ CVAR(Bool, gl_clamp_per_texture, false,  CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  
  CVAR(Bool, gl_trimsprites, true, CVAR_ARCHIVE);
  
***************
*** 210,216 ****
  		g = Scale(g, maxout_factor, maxv);
  		b = Scale(b, maxout_factor, maxv);
  	}
! 	return PalEntry(255,r,g,b);
  }
  
  
--- 210,216 ----
  		g = Scale(g, maxout_factor, maxv);
  		b = Scale(b, maxout_factor, maxv);
  	}
! 	return PalEntry(r,g,b);
  }
  
  
***************
*** 236,269 ****
  	bBrightmapDisablesFullbright = false;
  	bNoFilter = false;
  	bNoCompress = false;
  	areas = NULL;
  	areacount = 0;
  	mIsTransparent = -1;
  	shaderspeed = 1.f;
  	shaderindex = 0;
- 	precacheTime = 0;
  
! 	Material[1] = Material[0] = NULL;
! 	SystemTexture[1] = SystemTexture[0] = NULL;
  	Brightmap = NULL;
  }
  
  FTexture::MiscGLInfo::~MiscGLInfo()
  {
! 	for (int i = 0; i < 2; i++)
! 	{
! 		if (Material[i] != NULL) delete Material[i];
! 		Material[i] = NULL;
  
! 		if (SystemTexture[i] != NULL) delete SystemTexture[i];
! 		SystemTexture[i] = NULL;
! 	}
  
! 	// this is just a reference to another texture in the texture manager.
  	Brightmap = NULL;
  
  	if (areas != NULL) delete [] areas;
  	areas = NULL;
  }
  
  //===========================================================================
--- 236,272 ----
  	bBrightmapDisablesFullbright = false;
  	bNoFilter = false;
  	bNoCompress = false;
+ 	mExpanded = false;
  	areas = NULL;
  	areacount = 0;
  	mIsTransparent = -1;
  	shaderspeed = 1.f;
  	shaderindex = 0;
  
! 	Material = NULL;
! 	SystemTexture = NULL;
  	Brightmap = NULL;
+ 	DecalTexture = NULL;
  }
  
  FTexture::MiscGLInfo::~MiscGLInfo()
  {
! 	if (Material != NULL) delete Material;
! 	Material = NULL;
  
! 	if (SystemTexture != NULL) delete SystemTexture;
! 	SystemTexture = NULL;
  
! 	// this is managed by the texture manager so it may not be deleted here.
! 	//if (Brightmap != NULL) delete Brightmap;
  	Brightmap = NULL;
  
  	if (areas != NULL) delete [] areas;
  	areas = NULL;
+ 
+ 	if (DecalTexture != NULL) delete DecalTexture;
+ 	DecalTexture = NULL;
+ 
  }
  
  //===========================================================================
***************
*** 321,337 ****
  {
  	if (gl_precache)
  	{
! 		if (cache & 2)
! 		{
! 			FMaterial * gltex = FMaterial::ValidateTexture(this, false);
! 			if (gltex) gltex->Precache();
! 		}
! 		if (cache & 4)
! 		{
! 			FMaterial * gltex = FMaterial::ValidateTexture(this, true);
! 			if (gltex) gltex->Precache();
! 		}
! 		gl_info.precacheTime = TexMan.precacheTime;
  	}
  }
  
--- 324,331 ----
  {
  	if (gl_precache)
  	{
! 		FMaterial * gltex = FMaterial::ValidateTexture(this);
! 		if (gltex) gltex->Precache();
  	}
  }
  
***************
*** 343,354 ****
  
  void FTexture::UncacheGL()
  {
! 	if (gl_info.precacheTime != TexMan.precacheTime)
! 	{
! 		if (gl_info.Material[0]) gl_info.Material[0]->Clean(true);
! 		if (gl_info.Material[1]) gl_info.Material[1]->Clean(true);
! 		gl_info.precacheTime = 0;
! 	}
  }
  
  //==========================================================================
--- 337,343 ----
  
  void FTexture::UncacheGL()
  {
! 	if (gl_info.Material) gl_info.Material->Clean(true); 
  }
  
  //==========================================================================
***************
*** 569,575 ****
  		l1+=4;
  		for(x=1;x<w-1;x++, l1+=4)
  		{
! 			if (l1[MSB]==0 &&  !CHKPIX(-w) && !CHKPIX(-1) && !CHKPIX(1) && !CHKPIX(-w-1) && !CHKPIX(-w+1) && !CHKPIX(w-1) && !CHKPIX(w+1)) CHKPIX(w);
  		}
  		if (l1[MSB]==0 && !CHKPIX(-w) && !CHKPIX(-1)) CHKPIX(w);
  		l1+=4;
--- 558,564 ----
  		l1+=4;
  		for(x=1;x<w-1;x++, l1+=4)
  		{
! 			if (l1[MSB]==0 &&  !CHKPIX(-w) && !CHKPIX(-1) && !CHKPIX(1)) CHKPIX(w);
  		}
  		if (l1[MSB]==0 && !CHKPIX(-w) && !CHKPIX(-1)) CHKPIX(w);
  		l1+=4;
***************
*** 652,657 ****
--- 641,689 ----
  }
  
  
+ //===========================================================================
+ //
+ // A cloned texture. This is needed by the decal code which needs to assign
+ // a different texture type to some of its graphics.
+ //
+ //===========================================================================
+ 
+ FCloneTexture::FCloneTexture (FTexture *source, int usetype)
+ {
+ 	Name = "";
+ 	SourcePic = source;
+ 	CopySize(source);
+ 	bNoDecals = source->bNoDecals;
+ 	Rotations = source->Rotations;
+ 	UseType = usetype;
+ 	gl_info.bBrightmap = false;
+ 	id.SetInvalid();
+ 	SourceLump = -1;
+ }
+ 
+ FCloneTexture::~FCloneTexture ()
+ {
+ }
+ 
+ const BYTE *FCloneTexture::GetColumn (unsigned int column, const Span **spans_out)
+ {
+ 	return NULL;
+ }
+ 
+ const BYTE *FCloneTexture::GetPixels ()
+ {
+ 	return NULL;
+ }
+ 
+ void FCloneTexture::Unload ()
+ {
+ }
+ 
+ int FCloneTexture::CopyTrueColorPixels(FBitmap *bmp, int x, int y, int rotate, FCopyInfo *inf)
+ {
+ 	return SourcePic->CopyTrueColorPixels(bmp, x, y, rotate, inf);
+ }
+ 
  //==========================================================================
  //
  // Parses a brightmap definition
***************
*** 664,670 ****
  	bool disable_fullbright=false;
  	bool thiswad = false;
  	bool iwad = false;
! 	FTexture *bmtex = NULL;
  
  	sc.MustGetString();
  	if (sc.Compare("texture")) type = FTexture::TEX_Wall;
--- 696,703 ----
  	bool disable_fullbright=false;
  	bool thiswad = false;
  	bool iwad = false;
! 	int maplump = -1;
! 	FString maplumpname;
  
  	sc.MustGetString();
  	if (sc.Compare("texture")) type = FTexture::TEX_Wall;
***************
*** 701,715 ****
  		{
  			sc.MustGetString();
  
! 			if (bmtex != NULL)
  			{
  				Printf("Multiple brightmap definitions in texture %s\n", tex? tex->Name.GetChars() : "(null)");
  			}
  
! 			bmtex = TexMan.FindTexture(sc.String, FTexture::TEX_Any, FTextureManager::TEXMAN_TryAny);
  
! 			if (bmtex == NULL) 
  				Printf("Brightmap '%s' not found in texture '%s'\n", sc.String, tex? tex->Name.GetChars() : "(null)");
  		}
  	}
  	if (!tex)
--- 734,750 ----
  		{
  			sc.MustGetString();
  
! 			if (maplump >= 0)
  			{
  				Printf("Multiple brightmap definitions in texture %s\n", tex? tex->Name.GetChars() : "(null)");
  			}
  
! 			maplump = Wads.CheckNumForFullName(sc.String, true);
  
! 			if (maplump==-1) 
  				Printf("Brightmap '%s' not found in texture '%s'\n", sc.String, tex? tex->Name.GetChars() : "(null)");
+ 
+ 			maplumpname = sc.String;
  		}
  	}
  	if (!tex)
***************
*** 728,735 ****
  		}
  		if (!useme) return;
  	}
! 
! 	if (bmtex != NULL)
  	{
  		if (tex->bWarped != 0)
  		{
--- 763,770 ----
  		}
  		if (!useme) return;
  	}
! /*
! 	if (maplump != -1)
  	{
  		if (tex->bWarped != 0)
  		{
***************
*** 737,745 ****
  			return;
  		}
  
! 		bmtex->gl_info.bBrightmap = true;
! 		tex->gl_info.Brightmap = bmtex;
  	}	
  	tex->gl_info.bBrightmapDisablesFullbright = disable_fullbright;
  }
  
--- 772,804 ----
  			return;
  		}
  
! 		// Brightmap textures are stored in the texture manager so that multiple
! 		// instances of the same textures can be avoided.
! 		FTexture *brightmap;
! 		FTextureID brightmapId = TexMan.FindTextureByLumpNum(maplump);
! 
! 		if (!brightmapId.isValid())
! 		{
! 			// a texture for this lump has not been created yet.
! 			brightmap = FTexture::CreateTexture(maplump, tex->UseType);
! 			if (!brightmap)
! 			{
! 				Printf("Unable to create texture from '%s' in brightmap definition for '%s'\n", 
! 					maplumpname.GetChars(), tex->Name.GetChars());
! 				return;
! 			}
! 			brightmap->gl_info.bBrightmap = true;
! 			brightmap->Name[0] = 0;	// brightmaps don't have names
! 			TexMan.AddTexture(brightmap);
! 		}
! 		else
! 		{
! 			brightmap = TexMan[brightmapId];
! 		}
! 
! 		tex->gl_info.Brightmap = brightmap;
  	}	
+ */
  	tex->gl_info.bBrightmapDisablesFullbright = disable_fullbright;
  }
  
***************
*** 798,839 ****
  	}
  }
  
- 
- //==========================================================================
- //
- // Prints some texture info
- //
- //==========================================================================
- 
- CCMD(textureinfo)
- {
- 	int cntt = 0;
- 	for (int i = 0; i < TexMan.NumTextures(); i++)
- 	{
- 		FTexture *tex = TexMan.ByIndex(i);
- 		if (tex->gl_info.SystemTexture[0] || tex->gl_info.SystemTexture[1] || tex->gl_info.Material[0] || tex->gl_info.Material[1])
- 		{
- 			int lump = tex->GetSourceLump();
- 			Printf(PRINT_LOG, "Texture '%s' (Index %d, Lump %d, Name '%s'):\n", tex->Name.GetChars(), i, lump, Wads.GetLumpFullName(lump));
- 			if (tex->gl_info.Material[0])
- 			{
- 				Printf(PRINT_LOG, "in use (normal)\n");
- 			}
- 			else if (tex->gl_info.SystemTexture[0])
- 			{
- 				Printf(PRINT_LOG, "referenced (normal)\n");
- 			}
- 			if (tex->gl_info.Material[1])
- 			{
- 				Printf(PRINT_LOG, "in use (expanded)\n");
- 			}
- 			else if (tex->gl_info.SystemTexture[1])
- 			{
- 				Printf(PRINT_LOG, "referenced (normal)\n");
- 			}
- 			cntt++;
- 		}
- 	}
- 	Printf(PRINT_LOG, "%d system textures\n", cntt);
- }
- 
--- 857,859 ----
