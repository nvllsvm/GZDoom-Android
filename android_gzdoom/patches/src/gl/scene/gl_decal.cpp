*** doom/src/main/jni/gzdoom/src/gl/scene/gl_decal.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_decal.cpp	2017-06-18 23:34:13.320581772 -0400
***************
*** 75,81 ****
  	int light;
  	int rel;
  	float a;
! 	bool flipx, flipy;
  	DecalVertex dv[4];
  	FTextureID decalTile;
  	
--- 74,80 ----
  	int light;
  	int rel;
  	float a;
! 	bool flipx, flipy, loadAlpha;
  	DecalVertex dv[4];
  	FTextureID decalTile;
  	
***************
*** 103,109 ****
  	FMaterial *tex;
  
  
! 	tex = FMaterial::ValidateTexture(texture, true);
  
  
  	// the sectors are only used for their texture origin coordinates
--- 102,118 ----
  	FMaterial *tex;
  
  
! 	if (texture->UseType == FTexture::TEX_MiscPatch)
! 	{
! 		// We need to create a clone of this texture where we can force the
! 		// texture filtering offset in.
! 		if (texture->gl_info.DecalTexture == NULL)
! 		{
! 			texture->gl_info.DecalTexture = new FCloneTexture(texture, FTexture::TEX_Decal);
! 		}
! 		tex = FMaterial::ValidateTexture(texture->gl_info.DecalTexture);
! 	}
! 	else tex = FMaterial::ValidateTexture(texture);
  
  
  	// the sectors are only used for their texture origin coordinates
***************
*** 170,191 ****
  		rel = rellight + getExtraLight();
  	}
  	
  	FColormap p = Colormap;
  	
  	if (glset.nocoloredspritelighting)
  	{
! 		p.Decolorize();
  	}
  	
  	
  	
  	a = FIXED2FLOAT(decal->Alpha);
  	
  	// now clip the decal to the actual polygon
! 	float decalwidth = tex->TextureWidth()  * FIXED2FLOAT(decal->ScaleX);
! 	float decalheight= tex->TextureHeight() * FIXED2FLOAT(decal->ScaleY);
! 	float decallefto = tex->GetLeftOffset() * FIXED2FLOAT(decal->ScaleX);
! 	float decaltopo  = tex->GetTopOffset()  * FIXED2FLOAT(decal->ScaleY);
  
  	
  	float leftedge = glseg.fracleft * side->TexelLength;
--- 179,256 ----
  		rel = rellight + getExtraLight();
  	}
  	
+ 	int r = RPART(decal->AlphaColor);
+ 	int g = GPART(decal->AlphaColor);
+ 	int b = BPART(decal->AlphaColor);
  	FColormap p = Colormap;
  	
  	if (glset.nocoloredspritelighting)
  	{
! 		int v = (Colormap.LightColor.r * 77 + Colormap.LightColor.g*143 + Colormap.LightColor.b*35)/255;
! 		p.LightColor = PalEntry(p.colormap, v, v, v);
  	}
  	
+ 	float red, green, blue;
+ 	
+ 	if (decal->RenderStyle.Flags & STYLEF_RedIsAlpha)
+ 	{
+ 		loadAlpha = true;
+ 		p.colormap=CM_SHADE;
+ 
+ 		if (glset.lightmode != 8)
+ 		{
+ 			gl_GetLightColor(light, rel, &p, &red, &green, &blue);
+ 		}
+ 		else
+ 		{
+ 			gl_GetLightColor(lightlevel, rellight, &p, &red, &green, &blue);
+ 		}
+ 		
+ 		if (gl_lights && GLRenderer->mLightCount && !gl_fixedcolormap && gl_light_sprites)
+ 		{
+ 			float result[3];
+ 			fixed_t x, y;
+ 			decal->GetXY(seg->sidedef, x, y);
+ 			gl_GetSpriteLight(NULL, x, y, zpos, sub, Colormap.colormap-CM_DESAT0, result, line, side == line->sidedef[0]? 0:1);
+ 			if (glset.lightmode != 8)
+ 			{
+ 				red = clamp<float>(result[0]+red, 0, 1.0f);
+ 				green = clamp<float>(result[1]+green, 0, 1.0f);
+ 				blue = clamp<float>(result[2]+blue, 0, 1.0f);
+ 			}
+ 			else
+ 			{
+ 				gl_RenderState.SetDynLight(result[0], result[1], result[2]);
+ 			}
+ 		}
+ 
+ 		BYTE R = xs_RoundToInt(r * red);
+ 		BYTE G = xs_RoundToInt(g * green);
+ 		BYTE B = xs_RoundToInt(b * blue);
+ 
+ 		gl_ModifyColor(R,G,B, Colormap.colormap);
+ 
+ 		red = R/255.f;
+ 		green = G/255.f;
+ 		blue = B/255.f;
+ 	}	
+ 	else
+ 	{
+ 		loadAlpha = false;
+ 		
+ 		red = 1.f;
+ 		green = 1.f;
+ 		blue = 1.f;
+ 	}
  	
  	
  	a = FIXED2FLOAT(decal->Alpha);
  	
  	// now clip the decal to the actual polygon
! 	float decalwidth = tex->TextureWidth(GLUSE_PATCH)  * FIXED2FLOAT(decal->ScaleX);
! 	float decalheight= tex->TextureHeight(GLUSE_PATCH) * FIXED2FLOAT(decal->ScaleY);
! 	float decallefto = tex->GetLeftOffset(GLUSE_PATCH) * FIXED2FLOAT(decal->ScaleX);
! 	float decaltopo  = tex->GetTopOffset(GLUSE_PATCH)  * FIXED2FLOAT(decal->ScaleY);
  
  	
  	float leftedge = glseg.fracleft * side->TexelLength;
***************
*** 234,239 ****
--- 299,306 ----
  		
  	zpos+= FRACUNIT*(flipy? decalheight-decaltopo : decaltopo);
  
+ 	tex->BindPatch(p.colormap, decal->Translation);
+ 
  	dv[1].z=dv[2].z = FIXED2FLOAT(zpos);
  	dv[0].z=dv[3].z = dv[1].z - decalheight;
  	dv[1].v=dv[2].v = tex->GetVT();
***************
*** 292,316 ****
  		float vb = tex->GetVB();
  		for(i=0;i<4;i++) dv[i].v=vb-dv[i].v;
  	}
  
! 	// calculate dynamic light effect.
! 	if (gl_lights && GLRenderer->mLightCount && !gl_fixedcolormap && gl_light_sprites)
  	{
! 		// Note: This should be replaced with proper shader based lighting.
! 		fixed_t x, y;
! 		decal->GetXY(seg->sidedef, x, y);
! 		gl_SetDynSpriteLight(NULL, x, y, zpos, sub);
! 	}
  
! 	// alpha color only has an effect when using an alpha texture.
! 	if (decal->RenderStyle.Flags & STYLEF_RedIsAlpha)
  	{
! 		gl_RenderState.SetObjectColor(decal->AlphaColor);
  	}
  
- 	gl_SetColor(light, rel, p, a);
- 
- 	// for additively drawn decals we must temporarily set the fog color to black.
  	PalEntry fc = gl_RenderState.GetFogColor();
  	if (decal->RenderStyle.BlendOp == STYLEOP_Add && decal->RenderStyle.DestAlpha == STYLEALPHA_One)
  	{
--- 359,390 ----
  		float vb = tex->GetVB();
  		for(i=0;i<4;i++) dv[i].v=vb-dv[i].v;
  	}
+ 	// fog is set once per wall in the calling function and not per decal!
  
! 	if (loadAlpha)
  	{
! 		glColor4f(red, green, blue, a);
  
! 		if (glset.lightmode == 8)
! 		{
! 			if (gl_fixedcolormap)
! 				glVertexAttrib1f(VATTR_LIGHTLEVEL, 1.0);
! 			else
! 				glVertexAttrib1f(VATTR_LIGHTLEVEL, gl_CalcLightLevel(light, rel, false) / 255.0);
! 		}
! 	}
! 	else
  	{
! 		if (glset.lightmode == 8)
! 		{
! 			gl_SetColor(light, rel, &p, a, extralight); // Korshun.
! 		}
! 		else
! 		{
! 			gl_SetColor(light, rel, &p, a);
! 		}
  	}
  
  	PalEntry fc = gl_RenderState.GetFogColor();
  	if (decal->RenderStyle.BlendOp == STYLEOP_Add && decal->RenderStyle.DestAlpha == STYLEALPHA_One)
  	{
***************
*** 319,343 ****
  
  
  	gl_SetRenderStyle(decal->RenderStyle, false, false);
- 	gl_RenderState.SetMaterial(tex, CLAMP_XY, decal->Translation, 0, !!(decal->RenderStyle.Flags & STYLEF_RedIsAlpha));
- 
  
  	// If srcalpha is one it looks better with a higher alpha threshold
! 	if (decal->RenderStyle.SrcAlpha == STYLEALPHA_One) gl_RenderState.AlphaFunc(GL_GEQUAL, gl_mask_sprite_threshold);
  	else gl_RenderState.AlphaFunc(GL_GREATER, 0.f);
  
  	gl_RenderState.Apply();
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 	for (i = 0; i < 4; i++)
  	{
! 		ptr->Set(dv[i].x, dv[i].z, dv[i].y, dv[i].u, dv[i].v);
! 		ptr++;
  	}
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_FAN);
! 
  	rendered_decals++;
- 	gl_RenderState.SetTextureMode(TM_MODULATE);
- 	gl_RenderState.SetObjectColor(0xffffffff);
  	gl_RenderState.SetFog(fc,-1);
  	gl_RenderState.SetDynLight(0,0,0);
  }
--- 393,412 ----
  
  
  	gl_SetRenderStyle(decal->RenderStyle, false, false);
  
  	// If srcalpha is one it looks better with a higher alpha threshold
! 	if (decal->RenderStyle.SrcAlpha == STYLEALPHA_One) gl_RenderState.AlphaFunc(GL_GEQUAL, gl_mask_threshold);
  	else gl_RenderState.AlphaFunc(GL_GREATER, 0.f);
  
  	gl_RenderState.Apply();
! 	glBegin(GL_TRIANGLE_FAN);
! 	for(i=0;i<4;i++)
  	{
! 		glTexCoord2f(dv[i].u,dv[i].v);
! 		glVertex3f(dv[i].x,dv[i].z,dv[i].y);
  	}
! 	glEnd();
  	rendered_decals++;
  	gl_RenderState.SetFog(fc,-1);
  	gl_RenderState.SetDynLight(0,0,0);
  }
***************
*** 349,363 ****
  //==========================================================================
  void GLWall::DoDrawDecals()
  {
! 	if (seg->sidedef && seg->sidedef->AttachedDecals)
  	{
! 		gl_SetFog(lightlevel, rellight + getExtraLight(), &Colormap, false);
! 		DBaseDecal *decal = seg->sidedef->AttachedDecals;
! 		while (decal)
! 		{
! 			DrawDecal(decal);
! 			decal = decal->WallNext;
! 		}
  	}
  }
  
--- 418,428 ----
  //==========================================================================
  void GLWall::DoDrawDecals()
  {
! 	DBaseDecal *decal = seg->sidedef->AttachedDecals;
! 	while (decal)
  	{
! 		DrawDecal(decal);
! 		decal = decal->WallNext;
  	}
  }
  
