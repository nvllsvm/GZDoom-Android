*** /home/draje/Code/gzdoom/src/p_local.h	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_local.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 105,110 ****
--- 105,111 ----
  void	P_PlayerThink (player_t *player);
  void	P_PredictPlayer (player_t *player);
  void	P_UnPredictPlayer ();
+ void	P_PredictionLerpReset();
  
  //
  // P_MOBJ
***************
*** 132,138 ****
  	PF_NORANDOMZ = 16
  };
  
! AActor *P_SpawnPuff (AActor *source, const PClass *pufftype, fixed_t x, fixed_t y, fixed_t z, angle_t dir, int updown, int flags = 0);
  void	P_SpawnBlood (fixed_t x, fixed_t y, fixed_t z, angle_t dir, int damage, AActor *originator);
  void	P_BloodSplatter (fixed_t x, fixed_t y, fixed_t z, AActor *originator);
  void	P_BloodSplatter2 (fixed_t x, fixed_t y, fixed_t z, AActor *originator);
--- 133,139 ----
  	PF_NORANDOMZ = 16
  };
  
! AActor *P_SpawnPuff (AActor *source, const PClass *pufftype, fixed_t x, fixed_t y, fixed_t z, angle_t dir, int updown, int flags = 0, AActor *vict = NULL);
  void	P_SpawnBlood (fixed_t x, fixed_t y, fixed_t z, angle_t dir, int damage, AActor *originator);
  void	P_BloodSplatter (fixed_t x, fixed_t y, fixed_t z, AActor *originator);
  void	P_BloodSplatter2 (fixed_t x, fixed_t y, fixed_t z, AActor *originator);
***************
*** 170,176 ****
  int		P_Thing_Damage (int tid, AActor *whofor0, int amount, FName type);
  void	P_Thing_SetVelocity(AActor *actor, fixed_t vx, fixed_t vy, fixed_t vz, bool add, bool setbob);
  void P_RemoveThing(AActor * actor);
! bool P_Thing_Raise(AActor *thing);
  const PClass *P_GetSpawnableType(int spawnnum);
  
  //
--- 171,178 ----
  int		P_Thing_Damage (int tid, AActor *whofor0, int amount, FName type);
  void	P_Thing_SetVelocity(AActor *actor, fixed_t vx, fixed_t vy, fixed_t vz, bool add, bool setbob);
  void P_RemoveThing(AActor * actor);
! bool P_Thing_Raise(AActor *thing, AActor *raiser);
! bool P_Thing_CanRaise(AActor *thing);
  const PClass *P_GetSpawnableType(int spawnnum);
  
  //
***************
*** 462,467 ****
--- 464,470 ----
  
  AActor *P_LineAttack (AActor *t1, angle_t angle, fixed_t distance, int pitch, int damage, FName damageType, const PClass *pufftype, int flags = 0, AActor **victim = NULL, int *actualdamage = NULL);
  AActor *P_LineAttack (AActor *t1, angle_t angle, fixed_t distance, int pitch, int damage, FName damageType, FName pufftype, int flags = 0, AActor **victim = NULL, int *actualdamage = NULL);
+ AActor *P_LinePickActor (AActor *t1, angle_t angle, fixed_t distance, int pitch, DWORD actorMask, DWORD wallMask);
  void	P_TraceBleed (int damage, fixed_t x, fixed_t y, fixed_t z, AActor *target, angle_t angle, int pitch);
  void	P_TraceBleed (int damage, AActor *target, angle_t angle, int pitch);
  void	P_TraceBleed (int damage, AActor *target, AActor *missile);		// missile version
***************
*** 556,561 ****
--- 559,566 ----
  	DMG_NO_FACTOR = 16,
  	DMG_PLAYERATTACK = 32,
  	DMG_FOILINVUL = 64,
+ 	DMG_FOILBUDDHA = 128,
+ 	DMG_NO_PROTECT = 256,
  };
  
  
