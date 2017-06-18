*** doom/src/main/jni/gzdoom/src/gl/scene/gl_renderhacks.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_renderhacks.cpp	2017-06-18 23:34:13.330581864 -0400
***************
*** 48,53 ****
--- 48,54 ----
  #include "gl/renderer/gl_renderer.h"
  #include "gl/data/gl_data.h"
  #include "gl/dynlights/gl_glow.h"
+ #include "gl/dynlights/gl_lightbuffer.h"
  #include "gl/scene/gl_drawinfo.h"
  #include "gl/scene/gl_portal.h"
  #include "gl/utility/gl_clock.h"
***************
*** 162,167 ****
--- 163,169 ----
  				return;
  			}
  
+ 			//@sync-hack
  			for(unsigned int i=0;i<MissingUpperTextures.Size();i++)
  			{
  				if (MissingUpperTextures[i].sub == sub)
***************
*** 228,239 ****
  			}
  
  			// Ignore FF_FIX's because they are designed to abuse missing textures
! 			if (seg->backsector->e->XFloor.ffloors.Size() && (seg->backsector->e->XFloor.ffloors[0]->flags&(FF_FIX|FF_SEETHROUGH)) == FF_FIX)
  			{
  				totalms.Unclock();
  				return;
  			}
  
  			for(unsigned int i=0;i<MissingLowerTextures.Size();i++)
  			{
  				if (MissingLowerTextures[i].sub == sub)
--- 230,242 ----
  			}
  
  			// Ignore FF_FIX's because they are designed to abuse missing textures
! 			if (seg->backsector->e->XFloor.ffloors.Size() && seg->backsector->e->XFloor.ffloors[0]->flags&FF_FIX)
  			{
  				totalms.Unclock();
  				return;
  			}
  
+ 			//@sync-hack
  			for(unsigned int i=0;i<MissingLowerTextures.Size();i++)
  			{
  				if (MissingLowerTextures[i].sub == sub)
***************
*** 730,735 ****
--- 733,739 ----
  {
  	if (!(level.maptype == MAPTYPE_HEXEN))
  	{
+ 		//@sync-hack (probably not, this is only called from the main thread)
  		SubsectorHackInfo sh={sub, 0};
  		SubsectorHacks.Push (sh);
  	}
***************
*** 1029,1039 ****
--- 1033,1045 ----
  
  void FDrawInfo::AddFloorStack(sector_t * sec)
  {
+ 	//@sync-hack
  	FloorStacks.Push(sec);
  }
  
  void FDrawInfo::AddCeilingStack(sector_t * sec)
  {
+ 	//@sync-hack
  	CeilingStacks.Push(sec);
  }
  
