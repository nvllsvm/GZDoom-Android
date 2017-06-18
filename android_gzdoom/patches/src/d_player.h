*** /home/draje/Code/gzdoom/src/d_player.h	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/d_player.h	2017-06-18 09:09:24.161730580 -0400
***************
*** 76,83 ****
  void P_EnumPlayerColorSets(FName classname, TArray<int> *out);
  const char *GetPrintableDisplayName(const PClass *cls);
  
- class player_t;
- 
  class APlayerPawn : public AActor
  {
  	DECLARE_CLASS (APlayerPawn, AActor)
--- 76,81 ----
***************
*** 176,182 ****
  	PST_LIVE,	// Playing or camping.
  	PST_DEAD,	// Dead on the ground, view follows killer.
  	PST_REBORN,	// Ready to restart/respawn???
! 	PST_ENTER	// [BC] Entered the game
  } playerstate_t;
  
  
--- 174,181 ----
  	PST_LIVE,	// Playing or camping.
  	PST_DEAD,	// Dead on the ground, view follows killer.
  	PST_REBORN,	// Ready to restart/respawn???
! 	PST_ENTER,	// [BC] Entered the game
! 	PST_GONE	// Player has left the game
  } playerstate_t;
  
  
***************
*** 206,211 ****
--- 205,212 ----
  	CF_DOUBLEFIRINGSPEED= 1 << 21,		// Player owns a double firing speed artifact
  	CF_EXTREMELYDEAD	= 1 << 22,		// [RH] Reliably let the status bar know about extreme deaths.
  	CF_INFINITEAMMO		= 1 << 23,		// Player owns an infinite ammo artifact
+ 	CF_BUDDHA2			= 1 << 24,		// [MC] Absolute buddha. No voodoo can kill it either.
+ 	CF_GODMODE2			= 1 << 25,		// [MC] Absolute godmode. No voodoo can kill it either.
  	CF_BUDDHA			= 1 << 27,		// [SP] Buddha mode - take damage, but don't die
  	CF_NOCLIP2			= 1 << 30,		// [RH] More Quake-like noclip
  } cheat_t;
***************
*** 398,405 ****
  
  	int			inventorytics;
  	BYTE		CurrentPlayerClass;		// class # for this player instance
! 	bool		backpack;
! 	
  	int			frags[MAXPLAYERS];		// kills of other players
  	int			fragcount;				// [RH] Cumulative frags for this player
  	int			lastkilltime;			// [RH] For multikills
--- 399,405 ----
  
  	int			inventorytics;
  	BYTE		CurrentPlayerClass;		// class # for this player instance
! 
  	int			frags[MAXPLAYERS];		// kills of other players
  	int			fragcount;				// [RH] Cumulative frags for this player
  	int			lastkilltime;			// [RH] For multikills
***************
*** 444,490 ****
  	FName		LastDamageType;			// [RH] For damage-specific pain and death sounds
  
  	//Added by MC:
! 	angle_t		savedyaw;
! 	int			savedpitch;
! 
! 	angle_t		angle;		// The wanted angle that the bot try to get every tic.
! 							//  (used to get a smoth view movement)
! 	TObjPtr<AActor>		dest;		// Move Destination.
! 	TObjPtr<AActor>		prev;		// Previous move destination.
! 
! 
! 	TObjPtr<AActor>		enemy;		// The dead meat.
! 	TObjPtr<AActor>		missile;	// A threatening missile that needs to be avoided.
! 	TObjPtr<AActor>		mate;		// Friend (used for grouping in teamplay or coop).
! 	TObjPtr<AActor>		last_mate;	// If bots mate disappeared (not if died) that mate is
! 							// pointed to by this. Allows bot to roam to it if
! 							// necessary.
  
  	bool		settings_controller;	// Player can control game settings.
  
- 	//Skills
- 	struct botskill_t	skill;
- 
- 	//Tickers
- 	int			t_active;	// Open door, lower lift stuff, door must open and
- 							// lift must go down before bot does anything
- 							// radical like try a stuckmove
- 	int			t_respawn;
- 	int			t_strafe;
- 	int			t_react;
- 	int			t_fight;
- 	int			t_roam;
- 	int			t_rocket;
- 
- 	//Misc booleans
- 	bool		isbot;
- 	bool		first_shot;	// Used for reaction skill.
- 	bool		sleft;		// If false, strafe is right.
- 	bool		allround;
- 
- 	fixed_t		oldx;
- 	fixed_t		oldy;
- 
  	float		BlendR;		// [RH] Final blending values
  	float		BlendG;
  	float		BlendB;
--- 444,453 ----
  	FName		LastDamageType;			// [RH] For damage-specific pain and death sounds
  
  	//Added by MC:
! 	TObjPtr<DBot> Bot;
  
  	bool		settings_controller;	// Player can control game settings.
  
  	float		BlendR;		// [RH] Final blending values
  	float		BlendG;
  	float		BlendB;
