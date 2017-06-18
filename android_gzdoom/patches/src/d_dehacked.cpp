*** /home/draje/Code/gzdoom/src/d_dehacked.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/d_dehacked.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 2247,2253 ****
  
  		ReplaceSpecialChars (holdstring.LockBuffer());
  		holdstring.UnlockBuffer();
! 		GStrings.SetString (Line1, holdstring);
  		DPrintf ("%s set to:\n%s\n", Line1, holdstring.GetChars());
  	}
  
--- 2247,2256 ----
  
  		ReplaceSpecialChars (holdstring.LockBuffer());
  		holdstring.UnlockBuffer();
! 		// Account for a discrepancy between Boom's and ZDoom's name for the red skull key pickup message
! 		const char *ll = Line1;
! 		if (!stricmp(ll, "GOTREDSKULL")) ll = "GOTREDSKUL";
! 		GStrings.SetString (ll, holdstring);
  		DPrintf ("%s set to:\n%s\n", Line1, holdstring.GetChars());
  	}
  
