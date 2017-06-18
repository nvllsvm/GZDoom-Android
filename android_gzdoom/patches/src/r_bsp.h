*** /home/draje/Code/gzdoom/src/r_bsp.h	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/r_bsp.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 44,56 ****
  
  struct FWallTmapVals
  {
- 	float		DepthOrg, DepthScale;
  	float		UoverZorg, UoverZstep;
  	float		InvZorg, InvZstep;
  
  	void InitFromWallCoords(const FWallCoords *wallc);
  	void InitFromLine(int x1, int y1, int x2, int y2);
- 	void InitDepth();
  };
  
  extern FWallCoords WallC;
--- 44,54 ----
