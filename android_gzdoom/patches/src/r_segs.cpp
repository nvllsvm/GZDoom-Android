*** /home/draje/Code/gzdoom/src/r_segs.cpp	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/r_segs.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 2893,2898 ****
--- 2893,2900 ----
  { // swall = scale, lwall = texturecolumn
  	double top, bot, i;
  	double xrepeat = fabs((double)walxrepeat);
+ 	double depth_scale = WallT.InvZstep * WallTMapScale2;
+ 	double depth_org = -WallT.UoverZstep * WallTMapScale2;
  
  	i = x1 - centerx;
  	top = WallT.UoverZorg + WallT.UoverZstep * i;
***************
*** 2909,2915 ****
  		{
  			lwall[x] = xs_RoundToInt(frac * xrepeat);
  		}
! 		swall[x] = xs_RoundToInt(frac * WallT.DepthScale + WallT.DepthOrg);
  		top += WallT.UoverZstep;
  		bot += WallT.InvZstep;
  	}
--- 2911,2917 ----
  		{
  			lwall[x] = xs_RoundToInt(frac * xrepeat);
  		}
! 		swall[x] = xs_RoundToInt(frac * depth_scale + depth_org);
  		top += WallT.UoverZstep;
  		bot += WallT.InvZstep;
  	}
