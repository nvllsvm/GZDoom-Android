*** /home/draje/Code/gzdoom/src/p_states.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_states.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 39,44 ****
--- 39,45 ----
  #include "cmdlib.h"
  #include "i_system.h"
  #include "c_dispatch.h"
+ #include "v_text.h"
  #include "thingdef/thingdef.h"
  
  // Each state is owned by an actor. Actors can own any number of
***************
*** 699,704 ****
--- 700,709 ----
  	{
  		I_Error ("Attempt to get invalid state %s from actor %s.", label, type->TypeName.GetChars());
  	}
+ 	else
+ 	{
+ 		Printf (TEXTCOLOR_RED "Attempt to get invalid state %s from actor %s.\n", label, type->TypeName.GetChars());
+ 	}
  	delete[] namestart;		// free the allocated string buffer
  	return state;
  }
