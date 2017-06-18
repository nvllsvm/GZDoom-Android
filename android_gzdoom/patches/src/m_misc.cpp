*** /home/draje/Code/gzdoom/src/m_misc.cpp	2017-06-18 07:52:50.696346991 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/m_misc.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 137,142 ****
--- 137,168 ----
  	return length;
  }
  
+ //
+ // M_ReadFile (same as above but use malloc instead of new to allocate the buffer.)
+ //
+ int M_ReadFileMalloc (char const *name, BYTE **buffer)
+ {
+ 	int handle, count, length;
+ 	struct stat fileinfo;
+ 	BYTE *buf;
+ 
+ 	handle = open (name, O_RDONLY | O_BINARY, 0666);
+ 	if (handle == -1)
+ 		I_Error ("Couldn't read file %s", name);
+ 	if (fstat (handle,&fileinfo) == -1)
+ 		I_Error ("Couldn't read file %s", name);
+ 	length = fileinfo.st_size;
+ 	buf = (BYTE*)M_Malloc(length);
+ 	count = read (handle, buf, length);
+ 	close (handle);
+ 
+ 	if (count < length)
+ 		I_Error ("Couldn't read file %s", name);
+ 
+ 	*buffer = buf;
+ 	return length;
+ }
+ 
  //---------------------------------------------------------------------------
  //
  // PROC M_FindResponseFile
