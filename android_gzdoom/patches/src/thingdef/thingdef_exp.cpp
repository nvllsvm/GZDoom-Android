*** /home/draje/Code/gzdoom/src/thingdef/thingdef_exp.cpp	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/thingdef/thingdef_exp.cpp	2017-06-18 09:09:24.175064043 -0400
***************
*** 371,376 ****
--- 371,405 ----
  
  		return new FxRandom(rng, min, max, sc);
  	}
+ 	else if (sc.CheckToken(TK_RandomPick))
+ 	{
+ 		FRandom *rng;
+ 		TArray<FxExpression*> list;
+ 		list.Clear();
+ 		int index = 0;
+ 
+ 		if (sc.CheckToken('['))
+ 		{
+ 			sc.MustGetToken(TK_Identifier);
+ 			rng = FRandom::StaticFindRNG(sc.String);
+ 			sc.MustGetToken(']');
+ 		}
+ 		else
+ 		{
+ 			rng = &pr_exrandom;
+ 		}
+ 		sc.MustGetToken('(');
+ 
+ 		for (;;)
+ 		{
+ 			FxExpression *expr = ParseExpressionM(sc, cls);
+ 			list.Push(expr);
+ 			if (sc.CheckToken(')'))
+ 				break;
+ 			sc.MustGetToken(',');
+ 		}
+ 		return new FxRandomPick(rng, list, sc);
+ 	}
  	else if (sc.CheckToken(TK_FRandom))
  	{
  		FRandom *rng;
