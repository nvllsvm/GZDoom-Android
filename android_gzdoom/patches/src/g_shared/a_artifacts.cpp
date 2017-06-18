*** /home/draje/Code/gzdoom/src/g_shared/a_artifacts.cpp	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_shared/a_artifacts.cpp	2017-06-18 09:09:24.168397311 -0400
***************
*** 60,66 ****
  	}
  	if (BlendColor != 0)
  	{
! 		power->BlendColor = BlendColor;
  	}
  	if (Mode != NAME_None)
  	{
--- 60,67 ----
  	}
  	if (BlendColor != 0)
  	{
! 		if (BlendColor != MakeSpecialColormap(65535)) power->BlendColor = BlendColor;
! 		else power->BlendColor = 0;
  	}
  	if (Mode != NAME_None)
  	{
***************
*** 1296,1301 ****
--- 1297,1314 ----
  	PositionAccuracy ();
  }
  
+ bool APowerTargeter::HandlePickup(AInventory *item)
+ {
+ 	if (Super::HandlePickup(item))
+ 	{
+ 		InitEffect();	// reset the HUD sprites
+ 		return true;
+ 	}
+ 	return false;
+ }
+ 
+ 
+ 
  void APowerTargeter::DoEffect ()
  {
  	Super::DoEffect ();
***************
*** 1384,1389 ****
--- 1397,1438 ----
  	Owner->player->cheats &= ~CF_FRIGHTENING;
  }
  
+ // Buddha Powerup --------------------------------
+ 
+ IMPLEMENT_CLASS (APowerBuddha)
+ 
+ //===========================================================================
+ //
+ // APowerBuddha :: InitEffect
+ //
+ //===========================================================================
+ 
+ void APowerBuddha::InitEffect ()
+ {
+ 	Super::InitEffect();
+ 
+ 	if (Owner== NULL || Owner->player == NULL)
+ 		return;
+ 
+ 	Owner->player->cheats |= CF_BUDDHA;
+ }
+ 
+ //===========================================================================
+ //
+ // APowerBuddha :: EndEffect
+ //
+ //===========================================================================
+ 
+ void APowerBuddha::EndEffect ()
+ {
+ 	Super::EndEffect();
+ 
+ 	if (Owner== NULL || Owner->player == NULL)
+ 		return;
+ 
+ 	Owner->player->cheats &= ~CF_BUDDHA;
+ }
+ 
  // Scanner powerup ----------------------------------------------------------
  
  IMPLEMENT_CLASS (APowerScanner)
