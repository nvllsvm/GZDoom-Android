*** doom/src/main/jni/gzdoom/src/gl/models/gl_voxels.cpp	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/models/gl_voxels.cpp	2017-06-18 23:34:13.357248776 -0400
***************
*** 209,221 ****
  //
  //===========================================================================
  
! FVoxelModel::FVoxelModel(FVoxel *voxel, bool owned)
  {
! 	mVoxel = voxel;
! 	mOwningVoxel = owned;
! 	mPalette = new FVoxelTexture(voxel);
! 	Initialize();
! 	iindex = vindex = UINT_MAX;
  }
  
  //===========================================================================
--- 209,261 ----
  //
  //===========================================================================
  
! 
! class FVoxelVertexBuffer : public FVertexBuffer
  {
! 	unsigned int ibo_id;
! 	bool isint;
! 
! public:
! 	FVoxelVertexBuffer(TArray<FVoxelVertex> &verts, TArray<unsigned int> &indices);
! 	~FVoxelVertexBuffer();
! 	void BindVBO();
! 	bool IsInt() const { return isint; }
! };
! 
! 
! //===========================================================================
! //
! // 
! //
! //===========================================================================
! 
! FVoxelVertexBuffer::FVoxelVertexBuffer(TArray<FVoxelVertex> &verts, TArray<unsigned int> &indices)
! {
! 	ibo_id = 0;
! 	if (gl.flags&RFL_VBO)
! 	{
! 		glGenBuffers(1, &ibo_id);
! 
! 		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
! 		glBufferData(GL_ARRAY_BUFFER, verts.Size() * sizeof(FVoxelVertex), &verts[0], GL_STATIC_DRAW);
! 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
! 		if (verts.Size() > 65535)
! 		{
! 			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
! 			isint = true;
! 		}
! 		else
! 		{
! 			unsigned short *sbuffer = new unsigned short[indices.Size()];
! 			for(unsigned i=0;i<indices.Size();i++)
! 			{
! 				sbuffer[i] = (unsigned short)indices[i];
! 			}
! 			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.Size() * sizeof(unsigned short), sbuffer, GL_STATIC_DRAW);
! 			delete [] sbuffer;
! 			isint = false;
! 		}
! 	}
  }
  
  //===========================================================================
***************
*** 224,252 ****
  //
  //===========================================================================
  
! FVoxelModel::~FVoxelModel()
  {
! 	delete mPalette;
! 	if (mOwningVoxel) delete mVoxel;
  }
  
  
  //===========================================================================
  //
  // 
  //
  //===========================================================================
  
! void FVoxelModel::BuildVertexBuffer(FModelVertexBuffer *buf)
  {
! 	vindex = buf->vbo_shadowdata.Size();
! 	iindex = buf->ibo_shadowdata.Size();
  
! 	FModelVertex *mv = &buf->vbo_shadowdata[buf->vbo_shadowdata.Reserve(mVertices.Size())];
! 	unsigned int *mi = &buf->ibo_shadowdata[buf->ibo_shadowdata.Reserve(mIndices.Size())];
  
! 	memcpy(mv, &mVertices[0], sizeof(FModelVertex)* mVertices.Size());
! 	memcpy(mi, &mIndices[0], sizeof(unsigned int)* mIndices.Size());
  }
  
  //===========================================================================
--- 264,323 ----
  //
  //===========================================================================
  
! FVoxelVertexBuffer::~FVoxelVertexBuffer()
  {
! 	if (ibo_id != 0)
! 	{
! 		glDeleteBuffers(1, &ibo_id);
! 	}
! }
! 
! //===========================================================================
! //
! // 
! //
! //===========================================================================
! #define VVO ((FVoxelVertex*)NULL)
! 
! void FVoxelVertexBuffer::BindVBO()
! {
! 	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
! 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
! 	glVertexPointer(3,GL_FLOAT, sizeof(FVoxelVertex), &VVO->x);
! 	glTexCoordPointer(2,GL_FLOAT, sizeof(FVoxelVertex), &VVO->u);
! 	glEnableClientState(GL_VERTEX_ARRAY);
! 	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
! 	glEnableClientState(GL_INDEX_ARRAY);
  }
  
  
+ 
  //===========================================================================
  //
  // 
  //
  //===========================================================================
  
! FVoxelModel::FVoxelModel(FVoxel *voxel, bool owned)
  {
! 	mVoxel = voxel;
! 	mOwningVoxel = owned;
! 	mVBO = NULL;
! 	mPalette = new FVoxelTexture(voxel);
! 	Initialize();
! }
  
! //===========================================================================
! //
! // 
! //
! //===========================================================================
  
! FVoxelModel::~FVoxelModel()
! {
! 	CleanGLData();
! 	delete mPalette;
! 	if (mOwningVoxel) delete mVoxel;
  }
  
  //===========================================================================
***************
*** 255,268 ****
  //
  //===========================================================================
  
! unsigned int FVoxelModel::AddVertex(FModelVertex &vert, FVoxelMap &check)
  {
  	unsigned int index = check[vert];
  	if (index == 0xffffffff)
  	{
  		index = check[vert] =mVertices.Push(vert);
  	}
! 	return index;
  }
  
  //===========================================================================
--- 326,339 ----
  //
  //===========================================================================
  
! void FVoxelModel::AddVertex(FVoxelVertex &vert, FVoxelMap &check)
  {
  	unsigned int index = check[vert];
  	if (index == 0xffffffff)
  	{
  		index = check[vert] =mVertices.Push(vert);
  	}
! 	mIndices.Push(index);
  }
  
  //===========================================================================
***************
*** 277,284 ****
  	float PivotY = mVoxel->Mips[0].PivotY / 256.f;
  	float PivotZ = mVoxel->Mips[0].PivotZ / 256.f;
  	int h = mVoxel->Mips[0].SizeZ;
! 	FModelVertex vert;
! 	unsigned int indx[4];
  
  	vert.u = (((col & 15) * 255 / 16) + 7) / 255.f;
  	vert.v = (((col / 16) * 255 / 16) + 7) / 255.f;
--- 348,355 ----
  	float PivotY = mVoxel->Mips[0].PivotY / 256.f;
  	float PivotZ = mVoxel->Mips[0].PivotZ / 256.f;
  	int h = mVoxel->Mips[0].SizeZ;
! 	FVoxelVertex vert;
! 
  
  	vert.u = (((col & 15) * 255 / 16) + 7) / 255.f;
  	vert.v = (((col / 16) * 255 / 16) + 7) / 255.f;
***************
*** 286,314 ****
  	vert.x =  x1 - PivotX;
  	vert.z = -y1 + PivotY;
  	vert.y = -z1 + PivotZ;
! 	indx[0] = AddVertex(vert, check);
  
  	vert.x =  x2 - PivotX;
  	vert.z = -y2 + PivotY;
  	vert.y = -z2 + PivotZ;
! 	indx[1] = AddVertex(vert, check);
  
  	vert.x =  x4 - PivotX;
  	vert.z = -y4 + PivotY;
  	vert.y = -z4 + PivotZ;
! 	indx[2] = AddVertex(vert, check);
  
  	vert.x =  x3 - PivotX;
  	vert.z = -y3 + PivotY;
  	vert.y = -z3 + PivotZ;
! 	indx[3] = AddVertex(vert, check);
  
- 	mIndices.Push(indx[0]);
- 	mIndices.Push(indx[1]);
- 	mIndices.Push(indx[3]);
- 	mIndices.Push(indx[1]);
- 	mIndices.Push(indx[3]);
- 	mIndices.Push(indx[2]);
  }
  
  //===========================================================================
--- 357,379 ----
  	vert.x =  x1 - PivotX;
  	vert.z = -y1 + PivotY;
  	vert.y = -z1 + PivotZ;
! 	AddVertex(vert, check);
  
  	vert.x =  x2 - PivotX;
  	vert.z = -y2 + PivotY;
  	vert.y = -z2 + PivotZ;
! 	AddVertex(vert, check);
  
  	vert.x =  x4 - PivotX;
  	vert.z = -y4 + PivotY;
  	vert.y = -z4 + PivotZ;
! 	AddVertex(vert, check);
  
  	vert.x =  x3 - PivotX;
  	vert.z = -y3 + PivotY;
  	vert.y = -z3 + PivotZ;
! 	AddVertex(vert, check);
  
  }
  
  //===========================================================================
***************
*** 399,404 ****
--- 464,498 ----
  
  //===========================================================================
  //
+ // 
+ //
+ //===========================================================================
+ 
+ void FVoxelModel::MakeGLData()
+ {
+ 	if (gl.flags&RFL_VBO)
+ 	{
+ 		mVBO = new FVoxelVertexBuffer(mVertices, mIndices);
+ 	}
+ }
+ 
+ //===========================================================================
+ //
+ // 
+ //
+ //===========================================================================
+ 
+ void FVoxelModel::CleanGLData()
+ {
+ 	if (mVBO != NULL)
+ 	{
+ 		delete mVBO;
+ 		mVBO = NULL;
+ 	}
+ }
+ 
+ //===========================================================================
+ //
  // Voxels don't have frames so always return 0
  //
  //===========================================================================
***************
*** 410,426 ****
  
  //===========================================================================
  //
! // Voxels never interpolate between frames, they only have one.
  //
  //===========================================================================
  
! void FVoxelModel::RenderFrame(FTexture * skin, int frame, int frame2, double inter, int translation)
  {
! 	FMaterial * tex = FMaterial::ValidateTexture(skin, false);
! 	gl_RenderState.SetMaterial(tex, CLAMP_NOFILTER, translation, -1, false);
! 
  	gl_RenderState.Apply();
! 	GLRenderer->mModelVBO->SetupFrame(vindex, vindex, 0.f);
! 	glDrawElements(GL_TRIANGLES, mIndices.Size(), GL_UNSIGNED_INT, (void*)(intptr_t)(iindex * sizeof(unsigned int)));
  }
  
--- 504,549 ----
  
  //===========================================================================
  //
! // 
  //
  //===========================================================================
  
! void FVoxelModel::RenderFrame(FTexture * skin, int frame, int cm, int translation)
  {
! 	FMaterial * tex = FMaterial::ValidateTexture(skin);
! 	tex->Bind(cm, 0, translation);
  	gl_RenderState.Apply();
! 
! 	if (gl.flags&RFL_VBO)
! 	{
! 		if (mVBO == NULL) MakeGLData();
! 		if (mVBO != NULL)
! 		{
! 			mVBO->BindVBO();
! 			glDrawElements(GL_QUADS, mIndices.Size(), mVBO->IsInt()? GL_UNSIGNED_INT:GL_UNSIGNED_SHORT, 0);
! 			GLRenderer->mVBO->BindVBO();
! 			return;
! 		}
! 	}
! 
! 	glBegin(GL_QUADS);
! 	for(unsigned i=0;i < mIndices.Size(); i++)
! 	{
! 		FVoxelVertex *vert = &mVertices[mIndices[i]];
! 		glTexCoord2fv(&vert->u);
! 		glVertex3fv(&vert->x);
! 	}
! 	glEnd();
! }
! 
! //===========================================================================
! //
! // Voxels never interpolate between frames
! //
! //===========================================================================
! 
! void FVoxelModel::RenderFrameInterpolated(FTexture * skin, int frame, int frame2, double inter, int cm, int translation)
! {
! 	RenderFrame(skin, frame, cm, translation);
  }
  
