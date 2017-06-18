*** /home/draje/Code/gzdoom/src/thingdef/thingdef_exp.h	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/thingdef/thingdef_exp.h	2017-06-18 09:09:24.175064043 -0400
***************
*** 559,564 ****
--- 559,585 ----
  //
  //==========================================================================
  
+ class FxRandomPick : public FxExpression
+ {
+ protected:
+ 	FRandom * rng;
+ 	TDeletingArray<FxExpression*> min;
+ 
+ public:
+ 
+ 	FxRandomPick(FRandom *, TArray<FxExpression*> mi, const FScriptPosition &pos);
+ 	~FxRandomPick();
+ 	FxExpression *Resolve(FCompileContext&);
+ 
+ 	ExpVal EvalExpression(AActor *self);
+ };
+ 
+ //==========================================================================
+ //
+ //
+ //
+ //==========================================================================
+ 
  class FxFRandom : public FxRandom
  {
  public:
