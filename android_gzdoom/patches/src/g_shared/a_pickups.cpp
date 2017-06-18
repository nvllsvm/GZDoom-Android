*** /home/draje/Code/gzdoom/src/g_shared/a_pickups.cpp	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_shared/a_pickups.cpp	2017-06-18 09:09:24.168397311 -0400
***************
*** 499,505 ****
  {
  	if ((ItemFlags & IF_BIGPOWERUP) && !(dmflags2 & DF2_RESPAWN_SUPER)) return false;
  	if (ItemFlags & IF_NEVERRESPAWN) return false;
! 	return !!(dmflags & DF_ITEMS_RESPAWN);
  }
  
  //===========================================================================
--- 499,505 ----
  {
  	if ((ItemFlags & IF_BIGPOWERUP) && !(dmflags2 & DF2_RESPAWN_SUPER)) return false;
  	if (ItemFlags & IF_NEVERRESPAWN) return false;
! 	return !!((dmflags & DF_ITEMS_RESPAWN) || (ItemFlags & IF_ALWAYSRESPAWN));
  }
  
  //===========================================================================
***************
*** 1024,1031 ****
  	//Added by MC: Check if item taken was the roam destination of any bot
  	for (int i = 0; i < MAXPLAYERS; i++)
  	{
! 		if (playeringame[i] && this == players[i].dest)
!     		players[i].dest = NULL;
  	}
  }
  
--- 1024,1031 ----
  	//Added by MC: Check if item taken was the roam destination of any bot
  	for (int i = 0; i < MAXPLAYERS; i++)
  	{
! 		if (players[i].Bot != NULL && this == players[i].Bot->dest)
! 			players[i].Bot->dest = NULL;
  	}
  }
  
