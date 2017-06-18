*** /home/draje/Code/gzdoom/src/c_cvars.h	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/c_cvars.h	2017-06-18 09:09:24.161730580 -0400
***************
*** 159,164 ****
--- 159,165 ----
  	friend FBaseCVar *FindCVar (const char *var_name, FBaseCVar **prev);
  	friend FBaseCVar *FindCVarSub (const char *var_name, int namelen);
  	friend void UnlatchCVars (void);
+ 	friend void DestroyCVarsFlagged (DWORD flags);
  	friend void C_ArchiveCVars (FConfigFile *f, uint32 filter);
  	friend void C_SetCVarsToDefaults (void);
  	friend void FilterCompactCVars (TArray<FBaseCVar *> &cvars, uint32 filter);
***************
*** 190,195 ****
--- 191,199 ----
  // Called from G_InitNew()
  void UnlatchCVars (void);
  
+ // Destroy CVars with the matching flags; called from CCMD(restart)
+ void DestroyCVarsFlagged (DWORD flags);
+ 
  // archive cvars to FILE f
  void C_ArchiveCVars (FConfigFile *f, uint32 filter);
  
