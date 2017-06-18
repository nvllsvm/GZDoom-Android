*** /home/draje/Code/gzdoom/src/g_level.h	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_level.h	2017-06-18 09:09:24.161730580 -0400
***************
*** 52,58 ****
  #define GCC_YSEG
  #else
  #define MSVC_YSEG
! #define GCC_YSEG __attribute__((section(SECTION_YREG)))
  #endif
  
  struct FIntermissionDescriptor;
--- 52,58 ----
  #define GCC_YSEG
  #else
  #define MSVC_YSEG
! #define GCC_YSEG __attribute__((section(SECTION_YREG))) __attribute__((used))
  #endif
  
  struct FIntermissionDescriptor;
***************
*** 216,221 ****
--- 216,224 ----
  	LEVEL2_ENDGAME				= 0x20000000,	// This is an epilogue level that cannot be quit.
  	LEVEL2_NOAUTOSAVEHINT		= 0x40000000,	// tell the game that an autosave for this level does not need to be kept
  	LEVEL2_FORGETSTATE			= 0x80000000,	// forget this map's state in a hub
+ 	
+ 	// More flags!
+ 	LEVEL3_FORCEFAKECONTRAST	= 0x00000001,	// forces fake contrast even with fog enabled
  };
  
  
***************
*** 285,290 ****
--- 288,295 ----
  	int			sucktime;
  	DWORD		flags;
  	DWORD		flags2;
+ 	DWORD		flags3;
+ 
  	FString		Music;
  	FString		LevelName;
  	SBYTE		WallVertLight, WallHorizLight;
***************
*** 398,403 ****
--- 403,409 ----
  
  	DWORD		flags;
  	DWORD		flags2;
+ 	DWORD		flags3;
  
  	DWORD		fadeto;					// The color the palette fades to (usually black)
  	DWORD		outsidefog;				// The fog for sectors with sky ceilings
