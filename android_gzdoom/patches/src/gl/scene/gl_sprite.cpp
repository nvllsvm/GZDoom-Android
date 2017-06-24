*** doom/src/main/jni/gzdoom/src/gl/scene/gl_sprite.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_sprite.cpp	2017-06-18 23:34:13.337248593 -0400
***************
*** 112,119 ****
  //==========================================================================
  void GLSprite::Draw(int pass)
  {
! 	if (pass == GLPASS_DECALS || pass == GLPASS_LIGHTSONLY) return;
  
  
  
  	bool additivefog = false;
--- 110,121 ----
  //==========================================================================
  void GLSprite::Draw(int pass)
  {
! 	if (pass!=GLPASS_PLAIN && pass != GLPASS_ALL && pass!=GLPASS_TRANSLUCENT) return;
  
+ 	// Hack to enable bright sprites in faded maps
+ 	uint32 backupfade = Colormap.FadeColor.d;
+ 	if (gl_spritebrightfog && fullbright)
+ 		Colormap.FadeColor = 0;
  
  
  	bool additivefog = false;
***************
*** 124,147 ****
  	{
  		// The translucent pass requires special setup for the various modes.
  
! 		// for special render styles brightmaps would not look good - especially for subtractive.
! 		if (RenderStyle.BlendOp != STYLEOP_Add)
  		{
  			gl_RenderState.EnableBrightmap(false);
  		}
  
  		gl_SetRenderStyle(RenderStyle, false, 
  			// The rest of the needed checks are done inside gl_SetRenderStyle
! 			trans > 1.f - FLT_EPSILON && gl_usecolorblending && gl_fixedcolormap == CM_DEFAULT && actor && 
  			fullbright && gltexture && !gltexture->GetTransparent());
  
  		if (hw_styleflags == STYLEHW_NoAlphaTest)
  		{
! 			gl_RenderState.AlphaFunc(GL_GEQUAL, 0.f);
  		}
  		else
  		{
! 			gl_RenderState.AlphaFunc(GL_GEQUAL, gl_mask_sprite_threshold);
  		}
  
  		if (RenderStyle.BlendOp == STYLEOP_Shadow)
--- 126,150 ----
  	{
  		// The translucent pass requires special setup for the various modes.
  
! 		// Brightmaps will only be used when doing regular drawing ops and having no fog
! 		if (!gl_spritebrightfog && (!gl_isBlack(Colormap.FadeColor) || level.flags&LEVEL_HASFADETABLE || 
! 			RenderStyle.BlendOp != STYLEOP_Add))
  		{
  			gl_RenderState.EnableBrightmap(false);
  		}
  
  		gl_SetRenderStyle(RenderStyle, false, 
  			// The rest of the needed checks are done inside gl_SetRenderStyle
! 			trans > 1.f - FLT_EPSILON && gl_usecolorblending && gl_fixedcolormap < CM_FIRSTSPECIALCOLORMAP && actor && 
  			fullbright && gltexture && !gltexture->GetTransparent());
  
  		if (hw_styleflags == STYLEHW_NoAlphaTest)
  		{
! 			gl_RenderState.EnableAlphaTest(false);
  		}
  		else
  		{
! 			gl_RenderState.AlphaFunc(GL_GEQUAL,trans*gl_mask_sprite_threshold);
  		}
  
  		if (RenderStyle.BlendOp == STYLEOP_Shadow)
***************
*** 165,172 ****
  				minalpha*=factor;
  			}
  
! 			gl_RenderState.AlphaFunc(GL_GEQUAL, gl_mask_sprite_threshold);
! 			gl_RenderState.SetColor(0.2f,0.2f,0.2f,fuzzalpha, Colormap.desaturation);
  			additivefog = true;
  		}
  		else if (RenderStyle.BlendOp == STYLEOP_Add && RenderStyle.DestAlpha == STYLEALPHA_One)
--- 168,175 ----
  				minalpha*=factor;
  			}
  
! 			gl_RenderState.AlphaFunc(GL_GEQUAL,minalpha*gl_mask_sprite_threshold);
! 			glColor4f(0.2f,0.2f,0.2f,fuzzalpha);
  			additivefog = true;
  		}
  		else if (RenderStyle.BlendOp == STYLEOP_Add && RenderStyle.DestAlpha == STYLEALPHA_One)
***************
*** 176,188 ****
  	}
  	if (RenderStyle.BlendOp!=STYLEOP_Shadow)
  	{
! 		if (gl_lights && GLRenderer->mLightCount && !gl_fixedcolormap)
  		{
! 			gl_SetDynSpriteLight(gl_light_sprites ? actor : NULL, gl_light_particles ? particle : NULL);
  		}
! 		gl_SetColor(lightlevel, rel, Colormap, trans);
  	}
- 	gl_RenderState.SetObjectColor(ThingColor);
  
  	if (gl_isBlack(Colormap.FadeColor)) foglevel=lightlevel;
  
--- 179,202 ----
  	}
  	if (RenderStyle.BlendOp!=STYLEOP_Shadow)
  	{
! 		if (actor)
  		{
! 			lightlevel = gl_SetSpriteLighting(RenderStyle, actor, lightlevel, rel, &Colormap, ThingColor, trans,
! 							 fullbright || gl_fixedcolormap >= CM_FIRSTSPECIALCOLORMAP, false);
  		}
! 		else if (particle)
! 		{
! 			if (gl_light_particles)
! 			{
! 				lightlevel = gl_SetSpriteLight(particle, lightlevel, rel, &Colormap, trans, ThingColor);
! 			}
! 			else 
! 			{
! 				gl_SetColor(lightlevel, rel, &Colormap, trans, ThingColor);
! 			}
! 		}
! 		else return;
  	}
  
  	if (gl_isBlack(Colormap.FadeColor)) foglevel=lightlevel;
  
***************
*** 192,197 ****
--- 206,216 ----
  		additivefog = true;
  	}
  
+ 	if (RenderStyle.Flags & STYLEF_InvertOverlay) 
+ 	{
+ 		Colormap.FadeColor = Colormap.FadeColor.InverseColor();
+ 		additivefog=false;
+ 	}
  	if (RenderStyle.BlendOp == STYLEOP_RevSub || RenderStyle.BlendOp == STYLEOP_Sub)
  	{
  		if (!modelframe)
***************
*** 199,207 ****
  			// non-black fog with subtractive style needs special treatment
  			if (!gl_isBlack(Colormap.FadeColor))
  			{
! 				foglayer = true;
! 				// Due to the two-layer approach we need to force an alpha test that lets everything pass
! 				gl_RenderState.AlphaFunc(GL_GREATER, 0);
  			}
  		}
  		else RenderStyle.BlendOp = STYLEOP_Fuzz;	// subtractive with models is not going to work.
--- 218,235 ----
  			// non-black fog with subtractive style needs special treatment
  			if (!gl_isBlack(Colormap.FadeColor))
  			{
! 				if (gl.shadermodel >= 4 && !gl_nolayer)
! 				{
! 					// fog layer only works on modern hardware. 
! 					foglayer = true;
! 					// Due to the two-layer approach we need to force an alpha test that lets everything pass
! 					gl_RenderState.AlphaFunc(GL_GREATER, 0);
! 				}
! 				else
! 				{
! 					// this at least partially handles the fog issue
! 					Colormap.FadeColor = Colormap.FadeColor.InverseColor();
! 				}
  			}
  		}
  		else RenderStyle.BlendOp = STYLEOP_Fuzz;	// subtractive with models is not going to work.
***************
*** 214,220 ****
  		gl_RenderState.SetFog(0, 0);
  	}
  
! 	if (gltexture) gl_RenderState.SetMaterial(gltexture, CLAMP_XY, translation, OverrideShader, !!(RenderStyle.Flags & STYLEF_RedIsAlpha));
  	else if (!modelframe) gl_RenderState.EnableTexture(false);
  
  	if (!modelframe)
--- 242,248 ----
  		gl_RenderState.SetFog(0, 0);
  	}
  
! 	if (gltexture) gltexture->BindPatch(Colormap.colormap, translation, OverrideShader);
  	else if (!modelframe) gl_RenderState.EnableTexture(false);
  
  	if (!modelframe)
***************
*** 258,275 ****
  			v4 = Vector(x2, z2, y2);
  		}
  
! 		FFlatVertex *ptr;
! 		unsigned int offset, count;
! 		ptr = GLRenderer->mVBO->GetBuffer();
! 		ptr->Set(v1[0], v1[1], v1[2], ul, vt);
! 		ptr++;
! 		ptr->Set(v2[0], v2[1], v2[2], ur, vt);
! 		ptr++;
! 		ptr->Set(v3[0], v3[1], v3[2], ul, vb);
! 		ptr++;
! 		ptr->Set(v4[0], v4[1], v4[2], ur, vb);
! 		ptr++;
! 		GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP, &offset, &count);
  
  		if (foglayer)
  		{
--- 286,317 ----
  			v4 = Vector(x2, z2, y2);
  		}
  
! #ifdef __ANDROID__ //This hack is needed to get the additive blending alpha wot work, needs the change in renderstyle.cpp, not sure why this is needed
! 		if ((RenderStyle.SrcAlpha == STYLEALPHA_One) && (RenderStyle.DestAlpha == STYLEALPHA_One))
! 		{
! 			//LOGI("change = %f",trans);
! 			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
! 			//glColor4f(1,1,1,trans);
! 		}
! #endif
! 
! 		glBegin(GL_TRIANGLE_STRIP);
! 		if (gltexture)
! 		{
! 			glTexCoord2f(ul, vt); glVertex3fv(&v1[0]);
! 			glTexCoord2f(ur, vt); glVertex3fv(&v2[0]);
! 			glTexCoord2f(ul, vb); glVertex3fv(&v3[0]);
! 			glTexCoord2f(ur, vb); glVertex3fv(&v4[0]);
! 		}
! 		else	// Particle
! 		{
! 			glVertex3fv(&v1[0]);
! 			glVertex3fv(&v2[0]);
! 			glVertex3fv(&v3[0]);
! 			glVertex3fv(&v4[0]);
! 		}
! 
! 		glEnd();
  
  		if (foglayer)
  		{
***************
*** 279,291 ****
  			gl_RenderState.BlendEquation(GL_FUNC_ADD);
  			gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  			gl_RenderState.Apply();
! 			GLRenderer->mVBO->RenderArray(GL_TRIANGLE_STRIP, offset, count);
! 			gl_RenderState.SetFixedColormap(CM_DEFAULT);
  		}
  	}
  	else
  	{
! 		gl_RenderModel(this);
  	}
  
  	if (pass==GLPASS_TRANSLUCENT)
--- 321,349 ----
  			gl_RenderState.BlendEquation(GL_FUNC_ADD);
  			gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  			gl_RenderState.Apply();
! 
! 			glBegin(GL_TRIANGLE_STRIP);
! 			if (gltexture)
! 			{
! 				glTexCoord2f(ul, vt); glVertex3fv(&v1[0]);
! 				glTexCoord2f(ur, vt); glVertex3fv(&v2[0]);
! 				glTexCoord2f(ul, vb); glVertex3fv(&v3[0]);
! 				glTexCoord2f(ur, vb); glVertex3fv(&v4[0]);
! 			}
! 			else	// Particle
! 			{
! 				glVertex3fv(&v1[0]);
! 				glVertex3fv(&v2[0]);
! 				glVertex3fv(&v3[0]);
! 				glVertex3fv(&v4[0]);
! 			}
! 			glEnd();
! 
  		}
  	}
  	else
  	{
! 		gl_RenderModel(this, Colormap.colormap);
  	}
  
  	if (pass==GLPASS_TRANSLUCENT)
***************
*** 294,302 ****
  		gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  		gl_RenderState.BlendEquation(GL_FUNC_ADD);
  		gl_RenderState.SetTextureMode(TM_MODULATE);
  	}
  
! 	gl_RenderState.SetObjectColor(0xffffffff);
  	gl_RenderState.EnableTexture(true);
  	gl_RenderState.SetDynLight(0,0,0);
  }
--- 352,373 ----
  		gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  		gl_RenderState.BlendEquation(GL_FUNC_ADD);
  		gl_RenderState.SetTextureMode(TM_MODULATE);
+ 
+ 		// [BB] Restore the alpha test after drawing a smooth particle.
+ 		if (hw_styleflags == STYLEHW_NoAlphaTest)
+ 		{
+ 			gl_RenderState.EnableAlphaTest(true);
+ 		}
+ 		else
+ 		{
+ 			gl_RenderState.AlphaFunc(GL_GEQUAL,gl_mask_sprite_threshold);
+ 		}
  	}
  
! 	// End of gl_sprite_brightfog hack: restore FadeColor to normalcy
! 	if (backupfade != Colormap.FadeColor.d)
! 		Colormap.FadeColor = backupfade;
! 
  	gl_RenderState.EnableTexture(true);
  	gl_RenderState.SetDynLight(0,0,0);
  }
***************
*** 315,323 ****
  	{
  		list = GLDL_TRANSLUCENT;
  	}
  	else
  	{
! 		list = GLDL_MODELS;
  	}
  	gl_drawinfo->drawlists[list].AddSprite(this);
  }
--- 386,398 ----
  	{
  		list = GLDL_TRANSLUCENT;
  	}
+ 	else if ((!gl_isBlack (Colormap.FadeColor) || level.flags&LEVEL_HASFADETABLE))
+ 	{
+ 		list = GLDL_FOGMASKED;
+ 	}
  	else
  	{
! 		list = GLDL_MASKED;
  	}
  	gl_drawinfo->drawlists[list].AddSprite(this);
  }
***************
*** 359,364 ****
--- 434,446 ----
  				copySprite.Colormap.LightColor.b=(255+v+v)/3;
  			}
  
+ 			if (!gl_isWhite(ThingColor))
+ 			{
+ 				copySprite.Colormap.LightColor.r=(copySprite.Colormap.LightColor.r*ThingColor.r)>>8;
+ 				copySprite.Colormap.LightColor.g=(copySprite.Colormap.LightColor.g*ThingColor.g)>>8;
+ 				copySprite.Colormap.LightColor.b=(copySprite.Colormap.LightColor.b*ThingColor.b)>>8;
+ 			}
+ 
  			z1=copySprite.z2=maplightbottom;
  			vt=copySprite.vb=copySprite.vt+ 
  				(maplightbottom-copySprite.z1)*(copySprite.vb-copySprite.vt)/(z2-copySprite.z1);
***************
*** 398,503 ****
  				Colormap.LightColor.g=
  				Colormap.LightColor.b=(255+v+v)/3;
  			}
- 			return;
- 		}
- 	}
- }
- 
- //==========================================================================
- //
- // 
- //
- //==========================================================================
- 
- void GLSprite::PerformSpriteClipAdjustment(AActor *thing, fixed_t thingx, fixed_t thingy, float spriteheight)
- {
- 	bool smarterclip = false; // Set to true if one condition triggers the test below
- 	if (((thing->player || thing->flags3&MF3_ISMONSTER ||
- 		thing->IsKindOf(RUNTIME_CLASS(AInventory))) && (thing->flags&MF_ICECORPSE ||
- 		!(thing->flags&MF_CORPSE))) || (gl_spriteclip == 3 && (smarterclip = true)) || gl_spriteclip > 1)
- 	{
- 		float btm = 1000000.0f;
- 		float top = -1000000.0f;
- 		extsector_t::xfloor &x = thing->Sector->e->XFloor;
- 
- 		if (x.ffloors.Size())
- 		{
- 			for (unsigned int i = 0; i < x.ffloors.Size(); i++)
- 			{
- 				F3DFloor * ff = x.ffloors[i];
- 				fixed_t floorh = ff->top.plane->ZatPoint(thingx, thingy);
- 				fixed_t ceilingh = ff->bottom.plane->ZatPoint(thingx, thingy);
- 				if (floorh == thing->floorz)
- 				{
- 					btm = FIXED2FLOAT(floorh);
- 				}
- 				if (ceilingh == thing->ceilingz)
- 				{
- 					top = FIXED2FLOAT(ceilingh);
- 				}
- 				if (btm != 1000000.0f && top != -1000000.0f)
- 				{
- 					break;
- 				}
- 			}
- 		}
- 		else if (thing->Sector->heightsec && !(thing->Sector->heightsec->MoreFlags & SECF_IGNOREHEIGHTSEC))
- 		{
- 			if (thing->flags2&MF2_ONMOBJ && thing->floorz ==
- 				thing->Sector->heightsec->floorplane.ZatPoint(thingx, thingy))
- 			{
- 				btm = FIXED2FLOAT(thing->floorz);
- 				top = FIXED2FLOAT(thing->ceilingz);
- 			}
- 		}
- 		if (btm == 1000000.0f)
- 			btm = FIXED2FLOAT(thing->Sector->floorplane.ZatPoint(thingx, thingy) - thing->floorclip);
- 		if (top == -1000000.0f)
- 			top = FIXED2FLOAT(thing->Sector->ceilingplane.ZatPoint(thingx, thingy));
  
! 		// +/-1 to account for the one pixel empty frame around the sprite.
! 		float diffb = (z2+1) - btm;
! 		float difft = (z1-1) - top;
! 		if (diffb >= 0 /*|| !gl_sprite_clip_to_floor*/) diffb = 0;
! 		// Adjust sprites clipping into ceiling and adjust clipping adjustment for tall graphics
! 		if (smarterclip)
! 		{
! 			// Reduce slightly clipping adjustment of corpses
! 			if (thing->flags & MF_CORPSE || spriteheight > fabs(diffb))
  			{
! 				float ratio = clamp<float>((fabs(diffb) * (float)gl_sclipfactor / (spriteheight + 1)), 0.5, 1.0);
! 				diffb *= ratio;
! 			}
! 			if (!diffb)
! 			{
! 				if (difft <= 0) difft = 0;
! 				if (difft >= (float)gl_sclipthreshold)
! 				{
! 					// dumb copy of the above.
! 					if (!(thing->flags3&MF3_ISMONSTER) || (thing->flags&MF_NOGRAVITY) || (thing->flags&MF_CORPSE) || difft > (float)gl_sclipthreshold)
! 					{
! 						difft = 0;
! 					}
! 				}
! 				if (spriteheight > fabs(difft))
! 				{
! 					float ratio = clamp<float>((fabs(difft) * (float)gl_sclipfactor / (spriteheight + 1)), 0.5, 1.0);
! 					difft *= ratio;
! 				}
! 				z2 -= difft;
! 				z1 -= difft;
! 			}
! 		}
! 		if (diffb <= (0 - (float)gl_sclipthreshold))	// such a large displacement can't be correct! 
! 		{
! 			// for living monsters standing on the floor allow a little more.
! 			if (!(thing->flags3&MF3_ISMONSTER) || (thing->flags&MF_NOGRAVITY) || (thing->flags&MF_CORPSE) || diffb < (-1.8*(float)gl_sclipthreshold))
! 			{
! 				diffb = 0;
  			}
  		}
- 		z2 -= diffb;
- 		z1 -= diffb;
  	}
  }
  
--- 480,494 ----
  				Colormap.LightColor.g=
  				Colormap.LightColor.b=(255+v+v)/3;
  			}
  
! 			if (!gl_isWhite(ThingColor))
  			{
! 				Colormap.LightColor.r=(Colormap.LightColor.r*ThingColor.r)>>8;
! 				Colormap.LightColor.g=(Colormap.LightColor.g*ThingColor.g)>>8;
! 				Colormap.LightColor.b=(Colormap.LightColor.b*ThingColor.b)>>8;
  			}
+ 			return;
  		}
  	}
  }
  
***************
*** 599,663 ****
  		bool mirror;
  		FTextureID patch = gl_GetSpriteFrame(spritenum, thing->frame, -1, ang - thing->angle, &mirror);
  		if (!patch.isValid()) return;
! 		int type = thing->renderflags & RF_SPRITETYPEMASK;
! 		gltexture = FMaterial::ValidateTexture(patch, (type == RF_FACESPRITE), false);
  		if (!gltexture) return;
  
! 		vt = gltexture->GetSpriteVT();
! 		vb = gltexture->GetSpriteVB();
! 		gltexture->GetSpriteRect(&r);
! 		if (mirror)
! 		{
! 			r.left = -r.width - r.left;	// mirror the sprite's x-offset
! 			ul = gltexture->GetSpriteUL();
! 			ur = gltexture->GetSpriteUR();
  		}
  		else
  		{
! 			ul = gltexture->GetSpriteUR();
! 			ur = gltexture->GetSpriteUL();
  		}
  
! 		r.Scale(FIXED2FLOAT(spritescaleX), FIXED2FLOAT(spritescaleY));
! 
! 		float rightfac = -r.left;
! 		float leftfac = rightfac - r.width;
  
! 		z1 = z - r.top;
! 		z2 = z1 - r.height;
  
! 		float spriteheight = FIXED2FLOAT(spritescaleY) * r.height;
  
  		// Tests show that this doesn't look good for many decorations and corpses
! 		if (spriteheight > 0 && gl_spriteclip > 0 && (thing->renderflags & RF_SPRITETYPEMASK) == RF_FACESPRITE)
  		{
! 			PerformSpriteClipAdjustment(thing, thingx, thingy, spriteheight);
! 		}
  
! 		float viewvecX;
! 		float viewvecY;
! 		switch (thing->renderflags & RF_SPRITETYPEMASK)
! 		{
! 		case RF_FACESPRITE:
! 			viewvecX = GLRenderer->mViewVector.X;
! 			viewvecY = GLRenderer->mViewVector.Y;
! 
! 			x1 = x - viewvecY*leftfac;
! 			x2 = x - viewvecY*rightfac;
! 			y1 = y + viewvecX*leftfac;
! 			y2 = y + viewvecX*rightfac;
! 			break;
! 
! 		case RF_WALLSPRITE:
! 			viewvecX = FIXED2FLOAT(finecosine[thing->angle >> ANGLETOFINESHIFT]);
! 			viewvecY = FIXED2FLOAT(finesine[thing->angle >> ANGLETOFINESHIFT]);
! 
! 			x1 = x + viewvecY*leftfac;
! 			x2 = x + viewvecY*rightfac;
! 			y1 = y - viewvecX*leftfac;
! 			y2 = y - viewvecX*rightfac;
! 			break;
  		}
  	}
  	else 
  	{
--- 590,741 ----
  		bool mirror;
  		FTextureID patch = gl_GetSpriteFrame(spritenum, thing->frame, -1, ang - thing->angle, &mirror);
  		if (!patch.isValid()) return;
! 		gltexture=FMaterial::ValidateTexture(patch, false);
  		if (!gltexture) return;
  
! 		if (gl.flags & RFL_NPOT_TEXTURE)	// trimming only works if non-power-of-2 textures are supported
! 		{
! 			vt = gltexture->GetSpriteVT();
! 			vb = gltexture->GetSpriteVB();
! 			gltexture->GetRect(&r, GLUSE_SPRITE);
! 			if (mirror)
! 			{
! 				r.left=-r.width-r.left;	// mirror the sprite's x-offset
! 				ul = gltexture->GetSpriteUL();
! 				ur = gltexture->GetSpriteUR();
! 			}
! 			else
! 			{
! 				ul = gltexture->GetSpriteUR();
! 				ur = gltexture->GetSpriteUL();
! 			}
  		}
  		else
  		{
! 			vt = gltexture->GetVT();
! 			vb = gltexture->GetVB();
! 			gltexture->GetRect(&r, GLUSE_PATCH);
! 			if (mirror)
! 			{
! 				r.left=-r.width-r.left;	// mirror the sprite's x-offset
! 				ul = gltexture->GetUL();
! 				ur = gltexture->GetUR();
! 			}
! 			else
! 			{
! 				ul = gltexture->GetUR();
! 				ur = gltexture->GetUL();
! 			}
  		}
  
! 		r.Scale(FIXED2FLOAT(spritescaleX),FIXED2FLOAT(spritescaleY));
  
! 		float rightfac=-r.left;
! 		float leftfac=rightfac-r.width;
  
! 		z1=z-r.top;
! 		z2=z1-r.height;
  
+ 		float spriteheight = FIXED2FLOAT(spritescaleY) * gltexture->GetScaledHeightFloat(GLUSE_SPRITE);
+ 		
  		// Tests show that this doesn't look good for many decorations and corpses
! 		if (spriteheight>0 && gl_spriteclip>0)
  		{
! 			bool smarterclip = false; // Set to true if one condition triggers the test below
! 			if (((thing->player || thing->flags3&MF3_ISMONSTER ||
! 				thing->IsKindOf(RUNTIME_CLASS(AInventory)))	&& (thing->flags&MF_ICECORPSE ||
! 				!(thing->flags&MF_CORPSE))) || (gl_spriteclip==3 && (smarterclip = true)) || gl_spriteclip > 1)
! 			{
! 				float btm= 1000000.0f;
! 				float top=-1000000.0f;
! 				extsector_t::xfloor &x = thing->Sector->e->XFloor;
  
! 				if (x.ffloors.Size())
! 				{
! 					for(unsigned int i=0;i<x.ffloors.Size();i++)
! 					{
! 						F3DFloor * ff=x.ffloors[i];
! 						fixed_t floorh=ff->top.plane->ZatPoint(thingx, thingy);
! 						fixed_t ceilingh=ff->bottom.plane->ZatPoint(thingx, thingy);
! 						if (floorh==thing->floorz) 
! 						{
! 							btm=FIXED2FLOAT(floorh);
! 						}
! 						if (ceilingh==thing->ceilingz) 
! 						{
! 							top=FIXED2FLOAT(ceilingh);
! 						}
! 						if (btm != 1000000.0f && top != -1000000.0f)
! 						{
! 							break;
! 						}
! 					}
! 				}
! 				else if (thing->Sector->heightsec && !(thing->Sector->heightsec->MoreFlags & SECF_IGNOREHEIGHTSEC))
! 				{
! 					if (thing->flags2&MF2_ONMOBJ && thing->floorz==
! 						thing->Sector->heightsec->floorplane.ZatPoint(thingx, thingy))
! 					{
! 						btm=FIXED2FLOAT(thing->floorz);
! 						top=FIXED2FLOAT(thing->ceilingz);
! 					}
! 				}
! 				if (btm==1000000.0f) 
! 					btm= FIXED2FLOAT(thing->Sector->floorplane.ZatPoint(thingx, thingy)-thing->floorclip);
! 				if (top==-1000000.0f)
! 					top= FIXED2FLOAT(thing->Sector->ceilingplane.ZatPoint(thingx, thingy));
! 
! 				float diffb = z2 - btm;
! 				float difft = z1 - top;
! 				if (diffb >= 0 /*|| !gl_sprite_clip_to_floor*/) diffb = 0;
! 				// Adjust sprites clipping into ceiling and adjust clipping adjustment for tall graphics
! 				if (smarterclip)
! 				{
! 					// Reduce slightly clipping adjustment of corpses
! 					if (thing->flags & MF_CORPSE || spriteheight > abs(diffb))
! 					{
! 						float ratio = clamp<float>((abs(diffb) * (float)gl_sclipfactor/(spriteheight+1)), 0.5, 1.0);
! 						diffb*=ratio;
! 					}
! 					if (!diffb)
! 					{
! 						if (difft <= 0) difft = 0;
! 						if (difft >= (float)gl_sclipthreshold) 
! 						{
! 							// dumb copy of the above.
! 							if (!(thing->flags3&MF3_ISMONSTER) || (thing->flags&MF_NOGRAVITY) || (thing->flags&MF_CORPSE) || difft > (float)gl_sclipthreshold)
! 							{
! 								difft=0;
! 							}
! 						}
! 						if (spriteheight > abs(difft))
! 						{
! 							float ratio = clamp<float>((abs(difft) * (float)gl_sclipfactor/(spriteheight+1)), 0.5, 1.0);
! 							difft*=ratio;
! 						}
! 						z2-=difft;
! 						z1-=difft;
! 					}
! 				}
! 				if (diffb <= (0 - (float)gl_sclipthreshold))	// such a large displacement can't be correct! 
! 				{
! 					// for living monsters standing on the floor allow a little more.
! 					if (!(thing->flags3&MF3_ISMONSTER) || (thing->flags&MF_NOGRAVITY) || (thing->flags&MF_CORPSE) || diffb<(-1.8*(float)gl_sclipthreshold))
! 					{
! 						diffb=0;
! 					}
! 				}
! 				z2-=diffb;
! 				z1-=diffb;
! 			}
  		}
+ 		float viewvecX = GLRenderer->mViewVector.X;
+ 		float viewvecY = GLRenderer->mViewVector.Y;
+ 
+ 		x1=x-viewvecY*leftfac;
+ 		x2=x-viewvecY*rightfac;
+ 		y1=y+viewvecX*leftfac;
+ 		y2=y+viewvecX*rightfac;		
  	}
  	else 
  	{
***************
*** 676,682 ****
  	// allow disabling of the fullbright flag by a brightmap definition
  	// (e.g. to do the gun flashes of Doom's zombies correctly.
  	fullbright = (thing->flags5 & MF5_BRIGHT) ||
! 		((thing->renderflags & RF_FULLBRIGHT) && (!gltexture || !gltexture->tex->gl_info.bBrightmapDisablesFullbright));
  
  	lightlevel=fullbright? 255 : 
  		gl_ClampLight(rendersector->GetTexture(sector_t::ceiling) == skyflatnum ? 
--- 754,760 ----
  	// allow disabling of the fullbright flag by a brightmap definition
  	// (e.g. to do the gun flashes of Doom's zombies correctly.
  	fullbright = (thing->flags5 & MF5_BRIGHT) ||
! 		((thing->renderflags & RF_FULLBRIGHT) && (!gl_BrightmapsActive() || !gltexture || !gltexture->tex->gl_info.bBrightmapDisablesFullbright));
  
  	lightlevel=fullbright? 255 : 
  		gl_ClampLight(rendersector->GetTexture(sector_t::ceiling) == skyflatnum ? 
***************
*** 685,694 ****
  
  	lightlevel = (byte)gl_CheckSpriteGlow(rendersector, lightlevel, thingx, thingy, thingz);
  
- 	ThingColor = (thing->RenderStyle.Flags & STYLEF_ColorIsFixed) ? thing->fillcolor : 0xffffff;
- 	ThingColor.a = 255;
- 	RenderStyle = thing->RenderStyle;
- 
  	// colormap stuff is a little more complicated here...
  	if (gl_fixedcolormap) 
  	{
--- 763,768 ----
***************
*** 697,710 ****
  			|| (gl_enhanced_nv_stealth == 3))								// Any fixed colormap
  			enhancedvision=true;
  
! 		Colormap.Clear();
  
  		if (gl_fixedcolormap==CM_LITE)
  		{
  			if (gl_enhanced_nightvision &&
  				(thing->IsKindOf(RUNTIME_CLASS(AInventory)) || thing->flags3&MF3_ISMONSTER || thing->flags&MF_MISSILE || thing->flags&MF_CORPSE))
  			{
! 				RenderStyle.Flags |= STYLEF_InvertSource;
  			}
  		}
  	}
--- 771,784 ----
  			|| (gl_enhanced_nv_stealth == 3))								// Any fixed colormap
  			enhancedvision=true;
  
! 		Colormap.GetFixedColormap();
  
  		if (gl_fixedcolormap==CM_LITE)
  		{
  			if (gl_enhanced_nightvision &&
  				(thing->IsKindOf(RUNTIME_CLASS(AInventory)) || thing->flags3&MF3_ISMONSTER || thing->flags&MF_MISSILE || thing->flags&MF_CORPSE))
  			{
! 				Colormap.colormap = CM_FIRSTSPECIALCOLORMAP + INVERSECOLORMAP;
  			}
  		}
  	}
***************
*** 730,742 ****
  		}
  		else if (glset.nocoloredspritelighting)
  		{
! 			Colormap.Decolorize();
  		}
  	}
  
  	translation=thing->Translation;
  
! 	OverrideShader = -1;
  	trans = FIXED2FLOAT(thing->alpha);
  	hw_styleflags = STYLEHW_Normal;
  
--- 804,821 ----
  		}
  		else if (glset.nocoloredspritelighting)
  		{
! 			int v = (Colormap.LightColor.r /* * 77 */ + Colormap.LightColor.g /**143 */ + Colormap.LightColor.b /**35*/)/3;//255;
! 			Colormap.LightColor.r=
! 			Colormap.LightColor.g=
! 			Colormap.LightColor.b=(255+v+v)/3;
  		}
  	}
  
  	translation=thing->Translation;
  
! 	ThingColor=0xffffff;
! 	RenderStyle = thing->RenderStyle;
! 	OverrideShader = 0;
  	trans = FIXED2FLOAT(thing->alpha);
  	hw_styleflags = STYLEHW_Normal;
  
***************
*** 745,751 ****
  		RenderStyle.CheckFuzz();
  		if (RenderStyle.BlendOp == STYLEOP_Fuzz)
  		{
! 			if (gl_fuzztype != 0)
  			{
  				// Todo: implement shader selection here
  				RenderStyle = LegacyRenderStyles[STYLE_Translucent];
--- 824,830 ----
  		RenderStyle.CheckFuzz();
  		if (RenderStyle.BlendOp == STYLEOP_Fuzz)
  		{
! 			if (gl.shadermodel >= 4 && gl_fuzztype != 0)
  			{
  				// Todo: implement shader selection here
  				RenderStyle = LegacyRenderStyles[STYLE_Translucent];
***************
*** 808,816 ****
  		{
  			// enhanced vision makes them more visible!
  			trans=0.5f;
- 			FRenderStyle rs = RenderStyle;
  			RenderStyle = STYLE_Translucent;
- 			RenderStyle.Flags = rs.Flags;	// Flags must be preserved, at this point it can only be STYLEF_InvertSource
  		}
  		else if (thing->flags & MF_STEALTH)	
  		{
--- 887,893 ----
***************
*** 828,834 ****
  	particle=NULL;
  	
  	const bool drawWithXYBillboard = ( !(actor->renderflags & RF_FORCEYBILLBOARD)
- 									   && (actor->renderflags & RF_SPRITETYPEMASK) == RF_FACESPRITE
  									   && players[consoleplayer].camera
  									   && (gl_billboard_mode == 1 || actor->renderflags & RF_FORCEXYBILLBOARD ) );
  
--- 905,910 ----
***************
*** 874,880 ****
  
  	if (gl_fixedcolormap) 
  	{
! 		Colormap.Clear();
  	}
  	else if (!particle->bright)
  	{
--- 950,956 ----
  
  	if (gl_fixedcolormap) 
  	{
! 		Colormap.GetFixedColormap();
  	}
  	else if (!particle->bright)
  	{
***************
*** 907,913 ****
  	OverrideShader = 0;
  
  	ThingColor = particle->color;
! 	ThingColor.a = 255;
  
  	modelframe=NULL;
  	gltexture=NULL;
--- 983,990 ----
  	OverrideShader = 0;
  
  	ThingColor = particle->color;
! 	gl_ModifyColor(ThingColor.r, ThingColor.g, ThingColor.b, Colormap.colormap);
! 	ThingColor.a=0;
  
  	modelframe=NULL;
  	gltexture=NULL;
***************
*** 927,933 ****
  
  		if (lump != NULL)
  		{
! 			gltexture = FMaterial::ValidateTexture(lump, true);
  			translation = 0;
  
  			ul = gltexture->GetUL();
--- 1004,1010 ----
  
  		if (lump != NULL)
  		{
! 			gltexture=FMaterial::ValidateTexture(lump);
  			translation = 0;
  
  			ul = gltexture->GetUL();
***************
*** 935,941 ****
  			vt = gltexture->GetVT();
  			vb = gltexture->GetVB();
  			FloatRect r;
! 			gltexture->GetSpriteRect(&r);
  		}
  	}
  
--- 1012,1018 ----
  			vt = gltexture->GetVT();
  			vb = gltexture->GetVB();
  			FloatRect r;
! 			gltexture->GetRect(&r, GLUSE_PATCH);
  		}
  	}
  
