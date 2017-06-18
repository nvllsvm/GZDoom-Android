*** /home/draje/Code/gzdoom/src/p_spec.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_spec.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 73,78 ****
--- 73,79 ----
  static FRandom pr_playerinspecialsector ("PlayerInSpecialSector");
  void P_SetupPortals();
  
+ EXTERN_CVAR(Bool, cl_predict_specials)
  
  IMPLEMENT_POINTY_CLASS (DScroller)
   DECLARE_POINTER (m_Interpolations[0])
***************
*** 408,413 ****
--- 409,456 ----
  	return true;
  }
  
+ //============================================================================
+ //
+ // P_PredictLine
+ //
+ //============================================================================
+ 
+ bool P_PredictLine(line_t *line, AActor *mo, int side, int activationType)
+ {
+ 	int lineActivation;
+ 	INTBOOL buttonSuccess;
+ 	BYTE special;
+ 
+ 	// Only predict a very specifc section of specials
+ 	if (line->special != Teleport_Line &&
+ 		line->special != Teleport)
+ 	{
+ 		return false;
+ 	}
+ 
+ 	if (!P_TestActivateLine(line, mo, side, activationType) || !cl_predict_specials)
+ 	{
+ 		return false;
+ 	}
+ 
+ 	if (line->locknumber > 0) return false;
+ 	lineActivation = line->activation;
+ 	buttonSuccess = false;
+ 	buttonSuccess = P_ExecuteSpecial(line->special,
+ 		line, mo, side == 1, line->args[0],
+ 		line->args[1], line->args[2],
+ 		line->args[3], line->args[4]);
+ 
+ 	special = line->special;
+ 
+ 	// end of changed code
+ 	if (developer && buttonSuccess)
+ 	{
+ 		Printf("Line special %d predicted on line %i\n", special, int(line - lines));
+ 	}
+ 	return true;
+ }
+ 
  //
  // P_PlayerInSpecialSector
  // Called every tic frame
***************
*** 671,676 ****
--- 714,720 ----
  //============================================================================
  
  CVAR(Bool, showsecretsector, false, 0)
+ CVAR(Bool, cl_showsecretmessage, true, CVAR_ARCHIVE)
  
  void P_GiveSecret(AActor *actor, bool printmessage, bool playsound, int sectornum)
  {
***************
*** 680,686 ****
  		{
  			actor->player->secretcount++;
  		}
! 		if (actor->CheckLocalView (consoleplayer))
  		{
  			if (printmessage)
  			{
--- 724,730 ----
  		{
  			actor->player->secretcount++;
  		}
! 		if (cl_showsecretmessage && actor->CheckLocalView(consoleplayer))
  		{
  			if (printmessage)
  			{
