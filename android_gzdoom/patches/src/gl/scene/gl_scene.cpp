*** doom/src/main/jni/gzdoom/src/gl/scene/gl_scene.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_scene.cpp	2017-06-18 23:34:13.340581957 -0400
***************
*** 85,91 ****
  CVAR(Bool, gl_no_skyclear, false, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  CVAR(Float, gl_mask_threshold, 0.5f,CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  CVAR(Float, gl_mask_sprite_threshold, 0.5f,CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
! CVAR(Bool, gl_sort_textures, false, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  
  EXTERN_CVAR (Int, screenblocks)
  EXTERN_CVAR (Bool, cl_capfps)
--- 85,91 ----
  CVAR(Bool, gl_no_skyclear, false, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  CVAR(Float, gl_mask_threshold, 0.5f,CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  CVAR(Float, gl_mask_sprite_threshold, 0.5f,CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
! CVAR(Bool, gl_forcemultipass, false, 0)
  
  EXTERN_CVAR (Int, screenblocks)
  EXTERN_CVAR (Bool, cl_capfps)
***************
*** 249,257 ****
  
  void FGLRenderer::SetProjection(float fov, float ratio, float fovratio)
  {
  
  	float fovy = 2 * RAD2DEG(atan(tan(DEG2RAD(fov) / 2) / fovratio));
! 	gl_RenderState.mProjectionMatrix.perspective(fovy, ratio, 5.f, 65536.f);
  	gl_RenderState.Set2DMode(false);
  }
  
--- 249,259 ----
  
  void FGLRenderer::SetProjection(float fov, float ratio, float fovratio)
  {
+ 	glMatrixMode(GL_PROJECTION);
+ 	glLoadIdentity();
  
  	float fovy = 2 * RAD2DEG(atan(tan(DEG2RAD(fov) / 2) / fovratio));
! 	gluPerspective(fovy, ratio, 5.f, 65536.f);
  	gl_RenderState.Set2DMode(false);
  }
  
***************
*** 263,277 ****
  
  void FGLRenderer::SetViewMatrix(bool mirror, bool planemirror)
  {
  	float mult = mirror? -1:1;
  	float planemult = planemirror? -1:1;
  
! 	gl_RenderState.mViewMatrix.loadIdentity();
! 	gl_RenderState.mViewMatrix.rotate(GLRenderer->mAngles.Roll,  0.0f, 0.0f, 1.0f);
! 	gl_RenderState.mViewMatrix.rotate(GLRenderer->mAngles.Pitch, 1.0f, 0.0f, 0.0f);
! 	gl_RenderState.mViewMatrix.rotate(GLRenderer->mAngles.Yaw,   0.0f, mult, 0.0f);
! 	gl_RenderState.mViewMatrix.translate( GLRenderer->mCameraPos.X * mult, -GLRenderer->mCameraPos.Z*planemult, -GLRenderer->mCameraPos.Y);
! 	gl_RenderState.mViewMatrix.scale(-mult, planemult, 1);
  }
  
  
--- 265,291 ----
  
  void FGLRenderer::SetViewMatrix(bool mirror, bool planemirror)
  {
+ 	if (gl.shadermodel >= 4)
+ 	{
+ 		glActiveTexture(GL_TEXTURE7);
+ 		glMatrixMode(GL_TEXTURE);
+ 		glLoadIdentity();
+ 	}
+ 	glActiveTexture(GL_TEXTURE0);
+ 	glMatrixMode(GL_TEXTURE);
+ 	glLoadIdentity();
+ 
+ 	glMatrixMode(GL_MODELVIEW);
+ 	glLoadIdentity();
+ 
  	float mult = mirror? -1:1;
  	float planemult = planemirror? -1:1;
  
! 	glRotatef(GLRenderer->mAngles.Roll,  0.0f, 0.0f, 1.0f);
! 	glRotatef(GLRenderer->mAngles.Pitch, 1.0f, 0.0f, 0.0f);
! 	glRotatef(GLRenderer->mAngles.Yaw,   0.0f, mult, 0.0f);
! 	glTranslatef( GLRenderer->mCameraPos.X * mult, -GLRenderer->mCameraPos.Z*planemult, -GLRenderer->mCameraPos.Y);
! 	glScalef(-mult, planemult, 1);
  }
  
  
***************
*** 285,291 ****
  {
  	SetCameraPos(viewx, viewy, viewz, viewangle);
  	SetViewMatrix(mirror, planemirror);
- 	gl_RenderState.ApplyMatrices();
  }
  
  //-----------------------------------------------------------------------------
--- 299,304 ----
***************
*** 317,322 ****
--- 330,336 ----
  	gl_drawinfo->HandleHackedSubsectors();	// open sector hacks for deep water
  	gl_drawinfo->ProcessSectorStacks();		// merge visplanes of sector stacks
  
+ 	GLRenderer->mVBO->UnmapVBO ();
  	ProcessAll.Unclock();
  
  }
***************
*** 341,424 ****
  	gl_RenderState.EnableFog(true);
  	gl_RenderState.BlendFunc(GL_ONE,GL_ZERO);
  
! 	if (gl_sort_textures)
! 	{
! 		gl_drawinfo->drawlists[GLDL_PLAINWALLS].SortWalls();
! 		gl_drawinfo->drawlists[GLDL_PLAINFLATS].SortFlats();
! 		gl_drawinfo->drawlists[GLDL_MASKEDWALLS].SortWalls();
! 		gl_drawinfo->drawlists[GLDL_MASKEDFLATS].SortFlats();
! 		gl_drawinfo->drawlists[GLDL_MASKEDWALLSOFS].SortWalls();
! 	}
! 
! 	// if we don't have a persistently mapped buffer, we have to process all the dynamic lights up front,
! 	// so that we don't have to do repeated map/unmap calls on the buffer.
! 	if (mLightCount > 0 && gl_fixedcolormap == CM_DEFAULT && gl_lights && !(gl.flags & RFL_BUFFER_STORAGE))
! 	{
! 		GLRenderer->mLights->Begin();
! 		gl_drawinfo->drawlists[GLDL_PLAINWALLS].DrawWalls(GLPASS_LIGHTSONLY);
! 		gl_drawinfo->drawlists[GLDL_PLAINFLATS].DrawFlats(GLPASS_LIGHTSONLY);
! 		gl_drawinfo->drawlists[GLDL_MASKEDWALLS].DrawWalls(GLPASS_LIGHTSONLY);
! 		gl_drawinfo->drawlists[GLDL_MASKEDFLATS].DrawFlats(GLPASS_LIGHTSONLY);
! 		gl_drawinfo->drawlists[GLDL_MASKEDWALLSOFS].DrawWalls(GLPASS_LIGHTSONLY);
! 		gl_drawinfo->drawlists[GLDL_TRANSLUCENTBORDER].Draw(GLPASS_LIGHTSONLY);
! 		gl_drawinfo->drawlists[GLDL_TRANSLUCENT].Draw(GLPASS_LIGHTSONLY);
! 		GLRenderer->mLights->Finish();
! 	}
  
  	// Part 1: solid geometry. This is set up so that there are no transparent parts
  	glDepthFunc(GL_LESS);
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, 0.f);
! 	glDisable(GL_POLYGON_OFFSET_FILL);
  
  	int pass;
  
! 	if (mLightCount > 0 && gl_fixedcolormap == CM_DEFAULT && gl_lights && (gl.flags & RFL_BUFFER_STORAGE))
  	{
  		pass = GLPASS_ALL;
  	}
! 	else
  	{
  		pass = GLPASS_PLAIN;
  	}
  
  	gl_RenderState.EnableTexture(gl_texture);
! 	gl_RenderState.EnableBrightmap(true);
! 	gl_drawinfo->drawlists[GLDL_PLAINWALLS].DrawWalls(pass);
! 	gl_drawinfo->drawlists[GLDL_PLAINFLATS].DrawFlats(pass);
  
  
! 	// Part 2: masked geometry. This is set up so that only pixels with alpha>gl_mask_threshold will show
  	if (!gl_texture) 
  	{
  		gl_RenderState.EnableTexture(true);
  		gl_RenderState.SetTextureMode(TM_MASK);
  	}
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, gl_mask_threshold);
! 	gl_drawinfo->drawlists[GLDL_MASKEDWALLS].DrawWalls(pass);
! 	gl_drawinfo->drawlists[GLDL_MASKEDFLATS].DrawFlats(pass);
  
- 	// Part 3: masked geometry with polygon offset. This list is empty most of the time so only waste time on it when in use.
- 	if (gl_drawinfo->drawlists[GLDL_MASKEDWALLSOFS].Size() > 0)
- 	{
- 		glEnable(GL_POLYGON_OFFSET_FILL);
- 		glPolygonOffset(-1.0f, -128.0f);
- 		gl_drawinfo->drawlists[GLDL_MASKEDWALLSOFS].DrawWalls(pass);
- 		glDisable(GL_POLYGON_OFFSET_FILL);
- 		glPolygonOffset(0, 0);
- 	}
  
! 	gl_drawinfo->drawlists[GLDL_MODELS].Draw(pass);
  
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
! 	// Part 4: Draw decals (not a real pass)
  	glDepthFunc(GL_LEQUAL);
  	glEnable(GL_POLYGON_OFFSET_FILL);
  	glPolygonOffset(-1.0f, -128.0f);
  	glDepthMask(false);
  
! 	// this is the only geometry type on which decals can possibly appear
! 	gl_drawinfo->drawlists[GLDL_PLAINWALLS].DrawDecals();
  
  	gl_RenderState.SetTextureMode(TM_MODULATE);
  
--- 355,504 ----
  	gl_RenderState.EnableFog(true);
  	gl_RenderState.BlendFunc(GL_ONE,GL_ZERO);
  
! 	// First draw all single-pass stuff
  
  	// Part 1: solid geometry. This is set up so that there are no transparent parts
  	glDepthFunc(GL_LESS);
! 
! 
! 	gl_RenderState.EnableAlphaTest(false);
! 
! 	glDisable(GL_POLYGON_OFFSET_FILL);	// just in case
  
  	int pass;
  
! 	if (mLightCount > 0 && gl_fixedcolormap == CM_DEFAULT && gl_lights && gl_dynlight_shader)
  	{
  		pass = GLPASS_ALL;
  	}
! 	else if (gl_texture)
  	{
  		pass = GLPASS_PLAIN;
  	}
+ 	else
+ 	{
+ 		pass = GLPASS_BASE;
+ 	}
  
  	gl_RenderState.EnableTexture(gl_texture);
! 	gl_RenderState.EnableBrightmap(gl_fixedcolormap == CM_DEFAULT);
! 	gl_drawinfo->drawlists[GLDL_PLAIN].Sort();
! 	gl_drawinfo->drawlists[GLDL_PLAIN].Draw(pass);
! 	gl_RenderState.EnableBrightmap(false);
! 	gl_drawinfo->drawlists[GLDL_FOG].Sort();
! 	gl_drawinfo->drawlists[GLDL_FOG].Draw(pass);
! 	gl_drawinfo->drawlists[GLDL_LIGHTFOG].Sort();
! 	gl_drawinfo->drawlists[GLDL_LIGHTFOG].Draw(pass);
! 
  
+ 	gl_RenderState.EnableAlphaTest(true);
  
! 	// Part 2: masked geometry. This is set up so that only pixels with alpha>0.5 will show
  	if (!gl_texture) 
  	{
  		gl_RenderState.EnableTexture(true);
  		gl_RenderState.SetTextureMode(TM_MASK);
  	}
! 	if (pass == GLPASS_BASE) pass = GLPASS_BASE_MASKED;
! 	gl_RenderState.AlphaFunc(GL_GEQUAL,gl_mask_threshold);
! 	gl_RenderState.EnableBrightmap(true);
! 	gl_drawinfo->drawlists[GLDL_MASKED].Sort();
! 	gl_drawinfo->drawlists[GLDL_MASKED].Draw(pass);
! 	gl_RenderState.EnableBrightmap(false);
! 	gl_drawinfo->drawlists[GLDL_FOGMASKED].Sort();
! 	gl_drawinfo->drawlists[GLDL_FOGMASKED].Draw(pass);
! 	gl_drawinfo->drawlists[GLDL_LIGHTFOGMASKED].Sort();
! 	gl_drawinfo->drawlists[GLDL_LIGHTFOGMASKED].Draw(pass);
! 
! 	// And now the multipass stuff
! 	if (!gl_dynlight_shader && gl_lights)
! 	{
! 		// First pass: empty background with sector light only
! 
! 		// Part 1: solid geometry. This is set up so that there are no transparent parts
! 
! 		// remove any remaining texture bindings and shaders whick may get in the way.
! 		gl_RenderState.EnableTexture(false);
! 		gl_RenderState.EnableBrightmap(false);
! 		gl_RenderState.Apply();
! 		gl_drawinfo->drawlists[GLDL_LIGHT].Draw(GLPASS_BASE);
! 		gl_RenderState.EnableTexture(true);
! 
! 		// Part 2: masked geometry. This is set up so that only pixels with alpha>0.5 will show
! 		// This creates a blank surface that only fills the nontransparent parts of the texture
! 		gl_RenderState.SetTextureMode(TM_MASK);
! 		gl_RenderState.EnableBrightmap(true);
! 		gl_drawinfo->drawlists[GLDL_LIGHTBRIGHT].Draw(GLPASS_BASE_MASKED);
! 		gl_drawinfo->drawlists[GLDL_LIGHTMASKED].Draw(GLPASS_BASE_MASKED);
! 		gl_RenderState.EnableBrightmap(false);
! 		gl_RenderState.SetTextureMode(TM_MODULATE);
  
  
! 		// second pass: draw lights (on fogged surfaces they are added to the textures!)
! 		glDepthMask(false);
! 		if (mLightCount && !gl_fixedcolormap)
! 		{
! 			if (gl_SetupLightTexture())
! 			{
! 				gl_RenderState.BlendFunc(GL_ONE, GL_ONE);
! 				glDepthFunc(GL_EQUAL);
! 				if (glset.lightmode == 8) glVertexAttrib1f(VATTR_LIGHTLEVEL, 1.0f); // Korshun.
! 				for(int i=GLDL_FIRSTLIGHT; i<=GLDL_LASTLIGHT; i++)
! 				{
! 					gl_drawinfo->drawlists[i].Draw(GLPASS_LIGHT);
! 				}
! 				gl_RenderState.BlendEquation(GL_FUNC_ADD);
! 			}
! 			else gl_lights=false;
! 		}
! 
! 		// third pass: modulated texture
! 		glColor3f(1.0f, 1.0f, 1.0f);
! 		gl_RenderState.BlendFunc(GL_DST_COLOR, GL_ZERO);
! 		gl_RenderState.EnableFog(false);
! 		glDepthFunc(GL_LEQUAL);
! 		if (gl_texture) 
! 		{
! 			gl_RenderState.EnableAlphaTest(false);
! 			gl_drawinfo->drawlists[GLDL_LIGHT].Sort();
! 			gl_drawinfo->drawlists[GLDL_LIGHT].Draw(GLPASS_TEXTURE);
! 			gl_RenderState.EnableAlphaTest(true);
! 			gl_drawinfo->drawlists[GLDL_LIGHTBRIGHT].Sort();
! 			gl_drawinfo->drawlists[GLDL_LIGHTBRIGHT].Draw(GLPASS_TEXTURE);
! 			gl_drawinfo->drawlists[GLDL_LIGHTMASKED].Sort();
! 			gl_drawinfo->drawlists[GLDL_LIGHTMASKED].Draw(GLPASS_TEXTURE);
! 		}
! 
! 		// fourth pass: additive lights
! 		gl_RenderState.EnableFog(true);
! 		if (gl_lights && mLightCount && !gl_fixedcolormap)
! 		{
! 			gl_RenderState.BlendFunc(GL_ONE, GL_ONE);
! 			glDepthFunc(GL_EQUAL);
! 			if (gl_SetupLightTexture())
! 			{
! 				for(int i=0; i<GLDL_TRANSLUCENT; i++)
! 				{
! 					gl_drawinfo->drawlists[i].Draw(GLPASS_LIGHT_ADDITIVE);
! 				}
! 				gl_RenderState.BlendEquation(GL_FUNC_ADD);
! 			}
! 			else gl_lights=false;
! 		}
! 	}
  
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
! 	// Draw decals (not a real pass)
  	glDepthFunc(GL_LEQUAL);
  	glEnable(GL_POLYGON_OFFSET_FILL);
  	glPolygonOffset(-1.0f, -128.0f);
  	glDepthMask(false);
  
! 	for(int i=0; i<GLDL_TRANSLUCENT; i++)
! 	{
! 		gl_drawinfo->drawlists[i].Draw(GLPASS_DECALS);
! 	}
  
  	gl_RenderState.SetTextureMode(TM_MODULATE);
  
***************
*** 429,448 ****
  	// so they don't interfere with overlapping mid textures.
  	glPolygonOffset(1.0f, 128.0f);
  
! 	// Part 5: flood all the gaps with the back sector's flat texture
! 	// This will always be drawn like GLDL_PLAIN, depending on the fog settings
  	
  	glDepthMask(false);							// don't write to Z-buffer!
  	gl_RenderState.EnableFog(true);
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, 0.f);
  	gl_RenderState.BlendFunc(GL_ONE,GL_ZERO);
  	gl_drawinfo->DrawUnhandledMissingTextures();
  	glDepthMask(true);
  
  	glPolygonOffset(0.0f, 0.0f);
  	glDisable(GL_POLYGON_OFFSET_FILL);
- 	RenderAll.Unclock();
  
  }
  
  //-----------------------------------------------------------------------------
--- 509,529 ----
  	// so they don't interfere with overlapping mid textures.
  	glPolygonOffset(1.0f, 128.0f);
  
! 	// flood all the gaps with the back sector's flat texture
! 	// This will always be drawn like GLDL_PLAIN or GLDL_FOG, depending on the fog settings
  	
  	glDepthMask(false);							// don't write to Z-buffer!
  	gl_RenderState.EnableFog(true);
! 	gl_RenderState.EnableAlphaTest(false);
  	gl_RenderState.BlendFunc(GL_ONE,GL_ZERO);
  	gl_drawinfo->DrawUnhandledMissingTextures();
+ 	gl_RenderState.EnableAlphaTest(true);
  	glDepthMask(true);
  
  	glPolygonOffset(0.0f, 0.0f);
  	glDisable(GL_POLYGON_OFFSET_FILL);
  
+ 	RenderAll.Unclock();
  }
  
  //-----------------------------------------------------------------------------
***************
*** 461,467 ****
  	gl_RenderState.SetCameraPos(FIXED2FLOAT(viewx), FIXED2FLOAT(viewy), FIXED2FLOAT(viewz));
  
  	// final pass: translucent stuff
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, gl_mask_sprite_threshold);
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  	gl_RenderState.EnableBrightmap(true);
--- 542,549 ----
  	gl_RenderState.SetCameraPos(FIXED2FLOAT(viewx), FIXED2FLOAT(viewy), FIXED2FLOAT(viewz));
  
  	// final pass: translucent stuff
! 	gl_RenderState.EnableAlphaTest(true);
! 	gl_RenderState.AlphaFunc(GL_GEQUAL,gl_mask_sprite_threshold);
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  	gl_RenderState.EnableBrightmap(true);
***************
*** 471,477 ****
  
  	glDepthMask(true);
  
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, 0.5f);
  	RenderAll.Unclock();
  }
  
--- 553,559 ----
  
  	glDepthMask(true);
  
! 	gl_RenderState.AlphaFunc(GL_GEQUAL,0.5f);
  	RenderAll.Unclock();
  }
  
***************
*** 512,534 ****
  }
  
  
- static void FillScreen()
- {
- 	gl_RenderState.AlphaFunc(GL_GEQUAL, 0.f);
- 	gl_RenderState.EnableTexture(false);
- 	gl_RenderState.Apply();
- 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
- 	ptr->Set(0, 0, 0, 0, 0);
- 	ptr++;
- 	ptr->Set(0, (float)SCREENHEIGHT, 0, 0, 0);
- 	ptr++;
- 	ptr->Set((float)SCREENWIDTH, 0, 0, 0, 0);
- 	ptr++;
- 	ptr->Set((float)SCREENWIDTH, (float)SCREENHEIGHT, 0, 0, 0);
- 	ptr++;
- 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
- }
- 
  //==========================================================================
  //
  // Draws a blend over the entire view
--- 594,599 ----
***************
*** 555,566 ****
  		{
  			if (viewsector->heightsec && !(viewsector->MoreFlags&SECF_IGNOREHEIGHTSEC))
  			{
! 				switch (in_area)
  				{
  				default:
! 				case area_normal: blendv = viewsector->heightsec->midmap; break;
! 				case area_above: blendv = viewsector->heightsec->topmap; break;
! 				case area_below: blendv = viewsector->heightsec->bottommap; break;
  				}
  			}
  		}
--- 620,631 ----
  		{
  			if (viewsector->heightsec && !(viewsector->MoreFlags&SECF_IGNOREHEIGHTSEC))
  			{
! 				switch(in_area)
  				{
  				default:
! 				case area_normal: blendv=viewsector->heightsec->midmap; break;
! 				case area_above: blendv=viewsector->heightsec->topmap; break;
! 				case area_below: blendv=viewsector->heightsec->bottommap; break;
  				}
  			}
  		}
***************
*** 568,630 ****
  		{
  			TArray<lightlist_t> & lightlist = viewsector->e->XFloor.lightlist;
  
! 			for (unsigned int i = 0; i < lightlist.Size(); i++)
  			{
  				fixed_t lightbottom;
! 				if (i < lightlist.Size() - 1)
! 					lightbottom = lightlist[i + 1].plane.ZatPoint(viewx, viewy);
! 				else
! 					lightbottom = viewsector->floorplane.ZatPoint(viewx, viewy);
  
! 				if (lightbottom < viewz && (!lightlist[i].caster || !(lightlist[i].caster->flags&FF_FADEWALLS)))
  				{
  					// 3d floor 'fog' is rendered as a blending value
! 					blendv = lightlist[i].blend;
  					// If this is the same as the sector's it doesn't apply!
! 					if (blendv == viewsector->ColorMap->Fade) blendv = 0;
  					// a little hack to make this work for Legacy maps.
! 					if (blendv.a == 0 && blendv != 0) blendv.a = 128;
  					break;
  				}
  			}
  		}
  
! 		if (blendv.a == 0)
  		{
! 			blendv = R_BlendForColormap(blendv);
! 			if (blendv.a == 255)
! 			{
! 				// The calculated average is too dark so brighten it according to the palettes's overall brightness
! 				int maxcol = MAX<int>(MAX<int>(framebuffer->palette_brightness, blendv.r), MAX<int>(blendv.g, blendv.b));
! 				blendv.r = blendv.r * 255 / maxcol;
! 				blendv.g = blendv.g * 255 / maxcol;
! 				blendv.b = blendv.b * 255 / maxcol;
! 			}
  		}
  
! 		if (blendv.a == 255)
! 		{
  
! 			extra_red = blendv.r / 255.0f;
! 			extra_green = blendv.g / 255.0f;
! 			extra_blue = blendv.b / 255.0f;
  
! 			// If this is a multiplicative blend do it separately and add the additive ones on top of it.
! 			blendv = 0;
  
! 			// black multiplicative blends are ignored
! 			if (extra_red || extra_green || extra_blue)
! 			{
! 				gl_RenderState.BlendFunc(GL_DST_COLOR, GL_ZERO);
! 				gl_RenderState.SetColor(extra_red, extra_green, extra_blue, 1.0f);
! 				FillScreen();
! 			}
! 		}
! 		else if (blendv.a)
  		{
! 			V_AddBlend(blendv.r / 255.f, blendv.g / 255.f, blendv.b / 255.f, blendv.a / 255.0f, blend);
  		}
  	}
  
  	// This mostly duplicates the code in shared_sbar.cpp
  	// When I was writing this the original was called too late so that I
--- 633,703 ----
  		{
  			TArray<lightlist_t> & lightlist = viewsector->e->XFloor.lightlist;
  
! 			for(unsigned int i=0;i<lightlist.Size();i++)
  			{
  				fixed_t lightbottom;
! 				if (i<lightlist.Size()-1) 
! 					lightbottom=lightlist[i+1].plane.ZatPoint(viewx,viewy);
! 				else 
! 					lightbottom=viewsector->floorplane.ZatPoint(viewx,viewy);
  
! 				if (lightbottom<viewz && (!lightlist[i].caster || !(lightlist[i].caster->flags&FF_FADEWALLS)))
  				{
  					// 3d floor 'fog' is rendered as a blending value
! 					blendv=lightlist[i].blend;
  					// If this is the same as the sector's it doesn't apply!
! 					if (blendv == viewsector->ColorMap->Fade) blendv=0;
  					// a little hack to make this work for Legacy maps.
! 					if (blendv.a==0 && blendv!=0) blendv.a=128;
  					break;
  				}
  			}
  		}
+ 	}
  
! 	if (blendv.a==0)
! 	{
! 		blendv = R_BlendForColormap(blendv);
! 		if (blendv.a==255)
  		{
! 			// The calculated average is too dark so brighten it according to the palettes's overall brightness
! 			int maxcol = MAX<int>(MAX<int>(framebuffer->palette_brightness, blendv.r), MAX<int>(blendv.g, blendv.b));
! 			blendv.r = blendv.r * 255 / maxcol;
! 			blendv.g = blendv.g * 255 / maxcol;
! 			blendv.b = blendv.b * 255 / maxcol;
  		}
+ 	}
  
! 	if (blendv.a==255)
! 	{
  
! 		extra_red = blendv.r / 255.0f;
! 		extra_green = blendv.g / 255.0f;
! 		extra_blue = blendv.b / 255.0f;
  
! 		// If this is a multiplicative blend do it separately and add the additive ones on top of it!
! 		blendv=0;
  
! 		// black multiplicative blends are ignored
! 		if (extra_red || extra_green || extra_blue)
  		{
! 			gl_RenderState.EnableAlphaTest(false);
! 			gl_RenderState.EnableTexture(false);
! 			gl_RenderState.BlendFunc(GL_DST_COLOR,GL_ZERO);
! 			glColor4f(extra_red, extra_green, extra_blue, 1.0f);
! 			gl_RenderState.Apply(true);
! 			glBegin(GL_TRIANGLE_STRIP);
! 			glVertex2f( 0.0f, 0.0f);
! 			glVertex2f( 0.0f, (float)SCREENHEIGHT);
! 			glVertex2f( (float)SCREENWIDTH, 0.0f);
! 			glVertex2f( (float)SCREENWIDTH, (float)SCREENHEIGHT);
! 			glEnd();
  		}
  	}
+ 	else if (blendv.a)
+ 	{
+ 		V_AddBlend (blendv.r / 255.f, blendv.g / 255.f, blendv.b / 255.f, blendv.a/255.0f,blend);
+ 	}
  
  	// This mostly duplicates the code in shared_sbar.cpp
  	// When I was writing this the original was called too late so that I
***************
*** 646,653 ****
  	if (blend[3]>0.0f)
  	{
  		gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
! 		gl_RenderState.SetColor(blend[0], blend[1], blend[2], blend[3]);
! 		FillScreen();
  	}
  }
  
--- 719,734 ----
  	if (blend[3]>0.0f)
  	{
  		gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
! 		gl_RenderState.EnableAlphaTest(false);
! 		gl_RenderState.EnableTexture(false);
! 		glColor4fv(blend);
! 		gl_RenderState.Apply(true);
! 		glBegin(GL_TRIANGLE_STRIP);
! 		glVertex2f( 0.0f, 0.0f);
! 		glVertex2f( 0.0f, (float)SCREENHEIGHT);
! 		glVertex2f( (float)SCREENWIDTH, 0.0f);
! 		glVertex2f( (float)SCREENWIDTH, (float)SCREENHEIGHT);
! 		glEnd();
  	}
  }
  
***************
*** 661,668 ****
  
  void FGLRenderer::EndDrawScene(sector_t * viewsector)
  {
- 	gl_RenderState.EnableFog(false);
- 
  	// [BB] HUD models need to be rendered here. Make sure that
  	// DrawPlayerSprites is only called once. Either to draw
  	// HUD models or to draw the weapon sprites.
--- 742,747 ----
***************
*** 675,681 ****
--- 754,762 ----
  	}
  
  	glDisable(GL_STENCIL_TEST);
+ 	glDisable(GL_POLYGON_SMOOTH);
  
+ 	gl_RenderState.EnableFog(false);
  	framebuffer->Begin2D(false);
  
  	ResetViewport();
***************
*** 684,698 ****
  	{
  		DrawPlayerSprites (viewsector, false);
  	}
- 	gl_RenderState.SetFixedColormap(CM_DEFAULT);
- 	gl_RenderState.SetSoftLightLevel(-1);
  	DrawTargeterSprites();
  	DrawBlend(viewsector);
  
  	// Restore standard rendering state
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
! 	gl_RenderState.ResetColor();
  	gl_RenderState.EnableTexture(true);
  	glDisable(GL_SCISSOR_TEST);
  }
  
--- 765,778 ----
  	{
  		DrawPlayerSprites (viewsector, false);
  	}
  	DrawTargeterSprites();
  	DrawBlend(viewsector);
  
  	// Restore standard rendering state
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
! 	glColor3f(1.0f,1.0f,1.0f);
  	gl_RenderState.EnableTexture(true);
+ 	gl_RenderState.EnableAlphaTest(true);
  	glDisable(GL_SCISSOR_TEST);
  }
  
***************
*** 758,764 ****
  			}
  		}
  	}
- 	gl_RenderState.SetFixedColormap(gl_fixedcolormap);
  }
  
  //-----------------------------------------------------------------------------
--- 838,843 ----
***************
*** 797,804 ****
  	SetProjection(fov, ratio, fovratio);	// switch to perspective mode and set up clipper
  	SetCameraPos(viewx, viewy, viewz, viewangle);
  	SetViewMatrix(false, false);
- 	gl_RenderState.ApplyMatrices();
- 	GLRenderer->mLights->Clear();
  
  	clipper.Clear();
  	angle_t a1 = FrustumAngle();
--- 876,881 ----
***************
*** 823,829 ****
  	OpenGLFrameBuffer* GLTarget = static_cast<OpenGLFrameBuffer*>(screen);
  	AActor *&LastCamera = GLTarget->LastCamera;
  
- 	checkBenchActive();
  	if (player->camera != LastCamera)
  	{
  		// If the camera changed don't interpolate
--- 900,905 ----
***************
*** 832,839 ****
  		LastCamera=player->camera;
  	}
  
! 	gl_RenderState.SetVertexBuffer(mVBO);
! 	GLRenderer->mVBO->Reset();
  
  	// reset statistics counters
  	ResetProfilingData();
--- 908,914 ----
  		LastCamera=player->camera;
  	}
  
! 	mVBO->BindVBO();
  
  	// reset statistics counters
  	ResetProfilingData();
***************
*** 878,883 ****
--- 953,974 ----
  	All.Unclock();
  }
  
+ #ifdef __ANDROID__
+ byte * gles_convertRGB(byte * data, int width, int height)
+ {
+ 	//byte * temp = (byte *) Z_Malloc (width*height*3, TAG_TEMP_WORKSPACE, qfalse);
+ 	byte *src = data;
+ 	byte *dst = data;
+ 
+ 	for (int i=0; i<width*height; i++) {
+ 		for (int j=0; j<3; j++)
+ 			*(dst++) = *(src++);
+ 		src++;
+ 	}
+ 
+ 	return dst;
+ }
+ #endif
  //===========================================================================
  //
  // Render the view to a savegame picture
***************
*** 894,901 ****
  	bounds.height=height;
  	glFlush();
  	SetFixedColormap(player);
- 	gl_RenderState.SetVertexBuffer(mVBO);
- 	GLRenderer->mVBO->Reset();
  
  	// Check if there's some lights. If not some code can be skipped.
  	TThinkerIterator<ADynamicLight> it(STAT_DLIGHT);
--- 985,990 ----
***************
*** 904,918 ****
  	sector_t *viewsector = RenderViewpoint(players[consoleplayer].camera, &bounds, 
  								FieldOfView * 360.0f / FINEANGLES, 1.6f, 1.6f, true, false);
  	glDisable(GL_STENCIL_TEST);
- 	gl_RenderState.SetFixedColormap(CM_DEFAULT);
- 	gl_RenderState.SetSoftLightLevel(-1);
  	screen->Begin2D(false);
  	DrawBlend(viewsector);
  	glFlush();
  
  	byte * scr = (byte *)M_Malloc(width * height * 3);
  	glReadPixels(0,0,width, height,GL_RGB,GL_UNSIGNED_BYTE,scr);
  	M_CreatePNG (file, scr + ((height-1) * width * 3), NULL, SS_RGB, width, height, -width*3);
  	M_Free(scr);
  }
  
--- 993,1013 ----
  	sector_t *viewsector = RenderViewpoint(players[consoleplayer].camera, &bounds, 
  								FieldOfView * 360.0f / FINEANGLES, 1.6f, 1.6f, true, false);
  	glDisable(GL_STENCIL_TEST);
  	screen->Begin2D(false);
  	DrawBlend(viewsector);
  	glFlush();
  
+ #ifdef __ANDROID__ //Some androids do not like GL_RGB
+ 	byte * scr = (byte *)M_Malloc(width * height * 4);
+ 	glReadPixels(0,0,width, height,GL_RGBA,GL_UNSIGNED_BYTE,scr);
+ 	gles_convertRGB(scr,width,height);
+ 	M_CreatePNG (file, scr + ((height-1) * width * 3), NULL, SS_RGB, width, height, -width*3);
+ #else
  	byte * scr = (byte *)M_Malloc(width * height * 3);
  	glReadPixels(0,0,width, height,GL_RGB,GL_UNSIGNED_BYTE,scr);
  	M_CreatePNG (file, scr + ((height-1) * width * 3), NULL, SS_RGB, width, height, -width*3);
+ #endif
+ 
  	M_Free(scr);
  }
  
***************
*** 1063,1069 ****
  void FGLInterface::Init()
  {
  	gl_ParseDefs();
- 	gl_InitData();
  }
  
  //===========================================================================
--- 1158,1163 ----
***************
*** 1076,1090 ****
  
  void FGLInterface::RenderTextureView (FCanvasTexture *tex, AActor *Viewpoint, int FOV)
  {
! 	FMaterial * gltex = FMaterial::ValidateTexture(tex, false);
  
! 	int width = gltex->TextureWidth();
! 	int height = gltex->TextureHeight();
  
  	gl_fixedcolormap=CM_DEFAULT;
- 	gl_RenderState.SetFixedColormap(CM_DEFAULT);
  
! 	bool usefb = gl_usefb || width > screen->GetWidth() || height > screen->GetHeight();
  	if (!usefb)
  	{
  		glFlush();
--- 1170,1191 ----
  
  void FGLInterface::RenderTextureView (FCanvasTexture *tex, AActor *Viewpoint, int FOV)
  {
! 	FMaterial * gltex = FMaterial::ValidateTexture(tex);
  
! 	int width = gltex->TextureWidth(GLUSE_TEXTURE);
! 	int height = gltex->TextureHeight(GLUSE_TEXTURE);
  
  	gl_fixedcolormap=CM_DEFAULT;
  
! 	bool usefb;
! 
! 	if (gl.flags & RFL_FRAMEBUFFER)
! 	{
! 		usefb = gl_usefb || width > screen->GetWidth() || height > screen->GetHeight();
! 	}
! 	else usefb = false;
! 
! 
  	if (!usefb)
  	{
  		glFlush();
***************
*** 1111,1125 ****
  
  	GL_IRECT bounds;
  	bounds.left=bounds.top=0;
! 	bounds.width=FHardwareTexture::GetTexDimension(gltex->GetWidth());
! 	bounds.height=FHardwareTexture::GetTexDimension(gltex->GetHeight());
  
  	GLRenderer->RenderViewpoint(Viewpoint, &bounds, FOV, (float)width/height, (float)width/height, false, false);
  
  	if (!usefb)
  	{
  		glFlush();
! 		gl_RenderState.SetMaterial(gltex, 0, 0, -1, false);
  		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, bounds.width, bounds.height);
  	}
  	else
--- 1212,1226 ----
  
  	GL_IRECT bounds;
  	bounds.left=bounds.top=0;
! 	bounds.width=FHardwareTexture::GetTexDimension(gltex->GetWidth(GLUSE_TEXTURE));
! 	bounds.height=FHardwareTexture::GetTexDimension(gltex->GetHeight(GLUSE_TEXTURE));
  
  	GLRenderer->RenderViewpoint(Viewpoint, &bounds, FOV, (float)width/height, (float)width/height, false, false);
  
  	if (!usefb)
  	{
  		glFlush();
! 		gltex->Bind(CM_DEFAULT, 0, 0);
  		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, bounds.width, bounds.height);
  	}
  	else
***************
*** 1127,1132 ****
--- 1228,1235 ----
  		GLRenderer->EndOffscreen();
  	}
  
+ 	gltex->Bind(CM_DEFAULT, 0, 0);
+ 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TexFilter[gl_texture_filter].magfilter);
  	tex->SetUpdated();
  }
  
