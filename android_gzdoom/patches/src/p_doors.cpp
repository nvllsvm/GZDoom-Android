*** /home/draje/Code/gzdoom/src/p_doors.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_doors.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 460,466 ****
  						//		run into them (otherwise opening them would be
  						//		a real pain).
  					{
! 						if (!thing->player || thing->player->isbot)
  							return false;	// JDC: bad guys never close doors
  											//Added by MC: Neither do bots.
  
--- 460,466 ----
  						//		run into them (otherwise opening them would be
  						//		a real pain).
  					{
! 						if (!thing->player || thing->player->Bot != NULL)
  							return false;	// JDC: bad guys never close doors
  											//Added by MC: Neither do bots.
  
