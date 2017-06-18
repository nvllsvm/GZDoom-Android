*** /home/draje/Code/gzdoom/src/b_move.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/b_move.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 17,36 ****
  #include "gi.h"
  #include "a_keys.h"
  #include "d_event.h"
! 
! enum dirtype_t
! {
!     DI_EAST,
!     DI_NORTHEAST,
!     DI_NORTH,
!     DI_NORTHWEST,
!     DI_WEST,
!     DI_SOUTHWEST,
!     DI_SOUTH,
!     DI_SOUTHEAST,
!     DI_NODIR,
!     NUMDIRS
! };
  
  static FRandom pr_botopendoor ("BotOpenDoor");
  static FRandom pr_bottrywalk ("BotTryWalk");
--- 17,23 ----
  #include "gi.h"
  #include "a_keys.h"
  #include "d_event.h"
! #include "p_enemy.h"
  
  static FRandom pr_botopendoor ("BotOpenDoor");
  static FRandom pr_bottrywalk ("BotTryWalk");
***************
*** 39,100 ****
  // borrow some tables from p_enemy.cpp
  extern dirtype_t opposite[9];
  extern dirtype_t diags[4];
- extern fixed_t xspeed[8];
- extern fixed_t yspeed[8];
  
! extern TArray<line_t *> spechit;
! 
! //Called while the bot moves after its player->dest mobj
  //which can be a weapon/enemy/item whatever.
! void FCajunMaster::Roam (AActor *actor, ticcmd_t *cmd)
  {
  	int delta;
  
! 	if (Reachable(actor, actor->player->dest))
  	{ // Straight towards it.
! 		actor->player->angle = R_PointToAngle2(actor->x, actor->y, actor->player->dest->x, actor->player->dest->y);
  	}
! 	else if (actor->movedir < 8) // turn towards movement direction if not there yet
  	{
! 		actor->player->angle &= (angle_t)(7<<29);
! 		delta = actor->player->angle - (actor->movedir << 29);
  
  		if (delta > 0)
! 			actor->player->angle -= ANG45;
  		else if (delta < 0)
! 			actor->player->angle += ANG45;
  	}
  
  	// chase towards destination.
! 	if (--actor->movecount < 0 || !Move (actor, cmd))
  	{
! 		NewChaseDir (actor, cmd);
  	}
  }
  
! bool FCajunMaster::Move (AActor *actor, ticcmd_t *cmd)
  {
  	fixed_t tryx, tryy;
  	bool try_ok;
  	int good;
  
! 	if (actor->movedir == DI_NODIR)
  		return false;
  
! 	if ((unsigned)actor->movedir >= 8)
  		I_Error ("Weird bot movedir!");
  
! 	tryx = actor->x + 8*xspeed[actor->movedir];
! 	tryy = actor->y + 8*yspeed[actor->movedir];
  
! 	try_ok = CleanAhead (actor, tryx, tryy, cmd);
  
  	if (!try_ok) //Anything blocking that could be opened etc..
  	{
  		if (!spechit.Size ())
  			return false;
  
! 		actor->movedir = DI_NODIR;
  
  		good = 0;
  		line_t *ld;
--- 26,83 ----
  // borrow some tables from p_enemy.cpp
  extern dirtype_t opposite[9];
  extern dirtype_t diags[4];
  
! //Called while the bot moves after its dest mobj
  //which can be a weapon/enemy/item whatever.
! void DBot::Roam (ticcmd_t *cmd)
  {
  	int delta;
  
! 	if (Reachable(dest))
  	{ // Straight towards it.
! 		angle = R_PointToAngle2(player->mo->x, player->mo->y, dest->x, dest->y);
  	}
! 	else if (player->mo->movedir < 8) // turn towards movement direction if not there yet
  	{
! 		angle &= (angle_t)(7<<29);
! 		delta = angle - (player->mo->movedir << 29);
  
  		if (delta > 0)
! 			angle -= ANG45;
  		else if (delta < 0)
! 			angle += ANG45;
  	}
  
  	// chase towards destination.
! 	if (--player->mo->movecount < 0 || !Move (cmd))
  	{
! 		NewChaseDir (cmd);
  	}
  }
  
! bool DBot::Move (ticcmd_t *cmd)
  {
  	fixed_t tryx, tryy;
  	bool try_ok;
  	int good;
  
! 	if (player->mo->movedir == DI_NODIR)
  		return false;
  
! 	if ((unsigned)player->mo->movedir >= 8)
  		I_Error ("Weird bot movedir!");
  
! 	tryx = player->mo->x + 8*xspeed[player->mo->movedir];
! 	tryy = player->mo->y + 8*yspeed[player->mo->movedir];
  
! 	try_ok = bglobal.CleanAhead (player->mo, tryx, tryy, cmd);
  
  	if (!try_ok) //Anything blocking that could be opened etc..
  	{
  		if (!spechit.Size ())
  			return false;
  
! 		player->mo->movedir = DI_NODIR;
  
  		good = 0;
  		line_t *ld;
***************
*** 103,118 ****
  		{
  			bool tryit = true;
  
! 			if (ld->special == Door_LockedRaise && !P_CheckKeys (actor, ld->args[3], false))
  				tryit = false;
! 			else if (ld->special == Generic_Door && !P_CheckKeys (actor, ld->args[4], false))
  				tryit = false;
  
  			if (tryit &&
! 				(P_TestActivateLine (ld, actor, 0, SPAC_Use) ||
! 				 P_TestActivateLine (ld, actor, 0, SPAC_Push)))
  			{
! 				good |= ld == actor->BlockingLine ? 1 : 2;
  			}
  		}
  		if (good && ((pr_botopendoor() >= 203) ^ (good & 1)))
--- 86,101 ----
  		{
  			bool tryit = true;
  
! 			if (ld->special == Door_LockedRaise && !P_CheckKeys (player->mo, ld->args[3], false))
  				tryit = false;
! 			else if (ld->special == Generic_Door && !P_CheckKeys (player->mo, ld->args[4], false))
  				tryit = false;
  
  			if (tryit &&
! 				(P_TestActivateLine (ld, player->mo, 0, SPAC_Use) ||
! 				 P_TestActivateLine (ld, player->mo, 0, SPAC_Push)))
  			{
! 				good |= ld == player->mo->BlockingLine ? 1 : 2;
  			}
  		}
  		if (good && ((pr_botopendoor() >= 203) ^ (good & 1)))
***************
*** 130,145 ****
  	return true;
  }
  
! bool FCajunMaster::TryWalk (AActor *actor, ticcmd_t *cmd)
  {
!     if (!Move (actor, cmd))
          return false;
  
!     actor->movecount = pr_bottrywalk() & 60;
      return true;
  }
  
! void FCajunMaster::NewChaseDir (AActor *actor, ticcmd_t *cmd)
  {
      fixed_t     deltax;
      fixed_t     deltay;
--- 113,128 ----
  	return true;
  }
  
! bool DBot::TryWalk (ticcmd_t *cmd)
  {
!     if (!Move (cmd))
          return false;
  
!     player->mo->movecount = pr_bottrywalk() & 60;
      return true;
  }
  
! void DBot::NewChaseDir (ticcmd_t *cmd)
  {
      fixed_t     deltax;
      fixed_t     deltay;
***************
*** 151,157 ****
  
      dirtype_t   turnaround;
  
!     if (!actor->player->dest)
  	{
  #ifndef BOT_RELEASE_COMPILE
          Printf ("Bot tried move without destination\n");
--- 134,140 ----
  
      dirtype_t   turnaround;
  
!     if (!dest)
  	{
  #ifndef BOT_RELEASE_COMPILE
          Printf ("Bot tried move without destination\n");
***************
*** 159,169 ****
  		return;
  	}
  
!     olddir = (dirtype_t)actor->movedir;
      turnaround = opposite[olddir];
  
!     deltax = actor->player->dest->x - actor->x;
!     deltay = actor->player->dest->y - actor->y;
  
      if (deltax > 10*FRACUNIT)
          d[1] = DI_EAST;
--- 142,152 ----
  		return;
  	}
  
!     olddir = (dirtype_t)player->mo->movedir;
      turnaround = opposite[olddir];
  
!     deltax = dest->x - player->mo->x;
!     deltay = dest->y - player->mo->y;
  
      if (deltax > 10*FRACUNIT)
          d[1] = DI_EAST;
***************
*** 182,189 ****
      // try direct route
      if (d[1] != DI_NODIR && d[2] != DI_NODIR)
      {
!         actor->movedir = diags[((deltay<0)<<1)+(deltax>0)];
!         if (actor->movedir != turnaround && TryWalk(actor, cmd))
              return;
      }
  
--- 165,172 ----
      // try direct route
      if (d[1] != DI_NODIR && d[2] != DI_NODIR)
      {
!         player->mo->movedir = diags[((deltay<0)<<1)+(deltax>0)];
!         if (player->mo->movedir != turnaround && TryWalk(cmd))
              return;
      }
  
***************
*** 203,218 ****
  
      if (d[1]!=DI_NODIR)
      {
!         actor->movedir = d[1];
!         if (TryWalk (actor, cmd))
              return;
      }
  
      if (d[2]!=DI_NODIR)
      {
!         actor->movedir = d[2];
  
!         if (TryWalk(actor, cmd))
              return;
      }
  
--- 186,201 ----
  
      if (d[1]!=DI_NODIR)
      {
!         player->mo->movedir = d[1];
!         if (TryWalk (cmd))
              return;
      }
  
      if (d[2]!=DI_NODIR)
      {
!         player->mo->movedir = d[2];
  
!         if (TryWalk(cmd))
              return;
      }
  
***************
*** 220,228 ****
      // so pick another direction.
      if (olddir!=DI_NODIR)
      {
!         actor->movedir = olddir;
  
!         if (TryWalk(actor, cmd))
              return;
      }
  
--- 203,211 ----
      // so pick another direction.
      if (olddir!=DI_NODIR)
      {
!         player->mo->movedir = olddir;
  
!         if (TryWalk(cmd))
              return;
      }
  
***************
*** 235,243 ****
          {
              if (tdir!=turnaround)
              {
!                 actor->movedir = tdir;
  
!                 if (TryWalk(actor, cmd))
                      return;
              }
          }
--- 218,226 ----
          {
              if (tdir!=turnaround)
              {
!                 player->mo->movedir = tdir;
  
!                 if (TryWalk(cmd))
                      return;
              }
          }
***************
*** 250,258 ****
          {
              if (tdir!=turnaround)
              {
!                 actor->movedir = tdir;
  
!                 if (TryWalk(actor, cmd))
                      return;
              }
          }
--- 233,241 ----
          {
              if (tdir!=turnaround)
              {
!                 player->mo->movedir = tdir;
  
!                 if (TryWalk(cmd))
                      return;
              }
          }
***************
*** 260,271 ****
  
      if (turnaround !=  DI_NODIR)
      {
!         actor->movedir = turnaround;
!         if (TryWalk(actor, cmd))
              return;
      }
  
!     actor->movedir = DI_NODIR;  // can not move
  }
  
  
--- 243,254 ----
  
      if (turnaround !=  DI_NODIR)
      {
!         player->mo->movedir = turnaround;
!         if (TryWalk(cmd))
              return;
      }
  
!     player->mo->movedir = DI_NODIR;  // can not move
  }
  
  
***************
*** 324,371 ****
  #define MAXTURN (15*ANGLE_1) //Max degrees turned in one tic. Lower is smother but may cause the bot not getting where it should = crash
  #define TURNSENS 3 //Higher is smoother but slower turn.
  
! void FCajunMaster::TurnToAng (AActor *actor)
  {
      int maxturn = MAXTURN;
  
! 	if (actor->player->ReadyWeapon != NULL)
  	{
! 		if (actor->player->ReadyWeapon->WeaponFlags & WIF_BOT_EXPLOSIVE)
  		{
! 			if (actor->player->t_roam && !actor->player->missile)
  			{ //Keep angle that where when shot where decided.
  				return;
  			}
  		}
  
  
! 		if(actor->player->enemy)
! 			if(!actor->player->dest) //happens when running after item in combat situations, or normal, prevents weak turns
! 				if(actor->player->ReadyWeapon->ProjectileType == NULL && !(actor->player->ReadyWeapon->WeaponFlags & WIF_MELEEWEAPON))
! 					if(Check_LOS(actor, actor->player->enemy, SHOOTFOV+5*ANGLE_1))
  						maxturn = 3;
  	}
  
! 	int distance = actor->player->angle - actor->angle;
  
! 	if (abs (distance) < OKAYRANGE && !actor->player->enemy)
  		return;
  
  	distance /= TURNSENS;
  	if (abs (distance) > maxturn)
  		distance = distance < 0 ? -maxturn : maxturn;
  
! 	actor->angle += distance;
  }
  
! void FCajunMaster::Pitch (AActor *actor, AActor *target)
  {
  	double aim;
  	double diff;
  
! 	diff = target->z - actor->z;
! 	aim = atan (diff / (double)P_AproxDistance (actor->x - target->x, actor->y - target->y));
! 	actor->pitch = -(int)(aim * ANGLE_180/M_PI);
  }
  
  //Checks if a sector is dangerous.
--- 307,354 ----
  #define MAXTURN (15*ANGLE_1) //Max degrees turned in one tic. Lower is smother but may cause the bot not getting where it should = crash
  #define TURNSENS 3 //Higher is smoother but slower turn.
  
! void DBot::TurnToAng ()
  {
      int maxturn = MAXTURN;
  
! 	if (player->ReadyWeapon != NULL)
  	{
! 		if (player->ReadyWeapon->WeaponFlags & WIF_BOT_EXPLOSIVE)
  		{
! 			if (t_roam && !missile)
  			{ //Keep angle that where when shot where decided.
  				return;
  			}
  		}
  
  
! 		if(enemy)
! 			if(!dest) //happens when running after item in combat situations, or normal, prevents weak turns
! 				if(player->ReadyWeapon->ProjectileType == NULL && !(player->ReadyWeapon->WeaponFlags & WIF_MELEEWEAPON))
! 					if(Check_LOS(enemy, SHOOTFOV+5*ANGLE_1))
  						maxturn = 3;
  	}
  
! 	int distance = angle - player->mo->angle;
  
! 	if (abs (distance) < OKAYRANGE && !enemy)
  		return;
  
  	distance /= TURNSENS;
  	if (abs (distance) > maxturn)
  		distance = distance < 0 ? -maxturn : maxturn;
  
! 	player->mo->angle += distance;
  }
  
! void DBot::Pitch (AActor *target)
  {
  	double aim;
  	double diff;
  
! 	diff = target->z - player->mo->z;
! 	aim = atan (diff / (double)P_AproxDistance (player->mo->x - target->x, player->mo->y - target->y));
! 	player->mo->pitch = -(int)(aim * ANGLE_180/M_PI);
  }
  
  //Checks if a sector is dangerous.
***************
*** 388,391 ****
  		|| special == Damage_InstantDeath
  		|| special == sDamage_SuperHellslime;
  }
- 
--- 371,373 ----
