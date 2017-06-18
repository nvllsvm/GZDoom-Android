*** doom/src/main/jni/gzdoom/src/gl/scene/gl_sky.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_sky.cpp	2017-06-18 23:34:13.333915227 -0400
***************
*** 111,117 ****
  				}
  
  				FTextureID texno = s->GetTexture(pos);
! 				skyinfo.texture[0] = FMaterial::ValidateTexture(texno, false, true);
  				if (!skyinfo.texture[0] || skyinfo.texture[0]->tex->UseType == FTexture::TEX_Null) goto normalsky;
  				skyinfo.skytexno1 = texno;
  				skyinfo.x_offset[0] = ANGLE_TO_FLOAT(s->GetTextureXOffset(pos));
--- 111,117 ----
  				}
  
  				FTextureID texno = s->GetTexture(pos);
! 				skyinfo.texture[0] = FMaterial::ValidateTexture(texno, true);
  				if (!skyinfo.texture[0] || skyinfo.texture[0]->tex->UseType == FTexture::TEX_Null) goto normalsky;
  				skyinfo.skytexno1 = texno;
  				skyinfo.x_offset[0] = ANGLE_TO_FLOAT(s->GetTextureXOffset(pos));
***************
*** 123,129 ****
  			normalsky:
  				if (level.flags&LEVEL_DOUBLESKY)
  				{
! 					skyinfo.texture[1]=FMaterial::ValidateTexture(sky1texture, false, true);
  					skyinfo.x_offset[1] = GLRenderer->mSky1Pos;
  					skyinfo.doublesky = true;
  				}
--- 123,129 ----
  			normalsky:
  				if (level.flags&LEVEL_DOUBLESKY)
  				{
! 					skyinfo.texture[1]=FMaterial::ValidateTexture(sky1texture, true);
  					skyinfo.x_offset[1] = GLRenderer->mSky1Pos;
  					skyinfo.doublesky = true;
  				}
***************
*** 131,144 ****
  				if ((level.flags&LEVEL_SWAPSKIES || (sky1==PL_SKYFLAT) || (level.flags&LEVEL_DOUBLESKY)) &&
  					sky2texture!=sky1texture)	// If both skies are equal use the scroll offset of the first!
  				{
! 					skyinfo.texture[0]=FMaterial::ValidateTexture(sky2texture, false, true);
  					skyinfo.skytexno1=sky2texture;
  					skyinfo.sky2 = true;
  					skyinfo.x_offset[0] = GLRenderer->mSky2Pos;
  				}
  				else
  				{
! 					skyinfo.texture[0]=FMaterial::ValidateTexture(sky1texture, false, true);
  					skyinfo.skytexno1=sky1texture;
  					skyinfo.x_offset[0] = GLRenderer->mSky1Pos;
  				}
--- 131,144 ----
  				if ((level.flags&LEVEL_SWAPSKIES || (sky1==PL_SKYFLAT) || (level.flags&LEVEL_DOUBLESKY)) &&
  					sky2texture!=sky1texture)	// If both skies are equal use the scroll offset of the first!
  				{
! 					skyinfo.texture[0]=FMaterial::ValidateTexture(sky2texture, true);
  					skyinfo.skytexno1=sky2texture;
  					skyinfo.sky2 = true;
  					skyinfo.x_offset[0] = GLRenderer->mSky2Pos;
  				}
  				else
  				{
! 					skyinfo.texture[0]=FMaterial::ValidateTexture(sky1texture, true);
  					skyinfo.skytexno1=sky1texture;
  					skyinfo.x_offset[0] = GLRenderer->mSky1Pos;
  				}
