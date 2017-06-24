*** doom/src/main/jni/gzdoom/src/gl/scene/gl_portal.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_portal.cpp	2017-06-18 23:34:13.307248315 -0400
***************
*** 79,85 ****
  EXTERN_CVAR(Int, r_mirror_recursions)
  
  TArray<GLPortal *> GLPortal::portals;
- TArray<float> GLPortal::planestack;
  int GLPortal::recursion;
  int GLPortal::MirrorFlag;
  int GLPortal::PlaneMirrorFlag;
--- 77,82 ----
***************
*** 117,130 ****
  void GLPortal::ClearScreen()
  {
  	bool multi = !!glIsEnabled(GL_MULTISAMPLE);
! 	gl_MatrixStack.Push(gl_RenderState.mViewMatrix);
! 	gl_MatrixStack.Push(gl_RenderState.mProjectionMatrix);
  	screen->Begin2D(false);
  	screen->Dim(0, 1.f, 0, 0, SCREENWIDTH, SCREENHEIGHT);
  	glEnable(GL_DEPTH_TEST);
! 	gl_MatrixStack.Pop(gl_RenderState.mProjectionMatrix);
! 	gl_MatrixStack.Pop(gl_RenderState.mViewMatrix);
! 	gl_RenderState.ApplyMatrices();
  	if (multi) glEnable(GL_MULTISAMPLE);
  	gl_RenderState.Set2DMode(false);
  }
--- 114,130 ----
  void GLPortal::ClearScreen()
  {
  	bool multi = !!glIsEnabled(GL_MULTISAMPLE);
! 	glMatrixMode(GL_MODELVIEW);
! 	glPushMatrix();
! 	glMatrixMode(GL_PROJECTION);
! 	glPushMatrix();
  	screen->Begin2D(false);
  	screen->Dim(0, 1.f, 0, 0, SCREENWIDTH, SCREENHEIGHT);
  	glEnable(GL_DEPTH_TEST);
! 	glMatrixMode(GL_PROJECTION);
! 	glPopMatrix();
! 	glMatrixMode(GL_MODELVIEW);
! 	glPopMatrix();
  	if (multi) glEnable(GL_MULTISAMPLE);
  	gl_RenderState.Set2DMode(false);
  }
***************
*** 137,181 ****
  //-----------------------------------------------------------------------------
  void GLPortal::DrawPortalStencil()
  {
! 	if (mPrimIndices.Size() == 0)
  	{
! 		bool cap = NeedCap() && lines.Size() > 1;
! 		mPrimIndices.Resize(2 * lines.Size() + 4 * cap);
  
- 		for (unsigned int i = 0; i<lines.Size(); i++)
- 		{
- 			lines[i].RenderWall(GLWall::RWF_NORENDER, &mPrimIndices[i * 2]);
- 		}
- 
- 		if (cap)
- 		{
- 			// Cap the stencil at the top and bottom
- 			int n = lines.Size() * 2;
- 			FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
- 			ptr->Set(-32767.0f, 32767.0f, -32767.0f, 0, 0);
- 			ptr->Set(-32767.0f, 32767.0f, 32767.0f, 0, 0);
- 			ptr->Set(32767.0f, 32767.0f, 32767.0f, 0, 0);
- 			ptr->Set(32767.0f, 32767.0f, -32767.0f, 0, 0);
- 			mPrimIndices[n + 1] = GLRenderer->mVBO->GetCount(ptr, &mPrimIndices[n]);
- 			ptr->Set(-32767.0f, -32767.0f, -32767.0f, 0, 0);
- 			ptr->Set(-32767.0f, -32767.0f, 32767.0f, 0, 0);
- 			ptr->Set(32767.0f, -32767.0f, 32767.0f, 0, 0);
- 			ptr->Set(32767.0f, -32767.0f, -32767.0f, 0, 0);
- 			mPrimIndices[n + 3] = GLRenderer->mVBO->GetCount(ptr, &mPrimIndices[n + 2]);
- 		}
  	}
! 	gl_RenderState.Apply();
! 	for (unsigned int i = 0; i < mPrimIndices.Size(); i += 2)
  	{
! 		GLRenderer->mVBO->RenderArray(GL_TRIANGLE_FAN, mPrimIndices[i], mPrimIndices[i + 1]);
  	}
  }
  
  
  
- 
- 
- 
  //-----------------------------------------------------------------------------
  //
  // Start
--- 137,169 ----
  //-----------------------------------------------------------------------------
  void GLPortal::DrawPortalStencil()
  {
! 	for(unsigned int i=0;i<lines.Size();i++)
  	{
! 		lines[i].RenderWall(0, NULL);
  
  	}
! 
! 	if (NeedCap() && lines.Size() > 1)
  	{
! 		// Cap the stencil at the top and bottom 
! 		// (cheap ass version)
! 		glBegin(GL_TRIANGLE_FAN);
! 		glVertex3f(-32767.0f,32767.0f,-32767.0f);
! 		glVertex3f(-32767.0f,32767.0f, 32767.0f);
! 		glVertex3f( 32767.0f,32767.0f, 32767.0f);
! 		glVertex3f( 32767.0f,32767.0f,-32767.0f);
! 		glEnd();
! 		glBegin(GL_TRIANGLE_FAN);
! 		glVertex3f(-32767.0f,-32767.0f,-32767.0f);
! 		glVertex3f(-32767.0f,-32767.0f, 32767.0f);
! 		glVertex3f( 32767.0f,-32767.0f, 32767.0f);
! 		glVertex3f( 32767.0f,-32767.0f,-32767.0f);
! 		glEnd();
  	}
  }
  
  
  
  //-----------------------------------------------------------------------------
  //
  // Start
***************
*** 198,206 ****
  		glStencilFunc(GL_EQUAL,recursion,~0);		// create stencil
  		glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);		// increment stencil of valid pixels
  		glColorMask(0,0,0,0);						// don't write to the graphics buffer
- 		gl_RenderState.SetEffect(EFF_STENCIL);
  		gl_RenderState.EnableTexture(false);
! 		gl_RenderState.ResetColor();
  		glDepthFunc(GL_LESS);
  		gl_RenderState.Apply();
  
--- 186,193 ----
  		glStencilFunc(GL_EQUAL,recursion,~0);		// create stencil
  		glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);		// increment stencil of valid pixels
  		glColorMask(0,0,0,0);						// don't write to the graphics buffer
  		gl_RenderState.EnableTexture(false);
! 		glColor3f(1,1,1);
  		glDepthFunc(GL_LESS);
  		gl_RenderState.Apply();
  
***************
*** 211,226 ****
  			else if (gl_noquery) doquery = false;
  			
  			// If occlusion query is supported let's use it to avoid rendering portals that aren't visible
! 			if (!QueryObject) glGenQueries(1, &QueryObject);
! 			if (QueryObject) 
  			{
! 				glBeginQuery(GL_SAMPLES_PASSED, QueryObject);
  			}
- 			else doquery = false;	// some kind of error happened
  
  			DrawPortalStencil();
  
! 			glEndQuery(GL_SAMPLES_PASSED);
  
  			// Clear Z-buffer
  			glStencilFunc(GL_EQUAL,recursion+1,~0);		// draw sky into stencil
--- 198,220 ----
  			else if (gl_noquery) doquery = false;
  			
  			// If occlusion query is supported let's use it to avoid rendering portals that aren't visible
! 			if (doquery && gl.flags&RFL_OCCLUSION_QUERY)
  			{
! 				if (!QueryObject) glGenQueries(1, &QueryObject);
! 				if (QueryObject) 
! 				{
! 					glBeginQuery(GL_SAMPLES_PASSED_ARB, QueryObject);
! 				}
! 				else doquery = false;	// some kind of error happened
! 					
  			}
  
  			DrawPortalStencil();
  
! 			if (doquery && gl.flags&RFL_OCCLUSION_QUERY)
! 			{
! 				glEndQuery(GL_SAMPLES_PASSED_ARB);
! 			}
  
  			// Clear Z-buffer
  			glStencilFunc(GL_EQUAL,recursion+1,~0);		// draw sky into stencil
***************
*** 234,253 ****
  			gl_RenderState.EnableTexture(true);
  			glDepthFunc(GL_LESS);
  			glColorMask(1,1,1,1);
! 			gl_RenderState.SetEffect(EFF_NONE);
! 			glDepthRange(0, 1);
  
! 			GLuint sampleCount;
  
! 			glGetQueryObjectuiv(QueryObject, GL_QUERY_RESULT, &sampleCount);
  
! 			if (sampleCount==0) 	// not visible
! 			{
! 				// restore default stencil op.
! 				glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
! 				glStencilFunc(GL_EQUAL,recursion,~0);		// draw sky into stencil
! 				PortalAll.Unclock();
! 				return false;
  			}
  			FDrawInfo::StartDrawInfo();
  		}
--- 228,249 ----
  			gl_RenderState.EnableTexture(true);
  			glDepthFunc(GL_LESS);
  			glColorMask(1,1,1,1);
! 			glDepthRange(0,1);
  
! 			if (doquery && gl.flags&RFL_OCCLUSION_QUERY)
! 			{
! 				GLuint sampleCount;
  
! 				glGetQueryObjectuiv(QueryObject, GL_QUERY_RESULT_ARB, &sampleCount);
  
! 				if (sampleCount==0) 	// not visible
! 				{
! 					// restore default stencil op.
! 					glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
! 					glStencilFunc(GL_EQUAL,recursion,~0);		// draw sky into stencil
! 					PortalAll.Unclock();
! 					return false;
! 				}
  			}
  			FDrawInfo::StartDrawInfo();
  		}
***************
*** 264,270 ****
  			glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);		// this stage doesn't modify the stencil
  			gl_RenderState.EnableTexture(true);
  			glColorMask(1,1,1,1);
- 			gl_RenderState.SetEffect(EFF_NONE);
  			glDisable(GL_DEPTH_TEST);
  			glDepthMask(false);							// don't write to Z-buffer!
  		}
--- 260,265 ----
***************
*** 284,295 ****
  			glDisable(GL_DEPTH_TEST);
  		}
  	}
! 	planestack.Push(gl_RenderState.GetClipHeightTop());
! 	planestack.Push(gl_RenderState.GetClipHeightBottom());
! 	glDisable(GL_CLIP_DISTANCE0);
! 	glDisable(GL_CLIP_DISTANCE1);
! 	gl_RenderState.SetClipHeightBottom(-65536.f);
! 	gl_RenderState.SetClipHeightTop(65536.f);
  
  	// save viewpoint
  	savedviewx=viewx;
--- 279,287 ----
  			glDisable(GL_DEPTH_TEST);
  		}
  	}
! 	// The clip plane from the previous portal must be deactivated for this one.
! 	clipsave = glIsEnabled(GL_CLIP_PLANE0+renderdepth-1);
! 	if (clipsave) glDisable(GL_CLIP_PLANE0+renderdepth-1);
  
  	// save viewpoint
  	savedviewx=viewx;
***************
*** 349,363 ****
  
  	PortalAll.Clock();
  	GLRenderer->mCurrentPortal = NextPortal;
! 
! 	float f;
! 	planestack.Pop(f);
! 	gl_RenderState.SetClipHeightBottom(f);
! 	if (f > -65535.f) glEnable(GL_CLIP_DISTANCE0);
! 	planestack.Pop(f);
! 	gl_RenderState.SetClipHeightTop(f);
! 	if (f < 65535.f) glEnable(GL_CLIP_DISTANCE1);
! 
  	if (usestencil)
  	{
  		if (needdepth) FDrawInfo::EndDrawInfo();
--- 341,347 ----
  
  	PortalAll.Clock();
  	GLRenderer->mCurrentPortal = NextPortal;
! 	if (clipsave) glEnable (GL_CLIP_PLANE0+renderdepth-1);
  	if (usestencil)
  	{
  		if (needdepth) FDrawInfo::EndDrawInfo();
***************
*** 371,379 ****
  		in_area=savedviewarea;
  		GLRenderer->SetupView(viewx, viewy, viewz, viewangle, !!(MirrorFlag&1), !!(PlaneMirrorFlag&1));
  
  		glColorMask(0,0,0,0);						// no graphics
! 		gl_RenderState.SetEffect(EFF_NONE);
! 		gl_RenderState.ResetColor();
  		gl_RenderState.EnableTexture(false);
  		gl_RenderState.Apply();
  
--- 355,363 ----
  		in_area=savedviewarea;
  		GLRenderer->SetupView(viewx, viewy, viewz, viewangle, !!(MirrorFlag&1), !!(PlaneMirrorFlag&1));
  
+ 		glColor4f(1,1,1,1);
  		glColorMask(0,0,0,0);						// no graphics
! 		glColor3f(1,1,1);
  		gl_RenderState.EnableTexture(false);
  		gl_RenderState.Apply();
  
***************
*** 399,406 ****
  
  
  		gl_RenderState.EnableTexture(true);
! 		gl_RenderState.SetEffect(EFF_NONE);
! 		glColorMask(1, 1, 1, 1);
  		recursion--;
  
  		// restore old stencil op.
--- 383,389 ----
  
  
  		gl_RenderState.EnableTexture(true);
! 		glColorMask(1,1,1,1);
  		recursion--;
  
  		// restore old stencil op.
***************
*** 431,444 ****
  		// This draws a valid z-buffer into the stencil's contents to ensure it
  		// doesn't get overwritten by the level's geometry.
  
! 		gl_RenderState.ResetColor();
  		glDepthFunc(GL_LEQUAL);
  		glDepthRange(0,1);
  		glColorMask(0,0,0,0);						// no graphics
- 		gl_RenderState.SetEffect(EFF_STENCIL);
  		gl_RenderState.EnableTexture(false);
  		DrawPortalStencil();
- 		gl_RenderState.SetEffect(EFF_NONE);
  		gl_RenderState.EnableTexture(true);
  		glColorMask(1,1,1,1);
  		glDepthFunc(GL_LESS);
--- 414,425 ----
  		// This draws a valid z-buffer into the stencil's contents to ensure it
  		// doesn't get overwritten by the level's geometry.
  
! 		glColor4f(1,1,1,1);
  		glDepthFunc(GL_LEQUAL);
  		glDepthRange(0,1);
  		glColorMask(0,0,0,0);						// no graphics
  		gl_RenderState.EnableTexture(false);
  		DrawPortalStencil();
  		gl_RenderState.EnableTexture(true);
  		glColorMask(1,1,1,1);
  		glDepthFunc(GL_LESS);
***************
*** 633,639 ****
  
  	PlaneMirrorMode=0;
  
! 	glDisable(GL_DEPTH_CLAMP);
  
  	viewx = origin->PrevX + FixedMul(r_TicFrac, origin->x - origin->PrevX);
  	viewy = origin->PrevY + FixedMul(r_TicFrac, origin->y - origin->PrevY);
--- 614,620 ----
  
  	PlaneMirrorMode=0;
  
! 	glDisable(GL_DEPTH_CLAMP_NV);
  
  	viewx = origin->PrevX + FixedMul(r_TicFrac, origin->x - origin->PrevX);
  	viewy = origin->PrevY + FixedMul(r_TicFrac, origin->y - origin->PrevY);
***************
*** 663,669 ****
  	GLRenderer->DrawScene();
  	origin->flags&=~MF_JUSTHIT;
  	inskybox=false;
! 	glEnable(GL_DEPTH_CLAMP);
  	skyboxrecursion--;
  
  	PlaneMirrorMode=old_pm;
--- 644,650 ----
  	GLRenderer->DrawScene();
  	origin->flags&=~MF_JUSTHIT;
  	inskybox=false;
! 	glEnable(GL_DEPTH_CLAMP_NV);
  	skyboxrecursion--;
  
  	PlaneMirrorMode=old_pm;
***************
*** 798,820 ****
  	GLRenderer->SetupView(viewx, viewy, viewz, viewangle, !!(MirrorFlag&1), !!(PlaneMirrorFlag&1));
  	ClearClipper();
  
! 	float f = FIXED2FLOAT(planez);
! 	if (PlaneMirrorMode < 0)
! 	{
! 		gl_RenderState.SetClipHeightTop(f);	// ceiling mirror: clip everytihng with a z lower than the portal's ceiling
! 		glEnable(GL_CLIP_DISTANCE1);
! 	}
! 	else
! 	{
! 		gl_RenderState.SetClipHeightBottom(f);	// floor mirror: clip everything with a z higher than the portal's floor
! 		glEnable(GL_CLIP_DISTANCE0);
! 	}
  
  	GLRenderer->DrawScene();
! 	glDisable(GL_CLIP_DISTANCE0);
! 	glDisable(GL_CLIP_DISTANCE1);
! 	gl_RenderState.SetClipHeightBottom(-65536.f);
! 	gl_RenderState.SetClipHeightTop(65536.f);
  	PlaneMirrorFlag--;
  	PlaneMirrorMode=old_pm;
  }
--- 779,792 ----
  	GLRenderer->SetupView(viewx, viewy, viewz, viewangle, !!(MirrorFlag&1), !!(PlaneMirrorFlag&1));
  	ClearClipper();
  
! 	glEnable(GL_CLIP_PLANE0+renderdepth);
! 	// This only works properly for non-sloped planes so don't bother with the math.
! 	//double d[4]={origin->a/65536., origin->c/65536., origin->b/65536., FIXED2FLOAT(origin->d)};
! 	double d[4]={0, static_cast<double>(PlaneMirrorMode), 0, FIXED2FLOAT(origin->d)};
! 	glClipPlane(GL_CLIP_PLANE0+renderdepth, d);
  
  	GLRenderer->DrawScene();
! 	glDisable(GL_CLIP_PLANE0+renderdepth);
  	PlaneMirrorFlag--;
  	PlaneMirrorMode=old_pm;
  }
***************
*** 882,893 ****
  		// any mirror--use floats to avoid integer overflow. 
  		// Use doubles to avoid losing precision which is very important here.
  
! 		double dx = FIXED2DBL(v2->x - v1->x);
! 		double dy = FIXED2DBL(v2->y - v1->y);
! 		double x1 = FIXED2DBL(v1->x);
! 		double y1 = FIXED2DBL(v1->y);
! 		double x = FIXED2DBL(startx);
! 		double y = FIXED2DBL(starty);
  
  		// the above two cases catch len == 0
  		double r = ((x - x1)*dx + (y - y1)*dy) / (dx*dx + dy*dy);
--- 854,865 ----
  		// any mirror--use floats to avoid integer overflow. 
  		// Use doubles to avoid losing precision which is very important here.
  
! 		double dx = FIXED2FLOAT(v2->x - v1->x);
! 		double dy = FIXED2FLOAT(v2->y - v1->y);
! 		double x1 = FIXED2FLOAT(v1->x);
! 		double y1 = FIXED2FLOAT(v1->y);
! 		double x = FIXED2FLOAT(startx);
! 		double y = FIXED2FLOAT(starty);
  
  		// the above two cases catch len == 0
  		double r = ((x - x1)*dx + (y - y1)*dy) / (dx*dx + dy*dy);
***************
*** 985,991 ****
  	float z;
  	player_t * player=&players[consoleplayer];
  
! 	gltexture=FMaterial::ValidateTexture(sp->texture, false, true);
  	if (!gltexture) 
  	{
  		ClearScreen();
--- 957,963 ----
  	float z;
  	player_t * player=&players[consoleplayer];
  
! 	gltexture=FMaterial::ValidateTexture(sp->texture, true);
  	if (!gltexture) 
  	{
  		ClearScreen();
***************
*** 1000,1044 ****
  	if (gltexture && gltexture->tex->isFullbright())
  	{
  		// glowing textures are always drawn full bright without color
! 		gl_SetColor(255, 0, origin->colormap, 1.f);
  		gl_SetFog(255, 0, &origin->colormap, false);
  	}
  	else 
  	{
  		int rel = getExtraLight();
! 		gl_SetColor(origin->lightlevel, rel, origin->colormap, 1.0f);
  		gl_SetFog(origin->lightlevel, rel, &origin->colormap, false);
  	}
  
  
! 	gl_RenderState.SetMaterial(gltexture, CLAMP_NONE, 0, -1, false);
  
! 	gl_SetPlaneTextureRotation(sp, gltexture);
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, 0.f);
  	gl_RenderState.BlendFunc(GL_ONE,GL_ZERO);
  	gl_RenderState.Apply();
  
  
  
  	float vx=FIXED2FLOAT(viewx);
  	float vy=FIXED2FLOAT(viewy);
  
  	// Draw to some far away boundary
- 	// This is not drawn as larher strips because it causes visual glitches.
  	for(float x=-32768+vx; x<32768+vx; x+=4096)
  	{
  		for(float y=-32768+vy; y<32768+vy;y+=4096)
  		{
! 			FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 			ptr->Set(x, z, y, x / 64, -y / 64);
! 			ptr++;
! 			ptr->Set(x + 4096, z, y, x / 64 + 64, -y / 64);
! 			ptr++;
! 			ptr->Set(x, z, y + 4096, x / 64, -y / 64 - 64);
! 			ptr++;
! 			ptr->Set(x + 4096, z, y + 4096, x / 64 + 64, -y / 64 - 64);
! 			ptr++;
! 			GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  		}
  	}
  
--- 972,1021 ----
  	if (gltexture && gltexture->tex->isFullbright())
  	{
  		// glowing textures are always drawn full bright without color
! 		gl_SetColor(255, 0, NULL, 1.f);
  		gl_SetFog(255, 0, &origin->colormap, false);
  	}
  	else 
  	{
  		int rel = getExtraLight();
! 		gl_SetColor(origin->lightlevel, rel, &origin->colormap, 1.0f);
  		gl_SetFog(origin->lightlevel, rel, &origin->colormap, false);
  	}
  
  
! 	gltexture->Bind(origin->colormap.colormap);
  
! 	gl_RenderState.EnableAlphaTest(false);
  	gl_RenderState.BlendFunc(GL_ONE,GL_ZERO);
  	gl_RenderState.Apply();
  
  
+ 	bool pushed = gl_SetPlaneTextureRotation(sp, gltexture);
  
  	float vx=FIXED2FLOAT(viewx);
  	float vy=FIXED2FLOAT(viewy);
  
  	// Draw to some far away boundary
  	for(float x=-32768+vx; x<32768+vx; x+=4096)
  	{
  		for(float y=-32768+vy; y<32768+vy;y+=4096)
  		{
! 			glBegin(GL_TRIANGLE_FAN);
! 
! 			glTexCoord2f(x/64, -y/64);
! 			glVertex3f(x, z, y);
! 
! 			glTexCoord2f(x/64 + 64, -y/64);
! 			glVertex3f(x + 4096, z, y);
! 
! 			glTexCoord2f(x/64 + 64, -y/64 - 64);
! 			glVertex3f(x + 4096, z, y + 4096);
! 
! 			glTexCoord2f(x/64, -y/64 - 64);
! 			glVertex3f(x, z, y + 4096);
! 
! 			glEnd();
! 
  		}
  	}
  
***************
*** 1049,1078 ****
  	// Since I can't draw into infinity there can always be a
  	// small gap
  
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(-32768 + vx, z, -32768 + vy, 512.f, 0);
! 	ptr++;
! 	ptr->Set(-32768 + vx, vz, -32768 + vy, 512.f, tz);
! 	ptr++;
! 	ptr->Set(-32768 + vx, z, 32768 + vy, -512.f, 0);
! 	ptr++;
! 	ptr->Set(-32768 + vx, vz, 32768 + vy, -512.f, tz);
! 	ptr++;
! 	ptr->Set(32768 + vx, z, 32768 + vy, 512.f, 0);
! 	ptr++;
! 	ptr->Set(32768 + vx, vz, 32768 + vy, 512.f, tz);
! 	ptr++;
! 	ptr->Set(32768 + vx, z, -32768 + vy, -512.f, 0);
! 	ptr++;
! 	ptr->Set(32768 + vx, vz, -32768 + vy, -512.f, tz);
! 	ptr++;
! 	ptr->Set(-32768 + vx, z, -32768 + vy, 512.f, 0);
! 	ptr++;
! 	ptr->Set(-32768 + vx, vz, -32768 + vy, 512.f, tz);
! 	ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  
- 	gl_RenderState.EnableTextureMatrix(false);
  	PortalAll.Unclock();
  
  }
--- 1026,1066 ----
  	// Since I can't draw into infinity there can always be a
  	// small gap
  
! 	glBegin(GL_TRIANGLE_STRIP);
! 
! 	glTexCoord2f(512.f, 0);
! 	glVertex3f(-32768+vx, z, -32768+vy);
! 	glTexCoord2f(512.f, tz);
! 	glVertex3f(-32768+vx, vz, -32768+vy);
! 
! 	glTexCoord2f(-512.f, 0);
! 	glVertex3f(-32768+vx, z,  32768+vy);
! 	glTexCoord2f(-512.f, tz);
! 	glVertex3f(-32768+vx, vz,  32768+vy);
! 
! 	glTexCoord2f(512.f, 0);
! 	glVertex3f( 32768+vx, z,  32768+vy);
! 	glTexCoord2f(512.f, tz);
! 	glVertex3f( 32768+vx, vz,  32768+vy);
! 
! 	glTexCoord2f(-512.f, 0);
! 	glVertex3f( 32768+vx, z, -32768+vy);
! 	glTexCoord2f(-512.f, tz);
! 	glVertex3f( 32768+vx, vz, -32768+vy);
! 
! 	glTexCoord2f(512.f, 0);
! 	glVertex3f(-32768+vx, z, -32768+vy);
! 	glTexCoord2f(512.f, tz);
! 	glVertex3f(-32768+vx, vz, -32768+vy);
! 
! 	glEnd();
! 
! 	if (pushed)
! 	{
! 		glPopMatrix();
! 		glMatrixMode(GL_MODELVIEW);
! 	}
  
  	PortalAll.Unclock();
  
  }
