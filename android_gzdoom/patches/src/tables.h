*** /home/draje/Code/gzdoom/src/tables.h	2017-06-18 07:23:17.320140719 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/tables.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 112,118 ****
--- 112,122 ----
  }
  inline angle_t rad2bam(double ang)
  {
+ #ifdef __ANDROID__
+ 	return angle_t((int)(ang * (double(1<<30) / PI))) << 1;
+ #else
  	return angle_t(ang * (double(1<<30) / PI)) << 1;
+ #endif
  }
  
  #endif // __TABLES_H__
