*** /home/draje/Code/gzdoom/src/sound/i_music.cpp	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/sound/i_music.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 37,50 ****
  #include <windows.h>
  #include <mmsystem.h>
  #else
- #include <SDL.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <signal.h>
  #include <unistd.h>
  #include <wordexp.h>
  #include <stdio.h>
  #include "mus2midi.h"
  #define FALSE 0
--- 37,51 ----
  #include <windows.h>
  #include <mmsystem.h>
  #else
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <signal.h>
  #include <unistd.h>
+ #ifndef __ANDROID__
  #include <wordexp.h>
+ #endif
  #include <stdio.h>
  #include "mus2midi.h"
  #define FALSE 0
***************
*** 498,504 ****
  		}
  #endif
  	}
! 
  	// Check for various raw OPL formats
  	else if (
  		(id[0] == MAKE_ID('R','A','W','A') && id[1] == MAKE_ID('D','A','T','A')) ||		// Rdos Raw OPL
--- 499,505 ----
  		}
  #endif
  	}
! #ifndef ONLY_GPL
  	// Check for various raw OPL formats
  	else if (
  		(id[0] == MAKE_ID('R','A','W','A') && id[1] == MAKE_ID('D','A','T','A')) ||		// Rdos Raw OPL
***************
*** 507,512 ****
--- 508,514 ----
  	{
  		info = new OPLMUSSong (file, musiccache, len);
  	}
+ #endif
  	// Check for game music
  	else if ((fmt = GME_CheckFormat(id[0])) != NULL && fmt[0] != '\0')
  	{
