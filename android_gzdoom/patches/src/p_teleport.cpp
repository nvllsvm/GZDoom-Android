*** /home/draje/Code/gzdoom/src/p_teleport.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_teleport.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 74,92 ****
  //
  //==========================================================================
  
! void P_SpawnTeleportFog(fixed_t x, fixed_t y, fixed_t z, int spawnid)
  {
! 	const PClass *fog = P_GetSpawnableType(spawnid);
! 
! 	if (fog == NULL)
  	{
! 		AActor *mo = Spawn ("TeleportFog", x, y, z + TELEFOGHEIGHT, ALLOW_REPLACE);
  	}
  	else
  	{
! 		AActor *mo = Spawn (fog, x, y, z, ALLOW_REPLACE);
! 		if (mo != NULL) S_Sound(mo, CHAN_BODY, mo->SeeSound, 1.f, ATTN_NORM);
  	}
  }
  
  //
--- 74,95 ----
  //
  //==========================================================================
  
! void P_SpawnTeleportFog(AActor *mobj, fixed_t x, fixed_t y, fixed_t z, bool beforeTele, bool setTarget)
  {
! 	AActor *mo;
! 	if ((beforeTele ? mobj->TeleFogSourceType : mobj->TeleFogDestType) == NULL)
  	{
! 		//Do nothing.
! 		mo = NULL;
  	}
  	else
  	{
! 		mo = Spawn((beforeTele ? mobj->TeleFogSourceType : mobj->TeleFogDestType), x, y, z, ALLOW_REPLACE);
  	}
+ 
+ 	if (mo != NULL && setTarget)
+ 		mo->target = mobj;
+ 
  }
  
  //
***************
*** 96,101 ****
--- 99,106 ----
  bool P_Teleport (AActor *thing, fixed_t x, fixed_t y, fixed_t z, angle_t angle,
  				 bool useFog, bool sourceFog, bool keepOrientation, bool bHaltVelocity, bool keepHeight)
  {
+ 	bool predicting = (thing->player && (thing->player->cheats & CF_PREDICTING));
+ 
  	fixed_t oldx;
  	fixed_t oldy;
  	fixed_t oldz;
***************
*** 181,199 ****
  		angle = thing->angle;
  	}
  	// Spawn teleport fog at source and destination
! 	if (sourceFog)
  	{
  		fixed_t fogDelta = thing->flags & MF_MISSILE ? 0 : TELEFOGHEIGHT;
! 		AActor *fog = Spawn<ATeleportFog> (oldx, oldy, oldz + fogDelta, ALLOW_REPLACE);
! 		fog->target = thing;
  	}
  	if (useFog)
  	{
! 		fixed_t fogDelta = thing->flags & MF_MISSILE ? 0 : TELEFOGHEIGHT;
! 		an = angle >> ANGLETOFINESHIFT;
! 		AActor *fog = Spawn<ATeleportFog> (x + 20*finecosine[an],
! 			y + 20*finesine[an], thing->z + fogDelta, ALLOW_REPLACE);
! 		fog->target = thing;
  		if (thing->player)
  		{
  			// [RH] Zoom player's field of vision
--- 186,205 ----
  		angle = thing->angle;
  	}
  	// Spawn teleport fog at source and destination
! 	if (sourceFog && !predicting)
  	{
  		fixed_t fogDelta = thing->flags & MF_MISSILE ? 0 : TELEFOGHEIGHT;
! 		P_SpawnTeleportFog(thing, oldx, oldy, oldz, true, true); //Passes the actor through which then pulls the TeleFog metadate types based on properties.
  	}
  	if (useFog)
  	{
! 		if (!predicting)
! 		{
! 			fixed_t fogDelta = thing->flags & MF_MISSILE ? 0 : TELEFOGHEIGHT;
! 			an = angle >> ANGLETOFINESHIFT;
! 			P_SpawnTeleportFog(thing, x + 20 * finecosine[an], y + 20 * finesine[an], thing->z + fogDelta, false, true);
! 
! 		}
  		if (thing->player)
  		{
  			// [RH] Zoom player's field of vision
***************
*** 226,232 ****
  	return true;
  }
  
! static AActor *SelectTeleDest (int tid, int tag)
  {
  	AActor *searcher;
  
--- 232,238 ----
  	return true;
  }
  
! static AActor *SelectTeleDest (int tid, int tag, bool norandom)
  {
  	AActor *searcher;
  
***************
*** 276,282 ****
  		}
  		else
  		{
! 			if (count != 1)
  			{
  				count = 1 + (pr_teleport() % count);
  			}
--- 282,288 ----
  		}
  		else
  		{
! 			if (count != 1 && !norandom)
  			{
  				count = 1 + (pr_teleport() % count);
  			}
***************
*** 323,328 ****
--- 329,335 ----
  bool EV_Teleport (int tid, int tag, line_t *line, int side, AActor *thing, bool fog,
  				  bool sourceFog, bool keepOrientation, bool haltVelocity, bool keepHeight)
  {
+ 
  	AActor *searcher;
  	fixed_t z;
  	angle_t angle = 0;
***************
*** 334,339 ****
--- 341,347 ----
  	{ // Teleport function called with an invalid actor
  		return false;
  	}
+ 	bool predicting = (thing->player && (thing->player->cheats & CF_PREDICTING));
  	if (thing->flags2 & MF2_NOTELEPORT)
  	{
  		return false;
***************
*** 342,348 ****
  	{ // Don't teleport if hit back of line, so you can get out of teleporter.
  		return 0;
  	}
! 	searcher = SelectTeleDest (tid, tag);
  	if (searcher == NULL)
  	{
  		return false;
--- 350,356 ----
  	{ // Don't teleport if hit back of line, so you can get out of teleporter.
  		return 0;
  	}
! 	searcher = SelectTeleDest(tid, tag, predicting);
  	if (searcher == NULL)
  	{
  		return false;
***************
*** 390,396 ****
  			thing->velx = FixedMul(velx, c) - FixedMul(vely, s);
  			thing->vely = FixedMul(vely, c) + FixedMul(velx, s);
  		}
! 		if ((velx | vely) == 0 && thing->player != NULL && thing->player->mo == thing)
  		{
  			thing->player->mo->PlayIdle ();
  		}
--- 398,404 ----
  			thing->velx = FixedMul(velx, c) - FixedMul(vely, s);
  			thing->vely = FixedMul(vely, c) + FixedMul(velx, s);
  		}
! 		if ((velx | vely) == 0 && thing->player != NULL && thing->player->mo == thing && !predicting)
  		{
  			thing->player->mo->PlayIdle ();
  		}
