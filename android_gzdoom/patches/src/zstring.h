*** /home/draje/Code/gzdoom/src/zstring.h	2017-06-18 07:52:50.709680441 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/zstring.h	2017-06-18 11:01:39.604316297 -0400
***************
*** 47,52 ****
--- 47,70 ----
  #define PRINTFISH(x)
  #endif
  
+ #ifdef __clang__
+ #define IGNORE_FORMAT_PRE \
+ 	_Pragma("GCC diagnostic push") \
+ 	_Pragma("GCC diagnostic ignored \"-Wformat-invalid-specifier\"") \
+ 	_Pragma("GCC diagnostic ignored \"-Wformat-extra-args\"")
+ #define IGNORE_FORMAT_POST _Pragma("GCC diagnostic pop")
+ #elif defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ >= 6)))
+ #define IGNORE_FORMAT_PRE \
+ 	_Pragma("GCC diagnostic push") \
+ 	_Pragma("GCC diagnostic ignored \"-Wformat=\"") \
+ 	_Pragma("GCC diagnostic ignored \"-Wformat-extra-args\"")
+ #define IGNORE_FORMAT_POST _Pragma("GCC diagnostic pop")
+ #else
+ #define IGNORE_FORMAT_PRE
+ #define IGNORE_FORMAT_POST
+ #endif
+ 
+ 
  struct FStringData
  {
  	unsigned int Len;		// Length of string, excluding terminating null
***************
*** 278,283 ****
--- 296,311 ----
  	static FNullStringData NullString;
  
  	friend struct FStringData;
+ 
+ private:
+ 	// Prevent these from being called as current practices are to use Compare.
+ 	// Without this FStrings will be accidentally compared against char* ptrs.
+ 	bool operator == (const FString &illegal) const;
+ 	bool operator != (const FString &illegal) const;
+ 	bool operator < (const FString &illegal) const;
+ 	bool operator > (const FString &illegal) const;
+ 	bool operator <= (const FString &illegal) const;
+ 	bool operator >= (const FString &illegal) const;
  };
  
  namespace StringFormat
