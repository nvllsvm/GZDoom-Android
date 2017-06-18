*** /home/draje/Code/gzdoom/src/sc_man_scanner.re	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/sc_man_scanner.re	2017-06-18 09:09:24.165063945 -0400
***************
*** 158,163 ****
--- 158,164 ----
  		'random'					{ RET(TK_Random); }
  		'random2'					{ RET(TK_Random2); }
  		'frandom'					{ RET(TK_FRandom); }
+ 		'randompick'				{ RET(TK_RandomPick); }
  
  		L (L|D)*					{ RET(TK_Identifier); }
  
