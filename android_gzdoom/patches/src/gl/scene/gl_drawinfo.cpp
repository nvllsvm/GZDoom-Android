*** doom/src/main/jni/gzdoom/src/gl/scene/gl_drawinfo.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_drawinfo.cpp	2017-06-18 23:34:13.313915044 -0400
***************
*** 47,55 ****
  
  #include "gl/system/gl_cvars.h"
  #include "gl/data/gl_data.h"
- #include "gl/data/gl_vertexbuffer.h"
  #include "gl/scene/gl_drawinfo.h"
  #include "gl/scene/gl_portal.h"
  #include "gl/renderer/gl_lightdata.h"
  #include "gl/renderer/gl_renderstate.h"
  #include "gl/textures/gl_material.h"
--- 47,55 ----
  
  #include "gl/system/gl_cvars.h"
  #include "gl/data/gl_data.h"
  #include "gl/scene/gl_drawinfo.h"
  #include "gl/scene/gl_portal.h"
+ #include "gl/dynlights/gl_lightbuffer.h"
  #include "gl/renderer/gl_lightdata.h"
  #include "gl/renderer/gl_renderstate.h"
  #include "gl/textures/gl_material.h"
***************
*** 59,64 ****
--- 59,66 ----
  
  FDrawInfo * gl_drawinfo;
  
+ CVAR(Bool, gl_sort_textures, true, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
+ 
  //==========================================================================
  //
  //
***************
*** 642,647 ****
--- 644,650 ----
  			case GLDIT_SPRITE:
  				SortSpriteIntoPlane(head,node);
  				break;
+ 			case GLDIT_POLY: break;
  			}
  			node=next;
  		}
***************
*** 667,673 ****
  				case GLDIT_SPRITE:
  					SortSpriteIntoWall(head,node);
  					break;
! 
  				case GLDIT_FLAT: break;
  				}
  				node=next;
--- 670,676 ----
  				case GLDIT_SPRITE:
  					SortSpriteIntoWall(head,node);
  					break;
! 				case GLDIT_POLY: break;
  				case GLDIT_FLAT: break;
  				}
  				node=next;
***************
*** 689,695 ****
  //
  //
  //==========================================================================
! void GLDrawList::DoDraw(int pass, int i, bool trans)
  {
  	switch(drawitems[i].rendertype)
  	{
--- 692,698 ----
  //
  //
  //==========================================================================
! void GLDrawList::DoDraw(int pass, int i)
  {
  	switch(drawitems[i].rendertype)
  	{
***************
*** 697,703 ****
  		{
  			GLFlat * f=&flats[drawitems[i].index];
  			RenderFlat.Clock();
! 			f->Draw(pass, trans);
  			RenderFlat.Unclock();
  		}
  		break;
--- 700,706 ----
  		{
  			GLFlat * f=&flats[drawitems[i].index];
  			RenderFlat.Clock();
! 			f->Draw(pass);
  			RenderFlat.Unclock();
  		}
  		break;
***************
*** 719,724 ****
--- 722,728 ----
  			RenderSprite.Unclock();
  		}
  		break;
+ 	case GLDIT_POLY: break;
  	}
  }
  
***************
*** 735,747 ****
  		{
  			DoDrawSorted(head->left);
  		}
! 		DoDraw(GLPASS_TRANSLUCENT, head->itemindex, true);
  		if (head->equal)
  		{
  			SortNode * ehead=head->equal;
  			while (ehead)
  			{
! 				DoDraw(GLPASS_TRANSLUCENT, ehead->itemindex, true);
  				ehead=ehead->equal;
  			}
  		}
--- 739,751 ----
  		{
  			DoDrawSorted(head->left);
  		}
! 		DoDraw(GLPASS_TRANSLUCENT, head->itemindex);
  		if (head->equal)
  		{
  			SortNode * ehead=head->equal;
  			while (ehead)
  			{
! 				DoDraw(GLPASS_TRANSLUCENT, ehead->itemindex);
  				ehead=ehead->equal;
  			}
  		}
***************
*** 775,824 ****
  {
  	for(unsigned i=0;i<drawitems.Size();i++)
  	{
! 		DoDraw(pass, i, false);
! 	}
! }
! 
! //==========================================================================
! //
! //
! //
! //==========================================================================
! void GLDrawList::DrawWalls(int pass)
! {
! 	RenderWall.Clock();
! 	for(unsigned i=0;i<drawitems.Size();i++)
! 	{
! 		walls[drawitems[i].index].Draw(pass);
! 	}
! 	RenderWall.Unclock();
! }
! 
! //==========================================================================
! //
! //
! //
! //==========================================================================
! void GLDrawList::DrawFlats(int pass)
! {
! 	RenderFlat.Clock();
! 	for(unsigned i=0;i<drawitems.Size();i++)
! 	{
! 		flats[drawitems[i].index].Draw(pass, false);
! 	}
! 	RenderFlat.Unclock();
! }
! 
! //==========================================================================
! //
! //
! //
! //==========================================================================
! void GLDrawList::DrawDecals()
! {
! 	for(unsigned i=0;i<drawitems.Size();i++)
! 	{
! 		walls[drawitems[i].index].DoDrawDecals();
  	}
  }
  
--- 779,785 ----
  {
  	for(unsigned i=0;i<drawitems.Size();i++)
  	{
! 		DoDraw(pass, i);
  	}
  }
  
***************
*** 829,873 ****
  //==========================================================================
  static GLDrawList * sortinfo;
  
! static int __cdecl diwcmp (const void *a, const void *b)
! {
! 	const GLDrawItem * di1 = (const GLDrawItem *)a;
! 	GLWall * w1=&sortinfo->walls[di1->index];
! 
! 	const GLDrawItem * di2 = (const GLDrawItem *)b;
! 	GLWall * w2=&sortinfo->walls[di2->index];
! 
! 	if (w1->gltexture != w2->gltexture) return w1->gltexture - w2->gltexture;
! 	return ((w1->flags & 3) - (w2->flags & 3));
! }
! 
! static int __cdecl difcmp (const void *a, const void *b)
  {
! 	const GLDrawItem * di1 = (const GLDrawItem *)a;
! 	GLFlat * w1=&sortinfo->flats[di1->index];
! 
! 	const GLDrawItem * di2 = (const GLDrawItem *)b;
! 	GLFlat* w2=&sortinfo->flats[di2->index];
! 
! 	return w1->gltexture - w2->gltexture;
! }
  
  
! void GLDrawList::SortWalls()
! {
! 	if (drawitems.Size() > 1)
! 	{
! 		sortinfo=this;
! 		qsort(&drawitems[0], drawitems.Size(), sizeof(drawitems[0]), diwcmp);
  	}
  }
  
! void GLDrawList::SortFlats()
  {
! 	if (drawitems.Size() > 1)
  	{
  		sortinfo=this;
! 		qsort(&drawitems[0], drawitems.Size(), sizeof(drawitems[0]), difcmp);
  	}
  }
  
--- 790,850 ----
  //==========================================================================
  static GLDrawList * sortinfo;
  
! static int __cdecl dicmp (const void *a, const void *b)
  {
! 	const GLDrawItem * di[2];
! 	FMaterial * tx[2];
! 	int lights[2];
! 	int clamp[2];
! 	//colormap_t cm[2];
! 	di[0]=(const GLDrawItem *)a;
! 	di[1]=(const GLDrawItem *)b;
! 
! 	for(int i=0;i<2;i++)
! 	{
! 		switch(di[i]->rendertype)
! 		{
! 		case GLDIT_FLAT:
! 		{
! 			GLFlat * f=&sortinfo->flats[di[i]->index];
! 			tx[i]=f->gltexture;
! 			lights[i]=f->lightlevel;
! 			clamp[i] = 0;
! 		}
! 		break;
  
+ 		case GLDIT_WALL:
+ 		{
+ 			GLWall * w=&sortinfo->walls[di[i]->index];
+ 			tx[i]=w->gltexture;
+ 			lights[i]=w->lightlevel;
+ 			clamp[i] = w->flags & 3;
+ 		}
+ 		break;
  
! 		case GLDIT_SPRITE:
! 		{
! 			GLSprite * s=&sortinfo->sprites[di[i]->index];
! 			tx[i]=s->gltexture;
! 			lights[i]=s->lightlevel;
! 			clamp[i] = 4;
! 		}
! 		break;
! 		case GLDIT_POLY: break;
! 		}
  	}
+ 	if (tx[0]!=tx[1]) return tx[0]-tx[1];
+ 	if (clamp[0]!=clamp[1]) return clamp[0]-clamp[1];	// clamping forces different textures.
+ 	return lights[0]-lights[1];
  }
  
! 
! void GLDrawList::Sort()
  {
! 	if (drawitems.Size()!=0 && gl_sort_textures)
  	{
  		sortinfo=this;
! 		qsort(&drawitems[0], drawitems.Size(), sizeof(drawitems[0]), dicmp);
  	}
  }
  
***************
*** 878,883 ****
--- 855,861 ----
  //==========================================================================
  void GLDrawList::AddWall(GLWall * wall)
  {
+ 	//@sync-drawinfo
  	drawitems.Push(GLDrawItem(GLDIT_WALL,walls.Push(*wall)));
  }
  
***************
*** 888,893 ****
--- 866,872 ----
  //==========================================================================
  void GLDrawList::AddFlat(GLFlat * flat)
  {
+ 	//@sync-drawinfo
  	drawitems.Push(GLDrawItem(GLDIT_FLAT,flats.Push(*flat)));
  }
  
***************
*** 898,903 ****
--- 877,883 ----
  //==========================================================================
  void GLDrawList::AddSprite(GLSprite * sprite)
  {	
+ 	//@sync-drawinfo
  	drawitems.Push(GLDrawItem(GLDIT_SPRITE,sprites.Push(*sprite)));
  }
  
***************
*** 1005,1026 ****
  	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);		// increment stencil of valid pixels
  	glColorMask(0,0,0,0);						// don't write to the graphics buffer
  	gl_RenderState.EnableTexture(false);
! 	gl_RenderState.ResetColor();
  	glEnable(GL_DEPTH_TEST);
  	glDepthMask(true);
  
  	gl_RenderState.Apply();
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(ws->x1, ws->z1, ws->y1, 0, 0);
! 	ptr++;
! 	ptr->Set(ws->x1, ws->z2, ws->y1, 0, 0);
! 	ptr++;
! 	ptr->Set(ws->x2, ws->z2, ws->y2, 0, 0);
! 	ptr++;
! 	ptr->Set(ws->x2, ws->z1, ws->y2, 0, 0);
! 	ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_FAN);
! 
  
  	glStencilFunc(GL_EQUAL,recursion+1,~0);		// draw sky into stencil
  	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);		// this stage doesn't modify the stencil
--- 985,1001 ----
  	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);		// increment stencil of valid pixels
  	glColorMask(0,0,0,0);						// don't write to the graphics buffer
  	gl_RenderState.EnableTexture(false);
! 	glColor3f(1,1,1);
  	glEnable(GL_DEPTH_TEST);
  	glDepthMask(true);
  
  	gl_RenderState.Apply();
! 	glBegin(GL_TRIANGLE_FAN);
! 	glVertex3f(ws->x1, ws->z1, ws->y1);
! 	glVertex3f(ws->x1, ws->z2, ws->y1);
! 	glVertex3f(ws->x2, ws->z2, ws->y2);
! 	glVertex3f(ws->x2, ws->z1, ws->y2);
! 	glEnd();
  
  	glStencilFunc(GL_EQUAL,recursion+1,~0);		// draw sky into stencil
  	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);		// this stage doesn't modify the stencil
***************
*** 1038,1056 ****
  	glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
  	gl_RenderState.EnableTexture(false);
  	glColorMask(0,0,0,0);						// don't write to the graphics buffer
! 	gl_RenderState.ResetColor();
  
  	gl_RenderState.Apply();
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(ws->x1, ws->z1, ws->y1, 0, 0);
! 	ptr++;
! 	ptr->Set(ws->x1, ws->z2, ws->y1, 0, 0);
! 	ptr++;
! 	ptr->Set(ws->x2, ws->z2, ws->y2, 0, 0);
! 	ptr++;
! 	ptr->Set(ws->x2, ws->z1, ws->y2, 0, 0);
! 	ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_FAN);
  
  	// restore old stencil op.
  	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
--- 1013,1027 ----
  	glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
  	gl_RenderState.EnableTexture(false);
  	glColorMask(0,0,0,0);						// don't write to the graphics buffer
! 	glColor3f(1,1,1);
  
  	gl_RenderState.Apply();
! 	glBegin(GL_TRIANGLE_FAN);
! 	glVertex3f(ws->x1, ws->z1, ws->y1);
! 	glVertex3f(ws->x1, ws->z2, ws->y1);
! 	glVertex3f(ws->x2, ws->z2, ws->y2);
! 	glVertex3f(ws->x2, ws->z1, ws->y2);
! 	glEnd();
  
  	// restore old stencil op.
  	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
***************
*** 1075,1086 ****
  
  	plane.GetFromSector(sec, ceiling);
  
! 	gltexture=FMaterial::ValidateTexture(plane.texture, false, true);
  	if (!gltexture) return;
  
  	if (gl_fixedcolormap) 
  	{
! 		Colormap.Clear();
  		lightlevel=255;
  	}
  	else
--- 1046,1057 ----
  
  	plane.GetFromSector(sec, ceiling);
  
! 	gltexture=FMaterial::ValidateTexture(plane.texture, true);
  	if (!gltexture) return;
  
  	if (gl_fixedcolormap) 
  	{
! 		Colormap.GetFixedColormap();
  		lightlevel=255;
  	}
  	else
***************
*** 1095,1111 ****
  	}
  
  	int rel = getExtraLight();
! 	gl_SetColor(lightlevel, rel, Colormap, 1.0f);
  	gl_SetFog(lightlevel, rel, &Colormap, false);
! 	gl_RenderState.SetMaterial(gltexture, CLAMP_NONE, 0, -1, false);
  
  	float fviewx = FIXED2FLOAT(viewx);
  	float fviewy = FIXED2FLOAT(viewy);
  	float fviewz = FIXED2FLOAT(viewz);
  
- 	gl_SetPlaneTextureRotation(&plane, gltexture);
  	gl_RenderState.Apply();
  
  	float prj_fac1 = (planez-fviewz)/(ws->z1-fviewz);
  	float prj_fac2 = (planez-fviewz)/(ws->z2-fviewz);
  
--- 1066,1084 ----
  	}
  
  	int rel = getExtraLight();
! 	gl_SetColor(lightlevel, rel, &Colormap, 1.0f);
  	gl_SetFog(lightlevel, rel, &Colormap, false);
! 	gltexture->Bind(Colormap.colormap);
  
  	float fviewx = FIXED2FLOAT(viewx);
  	float fviewy = FIXED2FLOAT(viewy);
  	float fviewz = FIXED2FLOAT(viewz);
  
  	gl_RenderState.Apply();
  
+ 	bool pushed = gl_SetPlaneTextureRotation(&plane, gltexture);
+ 
+ 	glBegin(GL_TRIANGLE_FAN);
  	float prj_fac1 = (planez-fviewz)/(ws->z1-fviewz);
  	float prj_fac2 = (planez-fviewz)/(ws->z2-fviewz);
  
***************
*** 1121,1138 ****
  	float px4 = fviewx + prj_fac1 * (ws->x2-fviewx);
  	float py4 = fviewy + prj_fac1 * (ws->y2-fviewy);
  
! 	FFlatVertex *ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(px1, planez, py1, px1 / 64, -py1 / 64);
! 	ptr++;
! 	ptr->Set(px2, planez, py2, px2 / 64, -py2 / 64);
! 	ptr++;
! 	ptr->Set(px3, planez, py3, px3 / 64, -py3 / 64);
! 	ptr++;
! 	ptr->Set(px4, planez, py4, px4 / 64, -py4 / 64);
! 	ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_FAN);
  
! 	gl_RenderState.EnableTextureMatrix(false);
  }
  
  //==========================================================================
--- 1094,1118 ----
  	float px4 = fviewx + prj_fac1 * (ws->x2-fviewx);
  	float py4 = fviewy + prj_fac1 * (ws->y2-fviewy);
  
! 	glTexCoord2f(px1 / 64, -py1 / 64);
! 	glVertex3f(px1, planez, py1);
! 
! 	glTexCoord2f(px2 / 64, -py2 / 64);
! 	glVertex3f(px2, planez, py2);
! 
! 	glTexCoord2f(px3 / 64, -py3 / 64);
! 	glVertex3f(px3, planez, py3);
  
! 	glTexCoord2f(px4 / 64, -py4 / 64);
! 	glVertex3f(px4, planez, py4);
! 
! 	glEnd();
! 
! 	if (pushed)
! 	{
! 		glPopMatrix();
! 		glMatrixMode(GL_MODELVIEW);
! 	}
  }
  
  //==========================================================================
