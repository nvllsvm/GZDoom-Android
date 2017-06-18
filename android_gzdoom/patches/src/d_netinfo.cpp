*** /home/draje/Code/gzdoom/src/d_netinfo.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/d_netinfo.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 60,68 ****
  
  static FRandom pr_pickteam ("PickRandomTeam");
  
- extern bool st_firsttime;
- EXTERN_CVAR (Bool, teamplay)
- 
  CVAR (Float,	autoaim,				5000.f,		CVAR_USERINFO | CVAR_ARCHIVE);
  CVAR (String,	name,					"Player",	CVAR_USERINFO | CVAR_ARCHIVE);
  CVAR (Color,	color,					0x40cf00,	CVAR_USERINFO | CVAR_ARCHIVE);
--- 60,65 ----
