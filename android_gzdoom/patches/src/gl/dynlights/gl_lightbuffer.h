*** doom/src/main/jni/gzdoom/src/gl/dynlights/gl_lightbuffer.h	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/dynlights/gl_lightbuffer.h	2017-06-18 23:34:13.190580570 -0400
***************
*** 1,39 ****
  #ifndef __GL_LIGHTBUFFER_H
  #define __GL_LIGHTBUFFER_H
  
! #include "tarray.h"
! struct FDynLightData;
  
  class FLightBuffer
  {
! 	TArray<int> mIndices;
! 	unsigned int mBufferId;
! 	float * mBufferPointer;
! 
! 	unsigned int mBufferType;
! 	unsigned int mIndex;
! 	unsigned int mUploadIndex;
! 	unsigned int mLastMappedIndex;
! 	unsigned int mBlockAlign;
! 	unsigned int mBlockSize;
! 	unsigned int mBufferSize;
! 	unsigned int mByteSize;
  
! public:
  
  	FLightBuffer();
  	~FLightBuffer();
! 	void Clear();
! 	int UploadLights(FDynLightData &data);
! 	void Begin();
! 	void Finish();
! 	int BindUBO(unsigned int index);
! 	unsigned int GetBlockSize() const { return mBlockSize; }
! 	unsigned int GetBufferType() const { return mBufferType; }
! 	unsigned int GetIndexPtr() const { return mIndices.Size();	}
! 	void StoreIndex(int index) { mIndices.Push(index); }
! 	int GetIndex(int i) const { return mIndices[i];	}
  };
  
  #endif
  
--- 1,66 ----
  #ifndef __GL_LIGHTBUFFER_H
  #define __GL_LIGHTBUFFER_H
  
! #if 0
! class ADynamicLight;
! 
! const int MAX_DYNLIGHTS = 40000;	// should hopefully be enough
! 
! struct FLightRGB
! {
! 	unsigned char R,G,B,Type;	// Type is 0 for normal, 1 for additive and 2 for subtractive
! };
! 
! struct FLightPosition
! {
! 	float X,Z,Y,Distance;
! };
  
  class FLightBuffer
  {
! 	unsigned int mIDbuf_RGB;
! 	unsigned int mIDbuf_Position;
  
! 	unsigned int mIDtex_RGB;
! 	unsigned int mIDtex_Position;
  
+ public:
  	FLightBuffer();
  	~FLightBuffer();
! 	//void MapBuffer();
! 	//void UnmapBuffer();
! 	void BindTextures(int uniloc1, int uniloc2);
! 	//void AddLight(ADynamicLight *light, bool foggy);
! 	void CollectLightSources();
! };
! 
! class FLightIndexBuffer
! {
! 	unsigned int mIDBuffer;
! 	unsigned int mIDTexture;
! 
! 	TArray<unsigned short> mBuffer;
! 
! public:
! 
! 	FLightIndexBuffer();
! 	~FLightIndexBuffer();
! 	void AddLight(ADynamicLight *light);
! 	void SendBuffer();
! 	void BindTexture(int loc1);
! 
! 	void ClearBuffer()
! 	{
! 		mBuffer.Clear();
! 	}
! 
! 	int GetLightIndex()
! 	{
! 		return mBuffer.Size();
! 	}
! 
  };
  
  #endif
  
+ #endif
\ No newline at end of file
