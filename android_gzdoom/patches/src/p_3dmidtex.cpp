*** /home/draje/Code/gzdoom/src/p_3dmidtex.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_3dmidtex.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 258,263 ****
--- 258,270 ----
  
  bool P_LineOpening_3dMidtex(AActor *thing, const line_t *linedef, FLineOpening &open, bool restrict)
  {
+ 	// [TP] Impassible-like 3dmidtextures do not block missiles
+ 	if ((linedef->flags & ML_3DMIDTEX_IMPASS)
+ 		&& (thing->flags & MF_MISSILE || thing->BounceFlags & BOUNCE_MBF))
+ 	{
+ 		return false;
+ 	}
+ 
  	fixed_t tt, tb;
  
  	open.abovemidtex = false;
