*** /home/draje/Code/gzdoom/src/version.h	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/version.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 51,57 ****
  // Version identifier for network games.
  // Bump it every time you do a release unless you're certain you
  // didn't change anything that will affect sync.
! #define NETGAMEVERSION 230
  
  // Version stored in the ini's [LastRun] section.
  // Bump it if you made some configuration change that you want to
--- 51,57 ----
  // Version identifier for network games.
  // Bump it every time you do a release unless you're certain you
  // didn't change anything that will affect sync.
! #define NETGAMEVERSION 231
  
  // Version stored in the ini's [LastRun] section.
  // Bump it if you made some configuration change that you want to
***************
*** 76,82 ****
  
  // Use 4500 as the base git save version, since it's higher than the
  // SVN revision ever got.
! #define SAVEVER 4512
  
  #define SAVEVERSTRINGIFY2(x) #x
  #define SAVEVERSTRINGIFY(x) SAVEVERSTRINGIFY2(x)
--- 76,82 ----
  
  // Use 4500 as the base git save version, since it's higher than the
  // SVN revision ever got.
! #define SAVEVER 4518
  
  #define SAVEVERSTRINGIFY2(x) #x
  #define SAVEVERSTRINGIFY(x) SAVEVERSTRINGIFY2(x)
***************
*** 86,92 ****
  
  // This is so that derivates can use the same savegame versions without worrying about engine compatibility
  #define GAMESIG "GZDOOM"
! #define BASEWAD "gzdoom.pk3"
  
  // More stuff that needs to be different for derivatives.
  #define GAMENAME "GZDoom"
--- 86,92 ----
  
  // This is so that derivates can use the same savegame versions without worrying about engine compatibility
  #define GAMESIG "GZDOOM"
! #define BASEWAD "gzdoom_dev.pk3"
  
  // More stuff that needs to be different for derivatives.
  #define GAMENAME "GZDoom"
