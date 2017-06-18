*** /home/draje/Code/gzdoom/src/p_saveg.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_saveg.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 271,277 ****
  
  	dst->cheats |= chasecam;
  
! 	if (dst->isbot)
  	{
  		botinfo_t *thebot = bglobal.botinfo;
  		while (thebot && stricmp (name, thebot->name))
--- 271,277 ----
  
  	dst->cheats |= chasecam;
  
! 	if (dst->Bot != NULL)
  	{
  		botinfo_t *thebot = bglobal.botinfo;
  		while (thebot && stricmp (name, thebot->name))
***************
*** 280,289 ****
  		}
  		if (thebot)
  		{
! 			thebot->inuse = true;
  		}
  		bglobal.botnum++;
- 		bglobal.botingame[dst - players] = true;
  		dst->userinfo.TransferFrom(uibackup2);
  	}
  	else
--- 280,288 ----
  		}
  		if (thebot)
  		{
! 			thebot->inuse = BOTINUSE_Yes;
  		}
  		bglobal.botnum++;
  		dst->userinfo.TransferFrom(uibackup2);
  	}
  	else
***************
*** 570,576 ****
  				I_Error ("UnarchivePolyobjs: Invalid polyobj tag");
  			}
  			arc << angle;
! 			po->RotatePolyobj (angle);
  			arc << deltaX << deltaY << po->interpolation;
  			deltaX -= po->StartSpot.x;
  			deltaY -= po->StartSpot.y;
--- 569,575 ----
  				I_Error ("UnarchivePolyobjs: Invalid polyobj tag");
  			}
  			arc << angle;
! 			po->RotatePolyobj (angle, true);
  			arc << deltaX << deltaY << po->interpolation;
  			deltaX -= po->StartSpot.x;
  			deltaY -= po->StartSpot.y;
