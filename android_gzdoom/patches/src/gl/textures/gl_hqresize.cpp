*** doom/src/main/jni/gzdoom/src/gl/textures/gl_hqresize.cpp	2017-06-18 23:15:10.669973853 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/textures/gl_hqresize.cpp	2017-06-18 23:34:13.233914304 -0400
***************
*** 40,48 ****
  #include "gl/textures/gl_texture.h"
  #include "c_cvars.h"
  #include "gl/hqnx/hqx.h"
- #ifdef _MSC_VER
- #include "gl/hqnx_asm/hqnx_asm.h"
- #endif
  
  CUSTOM_CVAR(Int, gl_texture_hqresize, 0, CVAR_ARCHIVE | CVAR_GLOBALCONFIG | CVAR_NOINITCALL)
  {
--- 40,45 ----
***************
*** 182,219 ****
  	return newBuffer;
  }
  
- // [BB] hqnx scaling is only supported with the MS compiler.
- #ifdef _MSC_VER
- static unsigned char *hqNxAsmHelper( void (*hqNxFunction) ( int*, unsigned char*, int, int, int ),
- 							  const int N,
- 							  unsigned char *inputBuffer,
- 							  const int inWidth,
- 							  const int inHeight,
- 							  int &outWidth,
- 							  int &outHeight )
- {
- 	outWidth = N * inWidth;
- 	outHeight = N *inHeight;
- 
- 	static int initdone = false;
- 
- 	if (!initdone)
- 	{
- 		HQnX_asm::InitLUTs();
- 		initdone = true;
- 	}
- 
- 	HQnX_asm::CImage cImageIn;
- 	cImageIn.SetImage(inputBuffer, inWidth, inHeight, 32);
- 	cImageIn.Convert32To17();
- 
- 	unsigned char * newBuffer = new unsigned char[outWidth*outHeight*4];
- 	hqNxFunction( reinterpret_cast<int*>(cImageIn.m_pBitmap), newBuffer, cImageIn.m_Xres, cImageIn.m_Yres, outWidth*4 );
- 	delete[] inputBuffer;
- 	return newBuffer;
- }
- #endif
- 
  static unsigned char *hqNxHelper( void (*hqNxFunction) ( unsigned*, unsigned*, int, int ),
  							  const int N,
  							  unsigned char *inputBuffer,
--- 179,184 ----
***************
*** 238,244 ****
  	return newBuffer;
  }
  
- 
  //===========================================================================
  // 
  // [BB] Upsamples the texture in inputBuffer, frees inputBuffer and returns
--- 203,208 ----
***************
*** 260,265 ****
--- 224,233 ----
  	if ( inputTexture->bHasCanvas )
  		return inputBuffer;
  
+ 	// [BB] Don't upsample non-shader handled warped textures. Needs too much memory and time
+ 	if (gl.shadermodel == 2 || (gl.shadermodel == 3 && inputTexture->bWarped))
+ 		return inputBuffer;
+ 
  	switch (inputTexture->UseType)
  	{
  	case FTexture::TEX_Sprite:
***************
*** 281,291 ****
  		outWidth = inWidth;
  		outHeight = inHeight;
  		int type = gl_texture_hqresize;
! #ifdef _MSC_VER
! 		// ASM-hqNx does not preserve the alpha channel so fall back to C-version for such textures
! 		if (!hasAlpha && type > 3 && type <= 6)
  		{
! 			type += 3;
  		}
  #endif
  
--- 249,259 ----
  		outWidth = inWidth;
  		outHeight = inHeight;
  		int type = gl_texture_hqresize;
! #if 0
! 		// hqNx does not preserve the alpha channel so fall back to ScaleNx for such textures
! 		if (hasAlpha && type > 3)
  		{
! 			type -= 3;
  		}
  #endif
  
***************
*** 303,316 ****
  			return hqNxHelper( &hq3x_32, 3, inputBuffer, inWidth, inHeight, outWidth, outHeight );
  		case 6:
  			return hqNxHelper( &hq4x_32, 4, inputBuffer, inWidth, inHeight, outWidth, outHeight );
- #ifdef _MSC_VER
- 		case 7:
- 			return hqNxAsmHelper( &HQnX_asm::hq2x_32, 2, inputBuffer, inWidth, inHeight, outWidth, outHeight );
- 		case 8:
- 			return hqNxAsmHelper( &HQnX_asm::hq3x_32, 3, inputBuffer, inWidth, inHeight, outWidth, outHeight );
- 		case 9:
- 			return hqNxAsmHelper( &HQnX_asm::hq4x_32, 4, inputBuffer, inWidth, inHeight, outWidth, outHeight );
- #endif
  		}
  	}
  	return inputBuffer;
--- 271,276 ----
