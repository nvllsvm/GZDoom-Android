*** /home/draje/Code/gzdoom/src/p_mobj.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_mobj.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 84,90 ****
  
  // EXTERNAL DATA DECLARATIONS ----------------------------------------------
  
- extern cycle_t BotSupportCycles;
  extern int BotWTG;
  EXTERN_CVAR (Int,  cl_rockettrails)
  
--- 84,89 ----
***************
*** 252,260 ****
  		<< MinMissileChance
  		<< SpawnFlags
  		<< Inventory
! 		<< InventoryID
! 		<< id
! 		<< FloatBobPhase
  		<< Translation
  		<< SeeSound
  		<< AttackSound
--- 251,263 ----
  		<< MinMissileChance
  		<< SpawnFlags
  		<< Inventory
! 		<< InventoryID;
! 	if (SaveVersion < 4513)
! 	{
! 		SDWORD id;
! 		arc << id;
! 	}
! 	arc << FloatBobPhase
  		<< Translation
  		<< SeeSound
  		<< AttackSound
***************
*** 309,316 ****
  	}
  	arc << lastpush << lastbump
  		<< PainThreshold
! 		<< DamageFactor
! 		<< WeaveIndexXY << WeaveIndexZ
  		<< PoisonDamageReceived << PoisonDurationReceived << PoisonPeriodReceived << Poisoner
  		<< PoisonDamage << PoisonDuration << PoisonPeriod;
  	if (SaveVersion >= 3235)
--- 312,327 ----
  	}
  	arc << lastpush << lastbump
  		<< PainThreshold
! 		<< DamageFactor;
! 	if (SaveVersion >= 4516)
! 	{
! 		arc << DamageMultiply;
! 	}
! 	else
! 	{
! 		DamageMultiply = FRACUNIT;
! 	}
! 	arc << WeaveIndexXY << WeaveIndexZ
  		<< PoisonDamageReceived << PoisonDurationReceived << PoisonPeriodReceived << Poisoner
  		<< PoisonDamage << PoisonDuration << PoisonPeriod;
  	if (SaveVersion >= 3235)
***************
*** 322,327 ****
--- 333,349 ----
  	{
  		arc << FriendPlayer;
  	}
+ 	if (SaveVersion >= 4517)
+ 	{
+ 		arc << TeleFogSourceType
+ 			<< TeleFogDestType;
+ 	}
+ 	if (SaveVersion >= 4518)
+ 	{
+ 		arc << RipperLevel
+ 			<< RipLevelMin
+ 			<< RipLevelMax;
+ 	}
  
  	{
  		FString tagstr;
***************
*** 1163,1169 ****
  			P_DamageMobj (this, NULL, NULL, TELEFRAG_DAMAGE, NAME_Massacre);
  		}
  		while (health != prevhealth && health > 0);	//abort if the actor wasn't hurt.
! 		return true;
  	}
  	return false;
  }
--- 1185,1191 ----
  			P_DamageMobj (this, NULL, NULL, TELEFRAG_DAMAGE, NAME_Massacre);
  		}
  		while (health != prevhealth && health > 0);	//abort if the actor wasn't hurt.
! 		return health <= 0;
  	}
  	return false;
  }
***************
*** 1191,1207 ****
  	
  	if (target != NULL && ((target->flags & (MF_SHOOTABLE|MF_CORPSE)) || (target->flags6 & MF6_KILLED)) )
  	{
  		if (target->flags & MF_NOBLOOD) nextstate = mo->FindState(NAME_Crash);
  		if (nextstate == NULL) nextstate = mo->FindState(NAME_Death, NAME_Extreme);
  	}
  	if (nextstate == NULL) nextstate = mo->FindState(NAME_Death);
- 	mo->SetState (nextstate);
  	
- 	if (mo->ObjectFlags & OF_EuthanizeMe)
- 	{
- 		return;
- 	}
- 
  	if (line != NULL && line->special == Line_Horizon && !(mo->flags3 & MF3_SKYEXPLODE))
  	{
  		// [RH] Don't explode missiles on horizon lines.
--- 1213,1226 ----
  	
  	if (target != NULL && ((target->flags & (MF_SHOOTABLE|MF_CORPSE)) || (target->flags6 & MF6_KILLED)) )
  	{
+ 		if (mo->flags7 & MF7_HITTARGET)	mo->target = target;
+ 		if (mo->flags7 & MF7_HITMASTER)	mo->master = target;
+ 		if (mo->flags7 & MF7_HITTRACER)	mo->tracer = target;
  		if (target->flags & MF_NOBLOOD) nextstate = mo->FindState(NAME_Crash);
  		if (nextstate == NULL) nextstate = mo->FindState(NAME_Death, NAME_Extreme);
  	}
  	if (nextstate == NULL) nextstate = mo->FindState(NAME_Death);
  	
  	if (line != NULL && line->special == Line_Horizon && !(mo->flags3 & MF3_SKYEXPLODE))
  	{
  		// [RH] Don't explode missiles on horizon lines.
***************
*** 1276,1283 ****
  		}
  	}
  
! 	if (nextstate != NULL)
  	{
  		// [RH] Change render style of exploding rockets
  		if (mo->flags5 & MF5_DEHEXPLOSION)
  		{
--- 1295,1311 ----
  		}
  	}
  
! 	// play the sound before changing the state, so that AActor::Destroy can call S_RelinkSounds on it and the death state can override it.
! 	if (mo->DeathSound)
  	{
+ 		S_Sound (mo, CHAN_VOICE, mo->DeathSound, 1,
+ 			(mo->flags3 & MF3_FULLVOLDEATH) ? ATTN_NONE : ATTN_NORM);
+ 	}
+ 
+ 	mo->SetState (nextstate);
+ 	if (!(mo->ObjectFlags & OF_EuthanizeMe))
+ 	{
+ 		// The rest only applies if the missile actor still exists.
  		// [RH] Change render style of exploding rockets
  		if (mo->flags5 & MF5_DEHEXPLOSION)
  		{
***************
*** 1310,1320 ****
  
  		mo->flags &= ~MF_MISSILE;
  
- 		if (mo->DeathSound)
- 		{
- 			S_Sound (mo, CHAN_VOICE, mo->DeathSound, 1,
- 				(mo->flags3 & MF3_FULLVOLDEATH) ? ATTN_NONE : ATTN_NORM);
- 		}
  	}
  }
  
--- 1338,1343 ----
***************
*** 1651,1656 ****
--- 1674,1680 ----
  	int steps, step, totalsteps;
  	fixed_t startx, starty;
  	fixed_t oldfloorz = mo->floorz;
+ 	fixed_t oldz = mo->z;
  
  	fixed_t maxmove = (mo->waterlevel < 1) || (mo->flags & MF_MISSILE) || 
  					  (mo->player && mo->player->crouchoffset<-10*FRACUNIT) ? MAXMOVE : MAXMOVE/4;
***************
*** 1940,1959 ****
  				}
  				if (BlockingMobj && (BlockingMobj->flags2 & MF2_REFLECTIVE))
  				{
! 					angle = R_PointToAngle2(BlockingMobj->x, BlockingMobj->y, mo->x, mo->y);
! 
! 					// Change angle for deflection/reflection
! 					if (mo->AdjustReflectionAngle (BlockingMobj, angle))
  					{
! 						goto explode;
! 					}
  
! 					// Reflect the missile along angle
! 					mo->angle = angle;
! 					angle >>= ANGLETOFINESHIFT;
! 					mo->velx = FixedMul (mo->Speed>>1, finecosine[angle]);
! 					mo->vely = FixedMul (mo->Speed>>1, finesine[angle]);
! 					mo->velz = -mo->velz/2;
  					if (mo->flags2 & MF2_SEEKERMISSILE)
  					{
  						mo->tracer = mo->target;
--- 1964,2014 ----
  				}
  				if (BlockingMobj && (BlockingMobj->flags2 & MF2_REFLECTIVE))
  				{
! 					bool seeker = (mo->flags2 & MF2_SEEKERMISSILE) ? true : false;
! 					// Don't change the angle if there's THRUREFLECT on the monster.
! 					if (!(BlockingMobj->flags7 & MF7_THRUREFLECT))
  					{
! 						//int dir;
! 						//angle_t delta;
! 						angle = R_PointToAngle2(BlockingMobj->x, BlockingMobj->y, mo->x, mo->y);
! 						bool dontReflect = (mo->AdjustReflectionAngle(BlockingMobj, angle));
! 						// Change angle for deflection/reflection
  
! 						if (!dontReflect)
! 						{
! 							bool tg = (mo->target != NULL);
! 							bool blockingtg = (BlockingMobj->target != NULL);
! 							if (BlockingMobj->flags7 & MF7_AIMREFLECT && (tg || blockingtg))
! 							{
! 								AActor *origin;
! 								if (tg)
! 									origin = mo->target;
! 								else if (blockingtg)
! 									origin = BlockingMobj->target;
! 
! 								float speed = (float)(mo->Speed);
! 								//dest->x - source->x
! 								FVector3 velocity(origin->x - mo->x, origin->y - mo->y, (origin->z + (origin->height/2)) - mo->z);
! 								velocity.Resize(speed);
! 								mo->velx = (fixed_t)(velocity.X);
! 								mo->vely = (fixed_t)(velocity.Y);
! 								mo->velz = (fixed_t)(velocity.Z);
! 							}
! 							else
! 							{
! 								
! 								mo->angle = angle;
! 								angle >>= ANGLETOFINESHIFT;
! 								mo->velx = FixedMul(mo->Speed >> 1, finecosine[angle]);
! 								mo->vely = FixedMul(mo->Speed >> 1, finesine[angle]);
! 								mo->velz = -mo->velz / 2;
! 							}
! 						}
! 						else
! 						{
! 							goto explode;
! 						}						
! 					}
  					if (mo->flags2 & MF2_SEEKERMISSILE)
  					{
  						mo->tracer = mo->target;
***************
*** 2634,2651 ****
  	mo->PrevZ = z;		// Do not interpolate Z position if we changed it since spawning.
  
  	// spawn a teleport fog at old spot because of removal of the body?
! 	mo = Spawn ("TeleportFog", mobj->x, mobj->y, mobj->z, ALLOW_REPLACE);
! 	if (mo != NULL)
! 	{
! 		mo->z += TELEFOGHEIGHT;
! 	}
  
  	// spawn a teleport fog at the new spot
! 	mo = Spawn ("TeleportFog", x, y, z, ALLOW_REPLACE);
! 	if (mo != NULL)
! 	{
! 		mo->z += TELEFOGHEIGHT;
! 	}
  
  	// remove the old monster
  	mobj->Destroy ();
--- 2689,2698 ----
  	mo->PrevZ = z;		// Do not interpolate Z position if we changed it since spawning.
  
  	// spawn a teleport fog at old spot because of removal of the body?
! 	P_SpawnTeleportFog(mobj, mobj->x, mobj->y, mobj->z + TELEFOGHEIGHT, true);
  
  	// spawn a teleport fog at the new spot
! 	P_SpawnTeleportFog(mobj, x, y, z + TELEFOGHEIGHT, false);
  
  	// remove the old monster
  	mobj->Destroy ();
***************
*** 2884,2892 ****
  bool AActor::AdjustReflectionAngle (AActor *thing, angle_t &angle)
  {
  	if (flags2 & MF2_DONTREFLECT) return true;
  
  	// Change angle for reflection
! 	if (thing->flags4&MF4_SHIELDREFLECT)
  	{
  		// Shield reflection (from the Centaur
  		if (abs (angle - thing->angle)>>24 > 45)
--- 2931,2942 ----
  bool AActor::AdjustReflectionAngle (AActor *thing, angle_t &angle)
  {
  	if (flags2 & MF2_DONTREFLECT) return true;
+ 	if (thing->flags7 & MF7_THRUREFLECT) return false;
  
+ 	if (thing->flags7 & MF7_MIRRORREFLECT)
+ 		angle += ANGLE_180;
  	// Change angle for reflection
! 	else if (thing->flags4&MF4_SHIELDREFLECT)
  	{
  		// Shield reflection (from the Centaur
  		if (abs (angle - thing->angle)>>24 > 45)
***************
*** 2909,2914 ****
--- 2959,2971 ----
  		else 
  			angle -= ANG45;
  	}
+ 	else if (thing->flags7 & MF7_AIMREFLECT)
+ 	{
+ 		if (this->target != NULL)
+ 			A_Face(this, this->target);
+ 		else if (thing->target != NULL)
+ 			A_Face(this, thing->target);
+ 	}
  	else 
  		angle += ANGLE_1 * ((pr_reflect()%16)-8);
  	return false;
***************
*** 3016,3021 ****
--- 3073,3090 ----
  	}
  }
  
+ void AActor::SetRoll(angle_t r, bool interpolate)
+ {
+ 	if (r != roll)
+ 	{
+ 		roll = r;
+ 		if (player != NULL && interpolate)
+ 		{
+ 			player->cheats |= CF_INTERPVIEW;
+ 		}
+ 	}
+ }
+ 
  //
  // P_MobjThinker
  //
***************
*** 3112,3118 ****
  				special2++;
  			}
  			//Added by MC: Freeze mode.
! 			if (bglobal.freeze && !(player && !player->isbot))
  			{
  				return;
  			}
--- 3181,3187 ----
  				special2++;
  			}
  			//Added by MC: Freeze mode.
! 			if (bglobal.freeze && !(player && player->Bot == NULL))
  			{
  				return;
  			}
***************
*** 3226,3265 ****
  			}
  		}
  
! 		if (bglobal.botnum && consoleplayer == Net_Arbitrator && !demoplayback &&
  			((flags & (MF_SPECIAL|MF_MISSILE)) || (flags3 & MF3_ISMONSTER)))
  		{
  			BotSupportCycles.Clock();
  			bglobal.m_Thinking = true;
  			for (i = 0; i < MAXPLAYERS; i++)
  			{
! 				if (!playeringame[i] || !players[i].isbot)
  					continue;
  
  				if (flags3 & MF3_ISMONSTER)
  				{
  					if (health > 0
! 						&& !players[i].enemy
  						&& player ? !IsTeammate (players[i].mo) : true
  						&& P_AproxDistance (players[i].mo->x-x, players[i].mo->y-y) < MAX_MONSTER_TARGET_DIST
  						&& P_CheckSight (players[i].mo, this, SF_SEEPASTBLOCKEVERYTHING))
  					{ //Probably a monster, so go kill it.
! 						players[i].enemy = this;
  					}
  				}
  				else if (flags & MF_SPECIAL)
  				{ //Item pickup time
  					//clock (BotWTG);
! 					bglobal.WhatToGet (players[i].mo, this);
  					//unclock (BotWTG);
  					BotWTG++;
  				}
  				else if (flags & MF_MISSILE)
  				{
! 					if (!players[i].missile && (flags3 & MF3_WARNBOT))
  					{ //warn for incoming missiles.
! 						if (target != players[i].mo && bglobal.Check_LOS (players[i].mo, this, ANGLE_90))
! 							players[i].missile = this;
  					}
  				}
  			}
--- 3295,3334 ----
  			}
  		}
  
! 		if (bglobal.botnum && !demoplayback &&
  			((flags & (MF_SPECIAL|MF_MISSILE)) || (flags3 & MF3_ISMONSTER)))
  		{
  			BotSupportCycles.Clock();
  			bglobal.m_Thinking = true;
  			for (i = 0; i < MAXPLAYERS; i++)
  			{
! 				if (!playeringame[i] || players[i].Bot == NULL)
  					continue;
  
  				if (flags3 & MF3_ISMONSTER)
  				{
  					if (health > 0
! 						&& !players[i].Bot->enemy
  						&& player ? !IsTeammate (players[i].mo) : true
  						&& P_AproxDistance (players[i].mo->x-x, players[i].mo->y-y) < MAX_MONSTER_TARGET_DIST
  						&& P_CheckSight (players[i].mo, this, SF_SEEPASTBLOCKEVERYTHING))
  					{ //Probably a monster, so go kill it.
! 						players[i].Bot->enemy = this;
  					}
  				}
  				else if (flags & MF_SPECIAL)
  				{ //Item pickup time
  					//clock (BotWTG);
! 					players[i].Bot->WhatToGet (this);
  					//unclock (BotWTG);
  					BotWTG++;
  				}
  				else if (flags & MF_MISSILE)
  				{
! 					if (!players[i].Bot->missile && (flags3 & MF3_WARNBOT))
  					{ //warn for incoming missiles.
! 						if (target != players[i].mo && players[i].Bot->Check_LOS (this, ANGLE_90))
! 							players[i].Bot->missile = this;
  					}
  				}
  			}
***************
*** 3866,3871 ****
--- 3935,3941 ----
  	actor->touching_sectorlist = NULL;	// NULL head of sector list // phares 3/13/98
  	if (G_SkillProperty(SKILLP_FastMonsters))
  		actor->Speed = actor->GetClass()->Meta.GetMetaFixed(AMETA_FastSpeed, actor->Speed);
+ 	actor->DamageMultiply = FRACUNIT;
  
  
  	// set subsector and/or block links
***************
*** 4246,4251 ****
--- 4316,4327 ----
  	if ((unsigned)playernum >= (unsigned)MAXPLAYERS || !playeringame[playernum])
  		return NULL;
  
+ 	// Old lerp data needs to go
+ 	if (playernum == consoleplayer)
+ 	{
+ 		P_PredictionLerpReset();
+ 	}
+ 
  	p = &players[playernum];
  
  	if (p->cls == NULL)
***************
*** 4286,4297 ****
--- 4362,4376 ----
  	{
  		spawn_x = p->mo->x;
  		spawn_y = p->mo->y;
+ 		spawn_z = p->mo->z;
+ 
  		spawn_angle = p->mo->angle;
  	}
  	else
  	{
  		spawn_x = mthing->x;
  		spawn_y = mthing->y;
+ 
  		// Allow full angular precision but avoid roundoff errors for multiples of 45 degrees.
  		if (mthing->angle % 45 != 0)
  		{
***************
*** 4305,4318 ****
  		{
  			spawn_angle += 1 << ANGLETOFINESHIFT;
  		}
- 	}
  
! 	if (GetDefaultByType(p->cls)->flags & MF_SPAWNCEILING)
! 		spawn_z = ONCEILINGZ;
! 	else if (GetDefaultByType(p->cls)->flags2 & MF2_SPAWNFLOAT)
! 		spawn_z = FLOATRANDZ;
! 	else
! 		spawn_z = ONFLOORZ;
  
  	mobj = static_cast<APlayerPawn *>
  		(Spawn (p->cls, spawn_x, spawn_y, spawn_z, NO_REPLACE));
--- 4384,4397 ----
  		{
  			spawn_angle += 1 << ANGLETOFINESHIFT;
  		}
  
! 		if (GetDefaultByType(p->cls)->flags & MF_SPAWNCEILING)
! 			spawn_z = ONCEILINGZ;
! 		else if (GetDefaultByType(p->cls)->flags2 & MF2_SPAWNFLOAT)
! 			spawn_z = FLOATRANDZ;
! 		else
! 			spawn_z = ONFLOORZ;
! 	}
  
  	mobj = static_cast<APlayerPawn *>
  		(Spawn (p->cls, spawn_x, spawn_y, spawn_z, NO_REPLACE));
***************
*** 4358,4366 ****
  	mobj->pitch = mobj->roll = 0;
  	mobj->health = p->health;
  
- 	//Added by MC: Identification (number in the players[MAXPLAYERS] array)
-     mobj->id = playernum;
- 
  	// [RH] Set player sprite based on skin
  	if (!(mobj->flags4 & MF4_NOSKIN))
  	{
--- 4437,4442 ----
***************
*** 4432,4438 ****
  	{
  		APowerup *invul = static_cast<APowerup*>(p->mo->GiveInventoryType (RUNTIME_CLASS(APowerInvulnerable)));
  		invul->EffectTics = 3*TICRATE;
! 		invul->BlendColor = 0;				// don't mess with the view
  		p->mo->effects |= FX_RESPAWNINVUL;	// [RH] special effect
  	}
  
--- 4508,4515 ----
  	{
  		APowerup *invul = static_cast<APowerup*>(p->mo->GiveInventoryType (RUNTIME_CLASS(APowerInvulnerable)));
  		invul->EffectTics = 3*TICRATE;
! 		invul->BlendColor = 0;			// don't mess with the view
! 		invul->ItemFlags |= IF_UNDROPPABLE;	// Don't drop this
  		p->mo->effects |= FX_RESPAWNINVUL;	// [RH] special effect
  	}
  
***************
*** 4868,4874 ****
  // P_SpawnPuff
  //
  
! AActor *P_SpawnPuff (AActor *source, const PClass *pufftype, fixed_t x, fixed_t y, fixed_t z, angle_t dir, int updown, int flags)
  {
  	AActor *puff;
  	
--- 4945,4951 ----
  // P_SpawnPuff
  //
  
! AActor *P_SpawnPuff (AActor *source, const PClass *pufftype, fixed_t x, fixed_t y, fixed_t z, angle_t dir, int updown, int flags, AActor *vict)
  {
  	AActor *puff;
  	
***************
*** 4878,4886 ****
--- 4955,4978 ----
  	puff = Spawn (pufftype, x, y, z, ALLOW_REPLACE);
  	if (puff == NULL) return NULL;
  
+ 	if ((puff->flags4 & MF4_RANDOMIZE) && puff->tics > 0)
+ 	{
+ 		puff->tics -= pr_spawnpuff() & 3;
+ 		if (puff->tics < 1)
+ 			puff->tics = 1;
+ 	}
+ 
+ 	//Moved puff creation and target/master/tracer setting to here. 
+ 	if (puff && vict)
+ 	{
+ 		if (puff->flags7 & MF7_HITTARGET)	puff->target = vict;
+ 		if (puff->flags7 & MF7_HITMASTER)	puff->master = vict;
+ 		if (puff->flags7 & MF7_HITTRACER)	puff->tracer = vict;
+ 	}
  	// [BB] If the puff came from a player, set the target of the puff to this player.
  	if ( puff && (puff->flags5 & MF5_PUFFGETSOWNER))
  		puff->target = source;
+ 	
  
  	if (source != NULL) puff->angle = R_PointToAngle2(x, y, source->x, source->y);
  
***************
*** 5011,5020 ****
  				cls = cls->ParentClass;
  			}
  		}
  	}
  
- statedone:
- 	if (!(bloodtype <= 1)) th->renderflags |= RF_INVISIBLE;
  	if (bloodtype >= 1)
  		P_DrawSplash2 (40, x, y, z, dir, 2, bloodcolor);
  }
--- 5103,5113 ----
  				cls = cls->ParentClass;
  			}
  		}
+ 
+ 	statedone:
+ 		if (!(bloodtype <= 1)) th->renderflags |= RF_INVISIBLE;
  	}
  
  	if (bloodtype >= 1)
  		P_DrawSplash2 (40, x, y, z, dir, 2, bloodcolor);
  }
***************
*** 5850,5871 ****
  	return NULL;
  }
  
  bool AActor::IsTeammate (AActor *other)
  {
  	if (!other)
  		return false;
  	else if (!deathmatch && player && other->player)
- 		return true;
- 	int myTeam = DesignatedTeam;
- 	int otherTeam = other->DesignatedTeam;
- 	if (player)
- 		myTeam = player->userinfo.GetTeam();
- 	if (other->player)
- 		otherTeam = other->player->userinfo.GetTeam();
- 	if (teamplay && myTeam != TEAM_NONE && myTeam == otherTeam)
  	{
  		return true;
  	}
  	return false;
  }
  
--- 5943,5983 ----
  	return NULL;
  }
  
+ int AActor::GetTeam()
+ {
+ 	if (player)
+ 	{
+ 		return player->userinfo.GetTeam();
+ 	}
+ 
+ 	int myTeam = DesignatedTeam;
+ 
+ 	// Check for monsters that belong to a player on the team but aren't part of the team themselves.
+ 	if (myTeam == TEAM_NONE && FriendPlayer != 0)
+ 	{
+ 		myTeam = players[FriendPlayer - 1].userinfo.GetTeam();
+ 	}
+ 	return myTeam;
+ 
+ }
+ 
  bool AActor::IsTeammate (AActor *other)
  {
  	if (!other)
+ 	{
  		return false;
+ 	}
  	else if (!deathmatch && player && other->player)
  	{
  		return true;
  	}
+ 	else if (teamplay)
+ 	{
+ 		int myTeam = GetTeam();
+ 		int otherTeam = other->GetTeam();
+ 
+ 		return (myTeam != TEAM_NONE && myTeam == otherTeam);
+ 	}
  	return false;
  }
  
***************
*** 5961,5967 ****
  int AActor::DoSpecialDamage (AActor *target, int damage, FName damagetype)
  {
  	if (target->player && target->player->mo == target && damage < 1000 &&
! 		(target->player->cheats & CF_GODMODE))
  	{
  		return -1;
  	}
--- 6073,6079 ----
  int AActor::DoSpecialDamage (AActor *target, int damage, FName damagetype)
  {
  	if (target->player && target->player->mo == target && damage < 1000 &&
! 		(target->player->cheats & CF_GODMODE || target->player->cheats & CF_GODMODE2))
  	{
  		return -1;
  	}
