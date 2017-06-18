*** /home/draje/Code/gzdoom/src/thingdef/thingdef_function.cpp	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/thingdef/thingdef_function.cpp	2017-06-18 09:09:24.175064043 -0400
***************
*** 43,48 ****
--- 43,50 ----
  #include "tarray.h"
  #include "thingdef.h"
  #include "thingdef_exp.h"
+ #include "actor.h"
+ #include "actorptrselect.h"
  
  static TMap<FName, FxGlobalFunctionCall::Creator> CreatorMap;
  
***************
*** 142,148 ****
--- 144,154 ----
  		ret.Type = VAL_Float;
  
  		// shall we use the CRT's sin and cos functions?
+ #ifdef __ANDROID__
+ 		angle_t angle = angle_t((int)(v * ANGLE_90/90.));
+ #else
  		angle_t angle = angle_t(v * ANGLE_90/90.);
+ #endif
  		if (Name == NAME_Sin) ret.Float = FIXED2DBL (finesine[angle>>ANGLETOFINESHIFT]);
  		else ret.Float = FIXED2DBL (finecosine[angle>>ANGLETOFINESHIFT]);
  		return ret;
***************
*** 185,187 ****
--- 191,300 ----
  
  GLOBALFUNCTION_ADDER(Sqrt);
  
+ //==========================================================================
+ //
+ // Function: checkclass
+ //
+ //==========================================================================
+ 
+ class FxGlobalFunctionCall_CheckClass : public FxGlobalFunctionCall
+ {
+ 	public:
+ 		GLOBALFUNCTION_DEFINE(CheckClass);
+ 		
+ 		FxExpression *Resolve(FCompileContext& ctx)
+ 		{
+ 			CHECKRESOLVED();
+ 			
+ 			if (!ResolveArgs(ctx, 1, 3, false))
+ 			return NULL;
+ 			
+ 			for (int i = ArgList->Size(); i > 1;)
+ 			{
+ 				if (!(*ArgList)[--i]->ValueType.isNumeric())
+ 				{
+ 					ScriptPosition.Message(MSG_ERROR, "numeric value expected for parameter");
+ 					delete this;
+ 					return NULL;
+ 				}
+ 			}
+ 			
+ 			switch ((*ArgList)[0]->ValueType.Type)
+ 			{
+ 				case VAL_Class: case VAL_Name:break;
+ 				default:
+ 					ScriptPosition.Message(MSG_ERROR, "actor class expected for parameter");
+ 				delete this;
+ 				return NULL;
+ 			}
+ 			
+ 			ValueType = VAL_Float;
+ 			return this;
+ 		}
+ 		
+ 		ExpVal EvalExpression(AActor *self)
+ 		{
+ 			ExpVal ret;
+ 			ret.Type = VAL_Int;
+ 			
+ 			const PClass  * checkclass;
+ 			{
+ 				ExpVal v = (*ArgList)[0]->EvalExpression(self);
+ 				checkclass = v.GetClass();
+ 				if (!checkclass)
+ 				{
+ 					checkclass = PClass::FindClass(v.GetName());
+ 					if (!checkclass) { ret.Int = 0; return ret; }
+ 				}
+ 			}
+ 			
+ 			bool match_superclass = false;
+ 			int pick_pointer = AAPTR_DEFAULT;
+ 			
+ 			switch (ArgList->Size())
+ 			{
+ 				case 3: match_superclass = (*ArgList)[2]->EvalExpression(self).GetBool();
+ 				case 2: pick_pointer = (*ArgList)[1]->EvalExpression(self).GetInt();
+ 			}
+ 			
+ 			self = COPY_AAPTR(self, pick_pointer);
+ 			if (!self){ ret.Int = 0; return ret; }
+ 				ret.Int = match_superclass ? checkclass->IsAncestorOf(self->GetClass()) : checkclass == self->GetClass();
+ 			return ret;
+ 		}
+ 	};
+ 
+ GLOBALFUNCTION_ADDER(CheckClass);
+ 
+ //==========================================================================
+ //
+ // Function: ispointerequal
+ //
+ //==========================================================================
+ 
+ class FxGlobalFunctionCall_IsPointerEqual : public FxGlobalFunctionCall
+ {
+ 	public:
+ 		GLOBALFUNCTION_DEFINE(IsPointerEqual);
+ 		
+ 		FxExpression *Resolve(FCompileContext& ctx)
+ 		{
+ 			CHECKRESOLVED();
+ 			
+ 			if (!ResolveArgs(ctx, 2, 2, true))
+ 				return NULL;
+ 			
+ 			ValueType = VAL_Int;
+ 			return this;
+ 		}
+ 		
+ 		ExpVal EvalExpression(AActor *self)
+ 		{
+ 			ExpVal ret;
+ 			ret.Type = VAL_Int;
+ 			ret.Int = COPY_AAPTR(self, (*ArgList)[0]->EvalExpression(self).GetInt()) == COPY_AAPTR(self, (*ArgList)[1]->EvalExpression(self).GetInt());
+ 			return ret;
+ 		}
+ };
+ 
+ GLOBALFUNCTION_ADDER(IsPointerEqual);
