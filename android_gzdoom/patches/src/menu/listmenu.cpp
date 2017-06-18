*** /home/draje/Code/gzdoom/src/menu/listmenu.cpp	2017-06-18 07:52:50.696346991 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/menu/listmenu.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 300,306 ****
  		if ((DMenu::MenuTime%8) < 6)
  		{
  			screen->DrawText(ConFont, OptionSettings.mFontColorSelection,
! 				mXpos + xofs, mYpos + yofs, "\xd", DTA_Clean, true, TAG_DONE);
  		}
  	}
  	else
--- 300,311 ----
  		if ((DMenu::MenuTime%8) < 6)
  		{
  			screen->DrawText(ConFont, OptionSettings.mFontColorSelection,
! 				(mXpos + xofs - 160) * CleanXfac + screen->GetWidth() / 2,
! 				(mYpos + yofs - 100) * CleanYfac + screen->GetHeight() / 2,
! 				"\xd",
! 				DTA_CellX, 8 * CleanXfac,
! 				DTA_CellY, 8 * CleanYfac,
! 				TAG_DONE);
  		}
  	}
  	else
