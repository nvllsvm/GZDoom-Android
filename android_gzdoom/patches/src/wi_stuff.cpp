*** /home/draje/Code/gzdoom/src/wi_stuff.cpp	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/wi_stuff.cpp	2017-06-18 11:02:52.701646195 -0400
***************
*** 1330,1336 ****
  		for (i = 0; i < MAXPLAYERS; i++)
  		{
  			// If the player is in the game and not ready, stop checking
! 			if (playeringame[i] && !players[i].isbot && !playerready[i])
  				break;
  		}
  
--- 1330,1336 ----
  		for (i = 0; i < MAXPLAYERS; i++)
  		{
  			// If the player is in the game and not ready, stop checking
! 			if (playeringame[i] && players[i].Bot == NULL && !playerready[i])
  				break;
  		}
  
***************
*** 1429,1435 ****
  			clamp(int(g*255.f), 0, 255), 
  			clamp(int(b*255.f), 0, 255)), 0.8f, x, y - ypadding, (deaths_x - x) + (8 * CleanXfac), lineheight);
  
! 		if (playerready[pnum] || player->isbot) // Bots are automatically assumed ready, to prevent confusion
  			screen->DrawTexture(readyico, x - (readyico->GetWidth() * CleanXfac), y, DTA_CleanNoMove, true, TAG_DONE);
  
  		color = (EColorRange)HU_GetRowColor(player, pnum == consoleplayer);
--- 1429,1435 ----
  			clamp(int(g*255.f), 0, 255), 
  			clamp(int(b*255.f), 0, 255)), 0.8f, x, y - ypadding, (deaths_x - x) + (8 * CleanXfac), lineheight);
  
! 		if (playerready[pnum] || player->Bot != NULL) // Bots are automatically assumed ready, to prevent confusion
  			screen->DrawTexture(readyico, x - (readyico->GetWidth() * CleanXfac), y, DTA_CleanNoMove, true, TAG_DONE);
  
  		color = (EColorRange)HU_GetRowColor(player, pnum == consoleplayer);
***************
*** 1638,1644 ****
  		for (i = 0; i < MAXPLAYERS; i++)
  		{
  			// If the player is in the game and not ready, stop checking
! 			if (playeringame[i] && !players[i].isbot && !playerready[i])
  				break;
  		}
  
--- 1638,1644 ----
  		for (i = 0; i < MAXPLAYERS; i++)
  		{
  			// If the player is in the game and not ready, stop checking
! 			if (playeringame[i] && players[i].Bot == NULL && !playerready[i])
  				break;
  		}
  
***************
*** 1735,1741 ****
  			clamp(int(g*255.f), 0, 255),
  			clamp(int(b*255.f), 0, 255)), 0.8f, x, y - ypadding, (secret_x - x) + (8 * CleanXfac), lineheight);
  
! 		if (playerready[i] || player->isbot) // Bots are automatically assumed ready, to prevent confusion
  			screen->DrawTexture(readyico, x - (readyico->GetWidth() * CleanXfac), y, DTA_CleanNoMove, true, TAG_DONE);
  
  		color = (EColorRange)HU_GetRowColor(player, i == consoleplayer);
--- 1735,1741 ----
  			clamp(int(g*255.f), 0, 255),
  			clamp(int(b*255.f), 0, 255)), 0.8f, x, y - ypadding, (secret_x - x) + (8 * CleanXfac), lineheight);
  
! 		if (playerready[i] || player->Bot != NULL) // Bots are automatically assumed ready, to prevent confusion
  			screen->DrawTexture(readyico, x - (readyico->GetWidth() * CleanXfac), y, DTA_CleanNoMove, true, TAG_DONE);
  
  		color = (EColorRange)HU_GetRowColor(player, i == consoleplayer);
***************
*** 2010,2016 ****
  		{
  			if ((player->cmd.ucmd.buttons ^ player->oldbuttons) &&
  				((players[i].cmd.ucmd.buttons & players[i].oldbuttons)
! 					== players[i].oldbuttons) && !player->isbot)
  			{
  				acceleratestage = 1;
  				playerready[i] = true;
--- 2010,2016 ----
  		{
  			if ((player->cmd.ucmd.buttons ^ player->oldbuttons) &&
  				((players[i].cmd.ucmd.buttons & players[i].oldbuttons)
! 					== players[i].oldbuttons) && player->Bot == NULL)
  			{
  				acceleratestage = 1;
  				playerready[i] = true;
