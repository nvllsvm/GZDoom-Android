*** /home/draje/Code/gzdoom/src/p_acs.h	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_acs.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 308,313 ****
--- 308,314 ----
  	int GetScriptIndex (const ScriptPtr *ptr) const { ptrdiff_t index = ptr - Scripts; return index >= NumScripts ? -1 : (int)index; }
  	ScriptPtr *GetScriptPtr(int index) const { return index >= 0 && index < NumScripts ? &Scripts[index] : NULL; }
  	int GetLumpNum() const { return LumpNum; }
+ 	int GetDataSize() const { return DataSize; }
  	const char *GetModuleName() const { return ModuleName; }
  	ACSProfileInfo *GetFunctionProfileData(int index) { return index >= 0 && index < NumFunctions ? &FunctionProfileData[index] : NULL; }
  	ACSProfileInfo *GetFunctionProfileData(ScriptFunction *func) { return GetFunctionProfileData((int)(func - (ScriptFunction *)Functions)); }
