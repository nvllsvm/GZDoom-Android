*** /home/draje/Code/gzdoom/src/p_acs.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_acs.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 8353,8363 ****
--- 8659,8677 ----
  			break;
  
  		case PCD_SIN:
+ #ifdef __ANDROID__
+ 			STACK(1) = finesine[angle_t((int)(STACK(1)<<16))>>ANGLETOFINESHIFT];
+ #else
  			STACK(1) = finesine[angle_t(STACK(1)<<16)>>ANGLETOFINESHIFT];
+ #endif
  			break;
  
  		case PCD_COS:
+ #ifdef __ANDROID__
+ 			STACK(1) = finecosine[angle_t((int)(STACK(1)<<16))>>ANGLETOFINESHIFT];
+ #else
  			STACK(1) = finecosine[angle_t(STACK(1)<<16)>>ANGLETOFINESHIFT];
+ #endif
  			break;
  
  		case PCD_VECTORANGLE:
