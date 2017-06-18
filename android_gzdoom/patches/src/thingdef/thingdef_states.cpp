*** /home/draje/Code/gzdoom/src/thingdef/thingdef_states.cpp	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/thingdef/thingdef_states.cpp	2017-06-18 09:09:24.175064043 -0400
***************
*** 332,338 ****
  					goto endofstate;
  				}
  
! 				PSymbol *sym = bag.Info->Class->Symbols.FindSymbol (FName(sc.String, true), true);
  				if (sym != NULL && sym->SymbolType == SYM_ActionFunction)
  				{
  					PSymbolActionFunction *afd = static_cast<PSymbolActionFunction *>(sym);
--- 332,339 ----
  					goto endofstate;
  				}
  
! 				FName funcname = FName(sc.String, true);
! 				PSymbol *sym = bag.Info->Class->Symbols.FindSymbol (funcname, true);
  				if (sym != NULL && sym->SymbolType == SYM_ActionFunction)
  				{
  					PSymbolActionFunction *afd = static_cast<PSymbolActionFunction *>(sym);
***************
*** 434,440 ****
  						sc.MustGetString();
  						if (sc.Compare("("))
  						{
! 							sc.ScriptError("You cannot pass parameters to '%s'\n",sc.String);
  						}
  						sc.UnGet();
  					}
--- 435,441 ----
  						sc.MustGetString();
  						if (sc.Compare("("))
  						{
! 							sc.ScriptError("You cannot pass parameters to '%s'\n", funcname.GetChars());
  						}
  						sc.UnGet();
  					}
