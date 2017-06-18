*** /home/draje/Code/gzdoom/src/r_defs.h	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/r_defs.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 963,969 ****
  	vertex_t *V2() const;
  
  	//For GL
! 	FLightNode * lighthead;				// all blended lights that may affect this wall
  
  	seg_t **segs;	// all segs belonging to this sidedef in ascending order. Used for precise rendering
  	int numsegs;
--- 963,969 ----
  	vertex_t *V2() const;
  
  	//For GL
! 	FLightNode * lighthead[2];				// all blended lights that may affect this wall
  
  	seg_t **segs;	// all segs belonging to this sidedef in ascending order. Used for precise rendering
  	int numsegs;
***************
*** 1083,1089 ****
  
  	void BuildPolyBSP();
  	// subsector related GL data
! 	FLightNode *	lighthead;	// Light nodes (blended and additive)
  	int				validcount;
  	short			mapsection;
  	char			hacked;			// 1: is part of a render hack
--- 1083,1089 ----
  
  	void BuildPolyBSP();
  	// subsector related GL data
! 	FLightNode *	lighthead[2];	// Light nodes (blended and additive)
  	int				validcount;
  	short			mapsection;
  	char			hacked;			// 1: is part of a render hack
