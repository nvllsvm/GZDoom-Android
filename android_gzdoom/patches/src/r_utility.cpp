*** /home/draje/Code/gzdoom/src/r_utility.cpp	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/r_utility.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 731,736 ****
--- 731,772 ----
  
  //==========================================================================
  //
+ // R_RebuildViewInterpolation
+ //
+ //==========================================================================
+ 
+ void R_RebuildViewInterpolation(player_t *player)
+ {
+ 	if (player == NULL || player->camera == NULL)
+ 		return;
+ 
+ 	if (!NoInterpolateView)
+ 		return;
+ 	NoInterpolateView = false;
+ 
+ 	InterpolationViewer *iview = FindPastViewer(player->camera);
+ 
+ 	iview->oviewx = iview->nviewx;
+ 	iview->oviewy = iview->nviewy;
+ 	iview->oviewz = iview->nviewz;
+ 	iview->oviewpitch = iview->nviewpitch;
+ 	iview->oviewangle = iview->nviewangle;
+ }
+ 
+ //==========================================================================
+ //
+ // R_GetViewInterpolationStatus
+ //
+ //==========================================================================
+ 
+ bool R_GetViewInterpolationStatus()
+ {
+ 	return NoInterpolateView;
+ }
+ 
+ 
+ //==========================================================================
+ //
  // R_SetupFrame
  //
  //==========================================================================
