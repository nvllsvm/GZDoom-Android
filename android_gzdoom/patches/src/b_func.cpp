*** /home/draje/Code/gzdoom/src/b_func.cpp	2017-06-18 07:52:50.683013539 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/b_func.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 24,47 ****
  static FRandom pr_botdofire ("BotDoFire");
  
  
! //Checks TRUE reachability from
! //one looker to another. First mobj (looker) is looker.
! bool FCajunMaster::Reachable (AActor *looker, AActor *rtarget)
  {
! 	if (looker == rtarget)
  		return false;
  
  	if ((rtarget->Sector->ceilingplane.ZatPoint (rtarget->x, rtarget->y) -
  		 rtarget->Sector->floorplane.ZatPoint (rtarget->x, rtarget->y))
! 		< looker->height) //Where rtarget is, looker can't be.
  		return false;
  
! 	sector_t *last_s = looker->Sector;
! 	fixed_t last_z = last_s->floorplane.ZatPoint (looker->x, looker->y);
! 	fixed_t estimated_dist = P_AproxDistance (looker->x - rtarget->x, looker->y - rtarget->y);
  	bool reachable = true;
  
! 	FPathTraverse it(looker->x+looker->velx, looker->y+looker->vely, rtarget->x, rtarget->y, PT_ADDLINES|PT_ADDTHINGS);
  	intercept_t *in;
  	while ((in = it.Next()))
  	{
--- 24,46 ----
  static FRandom pr_botdofire ("BotDoFire");
  
  
! //Checks TRUE reachability from bot to a looker.
! bool DBot::Reachable (AActor *rtarget)
  {
! 	if (player->mo == rtarget)
  		return false;
  
  	if ((rtarget->Sector->ceilingplane.ZatPoint (rtarget->x, rtarget->y) -
  		 rtarget->Sector->floorplane.ZatPoint (rtarget->x, rtarget->y))
! 		< player->mo->height) //Where rtarget is, player->mo can't be.
  		return false;
  
! 	sector_t *last_s = player->mo->Sector;
! 	fixed_t last_z = last_s->floorplane.ZatPoint (player->mo->x, player->mo->y);
! 	fixed_t estimated_dist = P_AproxDistance (player->mo->x - rtarget->x, player->mo->y - rtarget->y);
  	bool reachable = true;
  
! 	FPathTraverse it(player->mo->x+player->mo->velx, player->mo->y+player->mo->vely, rtarget->x, rtarget->y, PT_ADDLINES|PT_ADDTHINGS);
  	intercept_t *in;
  	while ((in = it.Next()))
  	{
***************
*** 55,62 ****
  		frac = in->frac - FixedDiv (4*FRACUNIT, MAX_TRAVERSE_DIST);
  		dist = FixedMul (frac, MAX_TRAVERSE_DIST);
  
! 		hitx = it.Trace().x + FixedMul (looker->velx, frac);
! 		hity = it.Trace().y + FixedMul (looker->vely, frac);
  
  		if (in->isaline)
  		{
--- 54,61 ----
  		frac = in->frac - FixedDiv (4*FRACUNIT, MAX_TRAVERSE_DIST);
  		dist = FixedMul (frac, MAX_TRAVERSE_DIST);
  
! 		hitx = it.Trace().x + FixedMul (player->mo->velx, frac);
! 		hity = it.Trace().y + FixedMul (player->mo->vely, frac);
  
  		if (in->isaline)
  		{
***************
*** 76,82 ****
  				if (!bglobal.IsDangerous (s) &&		//Any nukage/lava?
  					(floorheight <= (last_z+MAXMOVEHEIGHT)
  					&& ((ceilingheight == floorheight && line->special)
! 						|| (ceilingheight - floorheight) >= looker->height))) //Does it fit?
  				{
  					last_z = floorheight;
  					last_s = s;
--- 75,81 ----
  				if (!bglobal.IsDangerous (s) &&		//Any nukage/lava?
  					(floorheight <= (last_z+MAXMOVEHEIGHT)
  					&& ((ceilingheight == floorheight && line->special)
! 						|| (ceilingheight - floorheight) >= player->mo->height))) //Does it fit?
  				{
  					last_z = floorheight;
  					last_s = s;
***************
*** 95,101 ****
  		}
  
  		thing = in->d.thing;
! 		if (thing == looker) //Can't reach self in this case.
  			continue;
  		if (thing == rtarget && (rtarget->Sector->floorplane.ZatPoint (rtarget->x, rtarget->y) <= (last_z+MAXMOVEHEIGHT)))
  		{
--- 94,100 ----
  		}
  
  		thing = in->d.thing;
! 		if (thing == player->mo) //Can't reach self in this case.
  			continue;
  		if (thing == rtarget && (rtarget->Sector->floorplane.ZatPoint (rtarget->x, rtarget->y) <= (last_z+MAXMOVEHEIGHT)))
  		{
***************
*** 115,130 ****
  //if these conditions are true, the function returns true.
  //GOOD TO KNOW is that the player's view angle
  //in doom is 90 degrees infront.
! bool FCajunMaster::Check_LOS (AActor *from, AActor *to, angle_t vangle)
  {
! 	if (!P_CheckSight (from, to, SF_SEEPASTBLOCKEVERYTHING))
  		return false; // out of sight
  	if (vangle == ANGLE_MAX)
  		return true;
  	if (vangle == 0)
  		return false; //Looker seems to be blind.
  
! 	return (angle_t)abs (R_PointToAngle2 (from->x, from->y, to->x, to->y) - from->angle) <= vangle/2;
  }
  
  //-------------------------------------
--- 114,129 ----
  //if these conditions are true, the function returns true.
  //GOOD TO KNOW is that the player's view angle
  //in doom is 90 degrees infront.
! bool DBot::Check_LOS (AActor *to, angle_t vangle)
  {
! 	if (!P_CheckSight (player->mo, to, SF_SEEPASTBLOCKEVERYTHING))
  		return false; // out of sight
  	if (vangle == ANGLE_MAX)
  		return true;
  	if (vangle == 0)
  		return false; //Looker seems to be blind.
  
! 	return (angle_t)abs (R_PointToAngle2 (player->mo->x, player->mo->y, to->x, to->y) - player->mo->angle) <= vangle/2;
  }
  
  //-------------------------------------
***************
*** 132,138 ****
  //-------------------------------------
  //The bot will check if it's time to fire
  //and do so if that is the case.
! void FCajunMaster::Dofire (AActor *actor, ticcmd_t *cmd)
  {
  	bool no_fire; //used to prevent bot from pumping rockets into nearby walls.
  	int aiming_penalty=0; //For shooting at shading target, if screen is red, MAKEME: When screen red.
--- 131,137 ----
  //-------------------------------------
  //The bot will check if it's time to fire
  //and do so if that is the case.
! void DBot::Dofire (ticcmd_t *cmd)
  {
  	bool no_fire; //used to prevent bot from pumping rockets into nearby walls.
  	int aiming_penalty=0; //For shooting at shading target, if screen is red, MAKEME: When screen red.
***************
*** 140,189 ****
  	fixed_t dist;
  	angle_t an;
  	int m;
- 	static bool inc[MAXPLAYERS];
- 	AActor *enemy = actor->player->enemy;
  
  	if (!enemy || !(enemy->flags & MF_SHOOTABLE) || enemy->health <= 0)
  		return;
  
! 	if (actor->player->ReadyWeapon == NULL)
  		return;
  
! 	if (actor->player->damagecount > actor->player->skill.isp)
  	{
! 		actor->player->first_shot = true;
  		return;
  	}
  
  	//Reaction skill thing.
! 	if (actor->player->first_shot &&
! 		!(actor->player->ReadyWeapon->WeaponFlags & WIF_BOT_REACTION_SKILL_THING))
  	{
! 		actor->player->t_react = (100-actor->player->skill.reaction+1)/((pr_botdofire()%3)+3);
  	}
! 	actor->player->first_shot = false;
! 	if (actor->player->t_react)
  		return;
  
  	//MAKEME: Decrease the rocket suicides even more.
  
  	no_fire = true;
! 	//actor->player->angle = R_PointToAngle2(actor->x, actor->y, actor->player->enemy->x, actor->player->enemy->y);
  	//Distance to enemy.
! 	dist = P_AproxDistance ((actor->x + actor->velx) - (enemy->x + enemy->velx),
! 		(actor->y + actor->vely) - (enemy->y + enemy->vely));
  
  	//FIRE EACH TYPE OF WEAPON DIFFERENT: Here should all the different weapons go.
! 	if (actor->player->ReadyWeapon->WeaponFlags & WIF_MELEEWEAPON)
  	{
! 		if ((actor->player->ReadyWeapon->ProjectileType != NULL))
  		{
! 			if (actor->player->ReadyWeapon->CheckAmmo (AWeapon::PrimaryFire, false, true))
  			{
  				// This weapon can fire a projectile and has enough ammo to do so
  				goto shootmissile;
  			}
! 			else if (!(actor->player->ReadyWeapon->WeaponFlags & WIF_AMMO_OPTIONAL))
  			{
  				// Ammo is required, so don't shoot. This is for weapons that shoot
  				// missiles that die at close range, such as the powered-up Phoneix Rod.
--- 139,186 ----
  	fixed_t dist;
  	angle_t an;
  	int m;
  
  	if (!enemy || !(enemy->flags & MF_SHOOTABLE) || enemy->health <= 0)
  		return;
  
! 	if (player->ReadyWeapon == NULL)
  		return;
  
! 	if (player->damagecount > skill.isp)
  	{
! 		first_shot = true;
  		return;
  	}
  
  	//Reaction skill thing.
! 	if (first_shot &&
! 		!(player->ReadyWeapon->WeaponFlags & WIF_BOT_REACTION_SKILL_THING))
  	{
! 		t_react = (100-skill.reaction+1)/((pr_botdofire()%3)+3);
  	}
! 	first_shot = false;
! 	if (t_react)
  		return;
  
  	//MAKEME: Decrease the rocket suicides even more.
  
  	no_fire = true;
! 	//angle = R_PointToAngle2(player->mo->x, player->mo->y, player->enemy->x, player->enemy->y);
  	//Distance to enemy.
! 	dist = P_AproxDistance ((player->mo->x + player->mo->velx) - (enemy->x + enemy->velx),
! 		(player->mo->y + player->mo->vely) - (enemy->y + enemy->vely));
  
  	//FIRE EACH TYPE OF WEAPON DIFFERENT: Here should all the different weapons go.
! 	if (player->ReadyWeapon->WeaponFlags & WIF_MELEEWEAPON)
  	{
! 		if ((player->ReadyWeapon->ProjectileType != NULL))
  		{
! 			if (player->ReadyWeapon->CheckAmmo (AWeapon::PrimaryFire, false, true))
  			{
  				// This weapon can fire a projectile and has enough ammo to do so
  				goto shootmissile;
  			}
! 			else if (!(player->ReadyWeapon->WeaponFlags & WIF_AMMO_OPTIONAL))
  			{
  				// Ammo is required, so don't shoot. This is for weapons that shoot
  				// missiles that die at close range, such as the powered-up Phoneix Rod.
***************
*** 196,246 ****
  			no_fire = (dist > (MELEERANGE*4));
  		}
  	}
! 	else if (actor->player->ReadyWeapon->WeaponFlags & WIF_BOT_BFG)
  	{
  		//MAKEME: This should be smarter.
! 		if ((pr_botdofire()%200)<=actor->player->skill.reaction)
! 			if(Check_LOS(actor, actor->player->enemy, SHOOTFOV))
  				no_fire = false;
  	}
! 	else if (actor->player->ReadyWeapon->ProjectileType != NULL)
  	{
! 		if (actor->player->ReadyWeapon->WeaponFlags & WIF_BOT_EXPLOSIVE)
  		{
  			//Special rules for RL
! 			an = FireRox (actor, enemy, cmd);
  			if(an)
  			{
! 				actor->player->angle = an;
  				//have to be somewhat precise. to avoid suicide.
! 				if (abs (actor->player->angle - actor->angle) < 12*ANGLE_1)
  				{
! 					actor->player->t_rocket = 9;
  					no_fire = false;
  				}
  			}
  		}
  		// prediction aiming
  shootmissile:
! 		dist = P_AproxDistance (actor->x - enemy->x, actor->y - enemy->y);
! 		m = dist / GetDefaultByType (actor->player->ReadyWeapon->ProjectileType)->Speed;
! 		SetBodyAt (enemy->x + enemy->velx*m*2, enemy->y + enemy->vely*m*2, enemy->z, 1);
! 		actor->player->angle = R_PointToAngle2 (actor->x, actor->y, body1->x, body1->y);
! 		if (Check_LOS (actor, enemy, SHOOTFOV))
  			no_fire = false;
  	}
  	else
  	{
  		//Other weapons, mostly instant hit stuff.
! 		actor->player->angle = R_PointToAngle2 (actor->x, actor->y, enemy->x, enemy->y);
  		aiming_penalty = 0;
  		if (enemy->flags & MF_SHADOW)
  			aiming_penalty += (pr_botdofire()%25)+10;
! 		if (enemy->Sector->lightlevel<WHATS_DARK/* && !(actor->player->powers & PW_INFRARED)*/)
  			aiming_penalty += pr_botdofire()%40;//Dark
! 		if (actor->player->damagecount)
! 			aiming_penalty += actor->player->damagecount; //Blood in face makes it hard to aim
! 		aiming_value = actor->player->skill.aiming - aiming_penalty;
  		if (aiming_value <= 0)
  			aiming_value = 1;
  		m = ((SHOOTFOV/2)-(aiming_value*SHOOTFOV/200)); //Higher skill is more accurate
--- 193,243 ----
  			no_fire = (dist > (MELEERANGE*4));
  		}
  	}
! 	else if (player->ReadyWeapon->WeaponFlags & WIF_BOT_BFG)
  	{
  		//MAKEME: This should be smarter.
! 		if ((pr_botdofire()%200)<=skill.reaction)
! 			if(Check_LOS(enemy, SHOOTFOV))
  				no_fire = false;
  	}
! 	else if (player->ReadyWeapon->ProjectileType != NULL)
  	{
! 		if (player->ReadyWeapon->WeaponFlags & WIF_BOT_EXPLOSIVE)
  		{
  			//Special rules for RL
! 			an = FireRox (enemy, cmd);
  			if(an)
  			{
! 				angle = an;
  				//have to be somewhat precise. to avoid suicide.
! 				if (abs (angle - player->mo->angle) < 12*ANGLE_1)
  				{
! 					t_rocket = 9;
  					no_fire = false;
  				}
  			}
  		}
  		// prediction aiming
  shootmissile:
! 		dist = P_AproxDistance (player->mo->x - enemy->x, player->mo->y - enemy->y);
! 		m = dist / GetDefaultByType (player->ReadyWeapon->ProjectileType)->Speed;
! 		bglobal.SetBodyAt (enemy->x + enemy->velx*m*2, enemy->y + enemy->vely*m*2, enemy->z, 1);
! 		angle = R_PointToAngle2 (player->mo->x, player->mo->y, bglobal.body1->x, bglobal.body1->y);
! 		if (Check_LOS (enemy, SHOOTFOV))
  			no_fire = false;
  	}
  	else
  	{
  		//Other weapons, mostly instant hit stuff.
! 		angle = R_PointToAngle2 (player->mo->x, player->mo->y, enemy->x, enemy->y);
  		aiming_penalty = 0;
  		if (enemy->flags & MF_SHADOW)
  			aiming_penalty += (pr_botdofire()%25)+10;
! 		if (enemy->Sector->lightlevel<WHATS_DARK/* && !(player->powers & PW_INFRARED)*/)
  			aiming_penalty += pr_botdofire()%40;//Dark
! 		if (player->damagecount)
! 			aiming_penalty += player->damagecount; //Blood in face makes it hard to aim
! 		aiming_value = skill.aiming - aiming_penalty;
  		if (aiming_value <= 0)
  			aiming_value = 1;
  		m = ((SHOOTFOV/2)-(aiming_value*SHOOTFOV/200)); //Higher skill is more accurate
***************
*** 249,266 ****
  
  		if (m)
  		{
! 			if (inc[actor->id])
! 				actor->player->angle += m;
  			else
! 				actor->player->angle -= m;
  		}
  
! 		if (abs (actor->player->angle - actor->angle) < 4*ANGLE_1)
  		{
! 			inc[actor->id] = !inc[actor->id];
  		}
  
! 		if (Check_LOS (actor, enemy, (SHOOTFOV/2)))
  			no_fire = false;
  	}
  	if (!no_fire) //If going to fire weapon
--- 246,263 ----
  
  		if (m)
  		{
! 			if (increase)
! 				angle += m;
  			else
! 				angle -= m;
  		}
  
! 		if (abs (angle - player->mo->angle) < 4*ANGLE_1)
  		{
! 			increase = !increase;
  		}
  
! 		if (Check_LOS (enemy, (SHOOTFOV/2)))
  			no_fire = false;
  	}
  	if (!no_fire) //If going to fire weapon
***************
*** 268,320 ****
  		cmd->ucmd.buttons |= BT_ATTACK;
  	}
  	//Prevents bot from jerking, when firing automatic things with low skill.
! 	//actor->angle = R_PointToAngle2(actor->x, actor->y, actor->player->enemy->x, actor->player->enemy->y);
  }
  
  
  //This function is called every
  //tick (for each bot) to set
  //the mate (teammate coop mate).
! AActor *FCajunMaster::Choose_Mate (AActor *bot)
  {
  	int count;
- 	int count2;
  	fixed_t closest_dist, test;
  	AActor *target;
  	AActor *observer;
- 	bool p_leader[MAXPLAYERS];
  
  	//is mate alive?
! 	if (bot->player->mate)
  	{
! 		if (bot->player->mate->health <= 0)
! 			bot->player->mate = NULL;
  		else
! 			bot->player->last_mate = bot->player->mate;
  	}
! 	if (bot->player->mate) //Still is..
! 		return bot->player->mate;
  
  	//Check old_mates status.
! 	if (bot->player->last_mate)
! 		if (bot->player->last_mate->health <= 0)
! 			bot->player->last_mate = NULL;
! 
! 	for (count = 0; count < MAXPLAYERS; count++)
! 	{
! 		if (!playeringame[count])
! 			continue;
! 		p_leader[count] = false;
! 		for (count2 = 0; count2 < MAXPLAYERS; count2++)
! 		{
! 			if (players[count].isbot
! 				&& players[count2].mate == players[count].mo)
! 			{
! 				p_leader[count] = true;
! 				break;
! 			}
! 		}
! 	}
  
  	target = NULL;
  	closest_dist = FIXED_MAX;
--- 265,312 ----
  		cmd->ucmd.buttons |= BT_ATTACK;
  	}
  	//Prevents bot from jerking, when firing automatic things with low skill.
! 	//player->mo->angle = R_PointToAngle2(player->mo->x, player->mo->y, player->enemy->x, player->enemy->y);
  }
  
+ bool FCajunMaster::IsLeader (player_t *player)
+ {
+ 	for (int count = 0; count < MAXPLAYERS; count++)
+ 	{
+ 		if (players[count].Bot != NULL
+ 			&& players[count].Bot->mate == player->mo)
+ 		{
+ 			return true;
+ 		}
+ 	}
+ 
+ 	return false;
+ }
  
  //This function is called every
  //tick (for each bot) to set
  //the mate (teammate coop mate).
! AActor *DBot::Choose_Mate ()
  {
  	int count;
  	fixed_t closest_dist, test;
  	AActor *target;
  	AActor *observer;
  
  	//is mate alive?
! 	if (mate)
  	{
! 		if (mate->health <= 0)
! 			mate = NULL;
  		else
! 			last_mate = mate;
  	}
! 	if (mate) //Still is..
! 		return mate;
  
  	//Check old_mates status.
! 	if (last_mate)
! 		if (last_mate->health <= 0)
! 			last_mate = NULL;
  
  	target = NULL;
  	closest_dist = FIXED_MAX;
***************
*** 330,347 ****
  
  		if (playeringame[count]
  			&& client->mo
! 			&& bot != client->mo
! 			&& (bot->IsTeammate (client->mo) || !deathmatch)
  			&& client->mo->health > 0
  			&& client->mo != observer
! 			&& ((bot->health/2) <= client->mo->health || !deathmatch)
! 			&& !p_leader[count]) //taken?
  		{
! 
! 			if (P_CheckSight (bot, client->mo, SF_IGNOREVISIBILITY))
  			{
! 				test = P_AproxDistance (client->mo->x - bot->x,
! 										client->mo->y - bot->y);
  
  				if (test < closest_dist)
  				{
--- 322,338 ----
  
  		if (playeringame[count]
  			&& client->mo
! 			&& player->mo != client->mo
! 			&& (player->mo->IsTeammate (client->mo) || !deathmatch)
  			&& client->mo->health > 0
  			&& client->mo != observer
! 			&& ((player->mo->health/2) <= client->mo->health || !deathmatch)
! 			&& !bglobal.IsLeader(client)) //taken?
  		{
! 			if (P_CheckSight (player->mo, client->mo, SF_IGNOREVISIBILITY))
  			{
! 				test = P_AproxDistance (client->mo->x - player->mo->x,
! 										client->mo->y - player->mo->y);
  
  				if (test < closest_dist)
  				{
***************
*** 354,368 ****
  
  /*
  	//Make a introducing to mate.
! 	if(target && target!=bot->player->last_mate)
  	{
  		if((P_Random()%(200*bglobal.botnum))<3)
  		{
! 			bot->player->chat = c_teamup;
  			if(target->bot)
! 					strcpy(bot->player->c_target, botsingame[target->bot_id]);
  						else if(target->player)
! 					strcpy(bot->player->c_target, player_names[target->play_id]);
  		}
  	}
  */
--- 345,359 ----
  
  /*
  	//Make a introducing to mate.
! 	if(target && target!=last_mate)
  	{
  		if((P_Random()%(200*bglobal.botnum))<3)
  		{
! 			chat = c_teamup;
  			if(target->bot)
! 					strcpy(c_target, botsingame[target->bot_id]);
  						else if(target->player)
! 					strcpy(c_target, player_names[target->play_id]);
  		}
  	}
  */
***************
*** 372,378 ****
  }
  
  //MAKEME: Make this a smart decision
! AActor *FCajunMaster::Find_enemy (AActor *bot)
  {
  	int count;
  	fixed_t closest_dist, temp; //To target.
--- 363,369 ----
  }
  
  //MAKEME: Make this a smart decision
! AActor *DBot::Find_enemy ()
  {
  	int count;
  	fixed_t closest_dist, temp; //To target.
***************
*** 382,396 ****
  
  	if (!deathmatch)
  	{ // [RH] Take advantage of the Heretic/Hexen code to be a little smarter
! 		return P_RoughMonsterSearch (bot, 20);
  	}
  
  	//Note: It's hard to ambush a bot who is not alone
! 	if (bot->player->allround || bot->player->mate)
  		vangle = ANGLE_MAX;
  	else
  		vangle = ENEMY_SCAN_FOV;
! 	bot->player->allround = false;
  
  	target = NULL;
  	closest_dist = FIXED_MAX;
--- 373,387 ----
  
  	if (!deathmatch)
  	{ // [RH] Take advantage of the Heretic/Hexen code to be a little smarter
! 		return P_RoughMonsterSearch (player->mo, 20);
  	}
  
  	//Note: It's hard to ambush a bot who is not alone
! 	if (allround || mate)
  		vangle = ANGLE_MAX;
  	else
  		vangle = ENEMY_SCAN_FOV;
! 	allround = false;
  
  	target = NULL;
  	closest_dist = FIXED_MAX;
***************
*** 403,423 ****
  	{
  		player_t *client = &players[count];
  		if (playeringame[count]
! 			&& !bot->IsTeammate (client->mo)
  			&& client->mo != observer
  			&& client->mo->health > 0
! 			&& bot != client->mo)
  		{
! 			if (Check_LOS (bot, client->mo, vangle)) //Here's a strange one, when bot is standing still, the P_CheckSight within Check_LOS almost always returns false. tought it should be the same checksight as below but.. (below works) something must be fuckin wierd screded up. 
! 			//if(P_CheckSight( bot, players[count].mo))
  			{
! 				temp = P_AproxDistance (client->mo->x - bot->x,
! 										client->mo->y - bot->y);
  
  				//Too dark?
  				if (temp > DARK_DIST &&
  					client->mo->Sector->lightlevel < WHATS_DARK /*&&
! 					bot->player->Powers & PW_INFRARED*/)
  					continue;
  
  				if (temp < closest_dist)
--- 394,414 ----
  	{
  		player_t *client = &players[count];
  		if (playeringame[count]
! 			&& !player->mo->IsTeammate (client->mo)
  			&& client->mo != observer
  			&& client->mo->health > 0
! 			&& player->mo != client->mo)
  		{
! 			if (Check_LOS (client->mo, vangle)) //Here's a strange one, when bot is standing still, the P_CheckSight within Check_LOS almost always returns false. tought it should be the same checksight as below but.. (below works) something must be fuckin wierd screded up. 
! 			//if(P_CheckSight(player->mo, players[count].mo))
  			{
! 				temp = P_AproxDistance (client->mo->x - player->mo->x,
! 										client->mo->y - player->mo->y);
  
  				//Too dark?
  				if (temp > DARK_DIST &&
  					client->mo->Sector->lightlevel < WHATS_DARK /*&&
! 					player->Powers & PW_INFRARED*/)
  					continue;
  
  				if (temp < closest_dist)
***************
*** 501,516 ****
  	return dist;
  }
  
! angle_t FCajunMaster::FireRox (AActor *bot, AActor *enemy, ticcmd_t *cmd)
  {
  	fixed_t dist;
  	angle_t ang;
  	AActor *actor;
  	int m;
  
! 	SetBodyAt (bot->x + FixedMul(bot->velx, 5*FRACUNIT),
! 			   bot->y + FixedMul(bot->vely, 5*FRACUNIT),
! 			   bot->z + (bot->height / 2), 2);
  
  	actor = bglobal.body2;
  
--- 492,507 ----
  	return dist;
  }
  
! angle_t DBot::FireRox (AActor *enemy, ticcmd_t *cmd)
  {
  	fixed_t dist;
  	angle_t ang;
  	AActor *actor;
  	int m;
  
! 	bglobal.SetBodyAt (player->mo->x + FixedMul(player->mo->velx, 5*FRACUNIT),
! 					   player->mo->y + FixedMul(player->mo->vely, 5*FRACUNIT),
! 					   player->mo->z + (player->mo->height / 2), 2);
  
  	actor = bglobal.body2;
  
***************
*** 520,535 ****
  	//Predict.
  	m = (((dist+1)/FRACUNIT) / GetDefaultByName("Rocket")->Speed);
  
! 	SetBodyAt (enemy->x + FixedMul(enemy->velx, (m+2*FRACUNIT)),
! 			   enemy->y + FixedMul(enemy->vely, (m+2*FRACUNIT)), ONFLOORZ, 1);
  	dist = P_AproxDistance(actor->x-bglobal.body1->x, actor->y-bglobal.body1->y);
  	//try the predicted location
  	if (P_CheckSight (actor, bglobal.body1, SF_IGNOREVISIBILITY)) //See the predicted location, so give a test missile
  	{
  		FCheckPosition tm;
! 		if (SafeCheckPosition (bot, actor->x, actor->y, tm))
  		{
! 			if (FakeFire (actor, bglobal.body1, cmd) >= SAFE_SELF_MISDIST)
  			{
  				ang = R_PointToAngle2 (actor->x, actor->y, bglobal.body1->x, bglobal.body1->y);
  				return ang;
--- 511,526 ----
  	//Predict.
  	m = (((dist+1)/FRACUNIT) / GetDefaultByName("Rocket")->Speed);
  
! 	bglobal.SetBodyAt (enemy->x + FixedMul(enemy->velx, (m+2*FRACUNIT)),
! 					   enemy->y + FixedMul(enemy->vely, (m+2*FRACUNIT)), ONFLOORZ, 1);
  	dist = P_AproxDistance(actor->x-bglobal.body1->x, actor->y-bglobal.body1->y);
  	//try the predicted location
  	if (P_CheckSight (actor, bglobal.body1, SF_IGNOREVISIBILITY)) //See the predicted location, so give a test missile
  	{
  		FCheckPosition tm;
! 		if (bglobal.SafeCheckPosition (player->mo, actor->x, actor->y, tm))
  		{
! 			if (bglobal.FakeFire (actor, bglobal.body1, cmd) >= SAFE_SELF_MISDIST)
  			{
  				ang = R_PointToAngle2 (actor->x, actor->y, bglobal.body1->x, bglobal.body1->y);
  				return ang;
***************
*** 539,547 ****
  	//Try fire straight.
  	if (P_CheckSight (actor, enemy, 0))
  	{
! 		if (FakeFire (bot, enemy, cmd) >= SAFE_SELF_MISDIST)
  		{
! 			ang = R_PointToAngle2(bot->x, bot->y, enemy->x, enemy->y);
  			return ang;
  		}
  	}
--- 530,538 ----
  	//Try fire straight.
  	if (P_CheckSight (actor, enemy, 0))
  	{
! 		if (bglobal.FakeFire (player->mo, enemy, cmd) >= SAFE_SELF_MISDIST)
  		{
! 			ang = R_PointToAngle2(player->mo->x, player->mo->y, enemy->x, enemy->y);
  			return ang;
  		}
  	}
***************
*** 559,561 ****
--- 550,574 ----
  	actor->flags = savedFlags;
  	return res;
  }
+ 
+ void FCajunMaster::StartTravel ()
+ {
+ 	for (int i = 0; i < MAXPLAYERS; ++i)
+ 	{
+ 		if (players[i].Bot != NULL)
+ 		{
+ 			players[i].Bot->ChangeStatNum (STAT_TRAVELLING);
+ 		}
+ 	}
+ }
+ 
+ void FCajunMaster::FinishTravel ()
+ {
+ 	for (int i = 0; i < MAXPLAYERS; ++i)
+ 	{
+ 		if (players[i].Bot != NULL)
+ 		{
+ 			players[i].Bot->ChangeStatNum (STAT_BOT);
+ 		}
+ 	}
+ }
