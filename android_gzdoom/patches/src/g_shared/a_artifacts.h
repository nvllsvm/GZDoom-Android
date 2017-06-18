*** /home/draje/Code/gzdoom/src/g_shared/a_artifacts.h	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/g_shared/a_artifacts.h	2017-06-18 09:09:24.168397311 -0400
***************
*** 173,178 ****
--- 173,179 ----
  	void EndEffect ();
  	void PositionAccuracy ();
  	void Travelled ();
+ 	bool HandlePickup(AInventory *item);
  };
  
  class APowerFrightener : public APowerup
***************
*** 181,186 ****
--- 182,195 ----
  protected:
  	void InitEffect ();
  	void EndEffect ();
+ };
+ 
+ class APowerBuddha : public APowerup
+ {
+ 	DECLARE_CLASS (APowerBuddha, APowerup)
+ protected:
+ 	void InitEffect ();
+ 	void EndEffect ();
  };
  
  class APowerTimeFreezer : public APowerup
