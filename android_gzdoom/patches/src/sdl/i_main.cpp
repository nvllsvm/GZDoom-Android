*** doom/src/main/jni/gzdoom/src/sdl/i_main.cpp	2017-07-01 18:06:20.415950933 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/android/i_main.cpp	2017-07-02 01:33:06.065396558 -0400
***************
*** 210,216 ****
  	size_t len = (char *)end - (char *)start;
  	if (mprotect(page, len, PROT_READ|PROT_WRITE|PROT_EXEC) != 0)
  	{
! 		fprintf(stderr, "mprotect failed\n");
  		exit(1);
  	}
  }
--- 210,216 ----
  	size_t len = (char *)end - (char *)start;
  	if (mprotect(page, len, PROT_READ|PROT_WRITE|PROT_EXEC) != 0)
  	{
! 		LOGI( "mprotect failed\n");
  		exit(1);
  	}
  }
***************
*** 235,250 ****
  void I_StartupJoysticks();
  void I_ShutdownJoysticks();
  
! int main (int argc, char **argv)
  {
- #if !defined (__APPLE__)
- 	{
- 		int s[4] = { SIGSEGV, SIGILL, SIGFPE, SIGBUS };
- 		cc_install_handlers(argc, argv, 4, s, "zdoom-crash.log", DoomSpecificInfo);
- 	}
- #endif // !__APPLE__
  
! 	printf(GAMENAME" %s - %s - SDL version\nCompiled on %s\n",
  		GetVersionString(), GetGitTime(), __DATE__);
  
  	seteuid (getuid ());
--- 235,245 ----
  void I_StartupJoysticks();
  void I_ShutdownJoysticks();
  
! int main_android (int argc, char **argv)
  {
  
! 
! 	LOGI(GAMENAME" %s - %s - SDL version\nCompiled on %s\n",
  		GetVersionString(), GetGitTime(), __DATE__);
  
  	seteuid (getuid ());
***************
*** 267,273 ****
  
  	if (SDL_Init (SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE|SDL_INIT_JOYSTICK) == -1)
  	{
! 		fprintf (stderr, "Could not initialize SDL:\n%s\n", SDL_GetError());
  		return -1;
  	}
  	atterm (SDL_Quit);
--- 262,268 ----
  
  	if (SDL_Init (SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_NOPARACHUTE|SDL_INIT_JOYSTICK) == -1)
  	{
! 		LOGE( "Could not initialize SDL:\n%s\n", SDL_GetError());
  		return -1;
  	}
  	atterm (SDL_Quit);
***************
*** 277,283 ****
  
  		if (SDL_VideoDriverName(viddriver, sizeof(viddriver)) != NULL)
  		{
! 			printf("Using video driver %s\n", viddriver);
  #ifdef USE_XCURSOR
  			UseXCursor = (strcmp(viddriver, "x11") == 0);
  #endif
--- 272,278 ----
  
  		if (SDL_VideoDriverName(viddriver, sizeof(viddriver)) != NULL)
  		{
! 			LOGI("Using video driver %s\n", viddriver);
  #ifdef USE_XCURSOR
  			UseXCursor = (strcmp(viddriver, "x11") == 0);
  #endif
***************
*** 355,365 ****
      {
  		I_ShutdownJoysticks();
  		if (error.GetMessage ())
! 			fprintf (stderr, "%s\n", error.GetMessage ());
  		exit (-1);
      }
      catch (...)
      {
  		call_terms ();
  		throw;
      }
--- 350,361 ----
      {
  		I_ShutdownJoysticks();
  		if (error.GetMessage ())
! 			LOGE("%s\n", error.GetMessage ());
  		exit (-1);
      }
      catch (...)
      {
+     	LOGI("main catch..");
  		call_terms ();
  		throw;
      }
