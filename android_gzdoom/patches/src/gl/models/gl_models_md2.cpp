*** doom/src/main/jni/gzdoom/src/gl/models/gl_models_md2.cpp	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/models/gl_models_md2.cpp	2017-06-18 23:34:13.350582049 -0400
***************
*** 162,169 ****
  		skins[i] = LoadSkin(path, buffer + info.offsetSkins + i*64);
  	}
  
- 	texCoords = new FTexCoord[info.numTexCoords];
- 	memcpy(texCoords, (byte*)buffer + info.offsetTexCoords, info.numTexCoords * sizeof(FTexCoord));
  
  	temp = (char*)buffer + info.offsetFrames;
  	frames = new ModelFrame[info.numFrames];
--- 160,165 ----
***************
*** 174,182 ****
  		dmd_packedVertex_t *pVtx;
  
  		memcpy(frame->name, pfr->name, sizeof(pfr->name));
! 		frame->vertices = new DMDModelVertex[info.numVertices];
! 		frame->normals = new DMDModelVertex[info.numVertices];
! 		frame->vindex = UINT_MAX;
  
  		// Translate each vertex.
  		for(k = 0, pVtx = pfr->vertices; k < info.numVertices; k++, pVtx++)
--- 170,177 ----
  		dmd_packedVertex_t *pVtx;
  
  		memcpy(frame->name, pfr->name, sizeof(pfr->name));
! 		frame->vertices = new FModelVertex[info.numVertices];
! 		frame->normals = new FModelVertex[info.numVertices];
  
  		// Translate each vertex.
  		for(k = 0, pVtx = pfr->vertices; k < info.numVertices; k++, pVtx++)
***************
*** 196,206 ****
  	for(i = 0; i < info.numLODs; i++)
  	{
  		lodInfo[i].numTriangles = LittleLong(lodInfo[i].numTriangles);
  		lodInfo[i].offsetTriangles = LittleLong(lodInfo[i].offsetTriangles);
  
! 		lods[i].triangles = triangles[i] = (FTriangle*)(buffer + lodInfo[i].offsetTriangles);
  	}
  
  	loaded=true;
  	return true;
  }
--- 191,215 ----
  	for(i = 0; i < info.numLODs; i++)
  	{
  		lodInfo[i].numTriangles = LittleLong(lodInfo[i].numTriangles);
+ 		lodInfo[i].numGlCommands = LittleLong(lodInfo[i].numGlCommands);
  		lodInfo[i].offsetTriangles = LittleLong(lodInfo[i].offsetTriangles);
+ 		lodInfo[i].offsetGlCommands = LittleLong(lodInfo[i].offsetGlCommands);
  
! 		triangles[i] = (FTriangle*)(buffer + lodInfo[i].offsetTriangles);
! 
! 		lods[i].glCommands = new int[lodInfo[i].numGlCommands];
! 		memcpy(lods[i].glCommands, buffer + lodInfo[i].offsetGlCommands, sizeof(int) * lodInfo[i].numGlCommands);
  	}
  
+ 	// Determine vertex usage at each LOD level.
+ 	vertexUsage = new char[info.numVertices];
+ 	memset(vertexUsage, 0, info.numVertices);
+ 
+ 	for(i = 0; i < info.numLODs; i++)
+ 		for(k = 0; k < lodInfo[i].numTriangles; k++)
+ 			for(c = 0; c < 3; c++)
+ 				vertexUsage[short(triangles[i][k].vertexIndices[c])] |= 1 << i;
+ 
  	loaded=true;
  	return true;
  }
***************
*** 229,275 ****
  
  	for(i = 0; i < info.numLODs; i++)
  	{
! 		if (lods[i].triangles != NULL) delete[] lods[i].triangles;
  	}
  
! 	if (texCoords != NULL) delete[] texCoords;
  }
  
- 
- void FDMDModel::BuildVertexBuffer(FModelVertexBuffer *buf)
- {
- 	for (int i = 0; i < info.numFrames; i++)
- 	{
- 		ModelFrame *frame = &frames[i];
- 		DMDModelVertex *vert = frame->vertices;
- 		DMDModelVertex *norm = frame->normals;
- 
- 		frame->vindex = buf->vbo_shadowdata.Size();
- 
- 
- 		FTriangle *tri = lods[0].triangles;
- 
- 		for (int i = 0; i < lodInfo[0].numTriangles; i++)
- 		{
- 			for (int j = 0; j < 3; j++)
- 			{
- 				FModelVertex bvert;
- 
- 				int ti = tri->textureIndices[j];
- 				int vi = tri->vertexIndices[j];
- 
- 				bvert.Set(vert[vi].xyz[0], vert[vi].xyz[1], vert[vi].xyz[2], (float)texCoords[ti].s /info.skinWidth, (float)texCoords[ti].t/info.skinHeight);
- 				bvert.SetNormal(norm[vi].xyz[0], norm[vi].xyz[1], norm[vi].xyz[2]);
- 				buf->vbo_shadowdata.Push(bvert);
- 			}
- 			tri++;
- 		}
- 
- 	}
- }
- 
- 
- 
  //===========================================================================
  //
  // FDMDModel::FindFrame
--- 238,249 ----
  
  	for(i = 0; i < info.numLODs; i++)
  	{
! 		delete [] lods[i].glCommands;
  	}
  
! 	if (vertexUsage != NULL) delete [] vertexUsage;
  }
  
  //===========================================================================
  //
  // FDMDModel::FindFrame
***************
*** 286,313 ****
  
  //===========================================================================
  //
! //
  //
  //===========================================================================
  
! void FDMDModel::RenderFrame(FTexture * skin, int frameno, int frameno2, double inter, int translation)
  {
! 	if (frameno >= info.numFrames || frameno2 >= info.numFrames) return;
  
  	if (!skin)
  	{
! 		if (info.numSkins == 0) return;
  		skin = skins[0];
  		if (!skin) return;
  	}
  
! 	FMaterial * tex = FMaterial::ValidateTexture(skin, false);
  
! 	gl_RenderState.SetMaterial(tex, CLAMP_NONE, translation, -1, false);
  
! 	gl_RenderState.Apply();
! 	GLRenderer->mModelVBO->SetupFrame(frames[frameno].vindex, frames[frameno2].vindex, inter);
! 	glDrawArrays(GL_TRIANGLES, 0, lodInfo[0].numTriangles * 3);
  }
  
  
--- 260,374 ----
  
  //===========================================================================
  //
! // Render a set of GL commands using the given data.
  //
  //===========================================================================
+ void FDMDModel::RenderGLCommands(void *glCommands, unsigned int numVertices,FModelVertex * vertices)
+ {
+ 	char   *pos;
+ 	FGLCommandVertex * v;
+ 	int     count;
  
! 	gl_RenderState.Apply();
! 	for(pos = (char*)glCommands; *pos;)
! 	{
! 		count = *(int *) pos;
! 		pos += 4;
! 
! 		// The type of primitive depends on the sign.
! 		glBegin(count > 0 ? GL_TRIANGLE_STRIP : GL_TRIANGLE_FAN);
! 		count = abs(count);
! 
! 		while(count--)
! 		{
! 			v = (FGLCommandVertex *) pos;
! 			pos += sizeof(FGLCommandVertex);
! 
! 			glTexCoord2fv(&v->s);
! 			glVertex3fv((float*)&vertices[v->index]);
! 		}
! 
! 		glEnd();
! 	}
! }
! 
! 
! void FDMDModel::RenderFrame(FTexture * skin, int frameno, int cm, int translation)
  {
! 	int activeLod;
! 
! 	if (frameno>=info.numFrames) return;
! 
! 	ModelFrame * frame = &frames[frameno];
! 	//int mainFlags = mf->flags;
  
  	if (!skin)
  	{
! 		if (info.numSkins==0) return;
  		skin = skins[0];
  		if (!skin) return;
  	}
  
! 	FMaterial * tex = FMaterial::ValidateTexture(skin);
  
! 	tex->Bind(cm, 0, translation);
  
! 	int numVerts = info.numVertices;
! 
! 	// Determine the suitable LOD.
! 	/*
! 	if(info.numLODs > 1 && rend_model_lod != 0)
! 	{
! 	float   lodFactor = rend_model_lod * screen->Width() / 640.0f / (GLRenderer->mCurrentFoV / 90.0f);
! 	if(lodFactor) lodFactor = 1 / lodFactor;
! 
! 	// Determine the LOD we will be using.
! 	activeLod = (int) (lodFactor * spr->distance);
! 	if(activeLod < 0) activeLod = 0;
! 	if(activeLod >= mdl->info.numLODs) activeLod = mdl->info.numLODs - 1;
! 	vertexUsage = mdl->vertexUsage;
! 	}
! 	else
! 	*/
! 	{
! 		activeLod = 0;
! 	}
! 
! 	RenderGLCommands(lods[activeLod].glCommands, numVerts, frame->vertices/*, modelColors, NULL*/);
! }
! 
! void FDMDModel::RenderFrameInterpolated(FTexture * skin, int frameno, int frameno2, double inter, int cm, int translation)
! {
! 	int activeLod = 0;
! 
! 	if (frameno>=info.numFrames || frameno2>=info.numFrames) return;
! 
! 	FModelVertex *vertices1 = frames[frameno].vertices;
! 	FModelVertex *vertices2 = frames[frameno2].vertices;
! 
! 	if (!skin)
! 	{
! 		if (info.numSkins==0) return;
! 		skin = skins[0];
! 		if (!skin) return;
! 	}
! 
! 	FMaterial * tex = FMaterial::ValidateTexture(skin);
! 
! 	tex->Bind(cm, 0, translation);
! 
! 	int numVerts = info.numVertices;
! 
! 	// [BB] Calculate the interpolated vertices by linear interpolation.
! 	FModelVertex *verticesInterpolated = new FModelVertex[numVerts];
! 	for( int k = 0; k < numVerts; k++ )
! 	{
! 		for ( int i = 0; i < 3; i++ )
! 			verticesInterpolated[k].xyz[i] = (1-inter)*vertices1[k].xyz[i]+ (inter)*vertices2[k].xyz[i];
! 	}
! 
! 	RenderGLCommands(lods[activeLod].glCommands, numVerts, verticesInterpolated/*, modelColors, NULL*/);
! 	delete[] verticesInterpolated;
  }
  
  
***************
*** 365,370 ****
--- 426,432 ----
  	header.magic = MD2_MAGIC;
  	header.version = 8;
  	header.flags = 0;
+ 	vertexUsage = NULL;
  	info.skinWidth = LittleLong(md2header->skinWidth);
  	info.skinHeight = LittleLong(md2header->skinHeight);
  	info.frameSize = LittleLong(md2header->frameSize);
***************
*** 396,406 ****
  
  	// The frames need to be unpacked.
  	md2_frames = (byte*)buffer + info.offsetFrames;
- 	frames = new ModelFrame[info.numFrames];
- 
- 	texCoords = new FTexCoord[info.numTexCoords];
- 	memcpy(texCoords, (byte*)buffer + info.offsetTexCoords, info.numTexCoords * sizeof(FTexCoord));
  
  
  	for(i = 0, frame = frames; i < info.numFrames; i++, frame++)
  	{
--- 458,465 ----
  
  	// The frames need to be unpacked.
  	md2_frames = (byte*)buffer + info.offsetFrames;
  
+ 	frames = new ModelFrame[info.numFrames];
  
  	for(i = 0, frame = frames; i < info.numFrames; i++, frame++)
  	{
***************
*** 408,416 ****
  		md2_triangleVertex_t *pVtx;
  
  		memcpy(frame->name, pfr->name, sizeof(pfr->name));
! 		frame->vertices = new DMDModelVertex[info.numVertices];
! 		frame->normals = new DMDModelVertex[info.numVertices];
! 		frame->vindex = UINT_MAX;
  
  		// Translate each vertex.
  		for(k = 0, pVtx = pfr->vertices; k < info.numVertices; k++, pVtx++)
--- 467,474 ----
  		md2_triangleVertex_t *pVtx;
  
  		memcpy(frame->name, pfr->name, sizeof(pfr->name));
! 		frame->vertices = new FModelVertex[info.numVertices];
! 		frame->normals = new FModelVertex[info.numVertices];
  
  		// Translate each vertex.
  		for(k = 0, pVtx = pfr->vertices; k < info.numVertices; k++, pVtx++)
***************
*** 429,437 ****
  	}
  
  
! 	lods[0].triangles = new FTriangle[lodInfo[0].numTriangles];
! 		
! 	memcpy(lods[0].triangles, buffer + lodInfo[0].offsetTriangles, sizeof(FTriangle) * lodInfo[0].numTriangles);
  
  	skins = new FTexture *[info.numSkins];
  
--- 487,494 ----
  	}
  
  
! 	lods[0].glCommands = new int[lodInfo[0].numGlCommands];
! 	memcpy(lods[0].glCommands, buffer + lodInfo[0].offsetGlCommands, sizeof(int) * lodInfo[0].numGlCommands);
  
  	skins = new FTexture *[info.numSkins];
  
