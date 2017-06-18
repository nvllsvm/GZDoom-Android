*** /home/draje/Code/gzdoom/src/thingdef/thingdef_function.cpp	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/thingdef/thingdef_function.cpp	2017-06-18 09:09:24.175064043 -0400
***************
*** 142,148 ****
--- 144,154 ----
  		ret.Type = VAL_Float;
  
  		// shall we use the CRT's sin and cos functions?
+ #ifdef __ANDROID__
+ 		angle_t angle = angle_t((int)(v * ANGLE_90/90.));
+ #else
  		angle_t angle = angle_t(v * ANGLE_90/90.);
+ #endif
  		if (Name == NAME_Sin) ret.Float = FIXED2DBL (finesine[angle>>ANGLETOFINESHIFT]);
  		else ret.Float = FIXED2DBL (finecosine[angle>>ANGLETOFINESHIFT]);
  		return ret;
