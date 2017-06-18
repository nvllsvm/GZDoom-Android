*** /home/draje/Code/gzdoom/src/c_dispatch.h	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/c_dispatch.h	2017-06-18 09:09:24.161730580 -0400
***************
*** 42,47 ****
--- 42,48 ----
  extern bool CheckCheatmode (bool printmsg = true);
  
  void C_ExecCmdLineParams ();
+ void C_ExecStoredSets();
  
  // Add commands to the console as if they were typed in. Can handle wait
  // and semicolon-separated commands. This function may modify the source
