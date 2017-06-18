*** /home/draje/Code/gzdoom/src/menu/menuinput.cpp	2017-06-18 07:23:17.300140515 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/menu/menuinput.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 101,107 ****
  //
  //
  //=============================================================================
! 
  bool DTextEnterMenu::Responder(event_t *ev)
  {
  	if (ev->type == EV_GUI_Event)
--- 101,109 ----
  //
  //
  //=============================================================================
! #ifdef __ANDROID__
! #include "w_wad.h"
! #endif
  bool DTextEnterMenu::Responder(event_t *ev)
  {
  	if (ev->type == EV_GUI_Event)
***************
*** 138,143 ****
--- 140,153 ----
  			}
  			else if (ch == '\r')
  			{
+ #ifdef __ANDROID__
+ 				//Save game name, allow blank names for Ouya
+ 				if (mSizeMode == 1 && !mEnterString[0])
+ 				{
+ 					strcpy(mEnterString,Wads.GetWadName (FWadCollection::IWAD_FILENUM));
+ 				}
+ #endif
+ 
  				if (mEnterString[0])
  				{
  					DMenu *parent = mParentMenu;
