*** /home/draje/Code/gzdoom/src/p_pspr.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_pspr.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 227,233 ****
  
  	// [SO] 9/2/02: People were able to do an awful lot of damage
  	// when they were observers...
! 	if (!player->isbot && bot_observer)
  	{
  		return;
  	}
--- 227,233 ----
  
  	// [SO] 9/2/02: People were able to do an awful lot of damage
  	// when they were observers...
! 	if (player->Bot == NULL && bot_observer)
  	{
  		return;
  	}
***************
*** 263,269 ****
  
  	// [SO] 9/2/02: People were able to do an awful lot of damage
  	// when they were observers...
! 	if (!player->isbot && bot_observer)
  	{
  		return;
  	}
--- 263,269 ----
  
  	// [SO] 9/2/02: People were able to do an awful lot of damage
  	// when they were observers...
! 	if (player->Bot == NULL && bot_observer)
  	{
  		return;
  	}
***************
*** 298,304 ****
  void P_ReloadWeapon (player_t *player, FState *state)
  {
  	AWeapon *weapon;
! 	if (!player->isbot && bot_observer)
  	{
  		return;
  	}
--- 298,304 ----
  void P_ReloadWeapon (player_t *player, FState *state)
  {
  	AWeapon *weapon;
! 	if (player->Bot == NULL && bot_observer)
  	{
  		return;
  	}
***************
*** 329,335 ****
  void P_ZoomWeapon (player_t *player, FState *state)
  {
  	AWeapon *weapon;
! 	if (!player->isbot && bot_observer)
  	{
  		return;
  	}
--- 329,335 ----
  void P_ZoomWeapon (player_t *player, FState *state)
  {
  	AWeapon *weapon;
! 	if (player->Bot == NULL && bot_observer)
  	{
  		return;
  	}
