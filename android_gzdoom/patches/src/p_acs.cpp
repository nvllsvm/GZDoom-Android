*** /home/draje/Code/gzdoom/src/p_acs.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_acs.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 1581,1600 ****
--- 1581,1608 ----
  	for (modnum = 0; modnum < StaticModules.Size(); ++modnum)
  	{
  		FBehavior *module = StaticModules[modnum];
+ 		int ModSize = module->GetDataSize();
  
  		if (arc.IsStoring())
  		{
  			arc.WriteString (module->ModuleName);
+ 			if (SaveVersion >= 4516) arc << ModSize;
  		}
  		else
  		{
  			char *modname = NULL;
  			arc << modname;
+ 			if (SaveVersion >= 4516) arc << ModSize;
  			if (stricmp (modname, module->ModuleName) != 0)
  			{
  				delete[] modname;
  				I_Error ("Level was saved with a different set of ACS modules.");
  			}
+ 			else if (ModSize != module->GetDataSize())
+ 			{
+ 				delete[] modname;
+ 				I_Error("ACS module %s has changed from what was saved. (Have %d bytes, save has %d bytes)", module->ModuleName, module->GetDataSize(), ModSize);
+ 			}
  			delete[] modname;
  		}
  		module->SerializeVars (arc);
***************
*** 1873,1879 ****
  				funcm->HasReturnValue = funcf->HasReturnValue;
  				funcm->ImportNum = funcf->ImportNum;
  				funcm->LocalCount = funcf->LocalCount;
! 				funcm->Address = funcf->Address;
  			}
  		}
  
--- 1881,1887 ----
  				funcm->HasReturnValue = funcf->HasReturnValue;
  				funcm->ImportNum = funcf->ImportNum;
  				funcm->LocalCount = funcf->LocalCount;
! 				funcm->Address = LittleLong(funcf->Address);
  			}
  		}
  
***************
*** 2058,2064 ****
  			const char *const parse = (char *)&chunk[2];
  			DWORD i;
  
! 			for (i = 0; i < chunk[1]; )
  			{
  				if (parse[i])
  				{
--- 2066,2072 ----
  			const char *const parse = (char *)&chunk[2];
  			DWORD i;
  
! 			for (i = 0; i < LittleLong(chunk[1]); )
  			{
  				if (parse[i])
  				{
***************
*** 2306,2311 ****
--- 2314,2325 ----
  	default:
  		break;
  	}
+ 
+ // [EP] Clang 3.5.0 optimizer miscompiles this function and causes random
+ // crashes in the program. I hope that Clang 3.5.x will fix this.
+ #if defined(__clang__) && __clang_major__ == 3 && __clang_minor__ >= 5
+ 	asm("" : "+g" (NumScripts));
+ #endif
  	for (i = 0; i < NumScripts; ++i)
  	{
  		Scripts[i].Flags = 0;
***************
*** 2345,2351 ****
  	scripts.b = FindChunk (MAKE_ID('S','F','L','G'));
  	if (scripts.dw != NULL)
  	{
! 		max = scripts.dw[1] / 4;
  		scripts.dw += 2;
  		for (i = max; i > 0; --i, scripts.w += 2)
  		{
--- 2359,2365 ----
  	scripts.b = FindChunk (MAKE_ID('S','F','L','G'));
  	if (scripts.dw != NULL)
  	{
! 		max = LittleLong(scripts.dw[1]) / 4;
  		scripts.dw += 2;
  		for (i = max; i > 0; --i, scripts.w += 2)
  		{
***************
*** 2361,2367 ****
  	scripts.b = FindChunk (MAKE_ID('S','V','C','T'));
  	if (scripts.dw != NULL)
  	{
! 		max = scripts.dw[1] / 4;
  		scripts.dw += 2;
  		for (i = max; i > 0; --i, scripts.w += 2)
  		{
--- 2375,2381 ----
  	scripts.b = FindChunk (MAKE_ID('S','V','C','T'));
  	if (scripts.dw != NULL)
  	{
! 		max = LittleLong(scripts.dw[1]) / 4;
  		scripts.dw += 2;
  		for (i = max; i > 0; --i, scripts.w += 2)
  		{
***************
*** 2379,2385 ****
  		int size = LittleLong(scripts.dw[1]);
  		if (size >= 6)
  		{
! 			int script_num = LittleShort(scripts.w[4]);
  			ScriptPtr *ptr = const_cast<ScriptPtr *>(FindScript(script_num));
  			if (ptr != NULL)
  			{
--- 2393,2399 ----
  		int size = LittleLong(scripts.dw[1]);
  		if (size >= 6)
  		{
! 			int script_num = LittleShort(scripts.sw[4]);
  			ScriptPtr *ptr = const_cast<ScriptPtr *>(FindScript(script_num));
  			if (ptr != NULL)
  			{
***************
*** 2675,2681 ****
  		{
  			return chunk;
  		}
! 		chunk += ((DWORD *)chunk)[1] + 8;
  	}
  	return NULL;
  }
--- 2689,2695 ----
  		{
  			return chunk;
  		}
! 		chunk += LittleLong(((DWORD *)chunk)[1]) + 8;
  	}
  	return NULL;
  }
***************
*** 2683,2696 ****
  BYTE *FBehavior::NextChunk (BYTE *chunk) const
  {
  	DWORD id = *(DWORD *)chunk;
! 	chunk += ((DWORD *)chunk)[1] + 8;
  	while (chunk != NULL && chunk < Data + DataSize)
  	{
  		if (((DWORD *)chunk)[0] == id)
  		{
  			return chunk;
  		}
! 		chunk += ((DWORD *)chunk)[1] + 8;
  	}
  	return NULL;
  }
--- 2697,2710 ----
  BYTE *FBehavior::NextChunk (BYTE *chunk) const
  {
  	DWORD id = *(DWORD *)chunk;
! 	chunk += LittleLong(((DWORD *)chunk)[1]) + 8;
  	while (chunk != NULL && chunk < Data + DataSize)
  	{
  		if (((DWORD *)chunk)[0] == id)
  		{
  			return chunk;
  		}
! 		chunk += LittleLong(((DWORD *)chunk)[1]) + 8;
  	}
  	return NULL;
  }
***************
*** 2875,2883 ****
  void DACSThinker::Serialize (FArchive &arc)
  {
  	int scriptnum;
  
  	Super::Serialize (arc);
! 	arc << Scripts << LastScript;
  	if (arc.IsStoring ())
  	{
  		ScriptMap::Iterator it(RunningScripts);
--- 2889,2945 ----
  void DACSThinker::Serialize (FArchive &arc)
  {
  	int scriptnum;
+ 	int scriptcount = 0;
  
  	Super::Serialize (arc);
! 	if (SaveVersion < 4515)
! 		arc << Scripts << LastScript;
! 	else
! 	{
! 		if (arc.IsStoring())
! 		{
! 			DLevelScript *script;
! 			script = Scripts;
! 			while (script)
! 			{
! 				scriptcount++;
! 
! 				// We want to store this list backwards, so we can't loose the last pointer
! 				if (script->next == NULL)
! 					break;
! 				script = script->next;
! 			}
! 			arc << scriptcount;
! 
! 			while (script)
! 			{
! 				arc << script;
! 				script = script->prev;
! 			}
! 		}
! 		else
! 		{
! 			// We are running through this list backwards, so the next entry is the last processed
! 			DLevelScript *next = NULL;
! 			arc << scriptcount;
! 			Scripts = NULL;
! 			LastScript = NULL;
! 			for (int i = 0; i < scriptcount; i++)
! 			{
! 				arc << Scripts;
! 
! 				Scripts->next = next;
! 				Scripts->prev = NULL;
! 				if (next != NULL)
! 					next->prev = Scripts;
! 
! 				next = Scripts;
! 
! 				if (i == 0)
! 					LastScript = Scripts;
! 			}
! 		}
! 	}
  	if (arc.IsStoring ())
  	{
  		ScriptMap::Iterator it(RunningScripts);
***************
*** 2963,2969 ****
  	DWORD i;
  
  	Super::Serialize (arc);
! 	arc << next << prev;
  
  	P_SerializeACSScriptNumber(arc, script, false);
  
--- 3025,3032 ----
  	DWORD i;
  
  	Super::Serialize (arc);
! 	if (SaveVersion < 4515)
! 		arc << next << prev;
  
  	P_SerializeACSScriptNumber(arc, script, false);
  
***************
*** 3618,3623 ****
--- 3681,3687 ----
  	APROP_AttackZOffset	= 40,
  	APROP_StencilColor	= 41,
  	APROP_Friction		= 42,
+ 	APROP_DamageMultiplier=43,
  };
  
  // These are needed for ACS's APROP_RenderStyle
***************
*** 3807,3812 ****
--- 3871,3880 ----
  		actor->DamageFactor = value;
  		break;
  
+ 	case APROP_DamageMultiplier:
+ 		actor->DamageMultiply = value;
+ 		break;
+ 
  	case APROP_MasterTID:
  		AActor *other;
  		other = SingleActorFromTID (value, NULL);
***************
*** 3837,3842 ****
--- 3905,3914 ----
  		actor->reactiontime = value;
  		break;
  
+ 	case APROP_MeleeRange:
+ 		actor->meleerange = value;
+ 		break;
+ 
  	case APROP_ViewHeight:
  		if (actor->IsKindOf (RUNTIME_CLASS (APlayerPawn)))
  			static_cast<APlayerPawn *>(actor)->ViewHeight = value;
***************
*** 3874,3879 ****
--- 3946,3952 ----
  	case APROP_Speed:		return actor->Speed;
  	case APROP_Damage:		return actor->Damage;	// Should this call GetMissileDamage() instead?
  	case APROP_DamageFactor:return actor->DamageFactor;
+ 	case APROP_DamageMultiplier: return actor->DamageMultiply;
  	case APROP_Alpha:		return actor->alpha;
  	case APROP_RenderStyle:	for (int style = STYLE_None; style < STYLE_Count; ++style)
  							{ // Check for a legacy render style that matches.
***************
*** 4218,4224 ****
  		{
  			classify |= ACTOR_VOODOODOLL;
  		}
! 		if (actor->player->isbot)
  		{
  			classify |= ACTOR_BOT;
  		}
--- 4291,4297 ----
  		{
  			classify |= ACTOR_VOODOODOLL;
  		}
! 		if (actor->player->Bot != NULL)
  		{
  			classify |= ACTOR_BOT;
  		}
***************
*** 4361,4366 ****
--- 4434,4448 ----
  	ACSF_ChangeActorAngle,
  	ACSF_ChangeActorPitch,		// 80
  	ACSF_GetArmorInfo,
+ 	ACSF_DropInventory,
+ 	ACSF_PickActor,
+ 	ACSF_IsPointerEqual,
+ 	ACSF_CanRaiseActor,
+ 	ACSF_SetActorTeleFog,		// 86
+ 	ACSF_SwapActorTeleFog,
+ 	ACSF_SetActorRoll,
+ 	ACSF_ChangeActorRoll,
+ 	ACSF_GetActorRoll,
  
  	/* Zandronum's - these must be skipped when we reach 99!
  	-100:ResetMap(0),
***************
*** 4672,4677 ****
--- 4754,4856 ----
  	}
  }
  
+ static void SetActorRoll(AActor *activator, int tid, int angle, bool interpolate)
+ {
+ 	if (tid == 0)
+ 	{
+ 		if (activator != NULL)
+ 		{
+ 			activator->SetRoll(angle << 16, interpolate);
+ 		}
+ 	}
+ 	else
+ 	{
+ 		FActorIterator iterator(tid);
+ 		AActor *actor;
+ 
+ 		while ((actor = iterator.Next()))
+ 		{
+ 			actor->SetRoll(angle << 16, interpolate);
+ 		}
+ 	}
+ }
+ 
+ static void SetActorTeleFog(AActor *activator, int tid, FName telefogsrc, FName telefogdest)
+ {
+ 	//Simply put, if it doesn't exist, it won't change. One can use "" in this scenario.
+ 	const PClass *check;
+ 	if (tid == 0)
+ 	{
+ 		if (activator != NULL)
+ 		{
+ 			check = PClass::FindClass(telefogsrc);
+ 			if (check == NULL || !stricmp(telefogsrc, "none") || !stricmp(telefogsrc, "null"))
+ 				activator->TeleFogSourceType = NULL;
+ 			else
+ 				activator->TeleFogSourceType = check;
+ 
+ 			check = PClass::FindClass(telefogdest);
+ 			if (check == NULL || !stricmp(telefogdest, "none") || !stricmp(telefogdest, "null"))
+ 				activator->TeleFogDestType = NULL;
+ 			else
+ 				activator->TeleFogDestType = check;
+ 		}
+ 	}
+ 	else
+ 	{
+ 		FActorIterator iterator(tid);
+ 		AActor *actor;
+ 
+ 		while ((actor = iterator.Next()))
+ 		{
+ 			check = PClass::FindClass(telefogsrc);
+ 			if (check == NULL || !stricmp(telefogsrc, "none") || !stricmp(telefogsrc, "null"))
+ 				actor->TeleFogSourceType = NULL;
+ 			else
+ 				actor->TeleFogSourceType = check;
+ 
+ 			check = PClass::FindClass(telefogdest);
+ 			if (check == NULL || !stricmp(telefogdest, "none") || !stricmp(telefogdest, "null"))
+ 				actor->TeleFogDestType = NULL;
+ 			else
+ 				actor->TeleFogDestType = check;
+ 		}
+ 	}
+ }
+ 
+ static int SwapActorTeleFog(AActor *activator, int tid)
+ {
+ 	int count = 0;
+ 	if (tid == 0)
+ 	{
+ 		if ((activator == NULL) || (activator->TeleFogSourceType = activator->TeleFogDestType)) 
+ 			return 0; //Does nothing if they're the same.
+ 		else 
+ 		{
+ 			const PClass *temp = activator->TeleFogSourceType;
+ 			activator->TeleFogSourceType = activator->TeleFogDestType;
+ 			activator->TeleFogDestType = temp;
+ 			return 1;
+ 		}
+ 	}
+ 	else
+ 	{
+ 		FActorIterator iterator(tid);
+ 		AActor *actor;
+ 		
+ 		while ((actor = iterator.Next()))
+ 		{
+ 			if (actor->TeleFogSourceType == actor->TeleFogDestType) 
+ 				continue; //They're the same. Save the effort.
+ 			const PClass *temp = actor->TeleFogSourceType;
+ 			actor->TeleFogSourceType = actor->TeleFogDestType;
+ 			actor->TeleFogDestType = temp;
+ 			count++;
+ 		}
+ 	}
+ 	return count;
+ }
+ 
  
  
  int DLevelScript::CallFunction(int argCount, int funcIndex, SDWORD *args, const SDWORD *stack, int stackdepth)
***************
*** 5485,5490 ****
--- 5664,5705 ----
  			break;
  		}
  
+ 		case ACSF_DropInventory:
+ 		{
+ 			const char *type = FBehavior::StaticLookupString(args[1]);
+ 			AInventory *inv;
+ 			
+ 			if (type != NULL)
+ 			{
+ 				if (args[0] == 0)
+ 				{
+ 					if (activator != NULL)
+ 					{
+ 						inv = activator->FindInventory(type);
+ 						if (inv)
+ 						{
+ 							activator->DropInventory(inv);
+ 						}
+ 					}
+ 				}
+ 				else
+ 				{
+ 					FActorIterator it(args[0]);
+ 					AActor *actor;
+ 					
+ 					while ((actor = it.Next()) != NULL)
+ 					{
+ 						inv = actor->FindInventory(type);
+ 						if (inv)
+ 						{
+ 							actor->DropInventory(inv);
+ 						}
+ 					}
+ 				}
+ 			}
+ 		break;
+ 		}
+ 
  		case ACSF_CheckFlag:
  		{
  			AActor *actor = SingleActorFromTID(args[0], activator);
***************
*** 5549,5554 ****
--- 5764,5880 ----
  				SetActorPitch(activator, args[0], args[1], argCount > 2 ? !!args[2] : false);
  			}
  			break;
+ 		case ACSF_SetActorTeleFog:
+ 			if (argCount >= 3)
+ 			{
+ 				SetActorTeleFog(activator, args[0], FBehavior::StaticLookupString(args[1]), FBehavior::StaticLookupString(args[2]));
+ 			}
+ 			break;
+ 		case ACSF_SwapActorTeleFog:
+ 			if (argCount >= 1)
+ 			{
+ 				return SwapActorTeleFog(activator, args[0]);
+ 			}
+ 			break;
+ 		case ACSF_PickActor:
+ 			if (argCount >= 5)
+ 			{
+ 				actor = SingleActorFromTID(args[0], activator);
+ 				if (actor == NULL)
+ 				{
+ 					return 0;
+ 				}
+ 
+ 				DWORD actorMask = MF_SHOOTABLE;
+ 				if (argCount >= 6) {
+ 					actorMask = args[5];
+ 				}
+ 
+ 				DWORD wallMask = ML_BLOCKEVERYTHING | ML_BLOCKHITSCAN;
+ 				if (argCount >= 7) {
+ 					wallMask = args[6];
+ 				}
+ 
+ 				bool forceTID = 0;
+ 				if (argCount >= 8)
+ 				{
+ 					if (args[7] != 0)
+ 						forceTID = 1;
+ 				}
+ 
+ 				AActor* pickedActor = P_LinePickActor(actor, args[1] << 16, args[3], args[2] << 16, actorMask, wallMask);
+ 				if (pickedActor == NULL) {
+ 					return 0;
+ 				}
+ 
+ 				if (!(forceTID) && (args[4] == 0) && (pickedActor->tid == 0))
+ 					return 0;
+ 
+ 				if ((pickedActor->tid == 0) || (forceTID))
+ 				{
+ 					pickedActor->RemoveFromHash();
+ 					pickedActor->tid = args[4];
+ 					pickedActor->AddToHash();
+ 				}
+ 				return 1;
+ 			}
+ 			break;
+ 
+ 		case ACSF_IsPointerEqual:
+ 			{
+ 				int tid1 = 0, tid2 = 0;
+ 				switch (argCount)
+ 				{
+ 				case 4: tid2 = args[3];
+ 				case 3: tid1 = args[2];
+ 				}
+ 
+ 				actor = SingleActorFromTID(tid1, activator);
+ 				AActor * actor2 = tid2 == tid1 ? actor : SingleActorFromTID(tid2, activator);
+ 
+ 				return COPY_AAPTR(actor, args[0]) == COPY_AAPTR(actor2, args[1]);
+ 			}
+ 			break;
+ 
+ 		case ACSF_CanRaiseActor:
+ 			if (argCount >= 1) {
+ 				if (args[0] == 0) {
+ 					actor = SingleActorFromTID(args[0], activator);
+ 					if (actor != NULL) {
+ 						return P_Thing_CanRaise(actor);
+ 					}
+ 				}
+ 
+ 				FActorIterator iterator(args[0]);
+ 				bool canraiseall = false;
+ 				while ((actor = iterator.Next()))
+ 				{
+ 					canraiseall = !P_Thing_CanRaise(actor) | canraiseall;
+ 				}
+ 				
+ 				return !canraiseall;
+ 			}
+ 			break;
+ 
+ 		// [Nash] Actor roll functions. Let's roll!
+ 		case ACSF_SetActorRoll:
+ 			actor = SingleActorFromTID(args[0], activator);
+ 			if (actor != NULL)
+ 			{
+ 				actor->SetRoll(args[1] << 16, false);
+ 			}
+ 			return 0;
+ 
+ 		case ACSF_ChangeActorRoll:
+ 			if (argCount >= 2)
+ 			{
+ 				SetActorRoll(activator, args[0], args[1], argCount > 2 ? !!args[2] : false);
+ 			}
+ 			break;
+ 
+ 		case ACSF_GetActorRoll:
+ 			actor = SingleActorFromTID(args[0], activator);
+ 			return actor != NULL? actor->roll >> 16 : 0;
  
  		default:
  			break;
***************
*** 7166,7172 ****
  				while (min <= max)
  				{
  					int mid = (min + max) / 2;
! 					SDWORD caseval = pc[mid*2];
  					if (caseval == STACK(1))
  					{
  						pc = activeBehavior->Ofs2PC (LittleLong(pc[mid*2+1]));
--- 7492,7498 ----
  				while (min <= max)
  				{
  					int mid = (min + max) / 2;
! 					SDWORD caseval = LittleLong(pc[mid*2]);
  					if (caseval == STACK(1))
  					{
  						pc = activeBehavior->Ofs2PC (LittleLong(pc[mid*2+1]));
***************
*** 7214,7235 ****
  			break;
  
  		case PCD_PRINTBINARY:
! #if (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ >= 6)))) || defined(__clang__)
! #define HAS_DIAGNOSTIC_PRAGMA
! #endif
! #ifdef HAS_DIAGNOSTIC_PRAGMA
! #pragma GCC diagnostic push
! #ifdef __clang__
! #pragma GCC diagnostic ignored "-Wformat-invalid-specifier"
! #else
! #pragma GCC diagnostic ignored "-Wformat="
! #endif
! #pragma GCC diagnostic ignored "-Wformat-extra-args"
! #endif
  			work.AppendFormat ("%B", STACK(1));
! #ifdef HAS_DIAGNOSTIC_PRAGMA
! #pragma GCC diagnostic pop
! #endif
  			--sp;
  			break;
  
--- 7540,7548 ----
  			break;
  
  		case PCD_PRINTBINARY:
! 			IGNORE_FORMAT_PRE
  			work.AppendFormat ("%B", STACK(1));
! 			IGNORE_FORMAT_POST
  			--sp;
  			break;
  
***************
*** 7532,7544 ****
  						AddToConsole (-1, consolecolor);
  						AddToConsole (-1, work);
  						AddToConsole (-1, bar);
- 						if (Logfile)
- 						{
- 							fputs (logbar, Logfile);
- 							fputs (work, Logfile);
- 							fputs (logbar, Logfile);
- 							fflush (Logfile);
- 						}
  					}
  				}
  			}
--- 7845,7850 ----
***************
*** 8353,8363 ****
--- 8659,8677 ----
  			break;
  
  		case PCD_SIN:
+ #ifdef __ANDROID__
+ 			STACK(1) = finesine[angle_t((int)(STACK(1)<<16))>>ANGLETOFINESHIFT];
+ #else
  			STACK(1) = finesine[angle_t(STACK(1)<<16)>>ANGLETOFINESHIFT];
+ #endif
  			break;
  
  		case PCD_COS:
+ #ifdef __ANDROID__
+ 			STACK(1) = finecosine[angle_t((int)(STACK(1)<<16))>>ANGLETOFINESHIFT];
+ #else
  			STACK(1) = finecosine[angle_t(STACK(1)<<16)>>ANGLETOFINESHIFT];
+ #endif
  			break;
  
  		case PCD_VECTORANGLE:
***************
*** 8514,8520 ****
  			}
  			else
  			{
! 				STACK(1) = players[STACK(1)].isbot;
  			}
  			break;
  
--- 8828,8834 ----
  			}
  			else
  			{
! 				STACK(1) = (players[STACK(1)].Bot != NULL);
  			}
  			break;
  
