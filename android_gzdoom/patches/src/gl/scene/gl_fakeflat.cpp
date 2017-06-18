*** doom/src/main/jni/gzdoom/src/gl/scene/gl_fakeflat.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_fakeflat.cpp	2017-06-18 23:34:13.290581495 -0400
***************
*** 311,319 ****
  
  		dest->vboindex[sector_t::ceiling] = sec->vboindex[sector_t::vbo_fakefloor];
  		dest->vboheight[sector_t::ceiling] = s->vboheight[sector_t::floor];
- 
- 		dest->portals[sector_t::ceiling] = NULL;
- 
  		if (!(s->MoreFlags & SECF_NOFAKELIGHT))
  		{
  			dest->lightlevel  = s->lightlevel;
--- 311,316 ----
***************
*** 350,361 ****
  			}
  		}
  	}
! 	else if (in_area == area_above)
  	{
! 		dest->ColorMap = s->ColorMap;
  		dest->SetPlaneTexZ(sector_t::ceiling, sec->GetPlaneTexZ(sector_t::ceiling));
  		dest->SetPlaneTexZ(sector_t::floor, s->GetPlaneTexZ(sector_t::ceiling));
! 		dest->ceilingplane = sec->ceilingplane;
  		dest->floorplane = s->ceilingplane;
  		dest->floorplane.FlipVert();
  
--- 347,358 ----
  			}
  		}
  	}
! 	else if (in_area==area_above)
  	{
! 		dest->ColorMap=s->ColorMap;
  		dest->SetPlaneTexZ(sector_t::ceiling, sec->GetPlaneTexZ(sector_t::ceiling));
  		dest->SetPlaneTexZ(sector_t::floor, s->GetPlaneTexZ(sector_t::ceiling));
! 		dest->ceilingplane= sec->ceilingplane;
  		dest->floorplane = s->ceilingplane;
  		dest->floorplane.FlipVert();
  
***************
*** 365,372 ****
  		dest->vboindex[sector_t::ceiling] = sec->vboindex[sector_t::ceiling];
  		dest->vboheight[sector_t::ceiling] = sec->vboheight[sector_t::ceiling];
  
- 		dest->portals[sector_t::floor] = NULL;
- 
  		if (!(s->MoreFlags & SECF_NOFAKELIGHT))
  		{
  			dest->lightlevel  = s->lightlevel;
--- 362,367 ----
