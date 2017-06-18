*** ../../../../gzdoom/src/thingdef/thingdef_codeptr.cpp	2017-06-19 01:07:23.461992144 -0400
--- thingdef_codeptr.cpp	2017-06-19 21:24:51.793811917 -0400
***************
*** 2056,2062 ****
--- 2056,2066 ----
  			bo->Speed = xyvel;
  		bo->angle = self->angle + (((pr_grenade()&7) - 4) << 24);
  
+ #ifdef __ANDROID__
+ 		angle_t pitch = angle_t((int)(-self->pitch)) >> ANGLETOFINESHIFT;
+ #else
  		angle_t pitch = angle_t(-self->pitch) >> ANGLETOFINESHIFT;
+ #endif
  		angle_t angle = bo->angle >> ANGLETOFINESHIFT;
  
  		// There are two vectors we are concerned about here: xy and z. We rotate
***************
*** 2068,2074 ****
--- 2072,2082 ----
  		fixed_t xy_velx = FixedMul(xy_xyscale, finecosine[angle]);
  		fixed_t xy_vely = FixedMul(xy_xyscale, finesine[angle]);
  
+ #ifdef __ANDROID__
+ 		pitch = angle_t((int)(self->pitch)) >> ANGLETOFINESHIFT;
+ #else
  		pitch = angle_t(self->pitch) >> ANGLETOFINESHIFT;
+ #endif
  		fixed_t z_xyscale = FixedMul(zvel, finesine[pitch]);
  		fixed_t z_velz = FixedMul(zvel, finecosine[pitch]);
  		fixed_t z_velx = FixedMul(z_xyscale, finecosine[angle]);
