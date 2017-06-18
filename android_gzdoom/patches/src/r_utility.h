*** /home/draje/Code/gzdoom/src/r_utility.h	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/r_utility.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 61,66 ****
--- 61,68 ----
  subsector_t *R_PointInSubsector (fixed_t x, fixed_t y);
  fixed_t R_PointToDist2 (fixed_t dx, fixed_t dy);
  void R_ResetViewInterpolation ();
+ void R_RebuildViewInterpolation(player_t *player);
+ bool R_GetViewInterpolationStatus();
  void R_SetViewSize (int blocks);
  void R_SetFOV (float fov);
  float R_GetFOV ();
