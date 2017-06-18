*** /home/draje/Code/gzdoom/src/v_text.cpp	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/v_text.cpp	2017-06-18 09:42:31.889709513 -0400
***************
*** 128,134 ****
  	{
  		va_list *more_p;
  		DWORD data;
- 		void *ptrval;
  
  		switch (tag)
  		{
--- 128,133 ----
***************
*** 150,164 ****
  		// We don't handle these. :(
  		case DTA_DestWidth:
  		case DTA_DestHeight:
- 			*(DWORD *)tags = TAG_IGNORE;
- 			data = va_arg (tags, DWORD);
- 			break;
- 
- 		// Translation is specified explicitly by the text.
  		case DTA_Translation:
! 			*(DWORD *)tags = TAG_IGNORE;
! 			ptrval = va_arg (tags, void*);
! 			break;
  
  		case DTA_CleanNoMove_1:
  			boolval = va_arg (tags, INTBOOL);
--- 149,157 ----
  		// We don't handle these. :(
  		case DTA_DestWidth:
  		case DTA_DestHeight:
  		case DTA_Translation:
! 			assert("Bad parameter for DrawText" && false);
! 			return;
  
  		case DTA_CleanNoMove_1:
  			boolval = va_arg (tags, INTBOOL);
