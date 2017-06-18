*** /home/draje/Code/gzdoom/src/c_cmds.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/c_cmds.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 124,129 ****
--- 124,138 ----
  	Net_WriteByte (CHT_GOD);
  }
  
+ CCMD(god2)
+ {
+ 	if (CheckCheatmode())
+ 		return;
+ 
+ 	Net_WriteByte(DEM_GENERICCHEAT);
+ 	Net_WriteByte(CHT_GOD2);
+ }
+ 
  CCMD (iddqd)
  {
  	if (CheckCheatmode ())
***************
*** 142,147 ****
--- 151,165 ----
  	Net_WriteByte(CHT_BUDDHA);
  }
  
+ CCMD(buddha2)
+ {
+ 	if (CheckCheatmode())
+ 		return;
+ 
+ 	Net_WriteByte(DEM_GENERICCHEAT);
+ 	Net_WriteByte(CHT_BUDDHA2);
+ }
+ 
  CCMD (notarget)
  {
  	if (CheckCheatmode ())
***************
*** 882,889 ****
  			linetarget->SpawnHealth());
  		PrintMiscActorInfo(linetarget);
  	}
! 	else Printf("No target found. Info cannot find actors that have\
! 				the NOBLOCKMAP flag or have height/radius of 0.\n");
  }
  
  //-----------------------------------------------------------------------------
--- 900,907 ----
  			linetarget->SpawnHealth());
  		PrintMiscActorInfo(linetarget);
  	}
! 	else Printf("No target found. Info cannot find actors that have "
! 				"the NOBLOCKMAP flag or have height/radius of 0.\n");
  }
  
  //-----------------------------------------------------------------------------
