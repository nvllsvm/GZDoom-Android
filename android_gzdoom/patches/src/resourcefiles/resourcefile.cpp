*** /home/draje/Code/gzdoom/src/resourcefiles/resourcefile.cpp	2017-06-18 07:23:17.316807352 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/resourcefiles/resourcefile.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 150,160 ****
  //
  //==========================================================================
  
  void FResourceLump::CheckEmbedded()
  {
  	// Checks for embedded archives
  	const char *c = strstr(FullName, ".wad");
! 	if (c && strlen(c) == 4 && !strchr(FullName, '/'))
  	{
  		// Mark all embedded WADs
  		Flags |= LUMPF_EMBEDDED;
--- 150,177 ----
  //
  //==========================================================================
  
+ static bool IsWadInFolder(const FResourceFile* const archive, const char* const resPath)
+ {
+ 	// Checks a special case when <somefile.wad> was put in
+ 	// <myproject> directory inside <myproject.zip>
+ 
+ 	if (NULL == archive)
+ 	{
+ 		return false;
+ 	}
+ 
+     const FString dirName = ExtractFileBase(archive->Filename);
+ 	const FString fileName = ExtractFileBase(resPath, true);
+ 	const FString filePath = dirName + '/' + fileName;
+ 
+ 	return 0 == filePath.CompareNoCase(resPath);
+ }
+ 
  void FResourceLump::CheckEmbedded()
  {
  	// Checks for embedded archives
  	const char *c = strstr(FullName, ".wad");
! 	if (c && strlen(c) == 4 && (!strchr(FullName, '/') || IsWadInFolder(Owner, FullName)))
  	{
  		// Mark all embedded WADs
  		Flags |= LUMPF_EMBEDDED;
