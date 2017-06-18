*** doom/src/main/jni/gzdoom/src/gl/dynlights/a_dynlight.cpp	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/dynlights/a_dynlight.cpp	2017-06-18 23:34:13.197247297 -0400
***************
*** 521,534 ****
  {
  	if (!subSec) return;
  
  	subSec->validcount = ::validcount;
  
! 	touching_subsectors = AddLightNode(&subSec->lighthead, subSec, this, touching_subsectors);
! 	if (subSec->sector->validcount != ::validcount)
! 	{
! 		touching_sector = AddLightNode(&subSec->render_sector->lighthead, subSec->sector, this, touching_sector);
! 		subSec->sector->validcount = ::validcount;
! 	}
  
  	for (unsigned int i = 0; i < subSec->numlines; i++)
  	{
--- 521,531 ----
  {
  	if (!subSec) return;
  
+ 	bool additive = (flags4&MF4_ADDITIVE) || gl_lights_additive;
+ 
  	subSec->validcount = ::validcount;
  
! 	touching_subsectors = AddLightNode(&subSec->lighthead[additive], subSec, this, touching_subsectors);
  
  	for (unsigned int i = 0; i < subSec->numlines; i++)
  	{
***************
*** 540,546 ****
  			if (DMulScale32 (y-seg->v1->y, seg->v2->x-seg->v1->x, seg->v1->x-x, seg->v2->y-seg->v1->y) <=0)
  			{
  				seg->linedef->validcount=validcount;
! 				touching_sides = AddLightNode(&seg->sidedef->lighthead, seg->sidedef, this, touching_sides);
  			}
  		}
  
--- 537,544 ----
  			if (DMulScale32 (y-seg->v1->y, seg->v2->x-seg->v1->x, seg->v1->x-x, seg->v2->y-seg->v1->y) <=0)
  			{
  				seg->linedef->validcount=validcount;
! 				touching_sides = AddLightNode(&seg->sidedef->lighthead[additive], 
! 											  seg->sidedef, this, touching_sides);
  			}
  		}
  
***************
*** 583,603 ****
  		node->lightsource = NULL;
  		node = node->nextTarget;
      }
- 	node = touching_sector;
- 	while (node)
- 	{
- 		node->lightsource = NULL;
- 		node = node->nextTarget;
- 	}
  
  	if (radius>0)
  	{
  		// passing in radius*radius allows us to do a distance check without any calls to sqrtf
  		subsector_t * subSec = R_PointInSubsector(x, y);
  		if (subSec)
  		{
  			float fradius = FIXED2FLOAT(radius);
- 			::validcount++;
  			CollectWithinRadius(subSec, fradius*fradius);
  		}
  	}
--- 581,595 ----
  		node->lightsource = NULL;
  		node = node->nextTarget;
      }
  
  	if (radius>0)
  	{
  		// passing in radius*radius allows us to do a distance check without any calls to sqrtf
+ 		::validcount++;
  		subsector_t * subSec = R_PointInSubsector(x, y);
  		if (subSec)
  		{
  			float fradius = FIXED2FLOAT(radius);
  			CollectWithinRadius(subSec, fradius*fradius);
  		}
  	}
***************
*** 626,642 ****
  		else
  			node = node->nextTarget;
  	}
- 
- 	node = touching_sector;
- 	while (node)
- 	{
- 		if (node->lightsource == NULL)
- 		{
- 			node = DeleteLightNode(node);
- 		}
- 		else
- 			node = node->nextTarget;
- 	}
  }
  
  
--- 618,623 ----
***************
*** 689,696 ****
  
  CCMD(listlights)
  {
! 	int walls, sectors, subsecs;
! 	int allwalls=0, allsectors=0, allsubsecs = 0;
  	int i=0;
  	ADynamicLight * dl;
  	TThinkerIterator<ADynamicLight> it;
--- 670,677 ----
  
  CCMD(listlights)
  {
! 	int walls, sectors;
! 	int allwalls=0, allsectors=0;
  	int i=0;
  	ADynamicLight * dl;
  	TThinkerIterator<ADynamicLight> it;
***************
*** 699,705 ****
  	{
  		walls=0;
  		sectors=0;
- 		subsecs = 0;
  		Printf("%s at (%f, %f, %f), color = 0x%02x%02x%02x, radius = %f ",
  			dl->target? dl->target->GetClass()->TypeName.GetChars() : dl->GetClass()->TypeName.GetChars(),
  			FIXED2FLOAT(dl->x), FIXED2FLOAT(dl->y), FIXED2FLOAT(dl->z), dl->args[LIGHT_RED], 
--- 680,685 ----
***************
*** 728,750 ****
  
  		while (node)
  		{
- 			allsubsecs++;
- 			subsecs++;
- 			node = node->nextTarget;
- 		}
- 
- 		node = dl->touching_sector;
- 
- 		while (node)
- 		{
  			allsectors++;
  			sectors++;
  			node = node->nextTarget;
  		}
! 		Printf("- %d walls, %d subsectors, %d sectors\n", walls, subsecs, sectors);
  
  	}
! 	Printf("%i dynamic lights, %d walls, %d subsectors, %d sectors\n\n\n", i, allwalls, allsubsecs, allsectors);
  }
  
  CCMD(listsublights)
--- 708,722 ----
  
  		while (node)
  		{
  			allsectors++;
  			sectors++;
  			node = node->nextTarget;
  		}
! 
! 		Printf("- %d walls, %d subsectors\n", walls, sectors);
  
  	}
! 	Printf("%i dynamic lights, %d walls, %d subsectors\n\n\n", i, allwalls, allsectors);
  }
  
  CCMD(listsublights)
***************
*** 753,767 ****
  	{
  		subsector_t *sub = &subsectors[i];
  		int lights = 0;
  
! 		FLightNode * node = sub->lighthead;
  		while (node != NULL)
  		{
  			lights++;
  			node = node->nextLight;
  		}
  
! 		Printf(PRINT_LOG, "Subsector %d - %d lights\n", i, lights);
  	}
  }
  
--- 725,746 ----
  	{
  		subsector_t *sub = &subsectors[i];
  		int lights = 0;
+ 		int addlights = 0;
  
! 		FLightNode * node = sub->lighthead[0];
  		while (node != NULL)
  		{
  			lights++;
  			node = node->nextLight;
  		}
  
! 		node = sub->lighthead[1];
! 		while (node != NULL)
! 		{
! 			addlights++;
! 			node = node->nextLight;
! 		}
! 		Printf(PRINT_LOG, "Subsector %d - %d lights, %d additive lights\n", i, lights, addlights);
  	}
  }
  
