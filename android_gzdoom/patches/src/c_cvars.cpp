*** /home/draje/Code/gzdoom/src/c_cvars.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/c_cvars.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 500,508 ****
  					goodv = false;
  				break;
  			default:
! 				if (value[i] < '0' && value[i] > '9' &&
! 					value[i] < 'A' && value[i] > 'F' &&
! 					value[i] < 'a' && value[i] > 'f')
  				{
  					goodv = false;
  				}
--- 500,509 ----
  					goodv = false;
  				break;
  			default:
! 				if (value[i] < '0' || 
! 					(value[i] > '9' && value[i] < 'A') || 
! 					(value[i] > 'F' && value[i] < 'a') || 
! 					value[i] > 'f')
  				{
  					goodv = false;
  				}
***************
*** 1514,1519 ****
--- 1515,1536 ----
  	}
  }
  
+ void DestroyCVarsFlagged (DWORD flags)
+ {
+ 	FBaseCVar *cvar = CVars;
+ 	FBaseCVar *next = cvar;
+ 
+ 	while(cvar)
+ 	{
+ 		next = cvar->m_Next;
+ 
+ 		if(cvar->Flags & flags)
+ 			delete cvar;
+ 
+ 		cvar = next;
+ 	}
+ }
+ 
  void C_SetCVarsToDefaults (void)
  {
  	FBaseCVar *cvar = CVars;
