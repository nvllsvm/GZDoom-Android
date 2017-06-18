*** /home/draje/Code/gzdoom/src/p_floor.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_floor.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 320,326 ****
  		rtn = true;
  		floor = new DFloor (sec);
  		floor->m_Type = floortype;
! 		floor->m_Crush = -1;
  		floor->m_Hexencrush = hexencrush;
  		floor->m_Speed = speed;
  		floor->m_ResetCount = 0;				// [RH]
--- 320,326 ----
  		rtn = true;
  		floor = new DFloor (sec);
  		floor->m_Type = floortype;
! 		floor->m_Crush = crush;
  		floor->m_Hexencrush = hexencrush;
  		floor->m_Speed = speed;
  		floor->m_ResetCount = 0;				// [RH]
***************
*** 374,380 ****
  			break;
  
  		case DFloor::floorRaiseAndCrushDoom:
- 			floor->m_Crush = crush;
  		case DFloor::floorRaiseToLowestCeiling:
  			floor->m_Direction = 1;
  			newheight = sec->FindLowestCeilingSurrounding (&spot);
--- 374,379 ----
***************
*** 406,412 ****
  			break;
  
  		case DFloor::floorRaiseAndCrush:
- 			floor->m_Crush = crush;
  			floor->m_Direction = 1;
  			newheight = sec->FindLowestCeilingPoint (&spot) - 8*FRACUNIT;
  			floor->m_FloorDestDist = sec->floorplane.PointToDist (spot, newheight);
--- 405,410 ----
