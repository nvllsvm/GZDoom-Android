*** doom/src/main/jni/gzdoom/src/gl/dynlights/gl_lightbuffer.cpp	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/dynlights/gl_lightbuffer.cpp	2017-06-18 23:34:13.203914027 -0400
***************
*** 38,221 ****
  **
  */
  
  #include "gl/system/gl_system.h"
! #include "gl/shaders/gl_shader.h"
! #include "gl/dynlights/gl_lightbuffer.h"
  #include "gl/dynlights/gl_dynlight.h"
! #include "gl/system/gl_interface.h"
! #include "gl/utility//gl_clock.h"
  
- static const int INITIAL_BUFFER_SIZE = 160000;	// This means 80000 lights per frame and 160000*16 bytes == 2.56 MB.
  
! float *mMap;
  
  FLightBuffer::FLightBuffer()
  {
  
! 	mBufferSize = INITIAL_BUFFER_SIZE;
! 	mByteSize = mBufferSize * sizeof(float);
! 	if (gl.flags & RFL_SHADER_STORAGE_BUFFER)
! 	{
! 		mBufferType = GL_SHADER_STORAGE_BUFFER;
! 		mBlockAlign = -1;
! 		mBlockSize = mBufferSize;
! 	}
! 	else
! 	{
! 		mBufferType = GL_UNIFORM_BUFFER;
! 		mBlockSize = gl.maxuniformblock / 16;
! 		if (mBlockSize > 2048) mBlockSize = 2048;	// we don't really need a larger buffer
! 		mBlockAlign = mBlockSize / 2;
! 	}
  
! 	glGenBuffers(1, &mBufferId);
! 	glBindBufferBase(mBufferType, LIGHTBUF_BINDINGPOINT, mBufferId);
! 	if (gl.flags & RFL_BUFFER_STORAGE)
! 	{
! 		glBufferStorage(mBufferType, mByteSize, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
! 		mBufferPointer = (float*)glMapBufferRange(mBufferType, 0, mByteSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
! 	}
! 	else
! 	{
! 		glBufferData(mBufferType, mByteSize, NULL, GL_DYNAMIC_DRAW);
! 		mBufferPointer = NULL;
! 	}
  
! 	Clear();
! 	mLastMappedIndex = UINT_MAX;
  }
  
  FLightBuffer::~FLightBuffer()
  {
! 	glBindBuffer(mBufferType, 0);
! 	glDeleteBuffers(1, &mBufferId);
  }
  
! void FLightBuffer::Clear()
  {
! 	mIndex = 0;
! 	mIndices.Clear();
! 	mUploadIndex = 0;
  }
  
- int FLightBuffer::UploadLights(FDynLightData &data)
- {
- 	int size0 = data.arrays[0].Size()/4;
- 	int size1 = data.arrays[1].Size()/4;
- 	int size2 = data.arrays[2].Size()/4;
- 	int totalsize = size0 + size1 + size2 + 1;
  
! 	if (mBlockAlign >= 0 && totalsize + (mIndex % mBlockAlign) > mBlockSize)
  	{
! 		mIndex = ((mIndex + mBlockAlign) / mBlockAlign) * mBlockAlign;
  
! 		// can't be rendered all at once.
! 		if (totalsize > mBlockSize)
! 		{
! 			int diff = totalsize - mBlockSize;
  
! 			size2 -= diff;
! 			if (size2 < 0)
! 			{
! 				size1 += size2;
! 				size2 = 0;
! 			}
! 			if (size1 < 0)
  			{
! 				size0 += size1;
! 				size1 = 0;
  			}
! 			totalsize = size0 + size1 + size2 + 1;
  		}
! 	}
  
! 	if (totalsize <= 1) return -1;
  
! 	if (mIndex + totalsize > mBufferSize)
! 	{
! 		// reallocate the buffer with twice the size
! 		unsigned int newbuffer;
! 
! 		// first unmap the old buffer
! 		glBindBuffer(mBufferType, mBufferId);
! 		glUnmapBuffer(mBufferType);
! 
! 		// create and bind the new buffer, bind the old one to a copy target (too bad that DSA is not yet supported well enough to omit this crap.)
! 		glGenBuffers(1, &newbuffer);
! 		glBindBufferBase(mBufferType, LIGHTBUF_BINDINGPOINT, newbuffer);
! 		glBindBuffer(GL_COPY_READ_BUFFER, mBufferId);
! 
! 		// create the new buffer's storage (twice as large as the old one)
! 		mBufferSize *= 2;
! 		mByteSize *= 2;
! 		if (gl.flags & RFL_BUFFER_STORAGE)
! 		{
! 			glBufferStorage(mBufferType, mByteSize, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
! 			mBufferPointer = (float*)glMapBufferRange(mBufferType, 0, mByteSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
! 		}
! 		else
! 		{
! 			glBufferData(mBufferType, mByteSize, NULL, GL_DYNAMIC_DRAW);
! 			mBufferPointer = (float*)glMapBufferRange(mBufferType, 0, mByteSize, GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_BUFFER_BIT);
! 		}
  
- 		// copy contents and delete the old buffer.
- 		glCopyBufferSubData(GL_COPY_READ_BUFFER, mBufferType, 0, 0, mByteSize/2);
- 		glBindBuffer(GL_COPY_READ_BUFFER, 0);
- 		glDeleteBuffers(1, &mBufferId);
- 		mBufferId = newbuffer;
  	}
  
- 	float *copyptr;
- 	
- 	assert(mBufferPointer != NULL);
- 	if (mBufferPointer == NULL) return -1;
- 	copyptr = mBufferPointer + mIndex * 4;
  
! 	float parmcnt[] = { 0, size0, size0 + size1, size0 + size1 + size2 };
  
! 	memcpy(&copyptr[0], parmcnt, 4 * sizeof(float));
! 	memcpy(&copyptr[4], &data.arrays[0][0], 4 * size0*sizeof(float));
! 	memcpy(&copyptr[4 + 4*size0], &data.arrays[1][0], 4 * size1*sizeof(float));
! 	memcpy(&copyptr[4 + 4*(size0 + size1)], &data.arrays[2][0], 4 * size2*sizeof(float));
  
! 	unsigned int bufferindex = mIndex;
! 	mIndex += totalsize;
! 	draw_dlight += (totalsize-1) / 2;
! 	return bufferindex;
  }
  
! void FLightBuffer::Begin()
  {
! 	if (!(gl.flags & RFL_BUFFER_STORAGE))
! 	{
! 		glBindBuffer(mBufferType, mBufferId);
! 		mBufferPointer = (float*)glMapBufferRange(mBufferType, 0, mByteSize, GL_MAP_WRITE_BIT);
! 	}
  }
  
! void FLightBuffer::Finish()
  {
! 	if (!(gl.flags & RFL_BUFFER_STORAGE))
  	{
! 		glBindBuffer(mBufferType, mBufferId);
! 		glUnmapBuffer(mBufferType);
! 		mBufferPointer = NULL;
  	}
  }
  
! int FLightBuffer::BindUBO(unsigned int index)
  {
! 	unsigned int offset = (index / mBlockAlign) * mBlockAlign;
  
! 	if (offset != mLastMappedIndex)
! 	{
! 		// this will only get called if a uniform buffer is used. For a shader storage buffer we only need to bind the buffer once at the start to all shader programs
! 		mLastMappedIndex = offset;
! 		glBindBufferRange(GL_UNIFORM_BUFFER, LIGHTBUF_BINDINGPOINT, mBufferId, offset*16, mBlockSize*16);	// we go from counting vec4's to counting bytes here.
! 	}
! 	return (index - offset);
  }
  
  
  
--- 38,243 ----
  **
  */
  
+ #if 0	// unused for now. Code doesn't work
+ 
  #include "gl/system/gl_system.h"
! #include "c_dispatch.h"
! #include "p_local.h"
! #include "vectors.h"
! #include "g_level.h"
! 
! #include "gl/system/gl_cvars.h"
! #include "gl/renderer/gl_renderer.h"
! #include "gl/renderer/gl_lightdata.h"
! #include "gl/data/gl_data.h"
  #include "gl/dynlights/gl_dynlight.h"
! #include "gl/dynlights/gl_lightbuffer.h"
! #include "gl/scene/gl_drawinfo.h"
! #include "gl/scene/gl_portal.h"
! #include "gl/shaders/gl_shader.h"
! #include "gl/textures/gl_material.h"
  
  
! //==========================================================================
! //
! //
! //
! //==========================================================================
  
  FLightBuffer::FLightBuffer()
  {
+ 	glGenBuffers(1, &mIDbuf_RGB);
+ 	glBindBuffer(GL_TEXTURE_BUFFER, mIDbuf_RGB);
  
! 	glGenBuffers(1, &mIDbuf_Position);
! 	glBindBuffer(GL_TEXTURE_BUFFER, mIDbuf_Position);
  
! 	glGenTextures(1, &mIDtex_RGB);
! 	glBindTexture(GL_TEXTURE_BUFFER, mIDtex_RGB);
! 	gl.TexBufferARB(GL_TEXTURE_BUFFER, GL_RGBA8, mIDbuf_RGB);
  
! 	glGenTextures(1, &mIDtex_Position);
! 	glBindTexture(GL_TEXTURE_BUFFER, mIDtex_Position);
! 	gl.TexBufferARB(GL_TEXTURE_BUFFER, GL_RGBA32F, mIDbuf_Position);
  }
  
+ 
+ //==========================================================================
+ //
+ //
+ //
+ //==========================================================================
+ 
  FLightBuffer::~FLightBuffer()
  {
! 	glBindBuffer(GL_TEXTURE_BUFFER, 0);
! 	glDeleteBuffers(1, &mIDbuf_RGB);
! 	glDeleteBuffers(1, &mIDbuf_Position);
! 
! 	glBindTexture(GL_TEXTURE_BUFFER, 0);
! 	glDeleteTextures(1, &mIDtex_RGB);
! 	glDeleteTextures(1, &mIDtex_Position);
! 
  }
  
! //==========================================================================
! //
! //
! //
! //==========================================================================
! 
! void FLightBuffer::BindTextures(int texunit1, int texunit2)
  {
! 	glActiveTexture(texunit1);
! 	glBindTexture(GL_TEXTURE_BUFFER, mIDtex_RGB);
! 	glActiveTexture(texunit2);
! 	glBindTexture(GL_TEXTURE_BUFFER, mIDtex_Position);
! 	glActiveTexture(GL_TEXTURE0);
  }
  
  
! //==========================================================================
! //
! // This collects all currently actove
! //
! //==========================================================================
! 
! void FLightBuffer::CollectLightSources()
! {
! 	if (gl_dynlight_shader && gl_lights && GLRenderer->mLightCount && gl_fixedcolormap == CM_DEFAULT)
  	{
! 		TArray<FLightRGB> pLights(100);
! 		TArray<FLightPosition> pPos(100);
! 		TThinkerIterator<ADynamicLight> it(STAT_DLIGHT);
  
! 		ADynamicLight *light;
  
! 		while ((light = it.Next()) != NULL)
! 		{
! 			if (!(light->flags2 & MF2_DORMANT))
  			{
! 				FLightRGB rgb;
! 				FLightPosition pos;
! 
! 				rgb.R = light->GetRed();
! 				rgb.G = light->GetGreen();
! 				rgb.B = light->GetBlue();
! 				rgb.Type = (light->flags4 & MF4_SUBTRACTIVE)? 128 : (light->flags4 & MF4_ADDITIVE || foggy)? 255:0;
! 				pos.X = FIXED2FLOAT(light->x);
! 				pos.Y = FIXED2FLOAT(light->y); 
! 				pos.Z =  FIXED2FLOAT(light->z);
! 				pos.Distance = (light->GetRadius() * gl_lights_size);
! 				light->bufferindex = pPos.Size();
! 				pLights.Push(rgb);
! 				pPos.Push(pos);
  			}
! 			else light->bufferindex = -1;
  		}
! 		GLRenderer->mLightCount = pPos.Size();
  
! 		glBindBuffer(GL_TEXTURE_BUFFER, mIDbuf_RGB);
! 		glBufferData(GL_TEXTURE_BUFFER, pLights.Size() * sizeof (FLightRGB), &pLights[0], GL_STREAM_DRAW);
  
! 		glBindBuffer(GL_TEXTURE_BUFFER, mIDbuf_Position);
! 		glBufferData(GL_TEXTURE_BUFFER, pPos.Size() * sizeof (FLightPosition), &pPos[0], GL_STREAM_DRAW);
  
  	}
+ }
  
  
! //==========================================================================
! //
! //
! //
! //==========================================================================
  
! FLightIndexBuffer::FLightIndexBuffer()
! {
! 	glGenBuffers(1, &mIDBuffer);
! 	glBindBuffer(GL_TEXTURE_BUFFER, mIDBuffer);
  
! 	glGenTextures(1, &mIDTexture);
! 	glBindTexture(GL_TEXTURE_BUFFER, mIDTexture);
! 	gl.TexBufferARB(GL_TEXTURE_BUFFER, GL_R16UI, mIDBuffer);
  }
  
! //==========================================================================
! //
! //
! //
! //==========================================================================
! 
! FLightIndexBuffer::~FLightIndexBuffer()
  {
! 	glBindBuffer(GL_TEXTURE_BUFFER, 0);
! 	glDeleteBuffers(1, &mIDBuffer);
! 
! 	glBindTexture(GL_TEXTURE_BUFFER, 0);
! 	glDeleteTextures(1, &mIDTexture);
  }
  
! 
! //==========================================================================
! //
! //
! //
! //==========================================================================
! 
! void FLightIndexBuffer::AddLight(ADynamicLight *light)
  {
! 	if (light->bufferindex >= 0)
  	{
! 		mBuffer.Push(light->bufferindex);
  	}
  }
  
! //==========================================================================
! //
! //
! //
! //==========================================================================
! 
! void FLightIndexBuffer::SendBuffer()
  {
! 	glBindBuffer(GL_TEXTURE_BUFFER, mIDBuffer);
! 	glBufferData(GL_TEXTURE_BUFFER, mBuffer.Size() * sizeof (short), &mBuffer[0], GL_STREAM_DRAW);
! 	glBindBuffer(GL_TEXTURE_BUFFER, 0);
! }
  
! 
! //==========================================================================
! //
! //
! //
! //==========================================================================
! 
! void FLightIndexBuffer::BindTexture(int texunit1)
! {
! 	glActiveTexture(texunit1);
! 	glBindTexture(GL_TEXTURE_BUFFER, mIDTexture);
! 	glActiveTexture(GL_TEXTURE0);
  }
  
  
  
+ #endif
\ No newline at end of file
