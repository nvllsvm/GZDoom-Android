*** /home/draje/Code/gzdoom/src/p_udmf.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_udmf.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 257,263 ****
--- 257,267 ----
  
  angle_t UDMFParserBase::CheckAngle(const char *key)
  {
+ #ifdef __ANDROID__
+ 	return angle_t((int)(CheckFloat(key) * ANGLE_90 / 90.));
+ #else
  	return angle_t(CheckFloat(key) * ANGLE_90 / 90.);
+ #endif
  }
  
  bool UDMFParserBase::CheckBool(const char *key)
***************
*** 696,701 ****
--- 700,706 ----
  
  			case NAME_FillColor:
  				th->fillcolor = CheckInt(key);
+ 				break;
  
  			case NAME_Health:
  				th->health = CheckInt(key);
***************
*** 1030,1040 ****
  				Flag(ld->flags, ML_BLOCKHITSCAN, key); 
  				continue;
  			
! 			// [Dusk] lock number
  			case NAME_Locknumber:
  				ld->locknumber = CheckInt(key);
  				continue;
  
  			default:
  				break;
  			}
--- 1035,1050 ----
  				Flag(ld->flags, ML_BLOCKHITSCAN, key); 
  				continue;
  			
! 			// [TP] Locks the special with a key
  			case NAME_Locknumber:
  				ld->locknumber = CheckInt(key);
  				continue;
  
+ 			// [TP] Causes a 3d midtex to behave like an impassible line
+ 			case NAME_Midtex3dimpassible:
+ 				Flag(ld->flags, ML_3DMIDTEX_IMPASS, key);
+ 				continue;
+ 
  			default:
  				break;
  			}
***************
*** 1081,1086 ****
--- 1091,1100 ----
  		{
  			ld->args[1] = -FName(arg1str);
  		}
+ 		if ((ld->flags & ML_3DMIDTEX_IMPASS) && !(ld->flags & ML_3DMIDTEX)) // [TP]
+ 		{
+ 			Printf ("Line %d has midtex3dimpassible without midtex3d.\n", index);
+ 		}
  	}
  
  	//===========================================================================
***************
*** 1531,1541 ****
  			double ulen = TVector3<double>(cp[0], cp[1], cp[2]).Length();
  
  			// normalize the vector, it must have a length of 1
! 			sec->floorplane.a = FLOAT2FIXED(cp[0] / ulen);
! 			sec->floorplane.b = FLOAT2FIXED(cp[1] / ulen);
! 			sec->floorplane.c = FLOAT2FIXED(cp[2] / ulen);
! 			sec->floorplane.d = FLOAT2FIXED(cp[3] / ulen);
! 			sec->floorplane.ic = FLOAT2FIXED(ulen / cp[2]);
  		}
  
  		if (lightcolor == -1 && fadecolor == -1 && desaturation == -1)
--- 1545,1555 ----
  			double ulen = TVector3<double>(cp[0], cp[1], cp[2]).Length();
  
  			// normalize the vector, it must have a length of 1
! 			sec->ceilingplane.a = FLOAT2FIXED(cp[0] / ulen);
! 			sec->ceilingplane.b = FLOAT2FIXED(cp[1] / ulen);
! 			sec->ceilingplane.c = FLOAT2FIXED(cp[2] / ulen);
! 			sec->ceilingplane.d = FLOAT2FIXED(cp[3] / ulen);
! 			sec->ceilingplane.ic = FLOAT2FIXED(ulen / cp[2]);
  		}
  
  		if (lightcolor == -1 && fadecolor == -1 && desaturation == -1)
