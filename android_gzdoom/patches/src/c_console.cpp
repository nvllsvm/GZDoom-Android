*** /home/draje/Code/gzdoom/src/c_console.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/c_console.cpp	2017-06-18 10:12:18.486537073 -0400
***************
*** 394,400 ****
--- 394,404 ----
  	while (hist != NULL)
  	{
  		History *next = hist->Newer;
+ #ifdef __ANDROID__
+         M_Free(hist);
+ #else
  		free (hist);
+ #endif
  		hist = next;
  	}
  	HistTail = HistHead = HistPos = NULL;
***************
*** 545,550 ****
--- 549,557 ----
  /* Adds a string to the console and also to the notify buffer */
  int PrintString (int printlevel, const char *outline)
  {
+ #ifdef __ANDROID__
+     LOGI("PrintString: %s", outline);
+ #endif
  	if (printlevel < msglevel || *outline == '\0')
  	{
  		return 0;
