*** /home/draje/Code/gzdoom/src/thingdef/thingdef_parse.cpp	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/thingdef/thingdef_parse.cpp	2017-06-18 09:09:24.175064043 -0400
***************
*** 374,379 ****
--- 374,381 ----
  		FScriptPosition::ErrorCounter++;
  	}
  
+ 
+ 
  	FName symname = sc.String;
  	if (sc.CheckToken('['))
  	{
***************
*** 391,396 ****
--- 393,407 ----
  	}
  	sc.MustGetToken(';');
  
+ 	// We must ensure that we do not define duplicates, even when they come from a parent table.
+ 	if (symt->FindSymbol(symname, true) != NULL)
+ 	{
+ 		sc.ScriptMessage ("'%s' is already defined in '%s' or one of its ancestors.",
+ 			symname.GetChars(), cls ? cls->TypeName.GetChars() : "Global");
+ 		FScriptPosition::ErrorCounter++;
+ 		return;
+ 	}
+ 
  	PSymbolVariable *sym = new PSymbolVariable(symname);
  	sym->offset = cls->Extend(sizeof(int) * (valuetype.Type == VAL_Array ? valuetype.size : 1));
  	sym->ValueType = valuetype;
