*** /home/draje/Code/gzdoom/src/compatibility.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/compatibility.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 82,87 ****
--- 82,88 ----
  	CP_SETWALLYSCALE,
  	CP_SETTHINGZ,
  	CP_SETTAG,
+ 	CP_SETTHINGFLAGS,
  };
  
  // EXTERNAL FUNCTION PROTOTYPES --------------------------------------------
***************
*** 141,146 ****
--- 142,148 ----
  	{ "maskedmidtex",			COMPATF_MASKEDMIDTEX, SLOT_COMPAT },
  	{ "badangles",				COMPATF2_BADANGLES, SLOT_COMPAT2 },
  	{ "floormove",				COMPATF2_FLOORMOVE, SLOT_COMPAT2 },
+ 	{ "soundcutoff",			COMPATF2_SOUNDCUTOFF, SLOT_COMPAT2 },
  
  	{ NULL, 0, 0 }
  };
***************
*** 317,322 ****
--- 319,333 ----
  				sc.MustGetNumber();
  				CompatParams.Push(sc.Number);
  			}
+ 			else if (sc.Compare("setthingflags"))
+ 			{
+ 				if (flags.ExtCommandIndex == ~0u) flags.ExtCommandIndex = CompatParams.Size();
+ 				CompatParams.Push(CP_SETTHINGFLAGS);
+ 				sc.MustGetNumber();
+ 				CompatParams.Push(sc.Number);
+ 				sc.MustGetNumber();
+ 				CompatParams.Push(sc.Number);
+ 			}
  			else 
  			{
  				sc.UnGet();
***************
*** 538,543 ****
--- 549,563 ----
  					}
  					i += 3;
  					break;
+ 				}
+ 				case CP_SETTHINGFLAGS:
+ 				{
+ 					if ((unsigned)CompatParams[i + 1] < MapThingsConverted.Size())
+ 					{
+ 						MapThingsConverted[CompatParams[i + 1]].flags = CompatParams[i + 2];
+ 					}
+ 					i += 3;
+ 					break;
  				}
  			}
  		}
