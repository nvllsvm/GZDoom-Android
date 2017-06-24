*** doom/src/main/jni/gzdoom/src/gl/textures/gl_hwtexture.cpp	2017-06-18 23:15:10.669973853 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/textures/gl_hwtexture.cpp	2017-06-18 23:34:13.247247761 -0400
***************
*** 54,59 ****
--- 54,60 ----
  
  extern TexFilter_s TexFilter[];
  extern int TexFormat[];
+ EXTERN_CVAR(Bool, gl_clamp_per_texture)
  
  
  //===========================================================================
***************
*** 71,178 ****
  int FHardwareTexture::GetTexDimension(int value)
  {
  	if (value > gl.max_texturesize) return gl.max_texturesize;
! 	return value;
! }
  
  
- //===========================================================================
- // 
- //	Quick'n dirty image rescaling.
- //
- // This will only be used when the source texture is larger than
- // what the hardware can manage (extremely rare in Doom)
- //
- // Code taken from wxWidgets
- //
- //===========================================================================
  
! struct BoxPrecalc
  {
! 	int boxStart;
! 	int boxEnd;
! };
  
! static void ResampleBoxPrecalc(TArray<BoxPrecalc>& boxes, int oldDim)
! {
! 	int newDim = boxes.Size();
! 	const double scale_factor_1 = double(oldDim) / newDim;
! 	const int scale_factor_2 = (int)(scale_factor_1 / 2);
  
! 	for (int dst = 0; dst < newDim; ++dst)
  	{
! 		// Source pixel in the Y direction
! 		const int src_p = int(dst * scale_factor_1);
! 
! 		BoxPrecalc& precalc = boxes[dst];
! 		precalc.boxStart = clamp<int>(int(src_p - scale_factor_1 / 2.0 + 1), 0, oldDim - 1);
! 		precalc.boxEnd = clamp<int>(MAX<int>(precalc.boxStart + 1, int(src_p + scale_factor_2)), 0, oldDim - 1);
  	}
! }
! 
! void FHardwareTexture::Resize(int width, int height, unsigned char *src_data, unsigned char *dst_data)
! {
! 
! 	// This function implements a simple pre-blur/box averaging method for
! 	// downsampling that gives reasonably smooth results To scale the image
! 	// down we will need to gather a grid of pixels of the size of the scale
! 	// factor in each direction and then do an averaging of the pixels.
! 
! 	TArray<BoxPrecalc> vPrecalcs(height);
! 	TArray<BoxPrecalc> hPrecalcs(width);
! 
! 	ResampleBoxPrecalc(vPrecalcs, texheight);
! 	ResampleBoxPrecalc(hPrecalcs, texwidth);
! 
! 	int averaged_pixels, averaged_alpha, src_pixel_index;
! 	double sum_r, sum_g, sum_b, sum_a;
! 
! 	for (int y = 0; y < height; y++)         // Destination image - Y direction
  	{
! 		// Source pixel in the Y direction
! 		const BoxPrecalc& vPrecalc = vPrecalcs[y];
  
! 		for (int x = 0; x < width; x++)      // Destination image - X direction
  		{
! 			// Source pixel in the X direction
! 			const BoxPrecalc& hPrecalc = hPrecalcs[x];
! 
! 			// Box of pixels to average
! 			averaged_pixels = 0;
! 			averaged_alpha = 0;
! 			sum_r = sum_g = sum_b = sum_a = 0.0;
! 
! 			for (int j = vPrecalc.boxStart; j <= vPrecalc.boxEnd; ++j)
! 			{
! 				for (int i = hPrecalc.boxStart; i <= hPrecalc.boxEnd; ++i)
! 				{
! 					// Calculate the actual index in our source pixels
! 					src_pixel_index = j * texwidth + i;
! 
! 					int a = src_data[src_pixel_index * 4 + 3];
! 					if (a > 0)	// do not use color from fully transparent pixels
! 					{
! 						sum_r += src_data[src_pixel_index * 4 + 0];
! 						sum_g += src_data[src_pixel_index * 4 + 1];
! 						sum_b += src_data[src_pixel_index * 4 + 2];
! 						sum_a += a;
! 						averaged_pixels++;
! 					}
! 					averaged_alpha++;
! 
! 				}
! 			}
! 
! 			// Calculate the average from the sum and number of averaged pixels
! 			dst_data[0] = (unsigned char)xs_CRoundToInt(sum_r / averaged_pixels);
! 			dst_data[1] = (unsigned char)xs_CRoundToInt(sum_g / averaged_pixels);
! 			dst_data[2] = (unsigned char)xs_CRoundToInt(sum_b / averaged_pixels);
! 			dst_data[3] = (unsigned char)xs_CRoundToInt(sum_a / averaged_alpha);
! 			dst_data += 4;
  		}
  	}
  }
! 
! 
  
  //===========================================================================
  // 
--- 72,132 ----
  int FHardwareTexture::GetTexDimension(int value)
  {
  	if (value > gl.max_texturesize) return gl.max_texturesize;
! 	if (gl.flags&RFL_NPOT_TEXTURE) return value;
  
+ 	int i=1;
+ 	while (i<value) i+=i;
+ #ifdef __ANDROID__
+ 	if (i>2048) i = 2048; //Dont go above this size
+ #endif
+ 	return i;
+ }
  
  
! #ifdef __ANDROID__
! void GL_ResampleTexture (unsigned *in, int inwidth, int inheight, unsigned *out,  int outwidth, int outheight)
  {
! 	LOGI("GL_ResampleTexture %dx%d -> %dx%d",inwidth,inheight,outwidth,outheight);
! 	int		i, j;
! 	unsigned	*inrow, *inrow2;
! 	unsigned	frac, fracstep;
! 	unsigned	p1[2048], p2[2048];
! 	byte		*pix1, *pix2, *pix3, *pix4;
  
! 	fracstep = inwidth*0x10000/outwidth;
  
! 	frac = fracstep>>2;
! 	for (i=0 ; i<outwidth ; i++)
  	{
! 		p1[i] = 4*(frac>>16);
! 		frac += fracstep;
  	}
! 	frac = 3*(fracstep>>2);
! 	for (i=0 ; i<outwidth ; i++)
  	{
! 		p2[i] = 4*(frac>>16);
! 		frac += fracstep;
! 	}
  
! 	for (i=0 ; i<outheight ; i++, out += outwidth)
! 	{
! 		inrow = in + inwidth*(int)((i+0.25)*inheight/outheight);
! 		inrow2 = in + inwidth*(int)((i+0.75)*inheight/outheight);
! 		frac = fracstep >> 1;
! 		for (j=0 ; j<outwidth ; j++)
  		{
! 			pix1 = (byte *)inrow + p1[j];
! 			pix2 = (byte *)inrow + p2[j];
! 			pix3 = (byte *)inrow2 + p1[j];
! 			pix4 = (byte *)inrow2 + p2[j];
! 			((byte *)(out+j))[0] = (pix1[0] + pix2[0] + pix3[0] + pix4[0])>>2;
! 			((byte *)(out+j))[1] = (pix1[1] + pix2[1] + pix3[1] + pix4[1])>>2;
! 			((byte *)(out+j))[2] = (pix1[2] + pix2[2] + pix3[2] + pix4[2])>>2;
! 			((byte *)(out+j))[3] = (pix1[3] + pix2[3] + pix3[3] + pix4[3])>>2;
  		}
  	}
  }
! #endif
  
  //===========================================================================
  // 
***************
*** 183,204 ****
  // strange crashes deep inside the GL driver when I didn't do it!
  //
  //===========================================================================
! 
! unsigned int FHardwareTexture::CreateTexture(unsigned char * buffer, int w, int h, int texunit, bool mipmap, int translation)
  {
  	int rh,rw;
  	int texformat=TexFormat[gl_texture_format];
  	bool deletebuffer=false;
  
! 	if (forcenocompression)
! 	{
! 		texformat = GL_RGBA8;
! 	}
! 	TranslatedTexture * glTex=GetTexID(translation);
! 	if (glTex->glTexID==0) glGenTextures(1,&glTex->glTexID);
! 	if (texunit != 0) glActiveTexture(GL_TEXTURE0+texunit);
! 	glBindTexture(GL_TEXTURE_2D, glTex->glTexID);
! 	lastbound[texunit] = glTex->glTexID;
  
  	if (!buffer)
  	{
--- 137,154 ----
  // strange crashes deep inside the GL driver when I didn't do it!
  //
  //===========================================================================
! void FHardwareTexture::LoadImage(unsigned char * buffer,int w, int h, unsigned int & glTexID,int wrapparam, bool alphatexture, int texunit)
  {
  	int rh,rw;
  	int texformat=TexFormat[gl_texture_format];
  	bool deletebuffer=false;
+ 	bool use_mipmapping = TexFilter[gl_texture_filter].mipmapping;
  
! 	if (alphatexture) texformat=GL_ALPHA8;
! 	else if (forcenocompression) texformat = GL_RGBA8;
! 	if (glTexID==0) glGenTextures(1,&glTexID);
! 	glBindTexture(GL_TEXTURE_2D, glTexID);
! 	lastbound[texunit]=glTexID;
  
  	if (!buffer)
  	{
***************
*** 208,214 ****
  		rh = GetTexDimension (h);
  
  		// The texture must at least be initialized if no data is present.
! 		glTex->mipmapped = false;
  		buffer=(unsigned char *)calloc(4,rw * (rh+1));
  		deletebuffer=true;
  		//texheight=-h;	
--- 158,165 ----
  		rh = GetTexDimension (h);
  
  		// The texture must at least be initialized if no data is present.
! 		mipmap=false;
! 		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, false);
  		buffer=(unsigned char *)calloc(4,rw * (rh+1));
  		deletebuffer=true;
  		//texheight=-h;	
***************
*** 218,230 ****
  		rw = GetTexDimension (w);
  		rh = GetTexDimension (h);
  
! 		if (rw < w || rh < h)
  		{
! 			// The texture is larger than what the hardware can handle so scale it down.
  			unsigned char * scaledbuffer=(unsigned char *)calloc(4,rw * (rh+1));
  			if (scaledbuffer)
  			{
! 				Resize(rw, rh, buffer, scaledbuffer);
  				deletebuffer=true;
  				buffer=scaledbuffer;
  			}
--- 169,211 ----
  		rw = GetTexDimension (w);
  		rh = GetTexDimension (h);
  
! 		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, (mipmap && use_mipmapping && !forcenofiltering));
! 
! 		if (rw == w && rh == h)
! 		{
! 		}
! 		else if (wrapparam==GL_REPEAT || rw < w || rh < h)
  		{
! 			// The image must be scaled to fit the texture
  			unsigned char * scaledbuffer=(unsigned char *)calloc(4,rw * (rh+1));
  			if (scaledbuffer)
  			{
! #ifdef __ANDROID__
! 				GL_ResampleTexture((unsigned *)buffer,w,h,(unsigned *)scaledbuffer,rw,rh);
! #else
! 				gluScaleImage(GL_RGBA,w, h,GL_UNSIGNED_BYTE,buffer, rw, rh, GL_UNSIGNED_BYTE,scaledbuffer);
! #endif
! 				deletebuffer=true;
! 				buffer=scaledbuffer;
! 			}
! 		}
! 		else
! 		{
! 			// The image must be copied to a larger buffer
! 			unsigned char * scaledbuffer=(unsigned char *)calloc(4,rw * (rh+1));
! 			if (scaledbuffer)
! 			{
! 				for(int y=0;y<h;y++)
! 				{
! 					memcpy(scaledbuffer + rw * y * 4, buffer + w * y * 4, w * 4);
! 					// duplicate the last row to eliminate texture filtering artifacts on borders!
! 					if (rw>w) 
! 						memcpy(	scaledbuffer + rw * y * 4 + w * 4,
! 						scaledbuffer + rw * y * 4 + w * 4 -4, 4);
! 				}
! 				// also duplicate the last line for the same reason!
! 				memcpy(	scaledbuffer + rw * h * 4, 	scaledbuffer + rw * (h-1) * 4, w*4 + 4);
! 				
  				deletebuffer=true;
  				buffer=scaledbuffer;
  			}
***************
*** 234,247 ****
  
  	if (deletebuffer) free(buffer);
  
! 	if (mipmap && TexFilter[gl_texture_filter].mipmapping)
  	{
! 		glGenerateMipmap(GL_TEXTURE_2D);
! 		glTex->mipmapped = true;
  	}
- 
- 	if (texunit != 0) glActiveTexture(GL_TEXTURE0);
- 	return glTex->glTexID;
  }
  
  
--- 215,249 ----
  
  	if (deletebuffer) free(buffer);
  
! 	// When using separate samplers the stuff below is not needed.
! 	// if (gl.flags & RFL_SAMPLER_OBJECTS) return;
! 
! 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapparam==GL_CLAMP? GL_CLAMP_TO_EDGE : GL_REPEAT);
! 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapparam==GL_CLAMP? GL_CLAMP_TO_EDGE : GL_REPEAT);
! 	clampmode = wrapparam==GL_CLAMP? GLT_CLAMPX|GLT_CLAMPY : 0;
! 
! 	if (forcenofiltering)
! 	{
! 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
! 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
! 		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.f);
! 	}
! 	else
  	{
! 		if (mipmap && use_mipmapping)
! 		{
! 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TexFilter[gl_texture_filter].minfilter);
! 			if (gl_texture_filter_anisotropic)
! 			{
! 				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, gl_texture_filter_anisotropic);
! 			}
! 		}
! 		else
! 		{
! 			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TexFilter[gl_texture_filter].magfilter);
! 		}
! 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TexFilter[gl_texture_filter].magfilter);
  	}
  }
  
  
***************
*** 250,265 ****
  //	Creates a texture
  //
  //===========================================================================
! FHardwareTexture::FHardwareTexture(int _width, int _height, bool nocompression) 
  {
  	forcenocompression = nocompression;
  	texwidth=_width;
  	texheight=_height;
  
! 	glDefTex.glTexID = 0;
! 	glDefTex.translation = 0;
! 	glDefTex.mipmapped = false;
  	glDepthID = 0;
  }
  
  
--- 252,270 ----
  //	Creates a texture
  //
  //===========================================================================
! FHardwareTexture::FHardwareTexture(int _width, int _height, bool _mipmap, bool wrap, bool nofilter, bool nocompression) 
  {
  	forcenocompression = nocompression;
+ 	mipmap=_mipmap;
  	texwidth=_width;
  	texheight=_height;
  
! 	int cm_arraysize = CM_FIRSTSPECIALCOLORMAP + SpecialColormaps.Size();
! 	glTexID = new unsigned[cm_arraysize];
! 	memset(glTexID,0,sizeof(unsigned int)*cm_arraysize);
! 	clampmode=0;
  	glDepthID = 0;
+ 	forcenofiltering = nofilter;
  }
  
  
***************
*** 268,287 ****
  //	Deletes a texture id and unbinds it from the texture units
  //
  //===========================================================================
! void FHardwareTexture::TranslatedTexture::Delete()
  {
! 	if (glTexID != 0) 
  	{
  		for(int i = 0; i < MAX_TEXTURES; i++)
  		{
! 			if (lastbound[i] == glTexID)
  			{
  				lastbound[i] = 0;
  			}
  		}
! 		glDeleteTextures(1, &glTexID);
! 		glTexID = 0;
! 		mipmapped = false;
  	}
  }
  
--- 273,290 ----
  //	Deletes a texture id and unbinds it from the texture units
  //
  //===========================================================================
! void FHardwareTexture::DeleteTexture(unsigned int texid)
  {
! 	if (texid != 0) 
  	{
  		for(int i = 0; i < MAX_TEXTURES; i++)
  		{
! 			if (lastbound[i] == texid)
  			{
  				lastbound[i] = 0;
  			}
  		}
! 		glDeleteTextures(1, &texid);
  	}
  }
  
***************
*** 296,308 ****
  
  	if (all)
  	{
! 		glDefTex.Delete();
  	}
! 	for(unsigned int i=0;i<glTex_Translated.Size();i++)
  	{
! 		glTex_Translated[i].Delete();
  	}
! 	glTex_Translated.Clear();
  	if (glDepthID != 0) glDeleteRenderbuffers(1, &glDepthID);
  }
  
--- 299,325 ----
  
  	if (all)
  	{
! 		for (int i=0;i<cm_arraysize;i++)
! 		{
! 			DeleteTexture(glTexID[i]);
! 		}
! 		//glDeleteTextures(cm_arraysize,glTexID);
! 		memset(glTexID,0,sizeof(unsigned int)*cm_arraysize);
! 	}
! 	else
! 	{
! 		for (int i=1;i<cm_arraysize;i++)
! 		{
! 			DeleteTexture(glTexID[i]);
! 		}
! 		//glDeleteTextures(cm_arraysize-1,glTexID+1);
! 		memset(glTexID+1,0,sizeof(unsigned int)*(cm_arraysize-1));
  	}
! 	for(unsigned int i=0;i<glTexID_Translated.Size();i++)
  	{
! 		DeleteTexture(glTexID_Translated[i].glTexID);
  	}
! 	glTexID_Translated.Clear();
  	if (glDepthID != 0) glDeleteRenderbuffers(1, &glDepthID);
  }
  
***************
*** 314,319 ****
--- 331,337 ----
  FHardwareTexture::~FHardwareTexture() 
  { 
  	Clean(true); 
+ 	delete [] glTexID;
  }
  
  
***************
*** 323,350 ****
  //
  //===========================================================================
  
! FHardwareTexture::TranslatedTexture *FHardwareTexture::GetTexID(int translation)
  {
! 	if (translation == 0)
  	{
! 		return &glDefTex;
  	}
  
  	// normally there aren't more than very few different 
  	// translations here so this isn't performance critical.
! 	for (unsigned int i = 0; i < glTex_Translated.Size(); i++)
  	{
! 		if (glTex_Translated[i].translation == translation)
  		{
! 			return &glTex_Translated[i];
  		}
  	}
  
! 	int add = glTex_Translated.Reserve(1);
! 	glTex_Translated[add].translation = translation;
! 	glTex_Translated[add].glTexID = 0;
! 	glTex_Translated[add].mipmapped = false;
! 	return &glTex_Translated[add];
  }
  
  //===========================================================================
--- 341,371 ----
  //
  //===========================================================================
  
! unsigned * FHardwareTexture::GetTexID(int cm, int translation)
  {
! 	if (cm < 0 || cm >= CM_MAXCOLORMAP) cm=CM_DEFAULT;
! 
! 	if (translation==0)
  	{
! 		return &glTexID[cm];
  	}
  
  	// normally there aren't more than very few different 
  	// translations here so this isn't performance critical.
! 	for(unsigned int i=0;i<glTexID_Translated.Size();i++)
  	{
! 		if (glTexID_Translated[i].cm == cm &&
! 			glTexID_Translated[i].translation == translation)
  		{
! 			return &glTexID_Translated[i].glTexID;
  		}
  	}
  
! 	int add = glTexID_Translated.Reserve(1);
! 	glTexID_Translated[add].cm=cm;
! 	glTexID_Translated[add].translation=translation;
! 	glTexID_Translated[add].glTexID=0;
! 	return &glTexID_Translated[add].glTexID;
  }
  
  //===========================================================================
***************
*** 352,375 ****
  //	Binds this patch
  //
  //===========================================================================
! unsigned int FHardwareTexture::Bind(int texunit, int translation, bool needmipmap)
  {
! 	TranslatedTexture *pTex = GetTexID(translation);
  
! 	if (pTex->glTexID != 0)
  	{
! 		if (lastbound[texunit] == pTex->glTexID) return pTex->glTexID;
! 		lastbound[texunit] = pTex->glTexID;
! 		if (texunit != 0) glActiveTexture(GL_TEXTURE0 + texunit);
! 		glBindTexture(GL_TEXTURE_2D, pTex->glTexID);
! 		// Check if we need mipmaps on a texture that was creted without them.
! 		if (needmipmap && !pTex->mipmapped && TexFilter[gl_texture_filter].mipmapping)
! 		{
! 			glGenerateMipmap(GL_TEXTURE_2D);
! 			pTex->mipmapped = true;
! 		}
  		if (texunit != 0) glActiveTexture(GL_TEXTURE0);
! 		return pTex->glTexID;
  	}
  	return 0;
  }
--- 373,390 ----
  //	Binds this patch
  //
  //===========================================================================
! unsigned int FHardwareTexture::Bind(int texunit, int cm,int translation)
  {
! 	unsigned int * pTexID=GetTexID(cm, translation);
  
! 	if (*pTexID!=0)
  	{
! 		if (lastbound[texunit]==*pTexID) return *pTexID;
! 		lastbound[texunit]=*pTexID;
! 		if (texunit != 0) glActiveTexture(GL_TEXTURE0+texunit);
! 		glBindTexture(GL_TEXTURE_2D, *pTexID);
  		if (texunit != 0) glActiveTexture(GL_TEXTURE0);
! 		return *pTexID;
  	}
  	return 0;
  }
***************
*** 392,398 ****
  	{
  		Unbind(texunit);
  	}
- 	FMaterial::ClearLastTexture();
  }
  
  //===========================================================================
--- 407,412 ----
***************
*** 403,417 ****
  
  int FHardwareTexture::GetDepthBuffer()
  {
! 	if (glDepthID == 0)
  	{
! 		glGenRenderbuffers(1, &glDepthID);
! 		glBindRenderbuffer(GL_RENDERBUFFER, glDepthID);
! 		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 
! 			GetTexDimension(texwidth), GetTexDimension(texheight));
! 		glBindRenderbuffer(GL_RENDERBUFFER, 0);
  	}
! 	return glDepthID;
  }
  
  
--- 417,435 ----
  
  int FHardwareTexture::GetDepthBuffer()
  {
! 	if (gl.flags & RFL_FRAMEBUFFER)
  	{
! 		if (glDepthID == 0)
! 		{
! 			glGenRenderbuffers(1, &glDepthID);
! 			glBindRenderbuffer(GL_RENDERBUFFER, glDepthID);
! 			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 
! 				GetTexDimension(texwidth), GetTexDimension(texheight));
! 			glBindRenderbuffer(GL_RENDERBUFFER, 0);
! 		}
! 		return glDepthID;
  	}
! 	return 0;
  }
  
  
***************
*** 423,429 ****
  
  void FHardwareTexture::BindToFrameBuffer()
  {
! 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glDefTex.glTexID, 0);
! 	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, GetDepthBuffer()); 
  }
  
--- 441,470 ----
  
  void FHardwareTexture::BindToFrameBuffer()
  {
! 	if (gl.flags & RFL_FRAMEBUFFER)
! 	{
! 		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glTexID[0], 0);
! 		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, GetDepthBuffer()); 
! 	}
! }
! 
! 
! //===========================================================================
! // 
! //	(re-)creates the texture
! //
! //===========================================================================
! unsigned int FHardwareTexture::CreateTexture(unsigned char * buffer, int w, int h, bool wrap, int texunit,
! 									  int cm, int translation)
! {
! 	if (cm < 0 || cm >= CM_MAXCOLORMAP) cm=CM_DEFAULT;
! 
! 	unsigned int * pTexID=GetTexID(cm, translation);
! 
! 	if (texunit != 0) glActiveTexture(GL_TEXTURE0+texunit);
! 	LoadImage(buffer, w, h, *pTexID, wrap? GL_REPEAT:GL_CLAMP, cm==CM_SHADE, texunit);
! 	if (texunit != 0) glActiveTexture(GL_TEXTURE0);
! 	return *pTexID;
  }
  
+ 
