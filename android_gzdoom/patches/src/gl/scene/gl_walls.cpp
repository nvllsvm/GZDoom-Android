*** doom/src/main/jni/gzdoom/src/gl/scene/gl_walls.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_walls.cpp	2017-06-18 23:34:13.323915136 -0400
***************
*** 53,58 ****
--- 53,59 ----
  #include "gl/data/gl_data.h"
  #include "gl/dynlights/gl_dynlight.h"
  #include "gl/dynlights/gl_glow.h"
+ #include "gl/dynlights/gl_lightbuffer.h"
  #include "gl/scene/gl_drawinfo.h"
  #include "gl/scene/gl_portal.h"
  #include "gl/textures/gl_material.h"
***************
*** 70,76 ****
  void GLWall::CheckGlowing()
  {
  	bottomglowcolor[3] = topglowcolor[3] = 0;
! 	if (!gl_isFullbright(Colormap.LightColor, lightlevel))
  	{
  		FTexture *tex = TexMan[topflat];
  		if (tex != NULL && tex->isGlowing())
--- 71,77 ----
  void GLWall::CheckGlowing()
  {
  	bottomglowcolor[3] = topglowcolor[3] = 0;
! 	if (!gl_isFullbright(Colormap.LightColor, lightlevel) && gl_GlowActive())
  	{
  		FTexture *tex = TexMan[topflat];
  		if (tex != NULL && tex->isGlowing())
***************
*** 114,121 ****
  		4,		//RENDERWALL_SECTORSTACK,      // special
  		4,		//RENDERWALL_PLANEMIRROR,      // special
  		4,		//RENDERWALL_MIRROR,           // special
! 		1,		//RENDERWALL_MIRRORSURFACE,    // only created here from RENDERWALL_MIRROR
! 		2,		//RENDERWALL_M2SNF,            // depends on render and texture settings, no fog, used on mid texture lines with a fog boundary.
  		3,		//RENDERWALL_COLOR,            // translucent
  		2,		//RENDERWALL_FFBLOCK           // depends on render and texture settings
  		4,		//RENDERWALL_COLORLAYER        // color layer needs special handling
--- 115,123 ----
  		4,		//RENDERWALL_SECTORSTACK,      // special
  		4,		//RENDERWALL_PLANEMIRROR,      // special
  		4,		//RENDERWALL_MIRROR,           // special
! 		1,		//RENDERWALL_MIRRORSURFACE,    // needs special handling
! 		2,		//RENDERWALL_M2SNF,            // depends on render and texture settings, no fog
! 		2,		//RENDERWALL_M2SFOG,            // depends on render and texture settings, no fog
  		3,		//RENDERWALL_COLOR,            // translucent
  		2,		//RENDERWALL_FFBLOCK           // depends on render and texture settings
  		4,		//RENDERWALL_COLORLAYER        // color layer needs special handling
***************
*** 131,137 ****
  		// light planes don't get drawn with fullbright rendering
  		if (!gltexture && passflag[type]!=4) return;
  
! 		Colormap.Clear();
  	}
  
  	CheckGlowing();
--- 133,139 ----
  		// light planes don't get drawn with fullbright rendering
  		if (!gltexture && passflag[type]!=4) return;
  
! 		Colormap.GetFixedColormap();
  	}
  
  	CheckGlowing();
***************
*** 144,161 ****
  	}
  	else if (passflag[type]!=4)	// non-translucent walls
  	{
  
  		bool masked;
  
! 		masked = passflag[type]==1? false : (gltexture && gltexture->isMasked());
! 
! 		if ((flags&GLWF_SKYHACK && type == RENDERWALL_M2S))
  		{
! 			list = GLDL_MASKEDWALLSOFS;
  		}
! 		else
  		{
! 			list = masked ? GLDL_MASKEDWALLS : GLDL_PLAINWALLS;
  		}
  		gl_drawinfo->drawlists[list].AddWall(this);
  
--- 146,191 ----
  	}
  	else if (passflag[type]!=4)	// non-translucent walls
  	{
+ 		static DrawListType list_indices[2][2][2]={
+ 			{ { GLDL_PLAIN, GLDL_FOG      }, { GLDL_MASKED,      GLDL_FOGMASKED      } },
+ 			{ { GLDL_LIGHT, GLDL_LIGHTFOG }, { GLDL_LIGHTMASKED, GLDL_LIGHTFOGMASKED } }
+ 		};
  
  		bool masked;
+ 		bool light = gl_forcemultipass;
  
! 		if (!gl_fixedcolormap)
  		{
! 			if (gl_lights && !gl_dynlight_shader)
! 			{
! 				if (seg->sidedef == NULL)
! 				{
! 					light = false;
! 				}
! 				else if (!(seg->sidedef->Flags & WALLF_POLYOBJ))
! 				{
! 					light = seg->sidedef->lighthead[0] != NULL;
! 				}
! 				else if (sub)
! 				{
! 					// for polyobjects we cannot use the side's light list. 
! 					// We must use the subsector's.
! 					light = sub->lighthead[0] != NULL;
! 				}
! 			}
  		}
! 		else 
  		{
! 			flags&=~GLWF_FOGGY;
! 		}
! 
! 		masked = passflag[type]==1? false : (light && type!=RENDERWALL_FFBLOCK) || (gltexture && gltexture->isMasked());
! 
! 		list = list_indices[light][masked][!!(flags&GLWF_FOGGY)];
! 		if (list == GLDL_LIGHT)
! 		{
! 			if (gltexture->tex->gl_info.Brightmap && gl_BrightmapsActive()) list = GLDL_LIGHTBRIGHT;
! 			if (flags & GLWF_GLOW) list = GLDL_LIGHTBRIGHT;
  		}
  		gl_drawinfo->drawlists[list].AddWall(this);
  
***************
*** 163,174 ****
  	else switch (type)
  	{
  	case RENDERWALL_COLORLAYER:
! 		gl_drawinfo->drawlists[GLDL_TRANSLUCENTBORDER].AddWall(this);
  		break;
  
  	// portals don't go into the draw list.
  	// Instead they are added to the portal manager
  	case RENDERWALL_HORIZON:
  		horizon=UniqueHorizons.Get(horizon);
  		portal=GLPortal::FindPortal(horizon);
  		if (!portal) portal=new GLHorizonPortal(horizon);
--- 193,205 ----
  	else switch (type)
  	{
  	case RENDERWALL_COLORLAYER:
! 		gl_drawinfo->drawlists[GLDL_TRANSLUCENT].AddWall(this);
  		break;
  
  	// portals don't go into the draw list.
  	// Instead they are added to the portal manager
  	case RENDERWALL_HORIZON:
+ 		//@sync-portal
  		horizon=UniqueHorizons.Get(horizon);
  		portal=GLPortal::FindPortal(horizon);
  		if (!portal) portal=new GLHorizonPortal(horizon);
***************
*** 176,187 ****
--- 207,220 ----
  		break;
  
  	case RENDERWALL_SKYBOX:
+ 		//@sync-portal
  		portal=GLPortal::FindPortal(skybox);
  		if (!portal) portal=new GLSkyboxPortal(skybox);
  		portal->AddLine(this);
  		break;
  
  	case RENDERWALL_SECTORSTACK:
+ 		//@sync-portal
  		portal = this->portal->GetGLPortal();
  		portal->AddLine(this);
  		break;
***************
*** 198,203 ****
--- 231,237 ----
  		break;
  
  	case RENDERWALL_MIRROR:
+ 		//@sync-portal
  		portal=GLPortal::FindPortal(seg->linedef);
  		if (!portal) portal=new GLMirrorPortal(seg->linedef);
  		portal->AddLine(this);
***************
*** 210,215 ****
--- 244,250 ----
  		break;
  
  	case RENDERWALL_SKY:
+ 		//@sync-portal
  		portal=GLPortal::FindPortal(sky);
  		if (!portal) portal=new GLSkyPortal(sky);
  		portal->AddLine(this);
***************
*** 490,496 ****
  				hi.colormap.LightColor = (light->extra_colormap)->Color;
  			}
  
! 			if (gl_fixedcolormap) hi.colormap.Clear();
  			horizon = &hi;
  			PutWall(0);
  		}
--- 525,531 ----
  				hi.colormap.LightColor = (light->extra_colormap)->Color;
  			}
  
! 			if (gl_fixedcolormap) hi.colormap.GetFixedColormap();
  			horizon = &hi;
  			PutWall(0);
  		}
***************
*** 519,526 ****
  				hi.colormap.LightColor = (light->extra_colormap)->Color;
  			}
  
! 			if (gl_fixedcolormap) hi.colormap.Clear();
! 			horizon = &hi;
  			PutWall(0);
  		}
  	}
--- 554,561 ----
  				hi.colormap.LightColor = (light->extra_colormap)->Color;
  			}
  
! 			if (gl_fixedcolormap) hi.colormap.GetFixedColormap();
! 			horizon=&hi;
  			PutWall(0);
  		}
  	}
***************
*** 778,789 ****
  		if ( (seg->linedef->flags & ML_DONTPEGBOTTOM) >0)
  		{
  			texturebottom = MAX(realfront->GetPlaneTexZ(sector_t::floor),realback->GetPlaneTexZ(sector_t::floor))+rowoffset;
! 			texturetop=texturebottom+(gltexture->TextureHeight()<<FRACBITS);
  		}
  		else
  		{
  			texturetop = MIN(realfront->GetPlaneTexZ(sector_t::ceiling),realback->GetPlaneTexZ(sector_t::ceiling))+rowoffset;
! 			texturebottom=texturetop-(gltexture->TextureHeight()<<FRACBITS);
  		}
  	}
  	else texturetop=texturebottom=0;
--- 813,824 ----
  		if ( (seg->linedef->flags & ML_DONTPEGBOTTOM) >0)
  		{
  			texturebottom = MAX(realfront->GetPlaneTexZ(sector_t::floor),realback->GetPlaneTexZ(sector_t::floor))+rowoffset;
! 			texturetop=texturebottom+(gltexture->TextureHeight(GLUSE_TEXTURE)<<FRACBITS);
  		}
  		else
  		{
  			texturetop = MIN(realfront->GetPlaneTexZ(sector_t::ceiling),realback->GetPlaneTexZ(sector_t::ceiling))+rowoffset;
! 			texturebottom=texturetop-(gltexture->TextureHeight(GLUSE_TEXTURE)<<FRACBITS);
  		}
  	}
  	else texturetop=texturebottom=0;
***************
*** 910,917 ****
  		fixed_t textureoffset = tci.TextureOffset(t_ofs);
  		int righttex=(textureoffset>>FRACBITS)+seg->sidedef->TexelLength;
  		
! 		if ((textureoffset==0 && righttex<=gltexture->TextureWidth()) || 
! 			(textureoffset>=0 && righttex==gltexture->TextureWidth()))
  		{
  			flags|=GLT_CLAMPX;
  		}
--- 945,952 ----
  		fixed_t textureoffset = tci.TextureOffset(t_ofs);
  		int righttex=(textureoffset>>FRACBITS)+seg->sidedef->TexelLength;
  		
! 		if ((textureoffset==0 && righttex<=gltexture->TextureWidth(GLUSE_TEXTURE)) || 
! 			(textureoffset>=0 && righttex==gltexture->TextureWidth(GLUSE_TEXTURE)))
  		{
  			flags|=GLT_CLAMPX;
  		}
***************
*** 979,985 ****
  		//
  		FloatRect *splitrect;
  		int v = gltexture->GetAreas(&splitrect);
- 		if (seg->frontsector == seg->backsector) flags |= GLWF_NOSPLIT;	// we don't need to do vertex splits if a line has both sides in the same sector
  		if (v>0 && !drawfogboundary && !(seg->linedef->flags&ML_WRAP_MIDTEX))
  		{
  			// split the poly!
--- 1014,1019 ----
***************
*** 1089,1107 ****
  		
  		if (rover->flags&FF_UPPERTEXTURE) 
  		{
! 			gltexture = FMaterial::ValidateTexture(seg->sidedef->GetTexture(side_t::top), false,  true);
  			if (!gltexture) return;
  			gltexture->GetTexCoordInfo(&tci, seg->sidedef->GetTextureXScale(side_t::top), seg->sidedef->GetTextureYScale(side_t::top));
  		}
  		else if (rover->flags&FF_LOWERTEXTURE) 
  		{
! 			gltexture = FMaterial::ValidateTexture(seg->sidedef->GetTexture(side_t::bottom), false, true);
  			if (!gltexture) return;
  			gltexture->GetTexCoordInfo(&tci, seg->sidedef->GetTextureXScale(side_t::bottom), seg->sidedef->GetTextureYScale(side_t::bottom));
  		}
  		else 
  		{
! 			gltexture = FMaterial::ValidateTexture(mastersd->GetTexture(side_t::mid), false, true);
  			if (!gltexture) return;
  			gltexture->GetTexCoordInfo(&tci, mastersd->GetTextureXScale(side_t::mid), mastersd->GetTextureYScale(side_t::mid));
  		}
--- 1123,1141 ----
  		
  		if (rover->flags&FF_UPPERTEXTURE) 
  		{
! 			gltexture = FMaterial::ValidateTexture(seg->sidedef->GetTexture(side_t::top), true);
  			if (!gltexture) return;
  			gltexture->GetTexCoordInfo(&tci, seg->sidedef->GetTextureXScale(side_t::top), seg->sidedef->GetTextureYScale(side_t::top));
  		}
  		else if (rover->flags&FF_LOWERTEXTURE) 
  		{
! 			gltexture = FMaterial::ValidateTexture(seg->sidedef->GetTexture(side_t::bottom), true);
  			if (!gltexture) return;
  			gltexture->GetTexCoordInfo(&tci, seg->sidedef->GetTextureXScale(side_t::bottom), seg->sidedef->GetTextureYScale(side_t::bottom));
  		}
  		else 
  		{
! 			gltexture = FMaterial::ValidateTexture(mastersd->GetTexture(side_t::mid), true);
  			if (!gltexture) return;
  			gltexture->GetTexCoordInfo(&tci, mastersd->GetTextureXScale(side_t::mid), mastersd->GetTextureYScale(side_t::mid));
  		}
***************
*** 1403,1409 ****
  //==========================================================================
  void GLWall::Process(seg_t *seg, sector_t * frontsector, sector_t * backsector)
  {
! 	vertex_t * v1, *v2;
  	fixed_t fch1;
  	fixed_t ffh1;
  	fixed_t fch2;
--- 1437,1443 ----
  //==========================================================================
  void GLWall::Process(seg_t *seg, sector_t * frontsector, sector_t * backsector)
  {
! 	vertex_t * v1, * v2;
  	fixed_t fch1;
  	fixed_t ffh1;
  	fixed_t fch2;
***************
*** 1412,1423 ****
  	sector_t * realback;
  
  #ifdef _DEBUG
! 	if (seg->linedef-lines==1276)
  	{
  		int a = 0;
  	}
  #endif
! 
  	// note: we always have a valid sidedef and linedef reference when getting here.
  
  	this->seg = seg;
--- 1446,1457 ----
  	sector_t * realback;
  
  #ifdef _DEBUG
! 	if (seg->linedef-lines==636)
  	{
  		int a = 0;
  	}
  #endif
! 		
  	// note: we always have a valid sidedef and linedef reference when getting here.
  
  	this->seg = seg;
***************
*** 1432,1455 ****
  	{
  		// Need these for aligning the textures
  		realfront = &sectors[frontsector->sectornum];
! 		realback = backsector ? &sectors[backsector->sectornum] : NULL;
  	}
  
  	if (seg->sidedef == seg->linedef->sidedef[0])
  	{
! 		v1 = seg->linedef->v1;
! 		v2 = seg->linedef->v2;
  	}
  	else
  	{
! 		v1 = seg->linedef->v2;
! 		v2 = seg->linedef->v1;
  	}
  
  	if (!(seg->sidedef->Flags & WALLF_POLYOBJ))
  	{
! 		glseg.fracleft = 0;
! 		glseg.fracright = 1;
  		if (gl_seamless)
  		{
  			if (v1->dirty) gl_RecalcVertexHeights(v1);
--- 1466,1489 ----
  	{
  		// Need these for aligning the textures
  		realfront = &sectors[frontsector->sectornum];
! 		realback = backsector? &sectors[backsector->sectornum] : NULL;
  	}
  
  	if (seg->sidedef == seg->linedef->sidedef[0])
  	{
! 		v1=seg->linedef->v1;
! 		v2=seg->linedef->v2;
  	}
  	else
  	{
! 		v1=seg->linedef->v2;
! 		v2=seg->linedef->v1;
  	}
  
  	if (!(seg->sidedef->Flags & WALLF_POLYOBJ))
  	{
! 		glseg.fracleft=0;
! 		glseg.fracright=1;
  		if (gl_seamless)
  		{
  			if (v1->dirty) gl_RecalcVertexHeights(v1);
***************
*** 1458,1499 ****
  	}
  	else	// polyobjects must be rendered per seg.
  	{
! 		if (abs(v1->x - v2->x) > abs(v1->y - v2->y))
  		{
! 			glseg.fracleft = float(seg->v1->x - v1->x) / float(v2->x - v1->x);
! 			glseg.fracright = float(seg->v2->x - v1->x) / float(v2->x - v1->x);
  		}
  		else
  		{
! 			glseg.fracleft = float(seg->v1->y - v1->y) / float(v2->y - v1->y);
! 			glseg.fracright = float(seg->v2->y - v1->y) / float(v2->y - v1->y);
  		}
! 		v1 = seg->v1;
! 		v2 = seg->v2;
  	}
  
  
! 	vertexes[0] = v1;
! 	vertexes[1] = v2;
  
! 	glseg.x1 = FIXED2FLOAT(v1->x);
! 	glseg.y1 = FIXED2FLOAT(v1->y);
! 	glseg.x2 = FIXED2FLOAT(v2->x);
! 	glseg.y2 = FIXED2FLOAT(v2->y);
! 	Colormap = frontsector->ColorMap;
! 	flags = 0;
! 	dynlightindex = UINT_MAX;
  
  	int rel = 0;
  	int orglightlevel = gl_ClampLight(frontsector->lightlevel);
! 	bool foggy = (!gl_isBlack(Colormap.FadeColor) || level.flags&LEVEL_HASFADETABLE);	// fog disables fake contrast
! 	lightlevel = gl_ClampLight(seg->sidedef->GetLightLevel(foggy, orglightlevel, false, &rel));
  	if (orglightlevel >= 253)			// with the software renderer fake contrast won't be visible above this.
  	{
! 		rellight = 0;
  	}
  	else if (lightlevel - rel > 256)	// the brighter part of fake contrast will be clamped so also clamp the darker part by the same amount for better looks
! 	{
  		rellight = 256 - lightlevel + rel;
  	}
  	else
--- 1492,1531 ----
  	}
  	else	// polyobjects must be rendered per seg.
  	{
! 		if (abs(v1->x-v2->x) > abs(v1->y-v2->y))
  		{
! 			glseg.fracleft = float(seg->v1->x - v1->x)/float(v2->x-v1->x);
! 			glseg.fracright = float(seg->v2->x - v1->x)/float(v2->x-v1->x);
  		}
  		else
  		{
! 			glseg.fracleft = float(seg->v1->y - v1->y)/float(v2->y-v1->y);
! 			glseg.fracright = float(seg->v2->y - v1->y)/float(v2->y-v1->y);
  		}
! 		v1=seg->v1;
! 		v2=seg->v2;
  	}
  
  
! 	vertexes[0]=v1;
! 	vertexes[1]=v2;
  
! 	glseg.x1= FIXED2FLOAT(v1->x);
! 	glseg.y1= FIXED2FLOAT(v1->y);
! 	glseg.x2= FIXED2FLOAT(v2->x);
! 	glseg.y2= FIXED2FLOAT(v2->y);
! 	Colormap=frontsector->ColorMap;
! 	flags = (!gl_isBlack(Colormap.FadeColor) || level.flags&LEVEL_HASFADETABLE)? GLWF_FOGGY : 0;
  
  	int rel = 0;
  	int orglightlevel = gl_ClampLight(frontsector->lightlevel);
! 	lightlevel = gl_ClampLight(seg->sidedef->GetLightLevel(!!(flags&GLWF_FOGGY), orglightlevel, false, &rel));
  	if (orglightlevel >= 253)			// with the software renderer fake contrast won't be visible above this.
  	{
! 		rellight = 0;					
  	}
  	else if (lightlevel - rel > 256)	// the brighter part of fake contrast will be clamped so also clamp the darker part by the same amount for better looks
! 	{	
  		rellight = 256 - lightlevel + rel;
  	}
  	else
***************
*** 1501,1535 ****
  		rellight = rel;
  	}
  
! 	alpha = 1.0f;
! 	RenderStyle = STYLE_Normal;
! 	gltexture = NULL;
! 
! 	topflat = frontsector->GetTexture(sector_t::ceiling);	// for glowing textures. These must be saved because
! 	bottomflat = frontsector->GetTexture(sector_t::floor);	// the sector passed here might be a temporary copy.
! 	topplane = frontsector->ceilingplane;
! 	bottomplane = frontsector->floorplane;
  
  	// Save a little time (up to 0.3 ms per frame ;) )
  	if (frontsector->floorplane.a | frontsector->floorplane.b)
  	{
! 		ffh1 = frontsector->floorplane.ZatPoint(v1);
! 		ffh2 = frontsector->floorplane.ZatPoint(v2);
! 		zfloor[0] = FIXED2FLOAT(ffh1);
! 		zfloor[1] = FIXED2FLOAT(ffh2);
  	}
  	else
  	{
! 		ffh1 = ffh2 = -frontsector->floorplane.d;
  		zfloor[0] = zfloor[1] = FIXED2FLOAT(ffh2);
  	}
  
  	if (frontsector->ceilingplane.a | frontsector->ceilingplane.b)
  	{
! 		fch1 = frontsector->ceilingplane.ZatPoint(v1);
! 		fch2 = frontsector->ceilingplane.ZatPoint(v2);
! 		zceil[0] = FIXED2FLOAT(fch1);
! 		zceil[1] = FIXED2FLOAT(fch2);
  	}
  	else
  	{
--- 1533,1565 ----
  		rellight = rel;
  	}
  
! 	alpha=1.0f;
! 	RenderStyle=STYLE_Normal;
! 	gltexture=NULL;
! 
! 	topflat=frontsector->GetTexture(sector_t::ceiling);	// for glowing textures. These must be saved because
! 	bottomflat=frontsector->GetTexture(sector_t::floor);	// the sector passed here might be a temporary copy.
  
  	// Save a little time (up to 0.3 ms per frame ;) )
  	if (frontsector->floorplane.a | frontsector->floorplane.b)
  	{
! 		ffh1=frontsector->floorplane.ZatPoint(v1); 
! 		ffh2=frontsector->floorplane.ZatPoint(v2); 
! 		zfloor[0]=FIXED2FLOAT(ffh1);
! 		zfloor[1]=FIXED2FLOAT(ffh2);
  	}
  	else
  	{
! 		ffh1 = ffh2 = -frontsector->floorplane.d; 
  		zfloor[0] = zfloor[1] = FIXED2FLOAT(ffh2);
  	}
  
  	if (frontsector->ceilingplane.a | frontsector->ceilingplane.b)
  	{
! 		fch1=frontsector->ceilingplane.ZatPoint(v1);
! 		fch2=frontsector->ceilingplane.ZatPoint(v2);
! 		zceil[0]= FIXED2FLOAT(fch1);
! 		zceil[1]= FIXED2FLOAT(fch2);
  	}
  	else
  	{
***************
*** 1538,1564 ****
  	}
  
  
! 	if (seg->linedef->special == Line_Horizon)
  	{
! 		SkyNormal(frontsector, v1, v2);
! 		DoHorizon(seg, frontsector, v1, v2);
  		return;
  	}
  
  	//return;
  	// [GZ] 3D middle textures are necessarily two-sided, even if they lack the explicit two-sided flag
! 	if (!backsector || !(seg->linedef->flags&(ML_TWOSIDED | ML_3DMIDTEX))) // one sided
  	{
  		// sector's sky
! 		SkyNormal(frontsector, v1, v2);
! 
  		// normal texture
! 		gltexture = FMaterial::ValidateTexture(seg->sidedef->GetTexture(side_t::mid), false, true);
! 		if (gltexture)
  		{
! 			DoTexture(RENDERWALL_M1S, seg, (seg->linedef->flags & ML_DONTPEGBOTTOM) > 0,
! 				realfront->GetPlaneTexZ(sector_t::ceiling), realfront->GetPlaneTexZ(sector_t::floor),	// must come from the original!
! 				fch1, fch2, ffh1, ffh2, 0);
  		}
  	}
  	else // two sided
--- 1568,1594 ----
  	}
  
  
! 	if (seg->linedef->special==Line_Horizon)
  	{
! 		SkyNormal(frontsector,v1,v2);
! 		DoHorizon(seg,frontsector, v1,v2);
  		return;
  	}
  
  	//return;
  	// [GZ] 3D middle textures are necessarily two-sided, even if they lack the explicit two-sided flag
! 	if (!backsector || !(seg->linedef->flags&(ML_TWOSIDED|ML_3DMIDTEX))) // one sided
  	{
  		// sector's sky
! 		SkyNormal(frontsector,v1,v2);
! 		
  		// normal texture
! 		gltexture=FMaterial::ValidateTexture(seg->sidedef->GetTexture(side_t::mid), true);
! 		if (gltexture) 
  		{
! 			DoTexture(RENDERWALL_M1S,seg,(seg->linedef->flags & ML_DONTPEGBOTTOM)>0,
! 							  realfront->GetPlaneTexZ(sector_t::ceiling),realfront->GetPlaneTexZ(sector_t::floor),	// must come from the original!
! 							  fch1,fch2,ffh1,ffh2,0);
  		}
  	}
  	else // two sided
***************
*** 1571,1578 ****
  
  		if (backsector->floorplane.a | backsector->floorplane.b)
  		{
! 			bfh1 = backsector->floorplane.ZatPoint(v1);
! 			bfh2 = backsector->floorplane.ZatPoint(v2);
  		}
  		else
  		{
--- 1601,1608 ----
  
  		if (backsector->floorplane.a | backsector->floorplane.b)
  		{
! 			bfh1=backsector->floorplane.ZatPoint(v1); 
! 			bfh2=backsector->floorplane.ZatPoint(v2); 
  		}
  		else
  		{
***************
*** 1581,1640 ****
  
  		if (backsector->ceilingplane.a | backsector->ceilingplane.b)
  		{
! 			bch1 = backsector->ceilingplane.ZatPoint(v1);
! 			bch2 = backsector->ceilingplane.ZatPoint(v2);
  		}
  		else
  		{
  			bch1 = bch2 = backsector->ceilingplane.d;
  		}
  
! 		SkyTop(seg, frontsector, backsector, v1, v2);
! 		SkyBottom(seg, frontsector, backsector, v1, v2);
! 
  		// upper texture
! 		if (frontsector->GetTexture(sector_t::ceiling) != skyflatnum || backsector->GetTexture(sector_t::ceiling) != skyflatnum)
  		{
! 			fixed_t bch1a = bch1, bch2a = bch2;
! 			if (frontsector->GetTexture(sector_t::floor) != skyflatnum || backsector->GetTexture(sector_t::floor) != skyflatnum)
  			{
  				// the back sector's floor obstructs part of this wall				
! 				if (ffh1 > bch1 && ffh2 > bch2)
  				{
! 					bch2a = ffh2;
! 					bch1a = ffh1;
  				}
  			}
  
! 			if (bch1a < fch1 || bch2a < fch2)
  			{
! 				gltexture = FMaterial::ValidateTexture(seg->sidedef->GetTexture(side_t::top), false, true);
! 				if (gltexture)
  				{
! 					DoTexture(RENDERWALL_TOP, seg, (seg->linedef->flags & (ML_DONTPEGTOP)) == 0,
! 						realfront->GetPlaneTexZ(sector_t::ceiling), realback->GetPlaneTexZ(sector_t::ceiling),
! 						fch1, fch2, bch1a, bch2a, 0);
  				}
! 				else if ((frontsector->ceilingplane.a | frontsector->ceilingplane.b |
! 					backsector->ceilingplane.a | backsector->ceilingplane.b) &&
! 					frontsector->GetTexture(sector_t::ceiling) != skyflatnum &&
! 					backsector->GetTexture(sector_t::ceiling) != skyflatnum)
  				{
! 					gltexture = FMaterial::ValidateTexture(frontsector->GetTexture(sector_t::ceiling), false, true);
  					if (gltexture)
  					{
! 						DoTexture(RENDERWALL_TOP, seg, (seg->linedef->flags & (ML_DONTPEGTOP)) == 0,
! 							realfront->GetPlaneTexZ(sector_t::ceiling), realback->GetPlaneTexZ(sector_t::ceiling),
! 							fch1, fch2, bch1a, bch2a, 0);
  					}
  				}
  				else if (!(seg->sidedef->Flags & WALLF_POLYOBJ))
  				{
! 					// skip processing if the back is a malformed subsector
! 					if (!(seg->PartnerSeg->Subsector->hacked & 4))
! 					{
! 						gl_drawinfo->AddUpperMissingTexture(seg->sidedef, sub, bch1a);
! 					}
  				}
  			}
  		}
--- 1611,1666 ----
  
  		if (backsector->ceilingplane.a | backsector->ceilingplane.b)
  		{
! 			bch1=backsector->ceilingplane.ZatPoint(v1);
! 			bch2=backsector->ceilingplane.ZatPoint(v2);
  		}
  		else
  		{
  			bch1 = bch2 = backsector->ceilingplane.d;
  		}
  
! 		SkyTop(seg,frontsector,backsector,v1,v2);
! 		SkyBottom(seg,frontsector,backsector,v1,v2);
! 		
  		// upper texture
! 		if (frontsector->GetTexture(sector_t::ceiling)!=skyflatnum || backsector->GetTexture(sector_t::ceiling)!=skyflatnum)
  		{
! 			fixed_t bch1a=bch1, bch2a=bch2;
! 			if (frontsector->GetTexture(sector_t::floor)!=skyflatnum || backsector->GetTexture(sector_t::floor)!=skyflatnum)
  			{
  				// the back sector's floor obstructs part of this wall				
! 				if (ffh1>bch1 && ffh2>bch2) 
  				{
! 					bch2a=ffh2;
! 					bch1a=ffh1;
  				}
  			}
  
! 			if (bch1a<fch1 || bch2a<fch2)
  			{
! 				gltexture=FMaterial::ValidateTexture(seg->sidedef->GetTexture(side_t::top), true);
! 				if (gltexture) 
  				{
! 					DoTexture(RENDERWALL_TOP,seg,(seg->linedef->flags & (ML_DONTPEGTOP))==0,
! 						realfront->GetPlaneTexZ(sector_t::ceiling),realback->GetPlaneTexZ(sector_t::ceiling),
! 						fch1,fch2,bch1a,bch2a,0);
  				}
! 				else if ((frontsector->ceilingplane.a | frontsector->ceilingplane.b | 
! 						 backsector->ceilingplane.a | backsector->ceilingplane.b) && 
! 						frontsector->GetTexture(sector_t::ceiling)!=skyflatnum &&
! 						backsector->GetTexture(sector_t::ceiling)!=skyflatnum)
  				{
! 					gltexture=FMaterial::ValidateTexture(frontsector->GetTexture(sector_t::ceiling), true);
  					if (gltexture)
  					{
! 						DoTexture(RENDERWALL_TOP,seg,(seg->linedef->flags & (ML_DONTPEGTOP))==0,
! 							realfront->GetPlaneTexZ(sector_t::ceiling),realback->GetPlaneTexZ(sector_t::ceiling),
! 							fch1,fch2,bch1a,bch2a,0);
  					}
  				}
  				else if (!(seg->sidedef->Flags & WALLF_POLYOBJ))
  				{
! 					gl_drawinfo->AddUpperMissingTexture(seg->sidedef, sub, bch1a);
  				}
  			}
  		}
***************
*** 1649,1713 ****
  			{
  				tex = tex->GetRawTexture();
  			}
! 			gltexture = FMaterial::ValidateTexture(tex, false);
  		}
  		else gltexture = NULL;
  
  		if (gltexture || drawfogboundary)
  		{
! 			DoMidTexture(seg, drawfogboundary, frontsector, backsector, realfront, realback,
  				fch1, fch2, ffh1, ffh2, bch1, bch2, bfh1, bfh2);
  		}
  
! 		if (backsector->e->XFloor.ffloors.Size() || frontsector->e->XFloor.ffloors.Size())
  		{
! 			DoFFloorBlocks(seg, frontsector, backsector, fch1, fch2, ffh1, ffh2, bch1, bch2, bfh1, bfh2);
  		}
! 
  		/* bottom texture */
  		// the back sector's ceiling obstructs part of this wall (specially important for sky sectors)
  		if (fch1<bfh1 && fch2<bfh2)
  		{
! 			bfh1 = fch1;
! 			bfh2 = fch2;
  		}
  
  		if (bfh1>ffh1 || bfh2>ffh2)
  		{
! 			gltexture = FMaterial::ValidateTexture(seg->sidedef->GetTexture(side_t::bottom), false, true);
! 			if (gltexture)
  			{
! 				DoTexture(RENDERWALL_BOTTOM, seg, (seg->linedef->flags & ML_DONTPEGBOTTOM) > 0,
! 					realback->GetPlaneTexZ(sector_t::floor), realfront->GetPlaneTexZ(sector_t::floor),
! 					bfh1, bfh2, ffh1, ffh2,
! 					frontsector->GetTexture(sector_t::ceiling) == skyflatnum && backsector->GetTexture(sector_t::ceiling) == skyflatnum ?
! 					realfront->GetPlaneTexZ(sector_t::floor) - realback->GetPlaneTexZ(sector_t::ceiling) :
! 					realfront->GetPlaneTexZ(sector_t::floor) - realfront->GetPlaneTexZ(sector_t::ceiling));
! 			}
! 			else if ((frontsector->floorplane.a | frontsector->floorplane.b |
! 				backsector->floorplane.a | backsector->floorplane.b) &&
! 				frontsector->GetTexture(sector_t::floor) != skyflatnum &&
! 				backsector->GetTexture(sector_t::floor) != skyflatnum)
  			{
  				// render it anyway with the sector's floor texture. With a background sky
  				// there are ugly holes otherwise and slopes are simply not precise enough
  				// to mach in any case.
! 				gltexture = FMaterial::ValidateTexture(frontsector->GetTexture(sector_t::floor), false, true);
  				if (gltexture)
  				{
! 					DoTexture(RENDERWALL_BOTTOM, seg, (seg->linedef->flags & ML_DONTPEGBOTTOM) > 0,
! 						realback->GetPlaneTexZ(sector_t::floor), realfront->GetPlaneTexZ(sector_t::floor),
! 						bfh1, bfh2, ffh1, ffh2, realfront->GetPlaneTexZ(sector_t::floor) - realfront->GetPlaneTexZ(sector_t::ceiling));
  				}
  			}
! 			else if (backsector->GetTexture(sector_t::floor) != skyflatnum &&
  				!(seg->sidedef->Flags & WALLF_POLYOBJ))
  			{
! 				// skip processing if the back is a malformed subsector
! 				if (!(seg->PartnerSeg->Subsector->hacked & 4))
! 				{
! 					gl_drawinfo->AddLowerMissingTexture(seg->sidedef, sub, bfh1);
! 				}
  			}
  		}
  	}
--- 1675,1735 ----
  			{
  				tex = tex->GetRawTexture();
  			}
! 			gltexture=FMaterial::ValidateTexture(tex);
  		}
  		else gltexture = NULL;
  
  		if (gltexture || drawfogboundary)
  		{
! 			DoMidTexture(seg, drawfogboundary, frontsector, backsector, realfront, realback, 
  				fch1, fch2, ffh1, ffh2, bch1, bch2, bfh1, bfh2);
  		}
  
! 		if (backsector->e->XFloor.ffloors.Size() || frontsector->e->XFloor.ffloors.Size()) 
  		{
! 			DoFFloorBlocks(seg,frontsector,backsector, fch1, fch2, ffh1, ffh2, bch1, bch2, bfh1, bfh2);
  		}
! 		
  		/* bottom texture */
  		// the back sector's ceiling obstructs part of this wall (specially important for sky sectors)
  		if (fch1<bfh1 && fch2<bfh2)
  		{
! 			bfh1=fch1;
! 			bfh2=fch2;
  		}
  
  		if (bfh1>ffh1 || bfh2>ffh2)
  		{
! 			gltexture=FMaterial::ValidateTexture(seg->sidedef->GetTexture(side_t::bottom), true);
! 			if (gltexture) 
  			{
! 				DoTexture(RENDERWALL_BOTTOM,seg,(seg->linedef->flags & ML_DONTPEGBOTTOM)>0,
! 					realback->GetPlaneTexZ(sector_t::floor),realfront->GetPlaneTexZ(sector_t::floor),
! 					bfh1,bfh2,ffh1,ffh2,
! 					frontsector->GetTexture(sector_t::ceiling)==skyflatnum && backsector->GetTexture(sector_t::ceiling)==skyflatnum ?
! 						realfront->GetPlaneTexZ(sector_t::floor)-realback->GetPlaneTexZ(sector_t::ceiling) : 
! 						realfront->GetPlaneTexZ(sector_t::floor)-realfront->GetPlaneTexZ(sector_t::ceiling));
! 			}
! 			else if ((frontsector->floorplane.a | frontsector->floorplane.b | 
! 					backsector->floorplane.a | backsector->floorplane.b) && 
! 					frontsector->GetTexture(sector_t::floor)!=skyflatnum &&
! 					backsector->GetTexture(sector_t::floor)!=skyflatnum)
  			{
  				// render it anyway with the sector's floor texture. With a background sky
  				// there are ugly holes otherwise and slopes are simply not precise enough
  				// to mach in any case.
! 				gltexture=FMaterial::ValidateTexture(frontsector->GetTexture(sector_t::floor), true);
  				if (gltexture)
  				{
! 					DoTexture(RENDERWALL_BOTTOM,seg,(seg->linedef->flags & ML_DONTPEGBOTTOM)>0,
! 						realback->GetPlaneTexZ(sector_t::floor),realfront->GetPlaneTexZ(sector_t::floor),
! 						bfh1,bfh2,ffh1,ffh2, realfront->GetPlaneTexZ(sector_t::floor)-realfront->GetPlaneTexZ(sector_t::ceiling));
  				}
  			}
! 			else if (backsector->GetTexture(sector_t::floor)!=skyflatnum && 
  				!(seg->sidedef->Flags & WALLF_POLYOBJ))
  			{
! 				gl_drawinfo->AddLowerMissingTexture(seg->sidedef, sub, bfh1);
  			}
  		}
  	}
***************
*** 1720,1738 ****
  //==========================================================================
  void GLWall::ProcessLowerMiniseg(seg_t *seg, sector_t * frontsector, sector_t * backsector)
  {
! 	if (frontsector->GetTexture(sector_t::floor) == skyflatnum) return;
  
! 	fixed_t ffh = frontsector->GetPlaneTexZ(sector_t::floor);
! 	fixed_t bfh = backsector->GetPlaneTexZ(sector_t::floor);
! 	if (bfh > ffh)
  	{
  		this->seg = seg;
  		this->sub = NULL;
  
! 		vertex_t * v1 = seg->v1;
! 		vertex_t * v2 = seg->v2;
! 		vertexes[0] = v1;
! 		vertexes[1] = v2;
  
  		glseg.x1 = FIXED2FLOAT(v1->x);
  		glseg.y1 = FIXED2FLOAT(v1->y);
--- 1742,1760 ----
  //==========================================================================
  void GLWall::ProcessLowerMiniseg(seg_t *seg, sector_t * frontsector, sector_t * backsector)
  {
! 	if (frontsector->GetTexture(sector_t::floor)==skyflatnum) return;
  
! 	fixed_t ffh = frontsector->GetPlaneTexZ(sector_t::floor); 
! 	fixed_t bfh = backsector->GetPlaneTexZ(sector_t::floor); 
! 	if (bfh>ffh)
  	{
  		this->seg = seg;
  		this->sub = NULL;
  
! 		vertex_t * v1=seg->v1;
! 		vertex_t * v2=seg->v2;
! 		vertexes[0]=v1;
! 		vertexes[1]=v2;
  
  		glseg.x1 = FIXED2FLOAT(v1->x);
  		glseg.y1 = FIXED2FLOAT(v1->y);
***************
*** 1741,1747 ****
  		glseg.fracleft = 0;
  		glseg.fracright = 1;
  
! 		flags = 0;
  
  		// can't do fake contrast without a sidedef
  		lightlevel = gl_ClampLight(frontsector->lightlevel);
--- 1763,1769 ----
  		glseg.fracleft = 0;
  		glseg.fracright = 1;
  
! 		flags = (!gl_isBlack(Colormap.FadeColor) || level.flags&LEVEL_HASFADETABLE)? GLWF_FOGGY : 0;
  
  		// can't do fake contrast without a sidedef
  		lightlevel = gl_ClampLight(frontsector->lightlevel);
***************
*** 1753,1770 ****
  
  		topflat = frontsector->GetTexture(sector_t::ceiling);	// for glowing textures
  		bottomflat = frontsector->GetTexture(sector_t::floor);
- 		topplane = frontsector->ceilingplane;
- 		bottomplane = frontsector->floorplane;
- 		dynlightindex = UINT_MAX;
  
  		zfloor[0] = zfloor[1] = FIXED2FLOAT(ffh);
  
! 		gltexture = FMaterial::ValidateTexture(frontsector->GetTexture(sector_t::floor), false, true);
  
! 		if (gltexture)
  		{
  			FTexCoordInfo tci;
! 			type = RENDERWALL_BOTTOM;
  			gltexture->GetTexCoordInfo(&tci, FRACUNIT, FRACUNIT);
  			SetWallCoordinates(seg, &tci, FIXED2FLOAT(bfh), bfh, bfh, ffh, ffh, 0);
  			PutWall(false);
--- 1775,1789 ----
  
  		topflat = frontsector->GetTexture(sector_t::ceiling);	// for glowing textures
  		bottomflat = frontsector->GetTexture(sector_t::floor);
  
  		zfloor[0] = zfloor[1] = FIXED2FLOAT(ffh);
  
! 		gltexture = FMaterial::ValidateTexture(frontsector->GetTexture(sector_t::floor), true);
  
! 		if (gltexture) 
  		{
  			FTexCoordInfo tci;
! 			type=RENDERWALL_BOTTOM;
  			gltexture->GetTexCoordInfo(&tci, FRACUNIT, FRACUNIT);
  			SetWallCoordinates(seg, &tci, FIXED2FLOAT(bfh), bfh, bfh, ffh, ffh, 0);
  			PutWall(false);
