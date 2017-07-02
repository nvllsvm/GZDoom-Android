*** doom/src/main/jni/gzdoom/src/sdl/i_system.cpp	2017-07-01 18:06:20.415950933 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/sdl/i_system.cpp	2017-07-02 13:35:56.184596660 -0400
***************
*** 337,342 ****
--- 337,349 ----
      I_InitSound ();
  }
  
+ #ifdef __ANDROID__
+ extern "C"
+ {
+     void appShutdown();
+ }
+ #endif
+ 
  //
  // I_Quit
  //
***************
*** 344,354 ****
--- 351,366 ----
  
  void I_Quit (void)
  {
+     LOGI("I_Quit");
      has_exited = 1;		/* Prevent infinitely recursive exits -- killough */
  
      if (demorecording)
  		G_CheckDemoStatus();
  
+ #ifdef __ANDROID__
+     appShutdown();
+ #endif
+ 
  	C_DeinitConsole();
  }
  
***************
*** 367,372 ****
--- 379,385 ----
  {
      static bool alreadyThrown = false;
      gameisdead = true;
+     LOGI("I_FatalError");
  
      if (!alreadyThrown)		// ignore all but the first message -- killough
      {
***************
*** 377,382 ****
--- 390,396 ----
  		va_start (argptr, error);
  		index = vsnprintf (errortext, MAX_ERRORTEXT, error, argptr);
  		va_end (argptr);
+         LOGI("I_FatalError: %s", errortext);
  
  #ifdef __APPLE__
  		Mac_I_FatalError(errortext);
***************
*** 395,400 ****
--- 409,415 ----
  
      if (!has_exited)	// If it hasn't exited yet, exit now -- killough
      {
+         LOGI("Fatal exit");
  		has_exited = 1;	// Prevent infinitely recursive exits -- killough
  		exit(-1);
      }
***************
*** 409,414 ****
--- 424,431 ----
      vsprintf (errortext, error, argptr);
      va_end (argptr);
  
+     LOGI("Error: %s", errortext);
+ 
      throw CRecoverableError (errortext);
  }
  
***************
*** 628,634 ****
  		return defaultiwad;
  	}
  
! #if !defined(__APPLE__)
  	const char *str;
  	if((str=getenv("KDE_FULL_SESSION")) && strcmp(str, "true") == 0)
  	{
--- 645,651 ----
  		return defaultiwad;
  	}
  
! #if !defined(__APPLE__) && !defined(__ANDROID__)
  	const char *str;
  	if((str=getenv("KDE_FULL_SESSION")) && strcmp(str, "true") == 0)
  	{
