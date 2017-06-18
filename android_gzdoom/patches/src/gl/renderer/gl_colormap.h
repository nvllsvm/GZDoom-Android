*** doom/src/main/jni/gzdoom/src/gl/renderer/gl_colormap.h	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/renderer/gl_colormap.h	2017-06-18 23:34:13.283914766 -0400
***************
*** 13,25 ****
  
  	CM_INVALID=-1,
  	CM_DEFAULT=0,					// untranslated
  	CM_FIRSTSPECIALCOLORMAP,		// first special fixed colormap
  
! 	CM_FOGLAYER = 0x10000000,	// Sprite shaped fog layer
  
  	// These are not to be passed to the texture manager
  	CM_LITE	= 0x20000000,		// special values to handle these items without excessive hacking
  	CM_TORCH= 0x20000010,		// These are not real color manipulations
  };
  
  #define CM_MAXCOLORMAP int(CM_FIRSTSPECIALCOLORMAP + SpecialColormaps.Size())
--- 13,30 ----
  
  	CM_INVALID=-1,
  	CM_DEFAULT=0,					// untranslated
+ 	CM_DESAT0=CM_DEFAULT,
+ 	CM_DESAT1,					// minimum desaturation
+ 	CM_DESAT31=CM_DESAT1+30,	// maximum desaturation = grayscale
  	CM_FIRSTSPECIALCOLORMAP,		// first special fixed colormap
  
! 	// special internal values for texture creation
! 	CM_SHADE= 0x10000002,		// alpha channel texture
  
  	// These are not to be passed to the texture manager
  	CM_LITE	= 0x20000000,		// special values to handle these items without excessive hacking
  	CM_TORCH= 0x20000010,		// These are not real color manipulations
+ 	CM_FOGLAYER= 0x20000020,	// Sprite shaped fog layer - this is only used as a parameter to FMaterial::BindPatch
  };
  
  #define CM_MAXCOLORMAP int(CM_FIRSTSPECIALCOLORMAP + SpecialColormaps.Size())
***************
*** 29,42 ****
  {
  	PalEntry		LightColor;		// a is saturation (0 full, 31=b/w, other=custom colormap)
  	PalEntry		FadeColor;		// a is fadedensity>>1
! 	int				desaturation;
  	int				blendfactor;
  
  	void Clear()
  	{
  		LightColor=0xffffff;
  		FadeColor=0;
! 		desaturation = 0;
  		blendfactor=0;
  	}
  
--- 34,47 ----
  {
  	PalEntry		LightColor;		// a is saturation (0 full, 31=b/w, other=custom colormap)
  	PalEntry		FadeColor;		// a is fadedensity>>1
! 	int				colormap;
  	int				blendfactor;
  
  	void Clear()
  	{
  		LightColor=0xffffff;
  		FadeColor=0;
! 		colormap = CM_DEFAULT;
  		blendfactor=0;
  	}
  
***************
*** 44,57 ****
  	{
  		LightColor.r=LightColor.g=LightColor.b=0xff;
  		blendfactor=0;
- 		desaturation = 0;
  	}
  
  
  	FColormap & operator=(FDynamicColormap * from)
  	{
  		LightColor = from->Color;
! 		desaturation = from->Desaturate;
  		FadeColor = from->Fade;
  		blendfactor = from->Color.a;
  		return * this;
--- 49,67 ----
  	{
  		LightColor.r=LightColor.g=LightColor.b=0xff;
  		blendfactor=0;
  	}
  
  
+ 	void GetFixedColormap()
+ 	{
+ 		Clear();
+ 		colormap = gl_fixedcolormap >= (int)CM_LITE? (int)CM_DEFAULT : gl_fixedcolormap;
+ 	}
+ 
  	FColormap & operator=(FDynamicColormap * from)
  	{
  		LightColor = from->Color;
! 		colormap = from->Desaturate>>3;
  		FadeColor = from->Fade;
  		blendfactor = from->Color.a;
  		return * this;
***************
*** 60,74 ****
  	void CopyLightColor(FDynamicColormap * from)
  	{
  		LightColor = from->Color;
! 		desaturation = from->Desaturate;
  		blendfactor = from->Color.a;
  	}
- 
- 	void Decolorize()	// this for 'nocoloredspritelighting' and not the same as desaturation. The normal formula results in a value that's too dark.
- 	{
- 		int v = (LightColor.r + LightColor.g + LightColor.b) / 3;
- 		LightColor.r = LightColor.g = LightColor.b = (255 + v + v) / 3;
- 	}
  };
  
  
--- 70,78 ----
  	void CopyLightColor(FDynamicColormap * from)
  	{
  		LightColor = from->Color;
! 		colormap = from->Desaturate>>3;
  		blendfactor = from->Color.a;
  	}
  };
  
  
