*** doom/src/main/jni/gzdoom/src/gl/utility/gl_clock.h	2017-06-18 23:15:10.669973853 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/utility/gl_clock.h	2017-06-18 23:34:13.410582604 -0400
***************
*** 5,17 ****
  #include "x86.h"
  #include "m_fixed.h"
  
- extern bool gl_benching;
- 
  #ifdef _MSC_VER
  
  extern double gl_SecondsPerCycle;
  extern double gl_MillisecPerCycle;
  
  __forceinline long long GetClockCycle ()
  {
  #if _M_X64
--- 5,16 ----
  #include "x86.h"
  #include "m_fixed.h"
  
  #ifdef _MSC_VER
  
  extern double gl_SecondsPerCycle;
  extern double gl_MillisecPerCycle;
  
+ 
  __forceinline long long GetClockCycle ()
  {
  #if _M_X64
***************
*** 76,88 ****
  		// Not using QueryPerformanceCounter directly, so we don't need
  		// to pull in the Windows headers for every single file that
  		// wants to do some profiling.
! 		long long time = (gl_benching? GetClockCycle() : 0);
  		Counter -= time;
  	}
  	
  	__forceinline void Unclock()
  	{
! 		long long time = (gl_benching? GetClockCycle() : 0);
  		Counter += time;
  	}
  	
--- 75,87 ----
  		// Not using QueryPerformanceCounter directly, so we don't need
  		// to pull in the Windows headers for every single file that
  		// wants to do some profiling.
! 		long long time = GetClockCycle();
  		Counter -= time;
  	}
  	
  	__forceinline void Unclock()
  	{
! 		long long time = GetClockCycle();
  		Counter += time;
  	}
  	
***************
*** 109,115 ****
  extern glcycle_t ProcessAll;
  extern glcycle_t RenderAll;
  extern glcycle_t Dirty;
- extern glcycle_t drawcalls;
  
  extern int iter_dlightf, iter_dlight, draw_dlight, draw_dlightf;
  extern int rendered_lines,rendered_flats,rendered_sprites,rendered_decals,render_vertexsplit,render_texsplit;
--- 108,113 ----
***************
*** 119,125 ****
  
  void ResetProfilingData();
  void CheckBench();
- void  checkBenchActive();
  
  
  #endif
\ No newline at end of file
--- 117,122 ----
