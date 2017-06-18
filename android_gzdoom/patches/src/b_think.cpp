*** /home/draje/Code/gzdoom/src/b_think.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/b_think.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 24,70 ****
  
  //This function is called each tic for each bot,
  //so this is what the bot does.
! void FCajunMaster::Think (AActor *actor, ticcmd_t *cmd)
  {
  	memset (cmd, 0, sizeof(*cmd));
  
! 	if (actor->player->enemy && actor->player->enemy->health <= 0)
! 		actor->player->enemy = NULL;
  
! 	if (actor->health > 0) //Still alive
  	{
  		if (teamplay || !deathmatch)
! 			actor->player->mate = Choose_Mate (actor);
  
! 		angle_t oldyaw = actor->angle;
! 		int oldpitch = actor->pitch;
  
! 		Set_enemy (actor);
! 		ThinkForMove (actor, cmd);
! 		TurnToAng (actor);
  
! 		cmd->ucmd.yaw = (short)((actor->angle - oldyaw) >> 16) / ticdup;
! 		cmd->ucmd.pitch = (short)((oldpitch - actor->pitch) >> 16);
  		if (cmd->ucmd.pitch == -32768)
  			cmd->ucmd.pitch = -32767;
  		cmd->ucmd.pitch /= ticdup;
! 		actor->angle = oldyaw + (cmd->ucmd.yaw << 16) * ticdup;
! 		actor->pitch = oldpitch - (cmd->ucmd.pitch << 16) * ticdup;
  	}
  
! 	if (actor->player->t_active)	actor->player->t_active--;
! 	if (actor->player->t_strafe)	actor->player->t_strafe--;
! 	if (actor->player->t_react)		actor->player->t_react--;
! 	if (actor->player->t_fight)		actor->player->t_fight--;
! 	if (actor->player->t_rocket)	actor->player->t_rocket--;
! 	if (actor->player->t_roam)		actor->player->t_roam--;
  
  	//Respawn ticker
! 	if (actor->player->t_respawn)
  	{
! 		actor->player->t_respawn--;
  	}
! 	else if (actor->health <= 0)
  	{ // Time to respawn
  		cmd->ucmd.buttons |= BT_USE;
  	}
--- 24,72 ----
  
  //This function is called each tic for each bot,
  //so this is what the bot does.
! void DBot::Think ()
  {
+ 	ticcmd_t *cmd = &netcmds[player - players][((gametic + 1)/ticdup)%BACKUPTICS];
+ 
  	memset (cmd, 0, sizeof(*cmd));
  
! 	if (enemy && enemy->health <= 0)
! 		enemy = NULL;
  
! 	if (player->mo->health > 0) //Still alive
  	{
  		if (teamplay || !deathmatch)
! 			mate = Choose_Mate ();
  
! 		angle_t oldyaw = player->mo->angle;
! 		int oldpitch = player->mo->pitch;
  
! 		Set_enemy ();
! 		ThinkForMove (cmd);
! 		TurnToAng ();
  
! 		cmd->ucmd.yaw = (short)((player->mo->angle - oldyaw) >> 16) / ticdup;
! 		cmd->ucmd.pitch = (short)((oldpitch - player->mo->pitch) >> 16);
  		if (cmd->ucmd.pitch == -32768)
  			cmd->ucmd.pitch = -32767;
  		cmd->ucmd.pitch /= ticdup;
! 		player->mo->angle = oldyaw + (cmd->ucmd.yaw << 16) * ticdup;
! 		player->mo->pitch = oldpitch - (cmd->ucmd.pitch << 16) * ticdup;
  	}
  
! 	if (t_active)	t_active--;
! 	if (t_strafe)	t_strafe--;
! 	if (t_react)	t_react--;
! 	if (t_fight)	t_fight--;
! 	if (t_rocket)	t_rocket--;
! 	if (t_roam)		t_roam--;
  
  	//Respawn ticker
! 	if (t_respawn)
  	{
! 		t_respawn--;
  	}
! 	else if (player->mo->health <= 0)
  	{ // Time to respawn
  		cmd->ucmd.buttons |= BT_USE;
  	}
***************
*** 72,133 ****
  
  //how the bot moves.
  //MAIN movement function.
! void FCajunMaster::ThinkForMove (AActor *actor, ticcmd_t *cmd)
  {
- 	player_t *b;
  	fixed_t dist;
  	bool stuck;
  	int r;
  
- 	b = actor->player;
- 	if (!b->isbot)
- 		return;
- 
  	stuck = false;
! 	dist = b->dest ? P_AproxDistance(actor->x-b->dest->x, actor->y-b->dest->y) : 0;
  
! 	if (b->missile &&
! 		((!b->missile->velx || !b->missile->vely) || !Check_LOS(actor, b->missile, SHOOTFOV*3/2)))
  	{
! 		b->sleft = !b->sleft;
! 		b->missile = NULL; //Probably ended its travel.
  	}
  
! 	if (actor->pitch > 0)
! 		actor->pitch -= 80;
! 	else if (actor->pitch <= -60)
! 		actor->pitch += 80;
  
  	//HOW TO MOVE:
! 	if (b->missile && (P_AproxDistance(actor->x-b->missile->x, actor->y-b->missile->y)<AVOID_DIST)) //try avoid missile got from P_Mobj.c thinking part.
  	{
! 		Pitch (actor, b->missile);
! 		actor->player->angle = R_PointToAngle2(actor->x, actor->y, b->missile->x, b->missile->y);
! 		cmd->ucmd.sidemove = b->sleft ? -SIDERUN : SIDERUN;
  		cmd->ucmd.forwardmove = -FORWARDRUN; //Back IS best.
  
! 		if ((P_AproxDistance(actor->x-b->oldx, actor->y-b->oldy)<50000)
! 			&& b->t_strafe<=0)
  		{
! 			b->t_strafe = 5;
! 			b->sleft = !b->sleft;
  		}
  
  		//If able to see enemy while avoiding missile, still fire at enemy.
! 		if (b->enemy && Check_LOS (actor, b->enemy, SHOOTFOV)) 
! 			Dofire (actor, cmd); //Order bot to fire current weapon
  	}
! 	else if (b->enemy && P_CheckSight (actor, b->enemy, 0)) //Fight!
  	{
! 		Pitch (actor, b->enemy);
  
  		//Check if it's more important to get an item than fight.
! 		if (b->dest && (b->dest->flags&MF_SPECIAL)) //Must be an item, that is close enough.
  		{
! #define is(x) b->dest->IsKindOf (PClass::FindClass (#x))
  			if (
  				(
! 				 (actor->health < b->skill.isp &&
  				  (is (Medikit) ||
  				   is (Stimpack) ||
  				   is (Soulsphere) ||
--- 74,130 ----
  
  //how the bot moves.
  //MAIN movement function.
! void DBot::ThinkForMove (ticcmd_t *cmd)
  {
  	fixed_t dist;
  	bool stuck;
  	int r;
  
  	stuck = false;
! 	dist = dest ? P_AproxDistance(player->mo->x-dest->x, player->mo->y-dest->y) : 0;
  
! 	if (missile &&
! 		((!missile->velx || !missile->vely) || !Check_LOS(missile, SHOOTFOV*3/2)))
  	{
! 		sleft = !sleft;
! 		missile = NULL; //Probably ended its travel.
  	}
  
! 	if (player->mo->pitch > 0)
! 		player->mo->pitch -= 80;
! 	else if (player->mo->pitch <= -60)
! 		player->mo->pitch += 80;
  
  	//HOW TO MOVE:
! 	if (missile && (P_AproxDistance(player->mo->x-missile->x, player->mo->y-missile->y)<AVOID_DIST)) //try avoid missile got from P_Mobj.c thinking part.
  	{
! 		Pitch (missile);
! 		angle = R_PointToAngle2(player->mo->x, player->mo->y, missile->x, missile->y);
! 		cmd->ucmd.sidemove = sleft ? -SIDERUN : SIDERUN;
  		cmd->ucmd.forwardmove = -FORWARDRUN; //Back IS best.
  
! 		if ((P_AproxDistance(player->mo->x-oldx, player->mo->y-oldy)<50000)
! 			&& t_strafe<=0)
  		{
! 			t_strafe = 5;
! 			sleft = !sleft;
  		}
  
  		//If able to see enemy while avoiding missile, still fire at enemy.
! 		if (enemy && Check_LOS (enemy, SHOOTFOV)) 
! 			Dofire (cmd); //Order bot to fire current weapon
  	}
! 	else if (enemy && P_CheckSight (player->mo, enemy, 0)) //Fight!
  	{
! 		Pitch (enemy);
  
  		//Check if it's more important to get an item than fight.
! 		if (dest && (dest->flags&MF_SPECIAL)) //Must be an item, that is close enough.
  		{
! #define is(x) dest->IsKindOf (PClass::FindClass (#x))
  			if (
  				(
! 				 (player->mo->health < skill.isp &&
  				  (is (Medikit) ||
  				   is (Stimpack) ||
  				   is (Soulsphere) ||
***************
*** 140,217 ****
  				  is (Megasphere)
  				 ) || 
  				 dist < (GETINCOMBAT/4) ||
! 				 (b->ReadyWeapon == NULL || b->ReadyWeapon->WeaponFlags & WIF_WIMPY_WEAPON)
  				)
! 				&& (dist < GETINCOMBAT || (b->ReadyWeapon == NULL || b->ReadyWeapon->WeaponFlags & WIF_WIMPY_WEAPON))
! 				&& Reachable (actor, b->dest))
  #undef is
  			{
  				goto roam; //Pick it up, no matter the situation. All bonuses are nice close up.
  			}
  		}
  
! 		b->dest = NULL; //To let bot turn right
  
! 		if (b->ReadyWeapon != NULL && !(b->ReadyWeapon->WeaponFlags & WIF_WIMPY_WEAPON))
! 			actor->flags &= ~MF_DROPOFF; //Don't jump off any ledges when fighting.
  
! 		if (!(b->enemy->flags3 & MF3_ISMONSTER))
! 			b->t_fight = AFTERTICS;
  
! 		if (b->t_strafe <= 0 &&
! 			(P_AproxDistance(actor->x-b->oldx, actor->y-b->oldy)<50000
  			|| ((pr_botmove()%30)==10))
  			)
  		{
  			stuck = true;
! 			b->t_strafe = 5;
! 			b->sleft = !b->sleft;
  		}
  
! 		b->angle = R_PointToAngle2(actor->x, actor->y, b->enemy->x, b->enemy->y);
  
! 		if (b->ReadyWeapon == NULL ||
! 			P_AproxDistance(actor->x-b->enemy->x, actor->y-b->enemy->y) >
! 			b->ReadyWeapon->MoveCombatDist)
  		{
  			// If a monster, use lower speed (just for cooler apperance while strafing down doomed monster)
! 			cmd->ucmd.forwardmove = (b->enemy->flags3 & MF3_ISMONSTER) ? FORWARDWALK : FORWARDRUN;
  		}
  		else if (!stuck) //Too close, so move away.
  		{
  			// If a monster, use lower speed (just for cooler apperance while strafing down doomed monster)
! 			cmd->ucmd.forwardmove = (b->enemy->flags3 & MF3_ISMONSTER) ? -FORWARDWALK : -FORWARDRUN;
  		}
  
  		//Strafing.
! 		if (b->enemy->flags3 & MF3_ISMONSTER) //It's just a monster so take it down cool.
  		{
! 			cmd->ucmd.sidemove = b->sleft ? -SIDEWALK : SIDEWALK;
  		}
  		else
  		{
! 			cmd->ucmd.sidemove = b->sleft ? -SIDERUN : SIDERUN;
  		}
! 		Dofire (actor, cmd); //Order bot to fire current weapon
  	}
! 	else if (b->mate && !b->enemy && (!b->dest || b->dest==b->mate)) //Follow mate move.
  	{
  		fixed_t matedist;
  
! 		Pitch (actor, b->mate);
  
! 		if (!Reachable (actor, b->mate))
  		{
! 			if (b->mate == b->dest && pr_botmove.Random() < 32)
  			{ // [RH] If the mate is the dest, pick a new dest sometimes
! 				b->dest = NULL;
  			}
  			goto roam;
  		}
  
! 		actor->player->angle = R_PointToAngle2(actor->x, actor->y, b->mate->x, b->mate->y);
  
! 		matedist = P_AproxDistance(actor->x - b->mate->x, actor->y - b->mate->y);
  		if (matedist > (FRIEND_DIST*2))
  			cmd->ucmd.forwardmove = FORWARDRUN;
  		else if (matedist > FRIEND_DIST)
--- 137,214 ----
  				  is (Megasphere)
  				 ) || 
  				 dist < (GETINCOMBAT/4) ||
! 				 (player->ReadyWeapon == NULL || player->ReadyWeapon->WeaponFlags & WIF_WIMPY_WEAPON)
  				)
! 				&& (dist < GETINCOMBAT || (player->ReadyWeapon == NULL || player->ReadyWeapon->WeaponFlags & WIF_WIMPY_WEAPON))
! 				&& Reachable (dest))
  #undef is
  			{
  				goto roam; //Pick it up, no matter the situation. All bonuses are nice close up.
  			}
  		}
  
! 		dest = NULL; //To let bot turn right
  
! 		if (player->ReadyWeapon != NULL && !(player->ReadyWeapon->WeaponFlags & WIF_WIMPY_WEAPON))
! 			player->mo->flags &= ~MF_DROPOFF; //Don't jump off any ledges when fighting.
  
! 		if (!(enemy->flags3 & MF3_ISMONSTER))
! 			t_fight = AFTERTICS;
  
! 		if (t_strafe <= 0 &&
! 			(P_AproxDistance(player->mo->x-oldx, player->mo->y-oldy)<50000
  			|| ((pr_botmove()%30)==10))
  			)
  		{
  			stuck = true;
! 			t_strafe = 5;
! 			sleft = !sleft;
  		}
  
! 		angle = R_PointToAngle2(player->mo->x, player->mo->y, enemy->x, enemy->y);
  
! 		if (player->ReadyWeapon == NULL ||
! 			P_AproxDistance(player->mo->x-enemy->x, player->mo->y-enemy->y) >
! 			player->ReadyWeapon->MoveCombatDist)
  		{
  			// If a monster, use lower speed (just for cooler apperance while strafing down doomed monster)
! 			cmd->ucmd.forwardmove = (enemy->flags3 & MF3_ISMONSTER) ? FORWARDWALK : FORWARDRUN;
  		}
  		else if (!stuck) //Too close, so move away.
  		{
  			// If a monster, use lower speed (just for cooler apperance while strafing down doomed monster)
! 			cmd->ucmd.forwardmove = (enemy->flags3 & MF3_ISMONSTER) ? -FORWARDWALK : -FORWARDRUN;
  		}
  
  		//Strafing.
! 		if (enemy->flags3 & MF3_ISMONSTER) //It's just a monster so take it down cool.
  		{
! 			cmd->ucmd.sidemove = sleft ? -SIDEWALK : SIDEWALK;
  		}
  		else
  		{
! 			cmd->ucmd.sidemove = sleft ? -SIDERUN : SIDERUN;
  		}
! 		Dofire (cmd); //Order bot to fire current weapon
  	}
! 	else if (mate && !enemy && (!dest || dest==mate)) //Follow mate move.
  	{
  		fixed_t matedist;
  
! 		Pitch (mate);
  
! 		if (!Reachable (mate))
  		{
! 			if (mate == dest && pr_botmove.Random() < 32)
  			{ // [RH] If the mate is the dest, pick a new dest sometimes
! 				dest = NULL;
  			}
  			goto roam;
  		}
  
! 		angle = R_PointToAngle2(player->mo->x, player->mo->y, mate->x, mate->y);
  
! 		matedist = P_AproxDistance(player->mo->x - mate->x, player->mo->y - mate->y);
  		if (matedist > (FRIEND_DIST*2))
  			cmd->ucmd.forwardmove = FORWARDRUN;
  		else if (matedist > FRIEND_DIST)
***************
*** 221,262 ****
  	}
  	else //Roam after something.
  	{
! 		b->first_shot = true;
  
  	/////
  	roam:
  	/////
! 		if (b->enemy && Check_LOS (actor, b->enemy, SHOOTFOV*3/2)) //If able to see enemy while avoiding missile , still fire at it.
! 			Dofire (actor, cmd); //Order bot to fire current weapon
  
! 		if (b->dest && !(b->dest->flags&MF_SPECIAL) && b->dest->health < 0)
  		{ //Roaming after something dead.
! 			b->dest = NULL;
  		}
  
! 		if (b->dest == NULL)
  		{
! 			if (b->t_fight && b->enemy) //Enemy/bot has jumped around corner. So what to do?
  			{
! 				if (b->enemy->player)
  				{
! 					if (((b->enemy->player->ReadyWeapon != NULL && b->enemy->player->ReadyWeapon->WeaponFlags & WIF_BOT_EXPLOSIVE) ||
! 						(pr_botmove()%100)>b->skill.isp) && b->ReadyWeapon != NULL && !(b->ReadyWeapon->WeaponFlags & WIF_WIMPY_WEAPON))
! 						b->dest = b->enemy;//Dont let enemy kill the bot by supressive fire. So charge enemy.
! 					else //hide while b->t_fight, but keep view at enemy.
! 						b->angle = R_PointToAngle2(actor->x, actor->y, b->enemy->x, b->enemy->y);
  				} //Just a monster, so kill it.
  				else
! 					b->dest = b->enemy;
  
! 				//VerifFavoritWeapon(actor->player); //Dont know why here.., but it must be here, i know the reason, but not why at this spot, uh.
  			}
  			else //Choose a distant target. to get things going.
  			{
  				r = pr_botmove();
  				if (r < 128)
  				{
! 					TThinkerIterator<AInventory> it (STAT_INVENTORY, firstthing);
  					AInventory *item = it.Next();
  
  					if (item != NULL || (item = it.Next()) != NULL)
--- 218,259 ----
  	}
  	else //Roam after something.
  	{
! 		first_shot = true;
  
  	/////
  	roam:
  	/////
! 		if (enemy && Check_LOS (enemy, SHOOTFOV*3/2)) //If able to see enemy while avoiding missile , still fire at it.
! 			Dofire (cmd); //Order bot to fire current weapon
  
! 		if (dest && !(dest->flags&MF_SPECIAL) && dest->health < 0)
  		{ //Roaming after something dead.
! 			dest = NULL;
  		}
  
! 		if (dest == NULL)
  		{
! 			if (t_fight && enemy) //Enemy/bot has jumped around corner. So what to do?
  			{
! 				if (enemy->player)
  				{
! 					if (((enemy->player->ReadyWeapon != NULL && enemy->player->ReadyWeapon->WeaponFlags & WIF_BOT_EXPLOSIVE) ||
! 						(pr_botmove()%100)>skill.isp) && player->ReadyWeapon != NULL && !(player->ReadyWeapon->WeaponFlags & WIF_WIMPY_WEAPON))
! 						dest = enemy;//Dont let enemy kill the bot by supressive fire. So charge enemy.
! 					else //hide while t_fight, but keep view at enemy.
! 						angle = R_PointToAngle2(player->mo->x, player->mo->y, enemy->x, enemy->y);
  				} //Just a monster, so kill it.
  				else
! 					dest = enemy;
  
! 				//VerifFavoritWeapon(player); //Dont know why here.., but it must be here, i know the reason, but not why at this spot, uh.
  			}
  			else //Choose a distant target. to get things going.
  			{
  				r = pr_botmove();
  				if (r < 128)
  				{
! 					TThinkerIterator<AInventory> it (STAT_INVENTORY, bglobal.firstthing);
  					AInventory *item = it.Next();
  
  					if (item != NULL || (item = it.Next()) != NULL)
***************
*** 271,330 ****
  						{
  							item = it.Next();
  						}
! 						firstthing = item;
! 						b->dest = item;
  					}
  				}
! 				else if (b->mate && (r < 179 || P_CheckSight(actor, b->mate)))
  				{
! 					b->dest = b->mate;
  				}
  				else if ((playeringame[(r&(MAXPLAYERS-1))]) && players[(r&(MAXPLAYERS-1))].mo->health > 0)
  				{
! 					b->dest = players[(r&(MAXPLAYERS-1))].mo; 
  				}
  			}
  
! 			if (b->dest)
  			{
! 				b->t_roam = MAXROAM;
  			}
  		}
! 		if (b->dest)
  		{ //Bot has a target so roam after it.
! 			Roam (actor, cmd);
  		}
  
  	} //End of movement main part.
  
! 	if (!b->t_roam && b->dest)
  	{
! 		b->prev = b->dest;
! 		b->dest = NULL;
  	}
  
! 	if (b->t_fight<(AFTERTICS/2))
! 		actor->flags |= MF_DROPOFF;
  
! 	b->oldx = actor->x;
! 	b->oldy = actor->y;
  }
  
  //BOT_WhatToGet
  //
  //Determines if the bot will roam after an item or not.
! void FCajunMaster::WhatToGet (AActor *actor, AActor *item)
  {
- 	player_t *b = actor->player;
- 
- 	if (b == NULL)
- 	{
- 		return;
- 	}
- 
  #define typeis(x) item->IsKindOf (PClass::FindClass (#x))
  	if ((item->renderflags & RF_INVISIBLE) //Under respawn and away.
! 		|| item == b->prev)
  	{
  		return;
  	}
--- 268,320 ----
  						{
  							item = it.Next();
  						}
! 						bglobal.firstthing = item;
! 						dest = item;
  					}
  				}
! 				else if (mate && (r < 179 || P_CheckSight(player->mo, mate)))
  				{
! 					dest = mate;
  				}
  				else if ((playeringame[(r&(MAXPLAYERS-1))]) && players[(r&(MAXPLAYERS-1))].mo->health > 0)
  				{
! 					dest = players[(r&(MAXPLAYERS-1))].mo; 
  				}
  			}
  
! 			if (dest)
  			{
! 				t_roam = MAXROAM;
  			}
  		}
! 		if (dest)
  		{ //Bot has a target so roam after it.
! 			Roam (cmd);
  		}
  
  	} //End of movement main part.
  
! 	if (!t_roam && dest)
  	{
! 		prev = dest;
! 		dest = NULL;
  	}
  
! 	if (t_fight<(AFTERTICS/2))
! 		player->mo->flags |= MF_DROPOFF;
  
! 	oldx = player->mo->x;
! 	oldy = player->mo->y;
  }
  
  //BOT_WhatToGet
  //
  //Determines if the bot will roam after an item or not.
! void DBot::WhatToGet (AActor *item)
  {
  #define typeis(x) item->IsKindOf (PClass::FindClass (#x))
  	if ((item->renderflags & RF_INVISIBLE) //Under respawn and away.
! 		|| item == prev)
  	{
  		return;
  	}
***************
*** 338,344 ****
  		// FIXME
  		AWeapon *heldWeapon;
  
! 		heldWeapon = static_cast<AWeapon *> (b->mo->FindInventory (item->GetClass()));
  		if (heldWeapon != NULL)
  		{
  			if (!weapgiveammo)
--- 328,334 ----
  		// FIXME
  		AWeapon *heldWeapon;
  
! 		heldWeapon = static_cast<AWeapon *> (player->mo->FindInventory (item->GetClass()));
  		if (heldWeapon != NULL)
  		{
  			if (!weapgiveammo)
***************
*** 354,392 ****
  	{
  		AAmmo *ammo = static_cast<AAmmo *> (item);
  		const PClass *parent = ammo->GetParentAmmo ();
! 		AInventory *holdingammo = b->mo->FindInventory (parent);
  
  		if (holdingammo != NULL && holdingammo->Amount >= holdingammo->MaxAmount)
  		{
  			return;
  		}
  	}
! 	else if ((typeis (Megasphere) || typeis (Soulsphere) || typeis (HealthBonus)) && actor->health >= deh.MaxSoulsphere)
  		return;
! 	else if (item->IsKindOf (RUNTIME_CLASS(AHealth)) && actor->health >= deh.MaxHealth /*MAXHEALTH*/)
  		return;
  
! 	if ((b->dest == NULL ||
! 		!(b->dest->flags & MF_SPECIAL)/* ||
! 		!Reachable (actor, b->dest)*/)/* &&
! 		Reachable (actor, item)*/)	// Calling Reachable slows this down tremendously
! 	{
! 		b->prev = b->dest;
! 		b->dest = item;
! 		b->t_roam = MAXROAM;
  	}
  }
  
! void FCajunMaster::Set_enemy (AActor *actor)
  {
  	AActor *oldenemy;
- 	AActor **enemy = &actor->player->enemy;
  
! 	if (*enemy
! 		&& (*enemy)->health > 0
! 		&& P_CheckSight (actor, *enemy))
  	{
! 		oldenemy = *enemy;
  	}
  	else
  	{
--- 344,381 ----
  	{
  		AAmmo *ammo = static_cast<AAmmo *> (item);
  		const PClass *parent = ammo->GetParentAmmo ();
! 		AInventory *holdingammo = player->mo->FindInventory (parent);
  
  		if (holdingammo != NULL && holdingammo->Amount >= holdingammo->MaxAmount)
  		{
  			return;
  		}
  	}
! 	else if ((typeis (Megasphere) || typeis (Soulsphere) || typeis (HealthBonus)) && player->mo->health >= deh.MaxSoulsphere)
  		return;
! 	else if (item->IsKindOf (RUNTIME_CLASS(AHealth)) && player->mo->health >= deh.MaxHealth /*MAXHEALTH*/)
  		return;
  
! 	if ((dest == NULL ||
! 		!(dest->flags & MF_SPECIAL)/* ||
! 		!Reachable (dest)*/)/* &&
! 		Reachable (item)*/)	// Calling Reachable slows this down tremendously
! 	{
! 		prev = dest;
! 		dest = item;
! 		t_roam = MAXROAM;
  	}
  }
  
! void DBot::Set_enemy ()
  {
  	AActor *oldenemy;
  
! 	if (enemy
! 		&& enemy->health > 0
! 		&& P_CheckSight (player->mo, enemy))
  	{
! 		oldenemy = enemy;
  	}
  	else
  	{
***************
*** 395,409 ****
  
  	// [RH] Don't even bother looking for a different enemy if this is not deathmatch
  	// and we already have an existing enemy.
! 	if (deathmatch || !*enemy)
  	{
! 		actor->player->allround = !!*enemy;
! 		*enemy = NULL;
! 		*enemy = Find_enemy(actor);
! 		if (!*enemy)
! 			*enemy = oldenemy; //Try go for last (it will be NULL if there wasn't anyone)
  	}
  	//Verify that that enemy is really something alive that bot can kill.
! 	if (*enemy && (((*enemy)->health < 0 || !((*enemy)->flags&MF_SHOOTABLE)) || actor->IsFriend(*enemy)))
! 		*enemy = NULL;
  }
--- 384,397 ----
  
  	// [RH] Don't even bother looking for a different enemy if this is not deathmatch
  	// and we already have an existing enemy.
! 	if (deathmatch || !enemy)
  	{
! 		allround = !!enemy;
! 		enemy = Find_enemy();
! 		if (!enemy)
! 			enemy = oldenemy; //Try go for last (it will be NULL if there wasn't anyone)
  	}
  	//Verify that that enemy is really something alive that bot can kill.
! 	if (enemy && ((enemy->health < 0 || !(enemy->flags&MF_SHOOTABLE)) || player->mo->IsFriend(enemy)))
! 		enemy = NULL;
  }
