*** doom/src/main/jni/gzdoom/src/gl/scene/gl_flats.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_flats.cpp	2017-06-18 23:34:13.293914859 -0400
***************
*** 70,87 ****
  // information
  //
  //==========================================================================
! static float tics;
! void gl_SetPlaneTextureRotation(const GLSectorPlane * secplane, FMaterial * gltexture)
  {
  	// only manipulate the texture matrix if needed.
  	if (secplane->xoffs != 0 || secplane->yoffs != 0 ||
  		secplane->xscale != FRACUNIT || secplane->yscale != FRACUNIT ||
  		secplane->angle != 0 || 
! 		gltexture->TextureWidth() != 64 ||
! 		gltexture->TextureHeight() != 64)
  	{
! 		float uoffs = FIXED2FLOAT(secplane->xoffs) / gltexture->TextureWidth();
! 		float voffs = FIXED2FLOAT(secplane->yoffs) / gltexture->TextureHeight();
  
  		float xscale1=FIXED2FLOAT(secplane->xscale);
  		float yscale1=FIXED2FLOAT(secplane->yscale);
--- 70,87 ----
  // information
  //
  //==========================================================================
! 
! bool gl_SetPlaneTextureRotation(const GLSectorPlane * secplane, FMaterial * gltexture)
  {
  	// only manipulate the texture matrix if needed.
  	if (secplane->xoffs != 0 || secplane->yoffs != 0 ||
  		secplane->xscale != FRACUNIT || secplane->yscale != FRACUNIT ||
  		secplane->angle != 0 || 
! 		gltexture->TextureWidth(GLUSE_TEXTURE) != 64 ||
! 		gltexture->TextureHeight(GLUSE_TEXTURE) != 64)
  	{
! 		float uoffs=FIXED2FLOAT(secplane->xoffs)/gltexture->TextureWidth(GLUSE_TEXTURE);
! 		float voffs=FIXED2FLOAT(secplane->yoffs)/gltexture->TextureHeight(GLUSE_TEXTURE);
  
  		float xscale1=FIXED2FLOAT(secplane->xscale);
  		float yscale1=FIXED2FLOAT(secplane->yscale);
***************
*** 91,134 ****
  		}
  		float angle=-ANGLE_TO_FLOAT(secplane->angle);
  
! 		float xscale2=64.f/gltexture->TextureWidth();
! 		float yscale2=64.f/gltexture->TextureHeight();
  
! 		gl_RenderState.mTextureMatrix.loadIdentity();
! 		gl_RenderState.mTextureMatrix.scale(xscale1 ,yscale1,1.0f);
! 		gl_RenderState.mTextureMatrix.translate(uoffs,voffs,0.0f);
! 		gl_RenderState.mTextureMatrix.scale(xscale2 ,yscale2,1.0f);
! 		gl_RenderState.mTextureMatrix.rotate(angle,0.0f,0.0f,1.0f);
! 		gl_RenderState.EnableTextureMatrix(true);
  	}
  }
  
  
- 
  //==========================================================================
  //
  // Flats 
  //
  //==========================================================================
- extern FDynLightData lightdata;
  
! void GLFlat::SetupSubsectorLights(int pass, subsector_t * sub, int *dli)
  {
  	Plane p;
  
! 	if (dli != NULL && *dli != -1)
! 	{
! 		gl_RenderState.ApplyLightIndex(GLRenderer->mLights->GetIndex(*dli));
! 		(*dli)++;
! 		return;
! 	}
! 
! 	lightdata.Clear();
! 	FLightNode * node = sub->lighthead;
  	while (node)
  	{
  		ADynamicLight * light = node->lightsource;
! 			
  		if (light->flags2&MF2_DORMANT)
  		{
  			node=node->nextLight;
--- 91,131 ----
  		}
  		float angle=-ANGLE_TO_FLOAT(secplane->angle);
  
! 		float xscale2=64.f/gltexture->TextureWidth(GLUSE_TEXTURE);
! 		float yscale2=64.f/gltexture->TextureHeight(GLUSE_TEXTURE);
  
! 		glMatrixMode(GL_TEXTURE);
! 		glPushMatrix();
! 		glScalef(xscale1 ,yscale1,1.0f);
! 		glTranslatef(uoffs,voffs,0.0f);
! 		glScalef(xscale2 ,yscale2,1.0f);
! 		glRotatef(angle,0.0f,0.0f,1.0f);
! 		return true;
  	}
+ 	return false;
  }
  
  
  //==========================================================================
  //
  // Flats 
  //
  //==========================================================================
  
! void GLFlat::DrawSubsectorLights(subsector_t * sub, int pass)
  {
  	Plane p;
+ 	Vector nearPt, up, right, t1;
+ 	float scale;
+ 	unsigned int k;
+ 	seg_t *v;
  
! 	FLightNode * node = sub->lighthead[pass==GLPASS_LIGHT_ADDITIVE];
! 	gl_RenderState.Apply();
  	while (node)
  	{
  		ADynamicLight * light = node->lightsource;
! 		
  		if (light->flags2&MF2_DORMANT)
  		{
  			node=node->nextLight;
***************
*** 146,285 ****
  		}
  
  		p.Set(plane.plane);
! 		gl_GetLight(p, light, false, false, lightdata);
! 		node = node->nextLight;
! 	}
  
! 	int d = GLRenderer->mLights->UploadLights(lightdata);
! 	if (pass == GLPASS_LIGHTSONLY)
! 	{
! 		GLRenderer->mLights->StoreIndex(d);
! 	}
! 	else
! 	{
! 		gl_RenderState.ApplyLightIndex(d);
  	}
  }
  
  //==========================================================================
  //
! //
  //
  //==========================================================================
  
! void GLFlat::DrawSubsector(subsector_t * sub)
  {
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 	if (plane.plane.a | plane.plane.b)
  	{
! 		for (unsigned int k = 0; k < sub->numlines; k++)
  		{
! 			vertex_t *vt = sub->firstline[k].v1;
! 			ptr->x = vt->fx;
! 			ptr->y = vt->fy;
! 			ptr->z = plane.plane.ZatPoint(vt->fx, vt->fy) + dz;
! 			ptr->u = vt->fx / 64.f;
! 			ptr->v = -vt->fy / 64.f;
! 			ptr++;
  		}
  	}
! 	else
  	{
! 		float zc = FIXED2FLOAT(plane.plane.Zat0()) + dz;
! 		for (unsigned int k = 0; k < sub->numlines; k++)
! 		{
! 			vertex_t *vt = sub->firstline[k].v1;
! 			ptr->x = vt->fx;
! 			ptr->y = vt->fy;
! 			ptr->z = zc;
! 			ptr->u = vt->fx / 64.f;
! 			ptr->v = -vt->fy / 64.f;
! 			ptr++;
! 		}
  	}
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_FAN);
! 
! 	flatvertices += sub->numlines;
! 	flatprimitives++;
  }
  
- 
  //==========================================================================
  //
  //
  //
  //==========================================================================
  
! void GLFlat::ProcessLights(bool istrans)
  {
! 	dynlightindex = GLRenderer->mLights->GetIndexPtr();
  
! 	if (sub)
  	{
! 		// This represents a single subsector
! 		SetupSubsectorLights(GLPASS_LIGHTSONLY, sub);
  	}
! 	else
! 	{
! 		// Draw the subsectors belonging to this sector
! 		for (int i=0; i<sector->subsectorcount; i++)
! 		{
! 			subsector_t * sub = sector->subsectors[i];
! 			if (gl_drawinfo->ss_renderflags[sub-subsectors]&renderflags || istrans)
! 			{
! 				SetupSubsectorLights(GLPASS_LIGHTSONLY, sub);
! 			}
! 		}
  
! 		// Draw the subsectors assigned to it due to missing textures
! 		if (!(renderflags&SSRF_RENDER3DPLANES))
! 		{
! 			gl_subsectorrendernode * node = (renderflags&SSRF_RENDERFLOOR)?
! 				gl_drawinfo->GetOtherFloorPlanes(sector->sectornum) :
! 				gl_drawinfo->GetOtherCeilingPlanes(sector->sectornum);
! 
! 			while (node)
! 			{
! 				SetupSubsectorLights(GLPASS_LIGHTSONLY, node->sub);
! 				node = node->next;
! 			}
! 		}
! 	}
  }
  
- 
  //==========================================================================
  //
  //
  //
  //==========================================================================
  
! void GLFlat::DrawSubsectors(int pass, bool processlights, bool istrans)
  {
! 	int dli = dynlightindex;
  
  	gl_RenderState.Apply();
  	if (sub)
  	{
  		// This represents a single subsector
! 		if (processlights) SetupSubsectorLights(GLPASS_ALL, sub, &dli);
  		DrawSubsector(sub);
  	}
  	else
  	{
  		if (vboindex >= 0)
  		{
  			int index = vboindex;
  			for (int i=0; i<sector->subsectorcount; i++)
  			{
  				subsector_t * sub = sector->subsectors[i];
! 				
  				if (gl_drawinfo->ss_renderflags[sub-subsectors]&renderflags || istrans)
  				{
! 					if (processlights) SetupSubsectorLights(GLPASS_ALL, sub, &dli);
! 					drawcalls.Clock();
  					glDrawArrays(GL_TRIANGLE_FAN, index, sub->numlines);
- 					drawcalls.Unclock();
  					flatvertices += sub->numlines;
  					flatprimitives++;
  				}
--- 143,289 ----
  		}
  
  		p.Set(plane.plane);
! 		if (!gl_SetupLight(p, light, nearPt, up, right, scale, Colormap.colormap, false, foggy)) 
! 		{
! 			node=node->nextLight;
! 			continue;
! 		}
! 		draw_dlightf++;
  
! 		// Render the light
! 		glBegin(GL_TRIANGLE_FAN);
! 		for(k = 0, v = sub->firstline; k < sub->numlines; k++, v++)
! 		{
! 			vertex_t *vt = v->v1;
! 			float zc = plane.plane.ZatPoint(vt->fx, vt->fy) + dz;
! 
! 			t1.Set(vt->fx, zc, vt->fy);
! 			Vector nearToVert = t1 - nearPt;
! 			glTexCoord2f( (nearToVert.Dot(right) * scale) + 0.5f, (nearToVert.Dot(up) * scale) + 0.5f);
! 
! 			glVertex3f(vt->fx, zc, vt->fy);
! 		}
! 
! 		glEnd();
! 		node = node->nextLight;
  	}
  }
  
+ 
  //==========================================================================
  //
! // Flats 
  //
  //==========================================================================
+ extern FDynLightData lightdata;
  
! bool GLFlat::SetupSubsectorLights(bool lightsapplied, subsector_t * sub)
  {
! 	Plane p;
! 
! 	lightdata.Clear();
! 	for(int i=0;i<2;i++)
  	{
! 		FLightNode * node = sub->lighthead[i];
! 		while (node)
  		{
! 			ADynamicLight * light = node->lightsource;
! 			
! 			if (light->flags2&MF2_DORMANT)
! 			{
! 				node=node->nextLight;
! 				continue;
! 			}
! 			iter_dlightf++;
! 
! 			// we must do the side check here because gl_SetupLight needs the correct plane orientation
! 			// which we don't have for Legacy-style 3D-floors
! 			fixed_t planeh = plane.plane.ZatPoint(light->x, light->y);
! 			if (gl_lights_checkside && ((planeh<light->z && ceiling) || (planeh>light->z && !ceiling)))
! 			{
! 				node=node->nextLight;
! 				continue;
! 			}
! 
! 			p.Set(plane.plane);
! 			gl_GetLight(p, light, Colormap.colormap, false, false, lightdata);
! 			node = node->nextLight;
  		}
  	}
! 
! 	int numlights[3];
! 
! 	lightdata.Combine(numlights, gl.MaxLights());
! 	if (numlights[2] > 0)
  	{
! 		draw_dlightf+=numlights[2]/2;
! 		gl_RenderState.EnableLight(true);
! 		gl_RenderState.SetLights(numlights, &lightdata.arrays[0][0]);
! 		gl_RenderState.Apply();
! 		return true;
  	}
! 	if (lightsapplied) 
! 	{
! 		gl_RenderState.EnableLight(false);
! 		gl_RenderState.Apply();
! 	}
! 	return false;
  }
  
  //==========================================================================
  //
  //
  //
  //==========================================================================
  
! void GLFlat::DrawSubsector(subsector_t * sub)
  {
! 	glBegin(GL_TRIANGLE_FAN);
  
! 	for(unsigned int k=0; k<sub->numlines; k++)
  	{
! 		vertex_t *vt = sub->firstline[k].v1;
! 		glTexCoord2f(vt->fx/64.f, -vt->fy/64.f);
! 		float zc = plane.plane.ZatPoint(vt->fx, vt->fy) + dz;
! 		glVertex3f(vt->fx, zc, vt->fy);
  	}
! 	glEnd();
  
! 	flatvertices += sub->numlines;
! 	flatprimitives++;
  }
  
  //==========================================================================
  //
  //
  //
  //==========================================================================
  
! void GLFlat::DrawSubsectors(int pass, bool istrans)
  {
! 	bool lightsapplied = false;
  
  	gl_RenderState.Apply();
  	if (sub)
  	{
  		// This represents a single subsector
! 		if (pass == GLPASS_ALL) lightsapplied = SetupSubsectorLights(lightsapplied, sub);
  		DrawSubsector(sub);
  	}
  	else
  	{
  		if (vboindex >= 0)
  		{
+ 			//glColor3f( 1.f,.5f,.5f);
  			int index = vboindex;
  			for (int i=0; i<sector->subsectorcount; i++)
  			{
  				subsector_t * sub = sector->subsectors[i];
! 				// This is just a quick hack to make translucent 3D floors and portals work.
  				if (gl_drawinfo->ss_renderflags[sub-subsectors]&renderflags || istrans)
  				{
! 					if (pass == GLPASS_ALL) lightsapplied = SetupSubsectorLights(lightsapplied, sub);
  					glDrawArrays(GL_TRIANGLE_FAN, index, sub->numlines);
  					flatvertices += sub->numlines;
  					flatprimitives++;
  				}
***************
*** 288,300 ****
  		}
  		else
  		{
  			// Draw the subsectors belonging to this sector
  			for (int i=0; i<sector->subsectorcount; i++)
  			{
  				subsector_t * sub = sector->subsectors[i];
  				if (gl_drawinfo->ss_renderflags[sub-subsectors]&renderflags || istrans)
  				{
! 					if (processlights) SetupSubsectorLights(GLPASS_ALL, sub, &dli);
  					DrawSubsector(sub);
  				}
  			}
--- 292,305 ----
  		}
  		else
  		{
+ 			//glColor3f( .5f,1.f,.5f); // these are for testing the VBO stuff.
  			// Draw the subsectors belonging to this sector
  			for (int i=0; i<sector->subsectorcount; i++)
  			{
  				subsector_t * sub = sector->subsectors[i];
  				if (gl_drawinfo->ss_renderflags[sub-subsectors]&renderflags || istrans)
  				{
! 					if (pass == GLPASS_ALL) lightsapplied = SetupSubsectorLights(lightsapplied, sub);
  					DrawSubsector(sub);
  				}
  			}
***************
*** 309,320 ****
  
  			while (node)
  			{
! 				if (processlights) SetupSubsectorLights(GLPASS_ALL, node->sub, &dli);
  				DrawSubsector(node->sub);
  				node = node->next;
  			}
  		}
  	}
  }
  
  
--- 314,326 ----
  
  			while (node)
  			{
! 				if (pass == GLPASS_ALL) lightsapplied = SetupSubsectorLights(lightsapplied, node->sub);
  				DrawSubsector(node->sub);
  				node = node->next;
  			}
  		}
  	}
+ 	gl_RenderState.EnableLight(false);
  }
  
  
***************
*** 323,334 ****
  //
  //
  //==========================================================================
! void GLFlat::Draw(int pass, bool trans)	// trans only has meaning for GLPASS_LIGHTSONLY
  {
  	int rel = getExtraLight();
  
  #ifdef _DEBUG
! 	if (sector->sectornum == 130)
  	{
  		int a = 0;
  	}
--- 329,341 ----
  //
  //
  //==========================================================================
! void GLFlat::Draw(int pass)
  {
+ 	int i;
  	int rel = getExtraLight();
  
  #ifdef _DEBUG
! 	if (sector->sectornum == 2)
  	{
  		int a = 0;
  	}
***************
*** 337,376 ****
  
  	switch (pass)
  	{
  	case GLPASS_PLAIN:			// Single-pass rendering
  	case GLPASS_ALL:
! 		gl_SetColor(lightlevel, rel, Colormap,1.0f);
! 		gl_SetFog(lightlevel, rel, &Colormap, false);
! 		gl_RenderState.SetMaterial(gltexture, CLAMP_NONE, 0, -1, false);
! 		gl_SetPlaneTextureRotation(&plane, gltexture);
! 		DrawSubsectors(pass, (pass == GLPASS_ALL || dynlightindex > -1), false);
! 		gl_RenderState.EnableTextureMatrix(false);
  		break;
  
! 	case GLPASS_LIGHTSONLY:
! 		if (!trans || gltexture)
  		{
! 			ProcessLights(trans);
  		}
  		break;
  
  	case GLPASS_TRANSLUCENT:
  		if (renderstyle==STYLE_Add) gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE);
! 		gl_SetColor(lightlevel, rel, Colormap, alpha);
  		gl_SetFog(lightlevel, rel, &Colormap, false);
! 		gl_RenderState.AlphaFunc(GL_GEQUAL, gl_mask_threshold);
  		if (!gltexture)	
  		{
  			gl_RenderState.EnableTexture(false);
! 			DrawSubsectors(pass, false, true);
  			gl_RenderState.EnableTexture(true);
  		}
  		else 
  		{
! 			gl_RenderState.SetMaterial(gltexture, CLAMP_NONE, 0, -1, false);
! 			gl_SetPlaneTextureRotation(&plane, gltexture);
! 			DrawSubsectors(pass, true, true);
! 			gl_RenderState.EnableTextureMatrix(false);
  		}
  		if (renderstyle==STYLE_Add) gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  		break;
--- 344,436 ----
  
  	switch (pass)
  	{
+ 	case GLPASS_BASE:
+ 		gl_SetColor(lightlevel, rel, &Colormap,1.0f);
+ 		if (!foggy) gl_SetFog(lightlevel, rel, &Colormap, false);
+ 		DrawSubsectors(pass, false);
+ 		break;
+ 
  	case GLPASS_PLAIN:			// Single-pass rendering
  	case GLPASS_ALL:
! 	case GLPASS_BASE_MASKED:
! 		gl_SetColor(lightlevel, rel, &Colormap,1.0f);
! 		if (!foggy || pass != GLPASS_BASE_MASKED) gl_SetFog(lightlevel, rel, &Colormap, false);
! 		// fall through
! 	case GLPASS_TEXTURE:
! 	{
! 		gltexture->Bind(Colormap.colormap);
! 		bool pushed = gl_SetPlaneTextureRotation(&plane, gltexture);
! 		DrawSubsectors(pass, false);
! 		if (pushed) 
! 		{
! 			glPopMatrix();
! 			glMatrixMode(GL_MODELVIEW);
! 		}
  		break;
+ 	}
+ 
+ 	case GLPASS_LIGHT:
+ 	case GLPASS_LIGHT_ADDITIVE:
  
! 		if (!foggy)	gl_SetFog((255+lightlevel)>>1, 0, &Colormap, false);
! 		else gl_SetFog(lightlevel, 0, &Colormap, true);	
! 
! 		if (sub)
  		{
! 			DrawSubsectorLights(sub, pass);
! 		}
! 		else
! 		{
! 			// Draw the subsectors belonging to this sector
! 			for (i=0; i<sector->subsectorcount; i++)
! 			{
! 				subsector_t * sub = sector->subsectors[i];
! 
! 				if (gl_drawinfo->ss_renderflags[sub-subsectors]&renderflags)
! 				{
! 					DrawSubsectorLights(sub, pass);
! 				}
! 			}
! 
! 			// Draw the subsectors assigned to it due to missing textures
! 			if (!(renderflags&SSRF_RENDER3DPLANES))
! 			{
! 				gl_subsectorrendernode * node = (renderflags&SSRF_RENDERFLOOR)?
! 					gl_drawinfo->GetOtherFloorPlanes(sector->sectornum) :
! 					gl_drawinfo->GetOtherCeilingPlanes(sector->sectornum);
! 
! 				while (node)
! 				{
! 					DrawSubsectorLights(node->sub, pass);
! 					node = node->next;
! 				}
! 			}
  		}
  		break;
  
  	case GLPASS_TRANSLUCENT:
  		if (renderstyle==STYLE_Add) gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE);
! 		gl_SetColor(lightlevel, rel, &Colormap, alpha);
  		gl_SetFog(lightlevel, rel, &Colormap, false);
! 		gl_RenderState.AlphaFunc(GL_GEQUAL,gl_mask_threshold*(alpha));
  		if (!gltexture)	
  		{
  			gl_RenderState.EnableTexture(false);
! 			DrawSubsectors(pass, true);
  			gl_RenderState.EnableTexture(true);
  		}
  		else 
  		{
! 			if (foggy) gl_RenderState.EnableBrightmap(false);
! 			gltexture->Bind(Colormap.colormap);
! 			bool pushed = gl_SetPlaneTextureRotation(&plane, gltexture);
! 			DrawSubsectors(pass, true);
! 			gl_RenderState.EnableBrightmap(true);
! 			if (pushed)
! 			{
! 				glPopMatrix();
! 				glMatrixMode(GL_MODELVIEW);
! 			}
  		}
  		if (renderstyle==STYLE_Add) gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  		break;
***************
*** 392,410 ****
  
  	if (gl_fixedcolormap) 
  	{
! 		Colormap.Clear();
  	}
! 	if (renderstyle!=STYLE_Translucent || alpha < 1.f - FLT_EPSILON || fog || gltexture == NULL)
  	{
! 		// translucent 3D floors go into the regular translucent list, translucent portals go into the translucent border list.
! 		list = (renderflags&SSRF_RENDER3DPLANES) ? GLDL_TRANSLUCENT : GLDL_TRANSLUCENTBORDER;
  	}
! 	else
  	{
  		bool masked = gltexture->isMasked() && ((renderflags&SSRF_RENDER3DPLANES) || stack);
! 		list = masked ? GLDL_MASKEDFLATS : GLDL_PLAINFLATS;
  	}
- 	gl_drawinfo->drawlists[list].AddFlat (this);
  }
  
  //==========================================================================
--- 452,493 ----
  
  	if (gl_fixedcolormap) 
  	{
! 		Colormap.GetFixedColormap();
  	}
! 	if (renderstyle!=STYLE_Translucent || alpha < 1.f - FLT_EPSILON || fog)
  	{
! 		int list = (renderflags&SSRF_RENDER3DPLANES) ? GLDL_TRANSLUCENT : GLDL_TRANSLUCENTBORDER;
! 		gl_drawinfo->drawlists[list].AddFlat (this);
  	}
! 	else if (gltexture != NULL)
  	{
+ 		static DrawListType list_indices[2][2][2]={
+ 			{ { GLDL_PLAIN, GLDL_FOG      }, { GLDL_MASKED,      GLDL_FOGMASKED      } },
+ 			{ { GLDL_LIGHT, GLDL_LIGHTFOG }, { GLDL_LIGHTMASKED, GLDL_LIGHTFOGMASKED } }
+ 		};
+ 
+ 		bool light = gl_forcemultipass;
  		bool masked = gltexture->isMasked() && ((renderflags&SSRF_RENDER3DPLANES) || stack);
! 
! 		if (!gl_fixedcolormap)
! 		{
! 			foggy = gl_CheckFog(&Colormap, lightlevel) || level.flags&LEVEL_HASFADETABLE;
! 
! 			if (gl_lights && !gl_dynlight_shader && GLRenderer->mLightCount)	// Are lights touching this sector?
! 			{
! 				for(int i=0;i<sector->subsectorcount;i++) if (sector->subsectors[i]->lighthead[0] != NULL)
! 				{
! 					light=true;
! 				}
! 			}
! 		}
! 		else foggy = false;
! 
! 		list = list_indices[light][masked][foggy];
! 		if (list == GLDL_LIGHT && gltexture->tex->gl_info.Brightmap && gl_BrightmapsActive()) list = GLDL_LIGHTBRIGHT;
! 
! 		gl_drawinfo->drawlists[list].AddFlat (this);
  	}
  }
  
  //==========================================================================
***************
*** 424,430 ****
  	{
  		if (plane.texture==skyflatnum) return;
  
! 		gltexture=FMaterial::ValidateTexture(plane.texture, false, true);
  		if (!gltexture) return;
  		if (gltexture->tex->isFullbright()) 
  		{
--- 507,513 ----
  	{
  		if (plane.texture==skyflatnum) return;
  
! 		gltexture=FMaterial::ValidateTexture(plane.texture, true);
  		if (!gltexture) return;
  		if (gltexture->tex->isFullbright()) 
  		{
***************
*** 500,506 ****
  	sector=&sectors[frontsector->sectornum];	
  	extsector_t::xfloor &x = sector->e->XFloor;
  	this->sub=NULL;
- 	dynlightindex = -1;
  
  	byte &srf = gl_drawinfo->sectorrenderflags[sector->sectornum];
  
--- 583,588 ----
