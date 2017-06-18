*** /home/draje/Code/gzdoom/src/r_things.cpp	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/r_things.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 927,933 ****
--- 928,938 ----
  		if (voxelspin != 0)
  		{
  			double ang = double(I_FPSTime()) * voxelspin / 1000;
+ #ifdef __ANDROID__
+ 			vis->angle -= angle_t((int)(ang * (4294967296.f / 360)));
+ #else
  			vis->angle -= angle_t(ang * (4294967296.f / 360));
+ #endif
  		}
  
  		vis->vx = viewx;
