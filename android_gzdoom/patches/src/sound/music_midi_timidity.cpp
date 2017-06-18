*** /home/draje/Code/gzdoom/src/sound/music_midi_timidity.cpp	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/sound/music_midi_timidity.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 8,14 ****
--- 8,16 ----
  
  #include <sys/types.h>
  #include <sys/wait.h>
+ #ifndef __ANDROID__
  #include <wordexp.h>
+ #endif
  #include <signal.h>
  
  int ChildQuit;
***************
*** 418,423 ****
--- 420,427 ----
  		LocalFree (msgBuf);
  	}
  	return false;
+ #elif defined(__ANDROID__)
+ 	return false;
  #else
  	if (WavePipe[0] != -1 && WavePipe[1] == -1 && Stream != NULL)
  	{
