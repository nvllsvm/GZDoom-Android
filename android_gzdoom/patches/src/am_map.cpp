*** /home/draje/Code/gzdoom/src/am_map.cpp	2017-06-18 07:52:50.683013539 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/am_map.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 1579,1584 ****
--- 1583,1591 ----
  	}
  }
  
+ #ifdef __ANDROID__
+ extern void Android_AM_controls(float *zoom, fixed_t *pan_x,fixed_t *pan_y);
+ #endif
  //=============================================================================
  //
  // Updates on Game Tick
***************
*** 1605,1610 ****
--- 1612,1620 ----
  		if (Button_AM_PanDown.bDown) m_paninc.y -= FTOM(F_PANINC);
  	}
  
+ #ifdef __ANDROID__
+ 	Android_AM_controls(&am_zoomdir,&m_paninc.x,&m_paninc.y);
+ #endif
  	// Change the zoom if necessary
  	if (Button_AM_ZoomIn.bDown || Button_AM_ZoomOut.bDown || am_zoomdir != 0)
  		AM_changeWindowScale();
