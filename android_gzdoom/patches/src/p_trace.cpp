*** /home/draje/Code/gzdoom/src/p_trace.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_trace.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 366,372 ****
  				Results->HitTexture = CurSector->GetTexture(sector_t::ceiling);
  			}
  			else if (entersector == NULL ||
! 				hitz <= bf || hitz >= bc ||
  				in->d.line->flags & WallMask)
  			{ // hit the wall
  				Results->HitType = TRACE_HitWall;
--- 366,372 ----
  				Results->HitTexture = CurSector->GetTexture(sector_t::ceiling);
  			}
  			else if (entersector == NULL ||
! 				hitz < bf || hitz > bc ||
  				in->d.line->flags & WallMask)
  			{ // hit the wall
  				Results->HitType = TRACE_HitWall;
