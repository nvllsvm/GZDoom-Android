*** /home/draje/Code/gzdoom/src/nodebuild_utility.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/nodebuild_utility.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 70,76 ****
--- 70,80 ----
  {
  	const double rad2bam = double(1<<30) / M_PI;
  	double ang = atan2 (double(y), double(x));
+ #ifdef __ANDROID__
+ 	return (angle_t((int)(ang * rad2bam))) << 1;
+ #else
  	return angle_t(ang * rad2bam) << 1;
+ #endif
  }
  
  void FNodeBuilder::FindUsedVertices (vertex_t *oldverts, int max)
