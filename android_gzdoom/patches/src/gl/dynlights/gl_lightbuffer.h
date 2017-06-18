*** /home/draje/Code/gzdoom/src/gl/dynlights/gl_lightbuffer.h	2017-06-18 07:52:50.689680265 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/gl/dynlights/gl_lightbuffer.h	2017-06-18 09:09:24.168397311 -0400
***************
*** 1,39 ****
! #ifndef __GL_LIGHTBUFFER_H
! #define __GL_LIGHTBUFFER_H
! 
! #include "tarray.h"
! struct FDynLightData;
! 
! class FLightBuffer
! {
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
! 
! public:
! 
! 	FLightBuffer();
! 	~FLightBuffer();
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
! };
! 
! #endif
! 
--- 1,39 ----
! #ifndef __GL_LIGHTBUFFER_H
! #define __GL_LIGHTBUFFER_H
! 
! #include "tarray.h"
! struct FDynLightData;
! 
! class FLightBuffer
! {
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
! 
! public:
! 
! 	FLightBuffer();
! 	~FLightBuffer();
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
! };
! 
! #endif
! 
