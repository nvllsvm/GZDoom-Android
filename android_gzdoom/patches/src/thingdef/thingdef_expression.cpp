*** /home/draje/Code/gzdoom/src/thingdef/thingdef_expression.cpp	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/thingdef/thingdef_expression.cpp	2017-06-18 09:09:24.175064043 -0400
***************
*** 88,93 ****
--- 88,95 ----
  DEFINE_MEMBER_VARIABLE(radius, AActor)
  DEFINE_MEMBER_VARIABLE(reactiontime, AActor)
  DEFINE_MEMBER_VARIABLE(meleerange, AActor)
+ DEFINE_MEMBER_VARIABLE(Speed, AActor)
+ DEFINE_MEMBER_VARIABLE(roll, AActor)
  
  
  //==========================================================================
***************
*** 1685,1690 ****
--- 1687,1759 ----
  	}
  	else
  	{
+ 		val.Int = (*rng)();
+ 	}
+ 	return val;
+ }
+ 
+ //==========================================================================
+ //
+ //
+ //
+ //==========================================================================
+ FxRandomPick::FxRandomPick(FRandom * r, TArray<FxExpression*> mi, const FScriptPosition &pos)
+ : FxExpression(pos)
+ {
+ 	for (unsigned int index = 0; index < mi.Size(); index++)
+ 	{
+ 		min.Push(new FxIntCast(mi[index]));
+ 	}
+ 	rng = r;
+ 	ValueType = VAL_Int;
+ }
+ 
+ //==========================================================================
+ //
+ //
+ //
+ //==========================================================================
+ 
+ FxRandomPick::~FxRandomPick()
+ {
+ }
+ 
+ //==========================================================================
+ //
+ //
+ //
+ //==========================================================================
+ 
+ FxExpression *FxRandomPick::Resolve(FCompileContext &ctx)
+ {
+ 	CHECKRESOLVED();
+ 	for (unsigned int index = 0; index < min.Size(); index++)
+ 	{
+ 		RESOLVE(min[index], ctx);
+ 		ABORT(min[index]);
+ 	}
+ 	return this;
+ };
+ 
+ 
+ //==========================================================================
+ //
+ //
+ //
+ //==========================================================================
+ 
+ ExpVal FxRandomPick::EvalExpression(AActor *self)
+ {
+ 	ExpVal val;
+ 	val.Type = VAL_Int;
+ 	int max = min.Size();
+ 	if (max > 0)
+ 	{
+ 		int select = (*rng)(max);
+ 		val.Int = min[select]->EvalExpression(self).GetInt();
+ 	}
+ 	else
+ 	{
  		val.Int = (*rng)();
  	}
  	return val;
