*** doom/src/main/jni/gzdoom/src/gl/scene/gl_vertex.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_vertex.cpp	2017-06-18 23:34:13.297248223 -0400
***************
*** 54,59 ****
--- 53,59 ----
  #include "gl/utility/gl_templates.h"
  
  EXTERN_CVAR(Bool, gl_seamless)
+ extern int vertexcount;
  
  //==========================================================================
  //
***************
*** 61,67 ****
  //
  //==========================================================================
  
! void GLWall::SplitUpperEdge(texcoord * tcs, FFlatVertex *&ptr)
  {
  	if (seg == NULL || seg->sidedef == NULL || (seg->sidedef->Flags & WALLF_POLYOBJ) || seg->sidedef->numsegs == 1) return;
  
--- 61,67 ----
  //
  //==========================================================================
  
! void GLWall::SplitUpperEdge(texcoord * tcs, bool glow)
  {
  	if (seg == NULL || seg->sidedef == NULL || (seg->sidedef->Flags & WALLF_POLYOBJ) || seg->sidedef->numsegs == 1) return;
  
***************
*** 73,79 ****
  	float facc = (zceil[1] - zceil[0]) / polyw;
  	float facf = (zfloor[1] - zfloor[0]) / polyw;
  
! 	for (int i = 0; i < sidedef->numsegs - 1; i++)
  	{
  		seg_t *cseg = sidedef->segs[i];
  		float sidefrac = cseg->sidefrac;
--- 73,79 ----
  	float facc = (zceil[1] - zceil[0]) / polyw;
  	float facf = (zfloor[1] - zfloor[0]) / polyw;
  
! 	for (int i=0; i < sidedef->numsegs - 1; i++)
  	{
  		seg_t *cseg = sidedef->segs[i];
  		float sidefrac = cseg->sidefrac;
***************
*** 82,94 ****
  
  		float fracfac = sidefrac - glseg.fracleft;
  
! 		ptr->x = cseg->v2->fx;
! 		ptr->y = cseg->v2->fy;
! 		ptr->z = ztop[0] + fact * fracfac;
! 		ptr->u = tcs[1].u + facu * fracfac;
! 		ptr->v = tcs[1].v + facv * fracfac;
! 		ptr++;
  	}
  }
  
  //==========================================================================
--- 82,94 ----
  
  		float fracfac = sidefrac - glseg.fracleft;
  
! 		if (glow) glVertexAttrib2f(VATTR_GLOWDISTANCE, zceil[0] - ztop[0] + (facc - fact) * fracfac, 
! 									 ztop[0] - zfloor[0] + (fact - facf) * fracfac);
! 
! 		glTexCoord2f(tcs[1].u + facu * fracfac, tcs[1].v + facv * fracfac);
! 		glVertex3f(cseg->v2->fx, ztop[0] + fact * fracfac, cseg->v2->fy);
  	}
+ 	vertexcount += sidedef->numsegs-1;
  }
  
  //==========================================================================
***************
*** 97,103 ****
  //
  //==========================================================================
  
! void GLWall::SplitLowerEdge(texcoord * tcs, FFlatVertex *&ptr)
  {
  	if (seg == NULL || seg->sidedef == NULL || (seg->sidedef->Flags & WALLF_POLYOBJ) || seg->sidedef->numsegs == 1) return;
  
--- 97,103 ----
  //
  //==========================================================================
  
! void GLWall::SplitLowerEdge(texcoord * tcs, bool glow)
  {
  	if (seg == NULL || seg->sidedef == NULL || (seg->sidedef->Flags & WALLF_POLYOBJ) || seg->sidedef->numsegs == 1) return;
  
***************
*** 109,115 ****
  	float facc = (zceil[1] - zceil[0]) / polyw;
  	float facf = (zfloor[1] - zfloor[0]) / polyw;
  
! 	for (int i = sidedef->numsegs - 2; i >= 0; i--)
  	{
  		seg_t *cseg = sidedef->segs[i];
  		float sidefrac = cseg->sidefrac;
--- 109,115 ----
  	float facc = (zceil[1] - zceil[0]) / polyw;
  	float facf = (zfloor[1] - zfloor[0]) / polyw;
  
! 	for (int i = sidedef->numsegs-2; i >= 0; i--)
  	{
  		seg_t *cseg = sidedef->segs[i];
  		float sidefrac = cseg->sidefrac;
***************
*** 118,130 ****
  
  		float fracfac = sidefrac - glseg.fracleft;
  
! 		ptr->x = cseg->v2->fx;
! 		ptr->y = cseg->v2->fy;
! 		ptr->z = zbottom[0] + facb * fracfac;
! 		ptr->u = tcs[0].u + facu * fracfac;
! 		ptr->v = tcs[0].v + facv * fracfac;
! 		ptr++;
  	}
  }
  
  //==========================================================================
--- 118,130 ----
  
  		float fracfac = sidefrac - glseg.fracleft;
  
! 		if (glow) glVertexAttrib2f(VATTR_GLOWDISTANCE, zceil[0] - zbottom[0] + (facc - facb) * fracfac, 
! 									 zbottom[0] - zfloor[0] + (facb - facf) * fracfac);
! 
! 		glTexCoord2f(tcs[0].u + facu * fracfac, tcs[0].v + facv * fracfac);
! 		glVertex3f(cseg->v2->fx, zbottom[0] + facb * fracfac, cseg->v2->fy);
  	}
+ 	vertexcount += sidedef->numsegs-1;
  }
  
  //==========================================================================
***************
*** 133,163 ****
  //
  //==========================================================================
  
! void GLWall::SplitLeftEdge(texcoord * tcs, FFlatVertex *&ptr)
  {
! 	if (vertexes[0] == NULL) return;
  
! 	vertex_t * vi = vertexes[0];
  
  	if (vi->numheights)
  	{
! 		int i = 0;
  
! 		float polyh1 = ztop[0] - zbottom[0];
! 		float factv1 = polyh1 ? (tcs[1].v - tcs[0].v) / polyh1 : 0;
! 		float factu1 = polyh1 ? (tcs[1].u - tcs[0].u) / polyh1 : 0;
  
! 		while (i<vi->numheights && vi->heightlist[i] <= zbottom[0]) i++;
  		while (i<vi->numheights && vi->heightlist[i] < ztop[0])
  		{
! 			ptr->x = glseg.x1;
! 			ptr->y = glseg.y1;
! 			ptr->z = vi->heightlist[i];
! 			ptr->u = factu1*(vi->heightlist[i] - ztop[0]) + tcs[1].u;
! 			ptr->v = factv1*(vi->heightlist[i] - ztop[0]) + tcs[1].v;
! 			ptr++;
  			i++;
  		}
  	}
  }
  
--- 133,162 ----
  //
  //==========================================================================
  
! void GLWall::SplitLeftEdge(texcoord * tcs, bool glow)
  {
! 	if (vertexes[0]==NULL) return;
  
! 	vertex_t * vi=vertexes[0];
  
  	if (vi->numheights)
  	{
! 		int i=0;
  
! 		float polyh1=ztop[0] - zbottom[0];
! 		float factv1=polyh1? (tcs[1].v - tcs[0].v) / polyh1:0;
! 		float factu1=polyh1? (tcs[1].u - tcs[0].u) / polyh1:0;
  
! 		while (i<vi->numheights && vi->heightlist[i] <= zbottom[0] ) i++;
  		while (i<vi->numheights && vi->heightlist[i] < ztop[0])
  		{
! 			if (glow) glVertexAttrib2f(VATTR_GLOWDISTANCE, zceil[0] - vi->heightlist[i], vi->heightlist[i] - zfloor[0]);
! 			glTexCoord2f(factu1*(vi->heightlist[i] - ztop[0]) + tcs[1].u,
! 						 factv1*(vi->heightlist[i] - ztop[0]) + tcs[1].v);
! 			glVertex3f(glseg.x1, vi->heightlist[i], glseg.y1);
  			i++;
  		}
+ 		vertexcount+=i;
  	}
  }
  
***************
*** 167,197 ****
  //
  //==========================================================================
  
! void GLWall::SplitRightEdge(texcoord * tcs, FFlatVertex *&ptr)
  {
! 	if (vertexes[1] == NULL) return;
  
! 	vertex_t * vi = vertexes[1];
  
  	if (vi->numheights)
  	{
! 		int i = vi->numheights - 1;
  
  		float polyh2 = ztop[1] - zbottom[1];
! 		float factv2 = polyh2 ? (tcs[2].v - tcs[3].v) / polyh2 : 0;
! 		float factu2 = polyh2 ? (tcs[2].u - tcs[3].u) / polyh2 : 0;
  
  		while (i>0 && vi->heightlist[i] >= ztop[1]) i--;
  		while (i>0 && vi->heightlist[i] > zbottom[1])
  		{
! 			ptr->x = glseg.x2;
! 			ptr->y = glseg.y2;
! 			ptr->z = vi->heightlist[i];
! 			ptr->u = factu2*(vi->heightlist[i] - ztop[1]) + tcs[2].u;
! 			ptr->v = factv2*(vi->heightlist[i] - ztop[1]) + tcs[2].v;
! 			ptr++;
  			i--;
  		}
  	}
  }
  
--- 166,195 ----
  //
  //==========================================================================
  
! void GLWall::SplitRightEdge(texcoord * tcs, bool glow)
  {
! 	if (vertexes[1]==NULL) return;
  
! 	vertex_t * vi=vertexes[1];
  
  	if (vi->numheights)
  	{
! 		int i=vi->numheights-1;
  
  		float polyh2 = ztop[1] - zbottom[1];
! 		float factv2 = polyh2? (tcs[2].v - tcs[3].v) / polyh2:0;
! 		float factu2 = polyh2? (tcs[2].u - tcs[3].u) / polyh2:0;
  
  		while (i>0 && vi->heightlist[i] >= ztop[1]) i--;
  		while (i>0 && vi->heightlist[i] > zbottom[1])
  		{
! 			if (glow) glVertexAttrib2f(VATTR_GLOWDISTANCE, zceil[1] - vi->heightlist[i], vi->heightlist[i] - zfloor[1]);
! 			glTexCoord2f(factu2 * (vi->heightlist[i] - ztop[1]) + tcs[2].u,
! 						 factv2 * (vi->heightlist[i] - ztop[1]) + tcs[2].v);
! 			glVertex3f(glseg.x2, vi->heightlist[i], glseg.y2);
  			i--;
  		}
+ 		vertexcount+=i;
  	}
  }
  
