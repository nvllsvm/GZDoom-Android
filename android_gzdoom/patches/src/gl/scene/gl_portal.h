*** doom/src/main/jni/gzdoom/src/gl/scene/gl_portal.h	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_portal.h	2017-06-18 23:34:13.317248408 -0400
***************
*** 82,88 ****
  	static int recursion;
  	static unsigned int QueryObject;
  protected:
- 	static TArray<float> planestack;
  	static int MirrorFlag;
  	static int PlaneMirrorFlag;
  	static int renderdepth;
--- 82,87 ----
***************
*** 102,110 ****
  	angle_t savedviewangle;
  	AActor * savedviewactor;
  	area_t savedviewarea;
  	GLPortal *NextPortal;
  	TArray<BYTE> savedmapsection;
- 	TArray<unsigned int> mPrimIndices;
  
  protected:
  	TArray<GLWall> lines;
--- 101,109 ----
  	angle_t savedviewangle;
  	AActor * savedviewactor;
  	area_t savedviewarea;
+ 	unsigned char clipsave;
  	GLPortal *NextPortal;
  	TArray<BYTE> savedmapsection;
  
  protected:
  	TArray<GLWall> lines;
