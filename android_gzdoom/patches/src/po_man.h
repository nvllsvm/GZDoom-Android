*** /home/draje/Code/gzdoom/src/po_man.h	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/po_man.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 74,80 ****
  
  	int GetMirror();
  	bool MovePolyobj (int x, int y, bool force = false);
! 	bool RotatePolyobj (angle_t angle);
  	void ClosestPoint(fixed_t fx, fixed_t fy, fixed_t &ox, fixed_t &oy, side_t **side) const;
  	void LinkPolyobj ();
  	void RecalcActorFloorCeil(FBoundingBox bounds) const;
--- 74,80 ----
  
  	int GetMirror();
  	bool MovePolyobj (int x, int y, bool force = false);
! 	bool RotatePolyobj (angle_t angle, bool fromsave = false);
  	void ClosestPoint(fixed_t fx, fixed_t fy, fixed_t &ox, fixed_t &oy, side_t **side) const;
  	void LinkPolyobj ();
  	void RecalcActorFloorCeil(FBoundingBox bounds) const;
