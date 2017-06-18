*** /home/draje/Code/gzdoom/src/g_shared/a_armor.cpp	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_shared/a_armor.cpp	2017-06-18 09:09:24.168397311 -0400
***************
*** 516,522 ****
--- 516,529 ----
  					// with the dragon skin bracers.
  					if (damage < 10000)
  					{
+ #if __APPLE__ && __GNUC__ == 4 && __GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__  == 1
+ 						// -O1 optimizer bug work around. Only needed for
+ 						// GCC 4.2.1 on OS X for 10.4/10.5 tools compatibility.
+ 						volatile fixed_t tmp = 300;
+ 						Slots[i] -= Scale (damage, SlotsIncrement[i], tmp);
+ #else
  						Slots[i] -= Scale (damage, SlotsIncrement[i], 300);
+ #endif
  						if (Slots[i] < 2*FRACUNIT)
  						{
  							Slots[i] = 0;
