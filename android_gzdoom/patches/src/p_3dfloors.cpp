*** /home/draje/Code/gzdoom/src/p_3dfloors.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_3dfloors.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 121,126 ****
--- 121,127 ----
  	
  	//Add the floor
  	ffloor = new F3DFloor;
+ 	ffloor->top.copied = ffloor->bottom.copied = false;
  	ffloor->top.model = ffloor->bottom.model = ffloor->model = sec2;
  	ffloor->target = sec;
  	ffloor->ceilingclip = ffloor->floorclip = NULL;
***************
*** 422,427 ****
--- 423,430 ----
  	F3DFloor *		pick;
  	unsigned		pickindex;
  	F3DFloor *		clipped=NULL;
+ 	F3DFloor *		solid=NULL;
+ 	fixed_t			solid_bottom=0;
  	fixed_t			clipped_top;
  	fixed_t			clipped_bottom=0;
  	fixed_t			maxheight, minheight;
***************
*** 479,484 ****
--- 482,488 ----
  			}
  
  			oldlist.Delete(pickindex);
+ 			fixed_t pick_bottom=pick->bottom.plane->ZatPoint(CenterSpot(sector));
  
  			if (pick->flags & FF_THISINSIDE)
  			{
***************
*** 488,497 ****
  			}
  			else if (pick->flags&(FF_SWIMMABLE|FF_TRANSLUCENT) && pick->flags&FF_EXISTS)
  			{
! 				clipped=pick;
! 				clipped_top=height;
! 				clipped_bottom=pick->bottom.plane->ZatPoint(CenterSpot(sector));
! 				ffloors.Push(pick);
  			}
  			else if (clipped && clipped_bottom<height)
  			{
--- 492,529 ----
  			}
  			else if (pick->flags&(FF_SWIMMABLE|FF_TRANSLUCENT) && pick->flags&FF_EXISTS)
  			{
! 				// We must check if this nonsolid segment gets clipped from the top by another 3D floor
! 				if (solid != NULL && solid_bottom < height)
! 				{
! 					ffloors.Push(pick);
! 					if (solid_bottom < pick_bottom)
! 					{
! 						// this one is fully covered
! 						pick->flags|=FF_CLIPPED;
! 						pick->flags&=~FF_EXISTS;
! 					}
! 					else
! 					{
! 						F3DFloor * dyn=new F3DFloor;
! 						*dyn=*pick;
! 						pick->flags|=FF_CLIPPED;
! 						pick->flags&=~FF_EXISTS;
! 						dyn->flags|=FF_DYNAMIC;
! 						dyn->top.copyPlane(&solid->bottom);
! 						ffloors.Push(dyn);
! 
! 						clipped = dyn;
! 						clipped_top = solid_bottom;
! 						clipped_bottom = pick_bottom;
! 					}
! 				}
! 				else
! 				{
! 					clipped = pick;
! 					clipped_top = height;
! 					clipped_bottom = pick_bottom;
! 					ffloors.Push(pick);
! 				}
  			}
  			else if (clipped && clipped_bottom<height)
  			{
***************
*** 501,512 ****
  				clipped->flags|=FF_CLIPPED;
  				clipped->flags&=~FF_EXISTS;
  				dyn->flags|=FF_DYNAMIC;
! 				dyn->bottom=pick->top;
  				ffloors.Push(dyn);
  				ffloors.Push(pick);
  
- 				fixed_t pick_bottom=pick->bottom.plane->ZatPoint(CenterSpot(sector));
- 
  				if (pick_bottom<=clipped_bottom)
  				{
  					clipped=NULL;
--- 533,542 ----
  				clipped->flags|=FF_CLIPPED;
  				clipped->flags&=~FF_EXISTS;
  				dyn->flags|=FF_DYNAMIC;
! 				dyn->bottom.copyPlane(&pick->top);
  				ffloors.Push(dyn);
  				ffloors.Push(pick);
  
  				if (pick_bottom<=clipped_bottom)
  				{
  					clipped=NULL;
***************
*** 517,530 ****
  					dyn=new F3DFloor;
  					*dyn=*clipped;
  					dyn->flags|=FF_DYNAMIC|FF_EXISTS;
! 					dyn->top=pick->bottom;
  					ffloors.Push(dyn);
  				}
  			}
  			else
  			{
! 				clipped=NULL;
  				ffloors.Push(pick);
  			}
  
  		}
--- 547,571 ----
  					dyn=new F3DFloor;
  					*dyn=*clipped;
  					dyn->flags|=FF_DYNAMIC|FF_EXISTS;
! 					dyn->top.copyPlane(&pick->bottom);
  					ffloors.Push(dyn);
+ 					clipped = dyn;
+ 					clipped_top = pick_bottom;
  				}
+ 				solid = pick;
+ 				solid_bottom = pick_bottom;
  			}
  			else
  			{
! 				clipped = NULL;
! 				if (solid == NULL || solid_bottom > pick_bottom)
! 				{
! 					// only if this one is lower
! 					solid = pick;
! 					solid_bottom = pick_bottom;
! 				}
  				ffloors.Push(pick);
+ 
  			}
  
  		}
***************
*** 912,914 ****
--- 953,981 ----
  }
  
  #endif
+ 
+ #include "c_dispatch.h"
+ 
+ 
+ CCMD (dump3df)
+ {
+ 	if (argv.argc() > 1) 
+ 	{
+ 		int sec = strtol(argv[1], NULL, 10);
+ 		sector_t *sector = &sectors[sec];
+ 		TArray<F3DFloor*> & ffloors=sector->e->XFloor.ffloors;
+ 
+ 		for (unsigned int i = 0; i < ffloors.Size(); i++)
+ 		{
+ 			fixed_t height=ffloors[i]->top.plane->ZatPoint(CenterSpot(sector));
+ 			fixed_t bheight=ffloors[i]->bottom.plane->ZatPoint(CenterSpot(sector));
+ 
+ 			IGNORE_FORMAT_PRE
+ 			Printf("FFloor %d @ top = %f (model = %d), bottom = %f (model = %d), flags = %B, alpha = %d %s %s\n", 
+ 				i, height / 65536., ffloors[i]->top.model->sectornum, 
+ 				bheight / 65536., ffloors[i]->bottom.model->sectornum,
+ 				ffloors[i]->flags, ffloors[i]->alpha, (ffloors[i]->flags&FF_EXISTS)? "Exists":"", (ffloors[i]->flags&FF_DYNAMIC)? "Dynamic":"");
+ 			IGNORE_FORMAT_POST
+ 		}
+ 	}
+ }
