*** doom/src/main/jni/gzdoom/src/gl/data/gl_vertexbuffer.cpp	2017-06-20 19:11:53.519649563 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/data/gl_vertexbuffer.cpp	2017-06-24 18:23:16.935437135 -0400
***************
*** 49,54 ****
--- 49,88 ----
  #include "gl/data/gl_data.h"
  #include "gl/data/gl_vertexbuffer.h"
  
+ extern "C"
+ {
+ 	#include "GL/nano_gl.h"
+ }
+ 
+ #define glBindBuffer			 nanoglBindBuffer
+ #define glDeleteBuffers			 nanoglDeleteBuffers
+ #define glGenBuffers			 nanoglGenBuffers
+ #define glBufferData			 nanoglBufferData
+ #define glBufferSubData			 nanoglBufferSubData
+ #define glMapBuffer				 nanoglMapBuffer
+ #define glUnmapBuffer			 nanoglUnmapBuffer
+ 
+ 
+ CUSTOM_CVAR(Int, gl_usevbo, -1, CVAR_ARCHIVE | CVAR_GLOBALCONFIG | CVAR_NOINITCALL)
+ {
+ 
+ 	if (self < -1 || self > 2 || !(gl.flags&RFL_VBO))
+ 	{
+ 		self = 0;
+ 	}
+ 	else if (self == -1)
+ 	{
+ 		if (!(gl.flags & RFL_NVIDIA)) self = 0;
+ 		else self = 2;
+ 	}
+ 	else if (GLRenderer != NULL && GLRenderer->mVBO != NULL && GLRenderer->mVBO->vbo_arg != self)
+ 	{
+ 		Printf("Vertex buffer use will be changed for the next level.\n");
+ 	}
+ 
+ 	//if (self != 1)
+ 	//	self = 1;
+ }
  
  //==========================================================================
  //
***************
*** 58,67 ****
  
  FVertexBuffer::FVertexBuffer()
  {
! 	vao_id = vbo_id = 0;
! 	glGenBuffers(1, &vbo_id);
! 	glGenVertexArrays(1, &vao_id);
! 
  }
  	
  FVertexBuffer::~FVertexBuffer()
--- 92,109 ----
  
  FVertexBuffer::FVertexBuffer()
  {
! 	LOGI("FVertexBuffer");
! 	vbo_id = 0;
! 	if (gl.flags&RFL_VBO)
! 	{
! 		if (gl_usevbo == -1) gl_usevbo.Callback();
! 
! 		LOGI("FVertexBuffer glGenBuffers");
! 		glGenBuffers(1, &vbo_id);
! 		//WTF does not return the 1
! 		LOGI("FVertexBuffer vbo_id = %d",vbo_id);
! 		vbo_id = 1;
! 	}
  }
  	
  FVertexBuffer::~FVertexBuffer()
***************
*** 70,84 ****
  	{
  		glDeleteBuffers(1, &vbo_id);
  	}
- 	if (vao_id != 0)
- 	{
- 		glDeleteVertexArrays(1, &vao_id);
- 	}
- }
- 
- void FVertexBuffer::BindVBO()
- {
- 	glBindVertexArray(vao_id);
  }
  
  //==========================================================================
--- 112,117 ----
***************
*** 90,149 ****
  FFlatVertexBuffer::FFlatVertexBuffer()
  : FVertexBuffer()
  {
! 	if (gl.flags & RFL_BUFFER_STORAGE)
  	{
! 		unsigned int bytesize = BUFFER_SIZE * sizeof(FFlatVertex);
! 		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
! 		glBufferStorage(GL_ARRAY_BUFFER, bytesize, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
! 		map = (FFlatVertex*)glMapBufferRange(GL_ARRAY_BUFFER, 0, bytesize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
! 
! 		glBindVertexArray(vao_id);
! 		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
! 		glVertexAttribPointer(VATTR_VERTEX, 3,GL_FLOAT, false, sizeof(FFlatVertex), &VTO->x);
! 		glVertexAttribPointer(VATTR_TEXCOORD, 2,GL_FLOAT, false, sizeof(FFlatVertex), &VTO->u);
! 		glEnableVertexAttribArray(VATTR_VERTEX);
! 		glEnableVertexAttribArray(VATTR_TEXCOORD);
! 		glBindVertexArray(0);
  	}
  	else
  	{
! 		vbo_shadowdata.Reserve(BUFFER_SIZE);
! 		map = &vbo_shadowdata[0];
  	}
! 	mNumReserved = mIndex = mCurIndex = 0;
  }
  
  FFlatVertexBuffer::~FFlatVertexBuffer()
  {
! 	if (gl.flags & RFL_BUFFER_STORAGE)
! 	{
! 		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
! 		glUnmapBuffer(GL_ARRAY_BUFFER);
! 		glBindBuffer(GL_ARRAY_BUFFER, 0);
! 	}
! }
! 
! //==========================================================================
! //
! // immediate mode fallback for drivers without GL_ARB_buffer_storage
! //
! // No single core method is performant enough  to handle this adequately
! // so we have to resort to immediate mode instead...
! //
! //==========================================================================
! 
! void FFlatVertexBuffer::ImmRenderBuffer(unsigned int primtype, unsigned int offset, unsigned int count)
! {
! 	// this will only get called if we can't acquire a persistently mapped buffer.
! #ifndef CORE_PROFILE
! 	glBegin(primtype);
! 	for (unsigned int i = 0; i < count; i++)
! 	{
! 		glVertexAttrib2fv(VATTR_TEXCOORD, &map[offset + i].u);
! 		glVertexAttrib3fv(VATTR_VERTEX, &map[offset + i].x);
! 	}
! 	glEnd();
! #endif
  }
  
  //==========================================================================
--- 123,142 ----
  FFlatVertexBuffer::FFlatVertexBuffer()
  : FVertexBuffer()
  {
! 	if (!(gl.flags&RFL_VBO)) 
  	{
! 		vbo_arg = 0;
  	}
  	else
  	{
! 		vbo_arg = gl_usevbo;
  	}
! 	map = NULL;
  }
  
  FFlatVertexBuffer::~FFlatVertexBuffer()
  {
! 	UnmapVBO();
  }
  
  //==========================================================================
***************
*** 159,164 ****
--- 152,158 ----
  	z = plane.ZatPoint(vt->fx, vt->fy);
  	u = vt->fx/64.f;
  	v = -vt->fy/64.f;
+ 	w = /*dc = df =*/ 0;
  }
  
  //==========================================================================
***************
*** 300,317 ****
  //
  //==========================================================================
  
  void FFlatVertexBuffer::UpdatePlaneVertices(sector_t *sec, int plane)
  {
  	int startvt = sec->vboindex[plane];
  	int countvt = sec->vbocount[plane];
  	secplane_t &splane = sec->GetSecPlane(plane);
  	FFlatVertex *vt = &vbo_shadowdata[startvt];
! 	FFlatVertex *mapvt = &map[startvt];
! 	for(int i=0; i<countvt; i++, vt++, mapvt++)
  	{
  		vt->z = splane.ZatPoint(vt->x, vt->y);
  		if (plane == sector_t::floor && sec->transdoor) vt->z -= 1;
! 		mapvt->z = vt->z;
  	}
  }
  
--- 294,351 ----
  //
  //==========================================================================
  
+ void FFlatVertexBuffer::MapVBO()
+ {
+ 	if (map == NULL)
+ 	{
+ 		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
+ 		map = (FFlatVertex*)glMapBufferRange(GL_ARRAY_BUFFER, 0, vbo_shadowdata.Size() * sizeof(FFlatVertex), 
+ 			GL_MAP_WRITE_BIT|GL_MAP_FLUSH_EXPLICIT_BIT|GL_MAP_UNSYNCHRONIZED_BIT);
+ 	}
+ }
+ 
+ //==========================================================================
+ //
+ //
+ //
+ //==========================================================================
+ 
+ void FFlatVertexBuffer::UnmapVBO()
+ {
+ 	if (map != NULL)
+ 	{
+ 		glUnmapBuffer(GL_ARRAY_BUFFER);
+ 		map = NULL;
+ 	}
+ }
+ 
+ //==========================================================================
+ //
+ //
+ //
+ //==========================================================================
+ 
  void FFlatVertexBuffer::UpdatePlaneVertices(sector_t *sec, int plane)
  {
  	int startvt = sec->vboindex[plane];
  	int countvt = sec->vbocount[plane];
  	secplane_t &splane = sec->GetSecPlane(plane);
  	FFlatVertex *vt = &vbo_shadowdata[startvt];
! 	for(int i=0; i<countvt; i++, vt++)
  	{
  		vt->z = splane.ZatPoint(vt->x, vt->y);
  		if (plane == sector_t::floor && sec->transdoor) vt->z -= 1;
! 	}
! 	if (gl.flags & RFL_MAP_BUFFER_RANGE)
! 	{
! 		MapVBO();
! 		if (map == NULL) return;	// Error
! 		memcpy(&map[startvt], &vbo_shadowdata[startvt], countvt * sizeof(FFlatVertex));
! 		glFlushMappedBufferRange(GL_ARRAY_BUFFER, startvt * sizeof(FFlatVertex), countvt * sizeof(FFlatVertex));
! 	}
! 	else
! 	{
! 		glBufferSubData(GL_ARRAY_BUFFER, startvt * sizeof(FFlatVertex), countvt * sizeof(FFlatVertex), &vbo_shadowdata[startvt]);
  	}
  }
  
***************
*** 323,334 ****
  
  void FFlatVertexBuffer::CreateVBO()
  {
! 	if (gl.flags & RFL_BUFFER_STORAGE)
  	{
- 		vbo_shadowdata.Resize(mNumReserved);
  		CreateFlatVBO();
! 		mCurIndex = mIndex = vbo_shadowdata.Size();
! 		memcpy(map, &vbo_shadowdata[0], vbo_shadowdata.Size() * sizeof(FFlatVertex));
  	}
  	else if (sectors)
  	{
--- 357,368 ----
  
  void FFlatVertexBuffer::CreateVBO()
  {
! 	vbo_shadowdata.Clear();
! 	if (vbo_arg > 0)
  	{
  		CreateFlatVBO();
! 		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
! 		glBufferData(GL_ARRAY_BUFFER, vbo_shadowdata.Size() * sizeof(FFlatVertex), &vbo_shadowdata[0], GL_DYNAMIC_DRAW);
  	}
  	else if (sectors)
  	{
***************
*** 340,346 ****
--- 374,400 ----
  			sectors[i].vboheight[1] = sectors[i].vboheight[0] = FIXED_MIN;
  		}
  	}
+ }
  
+ //==========================================================================
+ //
+ //
+ //
+ //==========================================================================
+ 
+ void FFlatVertexBuffer::BindVBO()
+ {
+ 	if (vbo_arg > 0)
+ 	{
+ 		UnmapVBO();
+ 		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
+ 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
+ 		glVertexPointer(3,GL_FLOAT, sizeof(FFlatVertex), &VTO->x);
+ 		glTexCoordPointer(2,GL_FLOAT, sizeof(FFlatVertex), &VTO->u);
+ 		glEnableClientState(GL_VERTEX_ARRAY);
+ 		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
+ 		glDisableClientState(GL_INDEX_ARRAY);
+ 	}
  }
  
  //==========================================================================
***************
*** 353,383 ****
  {
  	if (sector->GetPlaneTexZ(sector_t::ceiling) != sector->vboheight[sector_t::ceiling])
  	{
! 		UpdatePlaneVertices(sector, sector_t::ceiling);
! 		sector->vboheight[sector_t::ceiling] = sector->GetPlaneTexZ(sector_t::ceiling);
  	}
  	if (sector->GetPlaneTexZ(sector_t::floor) != sector->vboheight[sector_t::floor])
  	{
! 		UpdatePlaneVertices(sector, sector_t::floor);
! 		sector->vboheight[sector_t::floor] = sector->GetPlaneTexZ(sector_t::floor);
  	}
  }
  
  //==========================================================================
  //
  // checks the validity of all planes attached to this sector
! // and updates them if possible.
  //
  //==========================================================================
  
  void FFlatVertexBuffer::CheckUpdate(sector_t *sector)
  {
! 	if (gl.flags & RFL_BUFFER_STORAGE)
  	{
  		CheckPlanes(sector);
  		sector_t *hs = sector->GetHeightSec();
  		if (hs != NULL) CheckPlanes(hs);
! 		for (unsigned i = 0; i < sector->e->XFloor.ffloors.Size(); i++)
  			CheckPlanes(sector->e->XFloor.ffloors[i]->model);
  	}
! }
\ No newline at end of file
--- 407,445 ----
  {
  	if (sector->GetPlaneTexZ(sector_t::ceiling) != sector->vboheight[sector_t::ceiling])
  	{
! 		if (sector->ceilingdata == NULL) // only update if there's no thinker attached
! 		{
! 			UpdatePlaneVertices(sector, sector_t::ceiling);
! 			sector->vboheight[sector_t::ceiling] = sector->GetPlaneTexZ(sector_t::ceiling);
! 		}
  	}
  	if (sector->GetPlaneTexZ(sector_t::floor) != sector->vboheight[sector_t::floor])
  	{
! 		if (sector->floordata == NULL) // only update if there's no thinker attached
! 		{
! 			UpdatePlaneVertices(sector, sector_t::floor);
! 			sector->vboheight[sector_t::floor] = sector->GetPlaneTexZ(sector_t::floor);
! 		}
  	}
  }
  
  //==========================================================================
  //
  // checks the validity of all planes attached to this sector
! // and updates them if possible. Anything moving will not be
! // updated unless it stops. This is to ensure that we never
! // have to synchronize with the rendering process.
  //
  //==========================================================================
  
  void FFlatVertexBuffer::CheckUpdate(sector_t *sector)
  {
! 	if (vbo_arg == 2)
  	{
  		CheckPlanes(sector);
  		sector_t *hs = sector->GetHeightSec();
  		if (hs != NULL) CheckPlanes(hs);
! 		for(unsigned i = 0; i < sector->e->XFloor.ffloors.Size(); i++)
  			CheckPlanes(sector->e->XFloor.ffloors[i]->model);
  	}
! }
