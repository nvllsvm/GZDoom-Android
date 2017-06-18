*** /home/draje/Code/gzdoom/src/textures/textures.h	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/textures/textures.h	2017-06-18 11:04:57.342777750 -0400
***************
*** 329,337 ****
  
  	struct MiscGLInfo
  	{
! 		FMaterial *Material[2];
! 		FGLTexture *SystemTexture[2];
  		FTexture *Brightmap;
  		PalEntry GlowColor;
  		PalEntry FloorSkyColor;
  		PalEntry CeilingSkyColor;
--- 329,338 ----
  
  	struct MiscGLInfo
  	{
! 		FMaterial *Material;
! 		FGLTexture *SystemTexture;
  		FTexture *Brightmap;
+ 		FTexture *DecalTexture;					// This is needed for decals of UseType TEX_MiscPatch-
  		PalEntry GlowColor;
  		PalEntry FloorSkyColor;
  		PalEntry CeilingSkyColor;
***************
*** 349,356 ****
--- 350,359 ----
  		char bBrightmapChecked:1;				// Set to 1 if brightmap has been checked
  		bool bBrightmap:1;						// This is a brightmap
  		bool bBrightmapDisablesFullbright:1;	// This disables fullbright display
+ 		bool bDisableFullbright:1;				// This texture will not be displayed as fullbright sprite
  		bool bNoFilter:1;
  		bool bNoCompress:1;
+ 		bool mExpanded:1;
  
  		MiscGLInfo() throw ();
  		~MiscGLInfo();
