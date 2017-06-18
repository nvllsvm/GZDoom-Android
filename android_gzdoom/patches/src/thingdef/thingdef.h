*** /home/draje/Code/gzdoom/src/thingdef/thingdef.h	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/thingdef/thingdef.h	2017-06-18 09:09:24.175064043 -0400
***************
*** 266,276 ****
  #define GCC_MSEG
  #else
  #define MSVC_ASEG
! #define GCC_ASEG __attribute__((section(SECTION_AREG)))
  #define MSVC_PSEG
! #define GCC_PSEG __attribute__((section(SECTION_GREG)))
  #define MSVC_MSEG
! #define GCC_MSEG __attribute__((section(SECTION_MREG)))
  #endif
  
  
--- 266,276 ----
  #define GCC_MSEG
  #else
  #define MSVC_ASEG
! #define GCC_ASEG __attribute__((section(SECTION_AREG))) __attribute__((used))
  #define MSVC_PSEG
! #define GCC_PSEG __attribute__((section(SECTION_GREG))) __attribute__((used))
  #define MSVC_MSEG
! #define GCC_MSEG __attribute__((section(SECTION_MREG))) __attribute__((used))
  #endif
  
  
***************
*** 429,436 ****
--- 429,454 ----
  	const char *var = EvalExpressionName(ParameterIndex+i, self);
  #define ACTION_PARAM_NAME(var,i) \
  	FName var = EvalExpressionName(ParameterIndex+i, self);
+ 
+ #ifdef __ANDROID__
+ //WTF is wrong with the ARM compiler?!
+ #define ACTION_PARAM_ANGLE(var,i) \
+ 	float var##temp_android = EvalExpressionF(ParameterIndex+i, self); \
+ 	float var##temp_android2 = var##temp_android * ((float)ANGLE_90 / 90.f); \
+ 	angle_t var; \
+ 	if (var##temp_android2 < 0) \
+ 	{ \
+ 		var = -(var##temp_android2); \
+ 		var = -var; \
+ 	} \
+ 	else \
+ 	{ \
+ 		 var = (var##temp_android2); \
+ 	}
+ #else
  #define ACTION_PARAM_ANGLE(var,i) \
  	angle_t var = angle_t(EvalExpressionF(ParameterIndex+i, self)*ANGLE_90/90.f);
+ #endif
  
  #define ACTION_SET_RESULT(v) if (statecall != NULL) statecall->Result = v;
  
