*** doom/src/main/jni/gzdoom/src/gl/scene/gl_weapon.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_weapon.cpp	2017-06-18 23:34:13.303914951 -0400
***************
*** 70,76 ****
  //
  //==========================================================================
  
! void FGLRenderer::DrawPSprite (player_t * player,pspdef_t *psp,fixed_t sx, fixed_t sy, bool hudModelStep, int OverrideShader, bool alphatexture)
  {
  	float			fU1,fV1;
  	float			fU2,fV2;
--- 69,75 ----
  //
  //==========================================================================
  
! void FGLRenderer::DrawPSprite (player_t * player,pspdef_t *psp,fixed_t sx, fixed_t sy, int cm_index, bool hudModelStep, int OverrideShader)
  {
  	float			fU1,fV1;
  	float			fU2,fV2;
***************
*** 84,90 ****
  	// [BB] In the HUD model step we just render the model and break out. 
  	if ( hudModelStep )
  	{
! 		gl_RenderHUDModel( psp, sx, sy);
  		return;
  	}
  
--- 83,89 ----
  	// [BB] In the HUD model step we just render the model and break out. 
  	if ( hudModelStep )
  	{
! 		gl_RenderHUDModel( psp, sx, sy, cm_index );
  		return;
  	}
  
***************
*** 93,102 ****
  	FTextureID lump = gl_GetSpriteFrame(psp->sprite, psp->frame, 0, 0, &mirror);
  	if (!lump.isValid()) return;
  
! 	FMaterial * tex = FMaterial::ValidateTexture(lump, true, false);
  	if (!tex) return;
  
! 	gl_RenderState.SetMaterial(tex, CLAMP_XY_NOMIP, 0, OverrideShader, alphatexture);
  
  	int vw = viewwidth;
  	int vh = viewheight;
--- 92,101 ----
  	FTextureID lump = gl_GetSpriteFrame(psp->sprite, psp->frame, 0, 0, &mirror);
  	if (!lump.isValid()) return;
  
! 	FMaterial * tex = FMaterial::ValidateTexture(lump, false);
  	if (!tex) return;
  
! 	tex->BindPatch(cm_index, 0, OverrideShader);
  
  	int vw = viewwidth;
  	int vh = viewheight;
***************
*** 104,121 ****
  	// calculate edges of the shape
  	scalex = xratio[WidescreenRatio] * vw / 320;
  
! 	tx = sx - ((160 + tex->GetScaledLeftOffset())<<FRACBITS);
  	x1 = (FixedMul(tx, scalex)>>FRACBITS) + (vw>>1);
  	if (x1 > vw)	return; // off the right side
  	x1+=viewwindowx;
  
! 	tx +=  tex->TextureWidth() << FRACBITS;
  	x2 = (FixedMul(tx, scalex)>>FRACBITS) + (vw>>1);
  	if (x2 < 0) return; // off the left side
  	x2+=viewwindowx;
  
  	// killough 12/98: fix psprite positioning problem
! 	texturemid = (100<<FRACBITS) - (sy-(tex->GetScaledTopOffset()<<FRACBITS));
  
  	AWeapon * wi=player->ReadyWeapon;
  	if (wi && wi->YAdjust)
--- 103,120 ----
  	// calculate edges of the shape
  	scalex = xratio[WidescreenRatio] * vw / 320;
  
! 	tx = sx - ((160 + tex->GetScaledLeftOffset(GLUSE_PATCH))<<FRACBITS);
  	x1 = (FixedMul(tx, scalex)>>FRACBITS) + (vw>>1);
  	if (x1 > vw)	return; // off the right side
  	x1+=viewwindowx;
  
! 	tx +=  tex->TextureWidth(GLUSE_PATCH) << FRACBITS;
  	x2 = (FixedMul(tx, scalex)>>FRACBITS) + (vw>>1);
  	if (x2 < 0) return; // off the left side
  	x2+=viewwindowx;
  
  	// killough 12/98: fix psprite positioning problem
! 	texturemid = (100<<FRACBITS) - (sy-(tex->GetScaledTopOffset(GLUSE_PATCH)<<FRACBITS));
  
  	AWeapon * wi=player->ReadyWeapon;
  	if (wi && wi->YAdjust)
***************
*** 132,138 ****
  
  	scale = ((SCREENHEIGHT*vw)/SCREENWIDTH) / 200.0f;    
  	y1 = viewwindowy + (vh >> 1) - (int)(((float)texturemid / (float)FRACUNIT) * scale);
! 	y2 = y1 + (int)((float)tex->TextureHeight() * scale) + 1;
  
  	if (!mirror)
  	{
--- 131,137 ----
  
  	scale = ((SCREENHEIGHT*vw)/SCREENWIDTH) / 200.0f;    
  	y1 = viewwindowy + (vh >> 1) - (int)(((float)texturemid / (float)FRACUNIT) * scale);
! 	y2 = y1 + (int)((float)tex->TextureHeight(GLUSE_PATCH) * scale) + 1;
  
  	if (!mirror)
  	{
***************
*** 149,170 ****
  		fV2=tex->GetVB();
  	}
  
! 	if (tex->GetTransparent() || OverrideShader != -1)
  	{
! 		gl_RenderState.AlphaFunc(GL_GEQUAL, 0.f);
  	}
  	gl_RenderState.Apply();
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(x1, y1, 0, fU1, fV1);
! 	ptr++;
! 	ptr->Set(x1, y2, 0, fU1, fV2);
! 	ptr++;
! 	ptr->Set(x2, y1, 0, fU2, fV1);
! 	ptr++;
! 	ptr->Set(x2, y2, 0, fU2, fV2);
! 	ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, 0.5f);
  }
  
  //==========================================================================
--- 148,168 ----
  		fV2=tex->GetVB();
  	}
  
! 	if (tex->GetTransparent() || OverrideShader != 0)
  	{
! 		gl_RenderState.EnableAlphaTest(false);
  	}
  	gl_RenderState.Apply();
! 	glBegin(GL_TRIANGLE_STRIP);
! 	glTexCoord2f(fU1, fV1); glVertex2f(x1,y1);
! 	glTexCoord2f(fU1, fV2); glVertex2f(x1,y2);
! 	glTexCoord2f(fU2, fV1); glVertex2f(x2,y1);
! 	glTexCoord2f(fU2, fV2); glVertex2f(x2,y2);
! 	glEnd();
! 	if (tex->GetTransparent() || OverrideShader != 0)
! 	{
! 		gl_RenderState.EnableAlphaTest(true);
! 	}
  }
  
  //==========================================================================
***************
*** 194,225 ****
  		(players[consoleplayer].cheats & CF_CHASECAM))
  		return;
  
  	P_BobWeapon (player, &player->psprites[ps_weapon], &ofsx, &ofsy);
  
  	// check for fullbright
  	if (player->fixedcolormap==NOFIXEDCOLORMAP)
  	{
! 		for (i = 0, psp = player->psprites; i <= ps_flash; i++, psp++)
! 		{
  			if (psp->state != NULL)
  			{
  				bool disablefullbright = false;
  				FTextureID lump = gl_GetSpriteFrame(psp->sprite, psp->frame, 0, 0, NULL);
! 				if (lump.isValid())
  				{
! 					FMaterial * tex=FMaterial::ValidateTexture(lump, false, false);
  					if (tex)
  						disablefullbright = tex->tex->gl_info.bBrightmapDisablesFullbright;
  				}
  				statebright[i] = !!psp->state->GetFullbright() && !disablefullbright;
  			}
! 		}
  	}
  
  	if (gl_fixedcolormap) 
  	{
  		lightlevel=255;
! 		cm.Clear();
  		statebright[0] = statebright[1] = true;
  		fakesec = viewsector;
  	}
--- 192,227 ----
  		(players[consoleplayer].cheats & CF_CHASECAM))
  		return;
  
+ 	/*
+ 	if(!player || playermo->renderflags&RF_INVISIBLE || !r_drawplayersprites ||
+ 		mViewActor!=playermo || playermo->RenderStyle.BlendOp == STYLEOP_None) return;
+ 	*/
+ 
  	P_BobWeapon (player, &player->psprites[ps_weapon], &ofsx, &ofsy);
  
  	// check for fullbright
  	if (player->fixedcolormap==NOFIXEDCOLORMAP)
  	{
! 		for (i=0, psp=player->psprites; i<=ps_flash; i++,psp++)
  			if (psp->state != NULL)
  			{
  				bool disablefullbright = false;
  				FTextureID lump = gl_GetSpriteFrame(psp->sprite, psp->frame, 0, 0, NULL);
! 				if (lump.isValid() && gl_BrightmapsActive())
  				{
! 					FMaterial * tex=FMaterial::ValidateTexture(lump, false);
  					if (tex)
  						disablefullbright = tex->tex->gl_info.bBrightmapDisablesFullbright;
  				}
  				statebright[i] = !!psp->state->GetFullbright() && !disablefullbright;
  			}
! 				
  	}
  
  	if (gl_fixedcolormap) 
  	{
  		lightlevel=255;
! 		cm.GetFixedColormap();
  		statebright[0] = statebright[1] = true;
  		fakesec = viewsector;
  	}
***************
*** 284,292 ****
  		statebright[0] = statebright[1] = true;
  	}
  
! 	PalEntry ThingColor = (playermo->RenderStyle.Flags & STYLEF_ColorIsFixed) ? playermo->fillcolor : 0xffffff;
! 	ThingColor.a = 255;
! 
  	visstyle_t vis;
  
  	vis.RenderStyle=playermo->RenderStyle;
--- 286,292 ----
  		statebright[0] = statebright[1] = true;
  	}
  
! 	PalEntry ThingColor = playermo->fillcolor;
  	visstyle_t vis;
  
  	vis.RenderStyle=playermo->RenderStyle;
***************
*** 297,319 ****
  		playermo->Inventory->AlterWeaponSprite(&vis);
  		if (vis.colormap >= SpecialColormaps[0].Colormap && 
  			vis.colormap < SpecialColormaps[SpecialColormaps.Size()].Colormap && 
! 			gl_fixedcolormap == CM_DEFAULT)
  		{
! 			// this only happens for Strife's inverted weapon sprite
! 			vis.RenderStyle.Flags |= STYLEF_InvertSource;
  		}
  	}
  
  	// Set the render parameters
  
! 	int OverrideShader = -1;
  	float trans = 0.f;
  	if (vis.RenderStyle.BlendOp >= STYLEOP_Fuzz && vis.RenderStyle.BlendOp <= STYLEOP_FuzzOrRevSub)
  	{
  		vis.RenderStyle.CheckFuzz();
  		if (vis.RenderStyle.BlendOp == STYLEOP_Fuzz)
  		{
! 			if (gl_fuzztype != 0)
  			{
  				// Todo: implement shader selection here
  				vis.RenderStyle = LegacyRenderStyles[STYLE_Translucent];
--- 297,319 ----
  		playermo->Inventory->AlterWeaponSprite(&vis);
  		if (vis.colormap >= SpecialColormaps[0].Colormap && 
  			vis.colormap < SpecialColormaps[SpecialColormaps.Size()].Colormap && 
! 			cm.colormap == CM_DEFAULT)
  		{
! 			ptrdiff_t specialmap = (vis.colormap - SpecialColormaps[0].Colormap) / sizeof(FSpecialColormap);
! 			cm.colormap = int(CM_FIRSTSPECIALCOLORMAP + specialmap);
  		}
  	}
  
  	// Set the render parameters
  
! 	int OverrideShader = 0;
  	float trans = 0.f;
  	if (vis.RenderStyle.BlendOp >= STYLEOP_Fuzz && vis.RenderStyle.BlendOp <= STYLEOP_FuzzOrRevSub)
  	{
  		vis.RenderStyle.CheckFuzz();
  		if (vis.RenderStyle.BlendOp == STYLEOP_Fuzz)
  		{
! 			if (gl.shadermodel >= 4 && gl_fuzztype != 0)
  			{
  				// Todo: implement shader selection here
  				vis.RenderStyle = LegacyRenderStyles[STYLE_Translucent];
***************
*** 345,351 ****
  
  	// now draw the different layers of the weapon
  	gl_RenderState.EnableBrightmap(true);
- 	gl_RenderState.SetObjectColor(ThingColor);
  	if (statebright[0] || statebright[1])
  	{
  		// brighten the weapon to reduce the difference between
--- 345,350 ----
***************
*** 366,371 ****
--- 365,371 ----
  			if (statebright[i]) 
  			{
  				if (fakesec == viewsector || in_area != area_below)	
+ 					// under water areas keep most of their color for fullbright objects
  				{
  					cmc.LightColor.r=
  					cmc.LightColor.g=
***************
*** 373,399 ****
  				}
  				else
  				{
! 					// under water areas keep most of their color for fullbright objects
! 					cmc.LightColor.r = (3 * cmc.LightColor.r + 0xff) / 4;
  					cmc.LightColor.g = (3*cmc.LightColor.g + 0xff)/4;
  					cmc.LightColor.b = (3*cmc.LightColor.b + 0xff)/4;
  				}
  			}
! 			// set the lighting parameters
! 			if (vis.RenderStyle.BlendOp == STYLEOP_Shadow)
! 			{
! 				gl_RenderState.SetColor(0.2f, 0.2f, 0.2f, 0.33f, cmc.desaturation);
! 			}
! 			else
! 			{
! 				gl_SetDynSpriteLight(playermo, NULL);
! 				gl_SetColor(statebright[i] ? 255 : lightlevel, 0, cmc, trans, true);
! 			}
! 			DrawPSprite(player, psp, psp->sx + ofsx, psp->sy + ofsy, hudModelStep, OverrideShader, !!(vis.RenderStyle.Flags & STYLEF_RedIsAlpha));
  		}
  	}
- 	gl_RenderState.SetObjectColor(0xffffffff);
- 	gl_RenderState.SetDynLight(0, 0, 0);
  	gl_RenderState.EnableBrightmap(false);
  	glset.lightmode = oldlightmode;
  }
--- 373,389 ----
  				}
  				else
  				{
! 					cmc.LightColor.r = (3*cmc.LightColor.r + 0xff)/4;
  					cmc.LightColor.g = (3*cmc.LightColor.g + 0xff)/4;
  					cmc.LightColor.b = (3*cmc.LightColor.b + 0xff)/4;
  				}
  			}
! 			// set the lighting parameters (only calls glColor and glAlphaFunc)
! 			gl_SetSpriteLighting(vis.RenderStyle, playermo, statebright[i]? 255 : lightlevel, 
! 				0, &cmc, 0xffffff, trans, statebright[i], true);
! 			DrawPSprite (player,psp,psp->sx+ofsx, psp->sy+ofsy, cm.colormap, hudModelStep, OverrideShader);
  		}
  	}
  	gl_RenderState.EnableBrightmap(false);
  	glset.lightmode = oldlightmode;
  }
***************
*** 418,427 ****
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  	gl_RenderState.AlphaFunc(GL_GEQUAL,gl_mask_sprite_threshold);
  	gl_RenderState.BlendEquation(GL_FUNC_ADD);
! 	gl_RenderState.ResetColor();
  	gl_RenderState.SetTextureMode(TM_MODULATE);
  
  	// The Targeter's sprites are always drawn normally.
  	for (i=ps_targetcenter, psp = &player->psprites[ps_targetcenter]; i<NUMPSPRITES; i++,psp++)
! 		if (psp->state) DrawPSprite (player,psp,psp->sx, psp->sy, false, 0, false);
  }
\ No newline at end of file
--- 408,417 ----
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  	gl_RenderState.AlphaFunc(GL_GEQUAL,gl_mask_sprite_threshold);
  	gl_RenderState.BlendEquation(GL_FUNC_ADD);
! 	glColor3f(1.0f,1.0f,1.0f);
  	gl_RenderState.SetTextureMode(TM_MODULATE);
  
  	// The Targeter's sprites are always drawn normally.
  	for (i=ps_targetcenter, psp = &player->psprites[ps_targetcenter]; i<NUMPSPRITES; i++,psp++)
! 		if (psp->state) DrawPSprite (player,psp,psp->sx, psp->sy, CM_DEFAULT, false, 0);
  }
\ No newline at end of file
