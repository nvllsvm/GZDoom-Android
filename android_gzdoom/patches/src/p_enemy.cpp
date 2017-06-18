*** /home/draje/Code/gzdoom/src/p_enemy.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_enemy.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 1853,1859 ****
--- 1853,1863 ----
  
  	AActor *targ = NULL; // Shuts up gcc
  	fixed_t dist;
+ #ifdef __ANDROID__
+ 	angle_t fov = (fov_f == 0) ? ANGLE_180 : angle_t((int)(fov_f * ANGLE_90 / 90));
+ #else
  	angle_t fov = (fov_f == 0) ? ANGLE_180 : angle_t(fov_f * ANGLE_90 / 90);
+ #endif
  	FLookExParams params = { fov, minseedist, maxseedist, maxheardist, flags, seestate };
  
  	if (self->flags5 & MF5_INCONVERSATION)
