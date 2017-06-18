*** /home/draje/Code/gzdoom/src/p_sectors.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_sectors.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 965,971 ****
  		*pfakecontrast = 0;
  	}
  
! 	if (!foggy) // Don't do relative lighting in foggy sectors
  	{
  		if (!(Flags & WALLF_NOFAKECONTRAST) && r_fakecontrast != 0)
  		{
--- 965,971 ----
  		*pfakecontrast = 0;
  	}
  
! 	if (!foggy || level.flags3 & LEVEL3_FORCEFAKECONTRAST) // Don't do relative lighting in foggy sectors
  	{
  		if (!(Flags & WALLF_NOFAKECONTRAST) && r_fakecontrast != 0)
  		{
