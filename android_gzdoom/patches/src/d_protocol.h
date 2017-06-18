*** /home/draje/Code/gzdoom/src/d_protocol.h	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/d_protocol.h	2017-06-18 09:09:24.161730580 -0400
***************
*** 112,118 ****
  	DEM_DROPPLAYER,		// 13 Not implemented, takes a byte
  	DEM_CHANGEMAP,		// 14 Name of map to change to
  	DEM_SUICIDE,		// 15 Player wants to die
! 	DEM_ADDBOT,			// 16 Byte: player#, String: userinfo for bot
  	DEM_KILLBOTS,		// 17 Remove all bots from the world
  	DEM_INVUSEALL,		// 18 Use every item (panic!)
  	DEM_INVUSE,			// 19 4 bytes: ID of item to use
--- 112,118 ----
  	DEM_DROPPLAYER,		// 13 Not implemented, takes a byte
  	DEM_CHANGEMAP,		// 14 Name of map to change to
  	DEM_SUICIDE,		// 15 Player wants to die
! 	DEM_ADDBOT,			// 16 Byte: botshift, String: userinfo for bot, 4 Bytes: skill (aiming, perfection, reaction, isp)
  	DEM_KILLBOTS,		// 17 Remove all bots from the world
  	DEM_INVUSEALL,		// 18 Use every item (panic!)
  	DEM_INVUSE,			// 19 4 bytes: ID of item to use
***************
*** 219,225 ****
  	CHT_GIMMIEJ,
  	CHT_GIMMIEZ,
  	CHT_BUDDHA,
! 	CHT_NOCLIP2
  };
  
  void StartChunk (int id, BYTE **stream);
--- 219,227 ----
  	CHT_GIMMIEJ,
  	CHT_GIMMIEZ,
  	CHT_BUDDHA,
! 	CHT_NOCLIP2,
! 	CHT_BUDDHA2,
! 	CHT_GOD2
  };
  
  void StartChunk (int id, BYTE **stream);
