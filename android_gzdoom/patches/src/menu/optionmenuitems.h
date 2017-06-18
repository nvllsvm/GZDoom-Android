*** /home/draje/Code/gzdoom/src/menu/optionmenuitems.h	2017-06-18 07:23:17.300140515 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/menu/optionmenuitems.h	2017-06-18 09:09:24.171730677 -0400
***************
*** 379,384 ****
--- 379,387 ----
  //
  //=============================================================================
  
+ #ifdef __ANDROID__
+ extern bool bindingbutton;
+ #endif
  class FOptionMenuItemControl : public FOptionMenuItem
  {
  	FKeyBindings *mBindings;
***************
*** 423,428 ****
--- 426,434 ----
  		if (mkey == MKEY_Input)
  		{
  			mWaiting = false;
+ #ifdef __ANDROID__
+ 		bindingbutton = false;
+ #endif
  			mBindings->SetBind(mInput, mAction);
  			return true;
  		}
***************
*** 434,439 ****
--- 440,448 ----
  		else if (mkey == MKEY_Abort)
  		{
  			mWaiting = false;
+ #ifdef __ANDROID__
+ 		bindingbutton = false;
+ #endif
  			return true;
  		}
  		return false;
***************
*** 443,448 ****
--- 452,461 ----
  	{
  		S_Sound (CHAN_VOICE | CHAN_UI, "menu/choose", snd_menuvolume, ATTN_NONE);
  		mWaiting = true;
+ #ifdef __ANDROID__
+ 		mBindings->UnbindACommand(mAction); //Only allow one button to be bound to avoid confusion
+ 		bindingbutton = true;
+ #endif
  		DMenu *input = new DEnterKey(DMenu::CurrentMenu, &mInput);
  		M_ActivateMenu(input);
  		return true;
