*** /home/draje/Code/gzdoom/src/d_net.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/d_net.cpp	2017-06-18 10:53:19.843127453 -0400
***************
*** 39,45 ****
  #include "cmdlib.h"
  #include "s_sound.h"
  #include "m_cheat.h"
- #include "p_effect.h"
  #include "p_local.h"
  #include "c_dispatch.h"
  #include "sbar.h"
--- 39,44 ----
***************
*** 110,122 ****
  unsigned int	currrecvtime[MAXPLAYERS];
  unsigned int	lastglobalrecvtime;						// Identify the last time a packet was recieved.
  bool			hadlate;
  
  int 			nodeforplayer[MAXPLAYERS];
  int				playerfornode[MAXNETNODES];
  
  int 			maketic;
  int 			skiptics;
! int 			ticdup; 		
  
  void D_ProcessEvents (void); 
  void G_BuildTiccmd (ticcmd_t *cmd); 
--- 109,123 ----
  unsigned int	currrecvtime[MAXPLAYERS];
  unsigned int	lastglobalrecvtime;						// Identify the last time a packet was recieved.
  bool			hadlate;
+ int				netdelay[MAXNETNODES][BACKUPTICS];		// Used for storing network delay times.
+ int				lastaverage;
  
  int 			nodeforplayer[MAXPLAYERS];
  int				playerfornode[MAXNETNODES];
  
  int 			maketic;
  int 			skiptics;
! int 			ticdup;
  
  void D_ProcessEvents (void); 
  void G_BuildTiccmd (ticcmd_t *cmd); 
***************
*** 151,156 ****
--- 152,183 ----
  	}
  }
  
+ CVAR(Bool, net_ticbalance, false, CVAR_SERVERINFO | CVAR_NOSAVE)
+ CUSTOM_CVAR(Int, net_extratic, 0, CVAR_SERVERINFO | CVAR_NOSAVE)
+ {
+ 	if (self < 0)
+ 	{
+ 		self = 0;
+ 	}
+ 	else if (self > 2)
+ 	{
+ 		self = 2;
+ 	}
+ }
+ 
+ #ifdef _DEBUG
+ CVAR(Int, net_fakelatency, 0, 0);
+ 
+ struct PacketStore
+ {
+ 	int timer;
+ 	doomcom_t message;
+ };
+ 
+ static TArray<PacketStore> InBuffer;
+ static TArray<PacketStore> OutBuffer;
+ #endif
+ 
  // [RH] Special "ticcmds" get stored in here
  static struct TicSpecial
  {
***************
*** 158,164 ****
  	size_t used[BACKUPTICS];
  	BYTE *streamptr;
  	size_t streamoffs;
! 	int   specialsize;
  	int	  lastmaketic;
  	bool  okay;
  
--- 185,191 ----
  	size_t used[BACKUPTICS];
  	BYTE *streamptr;
  	size_t streamoffs;
! 	size_t specialsize;
  	int	  lastmaketic;
  	bool  okay;
  
***************
*** 197,207 ****
  	}
  
  	// Make more room for special commands.
! 	void GetMoreSpace ()
  	{
  		int i;
  
! 		specialsize <<= 1;
  
  		DPrintf ("Expanding special size to %d\n", specialsize);
  
--- 224,234 ----
  	}
  
  	// Make more room for special commands.
! 	void GetMoreSpace (size_t needed)
  	{
  		int i;
  
! 		specialsize = MAX(specialsize * 2, needed + 30);
  
  		DPrintf ("Expanding special size to %d\n", specialsize);
  
***************
*** 213,220 ****
  
  	void CheckSpace (size_t needed)
  	{
! 		if (streamoffs >= specialsize - needed)
! 			GetMoreSpace ();
  
  		streamoffs += needed;
  	}
--- 240,247 ----
  
  	void CheckSpace (size_t needed)
  	{
! 		if (streamoffs + needed >= specialsize)
! 			GetMoreSpace (streamoffs + needed);
  
  		streamoffs += needed;
  	}
***************
*** 347,352 ****
--- 374,382 ----
  		k += netbuffer[k] + 1;
  	}
  
+ 	// Network delay byte
+ 	k++;
+ 
  	if (netbuffer[0] & NCMD_MULTI)
  	{
  		count = netbuffer[k];
***************
*** 487,493 ****
  	doomcom.remotenode = node;
  	doomcom.datalength = len;
  
! 	I_NetCmd ();
  }
  
  //
--- 517,546 ----
  	doomcom.remotenode = node;
  	doomcom.datalength = len;
  
! #ifdef _DEBUG
! 	if (net_fakelatency / 2 > 0)
! 	{
! 		PacketStore store;
! 		store.message = doomcom;
! 		store.timer = I_GetTime(false) + ((net_fakelatency / 2) / (1000 / TICRATE));
! 		OutBuffer.Push(store);
! 	}
! 	else
! 		I_NetCmd();
! 
! 	for (unsigned int i = 0; i < OutBuffer.Size(); i++)
! 	{
! 		if (OutBuffer[i].timer <= I_GetTime(false))
! 		{
! 			doomcom = OutBuffer[i].message;
! 			I_NetCmd();
! 			OutBuffer.Delete(i);
! 			i = -1;
! 		}
! 	}
! #else
! 	I_NetCmd();
! #endif
  }
  
  //
***************
*** 509,520 ****
  
  	if (demoplayback)
  		return false;
! 				
  	doomcom.command = CMD_GET;
  	I_NetCmd ();
  	
  	if (doomcom.remotenode == -1)
  		return false;
  		
  	if (debugfile)
  	{
--- 562,603 ----
  
  	if (demoplayback)
  		return false;
! 
  	doomcom.command = CMD_GET;
  	I_NetCmd ();
+ 
+ #ifdef _DEBUG
+ 	if (net_fakelatency / 2 > 0 && doomcom.remotenode != -1)
+ 	{
+ 		PacketStore store;
+ 		store.message = doomcom;
+ 		store.timer = I_GetTime(false) + ((net_fakelatency / 2) / (1000 / TICRATE));
+ 		InBuffer.Push(store);
+ 		doomcom.remotenode = -1;
+ 	}
  	
  	if (doomcom.remotenode == -1)
+ 	{
+ 		bool gotmessage = false;
+ 		for (unsigned int i = 0; i < InBuffer.Size(); i++)
+ 		{
+ 			if (InBuffer[i].timer <= I_GetTime(false))
+ 			{
+ 				doomcom = InBuffer[i].message;
+ 				InBuffer.Delete(i);
+ 				gotmessage = true;
+ 				break;
+ 			}
+ 		}
+ 		if (!gotmessage)
+ 			return false;
+ 	}
+ #else
+ 	if (doomcom.remotenode == -1)
+ 	{
  		return false;
+ 	}
+ #endif
  		
  	if (debugfile)
  	{
***************
*** 570,575 ****
--- 653,661 ----
  
  	if (doomcom.datalength != NetbufferSize ())
  	{
+ 		Printf("Bad packet length %i (calculated %i)\n",
+ 			doomcom.datalength, NetbufferSize());
+ 
  		if (debugfile)
  			fprintf (debugfile,"---bad packet length %i (calculated %i)\n",
  				doomcom.datalength, NetbufferSize());
***************
*** 583,669 ****
  {
  	int i;
  
! 	for (i = netnode + 1; i < doomcom.numnodes; ++i)
! 	{
! 		if (nodeingame[i])
! 			break;
! 	}
! 	if (i == doomcom.numnodes)
! 	{
! 		doomcom.numnodes = netnode;
! 	}
! 
! 	nodeingame[netnode] = false;
! 	playeringame[netconsole] = false;
! 	nodejustleft[netnode] = false;
! 
! 	if (deathmatch)
! 	{
! 		Printf ("%s left the game with %d frags\n",
! 			players[netconsole].userinfo.GetName(),
! 			players[netconsole].fragcount);
! 	}
! 	else
  	{
! 		Printf ("%s left the game\n", players[netconsole].userinfo.GetName());
! 	}
  
! 	// [RH] Revert each player to their own view if spying through the player who left
! 	for (int ii = 0; ii < MAXPLAYERS; ++ii)
! 	{
! 		if (playeringame[ii] && players[ii].camera == players[netconsole].mo)
  		{
! 			players[ii].camera = players[ii].mo;
! 			if (ii == consoleplayer && StatusBar != NULL)
! 			{
! 				StatusBar->AttachToPlayer (&players[ii]);
! 			}
  		}
  	}
! 
! 	// [RH] Make the player disappear
! 	FBehavior::StaticStopMyScripts (players[netconsole].mo);
! 	if (players[netconsole].mo != NULL)
  	{
! 		P_DisconnectEffect (players[netconsole].mo);
! 		players[netconsole].mo->player = NULL;
! 		players[netconsole].mo->Destroy ();
! 		if (!(players[netconsole].mo->ObjectFlags & OF_EuthanizeMe))
! 		{ // We just destroyed a morphed player, so now the original player
! 		  // has taken their place. Destroy that one too.
! 			players[netconsole].mo->Destroy();
  		}
! 		players[netconsole].mo = NULL;
! 		players[netconsole].camera = NULL;
  	}
! 	// [RH] Let the scripts know the player left
! 	FBehavior::StaticStartTypedScripts (SCRIPT_Disconnect, NULL, true, netconsole);
  	if (netconsole == Net_Arbitrator)
  	{
- 		bglobal.RemoveAllBots (true);
- 		Printf ("Removed all bots\n");
- 
  		// Pick a new network arbitrator
  		for (int i = 0; i < MAXPLAYERS; i++)
  		{
! 			if (playeringame[i] && !players[i].isbot)
  			{
  				Net_Arbitrator = i;
  				players[i].settings_controller = true;
! 				Printf ("%s is the new arbitrator\n", players[i].userinfo.GetName());
  				break;
  			}
  		}
! 		if (debugfile && NetMode == NET_PacketServer)
  		{
! 			if (Net_Arbitrator == consoleplayer)
! 			{
! 				fprintf (debugfile, "I am the new master!\n");
! 			}
! 			else
! 			{
! 				fprintf (debugfile, "Node %d is the new master!\n", nodeforplayer[Net_Arbitrator]);
! 			}
  		}
  	}
  
--- 669,731 ----
  {
  	int i;
  
! 	if (nodeingame[netnode])
  	{
! 		for (i = netnode + 1; i < doomcom.numnodes; ++i)
! 		{
! 			if (nodeingame[i])
! 				break;
! 		}
! 		if (i == doomcom.numnodes)
! 		{
! 			doomcom.numnodes = netnode;
! 		}
  
! 		if (playeringame[netconsole])
  		{
! 			players[netconsole].playerstate = PST_GONE;
  		}
+ 		nodeingame[netnode] = false;
+ 		nodejustleft[netnode] = false;
  	}
! 	else if (nodejustleft[netnode]) // Packet Server
  	{
! 		if (netnode + 1 == doomcom.numnodes)
! 		{
! 			doomcom.numnodes = netnode;
  		}
! 		if (playeringame[netconsole])
! 		{
! 			players[netconsole].playerstate = PST_GONE;
! 		}
! 		nodejustleft[netnode] = false;
  	}
! 	else return;
! 
  	if (netconsole == Net_Arbitrator)
  	{
  		// Pick a new network arbitrator
  		for (int i = 0; i < MAXPLAYERS; i++)
  		{
! 			if (i != netconsole && playeringame[i] && players[i].Bot == NULL)
  			{
  				Net_Arbitrator = i;
  				players[i].settings_controller = true;
! 				Printf("%s is the new arbitrator\n", players[i].userinfo.GetName());
  				break;
  			}
  		}
! 	}
! 
! 	if (debugfile && NetMode == NET_PacketServer)
! 	{
! 		if (Net_Arbitrator == consoleplayer)
  		{
! 			fprintf(debugfile, "I am the new master!\n");
! 		}
! 		else
! 		{
! 			fprintf(debugfile, "Node %d is the new master!\n", nodeforplayer[Net_Arbitrator]);
  		}
  	}
  
***************
*** 741,747 ****
  			else
  			{
  				nodeingame[netnode] = false;
- 				playeringame[netconsole] = false;
  				nodejustleft[netnode] = true;
  			}
  			continue;
--- 803,808 ----
***************
*** 772,778 ****
  		}
  
  		if (netbuffer[0] & NCMD_QUITTERS)
- 
  		{
  			numplayers = netbuffer[k++];
  			for (int i = 0; i < numplayers; ++i)
--- 833,838 ----
***************
*** 782,787 ****
--- 842,850 ----
  			}
  		}
  
+ 		// Pull current network delay from node
+ 		netdelay[netnode][(nettics[netnode]+1) % BACKUPTICS] = netbuffer[k++];
+ 
  		playerbytes[0] = netconsole;
  		if (netbuffer[0] & NCMD_MULTI)
  		{
***************
*** 847,910 ****
  
  			for (i = 0; i < numplayers; ++i)
  			{
! 				int node = !players[playerbytes[i]].isbot ?
! 					nodeforplayer[playerbytes[i]] : netnode;
  
  				SkipTicCmd (&start, nettics[node] - realstart);
  				for (tics = nettics[node]; tics < realend; tics++)
  					ReadTicCmd (&start, playerbytes[i], tics);
- 			}
- 			// Update the number of tics received from each node. This must
- 			// be separate from the above loop in case the master is also
- 			// sending bot movements. If it's not separate, then the bots
- 			// will only move on the master, because the other players will
- 			// read the master's tics and then think they already got all
- 			// the tics for the bots and skip the bot tics included in the
- 			// packet.
- 			for (i = 0; i < numplayers; ++i)
- 			{
- 				if (!players[playerbytes[i]].isbot)
- 				{
- 					nettics[nodeforplayer[playerbytes[i]]] = realend;
- 				}
- 			}
- 		}
- 	}
- }
  
! void AdjustBots (int gameticdiv)
! {
! 	// [RH] This loop adjusts the bots' rotations for ticcmds that have
! 	// been already created but not yet executed. This way, the bot is still
! 	// able to create ticcmds that accurately reflect the state it wants to
! 	// be in even when gametic lags behind maketic.
! 	for (int i = 0; i < MAXPLAYERS; i++)
! 	{
! 		if (playeringame[i] && players[i].isbot && players[i].mo)
! 		{
! 			players[i].savedyaw = players[i].mo->angle;
! 			players[i].savedpitch = players[i].mo->pitch;
! 			for (int j = gameticdiv; j < maketic/ticdup; j++)
! 			{
! 				players[i].mo->angle += (netcmds[i][j%BACKUPTICS].ucmd.yaw << 16) * ticdup;
! 				players[i].mo->pitch -= (netcmds[i][j%BACKUPTICS].ucmd.pitch << 16) * ticdup;
  			}
  		}
  	}
  }
  
- void UnadjustBots ()
- {
- 	for (int i = 0; i < MAXPLAYERS; i++)
- 	{
- 		if (playeringame[i] && players[i].isbot && players[i].mo)
- 		{
- 			players[i].mo->angle = players[i].savedyaw;
- 			players[i].mo->pitch = players[i].savedpitch;
- 		}
- 	}
- }
- 
  //
  // NetUpdate
  // Builds ticcmds for console player,
--- 910,927 ----
  
  			for (i = 0; i < numplayers; ++i)
  			{
! 				int node = nodeforplayer[playerbytes[i]];
  
  				SkipTicCmd (&start, nettics[node] - realstart);
  				for (tics = nettics[node]; tics < realend; tics++)
  					ReadTicCmd (&start, playerbytes[i], tics);
  
! 				nettics[nodeforplayer[playerbytes[i]]] = realend;
  			}
  		}
  	}
  }
  
  //
  // NetUpdate
  // Builds ticcmds for console player,
***************
*** 934,940 ****
  	newtics = nowtime - gametime;
  	gametime = nowtime;
  
! 	if (newtics <= 0)	// nothing new to update
  	{
  		GetPackets ();
  		return;
--- 951,957 ----
  	newtics = nowtime - gametime;
  	gametime = nowtime;
  
! 	if (newtics <= 0 || pauseext)	// nothing new to update or window paused
  	{
  		GetPackets ();
  		return;
***************
*** 951,959 ****
  		newtics = 0;
  	}
  
! 	// build new ticcmds for console player (and bots if I am the arbitrator)
! 	AdjustBots (gametic / ticdup);
! 
  	for (i = 0; i < newtics; i++)
  	{
  		I_StartTic ();
--- 968,974 ----
  		newtics = 0;
  	}
  
! 	// build new ticcmds for console player
  	for (i = 0; i < newtics; i++)
  	{
  		I_StartTic ();
***************
*** 963,973 ****
  		
  		//Printf ("mk:%i ",maketic);
  		G_BuildTiccmd (&localcmds[maketic % LOCALCMDTICS]);
- 		if (maketic % ticdup == 0)
- 		{
- 			//Added by MC: For some of that bot stuff. The main bot function.
- 			bglobal.Main ((maketic / ticdup) % BACKUPTICS);
- 		}
  		maketic++;
  
  		if (ticdup == 1 || maketic == 0)
--- 978,983 ----
***************
*** 1047,1054 ****
  		}
  	}
  
- 	UnadjustBots ();
- 
  	if (singletics)
  		return; 		// singletic update is synchronous
  
--- 1057,1062 ----
***************
*** 1068,1086 ****
  
  	if (consoleplayer == Net_Arbitrator)
  	{
! 		for (j = 0; j < MAXPLAYERS; j++)
  		{
! 			if (playeringame[j])
  			{
! 				if (players[j].isbot || NetMode == NET_PacketServer)
  				{
  					count++;
  				}
  			}
- 		}
  
- 		if (NetMode == NET_PacketServer)
- 		{
  			// The loop above added the local player to the count a second time,
  			// and it also added the player being sent the packet to the count.
  			count -= 2;
--- 1076,1091 ----
  
  	if (consoleplayer == Net_Arbitrator)
  	{
! 		if (NetMode == NET_PacketServer)
  		{
! 			for (j = 0; j < MAXPLAYERS; j++)
  			{
! 				if (playeringame[j] && players[j].Bot == NULL)
  				{
  					count++;
  				}
  			}
  
  			// The loop above added the local player to the count a second time,
  			// and it also added the player being sent the packet to the count.
  			count -= 2;
***************
*** 1150,1160 ****
  			netbuffer[k++] = lowtic;
  		}
  
! 		numtics = lowtic - realstart;
  		if (numtics > BACKUPTICS)
  			I_Error ("NetUpdate: Node %d missed too many tics", i);
  
! 		resendto[i] = MAX (0, lowtic - doomcom.extratics);
  
  		if (numtics == 0 && resendOnly && !remoteresend[i] && nettics[i])
  		{
--- 1155,1172 ----
  			netbuffer[k++] = lowtic;
  		}
  
! 		numtics = MAX(0, lowtic - realstart);
  		if (numtics > BACKUPTICS)
  			I_Error ("NetUpdate: Node %d missed too many tics", i);
  
! 		switch (net_extratic)
! 		{
! 		case 0:
! 		default: 
! 			resendto[i] = lowtic; break;
! 		case 1: resendto[i] = MAX(0, lowtic - 1); break;
! 		case 2: resendto[i] = nettics[i]; break;
! 		}
  
  		if (numtics == 0 && resendOnly && !remoteresend[i] && nettics[i])
  		{
***************
*** 1190,1195 ****
--- 1202,1211 ----
  			}
  		}
  
+ 		// Send current network delay
+ 		// The number of tics we just made should be removed from the count.
+ 		netbuffer[k++] = ((maketic - newtics - gametic) / ticdup);
+ 
  		if (numtics > 0)
  		{
  			int l;
***************
*** 1199,1209 ****
  				netbuffer[0] |= NCMD_MULTI;
  				netbuffer[k++] = count;
  
! 				for (l = 1, j = 0; j < MAXPLAYERS; j++)
  				{
! 					if (playeringame[j] && j != playerfornode[i] && j != consoleplayer)
  					{
! 						if (players[j].isbot || NetMode == NET_PacketServer)
  						{
  							playerbytes[l++] = j;
  							netbuffer[k++] = j;
--- 1215,1225 ----
  				netbuffer[0] |= NCMD_MULTI;
  				netbuffer[k++] = count;
  
! 				if (NetMode == NET_PacketServer)
  				{
! 					for (l = 1, j = 0; j < MAXPLAYERS; j++)
  					{
! 						if (playeringame[j] && players[j].Bot == NULL && j != playerfornode[i] && j != consoleplayer)
  						{
  							playerbytes[l++] = j;
  							netbuffer[k++] = j;
***************
*** 1227,1233 ****
  					prev %= BACKUPTICS;
  
  					// The local player has their tics sent first, followed by
! 					// the other players/bots.
  					if (l == 0)
  					{
  						WriteWord (localcmds[localstart].consistancy, &cmddata);
--- 1243,1249 ----
  					prev %= BACKUPTICS;
  
  					// The local player has their tics sent first, followed by
! 					// the other players.
  					if (l == 0)
  					{
  						WriteWord (localcmds[localstart].consistancy, &cmddata);
***************
*** 1242,1265 ****
  					}
  					else if (i != 0)
  					{
! 						if (players[playerbytes[l]].isbot)
! 						{
  
! 							WriteWord (0, &cmddata);	// fake consistancy word
! 						}
! 						else
  						{
! 							int len;
! 							BYTE *spec;
! 
! 							WriteWord (netcmds[playerbytes[l]][start].consistancy, &cmddata);
! 							spec = NetSpecs[playerbytes[l]][start].GetData (&len);
! 							if (spec != NULL)
! 							{
! 								memcpy (cmddata, spec, len);
! 								cmddata += len;
! 							}
  						}
  						WriteUserCmdMessage (&netcmds[playerbytes[l]][start].ucmd,
  							prev >= 0 ? &netcmds[playerbytes[l]][prev].ucmd : NULL, &cmddata);
  					}
--- 1258,1274 ----
  					}
  					else if (i != 0)
  					{
! 						int len;
! 						BYTE *spec;
  
! 						WriteWord (netcmds[playerbytes[l]][start].consistancy, &cmddata);
! 						spec = NetSpecs[playerbytes[l]][start].GetData (&len);
! 						if (spec != NULL)
  						{
! 							memcpy (cmddata, spec, len);
! 							cmddata += len;
  						}
+ 
  						WriteUserCmdMessage (&netcmds[playerbytes[l]][start].ucmd,
  							prev >= 0 ? &netcmds[playerbytes[l]][prev].ucmd : NULL, &cmddata);
  					}
***************
*** 1299,1307 ****
  			// that it won't adapt. Fortunately, player prediction helps
  			// alleviate the lag somewhat.
  
! 			if (NetMode != NET_PacketServer)
  			{
! 				mastertics = nettics[nodeforplayer[Net_Arbitrator]];
  			}
  			if (nettics[0] <= mastertics)
  			{
--- 1308,1344 ----
  			// that it won't adapt. Fortunately, player prediction helps
  			// alleviate the lag somewhat.
  
! 			if (NetMode == NET_PeerToPeer)
  			{
! 				int totalavg = 0;
! 				if (net_ticbalance)
! 				{
! 					// Try to guess ahead the time it takes to send responses to the slowest node
! 					int nodeavg = 0, arbavg = 0;
! 
! 					for (j = 0; j < BACKUPTICS; j++)
! 					{
! 						arbavg += netdelay[nodeforplayer[Net_Arbitrator]][j];
! 						nodeavg += netdelay[0][j];
! 					}
! 					arbavg /= BACKUPTICS;
! 					nodeavg /= BACKUPTICS;
! 
! 					// We shouldn't adapt if we are already the arbitrator isn't what we are waiting for, otherwise it just adds more latency
! 					if (arbavg > nodeavg)
! 					{
! 						lastaverage = totalavg = ((arbavg + nodeavg) / 2);
! 					}
! 					else
! 					{
! 						// Allow room to guess two tics ahead
! 						if (nodeavg > (arbavg + 2) && lastaverage > 0)
! 							lastaverage--;
! 						totalavg = lastaverage;
! 					}
! 				}
! 					
! 				mastertics = nettics[nodeforplayer[Net_Arbitrator]] + totalavg;
  			}
  			if (nettics[0] <= mastertics)
  			{
***************
*** 1346,1354 ****
  //
  //  0 One byte set to NCMD_SETUP+2
  //  1 One byte for ticdup setting
! //  2 One byte for extratics setting
! //  3 One byte for NetMode setting
! //  4 String with starting map's name
  //  . Four bytes for the RNG seed
  //  . Stream containing remaining game info
  //
--- 1383,1390 ----
  //
  //  0 One byte set to NCMD_SETUP+2
  //  1 One byte for ticdup setting
! //  2 One byte for NetMode setting
! //  3 String with starting map's name
  //  . Four bytes for the RNG seed
  //  . Stream containing remaining game info
  //
***************
*** 1429,1438 ****
  			data->gotsetup[0] = 0x80;
  
  			ticdup = doomcom.ticdup = netbuffer[1];
! 			doomcom.extratics = netbuffer[2];
! 			NetMode = netbuffer[3];
  
! 			stream = &netbuffer[4];
  			s = ReadString (&stream);
  			startmap = s;
  			delete[] s;
--- 1465,1473 ----
  			data->gotsetup[0] = 0x80;
  
  			ticdup = doomcom.ticdup = netbuffer[1];
! 			NetMode = netbuffer[2];
  
! 			stream = &netbuffer[3];
  			s = ReadString (&stream);
  			startmap = s;
  			delete[] s;
***************
*** 1497,1505 ****
  	{
  		netbuffer[0] = NCMD_SETUP+2;
  		netbuffer[1] = (BYTE)doomcom.ticdup;
! 		netbuffer[2] = (BYTE)doomcom.extratics;
! 		netbuffer[3] = NetMode;
! 		stream = &netbuffer[4];
  		WriteString (startmap, &stream);
  		WriteLong (rngseed, &stream);
  		C_WriteCVars (&stream, CVAR_SERVERINFO, true);
--- 1532,1539 ----
  	{
  		netbuffer[0] = NCMD_SETUP+2;
  		netbuffer[1] = (BYTE)doomcom.ticdup;
! 		netbuffer[2] = NetMode;
! 		stream = &netbuffer[3];
  		WriteString (startmap, &stream);
  		WriteLong (rngseed, &stream);
  		C_WriteCVars (&stream, CVAR_SERVERINFO, true);
***************
*** 1634,1643 ****
  		resendto[i] = 0;				// which tic to start sending
  	}
  
  	// I_InitNetwork sets doomcom and netgame
  	if (I_InitNetwork ())
  	{
! 		NetMode = NET_PacketServer;
  	}
  	if (doomcom.id != DOOMCOM_ID)
  	{
--- 1668,1686 ----
  		resendto[i] = 0;				// which tic to start sending
  	}
  
+ 	// Packet server has proven to be rather slow over the internet. Print a warning about it.
+ 	v = Args->CheckValue("-netmode");
+ 	if (v != NULL && (atoi(v) != 0))
+ 	{
+ 		Printf(TEXTCOLOR_YELLOW "Notice: Using PacketServer (netmode 1) over the internet is prone to running too slow on some internet configurations."
+ 			"\nIf the game is running well below expected speeds, use netmode 0 (P2P) instead.\n");
+ 	}
+ 
  	// I_InitNetwork sets doomcom and netgame
  	if (I_InitNetwork ())
  	{
! 		// For now, stop auto selecting PacketServer, as it's more likely to cause confusion.
! 		//NetMode = NET_PacketServer;
  	}
  	if (doomcom.id != DOOMCOM_ID)
  	{
***************
*** 1647,1661 ****
  
  	consoleplayer = doomcom.consoleplayer;
  
! 	v = Args->CheckValue ("-netmode");
! 	if (v != NULL)
! 	{
! 		NetMode = atoi (v) != 0 ? NET_PacketServer : NET_PeerToPeer;
! 	}
! 	if (doomcom.numnodes > 1)
  	{
! 		Printf ("Selected " TEXTCOLOR_BLUE "%s" TEXTCOLOR_NORMAL " networking mode. (%s)\n", NetMode == NET_PeerToPeer ? "peer to peer" : "packet server",
! 			v != NULL ? "forced" : "auto");
  	}
  
  	// [RH] Setup user info
--- 1690,1712 ----
  
  	consoleplayer = doomcom.consoleplayer;
  
! 	if (consoleplayer == Net_Arbitrator)
  	{
! 		v = Args->CheckValue("-netmode");
! 		if (v != NULL)
! 		{
! 			NetMode = atoi(v) != 0 ? NET_PacketServer : NET_PeerToPeer;
! 		}
! 		if (doomcom.numnodes > 1)
! 		{
! 			Printf("Selected " TEXTCOLOR_BLUE "%s" TEXTCOLOR_NORMAL " networking mode. (%s)\n", NetMode == NET_PeerToPeer ? "peer to peer" : "packet server",
! 				v != NULL ? "forced" : "auto");
! 		}
! 
! 		if (Args->CheckParm("-extratic"))
! 		{
! 			net_extratic = 1;
! 		}
  	}
  
  	// [RH] Setup user info
***************
*** 1683,1688 ****
--- 1734,1744 ----
  	for (i = 0; i < doomcom.numnodes; i++)
  		nodeingame[i] = true;
  
+ 	if (consoleplayer != Net_Arbitrator && doomcom.numnodes > 1)
+ 	{
+ 		Printf("Arbitrator selected " TEXTCOLOR_BLUE "%s" TEXTCOLOR_NORMAL " networking mode.\n", NetMode == NET_PeerToPeer ? "peer to peer" : "packet server");
+ 	}
+ 
  	Printf ("player %i of %i (%i nodes)\n",
  			consoleplayer+1, doomcom.numplayers, doomcom.numnodes);
  }
***************
*** 1809,1814 ****
--- 1865,1873 ----
  		{
  			C_Ticker();
  			M_Ticker();
+ 			// Repredict the player for new buffered movement
+ 			P_UnPredictPlayer();
+ 			P_PredictPlayer(&players[consoleplayer]);
  		}
  		return;
  	}
***************
*** 1844,1849 ****
--- 1903,1911 ----
  		{
  			C_Ticker ();
  			M_Ticker ();
+ 			// Repredict the player for new buffered movement
+ 			P_UnPredictPlayer();
+ 			P_PredictPlayer(&players[consoleplayer]);
  			return;
  		}
  	}
***************
*** 1857,1862 ****
--- 1919,1925 ----
  	// run the count tics
  	if (counts > 0)
  	{
+ 		P_UnPredictPlayer();
  		while (counts--)
  		{
  			if (gametic > lowtic)
***************
*** 1876,1881 ****
--- 1939,1945 ----
  
  			NetUpdate ();	// check for new console commands
  		}
+ 		P_PredictPlayer(&players[consoleplayer]);
  		S_UpdateSounds (players[consoleplayer].camera);	// move positional sounds
  	}
  }
***************
*** 2130,2139 ****
  		break;
  
  	case DEM_ADDBOT:
! 		{
! 			BYTE num = ReadByte (stream);
! 			bglobal.DoAddBot (num, s = ReadString (stream));
! 		}
  		break;
  
  	case DEM_KILLBOTS:
--- 2194,2200 ----
  		break;
  
  	case DEM_ADDBOT:
! 		bglobal.TryAddBot (stream, player);
  		break;
  
  	case DEM_KILLBOTS:
***************
*** 2589,2598 ****
  	switch (type)
  	{
  		case DEM_SAY:
- 		case DEM_ADDBOT:
  			skip = strlen ((char *)(*stream + 1)) + 2;
  			break;
  
  		case DEM_GIVECHEAT:
  		case DEM_TAKECHEAT:
  			skip = strlen ((char *)(*stream)) + 3;
--- 2650,2662 ----
  	switch (type)
  	{
  		case DEM_SAY:
  			skip = strlen ((char *)(*stream + 1)) + 2;
  			break;
  
+ 		case DEM_ADDBOT:
+ 			skip = strlen ((char *)(*stream + 1)) + 6;
+ 			break;
+ 
  		case DEM_GIVECHEAT:
  		case DEM_TAKECHEAT:
  			skip = strlen ((char *)(*stream)) + 3;
***************
*** 2713,2719 ****
  CCMD (pings)
  {
  	int i;
- 
  	for (i = 0; i < MAXPLAYERS; i++)
  		if (playeringame[i])
  			Printf ("% 4d %s\n", currrecvtime[i] - lastrecvtime[i],
--- 2777,2782 ----
***************
*** 2755,2761 ****
  		return;
  	}
  
! 	if (players[playernum].isbot)
  	{
  		Printf ("Bots cannot be added to the controller list.\n");
  		return;
--- 2818,2824 ----
  		return;
  	}
  
! 	if (players[playernum].Bot != NULL)
  	{
  		Printf ("Bots cannot be added to the controller list.\n");
  		return;
