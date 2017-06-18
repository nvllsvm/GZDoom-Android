*** doom/src/main/jni/gzdoom/src/gl/utility/gl_clock.cpp	2017-06-18 23:15:10.669973853 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/utility/gl_clock.cpp	2017-06-18 23:34:13.407249240 -0400
***************
*** 23,29 ****
  glcycle_t ProcessAll;
  glcycle_t RenderAll;
  glcycle_t Dirty;
- glcycle_t drawcalls;
  int vertexcount, flatvertices, flatprimitives;
  
  int rendered_lines,rendered_flats,rendered_sprites,render_vertexsplit,render_texsplit,rendered_decals, rendered_portals;
--- 23,28 ----
***************
*** 97,103 ****
  	SetupFlat.Reset();
  	RenderSprite.Reset();
  	SetupSprite.Reset();
- 	drawcalls.Reset();
  
  	flatvertices=flatprimitives=vertexcount=0;
  	render_texsplit=render_vertexsplit=rendered_lines=rendered_flats=rendered_sprites=rendered_decals=rendered_portals = 0;
--- 96,101 ----
***************
*** 118,127 ****
  	str.AppendFormat("W: Render=%2.3f, Split = %2.3f, Setup=%2.3f, Clip=%2.3f\n"
  		"F: Render=%2.3f, Setup=%2.3f\n"
  		"S: Render=%2.3f, Setup=%2.3f\n"
! 		"All=%2.3f, Render=%2.3f, Setup=%2.3f, BSP = %2.3f, Portal=%2.3f, Drawcalls=%2.3f, Finish=%2.3f\n",
  	RenderWall.TimeMS(), SplitWall.TimeMS(), setupwall, clipwall, RenderFlat.TimeMS(), SetupFlat.TimeMS(),
  	RenderSprite.TimeMS(), SetupSprite.TimeMS(), All.TimeMS() + Finish.TimeMS(), RenderAll.TimeMS(),
! 	ProcessAll.TimeMS(), bsp, PortalAll.TimeMS(), drawcalls.TimeMS(), Finish.TimeMS());
  }
  
  static void AppendRenderStats(FString &out)
--- 116,125 ----
  	str.AppendFormat("W: Render=%2.3f, Split = %2.3f, Setup=%2.3f, Clip=%2.3f\n"
  		"F: Render=%2.3f, Setup=%2.3f\n"
  		"S: Render=%2.3f, Setup=%2.3f\n"
! 		"All=%2.3f, Render=%2.3f, Setup=%2.3f, BSP = %2.3f, Portal=%2.3f, Finish=%2.3f\n",
  	RenderWall.TimeMS(), SplitWall.TimeMS(), setupwall, clipwall, RenderFlat.TimeMS(), SetupFlat.TimeMS(),
  	RenderSprite.TimeMS(), SetupSprite.TimeMS(), All.TimeMS() + Finish.TimeMS(), RenderAll.TimeMS(),
! 	ProcessAll.TimeMS(), bsp, PortalAll.TimeMS(), Finish.TimeMS());
  }
  
  static void AppendRenderStats(FString &out)
***************
*** 221,233 ****
  		switchfps = false;
  	}
  	C_HideConsole ();
! }
! 
! bool gl_benching = false;
! 
! void  checkBenchActive()
! {
! 	FStat *stat = FStat::FindStat("rendertimes");
! 	gl_benching = ((stat != NULL && stat->isActive()) || printstats);
! }
! 
--- 219,222 ----
  		switchfps = false;
  	}
  	C_HideConsole ();
! }
\ No newline at end of file
