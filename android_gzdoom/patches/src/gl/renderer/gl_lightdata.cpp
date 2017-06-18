*** doom/src/main/jni/gzdoom/src/gl/renderer/gl_lightdata.cpp	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/renderer/gl_lightdata.cpp	2017-06-18 23:34:13.273914674 -0400
***************
*** 74,79 ****
--- 74,95 ----
  
  //==========================================================================
  //
+ //
+ //
+ //==========================================================================
+ 
+ bool gl_BrightmapsActive()
+ {
+ 	return gl.shadermodel == 4 || (gl.shadermodel == 3 && gl_brightmap_shader);
+ }
+ 
+ bool gl_GlowActive()
+ {
+ 	return gl.shadermodel == 4 || (gl.shadermodel == 3 && gl_glow_shader);
+ }
+ 
+ //==========================================================================
+ //
  // Sets up the fog tables
  //
  //==========================================================================
***************
*** 109,121 ****
  {
  	if (self>2) self=2;
  	if (self<0) self=0;
  }
! 
  CUSTOM_CVAR(Int, gl_lightmode, 3 ,CVAR_ARCHIVE|CVAR_NOINITCALL)
  {
  	int newself = self;
  	if (newself > 4) newself=8;	// use 8 for software lighting to avoid conflicts with the bit mask
  	if (newself < 0) newself=0;
  	if (self != newself) self = newself;
  	glset.lightmode = newself;
  }
--- 125,142 ----
  {
  	if (self>2) self=2;
  	if (self<0) self=0;
+ 	if (self == 2 && gl.shadermodel < 4) self = 1;
  }
! #ifdef __ANDROID__
! CUSTOM_CVAR(Int, gl_lightmode, 4 ,CVAR_ARCHIVE|CVAR_NOINITCALL)
! #else
  CUSTOM_CVAR(Int, gl_lightmode, 3 ,CVAR_ARCHIVE|CVAR_NOINITCALL)
+ #endif
  {
  	int newself = self;
  	if (newself > 4) newself=8;	// use 8 for software lighting to avoid conflicts with the bit mask
  	if (newself < 0) newself=0;
+ 	if ((newself == 2 || newself == 8) && gl.shadermodel < 4) newself = 3;
  	if (self != newself) self = newself;
  	glset.lightmode = newself;
  }
***************
*** 142,159 ****
  	int blendequation = renderops[style.BlendOp&15];
  	int texturemode = drawopaque? TM_OPAQUE : TM_MODULATE;
  
! 	if (style.Flags & STYLEF_RedIsAlpha)
! 	{
! 		texturemode = TM_REDTOALPHA;
! 	}
! 	else if (style.Flags & STYLEF_ColorIsFixed)
  	{
  		texturemode = TM_MASK;
  	}
  	else if (style.Flags & STYLEF_InvertSource)
  	{
! 		// The only place where InvertSource is used is for inverted sprites with the infrared powerup.
! 		texturemode = TM_INVERSE;
  	}
  
  	if (blendequation == -1)
--- 163,175 ----
  	int blendequation = renderops[style.BlendOp&15];
  	int texturemode = drawopaque? TM_OPAQUE : TM_MODULATE;
  
! 	if (style.Flags & STYLEF_ColorIsFixed)
  	{
  		texturemode = TM_MASK;
  	}
  	else if (style.Flags & STYLEF_InvertSource)
  	{
! 		texturemode = drawopaque? TM_INVERTOPAQUE : TM_INVERT;
  	}
  
  	if (blendequation == -1)
***************
*** 227,237 ****
  //
  //==========================================================================
  
! static PalEntry gl_CalcLightColor(int light, PalEntry pe, int blendfactor)
  {
  	int r,g,b;
  
! 	if (glset.lightmode == 8)
  	{
  		return pe;
  	}
--- 243,253 ----
  //
  //==========================================================================
  
! PalEntry gl_CalcLightColor(int light, PalEntry pe, int blendfactor, bool force)
  {
  	int r,g,b;
  
! 	if (glset.lightmode == 8 && !force)
  	{
  		return pe;
  	}
***************
*** 243,257 ****
  	}
  	else
  	{
- 		// This is what Legacy does with colored light in 3D volumes. No, it doesn't really make sense...
- 		// It also doesn't translate well to software style lighting.
  		int mixlight = light * (255 - blendfactor);
  
  		r = (mixlight + pe.r * blendfactor) / 255;
  		g = (mixlight + pe.g * blendfactor) / 255;
  		b = (mixlight + pe.b * blendfactor) / 255;
  	}
! 	return PalEntry(255, BYTE(r), BYTE(g), BYTE(b));
  }
  
  //==========================================================================
--- 259,325 ----
  	}
  	else
  	{
  		int mixlight = light * (255 - blendfactor);
  
  		r = (mixlight + pe.r * blendfactor) / 255;
  		g = (mixlight + pe.g * blendfactor) / 255;
  		b = (mixlight + pe.b * blendfactor) / 255;
  	}
! 	return PalEntry(BYTE(r), BYTE(g), BYTE(b));
! }
! 
! //==========================================================================
! //
! // Get current light color
! //
! //==========================================================================
! void gl_GetLightColor(int lightlevel, int rellight, const FColormap * cm, float * pred, float * pgreen, float * pblue, bool weapon)
! {
! 	float & r=*pred,& g=*pgreen,& b=*pblue;
! 	int torch=0;
! 
! 	if (gl_fixedcolormap) 
! 	{
! 		if (gl_fixedcolormap==CM_LITE)
! 		{
! 			if (gl_enhanced_nightvision) r=0.375f, g=1.0f, b=0.375f;
! 			else r=g=b=1.0f;
! 		}
! 		else if (gl_fixedcolormap>=CM_TORCH)
! 		{
! 			int flicker=gl_fixedcolormap-CM_TORCH;
! 			r=(0.8f+(7-flicker)/70.0f);
! 			if (r>1.0f) r=1.0f;
! 			b=g=r;
! 			if (gl_enhanced_nightvision) b*=0.75f;
! 		}
! 		else r=g=b=1.0f;
! 		return;
! 	}
! 
! 	PalEntry lightcolor = cm? cm->LightColor : PalEntry(255,255,255);
! 	int blendfactor = cm? cm->blendfactor : 0;
! 
! 	lightlevel = gl_CalcLightLevel(lightlevel, rellight, weapon);
! 	PalEntry pe = gl_CalcLightColor(lightlevel, lightcolor, blendfactor);
! 	r = pe.r/255.f;
! 	g = pe.g/255.f;
! 	b = pe.b/255.f;
! }
! 
! //==========================================================================
! //
! // set current light color
! //
! //==========================================================================
! void gl_SetColor(int light, int rellight, const FColormap * cm, float *red, float *green, float *blue, PalEntry ThingColor, bool weapon)
! { 
! 	float r,g,b;
! 	gl_GetLightColor(light, rellight, cm, &r, &g, &b, weapon);
! 
! 	*red = r * ThingColor.r/255.0f;
! 	*green = g * ThingColor.g/255.0f;
! 	*blue = b * ThingColor.b/255.0f;
  }
  
  //==========================================================================
***************
*** 259,277 ****
  // set current light color
  //
  //==========================================================================
! void gl_SetColor(int sectorlightlevel, int rellight, const FColormap &cm, float alpha, bool weapon)
  { 
! 	if (gl_fixedcolormap != CM_DEFAULT)
  	{
! 		gl_RenderState.SetColorAlpha(0xffffff, alpha, 0);
! 		gl_RenderState.SetSoftLightLevel(255);
  	}
  	else
! 	{
! 		int hwlightlevel = gl_CalcLightLevel(sectorlightlevel, rellight, weapon);
! 		PalEntry pe = gl_CalcLightColor(hwlightlevel, cm.LightColor, cm.blendfactor);
! 		gl_RenderState.SetColorAlpha(pe, alpha, cm.desaturation);
! 		gl_RenderState.SetSoftLightLevel(gl_ClampLight(sectorlightlevel + rellight));
  	}
  }
  
--- 327,355 ----
  // set current light color
  //
  //==========================================================================
! void gl_SetColor(int light, int rellight, const FColormap * cm, float alpha, PalEntry ThingColor, bool weapon)
  { 
! 	float r,g,b;
! 
! 	gl_GetLightColor(light, rellight, cm, &r, &g, &b, weapon);
! 
! 	if (glset.lightmode != 8)
  	{
! 		glColor4f(r * ThingColor.r/255.0f, g * ThingColor.g/255.0f, b * ThingColor.b/255.0f, alpha);
  	}
  	else
! 	{ 
! 		glColor4f(r, g, b, alpha);
! 
! 		if (gl_fixedcolormap)
! 		{
! 			glVertexAttrib1f(VATTR_LIGHTLEVEL, 1.0);
! 		}
! 		else
! 		{
! 			float lightlevel = gl_CalcLightLevel(light, rellight, weapon) / 255.0f;
! 			glVertexAttrib1f(VATTR_LIGHTLEVEL, lightlevel); 
! 		}
  	}
  }
  
***************
*** 375,435 ****
  //
  // Check if the current linedef is a candidate for a fog boundary
  //
- // Requirements for a fog boundary:
- // - front sector has no fog
- // - back sector has fog
- // - at least one of both does not have a sky ceiling.
- //
  //==========================================================================
  
  bool gl_CheckFog(sector_t *frontsector, sector_t *backsector)
  {
- 	if (gl_fixedcolormap) return false;
- 	if (frontsector == backsector) return false;	// there can't be a boundary if both sides are in the same sector.
- 
  	// Check for fog boundaries. This needs a few more checks for the sectors
  
  	PalEntry fogcolor = frontsector->ColorMap->Fade;
  
  	if ((fogcolor.d & 0xffffff) == 0)
  	{
! 		return false;
  	}
  	else if (outsidefogdensity != 0 && outsidefogcolor.a!=0xff && (fogcolor.d & 0xffffff) == (outsidefogcolor.d & 0xffffff))
  	{
  	}
  	else  if (fogdensity!=0 || (glset.lightmode & 4))
  	{
  		// case 3: level has fog density set
  	}
  	else 
  	{
  		// case 4: use light level
! 		if (frontsector->lightlevel >= 248) return false;
  	}
  
  	fogcolor = backsector->ColorMap->Fade;
  
  	if ((fogcolor.d & 0xffffff) == 0)
  	{
  	}
  	else if (outsidefogdensity != 0 && outsidefogcolor.a!=0xff && (fogcolor.d & 0xffffff) == (outsidefogcolor.d & 0xffffff))
  	{
! 		return false;
  	}
  	else  if (fogdensity!=0 || (glset.lightmode & 4))
  	{
  		// case 3: level has fog density set
! 		return false;
  	}
  	else 
  	{
  		// case 4: use light level
! 		if (backsector->lightlevel < 248) return false;
  	}
  
  	// in all other cases this might create more problems than it solves.
! 	return ((frontsector->GetTexture(sector_t::ceiling)!=skyflatnum || 
  			 backsector->GetTexture(sector_t::ceiling)!=skyflatnum));
  }
  
--- 453,512 ----
  //
  // Check if the current linedef is a candidate for a fog boundary
  //
  //==========================================================================
  
  bool gl_CheckFog(sector_t *frontsector, sector_t *backsector)
  {
  	// Check for fog boundaries. This needs a few more checks for the sectors
+ 	bool frontfog, backfog;
  
  	PalEntry fogcolor = frontsector->ColorMap->Fade;
  
  	if ((fogcolor.d & 0xffffff) == 0)
  	{
! 		frontfog = false;
  	}
  	else if (outsidefogdensity != 0 && outsidefogcolor.a!=0xff && (fogcolor.d & 0xffffff) == (outsidefogcolor.d & 0xffffff))
  	{
+ 		frontfog = true;
  	}
  	else  if (fogdensity!=0 || (glset.lightmode & 4))
  	{
  		// case 3: level has fog density set
+ 		frontfog = true;
  	}
  	else 
  	{
  		// case 4: use light level
! 		frontfog = frontsector->lightlevel < 248;
  	}
  
+ 	if (backsector == NULL) return frontfog;
+ 
  	fogcolor = backsector->ColorMap->Fade;
  
  	if ((fogcolor.d & 0xffffff) == 0)
  	{
+ 		backfog = false;
  	}
  	else if (outsidefogdensity != 0 && outsidefogcolor.a!=0xff && (fogcolor.d & 0xffffff) == (outsidefogcolor.d & 0xffffff))
  	{
! 		backfog = true;
  	}
  	else  if (fogdensity!=0 || (glset.lightmode & 4))
  	{
  		// case 3: level has fog density set
! 		backfog = true;
  	}
  	else 
  	{
  		// case 4: use light level
! 		backfog = backsector->lightlevel < 248;
  	}
  
  	// in all other cases this might create more problems than it solves.
! 	return (frontfog && !backfog && !gl_fixedcolormap &&
! 			(frontsector->GetTexture(sector_t::ceiling)!=skyflatnum || 
  			 backsector->GetTexture(sector_t::ceiling)!=skyflatnum));
  }
  
***************
*** 441,449 ****
--- 518,532 ----
  
  void gl_SetShaderLight(float level, float olight)
  {
+ #if 1 //ndef _DEBUG
  	const float MAXDIST = 256.f;
  	const float THRESHOLD = 96.f;
  	const float FACTOR = 0.75f;
+ #else
+ 	const float MAXDIST = 256.f;
+ 	const float THRESHOLD = 96.f;
+ 	const float FACTOR = 2.75f;
+ #endif
  
  	if (level > 0)
  	{
***************
*** 498,504 ****
  	// Make fog a little denser when inside a skybox
  	if (GLPortal::inskybox) fogdensity+=fogdensity/2;
  
! 
  	// no fog in enhanced vision modes!
  	if (fogdensity==0 || gl_fogmode == 0)
  	{
--- 581,598 ----
  	// Make fog a little denser when inside a skybox
  	if (GLPortal::inskybox) fogdensity+=fogdensity/2;
  
! 	/*
! #ifdef __ANDROID__  //Fog makes werid graphic artifacts on brutal doom, disable for now
! 	gl_RenderState.EnableFog(false);
! 	gl_RenderState.SetFog(0,0);
! 	return;
! #endif
! */
! #ifdef __ANDROID__	//The hell, for brutal doom white lines?
! 	//Now done in gl_interface.cpp
! 	//glDisable(GL_CLIP_PLANE0);
! 	//glEnable(GL_CLIP_PLANE0);
! #endif
  	// no fog in enhanced vision modes!
  	if (fogdensity==0 || gl_fogmode == 0)
  	{
***************
*** 523,540 ****
  		{
  			fogcolor=0;
  		}
  
  		gl_RenderState.EnableFog(true);
  		gl_RenderState.SetFog(fogcolor, fogdensity);
  
  		// Korshun: fullbright fog like in software renderer.
  		if (glset.lightmode == 8 && glset.brightfog && fogdensity != 0 && fogcolor != 0)
! 		{
! 			gl_RenderState.SetSoftLightLevel(255);
! 		}
  	}
  }
  
  
  //==========================================================================
  //
--- 617,658 ----
  		{
  			fogcolor=0;
  		}
+ 		// Handle desaturation
+ 		if (cmap->colormap != CM_DEFAULT)
+ 			gl_ModifyColor(fogcolor.r, fogcolor.g, fogcolor.b, cmap->colormap);
  
  		gl_RenderState.EnableFog(true);
  		gl_RenderState.SetFog(fogcolor, fogdensity);
  
  		// Korshun: fullbright fog like in software renderer.
  		if (glset.lightmode == 8 && glset.brightfog && fogdensity != 0 && fogcolor != 0)
! 			glVertexAttrib1f(VATTR_LIGHTLEVEL, 1.0);
  	}
  }
  
+ //==========================================================================
+ //
+ // Modifies a color according to a specified colormap
+ //
+ //==========================================================================
+ 
+ void gl_ModifyColor(BYTE & red, BYTE & green, BYTE & blue, int cm)
+ {
+ 	int gray = (red*77 + green*143 + blue*36)>>8;
+ 	if (cm >= CM_FIRSTSPECIALCOLORMAP && cm < CM_MAXCOLORMAP)
+ 	{
+ 		PalEntry pe = SpecialColormaps[cm - CM_FIRSTSPECIALCOLORMAP].GrayscaleToColor[gray];
+ 		red = pe.r;
+ 		green = pe.g;
+ 		blue = pe.b;
+ 	}
+ 	else if (cm >= CM_DESAT1 && cm <= CM_DESAT31)
+ 	{
+ 		gl_Desaturate(gray, red, green, blue, red, green, blue, cm - CM_DESAT0);
+ 	}
+ }
+ 
+ 
  
  //==========================================================================
  //
