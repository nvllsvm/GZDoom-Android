*** /home/draje/Code/gzdoom/src/thingdef/thingdef_properties.cpp	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/thingdef/thingdef_properties.cpp	2017-06-18 09:09:24.175064043 -0400
***************
*** 1418,1423 ****
--- 1418,1482 ----
  
  //==========================================================================
  //
+ //==========================================================================
+ DEFINE_PROPERTY(telefogsourcetype, S, Actor)
+ {
+ 	PROP_STRING_PARM(str, 0);
+ 	if (!stricmp(str, "") || (!stricmp(str, "none")) || (!stricmp(str, "null")) || *str == 0) defaults->TeleFogSourceType = NULL;
+ 	else defaults->TeleFogSourceType = FindClassTentative(str,"TeleportFog");
+ }
+ 
+ //==========================================================================
+ //
+ //==========================================================================
+ DEFINE_PROPERTY(telefogdesttype, S, Actor)
+ {
+ 	PROP_STRING_PARM(str, 0);
+ 	if (!stricmp(str, "") || (!stricmp(str, "none")) || (!stricmp(str, "null")) || *str == 0) defaults->TeleFogDestType = NULL;
+ 	else defaults->TeleFogDestType = FindClassTentative(str, "TeleportFog");
+ }
+ 
+ //==========================================================================
+ //
+ //==========================================================================
+ DEFINE_PROPERTY(ripperlevel, I, Actor)
+ {
+ 	PROP_INT_PARM(id, 0);
+ 	if (id < 0)
+ 	{
+ 		I_Error ("RipperLevel must not be negative");
+ 	}
+ 	defaults->RipperLevel = id;
+ }
+ 
+ //==========================================================================
+ //
+ //==========================================================================
+ DEFINE_PROPERTY(riplevelmin, I, Actor)
+ {
+ 	PROP_INT_PARM(id, 0);
+ 	if (id < 0)
+ 	{
+ 		I_Error ("RipLevelMin must not be negative");
+ 	}
+ 	defaults->RipLevelMin = id;
+ }
+ 
+ //==========================================================================
+ //
+ //==========================================================================
+ DEFINE_PROPERTY(riplevelmax, I, Actor)
+ {
+ 	PROP_INT_PARM(id, 0);
+ 	if (id < 0)
+ 	{
+ 		I_Error ("RipLevelMax must not be negative");
+ 	}
+ 	defaults->RipLevelMax = id;
+ }
+ 
+ //==========================================================================
+ //
  // Special inventory properties
  //
  //==========================================================================
***************
*** 2050,2055 ****
--- 2109,2119 ----
  			*pBlendColor = MakeSpecialColormap(v);
  			return;
  		}
+ 		else if (!stricmp(name, "none") && info->Class->IsDescendantOf(RUNTIME_CLASS(APowerupGiver)))
+ 		{
+ 			*pBlendColor = MakeSpecialColormap(65535);
+ 			return;
+ 		}
  
  		color = V_GetColor(NULL, name);
  	}
