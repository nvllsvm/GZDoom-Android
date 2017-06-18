*** /home/draje/Code/gzdoom/src/g_shared/a_morph.cpp	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_shared/a_morph.cpp	2017-06-18 09:09:24.168397311 -0400
***************
*** 12,17 ****
--- 12,18 ----
  #include "doomstat.h"
  #include "g_level.h"
  #include "farchive.h"
+ #include "p_enemy.h"
  
  static FRandom pr_morphmonst ("MorphMonster");
  
***************
*** 527,545 ****
  	if (actor->GetClass()->IsDescendantOf(RUNTIME_CLASS(AMorphedMonster)))
  	{
  		AMorphedMonster *fakeme = static_cast<AMorphedMonster *>(actor);
! 		if ((fakeme->UnmorphTime) &&
! 			(fakeme->MorphStyle & MORPH_UNDOBYDEATH) &&
! 			(fakeme->UnmorphedMe))
  		{
! 			AActor *realme = fakeme->UnmorphedMe;
! 			int realstyle = fakeme->MorphStyle;
! 			int realhealth = fakeme->health;
! 			if (P_UndoMonsterMorph(fakeme, !!(fakeme->MorphStyle & MORPH_UNDOBYDEATHFORCED)))
  			{
! 				*morphed = realme;
! 				*morphedstyle = realstyle;
! 				*morphedhealth = realhealth;
! 				return true;
  			}
  		}
  		fakeme->flags3 |= MF3_STAYMORPHED; // moved here from AMorphedMonster::Die()
--- 528,553 ----
  	if (actor->GetClass()->IsDescendantOf(RUNTIME_CLASS(AMorphedMonster)))
  	{
  		AMorphedMonster *fakeme = static_cast<AMorphedMonster *>(actor);
! 		AActor *realme = fakeme->UnmorphedMe;
! 		if (realme != NULL)
  		{
! 			if ((fakeme->UnmorphTime) &&
! 				(fakeme->MorphStyle & MORPH_UNDOBYDEATH))
  			{
! 				int realstyle = fakeme->MorphStyle;
! 				int realhealth = fakeme->health;
! 				if (P_UndoMonsterMorph(fakeme, !!(fakeme->MorphStyle & MORPH_UNDOBYDEATHFORCED)))
! 				{
! 					*morphed = realme;
! 					*morphedstyle = realstyle;
! 					*morphedhealth = realhealth;
! 					return true;
! 				}
! 			}
! 			if (realme->flags4 & MF4_BOSSDEATH)
! 			{
! 				realme->health = 0;	// make sure that A_BossDeath considers it dead.
! 				CALL_ACTION(A_BossDeath, realme);
  			}
  		}
  		fakeme->flags3 |= MF3_STAYMORPHED; // moved here from AMorphedMonster::Die()
