*** /home/draje/Code/gzdoom/src/p_udmf.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_udmf.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 257,263 ****
--- 257,267 ----
  
  angle_t UDMFParserBase::CheckAngle(const char *key)
  {
+ #ifdef __ANDROID__
+ 	return angle_t((int)(CheckFloat(key) * ANGLE_90 / 90.));
+ #else
  	return angle_t(CheckFloat(key) * ANGLE_90 / 90.);
+ #endif
  }
  
  bool UDMFParserBase::CheckBool(const char *key)
