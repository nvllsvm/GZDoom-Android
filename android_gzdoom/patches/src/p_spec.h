*** /home/draje/Code/gzdoom/src/p_spec.h	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_spec.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 166,171 ****
--- 166,172 ----
  // when needed
  bool	P_ActivateLine (line_t *ld, AActor *mo, int side, int activationType);
  bool	P_TestActivateLine (line_t *ld, AActor *mo, int side, int activationType);
+ bool	P_PredictLine (line_t *ld, AActor *mo, int side, int activationType);
  
  void 	P_PlayerInSpecialSector (player_t *player, sector_t * sector=NULL);
  void	P_PlayerOnSpecialFlat (player_t *player, int floorType);
***************
*** 514,521 ****
  	DPillar ();
  };
  
! bool EV_DoPillar (DPillar::EPillar type, int tag, fixed_t speed, fixed_t height,
! 				  fixed_t height2, int crush, bool hexencrush);
  
  //
  // P_DOORS
--- 515,522 ----
  	DPillar ();
  };
  
! bool EV_DoPillar (DPillar::EPillar type, line_t *line, int tag,
! 				  fixed_t speed, fixed_t height, fixed_t height2, int crush, bool hexencrush);
  
  //
  // P_DOORS
***************
*** 902,907 ****
--- 903,909 ----
  //
  // P_TELEPT
  //
+ void P_SpawnTeleportFog(AActor *mobj, fixed_t x, fixed_t y, fixed_t z, bool beforeTele = true, bool setTarget = false); //Spawns teleport fog. Pass the actor to pluck TeleFogFromType and TeleFogToType. 'from' determines if this is the fog to spawn at the old position (true) or new (false).
  bool P_Teleport (AActor *thing, fixed_t x, fixed_t y, fixed_t z, angle_t angle, bool useFog, bool sourceFog, bool keepOrientation, bool haltVelocity = true, bool keepHeight = false);
  bool EV_Teleport (int tid, int tag, line_t *line, int side, AActor *thing, bool fog, bool sourceFog, bool keepOrientation, bool haltVelocity = true, bool keepHeight = false);
  bool EV_SilentLineTeleport (line_t *line, int side, AActor *thing, int id, INTBOOL reverse);
