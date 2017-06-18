*** /home/draje/Code/gzdoom/src/p_map.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_map.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 76,81 ****
--- 76,144 ----
  
  //==========================================================================
  //
+ // GetCoefficientClosestPointInLine24
+ //
+ // Formula: (dotProduct(ldv1 - tm, ld) << 24) / dotProduct(ld, ld)
+ // with: ldv1 = (ld->v1->x, ld->v1->y), tm = (tm.x, tm.y)
+ // and ld = (ld->dx, ld->dy)
+ // Returns truncated to range [0, 1 << 24].
+ //
+ //==========================================================================
+ 
+ static fixed_t GetCoefficientClosestPointInLine24(line_t *ld, FCheckPosition &tm)
+ {
+ 	// [EP] Use 64 bit integers in order to keep the exact result of the
+ 	// multiplication, because in the case the vertexes have both the
+ 	// distance coordinates equal to the map limit (32767 units, which is
+ 	// 2147418112 in fixed_t notation), the product result would occupy
+ 	// 62 bits and the sum of two products would occupy 63 bits
+ 	// in the worst case. If instead the vertexes are very close (1 in
+ 	// fixed_t notation, which is 1.52587890625e-05 in float notation), the
+ 	// product and the sum can be 1 in the worst case, which is very tiny.
+ 
+ 	SQWORD r_num = ((SQWORD(tm.x - ld->v1->x)*ld->dx) +
+ 					(SQWORD(tm.y - ld->v1->y)*ld->dy));
+ 
+ 	// The denominator is always positive. Use this to avoid useless
+ 	// calculations.
+ 	SQWORD r_den = (SQWORD(ld->dx)*ld->dx + SQWORD(ld->dy)*ld->dy);
+ 
+ 	if (r_num <= 0) {
+ 		// [EP] The numerator is less or equal to zero, hence the closest
+ 		// point on the line is the first vertex. Truncate the result to 0.
+ 		return 0;
+ 	}
+ 
+ 	if (r_num >= r_den) {
+ 		// [EP] The division is greater or equal to 1, hence the closest
+ 		// point on the line is the second vertex. Truncate the result to
+ 		// 1 << 24.
+ 		return (1 << 24);
+ 	}
+ 
+ 	// [EP] Deal with the limited bits. The original formula is:
+ 	// r = (r_num << 24) / r_den,
+ 	// but r_num might be big enough to make the shift overflow.
+ 	// Since the numerator can't be saved in a 128bit integer,
+ 	// the denominator must be right shifted. If the denominator is
+ 	// less than (1 << 24), there would be a division by zero.
+ 	// Thanks to the fact that in this code path the denominator is greater
+ 	// than the numerator, it's possible to avoid this bad situation by
+ 	// just checking the last 24 bits of the numerator.
+ 	if ((r_num >> (63-24)) != 0) {
+ 		// [EP] In fact, if the numerator is greater than
+ 		// (1 << (63-24)), the denominator must be greater than
+ 		// (1 << (63-24)), hence the denominator won't be zero after
+ 		// the right shift by 24 places.
+ 		return (fixed_t)(r_num/(r_den >> 24));
+ 	}
+ 	// [EP] Having the last 24 bits all zero allows left shifting
+ 	// the numerator by 24 bits without overflow.
+ 	return (fixed_t)((r_num << 24)/r_den);
+ }
+ 
+ //==========================================================================
+ //
  // PIT_FindFloorCeiling
  //
  // only3d set means to only check against 3D floors and midtexes.
***************
*** 198,204 ****
  
  		if (ff_top > tmf.floorz)
  		{
! 			if (ff_top <= tmf.z || (!(flags && FFCF_3DRESTRICT) && (tmf.thing != NULL && ff_bottom < tmf.z && ff_top < tmf.z + tmf.thing->MaxStepHeight)))
  			{
  				tmf.dropoffz = tmf.floorz = ff_top;
  				tmf.floorpic = *rover->top.texture;
--- 261,267 ----
  
  		if (ff_top > tmf.floorz)
  		{
! 			if (ff_top <= tmf.z || (!(flags & FFCF_3DRESTRICT) && (tmf.thing != NULL && ff_bottom < tmf.z && ff_top < tmf.z + tmf.thing->MaxStepHeight)))
  			{
  				tmf.dropoffz = tmf.floorz = ff_top;
  				tmf.floorpic = *rover->top.texture;
***************
*** 380,386 ****
  		// ... and some items can never be telefragged while others will be telefragged by everything that teleports upon them.
  		if ((StompAlwaysFrags && !(th->flags6 & MF6_NOTELEFRAG)) || (th->flags7 & MF7_ALWAYSTELEFRAG))
  		{
! 			P_DamageMobj(th, thing, thing, TELEFRAG_DAMAGE, NAME_Telefrag, DMG_THRUSTLESS);
  			continue;
  		}
  		return false;
--- 443,451 ----
  		// ... and some items can never be telefragged while others will be telefragged by everything that teleports upon them.
  		if ((StompAlwaysFrags && !(th->flags6 & MF6_NOTELEFRAG)) || (th->flags7 & MF7_ALWAYSTELEFRAG))
  		{
! 			// Don't actually damage if predicting a teleport
! 			if (thing->player == NULL || !(thing->player->cheats & CF_PREDICTING))
! 				P_DamageMobj(th, thing, thing, TELEFRAG_DAMAGE, NAME_Telefrag, DMG_THRUSTLESS);
  			continue;
  		}
  		return false;
***************
*** 734,744 ****
  	else
  	{ // Find the point on the line closest to the actor's center, and use
  		// that to calculate openings
! 		float dx = (float)ld->dx;
! 		float dy = (float)ld->dy;
! 		fixed_t r = (fixed_t)(((float)(tm.x - ld->v1->x) * dx +
! 			(float)(tm.y - ld->v1->y) * dy) /
! 			(dx*dx + dy*dy) * 16777216.f);
  		/*		Printf ("%d:%d: %d  (%d %d %d %d)  (%d %d %d %d)\n", level.time, ld-lines, r,
  		ld->frontsector->floorplane.a,
  		ld->frontsector->floorplane.b,
--- 799,806 ----
  	else
  	{ // Find the point on the line closest to the actor's center, and use
  		// that to calculate openings
! 		fixed_t r = GetCoefficientClosestPointInLine24(ld, tm);
! 
  		/*		Printf ("%d:%d: %d  (%d %d %d %d)  (%d %d %d %d)\n", level.time, ld-lines, r,
  		ld->frontsector->floorplane.a,
  		ld->frontsector->floorplane.b,
***************
*** 825,830 ****
--- 887,906 ----
  	return true;
  }
  
+ 
+ //==========================================================================
+ //
+ // Isolated to keep the code readable and fix the logic
+ //
+ //==========================================================================
+ 
+ static bool CheckRipLevel(AActor *victim, AActor *projectile)
+ {
+ 	if (victim->RipLevelMin > 0 && projectile->RipperLevel < victim->RipLevelMin) return false;
+ 	if (victim->RipLevelMax > 0 && projectile->RipperLevel > victim->RipLevelMax) return false;
+ 	return true;
+ }
+ 
  //==========================================================================
  //
  // PIT_CheckThing
***************
*** 1109,1132 ****
  					//     cases where they are clearly supposed to do that
  					if (thing->IsFriend(tm.thing->target))
  					{
! 						// Friends never harm each other
! 						return false;
! 					}
! 					if (thing->TIDtoHate != 0 && thing->TIDtoHate == tm.thing->target->TIDtoHate)
! 					{
! 						// [RH] Don't hurt monsters that hate the same thing as you do
! 						return false;
  					}
! 					if (thing->GetSpecies() == tm.thing->target->GetSpecies() && !(thing->flags6 & MF6_DOHARMSPECIES))
  					{
! 						// Don't hurt same species or any relative -
! 						// but only if the target isn't one's hostile.
! 						if (!thing->IsHostile(tm.thing->target))
  						{
! 							// Allow hurting monsters the shooter hates.
! 							if (thing->tid == 0 || tm.thing->target->TIDtoHate != thing->tid)
  							{
! 								return false;
  							}
  						}
  					}
--- 1185,1211 ----
  					//     cases where they are clearly supposed to do that
  					if (thing->IsFriend(tm.thing->target))
  					{
! 						// Friends never harm each other, unless the shooter has the HARMFRIENDS set.
! 						if (!(thing->flags7 & MF7_HARMFRIENDS)) return false;
  					}
! 					else
  					{
! 						if (thing->TIDtoHate != 0 && thing->TIDtoHate == tm.thing->target->TIDtoHate)
  						{
! 							// [RH] Don't hurt monsters that hate the same thing as you do
! 							return false;
! 						}
! 						if (thing->GetSpecies() == tm.thing->target->GetSpecies() && !(thing->flags6 & MF6_DOHARMSPECIES))
! 						{
! 							// Don't hurt same species or any relative -
! 							// but only if the target isn't one's hostile.
! 							if (!thing->IsHostile(tm.thing->target))
  							{
! 								// Allow hurting monsters the shooter hates.
! 								if (thing->tid == 0 || tm.thing->target->TIDtoHate != thing->tid)
! 								{
! 									return false;
! 								}
  							}
  						}
  					}
***************
*** 1142,1148 ****
  		{
  			return true;
  		}
! 		if (tm.DoRipping && !(thing->flags5 & MF5_DONTRIP))
  		{
  			if (!(tm.thing->flags6 & MF6_NOBOSSRIP) || !(thing->flags2 & MF2_BOSS))
  			{
--- 1221,1228 ----
  		{
  			return true;
  		}
! 
! 		if ((tm.DoRipping && !(thing->flags5 & MF5_DONTRIP)) && CheckRipLevel(thing, tm.thing))
  		{
  			if (!(tm.thing->flags6 & MF6_NOBOSSRIP) || !(thing->flags2 & MF2_BOSS))
  			{
***************
*** 1218,1223 ****
--- 1298,1313 ----
  		{
  			P_GiveBody(thing, -damage);
  		}
+ 
+ 		if ((thing->flags7 & MF7_THRUREFLECT) && (thing->flags2 & MF2_REFLECTIVE) && (tm.thing->flags & MF_MISSILE))
+ 		{
+ 			if (tm.thing->flags2 & MF2_SEEKERMISSILE)
+ 			{
+ 				tm.thing->tracer = tm.thing->target;
+ 			}
+ 			tm.thing->target = thing;
+ 			return true;
+ 		}
  		return false;		// don't traverse any more
  	}
  	if (thing->flags2 & MF2_PUSHABLE && !(tm.thing->flags2 & MF2_CANNOTPUSH))
***************
*** 1578,1584 ****
  		{ // Don't clip against self
  			continue;
  		}
! 		if ((actor->flags & MF_MISSILE) && thing == actor->target)
  		{ // Don't clip against whoever shot the missile.
  			continue;
  		}
--- 1668,1674 ----
  		{ // Don't clip against self
  			continue;
  		}
! 		if ((actor->flags & MF_MISSILE) && (thing == actor->target))
  		{ // Don't clip against whoever shot the missile.
  			continue;
  		}
***************
*** 1917,1929 ****
  		}
  
  		//Added by MC: To prevent bot from getting into dangerous sectors.
! 		if (thing->player && thing->player->isbot && thing->flags & MF_SHOOTABLE)
  		{
  			if (tm.sector != thing->Sector
  				&& bglobal.IsDangerous(tm.sector))
  			{
! 				thing->player->prev = thing->player->dest;
! 				thing->player->dest = NULL;
  				thing->velx = 0;
  				thing->vely = 0;
  				thing->z = oldz;
--- 2007,2019 ----
  		}
  
  		//Added by MC: To prevent bot from getting into dangerous sectors.
! 		if (thing->player && thing->player->Bot != NULL && thing->flags & MF_SHOOTABLE)
  		{
  			if (tm.sector != thing->Sector
  				&& bglobal.IsDangerous(tm.sector))
  			{
! 				thing->player->Bot->prev = thing->player->Bot->dest;
! 				thing->player->Bot->dest = NULL;
  				thing->velx = 0;
  				thing->vely = 0;
  				thing->z = oldz;
***************
*** 1981,1993 ****
  		thing->AdjustFloorClip();
  	}
  
- 	// [RH] Don't activate anything if just predicting
- 	if (thing->player && (thing->player->cheats & CF_PREDICTING))
- 	{
- 		thing->flags6 &= ~MF6_INTRYMOVE;
- 		return true;
- 	}
- 
  	// if any special lines were hit, do the effect
  	if (!(thing->flags & (MF_TELEPORT | MF_NOCLIP)))
  	{
--- 2071,2076 ----
***************
*** 1998,2004 ****
  			oldside = P_PointOnLineSide(oldx, oldy, ld);
  			if (side != oldside && ld->special && !(thing->flags6 & MF6_NOTRIGGER))
  			{
! 				if (thing->player)
  				{
  					P_ActivateLine(ld, thing, oldside, SPAC_Cross);
  				}
--- 2081,2091 ----
  			oldside = P_PointOnLineSide(oldx, oldy, ld);
  			if (side != oldside && ld->special && !(thing->flags6 & MF6_NOTRIGGER))
  			{
! 				if (thing->player && (thing->player->cheats & CF_PREDICTING))
! 				{
! 					P_PredictLine(ld, thing, oldside, SPAC_Cross);
! 				}
! 				else if (thing->player)
  				{
  					P_ActivateLine(ld, thing, oldside, SPAC_Cross);
  				}
***************
*** 2024,2029 ****
--- 2111,2123 ----
  		}
  	}
  
+ 	// [RH] Don't activate anything if just predicting
+ 	if (thing->player && (thing->player->cheats & CF_PREDICTING))
+ 	{
+ 		thing->flags6 &= ~MF6_INTRYMOVE;
+ 		return true;
+ 	}
+ 
  	// [RH] Check for crossing fake floor/ceiling
  	newsec = thing->Sector;
  	if (newsec->heightsec && oldsec->heightsec && newsec->SecActTarget)
***************
*** 2151,2157 ****
  			{ // too big a step up
  				return false;
  			}
! 			else if ((thing->flags & MF_MISSILE) && !(thing->flags6 && MF6_STEPMISSILE) && tm.floorz > newz)
  			{ // [RH] Don't let normal missiles climb steps
  				return false;
  			}
--- 2245,2251 ----
  			{ // too big a step up
  				return false;
  			}
! 			else if ((thing->flags & MF_MISSILE) && !(thing->flags6 & MF6_STEPMISSILE) && tm.floorz > newz)
  			{ // [RH] Don't let normal missiles climb steps
  				return false;
  			}
***************
*** 2914,2931 ****
  extern FRandom pr_bounce;
  bool P_BounceActor(AActor *mo, AActor *BlockingMobj, bool ontop)
  {
  	if (mo && BlockingMobj && ((mo->BounceFlags & BOUNCE_AllActors)
! 		|| ((mo->flags & MF_MISSILE) && (!(mo->flags2 & MF2_RIP) || (BlockingMobj->flags5 & MF5_DONTRIP) || ((mo->flags6 & MF6_NOBOSSRIP) && (BlockingMobj->flags2 & MF2_BOSS))) && (BlockingMobj->flags2 & MF2_REFLECTIVE))
! 		|| ((BlockingMobj->player == NULL) && (!(BlockingMobj->flags3 & MF3_ISMONSTER)))
! 		))
  	{
  		if (mo->bouncecount > 0 && --mo->bouncecount == 0) return false;
  
  		if (!ontop)
  		{
  			fixed_t speed;
! 			angle_t angle = R_PointToAngle2(BlockingMobj->x,
! 				BlockingMobj->y, mo->x, mo->y) + ANGLE_1*((pr_bounce() % 16) - 8);
  			speed = P_AproxDistance(mo->velx, mo->vely);
  			speed = FixedMul(speed, mo->wallbouncefactor); // [GZ] was 0.75, using wallbouncefactor seems more consistent
  			mo->angle = angle;
--- 3008,3031 ----
  extern FRandom pr_bounce;
  bool P_BounceActor(AActor *mo, AActor *BlockingMobj, bool ontop)
  {
+ 	//Don't go through all of this if the actor is reflective and wants things to pass through them.
+ 	if (BlockingMobj && ((BlockingMobj->flags2 & MF2_REFLECTIVE) && (BlockingMobj->flags7 & MF7_THRUREFLECT))) return true;
  	if (mo && BlockingMobj && ((mo->BounceFlags & BOUNCE_AllActors)
! 		|| ((mo->flags & MF_MISSILE) && (!(mo->flags2 & MF2_RIP) 
! 		|| (BlockingMobj->flags5 & MF5_DONTRIP) 
! 		|| ((mo->flags6 & MF6_NOBOSSRIP) && (BlockingMobj->flags2 & MF2_BOSS))) && (BlockingMobj->flags2 & MF2_REFLECTIVE))
! 		|| ((BlockingMobj->player == NULL) && (!(BlockingMobj->flags3 & MF3_ISMONSTER)))))
  	{
  		if (mo->bouncecount > 0 && --mo->bouncecount == 0) return false;
  
+ 		if (mo->flags7 & MF7_HITTARGET)	mo->target = BlockingMobj;
+ 		if (mo->flags7 & MF7_HITMASTER)	mo->master = BlockingMobj;
+ 		if (mo->flags7 & MF7_HITTRACER)	mo->tracer = BlockingMobj;
+ 
  		if (!ontop)
  		{
  			fixed_t speed;
! 			angle_t angle = R_PointToAngle2(BlockingMobj->x,BlockingMobj->y, mo->x, mo->y) + ANGLE_1*((pr_bounce() % 16) - 8);
  			speed = P_AproxDistance(mo->velx, mo->vely);
  			speed = FixedMul(speed, mo->wallbouncefactor); // [GZ] was 0.75, using wallbouncefactor seems more consistent
  			mo->angle = angle;
***************
*** 3665,3670 ****
--- 3765,3772 ----
  			hity = t1->y + FixedMul(vy, dist);
  			hitz = shootz + FixedMul(vz, dist);
  
+ 			
+ 
  			// Spawn bullet puffs or blood spots, depending on target type.
  			if ((puffDefaults != NULL && puffDefaults->flags3 & MF3_PUFFONACTORS) ||
  				(trace.Actor->flags & MF_NOBLOOD) ||
***************
*** 3674,3680 ****
  					puffFlags |= PF_HITTHINGBLEED;
  
  				// We must pass the unreplaced puff type here 
! 				puff = P_SpawnPuff(t1, pufftype, hitx, hity, hitz, angle - ANG180, 2, puffFlags | PF_HITTHING);
  			}
  
  			// Allow puffs to inflict poison damage, so that hitscans can poison, too.
--- 3776,3782 ----
  					puffFlags |= PF_HITTHINGBLEED;
  
  				// We must pass the unreplaced puff type here 
! 				puff = P_SpawnPuff(t1, pufftype, hitx, hity, hitz, angle - ANG180, 2, puffFlags | PF_HITTHING, trace.Actor);
  			}
  
  			// Allow puffs to inflict poison damage, so that hitscans can poison, too.
***************
*** 3686,3692 ****
  			// [GZ] If MF6_FORCEPAIN is set, we need to call P_DamageMobj even if damage is 0!
  			// Note: The puff may not yet be spawned here so we must check the class defaults, not the actor.
  			int newdam = damage;
! 			if (damage || (puffDefaults != NULL && puffDefaults->flags6 & MF6_FORCEPAIN))
  			{
  				int dmgflags = DMG_INFLICTOR_IS_PUFF | pflag;
  				// Allow MF5_PIERCEARMOR on a weapon as well.
--- 3788,3794 ----
  			// [GZ] If MF6_FORCEPAIN is set, we need to call P_DamageMobj even if damage is 0!
  			// Note: The puff may not yet be spawned here so we must check the class defaults, not the actor.
  			int newdam = damage;
! 			if (damage || (puffDefaults != NULL && ((puffDefaults->flags6 & MF6_FORCEPAIN) || (puffDefaults->flags7 & MF7_CAUSEPAIN))))
  			{
  				int dmgflags = DMG_INFLICTOR_IS_PUFF | pflag;
  				// Allow MF5_PIERCEARMOR on a weapon as well.
***************
*** 3695,3701 ****
  				{
  					dmgflags |= DMG_NO_ARMOR;
  				}
! 
  				if (puff == NULL)
  				{
  					// Since the puff is the damage inflictor we need it here 
--- 3797,3803 ----
  				{
  					dmgflags |= DMG_NO_ARMOR;
  				}
! 				
  				if (puff == NULL)
  				{
  					// Since the puff is the damage inflictor we need it here 
***************
*** 3785,3790 ****
--- 3887,3938 ----
  
  //==========================================================================
  //
+ // P_LinePickActor
+ //
+ //==========================================================================
+ 
+ AActor *P_LinePickActor(AActor *t1, angle_t angle, fixed_t distance, int pitch,
+ 						DWORD actorMask, DWORD wallMask)
+ {
+ 	fixed_t vx, vy, vz, shootz;
+ 	
+ 	angle >>= ANGLETOFINESHIFT;
+ 	pitch = (angle_t)(pitch) >> ANGLETOFINESHIFT;
+ 
+ 	vx = FixedMul(finecosine[pitch], finecosine[angle]);
+ 	vy = FixedMul(finecosine[pitch], finesine[angle]);
+ 	vz = -finesine[pitch];
+ 
+ 	shootz = t1->z - t1->floorclip + (t1->height >> 1);
+ 	if (t1->player != NULL)
+ 	{
+ 		shootz += FixedMul(t1->player->mo->AttackZOffset, t1->player->crouchfactor);
+ 	}
+ 	else
+ 	{
+ 		shootz += 8 * FRACUNIT;
+ 	}
+ 
+ 	FTraceResults trace;
+ 	Origin TData;
+ 	
+ 	TData.Caller = t1;
+ 	TData.hitGhosts = true;
+ 	
+ 	if (Trace(t1->x, t1->y, shootz, t1->Sector, vx, vy, vz, distance,
+ 		actorMask, wallMask, t1, trace, TRACE_NoSky, CheckForActor, &TData))
+ 	{
+ 		if (trace.HitType == TRACE_HitActor)
+ 		{
+ 			return trace.Actor;
+ 		}
+ 	}
+ 
+ 	return NULL;
+ }
+ 
+ //==========================================================================
+ //
  //
  //
  //==========================================================================
***************
*** 4023,4029 ****
  	int flags;
  
  	assert(puffclass != NULL);		// Because we set it to a default above
! 	AActor *puffDefaults = GetDefaultByType(puffclass->GetReplacement());
  
  	flags = (puffDefaults->flags6 & MF6_NOTRIGGER) ? 0 : TRACE_PCross | TRACE_Impact;
  	rail_data.StopAtInvul = (puffDefaults->flags3 & MF3_FOILINVUL) ? false : true;
--- 4171,4177 ----
  	int flags;
  
  	assert(puffclass != NULL);		// Because we set it to a default above
! 	AActor *puffDefaults = GetDefaultByType(puffclass->GetReplacement()); //Contains all the flags such as FOILINVUL, etc.
  
  	flags = (puffDefaults->flags6 & MF6_NOTRIGGER) ? 0 : TRACE_PCross | TRACE_Impact;
  	rail_data.StopAtInvul = (puffDefaults->flags3 & MF3_FOILINVUL) ? false : true;
***************
*** 4056,4062 ****
  		z = shootz + FixedMul(hitdist, vz);
  
  		if ((hitactor->flags & MF_NOBLOOD) ||
! 			(hitactor->flags2 & (MF2_DORMANT | MF2_INVULNERABLE)))
  		{
  			spawnpuff = (puffclass != NULL);
  		}
--- 4204,4210 ----
  		z = shootz + FixedMul(hitdist, vz);
  
  		if ((hitactor->flags & MF_NOBLOOD) ||
! 			(hitactor->flags2 & MF2_DORMANT || ((hitactor->flags2 & MF2_INVULNERABLE) && !(puffDefaults->flags3 & MF3_FOILINVUL))))
  		{
  			spawnpuff = (puffclass != NULL);
  		}
***************
*** 4071,4083 ****
  		}
  		if (spawnpuff)
  		{
! 			P_SpawnPuff(source, puffclass, x, y, z, (source->angle + angleoffset) - ANG90, 1, puffflags);
  		}
  		if (puffDefaults && puffDefaults->PoisonDamage > 0 && puffDefaults->PoisonDuration != INT_MIN)
  		{
  			P_PoisonMobj(hitactor, thepuff ? thepuff : source, source, puffDefaults->PoisonDamage, puffDefaults->PoisonDuration, puffDefaults->PoisonPeriod, puffDefaults->PoisonDamageType);
  		}
! 		int newdam = P_DamageMobj(hitactor, thepuff ? thepuff : source, source, damage, damagetype, DMG_INFLICTOR_IS_PUFF);
  		if (bleed)
  		{
  			P_SpawnBlood(x, y, z, (source->angle + angleoffset) - ANG180, newdam > 0 ? newdam : damage, hitactor);
--- 4219,4236 ----
  		}
  		if (spawnpuff)
  		{
! 			P_SpawnPuff(source, puffclass, x, y, z, (source->angle + angleoffset) - ANG90, 1, puffflags, hitactor);
  		}
+ 		
  		if (puffDefaults && puffDefaults->PoisonDamage > 0 && puffDefaults->PoisonDuration != INT_MIN)
  		{
  			P_PoisonMobj(hitactor, thepuff ? thepuff : source, source, puffDefaults->PoisonDamage, puffDefaults->PoisonDuration, puffDefaults->PoisonPeriod, puffDefaults->PoisonDamageType);
  		}
! 		int dmgFlagPass = DMG_INFLICTOR_IS_PUFF;
! 		dmgFlagPass += (puffDefaults->flags3 & MF3_FOILINVUL) ? DMG_FOILINVUL : 0; //[MC]Because the original foilinvul check wasn't working.
! 		dmgFlagPass += (puffDefaults->flags7 & MF7_FOILBUDDHA) ? DMG_FOILBUDDHA : 0;
! 		int newdam = P_DamageMobj(hitactor, thepuff ? thepuff : source, source, damage, damagetype, dmgFlagPass);
! 
  		if (bleed)
  		{
  			P_SpawnBlood(x, y, z, (source->angle + angleoffset) - ANG180, newdam > 0 ? newdam : damage, hitactor);
***************
*** 4129,4135 ****
  
  void P_AimCamera(AActor *t1, fixed_t &CameraX, fixed_t &CameraY, fixed_t &CameraZ, sector_t *&CameraSector)
  {
! 	fixed_t distance = (fixed_t)(chase_dist * FRACUNIT);
  	angle_t angle = (t1->angle - ANG180) >> ANGLETOFINESHIFT;
  	angle_t pitch = (angle_t)(t1->pitch) >> ANGLETOFINESHIFT;
  	FTraceResults trace;
--- 4282,4288 ----
  
  void P_AimCamera(AActor *t1, fixed_t &CameraX, fixed_t &CameraY, fixed_t &CameraZ, sector_t *&CameraSector)
  {
! 	fixed_t distance = (fixed_t)(clamp<double>(chase_dist, 0, 30000) * FRACUNIT);
  	angle_t angle = (t1->angle - ANG180) >> ANGLETOFINESHIFT;
  	angle_t pitch = (angle_t)(t1->pitch) >> ANGLETOFINESHIFT;
  	FTraceResults trace;
***************
*** 4139,4145 ****
  	vy = FixedMul(finecosine[pitch], finesine[angle]);
  	vz = finesine[pitch];
  
! 	sz = t1->z - t1->floorclip + t1->height + (fixed_t)(chase_height * FRACUNIT);
  
  	if (Trace(t1->x, t1->y, sz, t1->Sector,
  		vx, vy, vz, distance, 0, 0, NULL, trace) &&
--- 4292,4298 ----
  	vy = FixedMul(finecosine[pitch], finesine[angle]);
  	vz = finesine[pitch];
  
! 	sz = t1->z - t1->floorclip + t1->height + (fixed_t)(clamp<double>(chase_height, -1000, 1000) * FRACUNIT);
  
  	if (Trace(t1->x, t1->y, sz, t1->Sector,
  		vx, vy, vz, distance, 0, 0, NULL, trace) &&
***************
*** 4615,4621 ****
  			points *= thing->GetClass()->Meta.GetMetaFixed(AMETA_RDFactor, FRACUNIT) / (double)FRACUNIT;
  
  			// points and bombdamage should be the same sign
! 			if ((points * bombdamage) > 0 && P_CheckSight(thing, bombspot, SF_IGNOREVISIBILITY | SF_IGNOREWATERBOUNDARY))
  			{ // OK to damage; target is in direct path
  				double velz;
  				double thrust;
--- 4768,4774 ----
  			points *= thing->GetClass()->Meta.GetMetaFixed(AMETA_RDFactor, FRACUNIT) / (double)FRACUNIT;
  
  			// points and bombdamage should be the same sign
! 			if (((bombspot->flags7 & MF7_CAUSEPAIN) || (points * bombdamage) > 0) && P_CheckSight(thing, bombspot, SF_IGNOREVISIBILITY | SF_IGNOREWATERBOUNDARY))
  			{ // OK to damage; target is in direct path
  				double velz;
  				double thrust;
***************
*** 4624,4630 ****
  
  				if (!(flags & RADF_NODAMAGE))
  					newdam = P_DamageMobj(thing, bombspot, bombsource, damage, bombmod);
! 				else if (thing->player == NULL && !(flags & RADF_NOIMPACTDAMAGE))
  					thing->flags2 |= MF2_BLASTED;
  
  				if (!(thing->flags & MF_ICECORPSE))
--- 4777,4783 ----
  
  				if (!(flags & RADF_NODAMAGE))
  					newdam = P_DamageMobj(thing, bombspot, bombsource, damage, bombmod);
! 				else if (thing->player == NULL && (!(flags & RADF_NOIMPACTDAMAGE) && !(thing->flags7 & MF7_DONTTHRUST)))
  					thing->flags2 |= MF2_BLASTED;
  
  				if (!(thing->flags & MF_ICECORPSE))
***************
*** 4636,4660 ****
  					{
  						if (bombsource == NULL || !(bombsource->flags2 & MF2_NODMGTHRUST))
  						{
! 							thrust = points * 0.5f / (double)thing->Mass;
! 							if (bombsource == thing)
! 							{
! 								thrust *= selfthrustscale;
! 							}
! 							velz = (double)(thing->z + (thing->height >> 1) - bombspot->z) * thrust;
! 							if (bombsource != thing)
  							{
! 								velz *= 0.5f;
! 							}
! 							else
! 							{
! 								velz *= 0.8f;
  							}
- 							angle_t ang = R_PointToAngle2(bombspot->x, bombspot->y, thing->x, thing->y) >> ANGLETOFINESHIFT;
- 							thing->velx += fixed_t(finecosine[ang] * thrust);
- 							thing->vely += fixed_t(finesine[ang] * thrust);
- 							if (!(flags & RADF_NODAMAGE))
- 								thing->velz += (fixed_t)velz;	// this really doesn't work well
  						}
  					}
  				}
--- 4789,4817 ----
  					{
  						if (bombsource == NULL || !(bombsource->flags2 & MF2_NODMGTHRUST))
  						{
! 							if (!(thing->flags7 & MF7_DONTTHRUST))
  							{
! 							
! 								thrust = points * 0.5f / (double)thing->Mass;
! 								if (bombsource == thing)
! 								{
! 									thrust *= selfthrustscale;
! 								}
! 								velz = (double)(thing->z + (thing->height >> 1) - bombspot->z) * thrust;
! 								if (bombsource != thing)
! 								{
! 									velz *= 0.5f;
! 								}
! 								else
! 								{
! 									velz *= 0.8f;
! 								}
! 								angle_t ang = R_PointToAngle2(bombspot->x, bombspot->y, thing->x, thing->y) >> ANGLETOFINESHIFT;
! 								thing->velx += fixed_t(finecosine[ang] * thrust);
! 								thing->vely += fixed_t(finesine[ang] * thrust);
! 								if (!(flags & RADF_NODAMAGE))
! 									thing->velz += (fixed_t)velz;	// this really doesn't work well
  							}
  						}
  					}
  				}
***************
*** 4968,4973 ****
--- 5125,5132 ----
  		// is normally for projectiles which would have exploded by now anyway...
  		if (thing->flags6 & MF6_THRUSPECIES && thing->GetSpecies() == intersect->GetSpecies())
  			continue;
+ 		if ((thing->flags & MF_MISSILE) && (intersect->flags2 & MF2_REFLECTIVE) && (intersect->flags7 & MF7_THRUREFLECT))
+ 			continue;
  		if (!(intersect->flags2 & MF2_PASSMOBJ) ||
  			(!(intersect->flags3 & MF3_ISMONSTER) && intersect->Mass > mymass) ||
  			(intersect->flags4 & MF4_ACTLIKEBRIDGE)
***************
*** 4976,4982 ****
  			// Can't push bridges or things more massive than ourself
  			return 2;
  		}
! 		fixed_t oldz = intersect->z;
  		P_AdjustFloorCeil(intersect, cpos);
  		intersect->z = thing->z + thing->height + 1;
  		if (P_PushUp(intersect, cpos))
--- 5135,5142 ----
  			// Can't push bridges or things more massive than ourself
  			return 2;
  		}
! 		fixed_t oldz;
! 		oldz = intersect->z;
  		P_AdjustFloorCeil(intersect, cpos);
  		intersect->z = thing->z + thing->height + 1;
  		if (P_PushUp(intersect, cpos))
