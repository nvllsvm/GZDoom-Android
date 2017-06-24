*** doom/src/main/jni/gzdoom/src/gl/models/gl_models_md3.cpp	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/models/gl_models_md3.cpp	2017-06-18 23:34:13.360582142 -0400
***************
*** 207,253 ****
  	return true;
  }
  
! void FMD3Model::BuildVertexBuffer(FModelVertexBuffer *buf)
  {
! 	for (int i = 0; i < numSurfaces; i++)
  	{
! 		MD3Surface * surf = &surfaces[i];
  
! 		surf->vindex = buf->vbo_shadowdata.Size();
! 		surf->iindex = buf->ibo_shadowdata.Size();
! 		for (int j = 0; j < numFrames * surf->numVertices; j++)
  		{
! 			MD3Vertex* vert = surf->vertices + j;
! 
! 			FModelVertex bvert;
  
! 			int tc = j % surf->numVertices;
! 			bvert.Set(vert->x, vert->z, vert->y, surf->texcoords[tc].s, surf->texcoords[tc].t);
! 			bvert.SetNormal(vert->nx, vert->nz, vert->ny);
! 			buf->vbo_shadowdata.Push(bvert);
! 		}
! 
! 		for (int k = 0; k < surf->numTriangles; k++)
! 		{
! 			for (int l = 0; l < 3; l++)
! 			{
! 				buf->ibo_shadowdata.Push(surf->tris[k].VertIndex[l]);
! 			}
  		}
  	}
  }
  
! 
! int FMD3Model::FindFrame(const char * name)
  {
! 	for (int i=0;i<numFrames;i++)
  	{
! 		if (!stricmp(name, frames[i].Name)) return i;
  	}
- 	return -1;
  }
  
! void FMD3Model::RenderFrame(FTexture * skin, int frameno, int frameno2, double inter, int translation)
  {
  	if (frameno>=numFrames || frameno2>=numFrames) return;
  
--- 206,269 ----
  	return true;
  }
  
! int FMD3Model::FindFrame(const char * name)
  {
! 	for (int i=0;i<numFrames;i++)
  	{
! 		if (!stricmp(name, frames[i].Name)) return i;
! 	}
! 	return -1;
! }
  
! void FMD3Model::RenderTriangles(MD3Surface * surf, MD3Vertex * vert)
! {
! 	gl_RenderState.Apply();
! 	glBegin(GL_TRIANGLES);
! 	for(int i=0; i<surf->numTriangles;i++)
! 	{
! 		for(int j=0;j<3;j++)
  		{
! 			int x = surf->tris[i].VertIndex[j];
  
! 			glTexCoord2fv(&surf->texcoords[x].s);
! 			glVertex3f(vert[x].x, vert[x].z, vert[x].y);
  		}
  	}
+ 	glEnd();
  }
  
! void FMD3Model::RenderFrame(FTexture * skin, int frameno, int cm, int translation)
  {
! 	if (frameno>=numFrames) return;
! 
! 	MD3Frame * frame = &frames[frameno];
! 
! 	// I can't confirm correctness of this because no model I have tested uses this information
! 	// glMatrixMode(GL_MODELVIEW);
! 	// glTranslatef(frame->origin[0], frame->origin[1], frame->origin[2]);
! 
! 	for(int i=0;i<numSurfaces;i++)
  	{
! 		MD3Surface * surf = &surfaces[i];
! 
! 		// [BB] In case no skin is specified via MODELDEF, check if the MD3 has a skin for the current surface.
! 		// Note: Each surface may have a different skin.
! 		FTexture *surfaceSkin = skin;
! 		if (!surfaceSkin)
! 		{
! 			if (surf->numSkins==0) return;
! 			surfaceSkin = surf->skins[0];
! 			if (!surfaceSkin) return;
! 		}
! 
! 		FMaterial * tex = FMaterial::ValidateTexture(surfaceSkin);
! 
! 		tex->Bind(cm, 0, translation);
! 		RenderTriangles(surf, surf->vertices + frameno * surf->numVertices);
  	}
  }
  
! void FMD3Model::RenderFrameInterpolated(FTexture * skin, int frameno, int frameno2, double inter, int cm, int translation)
  {
  	if (frameno>=numFrames || frameno2>=numFrames) return;
  
***************
*** 265,277 ****
  			if (!surfaceSkin) return;
  		}
  
! 		FMaterial * tex = FMaterial::ValidateTexture(surfaceSkin, false);
  
! 		gl_RenderState.SetMaterial(tex, CLAMP_NONE, translation, -1, false);
  
! 		gl_RenderState.Apply();
! 		GLRenderer->mModelVBO->SetupFrame(surf->vindex + frameno * surf->numVertices, surf->vindex + frameno2 * surf->numVertices, inter);
! 		glDrawElements(GL_TRIANGLES, surf->numTriangles * 3, GL_UNSIGNED_INT, (void*)(intptr_t)(surf->iindex * sizeof(unsigned int)));
  	}
  }
  
--- 281,306 ----
  			if (!surfaceSkin) return;
  		}
  
! 		FMaterial * tex = FMaterial::ValidateTexture(surfaceSkin);
! 
! 		tex->Bind(cm, 0, translation);
! 
! 		MD3Vertex* verticesInterpolated = new MD3Vertex[surfaces[i].numVertices];
! 		MD3Vertex* vertices1 = surf->vertices + frameno * surf->numVertices;
! 		MD3Vertex* vertices2 = surf->vertices + frameno2 * surf->numVertices;
! 
! 		// [BB] Calculate the interpolated vertices by linear interpolation.
! 		for( int k = 0; k < surf->numVertices; k++ )
! 		{
! 			verticesInterpolated[k].x = (1-inter)*vertices1[k].x+ (inter)*vertices2[k].x;
! 			verticesInterpolated[k].y = (1-inter)*vertices1[k].y+ (inter)*vertices2[k].y;
! 			verticesInterpolated[k].z = (1-inter)*vertices1[k].z+ (inter)*vertices2[k].z;
! 			// [BB] Apparently RenderTriangles doesn't use nx, ny, nz, so don't interpolate them.
! 		}
  
! 		RenderTriangles(surf, verticesInterpolated);
  
! 		delete[] verticesInterpolated;
  	}
  }
  
