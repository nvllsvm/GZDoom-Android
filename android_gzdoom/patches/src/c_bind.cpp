*** /home/draje/Code/gzdoom/src/c_bind.cpp	2017-06-18 07:52:50.686346901 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/c_bind.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 230,236 ****
--- 230,240 ----
  	NULL,		NULL,		NULL,		NULL,		NULL,		"pause",	NULL,		"home",		//C0
  	"uparrow",	"pgup",		NULL,		"leftarrow",NULL,		"rightarrow",NULL,		"end",		//C8
  	"downarrow","pgdn",		"ins",		"del",		NULL,		NULL,		NULL,		NULL,		//D0
+ #ifdef __APPLE__
+ 	NULL,		NULL,		NULL,		"command",	NULL,		"apps",		"power",	"sleep",	//D8
+ #else // !__APPLE__
  	NULL,		NULL,		NULL,		"lwin",		"rwin",		"apps",		"power",	"sleep",	//D8
+ #endif // __APPLE__
  	NULL,		NULL,		NULL,		"wake",		NULL,		"search",	"favorites","refresh",	//E0
  	"webstop",	"webforward","webback",	"mycomputer","mail",	"mediaselect",NULL,		NULL,		//E8
  	NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		NULL,		//F0
