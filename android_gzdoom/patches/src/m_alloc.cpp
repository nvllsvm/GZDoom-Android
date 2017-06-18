*** /home/draje/Code/gzdoom/src/m_alloc.cpp	2017-06-18 07:23:17.296807148 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/m_alloc.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 54,59 ****
--- 54,67 ----
  #define _msize(p)				malloc_size(p)
  #elif defined(__sun)
  #define _msize(p)				(*((size_t*)(p)-1))
+ #elif defined(__ANDROID__)
+ //#define _msize(p)				(*((size_t*)(p)-1)) //WTH Android does not have this function Google removed  it, this may work..
+ //Above crashes on 5.0 sometimes
+ extern "C"
+ {
+ //extern size_t malloc_usable_size(void* block);
+ }
+ #define _msize(p)	malloc_usable_size(p)
  #elif !defined(_WIN32)
  #define _msize(p)				malloc_usable_size(p)	// from glibc/FreeBSD
  #endif
