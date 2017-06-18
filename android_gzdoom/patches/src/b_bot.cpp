*** /home/draje/Code/gzdoom/src/b_bot.cpp	2017-06-18 07:52:50.683013539 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/b_bot.cpp	2017-06-18 10:53:13.119733588 -0400
***************
*** 1,7 ****
! // Cajun bot console commands.
  //
! // [RH] Moved out of d_netcmd.c (in Cajun source), because they don't really
! // belong there.
  
  #include "c_cvars.h"
  #include "c_dispatch.h"
--- 1,7 ----
! // Cajun bot
  //
! // [RH] Moved console commands out of d_netcmd.c (in Cajun source), because
! // they don't really belong there.
  
  #include "c_cvars.h"
  #include "c_dispatch.h"
***************
*** 14,19 ****
--- 14,115 ----
  #include "d_net.h"
  #include "farchive.h"
  
+ IMPLEMENT_POINTY_CLASS(DBot)
+  DECLARE_POINTER(dest)
+  DECLARE_POINTER(prev)
+  DECLARE_POINTER(enemy)
+  DECLARE_POINTER(missile)
+  DECLARE_POINTER(mate)
+  DECLARE_POINTER(last_mate)
+ END_POINTERS
+ 
+ DBot::DBot ()
+ : DThinker(STAT_BOT)
+ {
+ 	Clear ();
+ }
+ 
+ void DBot::Clear ()
+ {
+ 	player = NULL;
+ 	angle = 0;
+ 	dest = NULL;
+ 	prev = NULL;
+ 	enemy = NULL;
+ 	missile = NULL;
+ 	mate = NULL;
+ 	last_mate = NULL;
+ 	memset(&skill, 0, sizeof(skill));
+ 	t_active = 0;
+ 	t_respawn = 0;
+ 	t_strafe = 0;
+ 	t_react = 0;
+ 	t_fight = 0;
+ 	t_roam = 0;
+ 	t_rocket = 0;
+ 	first_shot = true;
+ 	sleft = false;
+ 	allround = false;
+ 	increase = false;
+ 	oldx = 0;
+ 	oldy = 0;
+ }
+ 
+ void DBot::Serialize (FArchive &arc)
+ {
+ 	Super::Serialize (arc);
+ 
+ 	if (SaveVersion < 4515)
+ 	{
+ 		angle_t savedyaw;
+ 		int savedpitch;
+ 		arc << savedyaw
+ 			<< savedpitch;
+ 	}
+ 	else
+ 	{
+ 		arc << player;
+ 	}
+ 
+ 	arc << angle
+ 		<< dest
+ 		<< prev
+ 		<< enemy
+ 		<< missile
+ 		<< mate
+ 		<< last_mate
+ 		<< skill
+ 		<< t_active
+ 		<< t_respawn
+ 		<< t_strafe
+ 		<< t_react
+ 		<< t_fight
+ 		<< t_roam
+ 		<< t_rocket
+ 		<< first_shot
+ 		<< sleft
+ 		<< allround
+ 		<< increase
+ 		<< oldx
+ 		<< oldy;
+ }
+ 
+ void DBot::Tick ()
+ {
+ 	Super::Tick ();
+ 
+ 	if (player->mo == NULL || bglobal.freeze)
+ 	{
+ 		return;
+ 	}
+ 
+ 	BotThinkCycles.Clock();
+ 	bglobal.m_Thinking = true;
+ 	Think ();
+ 	bglobal.m_Thinking = false;
+ 	BotThinkCycles.Unclock();
+ }
+ 
  CVAR (Int, bot_next_color, 11, 0)
  CVAR (Bool, bot_observer, false, 0)
  
***************
*** 55,63 ****
  		bot = bot->next;
  	if (bot)
  	{
! 		bot->inuse = false;
  		bot->lastteam = keepTeam ? players[i].userinfo.GetTeam() : TEAM_NONE;
  	}
  	players[i].~player_t();
  	::new(&players[i]) player_t;
  	players[i].userinfo.Reset();
--- 151,164 ----
  		bot = bot->next;
  	if (bot)
  	{
! 		bot->inuse = BOTINUSE_No;
  		bot->lastteam = keepTeam ? players[i].userinfo.GetTeam() : TEAM_NONE;
  	}
+ 	if (players[i].Bot != NULL)
+ 	{
+ 		players[i].Bot->Destroy ();
+ 		players[i].Bot = NULL;
+ 	}
  	players[i].~player_t();
  	::new(&players[i]) player_t;
  	players[i].userinfo.Reset();
***************
*** 66,71 ****
--- 167,178 ----
  
  CCMD (removebots)
  {
+ 	if (!players[consoleplayer].settings_controller)
+ 	{
+ 		Printf ("Only setting controllers can remove bots\n");
+ 		return;
+ 	}
+ 
  	Net_WriteByte (DEM_KILLBOTS);
  }
  
***************
*** 91,97 ****
  
  	while (thebot)
  	{
! 		Printf ("%s%s\n", thebot->name, thebot->inuse ? " (active)" : "");
  		thebot = thebot->next;
  		count++;
  	}
--- 198,204 ----
  
  	while (thebot)
  	{
! 		Printf ("%s%s\n", thebot->name, thebot->inuse == BOTINUSE_Yes ? " (active)" : "");
  		thebot = thebot->next;
  		count++;
  	}
