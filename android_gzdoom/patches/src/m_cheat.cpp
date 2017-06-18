*** /home/draje/Code/gzdoom/src/m_cheat.cpp	2017-06-18 07:52:50.696346991 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/m_cheat.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 99,104 ****
--- 99,121 ----
  			msg = GStrings("TXT_BUDDHAOFF");
  		break;
  
+ 	case CHT_GOD2:
+ 		player->cheats ^= CF_GODMODE2;
+ 		if (player->cheats & CF_GODMODE2)
+ 			msg = GStrings("STSTR_DQD2ON");
+ 		else
+ 			msg = GStrings("STSTR_DQD2OFF");
+ 		ST_SetNeedRefresh();
+ 		break;
+ 
+ 	case CHT_BUDDHA2:
+ 		player->cheats ^= CF_BUDDHA2;
+ 		if (player->cheats & CF_BUDDHA2)
+ 			msg = GStrings("TXT_BUDDHA2ON");
+ 		else
+ 			msg = GStrings("TXT_BUDDHA2OFF");
+ 		break;
+ 
  	case CHT_NOCLIP:
  		player->cheats ^= CF_NOCLIP;
  		if (player->cheats & CF_NOCLIP)
***************
*** 323,329 ****
  					player->mo->Translation = TRANSLATION(TRANSLATION_Players, BYTE(player-players));
  				}
  				player->mo->DamageType = NAME_None;
- //				player->mo->GiveDefaultInventory();
  				if (player->ReadyWeapon != NULL)
  				{
  					P_SetPsprite(player, ps_weapon, player->ReadyWeapon->GetUpState());
--- 340,345 ----
