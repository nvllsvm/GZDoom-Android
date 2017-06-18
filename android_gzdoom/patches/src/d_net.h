*** /home/draje/Code/gzdoom/src/d_net.h	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/d_net.h	2017-06-18 09:09:24.161730580 -0400
***************
*** 70,76 ****
  // info common to all nodes
  	SWORD	numnodes;		// console is always node 0.
  	SWORD	ticdup;			// 1 = no duplication, 2-5 = dup for slow nets
- 	SWORD	extratics;		// 1 = send a backup tic in every packet
  #ifdef DJGPP
  	SWORD	pad[5];			// keep things aligned for DOS drivers
  #endif
--- 70,75 ----
***************
*** 143,148 ****
--- 142,149 ----
  
  extern	int 			maketic;
  extern	int 			nettics[MAXNETNODES];
+ extern	int				netdelay[MAXNETNODES][BACKUPTICS];
+ extern	int 			nodeforplayer[MAXPLAYERS];
  
  extern	ticcmd_t		netcmds[MAXPLAYERS][BACKUPTICS];
  extern	int 			ticdup;
