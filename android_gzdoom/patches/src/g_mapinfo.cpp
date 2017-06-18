*** /home/draje/Code/gzdoom/src/g_mapinfo.cpp	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_mapinfo.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 1190,1195 ****
--- 1190,1198 ----
  	MITYPE_SETFLAG2,
  	MITYPE_CLRFLAG2,
  	MITYPE_SCFLAGS2,
+ 	MITYPE_SETFLAG3,
+ 	MITYPE_CLRFLAG3,
+ 	MITYPE_SCFLAGS3,
  	MITYPE_COMPATFLAG,
  };
  
***************
*** 1275,1280 ****
--- 1278,1284 ----
  	{ "rememberstate",					MITYPE_CLRFLAG2,	LEVEL2_FORGETSTATE, 0 },
  	{ "unfreezesingleplayerconversations",MITYPE_SETFLAG2,	LEVEL2_CONV_SINGLE_UNFREEZE, 0 },
  	{ "spawnwithweaponraised",			MITYPE_SETFLAG2,	LEVEL2_PRERAISEWEAPON, 0 },
+ 	{ "forcefakecontrast",				MITYPE_SETFLAG3,	LEVEL3_FORCEFAKECONTRAST, 0 },
  	{ "nobotnodes",						MITYPE_IGNORE,	0, 0 },		// Skulltag option: nobotnodes
  	{ "compat_shorttex",				MITYPE_COMPATFLAG, COMPATF_SHORTTEX, 0 },
  	{ "compat_stairs",					MITYPE_COMPATFLAG, COMPATF_STAIRINDEX, 0 },
***************
*** 1306,1311 ****
--- 1310,1316 ----
  	{ "compat_maskedmidtex",			MITYPE_COMPATFLAG, COMPATF_MASKEDMIDTEX, 0 },
  	{ "compat_badangles",				MITYPE_COMPATFLAG, 0, COMPATF2_BADANGLES },
  	{ "compat_floormove",				MITYPE_COMPATFLAG, 0, COMPATF2_FLOORMOVE },
+ 	{ "compat_soundcutoff",				MITYPE_COMPATFLAG, 0, COMPATF2_SOUNDCUTOFF },
  	{ "cd_start_track",					MITYPE_EATNEXT,	0, 0 },
  	{ "cd_end1_track",					MITYPE_EATNEXT,	0, 0 },
  	{ "cd_end2_track",					MITYPE_EATNEXT,	0, 0 },
***************
*** 1371,1376 ****
--- 1376,1395 ----
  				info.flags2 = (info.flags2 & handler->data2) | handler->data1;
  				break;
  
+ 			case MITYPE_SETFLAG3:
+ 				info.flags3 |= handler->data1;
+ 				info.flags3 |= handler->data2;
+ 				break;
+ 
+ 			case MITYPE_CLRFLAG3:
+ 				info.flags3 &= ~handler->data1;
+ 				info.flags3 |= handler->data2;
+ 				break;
+ 
+ 			case MITYPE_SCFLAGS3:
+ 				info.flags3 = (info.flags3 & handler->data2) | handler->data1;
+ 				break;
+ 
  			case MITYPE_COMPATFLAG:
  			{
  				int set = 1;
