*** /home/draje/Code/gzdoom/src/po_man.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/po_man.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 1051,1057 ****
  //
  //==========================================================================
  
! bool FPolyObj::RotatePolyobj (angle_t angle)
  {
  	int an;
  	bool blocked;
--- 1051,1057 ----
  //
  //==========================================================================
  
! bool FPolyObj::RotatePolyobj (angle_t angle, bool fromsave)
  {
  	int an;
  	bool blocked;
***************
*** 1073,1095 ****
  	validcount++;
  	UpdateBBox();
  
! 	for(unsigned i=0;i < Sidedefs.Size(); i++)
  	{
! 		if (CheckMobjBlocking(Sidedefs[i]))
  		{
! 			blocked = true;
  		}
! 	}
! 	if (blocked)
! 	{
! 		for(unsigned i=0;i < Vertices.Size(); i++)
  		{
! 			Vertices[i]->x = PrevPts[i].x;
! 			Vertices[i]->y = PrevPts[i].y;
  		}
- 		UpdateBBox();
- 		LinkPolyobj();
- 		return false;
  	}
  	this->angle += angle;
  	LinkPolyobj();
--- 1073,1099 ----
  	validcount++;
  	UpdateBBox();
  
! 	// If we are loading a savegame we do not really want to damage actors and be blocked by them. This can also cause crashes when trying to damage incompletely deserialized player pawns.
! 	if (!fromsave)
  	{
! 		for (unsigned i = 0; i < Sidedefs.Size(); i++)
  		{
! 			if (CheckMobjBlocking(Sidedefs[i]))
! 			{
! 				blocked = true;
! 			}
  		}
! 		if (blocked)
  		{
! 			for(unsigned i=0;i < Vertices.Size(); i++)
! 			{
! 				Vertices[i]->x = PrevPts[i].x;
! 				Vertices[i]->y = PrevPts[i].y;
! 			}
! 			UpdateBBox();
! 			LinkPolyobj();
! 			return false;
  		}
  	}
  	this->angle += angle;
  	LinkPolyobj();
