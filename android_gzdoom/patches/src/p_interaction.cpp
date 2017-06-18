*** /home/draje/Code/gzdoom/src/p_interaction.cpp	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_interaction.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 100,109 ****
  		return;
  
  	//Added by MC: Finished with this destination.
! 	if (toucher->player != NULL && toucher->player->isbot && special == toucher->player->dest)
  	{
! 		toucher->player->prev = toucher->player->dest;
!     	toucher->player->dest = NULL;
  	}
  
  	special->Touch (toucher);
--- 100,109 ----
  		return;
  
  	//Added by MC: Finished with this destination.
! 	if (toucher->player != NULL && toucher->player->Bot != NULL && special == toucher->player->Bot->dest)
  	{
! 		toucher->player->Bot->prev = toucher->player->Bot->dest;
! 		toucher->player->Bot->dest = NULL;
  	}
  
  	special->Touch (toucher);
***************
*** 593,599 ****
  		// even those caused by other monsters
  		players[0].killcount++;
  	}
! 	
  	if (player)
  	{
  		// [RH] Death messages
--- 593,599 ----
  		// even those caused by other monsters
  		players[0].killcount++;
  	}
! 
  	if (player)
  	{
  		// [RH] Death messages
***************
*** 606,624 ****
  		player->respawn_time = level.time + TICRATE;
  
  		//Added by MC: Respawn bots
! 		if (bglobal.botnum && consoleplayer == Net_Arbitrator && !demoplayback)
  		{
! 			if (player->isbot)
! 				player->t_respawn = (pr_botrespawn()%15)+((bglobal.botnum-1)*2)+TICRATE+1;
  
  			//Added by MC: Discard enemies.
  			for (int i = 0; i < MAXPLAYERS; i++)
  			{
! 				if (players[i].isbot && this == players[i].enemy)
  				{
! 					if (players[i].dest ==  players[i].enemy)
! 						players[i].dest = NULL;
! 					players[i].enemy = NULL;
  				}
  			}
  
--- 606,624 ----
  		player->respawn_time = level.time + TICRATE;
  
  		//Added by MC: Respawn bots
! 		if (bglobal.botnum && !demoplayback)
  		{
! 			if (player->Bot != NULL)
! 				player->Bot->t_respawn = (pr_botrespawn()%15)+((bglobal.botnum-1)*2)+TICRATE+1;
  
  			//Added by MC: Discard enemies.
  			for (int i = 0; i < MAXPLAYERS; i++)
  			{
! 				if (players[i].Bot != NULL && this == players[i].Bot->enemy)
  				{
! 					if (players[i].Bot->dest ==  players[i].Bot->enemy)
! 						players[i].Bot->dest = NULL;
! 					players[i].Bot->enemy = NULL;
  				}
  			}
  
***************
*** 925,930 ****
--- 925,935 ----
  		(inflictor->flags6 & MF6_FORCEPAIN) && !(inflictor->flags5 & MF5_PAINLESS));
  }
  
+ static inline bool isFakePain(AActor *target, AActor *inflictor)
+ {
+ 	return ((target->flags7 & MF7_ALLOWPAIN) || ((inflictor != NULL) && (inflictor->flags7 & MF7_CAUSEPAIN)));
+ }
+ 
  
  // Returns the amount of damage actually inflicted upon the target, or -1 if
  // the damage was cancelled.
***************
*** 938,949 ****
  	FState * woundstate = NULL;
  	PainChanceList * pc = NULL;
  	bool justhit = false;
! 
  	if (target == NULL || !((target->flags & MF_SHOOTABLE) || (target->flags6 & MF6_VULNERABLE)))
  	{ // Shouldn't happen
  		return -1;
  	}
  
  	// Spectral targets only take damage from spectral projectiles.
  	if (target->flags4 & MF4_SPECTRAL && damage < TELEFRAG_DAMAGE)
  	{
--- 943,964 ----
  	FState * woundstate = NULL;
  	PainChanceList * pc = NULL;
  	bool justhit = false;
! 	bool plrDontThrust = false;
! 	bool invulpain = false;
! 	bool fakedPain = false;
! 	bool forcedPain = false;
! 	int fakeDamage = 0;
! 	int holdDamage = 0;
! 	
  	if (target == NULL || !((target->flags & MF_SHOOTABLE) || (target->flags6 & MF6_VULNERABLE)))
  	{ // Shouldn't happen
  		return -1;
  	}
  
+ 	//Rather than unnecessarily call the function over and over again, let's be a little more efficient.
+ 	fakedPain = (isFakePain(target, inflictor)); 
+ 	forcedPain = (MustForcePain(target, inflictor));
+ 
  	// Spectral targets only take damage from spectral projectiles.
  	if (target->flags4 & MF4_SPECTRAL && damage < TELEFRAG_DAMAGE)
  	{
***************
*** 966,978 ****
  		}
  		return -1;
  	}
! 	if ((target->flags2 & MF2_INVULNERABLE) && damage < TELEFRAG_DAMAGE && !(flags & DMG_FORCED))
  	{ // actor is invulnerable
  		if (target->player == NULL)
  		{
  			if (inflictor == NULL || (!(inflictor->flags3 & MF3_FOILINVUL) && !(flags & DMG_FOILINVUL)))
  			{
! 				return -1;
  			}
  		}
  		else
--- 981,1000 ----
  		}
  		return -1;
  	}
! 	if ((target->flags2 & MF2_INVULNERABLE) && (damage < TELEFRAG_DAMAGE) && (!(flags & DMG_FORCED)))
  	{ // actor is invulnerable
  		if (target->player == NULL)
  		{
  			if (inflictor == NULL || (!(inflictor->flags3 & MF3_FOILINVUL) && !(flags & DMG_FOILINVUL)))
  			{
! 				if (fakedPain)
! 				{
! 					invulpain = true; //This returns -1 later.
! 					fakeDamage = damage; 
! 					goto fakepain; //The label is above the massive pile of checks.
! 				}
! 				else
! 					return -1;
  			}
  		}
  		else
***************
*** 980,990 ****
  			// Players are optionally excluded from getting thrust by damage.
  			if (static_cast<APlayerPawn *>(target)->PlayerFlags & PPF_NOTHRUSTWHENINVUL)
  			{
! 				return -1;
  			}
  		}
  		
  	}
  	if (inflictor != NULL)
  	{
  		if (inflictor->flags5 & MF5_PIERCEARMOR)
--- 1002,1022 ----
  			// Players are optionally excluded from getting thrust by damage.
  			if (static_cast<APlayerPawn *>(target)->PlayerFlags & PPF_NOTHRUSTWHENINVUL)
  			{
! 				if (fakedPain)
! 					plrDontThrust = 1;
! 				else
! 					return -1;
  			}
  		}
  		
  	}
+ 	if ((fakedPain) && (damage < TELEFRAG_DAMAGE))
+ 	{
+ 		//Intentionally do not jump to fakepain because the damage hasn't been dished out yet.
+ 		//Once it's dished out, THEN we can disregard damage factors affecting pain chances.
+ 		fakeDamage = damage;
+ 	}
+ 
  	if (inflictor != NULL)
  	{
  		if (inflictor->flags5 & MF5_PIERCEARMOR)
***************
*** 1010,1015 ****
--- 1042,1048 ----
  			// Invulnerable, and won't wake up
  			return -1;
  		}
+ 
  		player = target->player;
  		if (player && damage > 1 && damage < TELEFRAG_DAMAGE)
  		{
***************
*** 1032,1069 ****
  						return -1;
  				}
  			}
  
! 			damage = inflictor->DoSpecialDamage (target, damage, mod);
! 			if (damage == -1)
  			{
  				return -1;
  			}
  		}
  		// Handle active damage modifiers (e.g. PowerDamage)
! 		if (source != NULL && source->Inventory != NULL)
  		{
  			int olddam = damage;
! 			source->Inventory->ModifyDamage(olddam, mod, damage, false);
! 			if (olddam != damage && damage <= 0)
  			{ // Still allow FORCEPAIN
! 				if (MustForcePain(target, inflictor))
! 				{
  					goto dopain;
! 				}
  				return -1;
  			}
  		}
! 		// Handle passive damage modifiers (e.g. PowerProtection)
! 		if (target->Inventory != NULL)
  		{
  			int olddam = damage;
  			target->Inventory->ModifyDamage(olddam, mod, damage, true);
! 			if (olddam != damage && damage <= 0)
! 			{ // Still allow FORCEPAIN
! 				if (MustForcePain(target, inflictor))
! 				{
  					goto dopain;
! 				}
  				return -1;
  			}
  		}
--- 1065,1113 ----
  						return -1;
  				}
  			}
+ 			if (damage > 0)
+ 				damage = inflictor->DoSpecialDamage (target, damage, mod);
  
! 			if ((damage == -1) && (target->player == NULL)) //This isn't meant for the player.
  			{
+ 				if (fakedPain) //Hold off ending the function before we can deal the pain chances.
+ 					goto fakepain;
  				return -1;
  			}
  		}
  		// Handle active damage modifiers (e.g. PowerDamage)
! 		if (source != NULL)
  		{
  			int olddam = damage;
! 
! 			if (source->Inventory != NULL)
! 			{
! 				source->Inventory->ModifyDamage(olddam, mod, damage, false);
! 			}
! 			damage = FixedMul(damage, source->DamageMultiply);
! 
! 			if (((source->flags7 & MF7_CAUSEPAIN) && (fakeDamage <= 0)) || (olddam != damage && damage <= 0))
  			{ // Still allow FORCEPAIN
! 				if (forcedPain)
  					goto dopain;
! 				else if (fakedPain)
! 					goto fakepain;
! 
  				return -1;
  			}
  		}
! 		// Handle passive damage modifiers (e.g. PowerProtection), provided they are not afflicted with protection penetrating powers.
! 		if ((target->Inventory != NULL) && !(flags & DMG_NO_PROTECT))
  		{
  			int olddam = damage;
  			target->Inventory->ModifyDamage(olddam, mod, damage, true);
! 			if ((olddam != damage && damage <= 0) && target->player == NULL)
! 			{ // Still allow FORCEPAIN and make sure we're still passing along fake damage to hit enemies for their pain states.
! 				if (forcedPain)
  					goto dopain;
! 				else if (fakedPain)
! 					goto fakepain;
! 
  				return -1;
  			}
  		}
***************
*** 1071,1102 ****
  		if (!(flags & DMG_NO_FACTOR))
  		{
  			damage = FixedMul(damage, target->DamageFactor);
! 			if (damage >= 0)
  			{
  				damage = DamageTypeDefinition::ApplyMobjDamageFactor(damage, mod, target->GetClass()->ActorInfo->DamageFactors);
  			}
! 			if (damage <= 0)
  			{ // Still allow FORCEPAIN
! 				if (MustForcePain(target, inflictor))
! 				{
  					goto dopain;
! 				}
  				return -1;
  			}
  		}
! 
! 		damage = target->TakeSpecialDamage (inflictor, source, damage, mod);
  	}
! 	if (damage == -1)
  	{
  		return -1;
  	}
  	// Push the target unless the source's weapon's kickback is 0.
  	// (i.e. Gauntlets/Chainsaw)
! 	if (inflictor && inflictor != target	// [RH] Not if hurting own self
  		&& !(target->flags & MF_NOCLIP)
  		&& !(inflictor->flags2 & MF2_NODMGTHRUST)
  		&& !(flags & DMG_THRUSTLESS)
  		&& (source == NULL || source->player == NULL || !(source->flags2 & MF2_NODMGTHRUST)))
  	{
  		int kickback;
--- 1115,1151 ----
  		if (!(flags & DMG_NO_FACTOR))
  		{
  			damage = FixedMul(damage, target->DamageFactor);
! 			if (damage > 0)
  			{
  				damage = DamageTypeDefinition::ApplyMobjDamageFactor(damage, mod, target->GetClass()->ActorInfo->DamageFactors);
  			}
! 			if (damage <= 0 && target->player == NULL)
  			{ // Still allow FORCEPAIN
! 				if (forcedPain)
  					goto dopain;
! 				else if (fakedPain)
! 					goto fakepain;
! 
  				return -1;
  			}
  		}
! 		if (damage > 0)
! 			damage = target->TakeSpecialDamage (inflictor, source, damage, mod);
  	}
! 	if (damage == -1 && target->player == NULL) //Make sure it's not a player, the pain has yet to be processed with cheats.
  	{
+ 		if (fakedPain)
+ 			goto fakepain;
+ 
  		return -1;
  	}
  	// Push the target unless the source's weapon's kickback is 0.
  	// (i.e. Gauntlets/Chainsaw)
! 	if (!(plrDontThrust) && inflictor && inflictor != target	// [RH] Not if hurting own self
  		&& !(target->flags & MF_NOCLIP)
  		&& !(inflictor->flags2 & MF2_NODMGTHRUST)
  		&& !(flags & DMG_THRUSTLESS)
+ 		&& !(target->flags7 & MF7_DONTTHRUST)
  		&& (source == NULL || source->player == NULL || !(source->flags2 & MF2_NODMGTHRUST)))
  	{
  		int kickback;
***************
*** 1133,1143 ****
              {
                  fltthrust = clamp((damage * 0.125 * kickback) / target->Mass, 0., fltthrust);
              }
- 
  			thrust = FLOAT2FIXED(fltthrust);
! 
! 			// Don't apply ultra-small damage thrust
! 			if (thrust < FRACUNIT/100) thrust = 0;
  
  			// make fall forwards sometimes
  			if ((damage < 40) && (damage > target->health)
--- 1182,1191 ----
              {
                  fltthrust = clamp((damage * 0.125 * kickback) / target->Mass, 0., fltthrust);
              }
  			thrust = FLOAT2FIXED(fltthrust);
! 			// Don't apply ultra-small damage thrust.
! 			if (thrust < FRACUNIT / 100)
! 				thrust = 0;
  
  			// make fall forwards sometimes
  			if ((damage < 40) && (damage > target->health)
***************
*** 1146,1153 ****
  				 // [RH] But only if not too fast and not flying
  				 && thrust < 10*FRACUNIT
  				 && !(target->flags & MF_NOGRAVITY)
! 				 && (inflictor == NULL || !(inflictor->flags5 & MF5_NOFORWARDFALL))
! 				 )
  			{
  				ang += ANG180;
  				thrust *= 4;
--- 1194,1200 ----
  				 // [RH] But only if not too fast and not flying
  				 && thrust < 10*FRACUNIT
  				 && !(target->flags & MF_NOGRAVITY)
! 				 && (inflictor == NULL || !(inflictor->flags5 & MF5_NOFORWARDFALL)))
  			{
  				ang += ANG180;
  				thrust *= 4;
***************
*** 1193,1203 ****
  	//
  	if (player)
  	{
! 		
!         //Added by MC: Lets bots look allround for enemies if they survive an ambush.
!         if (player->isbot)
  		{
!             player->allround = true;
  		}
  
  		// end of game hell hack
--- 1240,1249 ----
  	//
  	if (player)
  	{
! 		//Added by MC: Lets bots look allround for enemies if they survive an ambush.
! 		if (player->Bot != NULL)
  		{
! 			player->Bot->allround = true;
  		}
  
  		// end of game hell hack
***************
*** 1210,1231 ****
  		if (!(flags & DMG_FORCED))
  		{
  			// check the real player, not a voodoo doll here for invulnerability effects
! 			if (damage < TELEFRAG_DAMAGE && ((player->mo->flags2 & MF2_INVULNERABLE) ||
! 				(player->cheats & CF_GODMODE)))
  			{ // player is invulnerable, so don't hurt him
! 				return -1;
  			}
  
  			if (!(flags & DMG_NO_ARMOR) && player->mo->Inventory != NULL)
  			{
  				int newdam = damage;
! 				player->mo->Inventory->AbsorbDamage (damage, mod, newdam);
! 				damage = newdam;
  				if (damage <= 0)
  				{
  					// If MF6_FORCEPAIN is set, make the player enter the pain state.
  					if (!(target->flags5 & MF5_NOPAIN) && inflictor != NULL &&
! 						(inflictor->flags6 & MF6_FORCEPAIN) && !(inflictor->flags5 & MF5_PAINLESS))
  					{
  						goto dopain;
  					}
--- 1256,1299 ----
  		if (!(flags & DMG_FORCED))
  		{
  			// check the real player, not a voodoo doll here for invulnerability effects
! 			if ((damage < TELEFRAG_DAMAGE && ((player->mo->flags2 & MF2_INVULNERABLE) ||
! 				(player->cheats & CF_GODMODE))) ||
! 				(player->cheats & CF_GODMODE2) || (player->mo->flags5 & MF5_NODAMAGE))
! 				//Absolutely no hurting if NODAMAGE is involved. Same for GODMODE2.
  			{ // player is invulnerable, so don't hurt him
! 				//Make sure no godmodes and NOPAIN flags are found first.
! 				//Then, check to see if the player has NODAMAGE or ALLOWPAIN, or inflictor has CAUSEPAIN.
! 				if ((player->cheats & CF_GODMODE) || (player->cheats & CF_GODMODE2) || (player->mo->flags5 & MF5_NOPAIN))
! 					return -1;
! 				else if ((((player->mo->flags7 & MF7_ALLOWPAIN) || (player->mo->flags5 & MF5_NODAMAGE)) || ((inflictor != NULL) && (inflictor->flags7 & MF7_CAUSEPAIN))))
! 				{
! 					invulpain = true;
! 					fakeDamage = damage;
! 					goto fakepain;
! 				}
! 				else
! 					return -1;
  			}
  
  			if (!(flags & DMG_NO_ARMOR) && player->mo->Inventory != NULL)
  			{
  				int newdam = damage;
! 				if (damage > 0)
! 				{
! 					player->mo->Inventory->AbsorbDamage(damage, mod, newdam);
! 				}
! 				if (damage < TELEFRAG_DAMAGE)
! 				{
! 					// if we are telefragging don't let the damage value go below that magic value. Some further checks would fail otherwise.
! 					damage = newdam;
! 				}
! 
  				if (damage <= 0)
  				{
  					// If MF6_FORCEPAIN is set, make the player enter the pain state.
  					if (!(target->flags5 & MF5_NOPAIN) && inflictor != NULL &&
! 						(inflictor->flags6 & MF6_FORCEPAIN) && !(inflictor->flags5 & MF5_PAINLESS) 
! 						&& (!(player->mo->flags2 & MF2_INVULNERABLE)) && (!(player->cheats & CF_GODMODE)) && (!(player->cheats & CF_GODMODE2)))
  					{
  						goto dopain;
  					}
***************
*** 1233,1239 ****
  				}
  			}
  			
! 			if (damage >= player->health
  				&& (G_SkillProperty(SKILLP_AutoUseHealth) || deathmatch)
  				&& !player->morphTics)
  			{ // Try to use some inventory health
--- 1301,1307 ----
  				}
  			}
  			
! 			if (damage >= player->health && damage < TELEFRAG_DAMAGE
  				&& (G_SkillProperty(SKILLP_AutoUseHealth) || deathmatch)
  				&& !player->morphTics)
  			{ // Try to use some inventory health
***************
*** 1255,1263 ****
  			// This does not save the player if damage >= TELEFRAG_DAMAGE, still need to
  			// telefrag him right? ;) (Unfortunately the damage is "absorbed" by armor,
  			// but telefragging should still do enough damage to kill the player)
! 			if ((player->cheats & CF_BUDDHA) && damage < TELEFRAG_DAMAGE
! 				// Ignore players that are already dead.
! 				&& player->playerstate != PST_DEAD)
  			{
  				// If this is a voodoo doll we need to handle the real player as well.
  				player->mo->health = target->health = player->health = 1;
--- 1323,1331 ----
  			// This does not save the player if damage >= TELEFRAG_DAMAGE, still need to
  			// telefrag him right? ;) (Unfortunately the damage is "absorbed" by armor,
  			// but telefragging should still do enough damage to kill the player)
! 			// Ignore players that are already dead.
! 			// [MC]Buddha2 absorbs telefrag damage, and anything else thrown their way.
! 			if ((player->cheats & CF_BUDDHA2) || (((player->cheats & CF_BUDDHA) || (player->mo->flags7 & MF7_BUDDHA)) && (damage < TELEFRAG_DAMAGE)) && (player->playerstate != PST_DEAD))
  			{
  				// If this is a voodoo doll we need to handle the real player as well.
  				player->mo->health = target->health = player->health = 1;
***************
*** 1290,1296 ****
  			damage = newdam;
  			if (damage <= 0)
  			{
! 				return damage;
  			}
  		}
  	
--- 1358,1367 ----
  			damage = newdam;
  			if (damage <= 0)
  			{
! 				if (fakedPain)
! 					goto fakepain;
! 				else
! 					return damage;
  			}
  		}
  	
***************
*** 1317,1359 ****
  
  
  	if (target->health <= 0)
! 	{ // Death
! 		target->special1 = damage;
  
! 		// use inflictor's death type if it got one.
! 		if (inflictor && inflictor->DeathType != NAME_None) mod = inflictor->DeathType;
  
! 		// check for special fire damage or ice damage deaths
! 		if (mod == NAME_Fire)
! 		{
! 			if (player && !player->morphTics)
! 			{ // Check for flame death
! 				if (!inflictor ||
! 					((target->health > -50) && (damage > 25)) ||
! 					!(inflictor->flags5 & MF5_SPECIALFIREDAMAGE))
  				{
  					target->DamageType = NAME_Fire;
  				}
  			}
  			else
  			{
! 				target->DamageType = NAME_Fire;
  			}
! 		}
! 		else
! 		{
! 			target->DamageType = mod;
! 		}
! 		if (source && source->tracer && (source->flags5 & MF5_SUMMONEDMONSTER))
! 		{ // Minotaur's kills go to his master
! 			// Make sure still alive and not a pointer to fighter head
! 			if (source->tracer->player && (source->tracer->player->mo == source->tracer))
! 			{
! 				source = source->tracer;
  			}
  		}
- 		target->Die (source, inflictor, flags);
- 		return damage;
  	}
  
  	woundstate = target->FindState(NAME_Wound, mod);
--- 1388,1440 ----
  
  
  	if (target->health <= 0)
! 	{ 
! 		//[MC]Buddha flag for monsters.
! 		if ((target->flags7 & MF7_BUDDHA) && (damage < TELEFRAG_DAMAGE) && ((inflictor == NULL || !(inflictor->flags3 & MF7_FOILBUDDHA)) && !(flags & DMG_FOILBUDDHA)))
! 		{ //FOILBUDDHA or Telefrag damage must kill it.
! 			target->health = 1;
! 		}
! 		else
! 		{
! 		
! 			// Death
! 			target->special1 = damage;
  
! 			// use inflictor's death type if it got one.
! 			if (inflictor && inflictor->DeathType != NAME_None) mod = inflictor->DeathType;
  
! 			// check for special fire damage or ice damage deaths
! 			if (mod == NAME_Fire)
! 			{
! 				if (player && !player->morphTics)
! 				{ // Check for flame death
! 					if (!inflictor ||
! 						((target->health > -50) && (damage > 25)) ||
! 						!(inflictor->flags5 & MF5_SPECIALFIREDAMAGE))
! 					{
! 						target->DamageType = NAME_Fire;
! 					}
! 				}
! 				else
  				{
  					target->DamageType = NAME_Fire;
  				}
  			}
  			else
  			{
! 				target->DamageType = mod;
  			}
! 			if (source && source->tracer && (source->flags5 & MF5_SUMMONEDMONSTER))
! 			{ // Minotaur's kills go to his master
! 				// Make sure still alive and not a pointer to fighter head
! 				if (source->tracer->player && (source->tracer->player->mo == source->tracer))
! 				{
! 					source = source->tracer;
! 				}
  			}
+ 			target->Die (source, inflictor, flags);
+ 			return damage;
  		}
  	}
  
  	woundstate = target->FindState(NAME_Wound, mod);
***************
*** 1368,1373 ****
--- 1449,1464 ----
  		}
  	}
  
+ fakepain: //Needed so we can skip the rest of the above, but still obey the original rules.
+ 
+ 	//CAUSEPAIN can always attempt to trigger the chances of pain.
+ 	//ALLOWPAIN can do the same, only if the (unfiltered aka fake) damage is greater than 0. 
+ 	if ((((target->flags7 & MF7_ALLOWPAIN) && (fakeDamage > 0))
+ 		|| ((inflictor != NULL) && (inflictor->flags7 & MF7_CAUSEPAIN))))
+ 	{
+ 		holdDamage = damage;	//Store the modified damage away after factors are taken into account.
+ 		damage = fakeDamage;	//Retrieve the original damage.
+ 	}
  	
  	if (!(target->flags5 & MF5_NOPAIN) && (inflictor == NULL || !(inflictor->flags5 & MF5_PAINLESS)) &&
  		(target->player != NULL || !G_SkillProperty(SKILLP_NoPain)) && !(target->flags & MF_SKULLFLY))
***************
*** 1383,1390 ****
  			}
  		}
  
! 		if ((damage >= target->PainThreshold && pr_damagemobj() < painchance) ||
! 			(inflictor != NULL && (inflictor->flags6 & MF6_FORCEPAIN)))
  		{
  dopain:	
  			if (mod == NAME_Electric)
--- 1474,1481 ----
  			}
  		}
  
! 		if ((((damage >= target->PainThreshold)) && (pr_damagemobj() < painchance)) 
! 			|| (inflictor != NULL && (inflictor->flags6 & MF6_FORCEPAIN)))
  		{
  dopain:	
  			if (mod == NAME_Electric)
***************
*** 1421,1426 ****
--- 1512,1518 ----
  			}
  		}
  	}
+ 	//ALLOWPAIN and CAUSEPAIN can still trigger infighting, even if no pain state is worked out.
  	target->reactiontime = 0;			// we're awake now...	
  	if (source)
  	{
***************
*** 1459,1464 ****
--- 1551,1564 ----
  	if (justhit && (target->target == source || !target->target || !target->IsFriend(target->target)))
  		target->flags |= MF_JUSTHIT;    // fight back!
  
+ 	if (invulpain) //Note that this takes into account all the cheats a player has, in terms of invulnerability.
+ 	{
+ 		return -1; //NOW we return -1!
+ 	}
+ 	else if (fakedPain)
+ 	{
+ 		return holdDamage;	//This is the calculated damage after all is said and done. 
+ 	}
  	return damage;
  }
  
***************
*** 1577,1583 ****
  
  bool P_PoisonPlayer (player_t *player, AActor *poisoner, AActor *source, int poison)
  {
! 	if((player->cheats&CF_GODMODE) || (player->mo->flags2 & MF2_INVULNERABLE))
  	{
  		return false;
  	}
--- 1677,1683 ----
  
  bool P_PoisonPlayer (player_t *player, AActor *poisoner, AActor *source, int poison)
  {
! 	if((player->cheats&CF_GODMODE) || (player->mo->flags2 & MF2_INVULNERABLE) || (player->cheats & CF_GODMODE2))
  	{
  		return false;
  	}
***************
*** 1628,1635 ****
  	{
  		return;
  	}
! 	if (damage < TELEFRAG_DAMAGE && ((target->flags2 & MF2_INVULNERABLE) ||
! 		(player->cheats & CF_GODMODE)))
  	{ // target is invulnerable
  		return;
  	}
--- 1728,1735 ----
  	{
  		return;
  	}
! 	if ((damage < TELEFRAG_DAMAGE && ((target->flags2 & MF2_INVULNERABLE) ||
! 		(player->cheats & CF_GODMODE))) || (player->cheats & CF_GODMODE2))
  	{ // target is invulnerable
  		return;
  	}
***************
*** 1673,1679 ****
  	target->health -= damage;
  	if (target->health <= 0)
  	{ // Death
! 		if (player->cheats & CF_BUDDHA)
  		{ // [SP] Save the player... 
  			player->health = target->health = 1;
  		}
--- 1773,1779 ----
  	target->health -= damage;
  	if (target->health <= 0)
  	{ // Death
! 		if ((((player->cheats & CF_BUDDHA) || (player->mo->flags7 & MF7_BUDDHA)) && damage < TELEFRAG_DAMAGE) || (player->cheats & CF_BUDDHA2))
  		{ // [SP] Save the player... 
  			player->health = target->health = 1;
  		}
***************
*** 1711,1717 ****
  		P_SetMobjState(target, target->info->painstate);
  	}
  */
- 	return;
  }
  
  
--- 1811,1816 ----
