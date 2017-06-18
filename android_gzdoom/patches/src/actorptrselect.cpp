*** /home/draje/Code/gzdoom/src/actorptrselect.cpp	2017-06-18 07:52:50.683013539 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/actorptrselect.cpp	2017-06-18 09:09:24.161730580 -0400
***************
*** 56,61 ****
--- 56,68 ----
  		case AAPTR_TRACER: return origin->tracer;
  		case AAPTR_FRIENDPLAYER:
  			return origin->FriendPlayer ? AAPTR_RESOLVE_PLAYERNUM(origin->FriendPlayer - 1) : NULL;
+ 
+ 		case AAPTR_GET_LINETARGET:
+ 			{
+ 				AActor *gettarget = NULL;
+ 				P_BulletSlope(origin, &gettarget);
+ 				return gettarget;
+ 			}
  		}
  	}
  
