*** doom/src/main/jni/gzdoom/src/gl/models/gl_models.cpp	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/models/gl_models.cpp	2017-06-18 23:34:13.363915506 -0400
***************
*** 69,74 ****
--- 68,74 ----
  CVAR(Bool, gl_interpolate_model_frames, true, CVAR_ARCHIVE)
  CVAR(Bool, gl_light_models, true, CVAR_ARCHIVE)
  EXTERN_CVAR(Int, gl_fogmode)
+ EXTERN_CVAR(Bool, gl_dynlight_shader)
  
  extern TDeletingArray<FVoxel *> Voxels;
  extern TDeletingArray<FVoxelDef *> VoxelDefs;
***************
*** 92,156 ****
  
  DeletingModelArray Models;
  
- 
- //===========================================================================
- //
- //
- //
- //===========================================================================
- 
- FModelVertexBuffer::FModelVertexBuffer()
- {
- 	ibo_id = 0;
- 	glGenBuffers(1, &ibo_id);
- 	//for (unsigned i = 1; i < Models.Size(); i++)
- 	for (int i = Models.Size() - 1; i >= 0; i--)
- 	{
- 		Models[i]->BuildVertexBuffer(this);
- 	}
- 
- 	glBindVertexArray(vao_id);
- 
- 	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
- 	glBufferData(GL_ARRAY_BUFFER,vbo_shadowdata.Size() * sizeof(FModelVertex), &vbo_shadowdata[0], GL_STATIC_DRAW);
- 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id);
- 	glBufferData(GL_ELEMENT_ARRAY_BUFFER,ibo_shadowdata.Size() * sizeof(unsigned int), &ibo_shadowdata[0], GL_STATIC_DRAW);
- 
- 	glEnableVertexAttribArray(VATTR_VERTEX);
- 	glEnableVertexAttribArray(VATTR_TEXCOORD);
- 	glEnableVertexAttribArray(VATTR_VERTEX2);
- 	glBindVertexArray(0);
- }
- 
- FModelVertexBuffer::~FModelVertexBuffer()
- {
- 	if (ibo_id != 0)
- 	{
- 		glDeleteBuffers(1, &ibo_id);
- 	}
- }
- 
- 
- //===========================================================================
- //
- // Sets up the buffer starts for frame interpolation
- // This must be called after gl_RenderState.Apply!
- //
- //===========================================================================
- 
- unsigned int FModelVertexBuffer::SetupFrame(unsigned int frame1, unsigned int frame2, float factor)
- {
- 	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
- 	glVertexAttribPointer(VATTR_VERTEX, 3, GL_FLOAT, false, sizeof(FModelVertex), &VMO[frame1].x);
- 	glVertexAttribPointer(VATTR_TEXCOORD, 2, GL_FLOAT, false, sizeof(FModelVertex), &VMO[frame1].u);
- 	glVertexAttribPointer(VATTR_VERTEX2, 3, GL_FLOAT, false, sizeof(FModelVertex), &VMO[frame2].x);
- 	return frame1;
- }
- 
- 
- 
- 
- 
  static TArray<FSpriteModelFrame> SpriteModelFrames;
  static int * SpriteModelHash;
  //TArray<FStateModelFrame> StateModelFrames;
--- 92,97 ----
***************
*** 678,683 ****
--- 619,625 ----
  						   const FState *curState,
  						   const int curTics,
  						   const PClass *ti,
+ 						   int cm,
  						   Matrix3x4 *normaltransform,
  						   int translation)
  {
***************
*** 734,752 ****
  
  		if (mdl!=NULL)
  		{
- 			gl_RenderState.SetVertexBuffer(GLRenderer->mModelVBO);
- 
  			if ( smfNext && smf->modelframes[i] != smfNext->modelframes[i] )
! 				mdl->RenderFrame(smf->skins[i], smf->modelframes[i], smfNext->modelframes[i], inter, translation);
  			else
! 				mdl->RenderFrame(smf->skins[i], smf->modelframes[i], smf->modelframes[i], 0.f, translation);
! 
! 			gl_RenderState.SetVertexBuffer(GLRenderer->mVBO);
  		}
  	}
  }
  
! void gl_RenderModel(GLSprite * spr)
  {
  	FSpriteModelFrame * smf = spr->modelframe;
  
--- 676,690 ----
  
  		if (mdl!=NULL)
  		{
  			if ( smfNext && smf->modelframes[i] != smfNext->modelframes[i] )
! 				mdl->RenderFrameInterpolated(smf->skins[i], smf->modelframes[i], smfNext->modelframes[i], inter, cm, translation);
  			else
! 				mdl->RenderFrame(smf->skins[i], smf->modelframes[i], cm, translation);
  		}
  	}
  }
  
! void gl_RenderModel(GLSprite * spr, int cm)
  {
  	FSpriteModelFrame * smf = spr->modelframe;
  
***************
*** 793,799 ****
  		{
  			if((x * cos(angle * M_PI / 180) + y * sin(angle * M_PI / 180)) / sqrt(x * x + y * y) < 0) pitch *= -1;
  		}
! 		else pitch = fabs(pitch);
  	}
  
  	if( smf->flags & MDL_ROTATING )
--- 731,737 ----
  		{
  			if((x * cos(angle * M_PI / 180) + y * sin(angle * M_PI / 180)) / sqrt(x * x + y * y) < 0) pitch *= -1;
  		}
! 		else pitch = abs(pitch);
  	}
  
  	if( smf->flags & MDL_ROTATING )
***************
*** 807,847 ****
  	// This is rather crappy way to transfer fixet_t type into angle in degrees, but its works!
  	if(smf->flags & MDL_INHERITACTORPITCH) pitch += float(static_cast<double>(spr->actor->pitch >> 16) / (1 << 13) * 45 + static_cast<double>(spr->actor->pitch & 0x0000FFFF) / (1 << 29) * 45);
  	if(smf->flags & MDL_INHERITACTORROLL) roll += float(static_cast<double>(spr->actor->roll >> 16) / (1 << 13) * 45 + static_cast<double>(spr->actor->roll & 0x0000FFFF) / (1 << 29) * 45);
! 
! 	gl_RenderState.mModelMatrix.loadIdentity();
  
  	// Model space => World space
! 	gl_RenderState.mModelMatrix.translate(spr->x, spr->z, spr->y );	
  	
  	// Applying model transformations:
  	// 1) Applying actor angle, pitch and roll to the model
! 	gl_RenderState.mModelMatrix.rotate(-angle, 0, 1, 0);
! 	gl_RenderState.mModelMatrix.rotate(pitch, 0, 0, 1);
! 	gl_RenderState.mModelMatrix.rotate(-roll, 1, 0, 0);
  	
  	// 2) Applying Doomsday like rotation of the weapon pickup models
  	// The rotation angle is based on the elapsed time.
  	
  	if( smf->flags & MDL_ROTATING )
  	{
! 		gl_RenderState.mModelMatrix.translate(smf->rotationCenterX, smf->rotationCenterY, smf->rotationCenterZ);
! 		gl_RenderState.mModelMatrix.rotate(rotateOffset, smf->xrotate, smf->yrotate, smf->zrotate);
! 		gl_RenderState.mModelMatrix.translate(-smf->rotationCenterX, -smf->rotationCenterY, -smf->rotationCenterZ);
  	}
  
  	// 3) Scaling model.
! 	gl_RenderState.mModelMatrix.scale(scaleFactorX, scaleFactorZ, scaleFactorY);
  
  	// 4) Aplying model offsets (model offsets do not depend on model scalings).
! 	gl_RenderState.mModelMatrix.translate(smf->xoffset / smf->xscale, smf->zoffset / smf->zscale, smf->yoffset / smf->yscale);
  	
  	// 5) Applying model rotations.
! 	gl_RenderState.mModelMatrix.rotate(-ANGLE_TO_FLOAT(smf->angleoffset), 0, 1, 0);
! 	gl_RenderState.mModelMatrix.rotate(smf->pitchoffset, 0, 0, 1);
! 	gl_RenderState.mModelMatrix.rotate(-smf->rolloffset, 1, 0, 0);
! 	gl_RenderState.EnableModelMatrix(true);
! 	gl_RenderFrameModels( smf, spr->actor->state, spr->actor->tics, RUNTIME_TYPE(spr->actor), NULL, translation );
! 	gl_RenderState.EnableModelMatrix(false);
  
  	glDepthFunc(GL_LESS);
  	if (!( spr->actor->RenderStyle == LegacyRenderStyles[STYLE_Normal] ))
--- 745,825 ----
  	// This is rather crappy way to transfer fixet_t type into angle in degrees, but its works!
  	if(smf->flags & MDL_INHERITACTORPITCH) pitch += float(static_cast<double>(spr->actor->pitch >> 16) / (1 << 13) * 45 + static_cast<double>(spr->actor->pitch & 0x0000FFFF) / (1 << 29) * 45);
  	if(smf->flags & MDL_INHERITACTORROLL) roll += float(static_cast<double>(spr->actor->roll >> 16) / (1 << 13) * 45 + static_cast<double>(spr->actor->roll & 0x0000FFFF) / (1 << 29) * 45);
! 		
! 	if (gl.shadermodel < 4)
! 	{
! 		glMatrixMode(GL_MODELVIEW);
! 		glPushMatrix();
! 	}
! 	else
! 	{
! 		glActiveTexture(GL_TEXTURE7);	// Hijack the otherwise unused seventh texture matrix for the model to world transformation.
! 		glMatrixMode(GL_TEXTURE);
! 		glLoadIdentity();
! 	}
  
  	// Model space => World space
! 	glTranslatef(spr->x, spr->z, spr->y );	
  	
  	// Applying model transformations:
  	// 1) Applying actor angle, pitch and roll to the model
! 	glRotatef(-angle, 0, 1, 0);
! 	glRotatef(pitch, 0, 0, 1);
! 	glRotatef(-roll, 1, 0, 0);
  	
  	// 2) Applying Doomsday like rotation of the weapon pickup models
  	// The rotation angle is based on the elapsed time.
  	
  	if( smf->flags & MDL_ROTATING )
  	{
! 		glTranslatef(smf->rotationCenterX, smf->rotationCenterY, smf->rotationCenterZ);
! 		glRotatef(rotateOffset, smf->xrotate, smf->yrotate, smf->zrotate);
! 		glTranslatef(-smf->rotationCenterX, -smf->rotationCenterY, -smf->rotationCenterZ);
  	}
  
  	// 3) Scaling model.
! 	glScalef(scaleFactorX, scaleFactorZ, scaleFactorY);
  
  	// 4) Aplying model offsets (model offsets do not depend on model scalings).
! 	glTranslatef(smf->xoffset / smf->xscale, smf->zoffset / smf->zscale, smf->yoffset / smf->yscale);
  	
  	// 5) Applying model rotations.
! 	glRotatef(-ANGLE_TO_FLOAT(smf->angleoffset), 0, 1, 0);
! 	glRotatef(smf->pitchoffset, 0, 0, 1);
! 	glRotatef(-smf->rolloffset, 1, 0, 0);
! 		
! 	if (gl.shadermodel >= 4) glActiveTexture(GL_TEXTURE0);
! 
! #if 0
! 	if (gl_light_models)
! 	{
! 		// The normal transform matrix only contains the inverse rotations and scalings but not the translations
! 		NormalTransform.MakeIdentity();
! 
! 		NormalTransform.Scale(1.f/scaleFactorX, 1.f/scaleFactorZ, 1.f/scaleFactorY);
! 		if( smf->flags & MDL_ROTATING ) NormalTransform.Rotate(smf->xrotate, smf->yrotate, smf->zrotate, -rotateOffset);
! 		if (pitch != 0) NormalTransform.Rotate(0,0,1,-pitch);
! 		if (angle != 0) NormalTransform.Rotate(0,1,0, angle);
! 
! 		gl_RenderFrameModels( smf, spr->actor->state, spr->actor->tics, RUNTIME_TYPE(spr->actor), cm, &ModelToWorld, &NormalTransform, translation );
! 	}
! #endif
! 
! 	gl_RenderFrameModels( smf, spr->actor->state, spr->actor->tics, RUNTIME_TYPE(spr->actor), cm, NULL, translation );
! 
! 	if (gl.shadermodel < 4)
! 	{
! 		glMatrixMode(GL_MODELVIEW);
! 		glPopMatrix();
! 	}
! 	else
! 	{
! 		glActiveTexture(GL_TEXTURE7);
! 		glMatrixMode(GL_TEXTURE);
! 		glLoadIdentity();
! 		glActiveTexture(GL_TEXTURE0);
! 		glMatrixMode(GL_MODELVIEW);
! 	}
  
  	glDepthFunc(GL_LESS);
  	if (!( spr->actor->RenderStyle == LegacyRenderStyles[STYLE_Normal] ))
***************
*** 855,861 ****
  //
  //===========================================================================
  
! void gl_RenderHUDModel(pspdef_t *psp, fixed_t ofsx, fixed_t ofsy)
  {
  	AActor * playermo=players[consoleplayer].camera;
  	FSpriteModelFrame *smf = gl_FindModelFrame(playermo->player->ReadyWeapon->GetClass(), psp->state->sprite, psp->state->GetFrame(), false);
--- 833,839 ----
  //
  //===========================================================================
  
! void gl_RenderHUDModel(pspdef_t *psp, fixed_t ofsx, fixed_t ofsy, int cm)
  {
  	AActor * playermo=players[consoleplayer].camera;
  	FSpriteModelFrame *smf = gl_FindModelFrame(playermo->player->ReadyWeapon->GetClass(), psp->state->sprite, psp->state->GetFrame(), false);
***************
*** 864,869 ****
--- 842,852 ----
  	if ( smf == NULL )
  		return;
  
+ 	// [BB] The model has to be drawn independtly from the position of the player,
+ 	// so we have to reset the GL_MODELVIEW matrix.
+ 	glMatrixMode(GL_MODELVIEW);
+ 	glPushMatrix();
+ 	glLoadIdentity();
  	glDepthFunc(GL_LEQUAL);
  
  	// [BB] In case the model should be rendered translucent, do back face culling.
***************
*** 875,905 ****
  		glFrontFace(GL_CCW);
  	}
  
- 	// [BB] The model has to be drawn independently from the position of the player,
- 	// so we have to reset the view matrix.
- 	gl_RenderState.mViewMatrix.loadIdentity();
- 
  	// Scaling model (y scale for a sprite means height, i.e. z in the world!).
! 	gl_RenderState.mViewMatrix.scale(smf->xscale, smf->zscale, smf->yscale);
  	
  	// Aplying model offsets (model offsets do not depend on model scalings).
! 	gl_RenderState.mViewMatrix.translate(smf->xoffset / smf->xscale, smf->zoffset / smf->zscale, smf->yoffset / smf->yscale);
  
  	// [BB] Weapon bob, very similar to the normal Doom weapon bob.
! 	gl_RenderState.mViewMatrix.rotate(FIXED2FLOAT(ofsx)/4, 0, 1, 0);
! 	gl_RenderState.mViewMatrix.rotate(-FIXED2FLOAT(ofsy-WEAPONTOP)/4, 1, 0, 0);
  
  	// [BB] For some reason the jDoom models need to be rotated.
! 	gl_RenderState.mViewMatrix.rotate(90.f, 0, 1, 0);
  
  	// Applying angleoffset, pitchoffset, rolloffset.
! 	gl_RenderState.mViewMatrix.rotate(-ANGLE_TO_FLOAT(smf->angleoffset), 0, 1, 0);
! 	gl_RenderState.mViewMatrix.rotate(smf->pitchoffset, 0, 0, 1);
! 	gl_RenderState.mViewMatrix.rotate(-smf->rolloffset, 1, 0, 0);
! 	gl_RenderState.ApplyMatrices();
  
! 	gl_RenderFrameModels( smf, psp->state, psp->tics, playermo->player->ReadyWeapon->GetClass(), NULL, 0 );
  
  	glDepthFunc(GL_LESS);
  	if (!( playermo->RenderStyle == LegacyRenderStyles[STYLE_Normal] ))
  		glDisable(GL_CULL_FACE);
--- 858,885 ----
  		glFrontFace(GL_CCW);
  	}
  
  	// Scaling model (y scale for a sprite means height, i.e. z in the world!).
! 	glScalef(smf->xscale, smf->zscale, smf->yscale);
  	
  	// Aplying model offsets (model offsets do not depend on model scalings).
! 	glTranslatef(smf->xoffset / smf->xscale, smf->zoffset / smf->zscale, smf->yoffset / smf->yscale);
  
  	// [BB] Weapon bob, very similar to the normal Doom weapon bob.
! 	glRotatef(FIXED2FLOAT(ofsx)/4, 0, 1, 0);
! 	glRotatef(-FIXED2FLOAT(ofsy-WEAPONTOP)/4, 1, 0, 0);
  
  	// [BB] For some reason the jDoom models need to be rotated.
! 	glRotatef(90., 0, 1, 0);
  
  	// Applying angleoffset, pitchoffset, rolloffset.
! 	glRotatef(-ANGLE_TO_FLOAT(smf->angleoffset), 0, 1, 0);
! 	glRotatef(smf->pitchoffset, 0, 0, 1);
! 	glRotatef(-smf->rolloffset, 1, 0, 0);
  
! 	gl_RenderFrameModels( smf, psp->state, psp->tics, playermo->player->ReadyWeapon->GetClass(), cm, NULL, 0 );
  
+ 	glMatrixMode(GL_MODELVIEW);
+ 	glPopMatrix();
  	glDepthFunc(GL_LESS);
  	if (!( playermo->RenderStyle == LegacyRenderStyles[STYLE_Normal] ))
  		glDisable(GL_CULL_FACE);
***************
*** 921,923 ****
--- 901,916 ----
  	return ( smf != NULL );
  }
  
+ //===========================================================================
+ //
+ // gl_CleanModelData
+ //
+ //===========================================================================
+ 
+ void gl_CleanModelData()
+ {
+ 	for (unsigned i=0;i<Models.Size(); i++)
+ 	{
+ 		Models[i]->CleanGLData();
+ 	}
+ }
