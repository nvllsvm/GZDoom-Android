*** doom/src/main/jni/gzdoom/src/gl/dynlights/gl_dynlight.h	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/dynlights/gl_dynlight.h	2017-06-18 23:34:13.203914027 -0400
***************
*** 95,101 ****
  	FState *targetState;
  	FLightNode * touching_sides;
  	FLightNode * touching_subsectors;
- 	FLightNode * touching_sector;
  
  private:
  	float DistToSeg(seg_t *seg);
--- 95,100 ----
***************
*** 183,190 ****
  
  
  
! bool gl_GetLight(Plane & p, ADynamicLight * light, bool checkside, bool forceadditive, FDynLightData &data);
! void gl_UploadLights(FDynLightData &data);
  
  
  #endif
--- 182,190 ----
  
  
  
! bool gl_GetLight(Plane & p, ADynamicLight * light, int desaturation, bool checkside, bool forceadditive, FDynLightData &data);
! bool gl_SetupLight(Plane & p, ADynamicLight * light, Vector & nearPt, Vector & up, Vector & right, float & scale, int desaturation, bool checkside=true, bool forceadditive=true);
! bool gl_SetupLightTexture();
  
  
  #endif
