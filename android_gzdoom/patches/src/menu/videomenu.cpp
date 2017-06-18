*** /home/draje/Code/gzdoom/src/menu/videomenu.cpp	2017-06-18 07:52:50.696346991 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/menu/videomenu.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 247,254 ****
  					if (Video != NULL)
  					{
  						while ((haveMode = Video->NextMode (&width, &height, &letterbox)) &&
! 							(ratiomatch >= 0 && CheckRatio (width, height) != ratiomatch))
  						{
  						}
  					}
  
--- 247,258 ----
  					if (Video != NULL)
  					{
  						while ((haveMode = Video->NextMode (&width, &height, &letterbox)) &&
! 							ratiomatch >= 0)
  						{
+ 							int ratio;
+ 							CheckRatio (width, height, &ratio);
+ 							if (ratio == ratiomatch)
+ 								break;
  						}
  					}
  
