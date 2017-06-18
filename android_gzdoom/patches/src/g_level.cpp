*** /home/draje/Code/gzdoom/src/g_level.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_level.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 1378,1393 ****
  //
  //
  //==========================================================================
  
  void G_SerializeLevel (FArchive &arc, bool hubLoad)
  {
! 	int i = level.totaltime;
! 	
! 	Renderer->StartSerialize(arc);
! 
! 	arc << level.flags
! 		<< level.flags2
! 		<< level.fadeto
  		<< level.found_secrets
  		<< level.found_items
  		<< level.killed_monsters
--- 1445,1463 ----
  //
  //
  //==========================================================================
+ void gl_SerializeGlobals(FArchive &arc);
  
  void G_SerializeLevel (FArchive &arc, bool hubLoad)
  {
! 		int i = level.totaltime;
! 		
! 		Renderer->StartSerialize(arc);
! 	#ifndef __ANDROID__
! 		gl_SerializeGlobals(arc);
! 	#endif
! 		arc << level.flags
! 			<< level.flags2
! 			<< level.fadeto
  		<< level.found_secrets
  		<< level.found_items
  		<< level.killed_monsters
