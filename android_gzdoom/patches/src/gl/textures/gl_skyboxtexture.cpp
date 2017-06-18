*** doom/src/main/jni/gzdoom/src/gl/textures/gl_skyboxtexture.cpp	2017-06-18 23:15:10.669973853 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/textures/gl_skyboxtexture.cpp	2017-06-18 23:34:13.247247761 -0400
***************
*** 132,146 ****
  //
  //-----------------------------------------------------------------------------
  
! void FSkyBox::PrecacheGL(int cache)
  {
! 	for (int i = 0; i < 6; i++)
! 	{
! 		if (faces[i])
! 		{
! 			faces[i]->PrecacheGL(cache);
! 		}
! 	}
  }
  
  //-----------------------------------------------------------------------------
--- 132,140 ----
  //
  //-----------------------------------------------------------------------------
  
! void FSkyBox::PrecacheGL()
  {
! 	//for(int i=0;i<6;i++) if (faces[i]) faces[i]->PrecacheGL();
  }
  
  //-----------------------------------------------------------------------------
