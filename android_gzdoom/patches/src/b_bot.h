*** /home/draje/Code/gzdoom/src/b_bot.h	2017-06-18 07:52:50.683013539 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/b_bot.h	2017-06-18 10:53:11.513052457 -0400
***************
*** 14,19 ****
--- 14,20 ----
  #include "d_ticcmd.h"
  #include "r_defs.h"
  #include "a_pickups.h"
+ #include "stats.h"
  
  #define FORWARDWALK		0x1900
  #define FORWARDRUN		0x3200
***************
*** 60,65 ****
--- 61,73 ----
  
  FArchive &operator<< (FArchive &arc, botskill_t &skill);
  
+ enum
+ {
+ 	BOTINUSE_No,
+ 	BOTINUSE_Waiting,
+ 	BOTINUSE_Yes,
+ };
+ 
  //Info about all bots in the bots.cfg
  //Updated during each level start.
  //Info given to bots when they're spawned.
***************
*** 69,75 ****
  	char *name;
  	char *info;
  	botskill_t skill;
! 	bool inuse;
  	int lastteam;
  };
  
--- 77,83 ----
  	char *name;
  	char *info;
  	botskill_t skill;
! 	int inuse;
  	int lastteam;
  };
  
***************
*** 81,115 ****
  
  	void ClearPlayer (int playernum, bool keepTeam);
  
! 	//(B_Game.c)
! 	void Main (int buf);
  	void Init ();
  	void End();
- 	void CleanBotstuff (player_t *p);
  	bool SpawnBot (const char *name, int color = NOCOLOR);
  	bool LoadBots ();
  	void ForgetBots ();
- 	void DoAddBot (int bnum, char *info);
- 	void RemoveAllBots (bool fromlist);
- 
- 	//(B_Func.c)
- 	bool Check_LOS (AActor *mobj1, AActor *mobj2, angle_t vangle);
  
! 	//(B_Think.c)
! 	void WhatToGet (AActor *actor, AActor *item);
  
! 	//(B_move.c)
! 	void Roam (AActor *actor, ticcmd_t *cmd);
! 	bool Move (AActor *actor, ticcmd_t *cmd);
! 	bool TryWalk (AActor *actor, ticcmd_t *cmd);
! 	void NewChaseDir (AActor *actor, ticcmd_t *cmd);
  	bool CleanAhead (AActor *thing, fixed_t x, fixed_t y, ticcmd_t *cmd);
- 	void TurnToAng (AActor *actor);
- 	void Pitch (AActor *actor, AActor *target);
  	bool IsDangerous (sector_t *sec);
  
  	TArray<FString> getspawned; //Array of bots (their names) which should be spawned when starting a game.
- 	bool botingame[MAXPLAYERS];
  	BYTE freeze:1;			//Game in freeze mode.
  	BYTE changefreeze:1;	//Game wants to change freeze mode.
  	int botnum;
--- 89,117 ----
  
  	void ClearPlayer (int playernum, bool keepTeam);
  
! 	//(b_game.cpp)
! 	void Main ();
  	void Init ();
  	void End();
  	bool SpawnBot (const char *name, int color = NOCOLOR);
+ 	void TryAddBot (BYTE **stream, int player);
+ 	void RemoveAllBots (bool fromlist);
  	bool LoadBots ();
  	void ForgetBots ();
  
! 	//(b_func.cpp)
! 	void StartTravel ();
! 	void FinishTravel ();
! 	bool IsLeader (player_t *player);
! 	void SetBodyAt (fixed_t x, fixed_t y, fixed_t z, int hostnum);
! 	fixed_t FakeFire (AActor *source, AActor *dest, ticcmd_t *cmd);
! 	bool SafeCheckPosition (AActor *actor, fixed_t x, fixed_t y, FCheckPosition &tm);
  
! 	//(b_move.cpp)
  	bool CleanAhead (AActor *thing, fixed_t x, fixed_t y, ticcmd_t *cmd);
  	bool IsDangerous (sector_t *sec);
  
  	TArray<FString> getspawned; //Array of bots (their names) which should be spawned when starting a game.
  	BYTE freeze:1;			//Game in freeze mode.
  	BYTE changefreeze:1;	//Game wants to change freeze mode.
  	int botnum;
***************
*** 123,153 ****
  	bool	 m_Thinking;
  
  private:
! 	//(B_Func.c)
! 	bool Reachable (AActor *actor, AActor *target);
! 	void Dofire (AActor *actor, ticcmd_t *cmd);
! 	AActor *Choose_Mate (AActor *bot);
! 	AActor *Find_enemy (AActor *bot);
! 	void SetBodyAt (fixed_t x, fixed_t y, fixed_t z, int hostnum);
! 	fixed_t FakeFire (AActor *source, AActor *dest, ticcmd_t *cmd);
! 	angle_t FireRox (AActor *bot, AActor *enemy, ticcmd_t *cmd);
! 	bool SafeCheckPosition (AActor *actor, fixed_t x, fixed_t y, FCheckPosition &tm);
! 
! 	//(B_Think.c)
! 	void Think (AActor *actor, ticcmd_t *cmd);
! 	void ThinkForMove (AActor *actor, ticcmd_t *cmd);
! 	void Set_enemy (AActor *actor);
  
  protected:
  	bool	 ctf;
- 	int		 loaded_bots;
  	int		 t_join;
  	bool	 observer; //Consoleplayer is observer.
  };
  
  
  //Externs
  extern FCajunMaster bglobal;
  
  EXTERN_CVAR (Float, bot_flag_return_time)
  EXTERN_CVAR (Int, bot_next_color)
--- 125,217 ----
  	bool	 m_Thinking;
  
  private:
! 	//(b_game.cpp)
! 	bool DoAddBot (BYTE *info, botskill_t skill);
  
  protected:
  	bool	 ctf;
  	int		 t_join;
  	bool	 observer; //Consoleplayer is observer.
  };
  
+ class DBot : public DThinker
+ {
+ 	DECLARE_CLASS(DBot,DThinker)
+ 	HAS_OBJECT_POINTERS
+ public:
+ 	DBot ();
+ 
+ 	void Clear ();
+ 	void Serialize (FArchive &arc);
+ 	void Tick ();
+ 
+ 	//(b_think.cpp)
+ 	void WhatToGet (AActor *item);
+ 
+ 	//(b_func.cpp)
+ 	bool Check_LOS (AActor *to, angle_t vangle);
+ 
+ 	player_t	*player;
+ 	angle_t		angle;		// The wanted angle that the bot try to get every tic.
+ 							//  (used to get a smooth view movement)
+ 	TObjPtr<AActor>		dest;		// Move Destination.
+ 	TObjPtr<AActor>		prev;		// Previous move destination.
+ 	TObjPtr<AActor>		enemy;		// The dead meat.
+ 	TObjPtr<AActor>		missile;	// A threatening missile that needs to be avoided.
+ 	TObjPtr<AActor>		mate;		// Friend (used for grouping in teamplay or coop).
+ 	TObjPtr<AActor>		last_mate;	// If bots mate disappeared (not if died) that mate is
+ 							// pointed to by this. Allows bot to roam to it if
+ 							// necessary.
+ 
+ 	//Skills
+ 	struct botskill_t	skill;
+ 
+ 	//Tickers
+ 	int			t_active;	// Open door, lower lift stuff, door must open and
+ 							// lift must go down before bot does anything
+ 							// radical like try a stuckmove
+ 	int			t_respawn;
+ 	int			t_strafe;
+ 	int			t_react;
+ 	int			t_fight;
+ 	int			t_roam;
+ 	int			t_rocket;
+ 
+ 	//Misc booleans
+ 	bool		first_shot;	// Used for reaction skill.
+ 	bool		sleft;		// If false, strafe is right.
+ 	bool		allround;
+ 	bool		increase;
+ 
+ 	fixed_t		oldx;
+ 	fixed_t		oldy;
+ 
+ private:
+ 	//(b_think.cpp)
+ 	void Think ();
+ 	void ThinkForMove (ticcmd_t *cmd);
+ 	void Set_enemy ();
+ 
+ 	//(b_func.cpp)
+ 	bool Reachable (AActor *target);
+ 	void Dofire (ticcmd_t *cmd);
+ 	AActor *Choose_Mate ();
+ 	AActor *Find_enemy ();
+ 	angle_t FireRox (AActor *enemy, ticcmd_t *cmd);
+ 
+ 	//(b_move.cpp)
+ 	void Roam (ticcmd_t *cmd);
+ 	bool Move (ticcmd_t *cmd);
+ 	bool TryWalk (ticcmd_t *cmd);
+ 	void NewChaseDir (ticcmd_t *cmd);
+ 	void TurnToAng ();
+ 	void Pitch (AActor *target);
+ };
+ 
  
  //Externs
  extern FCajunMaster bglobal;
+ extern cycle_t BotThinkCycles, BotSupportCycles;
  
  EXTERN_CVAR (Float, bot_flag_return_time)
  EXTERN_CVAR (Int, bot_next_color)
***************
*** 158,164 ****
  EXTERN_CVAR (Bool, bot_chat)
  
  #endif	// __B_BOT_H__
- 
- 
- 
- 
--- 222,224 ----
