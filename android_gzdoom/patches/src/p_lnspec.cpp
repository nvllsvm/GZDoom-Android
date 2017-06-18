*** /home/draje/Code/gzdoom/src/p_lnspec.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_lnspec.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 281,323 ****
  FUNC(LS_Floor_LowerByValue)
  // Floor_LowerByValue (tag, speed, height)
  {
! 	return EV_DoFloor (DFloor::floorLowerByValue, ln, arg0, SPEED(arg1), FRACUNIT*arg2, 0, 0, false);
  }
  
  FUNC(LS_Floor_LowerToLowest)
  // Floor_LowerToLowest (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorLowerToLowest, ln, arg0, SPEED(arg1), 0, 0, 0, false);
  }
  
  FUNC(LS_Floor_LowerToHighest)
  // Floor_LowerToHighest (tag, speed, adjust, hereticlower)
  {
! 	return EV_DoFloor (DFloor::floorLowerToHighest, ln, arg0, SPEED(arg1), (arg2-128)*FRACUNIT, 0, 0, false, arg3==1);
  }
  
  FUNC(LS_Floor_LowerToNearest)
  // Floor_LowerToNearest (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorLowerToNearest, ln, arg0, SPEED(arg1), 0, 0, 0, false);
  }
  
  FUNC(LS_Floor_RaiseByValue)
  // Floor_RaiseByValue (tag, speed, height)
  {
! 	return EV_DoFloor (DFloor::floorRaiseByValue, ln, arg0, SPEED(arg1), FRACUNIT*arg2, 0, 0, false);
  }
  
  FUNC(LS_Floor_RaiseToHighest)
  // Floor_RaiseToHighest (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorRaiseToHighest, ln, arg0, SPEED(arg1), 0, 0, 0, false);
  }
  
  FUNC(LS_Floor_RaiseToNearest)
  // Floor_RaiseToNearest (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorRaiseToNearest, ln, arg0, SPEED(arg1), 0, 0, 0, false);
  }
  
  FUNC(LS_Floor_RaiseAndCrush)
--- 281,323 ----
  FUNC(LS_Floor_LowerByValue)
  // Floor_LowerByValue (tag, speed, height)
  {
! 	return EV_DoFloor (DFloor::floorLowerByValue, ln, arg0, SPEED(arg1), FRACUNIT*arg2, -1, 0, false);
  }
  
  FUNC(LS_Floor_LowerToLowest)
  // Floor_LowerToLowest (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorLowerToLowest, ln, arg0, SPEED(arg1), 0, -1, 0, false);
  }
  
  FUNC(LS_Floor_LowerToHighest)
  // Floor_LowerToHighest (tag, speed, adjust, hereticlower)
  {
! 	return EV_DoFloor (DFloor::floorLowerToHighest, ln, arg0, SPEED(arg1), (arg2-128)*FRACUNIT, -1, 0, false, arg3==1);
  }
  
  FUNC(LS_Floor_LowerToNearest)
  // Floor_LowerToNearest (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorLowerToNearest, ln, arg0, SPEED(arg1), 0, -1, 0, false);
  }
  
  FUNC(LS_Floor_RaiseByValue)
  // Floor_RaiseByValue (tag, speed, height)
  {
! 	return EV_DoFloor (DFloor::floorRaiseByValue, ln, arg0, SPEED(arg1), FRACUNIT*arg2, -1, 0, false);
  }
  
  FUNC(LS_Floor_RaiseToHighest)
  // Floor_RaiseToHighest (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorRaiseToHighest, ln, arg0, SPEED(arg1), 0, -1, 0, false);
  }
  
  FUNC(LS_Floor_RaiseToNearest)
  // Floor_RaiseToNearest (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorRaiseToNearest, ln, arg0, SPEED(arg1), 0, -1, 0, false);
  }
  
  FUNC(LS_Floor_RaiseAndCrush)
***************
*** 335,347 ****
  FUNC(LS_Floor_RaiseByValueTimes8)
  // FLoor_RaiseByValueTimes8 (tag, speed, height)
  {
! 	return EV_DoFloor (DFloor::floorRaiseByValue, ln, arg0, SPEED(arg1), FRACUNIT*arg2*8, 0, 0, false);
  }
  
  FUNC(LS_Floor_LowerByValueTimes8)
  // Floor_LowerByValueTimes8 (tag, speed, height)
  {
! 	return EV_DoFloor (DFloor::floorLowerByValue, ln, arg0, SPEED(arg1), FRACUNIT*arg2*8, 0, 0, false);
  }
  
  FUNC(LS_Floor_CrushStop)
--- 335,347 ----
  FUNC(LS_Floor_RaiseByValueTimes8)
  // FLoor_RaiseByValueTimes8 (tag, speed, height)
  {
! 	return EV_DoFloor (DFloor::floorRaiseByValue, ln, arg0, SPEED(arg1), FRACUNIT*arg2*8, -1, 0, false);
  }
  
  FUNC(LS_Floor_LowerByValueTimes8)
  // Floor_LowerByValueTimes8 (tag, speed, height)
  {
! 	return EV_DoFloor (DFloor::floorLowerByValue, ln, arg0, SPEED(arg1), FRACUNIT*arg2*8, -1, 0, false);
  }
  
  FUNC(LS_Floor_CrushStop)
***************
*** 353,403 ****
  FUNC(LS_Floor_LowerInstant)
  // Floor_LowerInstant (tag, unused, height)
  {
! 	return EV_DoFloor (DFloor::floorLowerInstant, ln, arg0, 0, arg2*FRACUNIT*8, 0, 0, false);
  }
  
  FUNC(LS_Floor_RaiseInstant)
  // Floor_RaiseInstant (tag, unused, height)
  {
! 	return EV_DoFloor (DFloor::floorRaiseInstant, ln, arg0, 0, arg2*FRACUNIT*8, 0, 0, false);
  }
  
  FUNC(LS_Floor_MoveToValueTimes8)
  // Floor_MoveToValueTimes8 (tag, speed, height, negative)
  {
  	return EV_DoFloor (DFloor::floorMoveToValue, ln, arg0, SPEED(arg1),
! 					   arg2*FRACUNIT*8*(arg3?-1:1), 0, 0, false);
  }
  
  FUNC(LS_Floor_MoveToValue)
  // Floor_MoveToValue (tag, speed, height, negative)
  {
  	return EV_DoFloor (DFloor::floorMoveToValue, ln, arg0, SPEED(arg1),
! 					   arg2*FRACUNIT*(arg3?-1:1), 0, 0, false);
  }
  
  FUNC(LS_Floor_RaiseToLowestCeiling)
  // Floor_RaiseToLowestCeiling (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorRaiseToLowestCeiling, ln, arg0, SPEED(arg1), 0, 0, 0, false);
  }
  
  FUNC(LS_Floor_RaiseByTexture)
  // Floor_RaiseByTexture (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorRaiseByTexture, ln, arg0, SPEED(arg1), 0, 0, 0, false);
  }
  
  FUNC(LS_Floor_RaiseByValueTxTy)
  // Floor_RaiseByValueTxTy (tag, speed, height)
  {
! 	return EV_DoFloor (DFloor::floorRaiseAndChange, ln, arg0, SPEED(arg1), arg2*FRACUNIT, 0, 0, false);
  }
  
  FUNC(LS_Floor_LowerToLowestTxTy)
  // Floor_LowerToLowestTxTy (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorLowerAndChange, ln, arg0, SPEED(arg1), arg2*FRACUNIT, 0, 0, false);
  }
  
  FUNC(LS_Floor_Waggle)
--- 353,403 ----
  FUNC(LS_Floor_LowerInstant)
  // Floor_LowerInstant (tag, unused, height)
  {
! 	return EV_DoFloor (DFloor::floorLowerInstant, ln, arg0, 0, arg2*FRACUNIT*8, -1, 0, false);
  }
  
  FUNC(LS_Floor_RaiseInstant)
  // Floor_RaiseInstant (tag, unused, height)
  {
! 	return EV_DoFloor (DFloor::floorRaiseInstant, ln, arg0, 0, arg2*FRACUNIT*8, -1, 0, false);
  }
  
  FUNC(LS_Floor_MoveToValueTimes8)
  // Floor_MoveToValueTimes8 (tag, speed, height, negative)
  {
  	return EV_DoFloor (DFloor::floorMoveToValue, ln, arg0, SPEED(arg1),
! 					   arg2*FRACUNIT*8*(arg3?-1:1), -1, 0, false);
  }
  
  FUNC(LS_Floor_MoveToValue)
  // Floor_MoveToValue (tag, speed, height, negative)
  {
  	return EV_DoFloor (DFloor::floorMoveToValue, ln, arg0, SPEED(arg1),
! 					   arg2*FRACUNIT*(arg3?-1:1), -1, 0, false);
  }
  
  FUNC(LS_Floor_RaiseToLowestCeiling)
  // Floor_RaiseToLowestCeiling (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorRaiseToLowestCeiling, ln, arg0, SPEED(arg1), 0, -1, 0, false);
  }
  
  FUNC(LS_Floor_RaiseByTexture)
  // Floor_RaiseByTexture (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorRaiseByTexture, ln, arg0, SPEED(arg1), 0, -1, 0, false);
  }
  
  FUNC(LS_Floor_RaiseByValueTxTy)
  // Floor_RaiseByValueTxTy (tag, speed, height)
  {
! 	return EV_DoFloor (DFloor::floorRaiseAndChange, ln, arg0, SPEED(arg1), arg2*FRACUNIT, -1, 0, false);
  }
  
  FUNC(LS_Floor_LowerToLowestTxTy)
  // Floor_LowerToLowestTxTy (tag, speed)
  {
! 	return EV_DoFloor (DFloor::floorLowerAndChange, ln, arg0, SPEED(arg1), arg2*FRACUNIT, -1, 0, false);
  }
  
  FUNC(LS_Floor_Waggle)
***************
*** 519,537 ****
  FUNC(LS_Pillar_Build)
  // Pillar_Build (tag, speed, height)
  {
! 	return EV_DoPillar (DPillar::pillarBuild, arg0, SPEED(arg1), arg2*FRACUNIT, 0, -1, false);
  }
  
  FUNC(LS_Pillar_BuildAndCrush)
  // Pillar_BuildAndCrush (tag, speed, height, crush, crushtype)
  {
! 	return EV_DoPillar (DPillar::pillarBuild, arg0, SPEED(arg1), arg2*FRACUNIT, 0, arg3, CRUSHTYPE(arg4));
  }
  
  FUNC(LS_Pillar_Open)
  // Pillar_Open (tag, speed, f_height, c_height)
  {
! 	return EV_DoPillar (DPillar::pillarOpen, arg0, SPEED(arg1), arg2*FRACUNIT, arg3*FRACUNIT, -1, false);
  }
  
  FUNC(LS_Ceiling_LowerByValue)
--- 519,537 ----
  FUNC(LS_Pillar_Build)
  // Pillar_Build (tag, speed, height)
  {
! 	return EV_DoPillar (DPillar::pillarBuild, ln, arg0, SPEED(arg1), arg2*FRACUNIT, 0, -1, false);
  }
  
  FUNC(LS_Pillar_BuildAndCrush)
  // Pillar_BuildAndCrush (tag, speed, height, crush, crushtype)
  {
! 	return EV_DoPillar (DPillar::pillarBuild, ln, arg0, SPEED(arg1), arg2*FRACUNIT, 0, arg3, CRUSHTYPE(arg4));
  }
  
  FUNC(LS_Pillar_Open)
  // Pillar_Open (tag, speed, f_height, c_height)
  {
! 	return EV_DoPillar (DPillar::pillarOpen, ln, arg0, SPEED(arg1), arg2*FRACUNIT, arg3*FRACUNIT, -1, false);
  }
  
  FUNC(LS_Ceiling_LowerByValue)
***************
*** 1510,1516 ****
  
  	if (arg0==0)
  	{
! 		ok = P_Thing_Raise (it);
  	}
  	else
  	{
--- 1510,1516 ----
  
  	if (arg0==0)
  	{
! 		ok = P_Thing_Raise (it,NULL);
  	}
  	else
  	{
***************
*** 1518,1524 ****
  
  		while ( (target = iterator.Next ()) )
  		{
! 			ok |= P_Thing_Raise(target);
  		}
  	}
  	return ok;
--- 1518,1524 ----
  
  		while ( (target = iterator.Next ()) )
  		{
! 			ok |= P_Thing_Raise(target,NULL);
  		}
  	}
  	return ok;
***************
*** 1764,1770 ****
  	// more or less unintuitive value for the fourth arg to trigger Boom's broken behavior
  	if (arg3 != 1998 || !res)	// (1998 for the year in which Boom was released... :P)
  	{
! 		res |= EV_DoFloor (DFloor::floorLowerToLowest, ln, arg0, SPEED(arg1), 0, 0, 0, false);
  	}
  	return res;
  }
--- 1764,1770 ----
  	// more or less unintuitive value for the fourth arg to trigger Boom's broken behavior
  	if (arg3 != 1998 || !res)	// (1998 for the year in which Boom was released... :P)
  	{
! 		res |= EV_DoFloor (DFloor::floorLowerToLowest, ln, arg0, SPEED(arg1), 0, -1, 0, false);
  	}
  	return res;
  }
