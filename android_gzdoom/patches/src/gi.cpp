*** /home/draje/Code/gzdoom/src/gi.cpp	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/gi.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 290,295 ****
--- 290,297 ----
  			else gameinfo.mCheatMapArrow = "";
  		}
  		// Insert valid keys here.
+ 		GAMEINFOKEY_STRING(mCheatKey, "cheatKey")
+ 		GAMEINFOKEY_STRING(mEasyKey, "easyKey")
  		GAMEINFOKEY_STRING(TitlePage, "titlePage")
  		GAMEINFOKEY_STRINGARRAY(creditPages, "addcreditPage", 8, false)
  		GAMEINFOKEY_STRINGARRAY(creditPages, "CreditPage", 8, true)
