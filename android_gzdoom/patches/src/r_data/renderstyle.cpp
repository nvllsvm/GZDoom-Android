*** /home/draje/Code/gzdoom/src/r_data/renderstyle.cpp	2017-06-18 07:23:17.313473984 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/r_data/renderstyle.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 54,60 ****
--- 54,64 ----
  	{ { STYLEOP_FuzzOrAdd, STYLEALPHA_Src, STYLEALPHA_InvSrc, 0 } },									/* STYLE_OptFuzzy */
  	{ { STYLEOP_Add, STYLEALPHA_Src, STYLEALPHA_InvSrc, STYLEF_Alpha1 | STYLEF_ColorIsFixed } },		/* STYLE_Stencil */
  	{ { STYLEOP_Add, STYLEALPHA_Src, STYLEALPHA_InvSrc, 0 } },											/* STYLE_Translucent */
+ #ifdef __ANDROID__
+ 	{ { STYLEOP_Add, STYLEALPHA_One, STYLEALPHA_One, 0 } },												/* STYLE_Add *///This is a bug in the original code, should be ONE ONE, no?
+ #else
  	{ { STYLEOP_Add, STYLEALPHA_Src, STYLEALPHA_One, 0 } },												/* STYLE_Add */
+ #endif
  	{ { STYLEOP_Add, STYLEALPHA_Src, STYLEALPHA_InvSrc, STYLEF_RedIsAlpha | STYLEF_ColorIsFixed } },	/* STYLE_Shaded */
  	{ { STYLEOP_Add, STYLEALPHA_Src, STYLEALPHA_InvSrc, STYLEF_ColorIsFixed } },						/* STYLE_TranslucentStencil */
  	{ { STYLEOP_Shadow, 0, 0, 0 } },																	/* STYLE_Shadow */
