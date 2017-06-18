*** /home/draje/Code/gzdoom/src/p_pillar.cpp	2017-06-18 07:23:17.306807250 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_pillar.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 212,227 ****
  	}
  }
  
! bool EV_DoPillar (DPillar::EPillar type, int tag, fixed_t speed, fixed_t height,
! 				  fixed_t height2, int crush, bool hexencrush)
  {
  	bool rtn = false;
- 	int secnum = -1;
  
! 	while ((secnum = P_FindSectorFromTag (tag, secnum)) >= 0)
  	{
! 		sector_t *sec = &sectors[secnum];
  
  		if (sec->PlaneMoving(sector_t::floor) || sec->PlaneMoving(sector_t::ceiling))
  			continue;
  
--- 212,239 ----
  	}
  }
  
! bool EV_DoPillar (DPillar::EPillar type, line_t *line, int tag,
! 				  fixed_t speed, fixed_t height, fixed_t height2, int crush, bool hexencrush)
  {
+ 	int secnum;
+ 	sector_t *sec;
  	bool rtn = false;
  
! 	// check if a manual trigger; if so do just the sector on the backside
! 	if (tag == 0)
  	{
! 		if (!line || !(sec = line->backsector))
! 			return rtn;
! 		secnum = (int)(sec-sectors);
! 		goto manual_pillar;
! 	}
! 
! 	secnum = -1;
! 	while (tag && (secnum = P_FindSectorFromTag (tag, secnum)) >= 0)
! 	{
! 		sec = &sectors[secnum];
  
+ manual_pillar:
  		if (sec->PlaneMoving(sector_t::floor) || sec->PlaneMoving(sector_t::ceiling))
  			continue;
  
