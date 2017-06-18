*** /home/draje/Code/gzdoom/src/g_shared/sbarinfo.cpp	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_shared/sbarinfo.cpp	2017-06-18 09:09:24.168397311 -0400
***************
*** 1215,1221 ****
--- 1215,1225 ----
  			if(Scaled)
  			{
  				if(cx != 0 || cy != 0)
+ 				{
  					screen->VirtualToRealCoords(dcx, dcy, tmp, tmp, script->resW, script->resH, true);
+ 					if (cx == 0) dcx = 0;
+ 					if (cy == 0) dcy = 0;
+ 				}
  				if(cr != 0 || cb != 0 || clearDontDraw)
  					screen->VirtualToRealCoords(dcr, dcb, tmp, tmp, script->resW, script->resH, true);
  				screen->VirtualToRealCoords(dx, dy, w, h, script->resW, script->resH, true);
***************
*** 1276,1283 ****
  
  			// We can't use DTA_HUDRules since it forces a width and height.
  			// Translation: No high res.
! 			bool xright = rx < 0;
! 			bool ybot = ry < 0;
  
  			w = (forceWidth < 0 ? texture->GetScaledWidthDouble() : forceWidth);
  			h = (forceHeight < 0 ? texture->GetScaledHeightDouble() : forceHeight);
--- 1280,1287 ----
  
  			// We can't use DTA_HUDRules since it forces a width and height.
  			// Translation: No high res.
! 			bool xright = *x < 0;
! 			bool ybot = *y < 0;
  
  			w = (forceWidth < 0 ? texture->GetScaledWidthDouble() : forceWidth);
  			h = (forceHeight < 0 ? texture->GetScaledHeightDouble() : forceHeight);
