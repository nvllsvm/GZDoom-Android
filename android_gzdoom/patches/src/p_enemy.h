*** /home/draje/Code/gzdoom/src/p_enemy.h	2017-06-18 07:52:50.699680353 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/p_enemy.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 72,78 ****
  DECLARE_ACTION(A_BossDeath)
  
  void A_Chase(AActor *self);
! void A_FaceTarget (AActor *actor, angle_t max_turn = 0, angle_t max_pitch = ANGLE_270);
  
  bool A_RaiseMobj (AActor *, fixed_t speed);
  bool A_SinkMobj (AActor *, fixed_t speed);
--- 72,79 ----
  DECLARE_ACTION(A_BossDeath)
  
  void A_Chase(AActor *self);
! void A_FaceTarget(AActor *actor, angle_t max_turn = 0, angle_t max_pitch = ANGLE_270);
! void A_Face(AActor *self, AActor *other, angle_t max_turn = 0, angle_t max_pitch = ANGLE_270);
  
  bool A_RaiseMobj (AActor *, fixed_t speed);
  bool A_SinkMobj (AActor *, fixed_t speed);
