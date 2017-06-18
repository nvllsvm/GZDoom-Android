*** /home/draje/Code/gzdoom/src/zstrformat.cpp	2017-06-18 07:23:17.330140820 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/Doom/gzdoom_2/src/zstrformat.cpp	2017-06-18 09:06:42.506824531 -0400
***************
*** 692,698 ****
--- 692,702 ----
  			dblarg = va_arg(arglist, double);
  			obuff = dtoaresult = dtoa(dblarg, expchar ? 2 : 3, precision, &expt, &signflag, &dtoaend);
  //fp_common:
+ #ifdef __ANDROID__
+ 			decimal_point = '.';
+ #else
  			decimal_point = localeconv()->decimal_point;
+ #endif
  			flags |= F_SIGNED;
  			if (signflag)
  			{
***************
*** 1034,1044 ****
--- 1038,1054 ----
  
  int mysnprintf(char *buffer, size_t count, const char *format, ...)
  {
+ #ifdef __ANDROID__
+ 	va_list argptr;
+ 	va_start(argptr, format);
+ 	return vsnprintf(buffer,count,format,argptr);
+ #else
  	va_list argptr;
  	va_start(argptr, format);
  	int len = myvsnprintf(buffer, count, format, argptr);
  	va_end(argptr);
  	return len;
+ #endif
  }
  
  }
