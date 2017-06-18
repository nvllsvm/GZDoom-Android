*** /home/draje/Code/gzdoom/src/doomdata.h	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/doomdata.h	2017-06-18 09:09:24.161730580 -0400
***************
*** 162,167 ****
--- 162,168 ----
  	ML_BLOCKUSE					= 0x02000000,	// blocks all use actions through this line
  	ML_BLOCKSIGHT				= 0x04000000,	// blocks monster line of sight
  	ML_BLOCKHITSCAN				= 0x08000000,	// blocks hitscan attacks
+ 	ML_3DMIDTEX_IMPASS			= 0x10000000,	// [TP] if 3D midtex, behaves like a height-restricted ML_BLOCKING
  };
  
  
