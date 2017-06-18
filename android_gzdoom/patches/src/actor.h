*** /home/draje/Code/gzdoom/src/actor.h	2017-06-18 07:52:50.683013539 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/actor.h	2017-06-18 09:09:24.161730580 -0400
***************
*** 341,346 ****
--- 341,360 ----
  	MF7_ALWAYSTELEFRAG	= 0x00000004,	// will unconditionally be telefragged when in the way. Overrides all other settings.
  	MF7_HANDLENODELAY	= 0x00000008,	// respect NoDelay state flag
  	MF7_WEAPONSPAWN		= 0x00000010,	// subject to DF_NO_COOP_WEAPON_SPAWN dmflag
+ 	MF7_HARMFRIENDS		= 0x00000020,	// is allowed to harm friendly monsters.
+ 	MF7_BUDDHA			= 0x00000040,	// Behaves just like the buddha cheat. 
+ 	MF7_FOILBUDDHA		= 0x00000080,	// Similar to FOILINVUL, foils buddha mode.
+ 	MF7_DONTTHRUST		= 0x00000100,	// Thrusting functions do not take, and do not give thrust (damage) to actors with this flag.
+ 	MF7_ALLOWPAIN		= 0x00000200,	// Invulnerable or immune (via damagefactors) actors can still react to taking damage even if they don't.
+ 	MF7_CAUSEPAIN		= 0x00000400,	// Damage sources with this flag can cause similar effects like ALLOWPAIN.
+ 	MF7_THRUREFLECT		= 0x00000800,	// Actors who are reflective cause the missiles to not slow down or change angles.
+ 	MF7_MIRRORREFLECT	= 0x00001000,	// Actor is turned directly 180 degrees around when reflected.
+ 	MF7_AIMREFLECT		= 0x00002000,	// Actor is directly reflected straight back at the one who fired the projectile.
+ 	MF7_HITTARGET		= 0x00004000,	// The actor the projectile dies on is set to target, provided it's targetable anyway.
+ 	MF7_HITMASTER		= 0x00008000,	// Same as HITTARGET, except it's master instead of target.
+ 	MF7_HITTRACER		= 0x00010000,	// Same as HITTARGET, but for tracer.
+ 
+ 
  
  // --- mobj.renderflags ---
  
***************
*** 713,718 ****
--- 727,735 ----
  	// Transforms the actor into a finely-ground paste
  	virtual bool Grind(bool items);
  
+ 	// Get this actor's team
+ 	int GetTeam();
+ 
  	// Is the other actor on my team?
  	bool IsTeammate (AActor *other);
  
***************
*** 768,773 ****
--- 785,791 ----
  	// These also set CF_INTERPVIEW for players.
  	void SetPitch(int p, bool interpolate);
  	void SetAngle(angle_t ang, bool interpolate);
+ 	void SetRoll(angle_t roll, bool interpolate);
  
  	const PClass *GetBloodType(int type = 0) const
  	{
***************
*** 850,856 ****
  	DWORD			flags4;			// [RH] Even more flags!
  	DWORD			flags5;			// OMG! We need another one.
  	DWORD			flags6;			// Shit! Where did all the flags go?
! 	DWORD			flags7;			// 
  
  	// [BB] If 0, everybody can see the actor, if > 0, only members of team (VisibleToTeam-1) can see it.
  	DWORD			VisibleToTeam;
--- 868,874 ----
  	DWORD			flags4;			// [RH] Even more flags!
  	DWORD			flags5;			// OMG! We need another one.
  	DWORD			flags6;			// Shit! Where did all the flags go?
! 	DWORD			flags7;			// WHO WANTS TO BET ON 8!?
  
  	// [BB] If 0, everybody can see the actor, if > 0, only members of team (VisibleToTeam-1) can see it.
  	DWORD			VisibleToTeam;
***************
*** 937,945 ****
  	TObjPtr<AInventory>	Inventory;		// [RH] This actor's inventory
  	DWORD			InventoryID;	// A unique ID to keep track of inventory items
  
- 	//Added by MC:
- 	SDWORD id;						// Player ID (for items, # in list.)
- 
  	BYTE smokecounter;
  	BYTE FloatBobPhase;
  	BYTE FriendPlayer;				// [RH] Player # + 1 this friendly monster works for (so 0 is no player, 1 is player 0, etc)
--- 955,960 ----
***************
*** 965,973 ****
--- 980,994 ----
  	FNameNoInit DamageType;
  	FNameNoInit DamageTypeReceived;
  	fixed_t DamageFactor;
+ 	fixed_t DamageMultiply;
  
  	FNameNoInit PainType;
  	FNameNoInit DeathType;
+ 	const PClass *TeleFogSourceType;
+ 	const PClass *TeleFogDestType;
+ 	int RipperLevel;
+ 	int RipLevelMin;
+ 	int RipLevelMax;
  
  	FState *SpawnState;
  	FState *SeeState;
