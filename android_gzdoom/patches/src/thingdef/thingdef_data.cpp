*** /home/draje/Code/gzdoom/src/thingdef/thingdef_data.cpp	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/thingdef/thingdef_data.cpp	2017-06-18 09:09:24.175064043 -0400
***************
*** 241,246 ****
--- 241,258 ----
  	DEFINE_FLAG(MF7, NOTELESTOMP, AActor, flags7),
  	DEFINE_FLAG(MF7, ALWAYSTELEFRAG, AActor, flags7),
  	DEFINE_FLAG(MF7, WEAPONSPAWN, AActor, flags7),
+ 	DEFINE_FLAG(MF7, HARMFRIENDS, AActor, flags7),
+ 	DEFINE_FLAG(MF7, BUDDHA, AActor, flags7),
+ 	DEFINE_FLAG(MF7, FOILBUDDHA, AActor, flags7),
+ 	DEFINE_FLAG(MF7, DONTTHRUST, AActor, flags7),
+ 	DEFINE_FLAG(MF7, ALLOWPAIN, AActor, flags7),
+ 	DEFINE_FLAG(MF7, CAUSEPAIN, AActor, flags7),
+ 	DEFINE_FLAG(MF7, THRUREFLECT, AActor, flags7),
+ 	DEFINE_FLAG(MF7, MIRRORREFLECT, AActor, flags7),
+ 	DEFINE_FLAG(MF7, AIMREFLECT, AActor, flags7),
+ 	DEFINE_FLAG(MF7, HITTARGET, AActor, flags7),
+ 	DEFINE_FLAG(MF7, HITMASTER, AActor, flags7),
+ 	DEFINE_FLAG(MF7, HITTRACER, AActor, flags7),
  
  	// Effect flags
  	DEFINE_FLAG(FX, VISIBILITYPULSE, AActor, effects),
***************
*** 311,316 ****
--- 323,329 ----
  	DEFINE_FLAG(IF, NEVERRESPAWN, AInventory, ItemFlags),
  	DEFINE_FLAG(IF, NOSCREENFLASH, AInventory, ItemFlags),
  	DEFINE_FLAG(IF, TOSSED, AInventory, ItemFlags),
+ 	DEFINE_FLAG(IF, ALWAYSRESPAWN, AInventory, ItemFlags),
  
  	DEFINE_DEPRECATED_FLAG(PICKUPFLASH),
  	DEFINE_DEPRECATED_FLAG(INTERHUBSTRIP),};
