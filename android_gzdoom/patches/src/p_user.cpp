*** /home/draje/Code/gzdoom/src/p_user.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_user.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 62,69 ****
  
  // Variables for prediction
  CVAR (Bool, cl_noprediction, false, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  static player_t PredictionPlayerBackup;
! static BYTE PredictionActorBackup[sizeof(AActor)];
  static TArray<sector_t *> PredictionTouchingSectorsBackup;
  static TArray<AActor *> PredictionSectorListBackup;
  static TArray<msecnode_t *> PredictionSector_sprev_Backup;
--- 62,93 ----
  
  // Variables for prediction
  CVAR (Bool, cl_noprediction, false, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
+ CVAR(Bool, cl_predict_specials, true, CVAR_ARCHIVE | CVAR_GLOBALCONFIG)
+ 
+ CUSTOM_CVAR(Float, cl_predict_lerpscale, 0.05f, CVAR_ARCHIVE | CVAR_GLOBALCONFIG)
+ {
+ 	P_PredictionLerpReset();
+ }
+ CUSTOM_CVAR(Float, cl_predict_lerpthreshold, 2.00f, CVAR_ARCHIVE | CVAR_GLOBALCONFIG)
+ {
+ 	if (self < 0.1f)
+ 		self = 0.1f;
+ 	P_PredictionLerpReset();
+ }
+ 
+ struct PredictPos
+ {
+ 	int gametic;
+ 	fixed_t x;
+ 	fixed_t y;
+ 	fixed_t z;
+ 	fixed_t pitch;
+ 	fixed_t yaw;
+ } static PredictionLerpFrom, PredictionLerpResult, PredictionLast;
+ static int PredictionLerptics;
+ 
  static player_t PredictionPlayerBackup;
! static BYTE PredictionActorBackup[sizeof(APlayerPawn)];
  static TArray<sector_t *> PredictionTouchingSectorsBackup;
  static TArray<AActor *> PredictionSectorListBackup;
  static TArray<msecnode_t *> PredictionSector_sprev_Backup;
***************
*** 239,245 ****
    health(0),
    inventorytics(0),
    CurrentPlayerClass(0),
-   backpack(0),
    fragcount(0),
    lastkilltime(0),
    multicount(0),
--- 263,268 ----
***************
*** 272,299 ****
    respawn_time(0),
    camera(0),
    air_finished(0),
!   savedyaw(0),
!   savedpitch(0),
!   angle(0),
!   dest(0),
!   prev(0),
!   enemy(0),
!   missile(0),
!   mate(0),
!   last_mate(0),
!   t_active(0),
!   t_respawn(0),
!   t_strafe(0),
!   t_react(0),
!   t_fight(0),
!   t_roam(0),
!   t_rocket(0),
!   isbot(0),
!   first_shot(0),
!   sleft(0),
!   allround(0),
!   oldx(0),
!   oldy(0),
    BlendR(0),
    BlendG(0),
    BlendB(0),
--- 295,301 ----
    respawn_time(0),
    camera(0),
    air_finished(0),
!   Bot(0),
    BlendR(0),
    BlendG(0),
    BlendB(0),
***************
*** 312,318 ****
  	memset (&cmd, 0, sizeof(cmd));
  	memset (frags, 0, sizeof(frags));
  	memset (psprites, 0, sizeof(psprites));
- 	memset (&skill, 0, sizeof(skill));
  }
  
  player_t &player_t::operator=(const player_t &p)
--- 314,319 ----
***************
*** 340,346 ****
  	health = p.health;
  	inventorytics = p.inventorytics;
  	CurrentPlayerClass = p.CurrentPlayerClass;
- 	backpack = p.backpack;
  	memcpy(frags, &p.frags, sizeof(frags));
  	fragcount = p.fragcount;
  	lastkilltime = p.lastkilltime;
--- 341,346 ----
***************
*** 381,410 ****
  	camera = p.camera;
  	air_finished = p.air_finished;
  	LastDamageType = p.LastDamageType;
! 	savedyaw = p.savedyaw;
! 	savedpitch = p.savedpitch;
! 	angle = p.angle;
! 	dest = p.dest;
! 	prev = p.prev;
! 	enemy = p.enemy;
! 	missile = p.missile;
! 	mate = p.mate;
! 	last_mate = p.last_mate;
  	settings_controller = p.settings_controller;
- 	skill = p.skill;
- 	t_active = p.t_active;
- 	t_respawn = p.t_respawn;
- 	t_strafe = p.t_strafe;
- 	t_react = p.t_react;
- 	t_fight = p.t_fight;
- 	t_roam = p.t_roam;
- 	t_rocket = p.t_rocket;
- 	isbot = p.isbot;
- 	first_shot = p.first_shot;
- 	sleft = p.sleft;
- 	allround = p.allround;
- 	oldx = p.oldx;
- 	oldy = p.oldy;
  	BlendR = p.BlendR;
  	BlendG = p.BlendG;
  	BlendB = p.BlendB;
--- 381,388 ----
  	camera = p.camera;
  	air_finished = p.air_finished;
  	LastDamageType = p.LastDamageType;
! 	Bot = p.Bot;
  	settings_controller = p.settings_controller;
  	BlendR = p.BlendR;
  	BlendG = p.BlendG;
  	BlendB = p.BlendB;
***************
*** 446,457 ****
  	if (*&poisoner == old)			poisoner = replacement, changed++;
  	if (*&attacker == old)			attacker = replacement, changed++;
  	if (*&camera == old)			camera = replacement, changed++;
! 	if (*&dest == old)				dest = replacement, changed++;
! 	if (*&prev == old)				prev = replacement, changed++;
! 	if (*&enemy == old)				enemy = replacement, changed++;
! 	if (*&missile == old)			missile = replacement, changed++;
! 	if (*&mate == old)				mate = replacement, changed++;
! 	if (*&last_mate == old)			last_mate = replacement, changed++;
  	if (ReadyWeapon == old)			ReadyWeapon = static_cast<AWeapon *>(rep), changed++;
  	if (PendingWeapon == old)		PendingWeapon = static_cast<AWeapon *>(rep), changed++;
  	if (*&PremorphWeapon == old)	PremorphWeapon = static_cast<AWeapon *>(rep), changed++;
--- 424,430 ----
  	if (*&poisoner == old)			poisoner = replacement, changed++;
  	if (*&attacker == old)			attacker = replacement, changed++;
  	if (*&camera == old)			camera = replacement, changed++;
! 	if (*&Bot == old)				Bot = static_cast<DBot *>(rep), changed++;
  	if (ReadyWeapon == old)			ReadyWeapon = static_cast<AWeapon *>(rep), changed++;
  	if (PendingWeapon == old)		PendingWeapon = static_cast<AWeapon *>(rep), changed++;
  	if (*&PremorphWeapon == old)	PremorphWeapon = static_cast<AWeapon *>(rep), changed++;
***************
*** 466,477 ****
  	GC::Mark(poisoner);
  	GC::Mark(attacker);
  	GC::Mark(camera);
! 	GC::Mark(dest);
! 	GC::Mark(prev);
! 	GC::Mark(enemy);
! 	GC::Mark(missile);
! 	GC::Mark(mate);
! 	GC::Mark(last_mate);
  	GC::Mark(ReadyWeapon);
  	GC::Mark(ConversationNPC);
  	GC::Mark(ConversationPC);
--- 439,445 ----
  	GC::Mark(poisoner);
  	GC::Mark(attacker);
  	GC::Mark(camera);
! 	GC::Mark(Bot);
  	GC::Mark(ReadyWeapon);
  	GC::Mark(ConversationNPC);
  	GC::Mark(ConversationPC);
***************
*** 720,729 ****
  		// If we're the local player, then there's a bit more work to do.
  		// This also applies if we're a bot and this is the net arbitrator.
  		if (player - players == consoleplayer ||
! 			(player->isbot && consoleplayer == Net_Arbitrator))
  		{
  			FWeaponSlots local_slots(player->weapons);
! 			if (player->isbot)
  			{ // Bots only need weapons from KEYCONF, not INI modifications.
  				P_PlaybackKeyConfWeapons(&local_slots);
  			}
--- 688,697 ----
  		// If we're the local player, then there's a bit more work to do.
  		// This also applies if we're a bot and this is the net arbitrator.
  		if (player - players == consoleplayer ||
! 			(player->Bot != NULL && consoleplayer == Net_Arbitrator))
  		{
  			FWeaponSlots local_slots(player->weapons);
! 			if (player->Bot != NULL)
  			{ // Bots only need weapons from KEYCONF, not INI modifications.
  				P_PlaybackKeyConfWeapons(&local_slots);
  			}
***************
*** 1237,1245 ****
  {
  	if (player == NULL) return;
  
- 	// [GRB] Give inventory specified in DECORATE
- 	player->health = GetDefault ()->health;
- 
  	// HexenArmor must always be the first item in the inventory because
  	// it provides player class based protection that should not affect
  	// any other protection item.
--- 1205,1210 ----
***************
*** 1400,1406 ****
  					weap->SpawnState != ::GetDefault<AActor>()->SpawnState)
  				{
  					item = P_DropItem (this, weap->GetClass(), -1, 256);
! 					if (item != NULL)
  					{
  						if (weap->AmmoGive1 && weap->Ammo1)
  						{
--- 1365,1371 ----
  					weap->SpawnState != ::GetDefault<AActor>()->SpawnState)
  				{
  					item = P_DropItem (this, weap->GetClass(), -1, 256);
! 					if (item != NULL && item->IsKindOf(RUNTIME_CLASS(AWeapon)))
  					{
  						if (weap->AmmoGive1 && weap->Ammo1)
  						{
***************
*** 2155,2161 ****
  	if ((player->cmd.ucmd.buttons & BT_USE ||
  		((multiplayer || alwaysapplydmflags) && (dmflags & DF_FORCE_RESPAWN))) && !(dmflags2 & DF2_NO_RESPAWN))
  	{
! 		if (level.time >= player->respawn_time || ((player->cmd.ucmd.buttons & BT_USE) && !player->isbot))
  		{
  			player->cls = NULL;		// Force a new class if the player is using a random class
  			player->playerstate = (multiplayer || (level.flags2 & LEVEL2_ALLOWRESPAWN)) ? PST_REBORN : PST_ENTER;
--- 2120,2126 ----
  	if ((player->cmd.ucmd.buttons & BT_USE ||
  		((multiplayer || alwaysapplydmflags) && (dmflags & DF_FORCE_RESPAWN))) && !(dmflags2 & DF2_NO_RESPAWN))
  	{
! 		if (level.time >= player->respawn_time || ((player->cmd.ucmd.buttons & BT_USE) && player->Bot == NULL))
  		{
  			player->cls = NULL;		// Force a new class if the player is using a random class
  			player->playerstate = (multiplayer || (level.flags2 & LEVEL2_ALLOWRESPAWN)) ? PST_REBORN : PST_ENTER;
***************
*** 2625,2631 ****
  		{
  			if (player->mo->waterlevel < 3 ||
  				(player->mo->flags2 & MF2_INVULNERABLE) ||
! 				(player->cheats & (CF_GODMODE | CF_NOCLIP2)))
  			{
  				player->mo->ResetAirSupply ();
  			}
--- 2590,2597 ----
  		{
  			if (player->mo->waterlevel < 3 ||
  				(player->mo->flags2 & MF2_INVULNERABLE) ||
! 				(player->cheats & (CF_GODMODE | CF_NOCLIP2)) ||
! 				(player->cheats & CF_GODMODE2))
  			{
  				player->mo->ResetAirSupply ();
  			}
***************
*** 2637,2642 ****
--- 2603,2631 ----
  	}
  }
  
+ void P_PredictionLerpReset()
+ {
+ 	PredictionLerptics = PredictionLast.gametic = PredictionLerpFrom.gametic = PredictionLerpResult.gametic = 0;
+ }
+ 
+ bool P_LerpCalculate(PredictPos from, PredictPos to, PredictPos &result, float scale)
+ {
+ 	FVector3 vecFrom(FIXED2DBL(from.x), FIXED2DBL(from.y), FIXED2DBL(from.z));
+ 	FVector3 vecTo(FIXED2DBL(to.x), FIXED2DBL(to.y), FIXED2DBL(to.z));
+ 	FVector3 vecResult;
+ 	vecResult = vecTo - vecFrom;
+ 	vecResult *= scale;
+ 	vecResult = vecResult + vecFrom;
+ 	FVector3 delta = vecResult - vecTo;
+ 
+ 	result.x = FLOAT2FIXED(vecResult.X);
+ 	result.y = FLOAT2FIXED(vecResult.Y);
+ 	result.z = FLOAT2FIXED(vecResult.Z);
+ 
+ 	// As a fail safe, assume extrapolation is the threshold.
+ 	return (delta.LengthSquared() > cl_predict_lerpthreshold && scale <= 1.00f);
+ }
+ 
  void P_PredictPlayer (player_t *player)
  {
  	int maxtic;
***************
*** 2664,2671 ****
  	// Save original values for restoration later
  	PredictionPlayerBackup = *player;
  
! 	AActor *act = player->mo;
! 	memcpy (PredictionActorBackup, &act->x, sizeof(AActor)-((BYTE *)&act->x-(BYTE *)act));
  
  	act->flags &= ~MF_PICKUP;
  	act->flags2 &= ~MF2_PUSHWALL;
--- 2653,2660 ----
  	// Save original values for restoration later
  	PredictionPlayerBackup = *player;
  
! 	APlayerPawn *act = player->mo;
! 	memcpy(PredictionActorBackup, &act->x, sizeof(APlayerPawn) - ((BYTE *)&act->x - (BYTE *)act));
  
  	act->flags &= ~MF_PICKUP;
  	act->flags2 &= ~MF2_PUSHWALL;
***************
*** 2722,2732 ****
--- 2711,2777 ----
  	}
  	act->BlockNode = NULL;
  
+ 	// Values too small to be usable for lerping can be considered "off".
+ 	bool CanLerp = (!(cl_predict_lerpscale < 0.01f) && (ticdup == 1)), DoLerp = false, NoInterpolateOld = R_GetViewInterpolationStatus();
  	for (int i = gametic; i < maxtic; ++i)
  	{
+ 		if (!NoInterpolateOld)
+ 			R_RebuildViewInterpolation(player);
+ 
  		player->cmd = localcmds[i % LOCALCMDTICS];
  		P_PlayerThink (player);
  		player->mo->Tick ();
+ 
+ 		if (CanLerp && PredictionLast.gametic > 0 && i == PredictionLast.gametic && !NoInterpolateOld)
+ 		{
+ 			// Z is not compared as lifts will alter this with no apparent change
+ 			// Make lerping less picky by only testing whole units
+ 			DoLerp = ((PredictionLast.x >> 16) != (player->mo->x >> 16) ||
+ 				(PredictionLast.y >> 16) != (player->mo->y >> 16));
+ 
+ 			// Aditional Debug information
+ 			if (developer && DoLerp)
+ 			{
+ 				DPrintf("Lerp! Ltic (%d) && Ptic (%d) | Lx (%d) && Px (%d) | Ly (%d) && Py (%d)\n",
+ 					PredictionLast.gametic, i,
+ 					(PredictionLast.x >> 16), (player->mo->x >> 16),
+ 					(PredictionLast.y >> 16), (player->mo->y >> 16));
+ 			}
+ 		}
+ 	}
+ 
+ 	if (CanLerp)
+ 	{
+ 		if (NoInterpolateOld)
+ 			P_PredictionLerpReset();
+ 
+ 		else if (DoLerp)
+ 		{
+ 			// If lerping is already in effect, use the previous camera postion so the view doesn't suddenly snap
+ 			PredictionLerpFrom = (PredictionLerptics == 0) ? PredictionLast : PredictionLerpResult;
+ 			PredictionLerptics = 1;
+ 		}
+ 
+ 		PredictionLast.gametic = maxtic - 1;
+ 		PredictionLast.x = player->mo->x;
+ 		PredictionLast.y = player->mo->y;
+ 		PredictionLast.z = player->mo->z;
+ 
+ 		if (PredictionLerptics > 0)
+ 		{
+ 			if (PredictionLerpFrom.gametic > 0 &&
+ 				P_LerpCalculate(PredictionLerpFrom, PredictionLast, PredictionLerpResult, (float)PredictionLerptics * cl_predict_lerpscale))
+ 			{
+ 				PredictionLerptics++;
+ 				player->mo->x = PredictionLerpResult.x;
+ 				player->mo->y = PredictionLerpResult.y;
+ 				player->mo->z = PredictionLerpResult.z;
+ 			}
+ 			else
+ 			{
+ 				PredictionLerptics = 0;
+ 			}
+ 		}
  	}
  }
  
***************
*** 2739,2747 ****
  	if (player->cheats & CF_PREDICTING)
  	{
  		unsigned int i;
! 		AActor *act = player->mo;
  		AActor *savedcamera = player->camera;
  
  		*player = PredictionPlayerBackup;
  
  		// Restore the camera instead of using the backup's copy, because spynext/prev
--- 2784,2795 ----
  	if (player->cheats & CF_PREDICTING)
  	{
  		unsigned int i;
! 		APlayerPawn *act = player->mo;
  		AActor *savedcamera = player->camera;
  
+ 		TObjPtr<AInventory> InvSel = act->InvSel;
+ 		int inventorytics = player->inventorytics;
+ 
  		*player = PredictionPlayerBackup;
  
  		// Restore the camera instead of using the backup's copy, because spynext/prev
***************
*** 2749,2755 ****
  		player->camera = savedcamera;
  
  		act->UnlinkFromWorld();
! 		memcpy(&act->x, PredictionActorBackup, sizeof(AActor)-((BYTE *)&act->x - (BYTE *)act));
  
  		// The blockmap ordering needs to remain unchanged, too.
  		// Restore sector links and refrences.
--- 2797,2803 ----
  		player->camera = savedcamera;
  
  		act->UnlinkFromWorld();
! 		memcpy(&act->x, PredictionActorBackup, sizeof(APlayerPawn) - ((BYTE *)&act->x - (BYTE *)act));
  
  		// The blockmap ordering needs to remain unchanged, too.
  		// Restore sector links and refrences.
***************
*** 2854,2859 ****
--- 2902,2910 ----
  			}
  			block = block->NextBlock;
  		}
+ 
+ 		act->InvSel = InvSel;
+ 		player->inventorytics = inventorytics;
  	}
  }
  
***************
*** 2884,2892 ****
  		<< vely
  		<< centering
  		<< health
! 		<< inventorytics
! 		<< backpack
! 		<< fragcount
  		<< spreecount
  		<< multicount
  		<< lastkilltime
--- 2935,2947 ----
  		<< vely
  		<< centering
  		<< health
! 		<< inventorytics;
! 	if (SaveVersion < 4513)
! 	{
! 		bool backpack;
! 		arc << backpack;
! 	}
! 	arc << fragcount
  		<< spreecount
  		<< multicount
  		<< lastkilltime
***************
*** 2915,2923 ****
  		<< respawn_time
  		<< air_finished
  		<< turnticks
! 		<< oldbuttons
! 		<< isbot
! 		<< BlendR
  		<< BlendG
  		<< BlendB
  		<< BlendA;
--- 2970,2986 ----
  		<< respawn_time
  		<< air_finished
  		<< turnticks
! 		<< oldbuttons;
! 	bool IsBot;
! 	if (SaveVersion >= 4514)
! 	{
! 		arc << Bot;
! 	}
! 	else
! 	{
! 		arc << IsBot;
! 	}
! 	arc << BlendR
  		<< BlendG
  		<< BlendB
  		<< BlendA;
***************
*** 3000,3032 ****
  		onground = (mo->z <= mo->floorz) || (mo->flags2 & MF2_ONMOBJ) || (mo->BounceFlags & BOUNCE_MBF) || (cheats & CF_NOCLIP2);
  	}
  
! 	if (isbot)
  	{
! 		arc	<< angle
! 			<< dest
! 			<< prev
! 			<< enemy
! 			<< missile
! 			<< mate
! 			<< last_mate
! 			<< skill
! 			<< t_active
! 			<< t_respawn
! 			<< t_strafe
! 			<< t_react
! 			<< t_fight
! 			<< t_roam
! 			<< t_rocket
! 			<< first_shot
! 			<< sleft
! 			<< allround
! 			<< oldx
! 			<< oldy;
  	}
! 	else
  	{
! 		dest = prev = enemy = missile = mate = last_mate = NULL;
  	}
  	if (arc.IsLoading ())
  	{
  		// If the player reloaded because they pressed +use after dying, we
--- 3063,3099 ----
  		onground = (mo->z <= mo->floorz) || (mo->flags2 & MF2_ONMOBJ) || (mo->BounceFlags & BOUNCE_MBF) || (cheats & CF_NOCLIP2);
  	}
  
! 	if (SaveVersion < 4514 && IsBot)
  	{
! 		Bot = new DBot;
! 
! 		arc	<< Bot->angle
! 			<< Bot->dest
! 			<< Bot->prev
! 			<< Bot->enemy
! 			<< Bot->missile
! 			<< Bot->mate
! 			<< Bot->last_mate
! 			<< Bot->skill
! 			<< Bot->t_active
! 			<< Bot->t_respawn
! 			<< Bot->t_strafe
! 			<< Bot->t_react
! 			<< Bot->t_fight
! 			<< Bot->t_roam
! 			<< Bot->t_rocket
! 			<< Bot->first_shot
! 			<< Bot->sleft
! 			<< Bot->allround
! 			<< Bot->oldx
! 			<< Bot->oldy;
  	}
! 
! 	if (SaveVersion < 4516 && Bot != NULL)
  	{
! 		Bot->player = this;
  	}
+ 
  	if (arc.IsLoading ())
  	{
  		// If the player reloaded because they pressed +use after dying, we
