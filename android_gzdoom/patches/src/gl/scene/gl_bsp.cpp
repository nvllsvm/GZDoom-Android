*** doom/src/main/jni/gzdoom/src/gl/scene/gl_bsp.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/scene/gl_bsp.cpp	2017-06-18 23:34:13.347248685 -0400
***************
*** 186,194 ****
  		{
  			SetupWall.Clock();
  
! 			GLWall wall;
! 			wall.sub = currentsubsector;
! 			wall.Process(seg, currentsector, backsector);
  			rendered_lines++;
  
  			SetupWall.Unclock();
--- 186,205 ----
  		{
  			SetupWall.Clock();
  
! 			//if (!gl_multithreading)
! 			{
! 				GLWall wall;
! 				wall.sub = currentsubsector;
! 				wall.Process(seg, currentsector, backsector);
! 			}
! 			/*
! 			else
! 			{
! 				FJob *job = new FGLJobProcessWall(currentsubsector, seg, 
! 					currentsector->sectornum, backsector != NULL? backsector->sectornum : -1);
! 				GLRenderer->mThreadManager->AddJob(job);
! 			}
! 			*/
  			rendered_lines++;
  
  			SetupWall.Unclock();
***************
*** 330,340 ****
  	sector_t * sec=sub->sector;
  	if (sec->thinglist != NULL)
  	{
! 		// Handle all things in sector.
! 		for (AActor * thing = sec->thinglist; thing; thing = thing->snext)
  		{
! 			GLRenderer->ProcessSprite(thing, sector);
  		}
  	}
  	SetupSprite.Unclock();
  }
--- 341,361 ----
  	sector_t * sec=sub->sector;
  	if (sec->thinglist != NULL)
  	{
! 		//if (!gl_multithreading)
! 		{
! 			// Handle all things in sector.
! 			for (AActor * thing = sec->thinglist; thing; thing = thing->snext)
! 			{
! 				GLRenderer->ProcessSprite(thing, sector);
! 			}
! 		}
! 		/*
! 		else if (sec->thinglist != NULL)
  		{
! 			FJob *job = new FGLJobProcessSprites(sector);
! 			GLRenderer->mThreadManager->AddJob(job);
  		}
+ 		*/
  	}
  	SetupSprite.Unclock();
  }
***************
*** 395,404 ****
  	{
  		SetupSprite.Clock();
  
! 		for (i = ParticlesInSubsec[DWORD(sub-subsectors)]; i != NO_PARTICLE; i = Particles[i].snext)
  		{
! 			GLRenderer->ProcessParticle(&Particles[i], fakesector);
  		}
  		SetupSprite.Unclock();
  	}
  
--- 416,435 ----
  	{
  		SetupSprite.Clock();
  
! 		//if (!gl_multithreading)
! 		{
! 			for (i = ParticlesInSubsec[DWORD(sub-subsectors)]; i != NO_PARTICLE; i = Particles[i].snext)
! 			{
! 				GLRenderer->ProcessParticle(&Particles[i], fakesector);
! 			}
! 		}
! 		/*
! 		else if (ParticlesInSubsec[DWORD(sub-subsectors)] != NO_PARTICLE)
  		{
! 			FJob job = new FGLJobProcessParticles(sub);
! 			GLRenderer->mThreadManager->AddJob(job);
  		}
+ 		*/
  		SetupSprite.Unclock();
  	}
  
***************
*** 445,451 ****
  					srf |= SSRF_PROCESSED;
  
  					SetupFlat.Clock();
! 					GLRenderer->ProcessSector(fakesector);
  					SetupFlat.Unclock();
  				}
  				// mark subsector as processed - but mark for rendering only if it has an actual area.
--- 476,492 ----
  					srf |= SSRF_PROCESSED;
  
  					SetupFlat.Clock();
! 					//if (!gl_multithreading)
! 					{
! 						GLRenderer->ProcessSector(fakesector);
! 					}
! 					/*
! 					else
! 					{
! 						FJob *job = new FGLJobProcessFlats(sub);
! 						GLRenderer->mThreadManager->AddJob(job);
! 					}
! 					*/
  					SetupFlat.Unclock();
  				}
  				// mark subsector as processed - but mark for rendering only if it has an actual area.
