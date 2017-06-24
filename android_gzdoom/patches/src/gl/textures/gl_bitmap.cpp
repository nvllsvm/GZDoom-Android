*** doom/src/main/jni/gzdoom/src/gl/textures/gl_bitmap.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/textures/gl_bitmap.cpp	2017-06-18 23:34:13.233914304 -0400
***************
*** 51,75 ****
  //
  //===========================================================================
  template<class T>
! void iCopyColors(unsigned char * pout, const unsigned char * pin, int count, int step)
  {
  	int i;
  
! 	for(i=0;i<count;i++)
  	{
! 		if (T::A(pin) != 0)
  		{
! 			pout[0]=T::R(pin);
! 			pout[1]=T::G(pin);
! 			pout[2]=T::B(pin);
! 			pout[3]=T::A(pin);
  		}
- 		pout+=4;
- 		pin+=step;
  	}
  }
  
! typedef void (*CopyFunc)(unsigned char * pout, const unsigned char * pin, int count, int step);
  
  static CopyFunc copyfuncs[]={
  	iCopyColors<cRGB>,
--- 50,123 ----
  //
  //===========================================================================
  template<class T>
! void iCopyColors(unsigned char * pout, const unsigned char * pin, int cm, int count, int step)
  {
  	int i;
+ 	int fac;
  
! 	if (cm == CM_DEFAULT)
  	{
! 		for(i=0;i<count;i++)
  		{
! 			if (T::A(pin) != 0)
! 			{
! 				pout[0]=T::R(pin);
! 				pout[1]=T::G(pin);
! 				pout[2]=T::B(pin);
! 				pout[3]=T::A(pin);
! 			}
! 			pout+=4;
! 			pin+=step;
! 		}
! 	}
! 	else if (cm == CM_SHADE)
! 	{
! 		// Alpha shade uses the red channel for true color pics
! 		for(i=0;i<count;i++)
! 		{
! 			if (T::A(pin) != 0)
! 			{
! 				pout[0] = pout[1] =	pout[2] = 255;
! 				pout[3] = T::R(pin);
! 			}
! 			pout+=4;
! 			pin+=step;
! 		}
! 	}
! 	else if (cm >= CM_FIRSTSPECIALCOLORMAP && cm < CM_FIRSTSPECIALCOLORMAP + int(SpecialColormaps.Size()))
! 	{
! 		for(i=0;i<count;i++) 
! 		{
! 			if (T::A(pin) != 0)
! 			{
! 				PalEntry pe = SpecialColormaps[cm - CM_FIRSTSPECIALCOLORMAP].GrayscaleToColor[T::Gray(pin)];
! 				pout[0] = pe.r;
! 				pout[1] = pe.g;
! 				pout[2] = pe.b;
! 				pout[3] = T::A(pin);
! 			}
! 			pout+=4;
! 			pin+=step;
! 		}
! 	}
! 	else if (cm<=CM_DESAT31)
! 	{
! 		// Desaturated light settings.
! 		fac=cm-CM_DESAT0;
! 		for(i=0;i<count;i++)
! 		{
! 			if (T::A(pin) != 0)
! 			{
! 				gl_Desaturate(T::Gray(pin), T::R(pin), T::G(pin), T::B(pin), pout[0], pout[1], pout[2], fac);
! 				pout[3] = T::A(pin);
! 			}
! 			pout+=4;
! 			pin+=step;
  		}
  	}
  }
  
! typedef void (*CopyFunc)(unsigned char * pout, const unsigned char * pin, int cm, int count, int step);
  
  static CopyFunc copyfuncs[]={
  	iCopyColors<cRGB>,
***************
*** 99,110 ****
  		BYTE *buffer = GetPixels() + 4*originx + Pitch*originy;
  		for (int y=0;y<srcheight;y++)
  		{
! 			copyfuncs[ct](&buffer[y*Pitch], &patch[y*step_y], srcwidth, step_x);
  		}
  	}
  }
  
  //===========================================================================
  //
  // Paletted to True Color texture copy function
  //
--- 147,204 ----
  		BYTE *buffer = GetPixels() + 4*originx + Pitch*originy;
  		for (int y=0;y<srcheight;y++)
  		{
! 			copyfuncs[ct](&buffer[y*Pitch], &patch[y*step_y], cm, srcwidth, step_x);
  		}
  	}
  }
  
  //===========================================================================
+ // 
+ // Creates one of the special palette translations for the given palette
+ //
+ //===========================================================================
+ void ModifyPalette(PalEntry * pout, PalEntry * pin, int cm, int count)
+ {
+ 	int i;
+ 	int fac;
+ 
+ 	if (cm == CM_DEFAULT)
+ 	{
+ 		if (pin != pout)
+ 			memcpy(pout, pin, count * sizeof(PalEntry));
+ 	}
+ 	else if (cm >= CM_FIRSTSPECIALCOLORMAP && cm < CM_FIRSTSPECIALCOLORMAP + int(SpecialColormaps.Size()))
+ 	{
+ 		for(i=0;i<count;i++)
+ 		{
+ 			int gray = (pin[i].r*77 + pin[i].g*143 + pin[i].b*37) >> 8;
+ 			// This can be done in place so we cannot copy the color directly.
+ 			PalEntry pe = SpecialColormaps[cm - CM_FIRSTSPECIALCOLORMAP].GrayscaleToColor[gray];
+ 			pout[i].r = pe.r;
+ 			pout[i].g = pe.g;
+ 			pout[i].b = pe.b;
+ 			pout[i].a = pin[i].a;
+ 		}
+ 	}
+ 	else if (cm<=CM_DESAT31)
+ 	{
+ 		// Desaturated light settings.
+ 		fac=cm-CM_DESAT0;
+ 		for(i=0;i<count;i++)
+ 		{
+ 			int gray=(pin[i].r*77 + pin[i].g*143 + pin[i].b*36)>>8;
+ 			gl_Desaturate(gray, pin[i].r, pin[i].g, pin[i].b, pout[i].r, pout[i].g, pout[i].b, fac);
+ 			pout[i].a = pin[i].a;
+ 		}
+ 	}
+ 	else if (pin!=pout)
+ 	{
+ 		memcpy(pout, pin, count * sizeof(PalEntry));
+ 	}
+ }
+ 
+ 
+ //===========================================================================
  //
  // Paletted to True Color texture copy function
  //
***************
*** 120,140 ****
  	{
  		BYTE *buffer = GetPixels() + 4*originx + Pitch*originy;
  
! 		if (translation > 0)
  		{
! 			PalEntry *ptrans = GLTranslationPalette::GetPalette(translation);
! 			if (ptrans)
  			{
! 				for (i = 0; i < 256; i++)
! 				{
! 					penew[i] = (ptrans[i] & 0xffffff) | (palette[i] & 0xff000000);
! 				}
  			}
  		}
  		else
  		{
! 			memcpy(penew, palette, 256*sizeof(PalEntry));
  		}
  
  		// convert the image according to the translated palette.
  		for (y=0;y<srcheight;y++)
--- 214,261 ----
  	{
  		BYTE *buffer = GetPixels() + 4*originx + Pitch*originy;
  
! 		// CM_SHADE is an alpha map with 0==transparent and 1==opaque
! 		if (cm == CM_SHADE) 
  		{
! 			for(int i=0;i<256;i++) 
  			{
! 				if (palette[i].a != 0)
! 					penew[i]=PalEntry(i, 255,255,255);
! 				else
! 					penew[i]=PalEntry(0,255,255,255);	// If the palette contains transparent colors keep them.
  			}
  		}
  		else
  		{
! 			// apply any translation. 
! 			// The ice and blood color translations are done directly
! 			// because that yields better results.
! 			switch(translation)
! 			{
! 			default:
! 			{
! 				PalEntry *ptrans = GLTranslationPalette::GetPalette(translation);
! 				if (ptrans)
! 				{
! 					for(i = 0; i < 256; i++)
! 					{
! 						penew[i] = (ptrans[i]&0xffffff) | (palette[i]&0xff000000);
! 					}
! 					break;
! 				}
! 			}
! 
! 			case 0:
! 				memcpy(penew, palette, 256*sizeof(PalEntry));
! 				break;
! 			}
! 			if (cm!=0)
! 			{
! 				// Apply color modifications like invulnerability, desaturation and Boom colormaps
! 				ModifyPalette(penew, penew, cm, 256);
! 			}
  		}
+ 		// Now penew contains the actual palette that is to be used for creating the image.
  
  		// convert the image according to the translated palette.
  		for (y=0;y<srcheight;y++)
***************
*** 150,155 ****
--- 271,285 ----
  					buffer[pos+2] = penew[v].b;
  					buffer[pos+3] = penew[v].a;
  				}
+ 				/*
+ 				else if (penew[v].a!=255)
+ 				{
+ 					buffer[pos  ] = (buffer[pos  ] * penew[v].a + penew[v].r * (1-penew[v].a)) / 255;
+ 					buffer[pos+1] = (buffer[pos+1] * penew[v].a + penew[v].g * (1-penew[v].a)) / 255;
+ 					buffer[pos+2] = (buffer[pos+2] * penew[v].a + penew[v].b * (1-penew[v].a)) / 255;
+ 					buffer[pos+3] = clamp<int>(buffer[pos+3] + (( 255-buffer[pos+3]) * (255-penew[v].a))/255, 0, 255);
+ 				}
+ 				*/
  			}
  		}
  	}
