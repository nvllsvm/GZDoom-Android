*** /home/draje/Code/gzdoom/src/gl/renderer/gl_colormap.h	2017-06-18 07:52:50.693013627 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/gl/renderer/gl_colormap.h	2017-06-18 09:09:24.168397311 -0400
***************
*** 1,76 ****
! #ifndef __GL_COLORMAP
! #define __GL_COLORMAP
! 
! #include "doomtype.h"
! #include "v_palette.h"
! #include "r_data/colormaps.h"
! 
! extern DWORD gl_fixedcolormap;
! 
! 
! enum EColorManipulation
! {
! 
! 	CM_INVALID=-1,
! 	CM_DEFAULT=0,					// untranslated
! 	CM_FIRSTSPECIALCOLORMAP,		// first special fixed colormap
! 
! 	CM_FOGLAYER = 0x10000000,	// Sprite shaped fog layer
! 
! 	// These are not to be passed to the texture manager
! 	CM_LITE	= 0x20000000,		// special values to handle these items without excessive hacking
! 	CM_TORCH= 0x20000010,		// These are not real color manipulations
! };
! 
! #define CM_MAXCOLORMAP int(CM_FIRSTSPECIALCOLORMAP + SpecialColormaps.Size())
! 
!   // for internal use
! struct FColormap
! {
! 	PalEntry		LightColor;		// a is saturation (0 full, 31=b/w, other=custom colormap)
! 	PalEntry		FadeColor;		// a is fadedensity>>1
! 	int				desaturation;
! 	int				blendfactor;
! 
! 	void Clear()
! 	{
! 		LightColor=0xffffff;
! 		FadeColor=0;
! 		desaturation = 0;
! 		blendfactor=0;
! 	}
! 
! 	void ClearColor()
! 	{
! 		LightColor.r=LightColor.g=LightColor.b=0xff;
! 		blendfactor=0;
! 		desaturation = 0;
! 	}
! 
! 
! 	FColormap & operator=(FDynamicColormap * from)
! 	{
! 		LightColor = from->Color;
! 		desaturation = from->Desaturate;
! 		FadeColor = from->Fade;
! 		blendfactor = from->Color.a;
! 		return * this;
! 	}
! 
! 	void CopyLightColor(FDynamicColormap * from)
! 	{
! 		LightColor = from->Color;
! 		desaturation = from->Desaturate;
! 		blendfactor = from->Color.a;
! 	}
! 
! 	void Decolorize()	// this for 'nocoloredspritelighting' and not the same as desaturation. The normal formula results in a value that's too dark.
! 	{
! 		int v = (LightColor.r + LightColor.g + LightColor.b) / 3;
! 		LightColor.r = LightColor.g = LightColor.b = (255 + v + v) / 3;
! 	}
! };
! 
! 
! 
! #endif
--- 1,76 ----
! #ifndef __GL_COLORMAP
! #define __GL_COLORMAP
! 
! #include "doomtype.h"
! #include "v_palette.h"
! #include "r_data/colormaps.h"
! 
! extern DWORD gl_fixedcolormap;
! 
! 
! enum EColorManipulation
! {
! 
! 	CM_INVALID=-1,
! 	CM_DEFAULT=0,					// untranslated
! 	CM_FIRSTSPECIALCOLORMAP,		// first special fixed colormap
! 
! 	CM_FOGLAYER = 0x10000000,	// Sprite shaped fog layer
! 
! 	// These are not to be passed to the texture manager
! 	CM_LITE	= 0x20000000,		// special values to handle these items without excessive hacking
! 	CM_TORCH= 0x20000010,		// These are not real color manipulations
! };
! 
! #define CM_MAXCOLORMAP int(CM_FIRSTSPECIALCOLORMAP + SpecialColormaps.Size())
! 
!   // for internal use
! struct FColormap
! {
! 	PalEntry		LightColor;		// a is saturation (0 full, 31=b/w, other=custom colormap)
! 	PalEntry		FadeColor;		// a is fadedensity>>1
! 	int				desaturation;
! 	int				blendfactor;
! 
! 	void Clear()
! 	{
! 		LightColor=0xffffff;
! 		FadeColor=0;
! 		desaturation = 0;
! 		blendfactor=0;
! 	}
! 
! 	void ClearColor()
! 	{
! 		LightColor.r=LightColor.g=LightColor.b=0xff;
! 		blendfactor=0;
! 		desaturation = 0;
! 	}
! 
! 
! 	FColormap & operator=(FDynamicColormap * from)
! 	{
! 		LightColor = from->Color;
! 		desaturation = from->Desaturate;
! 		FadeColor = from->Fade;
! 		blendfactor = from->Color.a;
! 		return * this;
! 	}
! 
! 	void CopyLightColor(FDynamicColormap * from)
! 	{
! 		LightColor = from->Color;
! 		desaturation = from->Desaturate;
! 		blendfactor = from->Color.a;
! 	}
! 
! 	void Decolorize()	// this for 'nocoloredspritelighting' and not the same as desaturation. The normal formula results in a value that's too dark.
! 	{
! 		int v = (LightColor.r + LightColor.g + LightColor.b) / 3;
! 		LightColor.r = LightColor.g = LightColor.b = (255 + v + v) / 3;
! 	}
! };
! 
! 
! 
! #endif
