*** /home/draje/Code/gzdoom/src/r_main.cpp	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/r_main.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 332,338 ****
  		Printf ("Visibility cannot be changed in net games.\n");
  	}
  }
! 
  //==========================================================================
  //
  // R_SetWindow
--- 331,337 ----
  		Printf ("Visibility cannot be changed in net games.\n");
  	}
  }
! #ifndef ONLY_GPL
  //==========================================================================
  //
  // R_SetWindow
***************
*** 407,413 ****
  	// Reset r_*Visibility vars
  	R_SetVisibility (R_GetVisibility ());
  }
! 
  //==========================================================================
  //
  // CVAR r_columnmethod
--- 405,411 ----
  	// Reset r_*Visibility vars
  	R_SetVisibility (R_GetVisibility ());
  }
! #endif
  //==========================================================================
  //
  // CVAR r_columnmethod
***************
*** 427,433 ****
  		setsizeneeded = true;
  	}
  }
! 
  //==========================================================================
  //
  // R_Init
--- 425,431 ----
  		setsizeneeded = true;
  	}
  }
! #ifndef ONLY_GPL
  //==========================================================================
  //
  // R_Init
***************
*** 887,893 ****
  	R_PlaneInitData ();
  }
  
! 
  //==========================================================================
  //
  // STAT fps
--- 885,891 ----
  	R_PlaneInitData ();
  }
  
! #endif
  //==========================================================================
  //
  // STAT fps
