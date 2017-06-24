*** doom/src/main/jni/gzdoom/src/gl/scene/gl_skydome.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_skydome.cpp	2017-06-18 23:34:13.310581678 -0400
***************
*** 64,98 ****
  //
  //-----------------------------------------------------------------------------
  
! CVAR(Float, skyoffset, 0, 0)	// for testing
  
  extern int skyfog;
  
! //-----------------------------------------------------------------------------
! //
! //
! //
! //-----------------------------------------------------------------------------
! 
! FSkyVertexBuffer::FSkyVertexBuffer()
! {
! 	CreateDome();
  
! 	glBindVertexArray(vao_id);
! 	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
! 	glVertexAttribPointer(VATTR_VERTEX, 3, GL_FLOAT, false, sizeof(FSkyVertex), &VSO->x);
! 	glVertexAttribPointer(VATTR_TEXCOORD, 2, GL_FLOAT, false, sizeof(FSkyVertex), &VSO->u);
! 	glVertexAttribPointer(VATTR_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(FSkyVertex), &VSO->color);
! 	glEnableVertexAttribArray(VATTR_VERTEX);
! 	glEnableVertexAttribArray(VATTR_TEXCOORD);
! 	glEnableVertexAttribArray(VATTR_COLOR);
! 	glBindVertexArray(0);
  
! }
  
- FSkyVertexBuffer::~FSkyVertexBuffer()
- {
- }
  
  //-----------------------------------------------------------------------------
  //
--- 63,90 ----
  //
  //-----------------------------------------------------------------------------
  
! CVAR (Int, gl_sky_detail, 16, CVAR_ARCHIVE | CVAR_GLOBALCONFIG)
! EXTERN_CVAR (Bool, r_stretchsky)
  
  extern int skyfog;
  
! // The texture offset to be applied to the texture coordinates in SkyVertex().
  
! static angle_t maxSideAngle = ANGLE_180 / 3;
! static int rows, columns;	
! static fixed_t scale = 10000 << FRACBITS;
! static bool yflip;
! static int texw;
! static float yAdd;
! static bool foglayer;
! static bool secondlayer;
! static float R,G,B;
! static bool skymirror; 
  
! #define SKYHEMI_UPPER		0x1
! #define SKYHEMI_LOWER		0x2
! #define SKYHEMI_JUST_CAP	0x4	// Just draw the top or bottom cap.
  
  
  //-----------------------------------------------------------------------------
  //
***************
*** 100,253 ****
  //
  //-----------------------------------------------------------------------------
  
! void FSkyVertexBuffer::SkyVertex(int r, int c, bool yflip)
  {
! 	static const angle_t maxSideAngle = ANGLE_180 / 3;
! 	static const fixed_t scale = 10000 << FRACBITS;
! 
! 	angle_t topAngle= (angle_t)(c / (float)mColumns * ANGLE_MAX);
! 	angle_t sideAngle = maxSideAngle * (mRows - r) / mRows;
  	fixed_t height = finesine[sideAngle>>ANGLETOFINESHIFT];
  	fixed_t realRadius = FixedMul(scale, finecosine[sideAngle>>ANGLETOFINESHIFT]);
  	fixed_t x = FixedMul(realRadius, finecosine[topAngle>>ANGLETOFINESHIFT]);
  	fixed_t y = (!yflip) ? FixedMul(scale, height) : FixedMul(scale, height) * -1;
  	fixed_t z = FixedMul(realRadius, finesine[topAngle>>ANGLETOFINESHIFT]);
! 
! 	FSkyVertex vert;
  	
! 	vert.color = r == 0 ? 0xffffff : 0xffffffff;
! 		
! 	// And the texture coordinates.
! 	if(!yflip)	// Flipped Y is for the lower hemisphere.
! 	{
! 		vert.u = (-c / (float)mColumns) ;
! 		vert.v = (r / (float)mRows);
! 	}
! 	else
  	{
! 		vert.u = (-c / (float)mColumns);
! 		vert.v = 1.0f + ((mRows - r) / (float)mRows);
  	}
- 
  	if (r != 4) y+=FRACUNIT*300;
  	// And finally the vertex.
! 	vert.x =-FIXED2FLOAT(x);	// Doom mirrors the sky vertically!
! 	vert.y = FIXED2FLOAT(y) - 1.f;
! 	vert.z = FIXED2FLOAT(z);
! 
! 	mVertices.Push(vert);
  }
  
  
  //-----------------------------------------------------------------------------
  //
! //
  //
  //-----------------------------------------------------------------------------
  
! void FSkyVertexBuffer::CreateSkyHemisphere(int hemi)
  {
  	int r, c;
! 	bool yflip = !!(hemi & SKYHEMI_LOWER);
! 
! 	mPrimStart.Push(mVertices.Size());
! 
! 	for (c = 0; c < mColumns; c++)
  	{
! 		SkyVertex(1, c, yflip);
  	}
! 
! 	// The total number of triangles per hemisphere can be calculated
! 	// as follows: rows * columns * 2 + 2 (for the top cap).
! 	for (r = 0; r < mRows; r++)
  	{
! 		mPrimStart.Push(mVertices.Size());
! 		for (c = 0; c <= mColumns; c++)
! 		{
! 			SkyVertex(r + yflip, c, yflip);
! 			SkyVertex(r + 1 - yflip, c, yflip);
! 		}
  	}
- }
- 
- //-----------------------------------------------------------------------------
- //
- //
- //
- //-----------------------------------------------------------------------------
- 
- void FSkyVertexBuffer::CreateDome()
- {
- 	// the first thing we put into the buffer is the fog layer object which is just 4 triangles around the viewpoint.
- 
- 	mVertices.Reserve(12);
- 	mVertices[0].Set( 1.0f,  1.0f, -1.0f);
- 	mVertices[1].Set( 1.0f, -1.0f, -1.0f);
- 	mVertices[2].Set(-1.0f,  0.0f, -1.0f);
- 
- 	mVertices[3].Set( 1.0f,  1.0f, -1.0f);
- 	mVertices[4].Set( 1.0f, -1.0f, -1.0f);
- 	mVertices[5].Set( 0.0f,  0.0f,  1.0f);
- 
- 	mVertices[6].Set(-1.0f, 0.0f, -1.0f);
- 	mVertices[7].Set( 1.0f, 1.0f, -1.0f);
- 	mVertices[8].Set( 0.0f, 0.0f,  1.0f);
- 
- 	mVertices[9].Set(1.0f, -1.0f, -1.0f);
- 	mVertices[10].Set(-1.0f, 0.0f, -1.0f);
- 	mVertices[11].Set( 0.0f, 0.0f,  1.0f);
- 
- 	mColumns = 128;
- 	mRows = 4;
- 	CreateSkyHemisphere(SKYHEMI_UPPER);
- 	CreateSkyHemisphere(SKYHEMI_LOWER);
- 	mPrimStart.Push(mVertices.Size());
- 	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
- 	glBufferData(GL_ARRAY_BUFFER, mVertices.Size() * sizeof(FSkyVertex), &mVertices[0], GL_STATIC_DRAW);
- }
  
! //-----------------------------------------------------------------------------
! //
! //
! //
! //-----------------------------------------------------------------------------
! 
! inline void FSkyVertexBuffer::RenderRow(int prim, int row)
! {
! 	glDrawArrays(prim, mPrimStart[row], mPrimStart[row + 1] - mPrimStart[row]);
! }
! 
! //-----------------------------------------------------------------------------
! //
! //
! //
! //-----------------------------------------------------------------------------
  
- void FSkyVertexBuffer::RenderDome(FMaterial *tex, int mode)
- {
- 	int rc = mRows + 1;
  
! 	// The caps only get drawn for the main layer but not for the overlay.
! 	if (mode == SKYMODE_MAINLAYER && tex != NULL)
  	{
! 		PalEntry pe = tex->tex->GetSkyCapColor(false);
! 		gl_RenderState.SetObjectColor(pe);
  		gl_RenderState.EnableTexture(false);
! 		gl_RenderState.Apply();
! 		RenderRow(GL_TRIANGLE_FAN, 0);
  
- 		pe = tex->tex->GetSkyCapColor(true);
- 		gl_RenderState.SetObjectColor(pe);
- 		gl_RenderState.Apply();
- 		RenderRow(GL_TRIANGLE_FAN, rc);
  		gl_RenderState.EnableTexture(true);
  	}
! 	gl_RenderState.SetObjectColor(0xffffffff);
! 	gl_RenderState.Apply();
! 	for (int i = 1; i <= mRows; i++)
  	{
! 		RenderRow(GL_TRIANGLE_STRIP, i);
! 		RenderRow(GL_TRIANGLE_STRIP, rc + i);
  	}
  }
  
--- 92,240 ----
  //
  //-----------------------------------------------------------------------------
  
! static void SkyVertex(int r, int c)
  {
! 	angle_t topAngle= (angle_t)(c / (float)columns * ANGLE_MAX);
! 	angle_t sideAngle = maxSideAngle * (rows - r) / rows;
  	fixed_t height = finesine[sideAngle>>ANGLETOFINESHIFT];
  	fixed_t realRadius = FixedMul(scale, finecosine[sideAngle>>ANGLETOFINESHIFT]);
  	fixed_t x = FixedMul(realRadius, finecosine[topAngle>>ANGLETOFINESHIFT]);
  	fixed_t y = (!yflip) ? FixedMul(scale, height) : FixedMul(scale, height) * -1;
  	fixed_t z = FixedMul(realRadius, finesine[topAngle>>ANGLETOFINESHIFT]);
! 	float fx, fy, fz;
! 	float color = r * 1.f / rows;
! 	float u, v;
! 	float timesRepeat;
  	
! 	timesRepeat = (short)(4 * (256.f / texw));
! 	if (timesRepeat == 0.f) timesRepeat = 1.f;
! 	
! 	if (!foglayer)
  	{
! 		gl_SetColor(255, 0, NULL, r==0? 0.0f : 1.0f);
! 		
! 		// And the texture coordinates.
! 		if(!yflip)	// Flipped Y is for the lower hemisphere.
! 		{
! 			u = (-timesRepeat * c / (float)columns) ;
! 			v = (r / (float)rows) + yAdd;
! 		}
! 		else
! 		{
! 			u = (-timesRepeat * c / (float)columns) ;
! 			v = 1.0f + ((rows-r)/(float)rows) + yAdd;
! 		}
! 		
! 		
! 		glTexCoord2f(skymirror? -u:u, v);
  	}
  	if (r != 4) y+=FRACUNIT*300;
  	// And finally the vertex.
! 	fx =-FIXED2FLOAT(x);	// Doom mirrors the sky vertically!
! 	fy = FIXED2FLOAT(y);
! 	fz = FIXED2FLOAT(z);
! 	glVertex3f(fx, fy - 1.f, fz);
  }
  
  
  //-----------------------------------------------------------------------------
  //
! // Hemi is Upper or Lower. Zero is not acceptable.
! // The current texture is used. SKYHEMI_NO_TOPCAP can be used.
  //
  //-----------------------------------------------------------------------------
  
! static void RenderSkyHemisphere(int hemi, bool mirror)
  {
  	int r, c;
! 	
! 	if (hemi & SKYHEMI_LOWER)
  	{
! 		yflip = true;
  	}
! 	else
  	{
! 		yflip = false;
  	}
  
! 	skymirror = mirror;
! 	
! 	// The top row (row 0) is the one that's faded out.
! 	// There must be at least 4 columns. The preferable number
! 	// is 4n, where n is 1, 2, 3... There should be at least
! 	// two rows because the first one is always faded.
! 	rows = 4;
! 	
! 	if (hemi & SKYHEMI_JUST_CAP)
! 	{
! 		return;
! 	}
  
  
! 	// Draw the cap as one solid color polygon
! 	if (!foglayer)
  	{
! 		columns = 4 * (gl_sky_detail > 0 ? gl_sky_detail : 1);
! 		foglayer=true;
  		gl_RenderState.EnableTexture(false);
! 		gl_RenderState.Apply(true);
! 
! 
! 		if (!secondlayer)
! 		{
! 			glColor3f(R, G ,B);
! 			glBegin(GL_TRIANGLE_FAN);
! 			for(c = 0; c < columns; c++)
! 			{
! 				SkyVertex(1, c);
! 			}
! 			glEnd();
! 		}
  
  		gl_RenderState.EnableTexture(true);
+ 		foglayer=false;
+ 		gl_RenderState.Apply();
  	}
! 	else
  	{
! 		gl_RenderState.Apply(true);
! 		columns=4;	// no need to do more!
! 		glBegin(GL_TRIANGLE_FAN);
! 		for(c = 0; c < columns; c++)
! 		{
! 			SkyVertex(0, c);
! 		}
! 		glEnd();
! 	}
! 	
! 	// The total number of triangles per hemisphere can be calculated
! 	// as follows: rows * columns * 2 + 2 (for the top cap).
! 	for(r = 0; r < rows; r++)
! 	{
! 		if (yflip)
! 		{
! 			glBegin(GL_TRIANGLE_STRIP);
!             SkyVertex(r + 1, 0);
! 			SkyVertex(r, 0);
! 			for(c = 1; c <= columns; c++)
! 			{
! 				SkyVertex(r + 1, c);
! 				SkyVertex(r, c);
! 			}
! 			glEnd();
! 		}
! 		else
! 		{
! 			glBegin(GL_TRIANGLE_STRIP);
!             SkyVertex(r, 0);
! 			SkyVertex(r + 1, 0);
! 			for(c = 1; c <= columns; c++)
! 			{
! 				SkyVertex(r, c);
! 				SkyVertex(r + 1, c);
! 			}
! 			glEnd();
! 		}
  	}
  }
  
***************
*** 257,308 ****
  //
  //
  //-----------------------------------------------------------------------------
  
! void RenderDome(FMaterial * tex, float x_offset, float y_offset, bool mirror, int mode)
  {
  	int texh = 0;
! 	int texw = 0;
  
! 	// 57 world units roughly represent one sky texel for the glTranslate call.
  	const float skyoffsetfactor = 57;
  
  	if (tex)
  	{
! 		gl_RenderState.SetMaterial(tex, CLAMP_NONE, 0, -1, false);
! 		texw = tex->TextureWidth();
! 		texh = tex->TextureHeight();
! 		gl_RenderState.EnableModelMatrix(true);
  
! 		gl_RenderState.mModelMatrix.loadIdentity();
! 		gl_RenderState.mModelMatrix.rotate(-180.0f+x_offset, 0.f, 1.f, 0.f);
  
- 		float xscale = 1024.f / float(texw);
- 		float yscale = 1.f;
  		if (texh < 200)
  		{
! 			gl_RenderState.mModelMatrix.translate(0.f, -1250.f, 0.f);
! 			gl_RenderState.mModelMatrix.scale(1.f, texh/230.f, 1.f);
  		}
  		else if (texh <= 240)
  		{
! 			gl_RenderState.mModelMatrix.translate(0.f, (200 - texh + tex->tex->SkyOffset + skyoffset)*skyoffsetfactor, 0.f);
! 			gl_RenderState.mModelMatrix.scale(1.f, 1.f + ((texh-200.f)/200.f) * 1.17f, 1.f);
  		}
  		else
  		{
! 			gl_RenderState.mModelMatrix.translate(0.f, (-40 + tex->tex->SkyOffset + skyoffset)*skyoffsetfactor, 0.f);
! 			gl_RenderState.mModelMatrix.scale(1.f, 1.2f * 1.17f, 1.f);
! 			yscale = 240.f / texh;
! 		}
! 		gl_RenderState.EnableTextureMatrix(true);
! 		gl_RenderState.mTextureMatrix.loadIdentity();
! 		gl_RenderState.mTextureMatrix.scale(mirror? -xscale : xscale, yscale, 1.f);
! 		gl_RenderState.mTextureMatrix.translate(1.f, y_offset / texh, 1.f);
  	}
  
! 	GLRenderer->mSkyVBO->RenderDome(tex, mode);
! 	gl_RenderState.EnableTextureMatrix(false);
! 	gl_RenderState.EnableModelMatrix(false);
  }
  
  
--- 244,342 ----
  //
  //
  //-----------------------------------------------------------------------------
+ CVAR(Float, skyoffset, 0, 0)	// for testing
  
! static void RenderDome(FTextureID texno, FMaterial * tex, float x_offset, float y_offset, bool mirror, int CM_Index)
  {
  	int texh = 0;
! 	bool texscale = false;
  
! 	// 57 worls units roughly represent one sky texel for the glTranslate call.
  	const float skyoffsetfactor = 57;
  
  	if (tex)
  	{
! 		glPushMatrix();
! 		tex->Bind(CM_Index, 0, 0);
! 		texw = tex->TextureWidth(GLUSE_TEXTURE);
! 		texh = tex->TextureHeight(GLUSE_TEXTURE);
  
! 		glRotatef(-180.0f+x_offset, 0.f, 1.f, 0.f);
! 		yAdd = y_offset/texh;
  
  		if (texh < 200)
  		{
! 			glTranslatef(0.f, -1250.f, 0.f);
! 			glScalef(1.f, texh/230.f, 1.f);
  		}
  		else if (texh <= 240)
  		{
! 			glTranslatef(0.f, (200 - texh + tex->tex->SkyOffset + skyoffset)*skyoffsetfactor, 0.f);
! 			glScalef(1.f, 1.f + ((texh-200.f)/200.f) * 1.17f, 1.f);
  		}
  		else
  		{
! 			glTranslatef(0.f, (-40 + tex->tex->SkyOffset + skyoffset)*skyoffsetfactor, 0.f);
! 			glScalef(1.f, 1.2f * 1.17f, 1.f);
! 			glMatrixMode(GL_TEXTURE);
! 			glPushMatrix();
! 			glLoadIdentity();
! 			glScalef(1.f, 240.f / texh, 1.f);
! 			glMatrixMode(GL_MODELVIEW);
! 			texscale = true;
! 		}
! 	}
! 
! 	if (tex && !secondlayer) 
! 	{
! 		PalEntry pe = tex->tex->GetSkyCapColor(false);
! 		if (CM_Index!=CM_DEFAULT) ModifyPalette(&pe, &pe, CM_Index, 1);
! 
! 		R=pe.r/255.0f;
! 		G=pe.g/255.0f;
! 		B=pe.b/255.0f;
! 
! 		if (gl_fixedcolormap)
! 		{
! 			float rr, gg, bb;
! 
! 			gl_GetLightColor(255, 0, NULL, &rr, &gg, &bb);
! 			R*=rr;
! 			G*=gg;
! 			B*=bb;
! 		}
! 	}
! 
! 	RenderSkyHemisphere(SKYHEMI_UPPER, mirror);
! 
! 	if (tex && !secondlayer) 
! 	{
! 		PalEntry pe = tex->tex->GetSkyCapColor(true);
! 		if (CM_Index!=CM_DEFAULT) ModifyPalette(&pe, &pe, CM_Index, 1);
! 		R=pe.r/255.0f;
! 		G=pe.g/255.0f;
! 		B=pe.b/255.0f;
! 
! 		if (gl_fixedcolormap != CM_DEFAULT)
! 		{
! 			float rr,gg,bb;
! 
! 			gl_GetLightColor(255, 0, NULL, &rr, &gg, &bb);
! 			R*=rr;
! 			G*=gg;
! 			B*=bb;
! 		}
  	}
  
! 	RenderSkyHemisphere(SKYHEMI_LOWER, mirror);
! 	if (texscale)
! 	{
! 		glMatrixMode(GL_TEXTURE);
! 		glPopMatrix();
! 		glMatrixMode(GL_MODELVIEW);
! 	}
! 	if (tex) glPopMatrix();
! 
  }
  
  
***************
*** 312,460 ****
  //
  //-----------------------------------------------------------------------------
  
! static void RenderBox(FTextureID texno, FMaterial * gltex, float x_offset, bool sky2)
  {
  	FSkyBox * sb = static_cast<FSkyBox*>(gltex->tex);
  	int faces;
  	FMaterial * tex;
  
- 	gl_RenderState.EnableModelMatrix(true);
- 	gl_RenderState.mModelMatrix.loadIdentity();
- 
  	if (!sky2)
! 		gl_RenderState.mModelMatrix.rotate(-180.0f+x_offset, glset.skyrotatevector.X, glset.skyrotatevector.Z, glset.skyrotatevector.Y);
  	else
! 		gl_RenderState.mModelMatrix.rotate(-180.0f+x_offset, glset.skyrotatevector2.X, glset.skyrotatevector2.Z, glset.skyrotatevector2.Y);
  
- 	FFlatVertex *ptr;
  	if (sb->faces[5]) 
  	{
  		faces=4;
  
  		// north
! 		tex = FMaterial::ValidateTexture(sb->faces[0], false);
! 		gl_RenderState.SetMaterial(tex, CLAMP_XY, 0, -1, false);
  		gl_RenderState.Apply();
! 
! 		ptr = GLRenderer->mVBO->GetBuffer();
! 		ptr->Set(128.f, 128.f, -128.f, 0, 0);
! 		ptr++;
! 		ptr->Set(-128.f, 128.f, -128.f, 1, 0);
! 		ptr++;
! 		ptr->Set(128.f, -128.f, -128.f, 0, 1);
! 		ptr++;
! 		ptr->Set(-128.f, -128.f, -128.f, 1, 1);
! 		ptr++;
! 		GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  
  		// east
! 		tex = FMaterial::ValidateTexture(sb->faces[1], false);
! 		gl_RenderState.SetMaterial(tex, CLAMP_XY, 0, -1, false);
  		gl_RenderState.Apply();
! 
! 		ptr = GLRenderer->mVBO->GetBuffer();
! 		ptr->Set(-128.f, 128.f, -128.f, 0, 0);
! 		ptr++;
! 		ptr->Set(-128.f, 128.f, 128.f, 1, 0);
! 		ptr++;
! 		ptr->Set(-128.f, -128.f, -128.f, 0, 1);
! 		ptr++;
! 		ptr->Set(-128.f, -128.f, 128.f, 1, 1);
! 		ptr++;
! 		GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  
  		// south
! 		tex = FMaterial::ValidateTexture(sb->faces[2], false);
! 		gl_RenderState.SetMaterial(tex, CLAMP_XY, 0, -1, false);
  		gl_RenderState.Apply();
! 
! 		ptr = GLRenderer->mVBO->GetBuffer();
! 		ptr->Set(-128.f, 128.f, 128.f, 0, 0);
! 		ptr++;
! 		ptr->Set(128.f, 128.f, 128.f, 1, 0);
! 		ptr++;
! 		ptr->Set(-128.f, -128.f, 128.f, 0, 1);
! 		ptr++;
! 		ptr->Set(128.f, -128.f, 128.f, 1, 1);
! 		ptr++;
! 		GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  
  		// west
! 		tex = FMaterial::ValidateTexture(sb->faces[3], false);
! 		gl_RenderState.SetMaterial(tex, CLAMP_XY, 0, -1, false);
  		gl_RenderState.Apply();
! 
! 		ptr = GLRenderer->mVBO->GetBuffer();
! 		ptr->Set(128.f, 128.f, 128.f, 0, 0);
! 		ptr++;
! 		ptr->Set(128.f, 128.f, -128.f, 1, 0);
! 		ptr++;
! 		ptr->Set(128.f, -128.f, 128.f, 0, 1);
! 		ptr++;
! 		ptr->Set(128.f, -128.f, -128.f, 1, 1);
! 		ptr++;
! 		GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  	}
  	else 
  	{
  		faces=1;
  
! 		ptr = GLRenderer->mVBO->GetBuffer();
! 		ptr->Set(128.f, 128.f, -128.f, 0, 0);
! 		ptr++;
! 		ptr->Set(128.f, -128.f, -128.f, 0, 1);
! 		ptr++;
! 		ptr->Set(-128.f, 128.f, -128.f, 0.25f, 0);
! 		ptr++;
! 		ptr->Set(-128.f, -128.f, -128.f, 0.25f, 1);
! 		ptr++;
! 		ptr->Set(-128.f, 128.f, 128.f, 0.5f, 0);
! 		ptr++;
! 		ptr->Set(-128.f, -128.f, 128.f, 0.5f, 1);
! 		ptr++;
! 		ptr->Set(128.f, 128.f, 128.f, 0.75f, 0);
! 		ptr++;
! 		ptr->Set(128.f, -128.f, 128.f, 0.75f, 1);
! 		ptr++;
! 		ptr->Set(128.f, 128.f, -128.f, 1, 0);
! 		ptr++;
! 		ptr->Set(128.f, -128.f, -128.f, 1, 1);
! 		ptr++;
! 		GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  	}
  
  	// top
! 	tex = FMaterial::ValidateTexture(sb->faces[faces], false);
! 	gl_RenderState.SetMaterial(tex, CLAMP_XY, 0, -1, false);
  	gl_RenderState.Apply();
  
- 	ptr = GLRenderer->mVBO->GetBuffer();
- 	ptr->Set(128.f, 128.f, -128.f, 0, sb->fliptop);
- 	ptr++;
- 	ptr->Set(-128.f, 128.f, -128.f, 1, sb->fliptop);
- 	ptr++;
- 	ptr->Set(128.f, 128.f, 128.f, 0, !sb->fliptop);
- 	ptr++;
- 	ptr->Set(-128.f, 128.f, 128.f, 1, !sb->fliptop);
- 	ptr++;
- 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  
  	// bottom
! 	tex = FMaterial::ValidateTexture(sb->faces[faces+1], false);
! 	gl_RenderState.SetMaterial(tex, CLAMP_XY, 0, -1, false);
  	gl_RenderState.Apply();
  
- 	ptr = GLRenderer->mVBO->GetBuffer();
- 	ptr->Set(128.f, -128.f, -128.f, 0, 0);
- 	ptr++;
- 	ptr->Set(-128.f, -128.f, -128.f, 1, 0);
- 	ptr++;
- 	ptr->Set(128.f, -128.f, 128.f, 0, 1);
- 	ptr++;
- 	ptr->Set(-128.f, -128.f, 128.f, 1, 1);
- 	ptr++;
- 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
- 	gl_RenderState.EnableModelMatrix(false);
  }
  
  //-----------------------------------------------------------------------------
--- 346,530 ----
  //
  //-----------------------------------------------------------------------------
  
! static void RenderBox(FTextureID texno, FMaterial * gltex, float x_offset, int CM_Index, bool sky2)
  {
  	FSkyBox * sb = static_cast<FSkyBox*>(gltex->tex);
  	int faces;
  	FMaterial * tex;
  
  	if (!sky2)
! 		glRotatef(-180.0f+x_offset, glset.skyrotatevector.X, glset.skyrotatevector.Z, glset.skyrotatevector.Y);
  	else
! 		glRotatef(-180.0f+x_offset, glset.skyrotatevector2.X, glset.skyrotatevector2.Z, glset.skyrotatevector2.Y);
! 
! 	glColor3f(R, G ,B);
  
  	if (sb->faces[5]) 
  	{
  		faces=4;
  
  		// north
! 		tex = FMaterial::ValidateTexture(sb->faces[0]);
! 		tex->Bind(CM_Index, GLT_CLAMPX|GLT_CLAMPY, 0);
  		gl_RenderState.Apply();
! 		glBegin(GL_TRIANGLE_FAN);
! 		glTexCoord2f(0, 0);
! 		glVertex3f(128.f, 128.f, -128.f);
! 		glTexCoord2f(1, 0);
! 		glVertex3f(-128.f, 128.f, -128.f);
! 		glTexCoord2f(1, 1);
! 		glVertex3f(-128.f, -128.f, -128.f);
! 		glTexCoord2f(0, 1);
! 		glVertex3f(128.f, -128.f, -128.f);
! 		glEnd();
  
  		// east
! 		tex = FMaterial::ValidateTexture(sb->faces[1]);
! 		tex->Bind(CM_Index, GLT_CLAMPX|GLT_CLAMPY, 0);
  		gl_RenderState.Apply();
! 		glBegin(GL_TRIANGLE_FAN);
! 		glTexCoord2f(0, 0);
! 		glVertex3f(-128.f, 128.f, -128.f);
! 		glTexCoord2f(1, 0);
! 		glVertex3f(-128.f, 128.f, 128.f);
! 		glTexCoord2f(1, 1);
! 		glVertex3f(-128.f, -128.f, 128.f);
! 		glTexCoord2f(0, 1);
! 		glVertex3f(-128.f, -128.f, -128.f);
! 		glEnd();
  
  		// south
! 		tex = FMaterial::ValidateTexture(sb->faces[2]);
! 		tex->Bind(CM_Index, GLT_CLAMPX|GLT_CLAMPY, 0);
  		gl_RenderState.Apply();
! 		glBegin(GL_TRIANGLE_FAN);
! 		glTexCoord2f(0, 0);
! 		glVertex3f(-128.f, 128.f, 128.f);
! 		glTexCoord2f(1, 0);
! 		glVertex3f(128.f, 128.f, 128.f);
! 		glTexCoord2f(1, 1);
! 		glVertex3f(128.f, -128.f, 128.f);
! 		glTexCoord2f(0, 1);
! 		glVertex3f(-128.f, -128.f, 128.f);
! 		glEnd();
  
  		// west
! 		tex = FMaterial::ValidateTexture(sb->faces[3]);
! 		tex->Bind(CM_Index, GLT_CLAMPX|GLT_CLAMPY, 0);
  		gl_RenderState.Apply();
! 		glBegin(GL_TRIANGLE_FAN);
! 		glTexCoord2f(0, 0);
! 		glVertex3f(128.f, 128.f, 128.f);
! 		glTexCoord2f(1, 0);
! 		glVertex3f(128.f, 128.f, -128.f);
! 		glTexCoord2f(1, 1);
! 		glVertex3f(128.f, -128.f, -128.f);
! 		glTexCoord2f(0, 1);
! 		glVertex3f(128.f, -128.f, 128.f);
! 		glEnd();
  	}
  	else 
  	{
  		faces=1;
+ 		// all 4 sides
+ 		tex = FMaterial::ValidateTexture(sb->faces[0]);
+ 		tex->Bind(CM_Index, GLT_CLAMPX|GLT_CLAMPY, 0);
  
! 		gl_RenderState.Apply();
! 		glBegin(GL_TRIANGLE_FAN);
! 		glTexCoord2f(0, 0);
! 		glVertex3f(128.f, 128.f, -128.f);
! 		glTexCoord2f(.25f, 0);
! 		glVertex3f(-128.f, 128.f, -128.f);
! 		glTexCoord2f(.25f, 1);
! 		glVertex3f(-128.f, -128.f, -128.f);
! 		glTexCoord2f(0, 1);
! 		glVertex3f(128.f, -128.f, -128.f);
! 		glEnd();
! 
! 		// east
! 		glBegin(GL_TRIANGLE_FAN);
! 		glTexCoord2f(.25f, 0);
! 		glVertex3f(-128.f, 128.f, -128.f);
! 		glTexCoord2f(.5f, 0);
! 		glVertex3f(-128.f, 128.f, 128.f);
! 		glTexCoord2f(.5f, 1);
! 		glVertex3f(-128.f, -128.f, 128.f);
! 		glTexCoord2f(.25f, 1);
! 		glVertex3f(-128.f, -128.f, -128.f);
! 		glEnd();
! 
! 		// south
! 		glBegin(GL_TRIANGLE_FAN);
! 		glTexCoord2f(.5f, 0);
! 		glVertex3f(-128.f, 128.f, 128.f);
! 		glTexCoord2f(.75f, 0);
! 		glVertex3f(128.f, 128.f, 128.f);
! 		glTexCoord2f(.75f, 1);
! 		glVertex3f(128.f, -128.f, 128.f);
! 		glTexCoord2f(.5f, 1);
! 		glVertex3f(-128.f, -128.f, 128.f);
! 		glEnd();
! 
! 		// west
! 		glBegin(GL_TRIANGLE_FAN);
! 		glTexCoord2f(.75f, 0);
! 		glVertex3f(128.f, 128.f, 128.f);
! 		glTexCoord2f(1, 0);
! 		glVertex3f(128.f, 128.f, -128.f);
! 		glTexCoord2f(1, 1);
! 		glVertex3f(128.f, -128.f, -128.f);
! 		glTexCoord2f(.75f, 1);
! 		glVertex3f(128.f, -128.f, 128.f);
! 		glEnd();
  	}
  
  	// top
! 	tex = FMaterial::ValidateTexture(sb->faces[faces]);
! 	tex->Bind(CM_Index, GLT_CLAMPX|GLT_CLAMPY, 0);
  	gl_RenderState.Apply();
+ 	glBegin(GL_TRIANGLE_FAN);
+ 	if (!sb->fliptop)
+ 	{
+ 		glTexCoord2f(0, 0);
+ 		glVertex3f(128.f, 128.f, -128.f);
+ 		glTexCoord2f(1, 0);
+ 		glVertex3f(-128.f, 128.f, -128.f);
+ 		glTexCoord2f(1, 1);
+ 		glVertex3f(-128.f, 128.f, 128.f);
+ 		glTexCoord2f(0, 1);
+ 		glVertex3f(128.f, 128.f, 128.f);
+ 	}
+ 	else
+ 	{
+ 		glTexCoord2f(0, 0);
+ 		glVertex3f(128.f, 128.f, 128.f);
+ 		glTexCoord2f(1, 0);
+ 		glVertex3f(-128.f, 128.f, 128.f);
+ 		glTexCoord2f(1, 1);
+ 		glVertex3f(-128.f, 128.f, -128.f);
+ 		glTexCoord2f(0, 1);
+ 		glVertex3f(128.f, 128.f, -128.f);
+ 	}
+ 	glEnd();
  
  
  	// bottom
! 	tex = FMaterial::ValidateTexture(sb->faces[faces+1]);
! 	tex->Bind(CM_Index, GLT_CLAMPX|GLT_CLAMPY, 0);
  	gl_RenderState.Apply();
+ 	glBegin(GL_TRIANGLE_FAN);
+ 	glTexCoord2f(0, 0);
+ 	glVertex3f(128.f, -128.f, -128.f);
+ 	glTexCoord2f(1, 0);
+ 	glVertex3f(-128.f, -128.f, -128.f);
+ 	glTexCoord2f(1, 1);
+ 	glVertex3f(-128.f, -128.f, 128.f);
+ 	glTexCoord2f(0, 1);
+ 	glVertex3f(128.f, -128.f, 128.f);
+ 	glEnd();
+ 
  
  }
  
  //-----------------------------------------------------------------------------
***************
*** 465,523 ****
  void GLSkyPortal::DrawContents()
  {
  	bool drawBoth = false;
  
  	// We have no use for Doom lighting special handling here, so disable it for this function.
  	int oldlightmode = glset.lightmode;
  	if (glset.lightmode == 8) glset.lightmode = 2;
  
  
! 	gl_RenderState.ResetColor();
  	gl_RenderState.EnableFog(false);
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, 0.f);
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
! 	gl_MatrixStack.Push(gl_RenderState.mViewMatrix);
  	GLRenderer->SetupView(0, 0, 0, viewangle, !!(MirrorFlag&1), !!(PlaneMirrorFlag&1));
  
  	if (origin->texture[0] && origin->texture[0]->tex->gl_info.bSkybox)
  	{
! 		RenderBox(origin->skytexno1, origin->texture[0], origin->x_offset[0], origin->sky2);
  	}
  	else
  	{
- 		gl_RenderState.SetVertexBuffer(GLRenderer->mSkyVBO);
  		if (origin->texture[0]==origin->texture[1] && origin->doublesky) origin->doublesky=false;	
  
  		if (origin->texture[0])
  		{
  			gl_RenderState.SetTextureMode(TM_OPAQUE);
! 			RenderDome(origin->texture[0], origin->x_offset[0], origin->y_offset, origin->mirrored, FSkyVertexBuffer::SKYMODE_MAINLAYER);
  			gl_RenderState.SetTextureMode(TM_MODULATE);
  		}
  		
! 		gl_RenderState.AlphaFunc(GL_GEQUAL, 0.05f);
  		
  		if (origin->doublesky && origin->texture[1])
  		{
! 			RenderDome(origin->texture[1], origin->x_offset[1], origin->y_offset, false, FSkyVertexBuffer::SKYMODE_SECONDLAYER);
  		}
  
! 		if (skyfog>0 && gl_fixedcolormap == CM_DEFAULT && (origin->fadecolor & 0xffffff) != 0)
  		{
- 			PalEntry FadeColor = origin->fadecolor;
- 			FadeColor.a = clamp<int>(skyfog, 0, 255);
- 
  			gl_RenderState.EnableTexture(false);
! 			gl_RenderState.SetObjectColor(FadeColor);
! 			gl_RenderState.Apply();
! 			glDrawArrays(GL_TRIANGLES, 0, 12);
  			gl_RenderState.EnableTexture(true);
! 			gl_RenderState.SetObjectColor(0xffffffff);
  		}
- 		gl_RenderState.SetVertexBuffer(GLRenderer->mVBO);
  	}
! 	gl_MatrixStack.Pop(gl_RenderState.mViewMatrix);
! 	gl_RenderState.ApplyMatrices();
  	glset.lightmode = oldlightmode;
  }
  
--- 535,614 ----
  void GLSkyPortal::DrawContents()
  {
  	bool drawBoth = false;
+ 	int CM_Index;
+ 	PalEntry FadeColor(0,0,0,0);
  
  	// We have no use for Doom lighting special handling here, so disable it for this function.
  	int oldlightmode = glset.lightmode;
  	if (glset.lightmode == 8) glset.lightmode = 2;
  
  
! 	if (gl_fixedcolormap) 
! 	{
! 		CM_Index=gl_fixedcolormap<CM_FIRSTSPECIALCOLORMAP + SpecialColormaps.Size()? gl_fixedcolormap:CM_DEFAULT;
! 	}
! 	else 
! 	{
! 		CM_Index=CM_DEFAULT;
! 		FadeColor=origin->fadecolor;
! 	}
! 
  	gl_RenderState.EnableFog(false);
! 	gl_RenderState.EnableAlphaTest(false);
  	gl_RenderState.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
! 	glMatrixMode(GL_MODELVIEW);
! 	glPushMatrix();
  	GLRenderer->SetupView(0, 0, 0, viewangle, !!(MirrorFlag&1), !!(PlaneMirrorFlag&1));
  
  	if (origin->texture[0] && origin->texture[0]->tex->gl_info.bSkybox)
  	{
! 		if (gl_fixedcolormap != CM_DEFAULT)
! 		{						
! 			float rr,gg,bb;
! 
! 			gl_GetLightColor(255, 0, NULL, &rr, &gg, &bb);
! 			R=rr;
! 			G=gg;
! 			B=bb;
! 		}
! 		else R=G=B=1.f;
! 
! 		RenderBox(origin->skytexno1, origin->texture[0], origin->x_offset[0], CM_Index, origin->sky2);
! 		gl_RenderState.EnableAlphaTest(true);
  	}
  	else
  	{
  		if (origin->texture[0]==origin->texture[1] && origin->doublesky) origin->doublesky=false;	
  
  		if (origin->texture[0])
  		{
  			gl_RenderState.SetTextureMode(TM_OPAQUE);
! 			RenderDome(origin->skytexno1, origin->texture[0], origin->x_offset[0], origin->y_offset, origin->mirrored, CM_Index);
  			gl_RenderState.SetTextureMode(TM_MODULATE);
  		}
  		
! 		gl_RenderState.EnableAlphaTest(true);
! 		gl_RenderState.AlphaFunc(GL_GEQUAL,0.05f);
  		
  		if (origin->doublesky && origin->texture[1])
  		{
! 			secondlayer=true;
! 			RenderDome(FNullTextureID(), origin->texture[1], origin->x_offset[1], origin->y_offset, false, CM_Index);
! 			secondlayer=false;
  		}
  
! 		if (skyfog>0 && (FadeColor.r ||FadeColor.g || FadeColor.b))
  		{
  			gl_RenderState.EnableTexture(false);
! 			foglayer=true;
! 			glColor4f(FadeColor.r/255.0f,FadeColor.g/255.0f,FadeColor.b/255.0f,skyfog/255.0f);
! 			RenderDome(FNullTextureID(), NULL, 0, 0, false, CM_DEFAULT);
  			gl_RenderState.EnableTexture(true);
! 			foglayer=false;
  		}
  	}
! 	glPopMatrix();
  	glset.lightmode = oldlightmode;
  }
  
