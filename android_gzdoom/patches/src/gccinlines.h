*** /home/draje/Code/gzdoom/src/gccinlines.h	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/gccinlines.h	2017-06-18 09:09:24.165063945 -0400
***************
*** 41,47 ****
  		 :   "a,a,a,a,a,a" (a),
  		     "m,r,m,r,d,d" (b),
  		     "r,r,m,m,r,m" (c)
! 		 : "%cc"
  			);
  
  	return result;
--- 41,47 ----
  		 :   "a,a,a,a,a,a" (a),
  		     "m,r,m,r,d,d" (b),
  		     "r,r,m,m,r,m" (c)
! 		 : "cc"
  			);
  
  	return result;
***************
*** 59,65 ****
  		 :  "a,a,a,a" (a),
  		    "m,r,m,r" (b),
  		    "c,c,I,I" (c)
! 		 : "%cc"
  			);
  	return result;
  }
--- 59,65 ----
  		 :  "a,a,a,a" (a),
  		    "m,r,m,r" (b),
  		    "c,c,I,I" (c)
! 		 : "cc"
  			);
  	return result;
  }
***************
*** 210,216 ****
  		: "a" (lo),
  		  "d" (hi),
  		  "r" (b)
! 		: "%cc");
  	return result;
  }
  
--- 210,216 ----
  		: "a" (lo),
  		  "d" (hi),
  		  "r" (b)
! 		: "cc");
  	return result;
  }
  
***************
*** 226,232 ****
  		 "=&d,d" (dummy)
  		: "a,a" (a),
  		  "r,m" (b)
! 		: "%cc");
  	return result;
  }
  
--- 226,232 ----
  		 "=&d,d" (dummy)
  		: "a,a" (a),
  		  "r,m" (b)
! 		: "cc");
  	return result;
  }
  
***************
*** 241,247 ****
  			: "a,a" (a<<s), \
  			  "d,d" (a>>(32-s)), \
  			  "r,m" (b) \
! 			: "%cc"); \
  		return result; \
  	}
  
--- 241,247 ----
  			: "a,a" (a<<s), \
  			  "d,d" (a>>(32-s)), \
  			  "r,m" (b) \
! 			: "cc"); \
  		return result; \
  	}
  
***************
*** 287,293 ****
  		 "=d,d" (dummy)
  		: "d,d" (a),
  		  "r,m" (b)
! 		: "%cc");
  	return result;
  }
  
--- 287,293 ----
  		 "=d,d" (dummy)
  		: "d,d" (a),
  		  "r,m" (b)
! 		: "cc");
  	return result;
  }
  
***************
*** 313,319 ****
  		 "rep stosw"
  		:"=D" (buff), "=c" (count)
  		:"D" (buff), "c" (count), "a" (clear|(clear<<16))
! 		:"%cc");
  }
  
  static inline SDWORD ksgn (SDWORD a)
--- 313,319 ----
  		 "rep stosw"
  		:"=D" (buff), "=c" (count)
  		:"D" (buff), "c" (count), "a" (clear|(clear<<16))
! 		:"cc");
  }
  
  static inline SDWORD ksgn (SDWORD a)
***************
*** 327,332 ****
  		 "adc $0,%1"
  		:"=r" (dummy), "=r" (result)
  		:"0" (a)
! 		:"%cc");
  	return result;
  }
--- 327,332 ----
  		 "adc $0,%1"
  		:"=r" (dummy), "=r" (result)
  		:"0" (a)
! 		:"cc");
  	return result;
  }
