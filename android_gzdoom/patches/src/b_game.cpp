*** /home/draje/Code/gzdoom/src/b_game.cpp	2017-06-18 07:52:50.683013539 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/b_game.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 89,137 ****
  	BOTCFG_TEAM
  };
  
- static bool waitingforspawn[MAXPLAYERS];
- 
  FCajunMaster::~FCajunMaster()
  {
  	ForgetBots();
  }
  
! //This function is called every tick (from g_game.c),
! //send bots into thinking (+more).
! void FCajunMaster::Main (int buf)
  {
- 	int i;
- 
  	BotThinkCycles.Reset();
  
! 	if (consoleplayer != Net_Arbitrator || demoplayback)
! 		return;
! 
! 	if (gamestate != GS_LEVEL)
  		return;
  
- 	m_Thinking = true;
- 
- 	//Think for bots.
- 	if (botnum)
- 	{
- 		BotThinkCycles.Clock();
- 		for (i = 0; i < MAXPLAYERS; i++)
- 		{
- 			if (playeringame[i] && players[i].mo && !freeze && players[i].isbot)
- 				Think (players[i].mo, &netcmds[i][buf]);
- 		}
- 		BotThinkCycles.Unclock();
- 	}
- 
  	//Add new bots?
  	if (wanted_botnum > botnum && !freeze)
  	{
  		if (t_join == ((wanted_botnum - botnum) * SPAWN_DELAY))
  		{
!             if (!SpawnBot (getspawned[spawn_tries]))
  				wanted_botnum--;
!             spawn_tries++;
  		}
  
  		t_join--;
--- 89,115 ----
  	BOTCFG_TEAM
  };
  
  FCajunMaster::~FCajunMaster()
  {
  	ForgetBots();
  }
  
! //This function is called every tick (from g_game.c).
! void FCajunMaster::Main ()
  {
  	BotThinkCycles.Reset();
  
! 	if (demoplayback || gamestate != GS_LEVEL || consoleplayer != Net_Arbitrator)
  		return;
  
  	//Add new bots?
  	if (wanted_botnum > botnum && !freeze)
  	{
  		if (t_join == ((wanted_botnum - botnum) * SPAWN_DELAY))
  		{
! 			if (!SpawnBot (getspawned[spawn_tries]))
  				wanted_botnum--;
! 			spawn_tries++;
  		}
  
  		t_join--;
***************
*** 156,169 ****
  		players[consoleplayer].mo->flags2 &= ~MF2_FLY;
  		players[consoleplayer].mo->LinkToWorld ();
  	}
- 
- 	m_Thinking = false;
  }
  
  void FCajunMaster::Init ()
  {
! 	int i;
! 
  	botnum = 0;
  	firstthing = NULL;
  	spawn_tries = 0;
--- 134,144 ----
  		players[consoleplayer].mo->flags2 &= ~MF2_FLY;
  		players[consoleplayer].mo->LinkToWorld ();
  	}
  }
  
  void FCajunMaster::Init ()
  {
! 	LOGI("FCajunMaster::Init");
  	botnum = 0;
  	firstthing = NULL;
  	spawn_tries = 0;
***************
*** 172,189 ****
  	body1 = NULL;
  	body2 = NULL;
  
- 	//Remove all bots upon each level start, they'll get spawned instead.
- 	for (i = 0; i < MAXPLAYERS; i++)
- 	{
- 		waitingforspawn[i] = false;
- 		if (playeringame[i] && players[i].isbot)
- 		{
- 			CleanBotstuff (&players[i]);
- 			players[i].isbot = false;
- 			botingame[i] = false;
- 		}
- 	}
- 
  	if (ctf && teamplay == false)
  		teamplay = true; //Need teamplay for ctf. (which is not done yet)
  
--- 147,152 ----
***************
*** 199,205 ****
  
  		while (thebot != NULL)
  		{
! 			thebot->inuse = false;
  			thebot = thebot->next;
  		}
  	}
--- 162,168 ----
  
  		while (thebot != NULL)
  		{
! 			thebot->inuse = BOTINUSE_No;
  			thebot = thebot->next;
  		}
  	}
***************
*** 212,230 ****
  
  	//Arrange wanted botnum and their names, so they can be spawned next level.
  	getspawned.Clear();
! 	for (i = 0; i < MAXPLAYERS; i++)
  	{
! 		if (playeringame[i] && players[i].isbot)
  		{
! 			if (deathmatch)
  			{
  				getspawned.Push(players[i].userinfo.GetName());
  			}
- 			CleanBotstuff (&players[i]);
  		}
! 	}
! 	if (deathmatch)
! 	{
  		wanted_botnum = botnum;
  	}
  }
--- 175,190 ----
  
  	//Arrange wanted botnum and their names, so they can be spawned next level.
  	getspawned.Clear();
! 	if (deathmatch)
  	{
! 		for (i = 0; i < MAXPLAYERS; i++)
  		{
! 			if (players[i].Bot != NULL)
  			{
  				getspawned.Push(players[i].userinfo.GetName());
  			}
  		}
! 
  		wanted_botnum = botnum;
  	}
  }
***************
*** 240,251 ****
  //The color parameter can be either a
  //color (range from 0-10), or = NOCOLOR.
  //The color parameter overides bots
! //induvidual colors if not = NOCOLOR.
  
  bool FCajunMaster::SpawnBot (const char *name, int color)
  {
- 	int playernumber;
- 
  	//COLORS
  	static const char colors[11][17] =
  	{
--- 200,209 ----
  //The color parameter can be either a
  //color (range from 0-10), or = NOCOLOR.
  //The color parameter overides bots
! //individual colors if not = NOCOLOR.
  
  bool FCajunMaster::SpawnBot (const char *name, int color)
  {
  	//COLORS
  	static const char colors[11][17] =
  	{
***************
*** 262,325 ****
  		"\\color\\cf df 90"		//10 = Bleached Bone
  	};
  
! 	for (playernumber = 0; playernumber < MAXPLAYERS; playernumber++)
! 	{
! 		if (!playeringame[playernumber] && !waitingforspawn[playernumber])
! 		{
! 			break;
! 		}
! 	}
! 
! 	if (playernumber == MAXPLAYERS)
! 	{
! 		Printf ("The maximum of %d players/bots has been reached\n", MAXPLAYERS);
! 		return false;
! 	}
! 
! 	botinfo_t *thebot;
  
  	if (name)
  	{
- 		thebot = botinfo;
- 
  		// Check if exist or already in the game.
  		while (thebot && stricmp (name, thebot->name))
  			thebot = thebot->next;
  
  		if (thebot == NULL)
  		{
     		 	Printf ("couldn't find %s in %s\n", name, BOTFILENAME);
  			return false;
  		}
! 		else if (thebot->inuse)
  		{
-    		 	Printf ("%s is already in the thick\n", name);
  			return false;
  		}
! 	}
! 	else if (botnum < loaded_bots)
! 	{
! 		bool vacant = false;  //Spawn a random bot from bots.cfg if no name given.
! 		while (!vacant)
  		{
! 			int rnum = (pr_botspawn() % loaded_bots);
! 			thebot = botinfo;
! 			while (rnum)
! 				--rnum, thebot = thebot->next;
! 			if (!thebot->inuse)
! 				vacant = true;
  		}
  	}
  	else
  	{
! 		Printf ("Couldn't spawn bot; no bot left in %s\n", BOTFILENAME);
! 		return false;
  	}
  
! 	waitingforspawn[playernumber] = true;
  
  	Net_WriteByte (DEM_ADDBOT);
! 	Net_WriteByte (playernumber);
  	{
  		//Set color.
  		char concat[512];
--- 220,288 ----
  		"\\color\\cf df 90"		//10 = Bleached Bone
  	};
  
! 	botinfo_t *thebot = botinfo;
! 	int botshift = 0;
  
  	if (name)
  	{
  		// Check if exist or already in the game.
  		while (thebot && stricmp (name, thebot->name))
+ 		{
+ 			botshift++;
  			thebot = thebot->next;
+ 		}
  
  		if (thebot == NULL)
  		{
     		 	Printf ("couldn't find %s in %s\n", name, BOTFILENAME);
  			return false;
  		}
! 		else if (thebot->inuse == BOTINUSE_Waiting)
  		{
  			return false;
  		}
! 		else if (thebot->inuse == BOTINUSE_Yes)
  		{
!    		 	Printf ("%s is already in the thick\n", name);
! 			return false;
  		}
  	}
  	else
  	{
! 		//Spawn a random bot from bots.cfg if no name given.
! 		TArray<botinfo_t *> BotInfoAvailable;
! 
! 		while (thebot)
! 		{
! 			if (thebot->inuse == BOTINUSE_No)
! 				BotInfoAvailable.Push (thebot);
! 
! 			thebot = thebot->next;
! 		}
! 
! 		if (BotInfoAvailable.Size () == 0)
! 		{
! 			Printf ("Couldn't spawn bot; no bot left in %s\n", BOTFILENAME);
! 			return false;
! 		}
! 
! 		thebot = BotInfoAvailable[pr_botspawn() % BotInfoAvailable.Size ()];
! 
! 		botinfo_t *thebot2 = botinfo;
! 		while (thebot2)
! 		{
! 			if (thebot == thebot2)
! 				break;
! 
! 			botshift++;
! 			thebot2 = thebot2->next;
! 		}
  	}
  
! 	thebot->inuse = BOTINUSE_Waiting;
  
  	Net_WriteByte (DEM_ADDBOT);
! 	Net_WriteByte (botshift);
  	{
  		//Set color.
  		char concat[512];
***************
*** 335,386 ****
  		}
  		Net_WriteString (concat);
  	}
! 
! 	players[playernumber].skill = thebot->skill;
! 
! 	thebot->inuse = true;
! 
! 	//Increment this.
! 	botnum++;
  
  	return true;
  }
  
! void FCajunMaster::DoAddBot (int bnum, char *info)
  {
! 	BYTE *infob = (BYTE *)info;
! 	D_ReadUserInfoStrings (bnum, &infob, false);
! 	if (!deathmatch && playerstarts[bnum].type == 0)
  	{
! 		Printf ("%s tried to join, but there was no player %d start\n",
! 			players[bnum].userinfo.GetName(), bnum+1);
! 		ClearPlayer (bnum, false);	// Make the bot inactive again
! 		if (botnum > 0)
  		{
! 			botnum--;
  		}
  	}
  	else
  	{
! 		multiplayer = true; //Prevents cheating and so on; emulates real netgame (almost).
! 		players[bnum].isbot = true;
! 		playeringame[bnum] = true;
! 		players[bnum].mo = NULL;
! 		players[bnum].playerstate = PST_ENTER;
! 		botingame[bnum] = true;
  
! 		if (teamplay)
! 			Printf ("%s joined the %s team\n", players[bnum].userinfo.GetName(), Teams[players[bnum].userinfo.GetTeam()].GetName());
! 		else
! 			Printf ("%s joined the game\n", players[bnum].userinfo.GetName());
  
! 		G_DoReborn (bnum, true);
! 		if (StatusBar != NULL)
  		{
! 			StatusBar->MultiplayerChanged ();
  		}
  	}
! 	waitingforspawn[bnum] = false;
  }
  
  void FCajunMaster::RemoveAllBots (bool fromlist)
--- 298,403 ----
  		}
  		Net_WriteString (concat);
  	}
! 	Net_WriteByte(thebot->skill.aiming);
! 	Net_WriteByte(thebot->skill.perfection);
! 	Net_WriteByte(thebot->skill.reaction);
! 	Net_WriteByte(thebot->skill.isp);
  
  	return true;
  }
  
! void FCajunMaster::TryAddBot (BYTE **stream, int player)
  {
! 	int botshift = ReadByte (stream);
! 	char *info = ReadString (stream);
! 	botskill_t skill;
! 	skill.aiming = ReadByte (stream);
! 	skill.perfection = ReadByte (stream);
! 	skill.reaction = ReadByte (stream);
! 	skill.isp = ReadByte (stream);
! 
! 	botinfo_t *thebot = NULL;
! 
! 	if (consoleplayer == player)
  	{
! 		thebot = botinfo;
! 
! 		while (botshift > 0)
! 		{
! 			thebot = thebot->next;
! 			botshift--;
! 		}
! 	}
! 
! 	if (DoAddBot ((BYTE *)info, skill))
! 	{
! 		//Increment this.
! 		botnum++;
! 
! 		if (thebot != NULL)
  		{
! 			thebot->inuse = BOTINUSE_Yes;
  		}
  	}
  	else
  	{
! 		if (thebot != NULL)
! 		{
! 			thebot->inuse = BOTINUSE_No;
! 		}
! 	}
  
! 	delete[] info;
! }
! 
! bool FCajunMaster::DoAddBot (BYTE *info, botskill_t skill)
! {
! 	int bnum;
  
! 	for (bnum = 0; bnum < MAXPLAYERS; bnum++)
! 	{
! 		if (!playeringame[bnum])
  		{
! 			break;
  		}
  	}
! 
! 	if (bnum == MAXPLAYERS)
! 	{
! 		Printf ("The maximum of %d players/bots has been reached\n", MAXPLAYERS);
! 		return false;
! 	}
! 
! 	D_ReadUserInfoStrings (bnum, &info, false);
! 
! 	if (!deathmatch && playerstarts[bnum].type == 0)
! 	{
! 		Printf ("%s tried to join, but there was no player %d start\n",
! 			players[bnum].userinfo.GetName(), bnum+1);
! 		ClearPlayer (bnum, false);	// Make the bot inactive again
! 		return false;
! 	}
! 
! 	multiplayer = true; //Prevents cheating and so on; emulates real netgame (almost).
! 	players[bnum].Bot = new DBot;
! 	players[bnum].Bot->player = &players[bnum];
! 	players[bnum].Bot->skill = skill;
! 	playeringame[bnum] = true;
! 	players[bnum].mo = NULL;
! 	players[bnum].playerstate = PST_ENTER;
! 
! 	if (teamplay)
! 		Printf ("%s joined the %s team\n", players[bnum].userinfo.GetName(), Teams[players[bnum].userinfo.GetTeam()].GetName());
! 	else
! 		Printf ("%s joined the game\n", players[bnum].userinfo.GetName());
! 
! 	G_DoReborn (bnum, true);
! 	if (StatusBar != NULL)
! 	{
! 		StatusBar->MultiplayerChanged ();
! 	}
! 
! 	return true;
  }
  
  void FCajunMaster::RemoveAllBots (bool fromlist)
***************
*** 389,401 ****
  
  	for (i = 0; i < MAXPLAYERS; ++i)
  	{
! 		if (playeringame[i] && botingame[i])
  		{
  			// If a player is looking through this bot's eyes, make him
  			// look through his own eyes instead.
  			for (j = 0; j < MAXPLAYERS; ++j)
  			{
! 				if (i != j && playeringame[j] && !botingame[j])
  				{
  					if (players[j].camera == players[i].mo)
  					{
--- 406,418 ----
  
  	for (i = 0; i < MAXPLAYERS; ++i)
  	{
! 		if (players[i].Bot != NULL)
  		{
  			// If a player is looking through this bot's eyes, make him
  			// look through his own eyes instead.
  			for (j = 0; j < MAXPLAYERS; ++j)
  			{
! 				if (i != j && playeringame[j] && players[j].Bot == NULL)
  				{
  					if (players[j].camera == players[i].mo)
  					{
***************
*** 415,448 ****
  	if (fromlist)
  	{
  		wanted_botnum = 0;
- 		for (i = 0; i < MAXPLAYERS; i++)
- 			waitingforspawn[i] = false;
  	}
  	botnum = 0;
  }
  
- //Clean the bot part of the player_t
- //Used when bots are respawned or at level starts.
- void FCajunMaster::CleanBotstuff (player_t *p)
- {
- 	p->angle = ANG45;
- 	p->dest = NULL;
- 	p->enemy = NULL; //The dead meat.
- 	p->missile = NULL; //A threatening missile that needs to be avoided.
- 	p->mate = NULL;    //Friend (used for grouping in templay or coop.
- 	p->last_mate = NULL; //If bot's mate dissapeared (not if died) that mate is pointed to by this. Allows bot to roam to it if necessary.
- 	//Tickers
- 	p->t_active = 0; //Open door, lower lift stuff, door must open and lift must go down before bot does anything radical like try a stuckmove
- 	p->t_respawn = 0;
- 	p->t_strafe = 0;
- 	p->t_react = 0;
- 	//Misc bools
- 	p->isbot = true; //Important.
- 	p->first_shot = true; //Used for reaction skill.
- 	p->sleft = false; //If false, strafe is right.
- 	p->allround = false;
- }
- 
  
  //------------------
  //Reads data for bot from
--- 432,441 ----
***************
*** 495,501 ****
  	}
  
  	botinfo = NULL;
- 	loaded_bots = 0;
  }
  
  bool FCajunMaster::LoadBots ()
--- 488,493 ----
***************
*** 503,509 ****
  	FScanner sc;
  	FString tmp;
  	bool gotteam = false;
! 
  	bglobal.ForgetBots ();
  	tmp = M_GetCajunPath(BOTFILENAME);
  	if (tmp.IsEmpty())
--- 495,502 ----
  	FScanner sc;
  	FString tmp;
  	bool gotteam = false;
! 	int loaded_bots = 0;
! LOGI("LoadBots 1");
  	bglobal.ForgetBots ();
  	tmp = M_GetCajunPath(BOTFILENAME);
  	if (tmp.IsEmpty())
***************
*** 512,518 ****
  		return false;
  	}
  	sc.OpenFile(tmp);
! 
  	while (sc.GetString ())
  	{
  		if (!sc.Compare ("{"))
--- 505,511 ----
  		return false;
  	}
  	sc.OpenFile(tmp);
! 	LOGI("LoadBots 2");
  	while (sc.GetString ())
  	{
  		if (!sc.Compare ("{"))
***************
*** 619,627 ****
  		newinfo->next = bglobal.botinfo;
  		newinfo->lastteam = TEAM_NONE;
  		bglobal.botinfo = newinfo;
! 		bglobal.loaded_bots++;
  	}
! 	Printf ("%d bots read from %s\n", bglobal.loaded_bots, BOTFILENAME);
  	return true;
  }
  
--- 612,620 ----
  		newinfo->next = bglobal.botinfo;
  		newinfo->lastteam = TEAM_NONE;
  		bglobal.botinfo = newinfo;
! 		loaded_bots++;
  	}
! 	Printf ("%d bots read from %s\n", loaded_bots, BOTFILENAME);
  	return true;
  }
  
