*** /home/draje/Code/gzdoom/src/m_misc.h	2017-06-18 07:52:50.696346991 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/m_misc.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 33,38 ****
--- 33,39 ----
  
  bool M_WriteFile (char const *name, void *source, int length);
  int M_ReadFile (char const *name, BYTE **buffer);
+ int M_ReadFileMalloc (char const *name, BYTE **buffer);
  void M_FindResponseFile (void);
  
  // [RH] M_ScreenShot now accepts a filename parameter.
