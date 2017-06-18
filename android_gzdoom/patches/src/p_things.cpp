*** /home/draje/Code/gzdoom/src/p_things.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_things.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 92,98 ****
  				mobj->angle = (angle != ANGLE_MAX ? angle : spot->angle);
  				if (fog)
  				{
! 					Spawn<ATeleportFog> (spot->x, spot->y, spot->z + TELEFOGHEIGHT, ALLOW_REPLACE);
  				}
  				if (mobj->flags & MF_SPECIAL)
  					mobj->flags |= MF_DROPPED;	// Don't respawn
--- 92,98 ----
  				mobj->angle = (angle != ANGLE_MAX ? angle : spot->angle);
  				if (fog)
  				{
! 					P_SpawnTeleportFog(mobj, spot->x, spot->y, spot->z + TELEFOGHEIGHT, false);
  				}
  				if (mobj->flags & MF_SPECIAL)
  					mobj->flags |= MF_DROPPED;	// Don't respawn
***************
*** 130,137 ****
  	{
  		if (fog)
  		{
! 			Spawn<ATeleportFog> (x, y, z + TELEFOGHEIGHT, ALLOW_REPLACE);
! 			Spawn<ATeleportFog> (oldx, oldy, oldz + TELEFOGHEIGHT, ALLOW_REPLACE);
  		}
  		source->PrevX = x;
  		source->PrevY = y;
--- 130,137 ----
  	{
  		if (fog)
  		{
! 			P_SpawnTeleportFog(source, x, y, z);
! 			P_SpawnTeleportFog(source, oldx, oldy, oldz, false);
  		}
  		source->PrevX = x;
  		source->PrevY = y;
***************
*** 402,414 ****
  	// Don't remove live players.
  	if (actor->player == NULL || actor != actor->player->mo)
  	{
  		// be friendly to the level statistics. ;)
  		actor->ClearCounters();
  		actor->Destroy ();
  	}
  }
  
! bool P_Thing_Raise(AActor *thing)
  {
  	FState * RaiseState = thing->GetRaiseState();
  	if (RaiseState == NULL)
--- 402,418 ----
  	// Don't remove live players.
  	if (actor->player == NULL || actor != actor->player->mo)
  	{
+ 		// Don't also remove owned inventory items
+ 		if (actor->IsKindOf(RUNTIME_CLASS(AInventory)) && static_cast<AInventory*>(actor)->Owner != NULL) return;
+ 
  		// be friendly to the level statistics. ;)
  		actor->ClearCounters();
  		actor->Destroy ();
  	}
+ 
  }
  
! bool P_Thing_Raise(AActor *thing, AActor *raiser)
  {
  	FState * RaiseState = thing->GetRaiseState();
  	if (RaiseState == NULL)
***************
*** 441,450 ****
--- 445,494 ----
  
  	thing->Revive();
  
+ 	if (raiser != NULL)
+ 	{
+ 		// Let's copy the friendliness of the one who raised it.
+ 		thing->CopyFriendliness(raiser, false);
+ 	}
+ 
  	thing->SetState (RaiseState);
  	return true;
  }
  
+ bool P_Thing_CanRaise(AActor *thing)
+ {
+ 	FState * RaiseState = thing->GetRaiseState();
+ 	if (RaiseState == NULL)
+ 	{
+ 		return false;
+ 	}
+ 	
+ 	AActor *info = thing->GetDefault();
+ 
+ 	// Check against real height and radius
+ 	int oldflags = thing->flags;
+ 	fixed_t oldheight = thing->height;
+ 	fixed_t oldradius = thing->radius;
+ 
+ 	thing->flags |= MF_SOLID;
+ 	thing->height = info->height;
+ 	thing->radius = info->radius;
+ 
+ 	bool check = P_CheckPosition (thing, thing->x, thing->y);
+ 
+ 	// Restore checked properties
+ 	thing->flags = oldflags;
+ 	thing->radius = oldradius;
+ 	thing->height = oldheight;
+ 
+ 	if (!check)
+ 	{
+ 		return false;
+ 	}
+ 
+ 	return true;
+ }
+ 
  void P_Thing_SetVelocity(AActor *actor, fixed_t vx, fixed_t vy, fixed_t vz, bool add, bool setbob)
  {
  	if (actor != NULL)
