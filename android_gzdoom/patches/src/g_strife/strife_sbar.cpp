*** /home/draje/Code/gzdoom/src/g_strife/strife_sbar.cpp	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_strife/strife_sbar.cpp	2017-06-18 09:09:24.168397311 -0400
***************
*** 601,607 ****
  			screen->DrawText(SmallFont2, CR_UNTRANSLATED, left + 210 * xscale, top + 8 * yscale, buff,
  				DTA_CleanNoMove, true, TAG_DONE);
  
! 			if (CPlayer->LogText != NULL)
  			{
  				FBrokenLines *lines = V_BreakLines(SmallFont2, 272, CPlayer->LogText);
  				for (i = 0; lines[i].Width >= 0; ++i)
--- 601,607 ----
  			screen->DrawText(SmallFont2, CR_UNTRANSLATED, left + 210 * xscale, top + 8 * yscale, buff,
  				DTA_CleanNoMove, true, TAG_DONE);
  
! 			if (CPlayer->LogText.IsNotEmpty())
  			{
  				FBrokenLines *lines = V_BreakLines(SmallFont2, 272, CPlayer->LogText);
  				for (i = 0; lines[i].Width >= 0; ++i)
