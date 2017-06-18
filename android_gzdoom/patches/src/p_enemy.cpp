*** /home/draje/Code/gzdoom/src/p_enemy.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_enemy.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 1592,1598 ****
  		}
  #endif
  		// [SP] If you don't see any enemies in deathmatch, look for players (but only when friend to a specific player.)
! 		if (actor->FriendPlayer == 0 && (!teamplay || actor->DesignatedTeam == TEAM_NONE)) return result;
  		if (result || !deathmatch) return true;
  
  
--- 1592,1598 ----
  		}
  #endif
  		// [SP] If you don't see any enemies in deathmatch, look for players (but only when friend to a specific player.)
! 		if (actor->FriendPlayer == 0 && (!teamplay || actor->GetTeam() == TEAM_NONE)) return result;
  		if (result || !deathmatch) return true;
  
  
***************
*** 1853,1859 ****
--- 1853,1863 ----
  
  	AActor *targ = NULL; // Shuts up gcc
  	fixed_t dist;
+ #ifdef __ANDROID__
+ 	angle_t fov = (fov_f == 0) ? ANGLE_180 : angle_t((int)(fov_f * ANGLE_90 / 90));
+ #else
  	angle_t fov = (fov_f == 0) ? ANGLE_180 : angle_t(fov_f * ANGLE_90 / 90);
+ #endif
  	FLookExParams params = { fov, minseedist, maxseedist, maxheardist, flags, seestate };
  
  	if (self->flags5 & MF5_INCONVERSATION)
***************
*** 3161,3167 ****
  	ACTION_PARAM_START(1);
  	ACTION_PARAM_NAME(damagetype, 0);
  
! 	P_DamageMobj (self, NULL, NULL, self->health, damagetype, DMG_FORCED);
  }
  
  //
--- 3165,3174 ----
  	ACTION_PARAM_START(1);
  	ACTION_PARAM_NAME(damagetype, 0);
  
! 	if (self->flags & MF_MISSILE)
! 		P_ExplodeMissile(self, NULL, NULL);
! 	else
! 		P_DamageMobj (self, NULL, NULL, self->health, damagetype, DMG_FORCED);
  }
  
  //
***************
*** 3275,3287 ****
  	{
  		if (type == NAME_Fatso)
  		{
! 			EV_DoFloor (DFloor::floorLowerToLowest, NULL, 666, FRACUNIT, 0, 0, 0, false);
  			return;
  		}
  		
  		if (type == NAME_Arachnotron)
  		{
! 			EV_DoFloor (DFloor::floorRaiseByTexture, NULL, 667, FRACUNIT, 0, 0, 0, false);
  			return;
  		}
  	}
--- 3282,3294 ----
  	{
  		if (type == NAME_Fatso)
  		{
! 			EV_DoFloor (DFloor::floorLowerToLowest, NULL, 666, FRACUNIT, 0, -1, 0, false);
  			return;
  		}
  		
  		if (type == NAME_Arachnotron)
  		{
! 			EV_DoFloor (DFloor::floorRaiseByTexture, NULL, 667, FRACUNIT, 0, -1, 0, false);
  			return;
  		}
  	}
***************
*** 3290,3300 ****
  		switch (level.flags & LEVEL_SPECACTIONSMASK)
  		{
  		case LEVEL_SPECLOWERFLOOR:
! 			EV_DoFloor (DFloor::floorLowerToLowest, NULL, 666, FRACUNIT, 0, 0, 0, false);
  			return;
  		
  		case LEVEL_SPECLOWERFLOORTOHIGHEST:
! 			EV_DoFloor (DFloor::floorLowerToHighest, NULL, 666, FRACUNIT, 0, 0, 0, false);
  			return;
  		
  		case LEVEL_SPECOPENDOOR:
--- 3297,3307 ----
  		switch (level.flags & LEVEL_SPECACTIONSMASK)
  		{
  		case LEVEL_SPECLOWERFLOOR:
! 			EV_DoFloor (DFloor::floorLowerToLowest, NULL, 666, FRACUNIT, 0, -1, 0, false);
  			return;
  		
  		case LEVEL_SPECLOWERFLOORTOHIGHEST:
! 			EV_DoFloor (DFloor::floorLowerToHighest, NULL, 666, FRACUNIT, 0, -1, 0, false);
  			return;
  		
  		case LEVEL_SPECOPENDOOR:
