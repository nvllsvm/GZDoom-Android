*** /home/draje/Code/gzdoom/src/p_3dfloors.h	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_3dfloors.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 77,82 ****
--- 77,89 ----
  		sector_t *		model;
  		int				isceiling;
  		int				vindex;
+ 		bool			copied;
+ 
+ 		void copyPlane(planeref * other)
+ 		{
+ 			*this = *other;
+ 			copied = true;
+ 		}
  	};
  
  	planeref			bottom;
