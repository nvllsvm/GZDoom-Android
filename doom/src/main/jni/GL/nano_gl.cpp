/*
Copyright (C) 2007-2008 Olli Hinkka

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 */

#include <stdio.h>
#include <string.h>

extern "C"
{


#define USE_GLES 1

#define COLOR_BYTE 0

#define CHECK_OVERFLOW 1

// give error : E/GLLogger(  857): called unimplemented OpenGL ES API
//#include "glesinterface.h"

#define __MULTITEXTURE_SUPPORT__

#include "gl.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES/gl.h>
#include "nano_gl.h"

#include <android/log.h>

#define LOGI(...)
#define LOGE(...)

void FatalError(char * print)
{
	LOGE("Nango GL: %s", print);
	//*(int *)0 = 0;
}



void GL_PrintError(int errornumber, const char *filename, int linenumber)
{
	switch(errornumber)
	{
#ifdef GL_INVALID_ENUM
	case GL_INVALID_ENUM:
		LOGI("GL_INVALID_ENUM at %s:%i\n", filename, linenumber);
		break;
#endif
#ifdef GL_INVALID_VALUE
	case GL_INVALID_VALUE:
		LOGI("GL_INVALID_VALUE at %s:%i\n", filename, linenumber);
		break;
#endif
#ifdef GL_INVALID_OPERATION
	case GL_INVALID_OPERATION:
		LOGI("GL_INVALID_OPERATION at %s:%i\n", filename, linenumber);
		break;
#endif
#ifdef GL_STACK_OVERFLOW
	case GL_STACK_OVERFLOW:
		LOGI("GL_STACK_OVERFLOW at %s:%i\n", filename, linenumber);
		break;
#endif
#ifdef GL_STACK_UNDERFLOW
	case GL_STACK_UNDERFLOW:
		LOGI("GL_STACK_UNDERFLOW at %s:%i\n", filename, linenumber);
		break;
#endif
#ifdef GL_OUT_OF_MEMORY
	case GL_OUT_OF_MEMORY:
		LOGI("GL_OUT_OF_MEMORY at %s:%i\n", filename, linenumber);
		break;
#endif
#ifdef GL_TABLE_TOO_LARGE
	case GL_TABLE_TOO_LARGE:
		LOGI("GL_TABLE_TOO_LARGE at %s:%i\n", filename, linenumber);
		break;
#endif
#ifdef GL_INVALID_FRAMEBUFFER_OPERATION
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		LOGI("GL_INVALID_FRAMEBUFFER_OPERATION at %s:%i\n", filename, linenumber);
		break;
#endif
	default:
		LOGI("GL UNKNOWN (%i) at %s:%i\n", errornumber, filename, linenumber);
		break;
	}

	*(int *)0 = 0;
}



//#define CHECKGLERROR {int errornumber = glGetError();if (errornumber) GL_PrintError(errornumber, __FILE__, __LINE__);};
#define CHECKGLERROR



#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1

typedef struct
{
	GLboolean alpha_test;
	GLboolean blend;
	GLboolean clip_planei;
	GLboolean color_logic_op;
	GLboolean color_material;
	GLboolean cull_face;
	GLboolean depth_test;
	GLboolean dither;
	GLboolean fog;
	GLboolean lighti;
	GLboolean lighting;
	GLboolean line_smooth;
	GLboolean matrix_palette_oes;
	GLboolean multisample;
	GLboolean normalize;
	GLboolean point_smooth;
	GLboolean point_sprite_oes;
	GLboolean polygon_offset_fill;
	GLboolean rescale_normal;
	GLboolean sample_alpha_to_coverage;
	GLboolean sample_alpha_to_one;
	GLboolean sample_coverage;
	GLboolean scissor_test;
	GLboolean stencil_test;
	GLboolean depthmask;
	GLboolean colormask_r;
	GLboolean colormask_g;
	GLboolean colormask_b;
	GLboolean colormask_a;
	GLboolean clip_plane_0;
	GLboolean clip_plane_1;
	GLboolean clip_plane_2;
	GLboolean clip_plane_3;
	GLboolean clip_plane_4;
	GLboolean clip_plane_5;

	GLclampf depth_range_near;
	GLclampf depth_range_far;
	GLenum depth_func;
	GLenum cullface;
	GLenum shademodel;
	GLenum sfactor;
	GLenum dfactor;
	GLenum matrixmode;
} nanoState;

static nanoState nanoglState;

static nanoState nanoglInitState =
{
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_TRUE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_TRUE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_TRUE,

		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,

		//clip
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		GL_FALSE,
		0.0f,
		1.0f,
		GL_LESS,
		GL_BACK,
		GL_SMOOTH,
		GL_ONE,
		GL_ZERO,
		GL_MODELVIEW,
};

struct booleanstate
{
	GLboolean value;
	GLboolean changed;
};

struct floatstate
{
	GLfloat value;
	GLboolean changed;
};

struct uintstate
{
	GLuint value;
	GLboolean changed;
};

struct ptrstate
{
	GLint size;
	GLenum type;
	GLsizei stride;
	GLvoid* ptr;
	GLboolean changed;
	GLboolean enabled;
};


typedef struct
{
	struct booleanstate texture_2d;
	struct floatstate texture_env_mode;
	struct uintstate boundtexture;
	struct ptrstate vertex_array;
	struct ptrstate color_array;
	struct ptrstate texture_coord_array;
} nanotmuState;

static nanotmuState tmuState0;
static nanotmuState tmuState1;

static nanotmuState tmuInitState =
{
		{GL_FALSE, GL_FALSE},
		{GL_MODULATE,GL_FALSE},
		{0x7fffffff,GL_FALSE},
		{4,GL_FLOAT,0, NULL, GL_FALSE, GL_FALSE},
		{4,GL_FLOAT,0, NULL, GL_FALSE, GL_FALSE},
		{4,GL_FLOAT,0, NULL, GL_FALSE, GL_FALSE},
};

static nanotmuState* activetmuState = &tmuState0;

//extern "C++" GlESInterface* GLESIMPL ;
//extern GlESInterface* GLESIMPL ;
#define GLESIMPL

static GLenum wrapperPrimitiveMode = GL_QUADS;
GLboolean useTexCoordArray = GL_FALSE;
static GLenum activetmu = GL_TEXTURE0;
static GLenum clientactivetmu = GL_TEXTURE0;

#if defined(__MULTITEXTURE_SUPPORT__)
GLboolean useMultiTexCoordArray = GL_FALSE;
#endif


#if !defined (__WINS__)
#define __FORCEINLINE __forceinline
#else
#define __FORCEINLINE
#endif

static GLboolean delayedttmuchange = GL_FALSE;
static GLenum delayedtmutarget = GL_TEXTURE0;


typedef struct
{
	float x;
	float y;
	float z;
#if !defined(__MULTITEXTURE_SUPPORT__)
	float padding;
#endif

#if COLOR_BYTE
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
#else
	float red;
	float green;
	float blue;
	float alpha;
#endif

	float s;
	float t;
#if defined(__MULTITEXTURE_SUPPORT__)
	float s_multi;
	float t_multi;
#endif
} VertexAttrib;




#define SIZE_VERTEXATTRIBS 80000
static VertexAttrib vertexattribs[SIZE_VERTEXATTRIBS];

#define SIZE_INDEXARRAY 300000
static GLushort indexArray[SIZE_INDEXARRAY];

static GLuint vertexCount = 0;
static GLuint indexCount = 0;
static GLuint vertexMark = 0;
static int indexbase = 0;

static VertexAttrib* ptrVertexAttribArray = NULL;
static VertexAttrib* ptrVertexAttribArrayMark = NULL;

static VertexAttrib currentVertexAttrib;
/*
#if defined(__MULTITEXTURE_SUPPORT__)
static VertexAttrib currentVertexAttribInit = {0.0f,0.0f,0.0f,
                                    255,255,255,255,
                                    0.0f,0.0f,
                                    0.0f,0.0f
                                    };
#else
static VertexAttrib currentVertexAttribInit = {0.0f,0.0f,0.0f,0.0f,
                                    255,255,255,255,
                                    0.0f,0.0f,
                                    };
#endif
 */

static GLushort* ptrIndexArray = NULL;

static GLboolean arraysValid = GL_FALSE;

void InitGLStructs()
{
	ptrVertexAttribArray = vertexattribs;
	ptrVertexAttribArrayMark = ptrVertexAttribArray;
	ptrIndexArray = indexArray;

	memcpy(&nanoglState, &nanoglInitState, sizeof( nanoState));
	memcpy(&tmuState0,&tmuInitState,sizeof( nanotmuState));
	memcpy(&tmuState1,&tmuInitState,sizeof( nanotmuState));

	//memcpy(&currentVertexAttrib,&currentVertexAttribInit,sizeof( VertexAttrib));
	currentVertexAttrib.x = 0.0f;
	currentVertexAttrib.y = 0.0f;
	currentVertexAttrib.z = 0.0f;
	currentVertexAttrib.red 	=0;
	currentVertexAttrib.green	=0;
	currentVertexAttrib.blue	=0;
	currentVertexAttrib.alpha	=0;
	currentVertexAttrib.s	 = 0.0f;
	currentVertexAttrib.t	 = 0.0f;
#if defined(__MULTITEXTURE_SUPPORT__)
	currentVertexAttrib.s_multi	 = 0.0f;
	currentVertexAttrib.t_multi	 = 0.0f;
#endif

	activetmuState = &tmuState0;
	wrapperPrimitiveMode = GL_QUADS;
	useTexCoordArray = GL_FALSE;
	activetmu = GL_TEXTURE0;
	clientactivetmu = GL_TEXTURE0;
	delayedttmuchange = GL_FALSE;
	delayedtmutarget = GL_TEXTURE0;
	vertexCount = 0;
	indexCount = 0;
	vertexMark = 0;
	indexbase = 0;
	arraysValid = GL_FALSE;
}


void FlushOnStateChange()
{
	//LOGI("FlushOnStateChange");

	if (delayedttmuchange)
	{
		LOGI("FlushOnStateChange delayedttmuchange");
		delayedttmuchange = GL_FALSE;
		GLESIMPL glActiveTexture(delayedtmutarget);
		CHECKGLERROR;
	}

	if (!vertexCount)
		return;

	//LOGI("FlushOnStateChange drawing %d", vertexCount);


	if (!arraysValid)
	{
		//LOGI("FlushOnStateChange arrays NOT valid");

		GLESIMPL glClientActiveTexture(GL_TEXTURE0);
		CHECKGLERROR;
		if (wrapperPrimitiveMode == GL_LINES)
			GLESIMPL glVertexPointer(2, GL_FLOAT, sizeof(VertexAttrib), &vertexattribs[0].x);
		else
			GLESIMPL glVertexPointer(3, GL_FLOAT, sizeof(VertexAttrib), &vertexattribs[0].x);
		CHECKGLERROR;
#if COLOR_BYTE
		GLESIMPL glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(VertexAttrib), &vertexattribs[0].red);
		CHECKGLERROR;
#else
		GLESIMPL glColorPointer(4, GL_FLOAT, sizeof(VertexAttrib), &vertexattribs[0].red);
		CHECKGLERROR;
#endif
		GLESIMPL glTexCoordPointer(2, GL_FLOAT, sizeof(VertexAttrib), &vertexattribs[0].s);
		CHECKGLERROR;
		GLESIMPL glEnableClientState(GL_VERTEX_ARRAY);
		CHECKGLERROR;
		GLESIMPL glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CHECKGLERROR;
		GLESIMPL glEnableClientState(GL_COLOR_ARRAY);
		CHECKGLERROR;
#if defined(__MULTITEXTURE_SUPPORT__)
		GLESIMPL glClientActiveTexture(GL_TEXTURE1);
		CHECKGLERROR;
		GLESIMPL glTexCoordPointer(2, GL_FLOAT, sizeof(VertexAttrib), &vertexattribs[0].s_multi);
		CHECKGLERROR;
		GLESIMPL glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CHECKGLERROR;
		GLESIMPL glClientActiveTexture(GL_TEXTURE0);
		CHECKGLERROR;
#endif
		arraysValid = GL_TRUE;
	}


#if CHECK_OVERFLOW
	if ( ptrVertexAttribArray >= vertexattribs + SIZE_VERTEXATTRIBS){

		FatalError("vertexattribs overflow\n");
	}
	if ( ptrIndexArray >= indexArray + SIZE_INDEXARRAY){

		FatalError("indexArray overflow\n");
	}
#endif

	//LOGI("FlushOnStateChange draw ");
	//glEnable(GL_DEPTH_TEST) ;
	//glClear(GL_DEPTH_BUFFER_BIT);

	if (wrapperPrimitiveMode == GL_LINES)
	{
		GLESIMPL glDrawElements( GL_LINES,vertexCount,GL_UNSIGNED_SHORT, indexArray );
		//LOGI("vertexCount = %d, index =  %d %d",vertexCount,indexArray[0],indexArray[1]);
		//LOGI("0.x = %f, 0.y = %f", vertexattribs[0].x, vertexattribs[0].y);
		//LOGI("1.x = %f, 1.y = %f", vertexattribs[1].x, vertexattribs[1].y);
		//LOGI("0.color = %f, %f, %f, %f", vertexattribs[0].red, vertexattribs[0].green, vertexattribs[0].blue, vertexattribs[0].alpha);
	}
	else if (wrapperPrimitiveMode == GL_QUADS)
	{
		//glDisableClientState( GL_COLOR_ARRAY );
		GLESIMPL glDrawElements( GL_TRIANGLES,vertexCount,GL_UNSIGNED_SHORT, indexArray );
		//LOGI("vertexCount = %d, index =  %d %d",vertexCount,indexArray[0],indexArray[1]);
		//LOGI("0.x = %f, 0.y = %f", vertexattribs[0].x, vertexattribs[0].y);
		//LOGI("1.x = %f, 1.y = %f", vertexattribs[1].x, vertexattribs[1].y);
		//LOGI("0.color = %f, %f, %f, %f", vertexattribs[0].red, vertexattribs[0].green, vertexattribs[0].blue, vertexattribs[0].alpha);
	}
	else
		GLESIMPL glDrawElements( GL_TRIANGLES,vertexCount,GL_UNSIGNED_SHORT, indexArray );
	CHECKGLERROR;


#if defined(__MULTITEXTURE_SUPPORT__)
	useMultiTexCoordArray = GL_FALSE;
#endif
	vertexCount = 0;
	indexCount = 0;
	ptrVertexAttribArray = vertexattribs;
	ptrVertexAttribArrayMark = ptrVertexAttribArray;
	ptrIndexArray = indexArray;
	useTexCoordArray = GL_FALSE;
}

int stackTextureState;
void nanoPushState()
{
	stackTextureState = activetmuState->boundtexture.value;
}

void nanoPopState()
{

	if (tmuState0.color_array.enabled)
	{
		GLESIMPL glEnableClientState(GL_COLOR_ARRAY);
		CHECKGLERROR;
	}
	else
	{
		GLESIMPL glDisableClientState(GL_COLOR_ARRAY);
		CHECKGLERROR;
	}

	if (tmuState0.vertex_array.enabled)
	{
		GLESIMPL glEnableClientState(GL_VERTEX_ARRAY);
		CHECKGLERROR;
	}
	else
	{
		GLESIMPL glDisableClientState(GL_VERTEX_ARRAY);
		CHECKGLERROR;
	}

	if (tmuState0.texture_coord_array.enabled)
	{
		GLESIMPL glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CHECKGLERROR;
	}
	else
	{
		GLESIMPL glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		CHECKGLERROR;
	}
	GLESIMPL glVertexPointer(tmuState0.vertex_array.size,
			tmuState0.vertex_array.type,
			tmuState0.vertex_array.stride,
			tmuState0.vertex_array.ptr);

	GLESIMPL glTexCoordPointer(tmuState0.texture_coord_array.size,
			tmuState0.texture_coord_array.type,
			tmuState0.texture_coord_array.stride,
			tmuState0.texture_coord_array.ptr);

	GLESIMPL glColorPointer(tmuState0.color_array.size,
			tmuState0.color_array.type,
			tmuState0.color_array.stride,
			tmuState0.color_array.ptr);

	GLESIMPL glMatrixMode(nanoglState.matrixmode);

	if (nanoglState.fog) //For Duke3d
		GLESIMPL glEnable(GL_FOG);


	GLESIMPL	glColor4f (currentVertexAttrib.red, currentVertexAttrib.green, currentVertexAttrib.blue, currentVertexAttrib.alpha);

	GLESIMPL glBlendFunc(nanoglState.sfactor, nanoglState.dfactor);

	GLESIMPL glBindTexture(GL_TEXTURE_2D, stackTextureState);
	CHECKGLERROR;

	GLESIMPL glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, activetmuState->texture_env_mode.value);

	arraysValid = GL_FALSE;
}


void nanoglBegin(GLenum mode)
{
	wrapperPrimitiveMode = mode;
	vertexMark = vertexCount;
	ptrVertexAttribArrayMark = ptrVertexAttribArray;
	indexbase = indexCount;
}

void nanoglEnd(void)
{
	int count ;

	vertexCount+=((unsigned char*)ptrVertexAttribArray-(unsigned char*)ptrVertexAttribArrayMark)/sizeof(VertexAttrib);
	if (vertexCount < ((wrapperPrimitiveMode == GL_LINES)?2:3))
	{
		return;
	}
	switch (wrapperPrimitiveMode)
	{
	case GL_LINES:
	{
		int  vcount = (vertexCount-vertexMark)/2;
		for ( count = 0; count < vcount; count++)
		{
			*ptrIndexArray++ = indexCount;
			*ptrIndexArray++ = indexCount+1;
			indexCount+=2;
		}
		//indexCount+=4;
		//vertexCount+=2;
	}
	break;
	case GL_QUADS:
	{
		int  vcount = (vertexCount-vertexMark);
		for (int i = 0; i < vcount * 3 ; i+=6 ) {
			/*
				int q = i / 6 * 4;
				quad_indexes[ i + 0 ] = q + 0;
				quad_indexes[ i + 1 ] = q + 1;
				quad_indexes[ i + 2 ] = q + 2;

				quad_indexes[ i + 3 ] = q + 0;
				quad_indexes[ i + 4 ] = q + 2;
				quad_indexes[ i + 5 ] = q + 3;
			 */
			int q = i / 6 * 4;
			ptrIndexArray[ i + 0 ] = q + 0;
			ptrIndexArray[ i + 1 ] = q + 1;
			ptrIndexArray[ i + 2 ] = q + 2;

			ptrIndexArray[ i + 3 ] = q + 0;
			ptrIndexArray[ i + 4 ] = q + 2;
			ptrIndexArray[ i + 5 ] = q + 3;
			/*
			 *ptrIndexArray++ = indexCount;
			 *ptrIndexArray++ = indexCount+1;
			 *ptrIndexArray++ = indexCount+2;
			 *ptrIndexArray++ = indexCount;
			 *ptrIndexArray++ = indexCount+2;
			 *ptrIndexArray++ = indexCount+3;
			 */
			indexCount+=4;
			//vertexCount+=2;
		}
		vertexCount =  vcount / 4 * 6;
	}
	break;
	case GL_TRIANGLES:
	{
		int  vcount = (vertexCount-vertexMark)/3;
		for ( count = 0; count < vcount; count++)
		{
			*ptrIndexArray++ = indexCount;
			*ptrIndexArray++ = indexCount+1;
			*ptrIndexArray++ = indexCount+2;
			indexCount+=3;
		}
	}
	break;
	case GL_TRIANGLE_STRIP:
	{
		*ptrIndexArray++ = indexCount;
		*ptrIndexArray++ = indexCount+1;
		*ptrIndexArray++ = indexCount+2;
		indexCount+=3;
		int vcount = ((vertexCount-vertexMark)-3);

		if (vcount && ((long)ptrIndexArray & 0x02))
		{
			*ptrIndexArray++ = indexCount-1; // 2
			*ptrIndexArray++ = indexCount-2; // 1
			*ptrIndexArray++ = indexCount;   // 3
			indexCount++;
			vcount-=1;

			/*
                for ( count = 0; count < vcount; count++)
					{
			 *ptrIndexArray++ = indexCount-2; // 2
			 *ptrIndexArray++ = indexCount-1; // 1
			 *ptrIndexArray++ = indexCount;   // 3
				   indexCount++;
				   }
			 */

			// bullshit optimization ....

			int odd = vcount&1;
			vcount/=2;
			unsigned int* longptr = (unsigned int*) ptrIndexArray;

			for ( count = 0; count < vcount; count++)
			{
				*(longptr++) = (indexCount-2) | ((indexCount-1)<<16);
				*(longptr++) = (indexCount) | ((indexCount)<<16);
				*(longptr++) = (indexCount-1) | ((indexCount+1)<<16);
				indexCount+=2;
			}
			ptrIndexArray = (unsigned short*)(longptr);


			if (odd)
			{
				*ptrIndexArray++ = indexCount-2; // 2
				*ptrIndexArray++ = indexCount-1; // 1
				*ptrIndexArray++ = indexCount;   // 3
				indexCount++;
			}

		}
		else
		{


			//already aligned
			int odd = vcount&1;
			vcount/=2;
			unsigned int* longptr = (unsigned int*) ptrIndexArray;

			for ( count = 0; count < vcount; count++)
			{
				*(longptr++) = (indexCount-1) | ((indexCount-2)<<16);
				*(longptr++) = (indexCount) | ((indexCount-1)<<16);
				*(longptr++) = (indexCount) | ((indexCount+1)<<16);
				indexCount+=2;

			}
			ptrIndexArray = (unsigned short*)(longptr);
			if (odd)
			{

				*ptrIndexArray++ = indexCount-1; // 2
				*ptrIndexArray++ = indexCount-2; // 1
				*ptrIndexArray++ = indexCount;   // 3
				indexCount++;
			}


		}
		vertexCount+=(vertexCount-vertexMark-3)*2;
	}


	break;
	case GL_POLYGON:
	case GL_TRIANGLE_FAN:
	{

		*ptrIndexArray++ = indexCount++;
		*ptrIndexArray++ = indexCount++;
		*ptrIndexArray++ = indexCount++;
		int vcount = ((vertexCount-vertexMark)-3);
		for ( count = 0; count < vcount; count++)
		{
			*ptrIndexArray++ = indexbase;
			*ptrIndexArray++ = indexCount-1;
			*ptrIndexArray++ = indexCount++;
			vertexCount+=2;
		}

	}
	break;

	default:
		break;
	}

	// flush after glEnd()
	if (wrapperPrimitiveMode == GL_LINES)
		FlushOnStateChange(); //For gzdoom automap

}

void nanoglEnable (GLenum cap)
{
	GLboolean statechanged = GL_FALSE;
	switch(cap)
	{
	case GL_ALPHA_TEST:
	{
		if (!nanoglState.alpha_test)
		{
			nanoglState.alpha_test = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_BLEND:
	{
		if (!nanoglState.blend)
		{
			nanoglState.blend = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0:
	{
		if (!nanoglState.clip_plane_0)
		{
			nanoglState.clip_plane_0 = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0 + 1:
	{
		if (!nanoglState.clip_plane_1)
		{
			nanoglState.clip_plane_1 = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0 + 2:
	{
		if (!nanoglState.clip_plane_2)
		{
			nanoglState.clip_plane_2 = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0 + 3:
	{
		if (!nanoglState.clip_plane_3)
		{
			nanoglState.clip_plane_3 = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0 + 4:
	{
		if (!nanoglState.clip_plane_4)
		{
			nanoglState.clip_plane_4 = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0 + 5:
	{
		if (!nanoglState.clip_plane_5)
		{
			nanoglState.clip_plane_5 = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_COLOR_LOGIC_OP:
	{
		if (!nanoglState.color_logic_op)
		{
			nanoglState.color_logic_op = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_COLOR_MATERIAL:
	{
		if (!nanoglState.color_material)
		{
			nanoglState.color_material = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CULL_FACE:
	{
		if (!nanoglState.cull_face)
		{
			nanoglState.cull_face = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_DEPTH_TEST:
	{
		if (!nanoglState.depth_test)
		{
			nanoglState.depth_test = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_DITHER:
	{
		if (!nanoglState.dither)
		{
			nanoglState.dither = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_FOG:
		//case GL_LIGHTi
	{
		if (!nanoglState.fog)
		{
			nanoglState.fog = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_LIGHTING:
	{
		if (!nanoglState.lighting)
		{
			nanoglState.lighting = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_LINE_SMOOTH:
	{
		if (!nanoglState.line_smooth)
		{
			nanoglState.line_smooth = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	/*        case GL_MATRIX_PALETTE_OES:
            {
            if (!nanoglState.matrix_palette_oes)
                {
                nanoglState.matrix_palette_oes = GL_TRUE;
                statechanged = GL_TRUE;
                }
            break;
            }*/
	case GL_MULTISAMPLE:
	{
		if (!nanoglState.multisample)
		{
			nanoglState.multisample = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_NORMALIZE:
	{
		if (!nanoglState.normalize)
		{
			nanoglState.normalize = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	/*        case GL_POINT_SPRITE_OES:
            {
            if (!nanoglState.point_sprite_oes)
                {
                nanoglState.point_sprite_oes = GL_TRUE;
                statechanged = GL_TRUE;
                }
            break;
            }*/
	case GL_POLYGON_OFFSET_FILL:
	{
		if (!nanoglState.polygon_offset_fill)
		{
			nanoglState.polygon_offset_fill = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_RESCALE_NORMAL:
	{
		if (!nanoglState.rescale_normal)
		{
			nanoglState.rescale_normal = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_SAMPLE_ALPHA_TO_COVERAGE:
	{
		if (!nanoglState.sample_alpha_to_coverage)
		{
			nanoglState.sample_alpha_to_coverage = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_SAMPLE_ALPHA_TO_ONE:
	{
		if (!nanoglState.sample_alpha_to_one)
		{
			nanoglState.sample_alpha_to_one = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_SAMPLE_COVERAGE:
	{
		if (!nanoglState.sample_coverage)
		{
			nanoglState.sample_coverage = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_SCISSOR_TEST:
	{
		if (!nanoglState.scissor_test)
		{
			nanoglState.scissor_test = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_STENCIL_TEST:
	{

		if (!nanoglState.stencil_test)
		{
			nanoglState.stencil_test = GL_TRUE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_TEXTURE_2D:
	{
		if (!activetmuState->texture_2d.value)
		{
			FlushOnStateChange();
			GLESIMPL glEnable(cap);
			CHECKGLERROR;
			activetmuState->texture_2d.value = GL_TRUE;
			return;
		}
		break;
	}
	default:
		LOGI("Enable 0x%0x",cap);
		FatalError("Unhandeled glEnable ");
		statechanged = GL_TRUE;
		break;
	}

	if (statechanged)
	{
		FlushOnStateChange();
		GLESIMPL glEnable(cap);
		CHECKGLERROR;
	}
}

void nanoglDisable (GLenum cap)
{
	GLboolean statechanged = GL_FALSE;
	switch(cap)
	{
	case GL_ALPHA_TEST:
	{
		if (nanoglState.alpha_test)
		{
			nanoglState.alpha_test = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_BLEND:
	{
		if (nanoglState.blend)
		{
			nanoglState.blend = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0:
	{
		if (nanoglState.clip_plane_0)
		{
			nanoglState.clip_plane_0 = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0 + 1:
	{
		if (nanoglState.clip_plane_1)
		{
			nanoglState.clip_plane_1 = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0 + 2:
	{
		if (nanoglState.clip_plane_2)
		{
			nanoglState.clip_plane_2 = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0 + 3:
	{
		if (nanoglState.clip_plane_3)
		{
			nanoglState.clip_plane_3 = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0 + 4:
	{
		if (nanoglState.clip_plane_4)
		{
			nanoglState.clip_plane_4 = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CLIP_PLANE0 + 5:
	{
		if (nanoglState.clip_plane_5)
		{
			nanoglState.clip_plane_5 = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_COLOR_LOGIC_OP:
	{
		if (nanoglState.color_logic_op)
		{
			nanoglState.color_logic_op = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_COLOR_MATERIAL:
	{
		if (nanoglState.color_material)
		{
			nanoglState.color_material = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_CULL_FACE:
	{
		if (nanoglState.cull_face)
		{
			nanoglState.cull_face = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_DEPTH_TEST:
	{
		if (nanoglState.depth_test)
		{
			nanoglState.depth_test = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_DITHER:
	{
		if (nanoglState.dither)
		{
			nanoglState.dither = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_FOG:
		//case GL_LIGHTi
	{
		if (nanoglState.fog)
		{
			nanoglState.fog = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_LIGHTING:
	{
		if (nanoglState.lighting)
		{
			nanoglState.lighting = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_LINE_SMOOTH:
	{
		if (nanoglState.line_smooth)
		{
			nanoglState.line_smooth = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	/*        case GL_MATRIX_PALETTE_OES:
            {
            if (nanoglState.matrix_palette_oes)
                {
                nanoglState.matrix_palette_oes = GL_FALSE;
                statechanged = GL_TRUE;
                }
            break;
            }*/
	case GL_MULTISAMPLE:
	{
		if (nanoglState.multisample)
		{
			nanoglState.multisample = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_NORMALIZE:
	{
		if (nanoglState.normalize)
		{
			nanoglState.normalize = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	/*        case GL_POINT_SPRITE_OES:
            {
            if (nanoglState.point_sprite_oes)
                {
                nanoglState.point_sprite_oes = GL_FALSE;
                statechanged = GL_TRUE;
                }
            break;
            }*/
	case GL_POLYGON_OFFSET_FILL:
	{
		if (nanoglState.polygon_offset_fill)
		{
			nanoglState.polygon_offset_fill = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_RESCALE_NORMAL:
	{
		if (nanoglState.rescale_normal)
		{
			nanoglState.rescale_normal = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_SAMPLE_ALPHA_TO_COVERAGE:
	{
		if (nanoglState.sample_alpha_to_coverage)
		{
			nanoglState.sample_alpha_to_coverage = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_SAMPLE_ALPHA_TO_ONE:
	{
		if (nanoglState.sample_alpha_to_one)
		{
			nanoglState.sample_alpha_to_one = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_SAMPLE_COVERAGE:
	{
		if (nanoglState.sample_coverage)
		{
			nanoglState.sample_coverage = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_SCISSOR_TEST:
	{
		if (nanoglState.scissor_test)
		{
			nanoglState.scissor_test = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_STENCIL_TEST:
	{
		if (nanoglState.stencil_test)
		{
			nanoglState.stencil_test = GL_FALSE;
			statechanged = GL_TRUE;
		}
		break;
	}
	case GL_TEXTURE_2D:
	{
		if (activetmuState->texture_2d.value)
		{
			FlushOnStateChange();
			GLESIMPL glDisable(cap);
			CHECKGLERROR;
			activetmuState->texture_2d.value = GL_FALSE;
			return;


		}
		break;
	}
	default:
		LOGI("Disable 0x%0x",cap);
		FatalError("Unhandeled disable ");
		statechanged = GL_TRUE;
		break;
	}

	if (statechanged)
	{
		FlushOnStateChange();
		GLESIMPL glDisable(cap);
		CHECKGLERROR;
	}
}

void nanoglVertex2i(GLint x, GLint y)
{
	nanoglVertex3f((float)x,(float)y,0.0f);
}

void nanoglVertex2f(GLfloat x, GLfloat y)
{
	nanoglVertex3f(x,y,0.0f);
}
void nanoglVertex2d(GLdouble x, GLdouble y)
{
	nanoglVertex2f((GLfloat)x,(GLfloat)y);
}

static inline unsigned char to_byte(float value)
{
	int retval = ( int)(value *255.0f);
	if (retval > 255)
	{
		retval = 255;
	}
	return (unsigned char)retval;
}

void nanoglBlendEquation(GLenum mode)
{
	//GLESIMPL glBlendEquation(mode);
	CHECKGLERROR;
}

void nanoglColor3f(	GLfloat red, GLfloat green, GLfloat blue)
{
	nanoglColor4f( red, green, blue, 1.0f );
}

void nanoglTexCoord2fv( const GLfloat *v )
{
	nanoglTexCoord2f( v[0], v[1] );
}

void nanoglTexCoord2d(GLdouble s, GLdouble t)
{
	nanoglTexCoord2f(s,t);
}

void nanoglTexCoord2f(GLfloat s, GLfloat t)
{
	currentVertexAttrib.s = s;
	currentVertexAttrib.t = t;
}

void nanoglViewport (GLint x, GLint y, GLsizei width, GLsizei height)
{
	FlushOnStateChange();
	GLESIMPL glViewport(x,y,width,height);
	CHECKGLERROR;
}

void nanoglLoadIdentity (void)
{
	FlushOnStateChange();
	GLESIMPL glLoadIdentity();
	CHECKGLERROR;
}


void nanoglColor4b (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
{
	//LOGI("nanoglColor4b %d %d %d %d",red,green,blue,alpha);
	nanoglColor4f((float)red/(float)255,(float)green/(float)255,(float)blue/(float)255,(float)alpha/(float)255);
}

void nanoglColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
#if	COLOR_BYTE
	currentVertexAttrib.red = to_byte(red);
	currentVertexAttrib.green = to_byte(green);
	currentVertexAttrib.blue = to_byte(blue);
	currentVertexAttrib.alpha = to_byte(alpha);
#else
	currentVertexAttrib.red = (red);
	currentVertexAttrib.green = (green);
	currentVertexAttrib.blue = (blue);
	currentVertexAttrib.alpha = (alpha);
#endif
	glColor4f (red, green, blue, alpha);
}

void nanoglOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	FlushOnStateChange();
#if USE_GLES
	GLESIMPL glOrthof(left,right,bottom,top, zNear,zFar);
	CHECKGLERROR;
#else
	GLESIMPL glOrtho(left,right,bottom,top, zNear,zFar);
	CHECKGLERROR;
#endif
}



void nanoglMatrixMode (GLenum mode)
{
	if (nanoglState.matrixmode == mode)
	{
		return;
	}
	nanoglState.matrixmode = mode;
	FlushOnStateChange();
	GLESIMPL glMatrixMode(nanoglState.matrixmode);
	CHECKGLERROR;
}

void nanoglTexParameterf (GLenum target, GLenum pname, GLfloat param)
{
	if (pname == GL_TEXTURE_BORDER_COLOR)
	{
		return; // not supported by opengl es
	}
	if (    (pname == GL_TEXTURE_WRAP_S ||
			pname == GL_TEXTURE_WRAP_T) &&
			param == GL_CLAMP)
	{
		param = GL_CLAMP_TO_EDGE;
	}

	FlushOnStateChange();
	GLESIMPL glTexParameterf(target, pname,param);
	CHECKGLERROR;
}

void nanoglTexParameteri (GLenum target, GLenum pname, GLint param)
{
	if (pname == GL_TEXTURE_BORDER_COLOR)
	{
		return; // not supported by opengl es
	}
	if (    (pname == GL_TEXTURE_WRAP_S ||
			pname == GL_TEXTURE_WRAP_T) &&
			param == GL_CLAMP)
	{
		param = GL_CLAMP_TO_EDGE;
	}

	FlushOnStateChange();
	GLESIMPL glTexParameteri(target, pname,param);
	CHECKGLERROR;
}

void nanoglTexParameterfv(	GLenum target, GLenum pname, const GLfloat *params)
{
	FlushOnStateChange();
	//TODO
	nanoglTexParameterf(target, pname, params[0]);
	CHECKGLERROR;
}

void nanoglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
	FlushOnStateChange();
	//internalformat = format;
	if (internalformat == GL_LUMINANCE)
	{
		format = GL_LUMINANCE;
		//internalformat = GL_RGB;
		//format =GL_RGB;
	}
	else
	{
		if (internalformat != format)
		{
			internalformat = format;
		}
	}
	GLESIMPL glTexImage2D(target, level, internalformat, width, height,border,format,type,pixels);
	CHECKGLERROR;
}

void nanoglDrawBuffer(GLenum mode)
{
	FatalError("nanoglDrawBuffer not imp");
}

void nanoglTranslatef (GLfloat x, GLfloat y, GLfloat z)
{
	FlushOnStateChange();
	GLESIMPL glTranslatef(x,y,z);
}

void nanoglRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	FlushOnStateChange();
	GLESIMPL glRotatef(angle, x, y, z);
}

void nanoglScalef (GLfloat x, GLfloat y, GLfloat z)
{
	FlushOnStateChange();
	GLESIMPL glScalef(x,y,z);
}

//void nanoglDepthRange(GLclampf zNear, GLclampf zFar)
void nanoglDepthRange(GLclampd zNear, GLclampd zFar)
{
	if ((nanoglState.depth_range_near == zNear) &&(nanoglState.depth_range_far == zFar))
	{
		return;
	}
	else
	{
		nanoglState.depth_range_near = zNear;
		nanoglState.depth_range_far = zFar;
	}
	FlushOnStateChange();
#if USE_GLES
	GLfloat n = zNear;
	GLfloat f = zFar;

	GLESIMPL glDepthRangef(n, f);
	CHECKGLERROR;
#else
	GLESIMPL glDepthRange(zNear, zFar);
	CHECKGLERROR;
#endif
}

void nanoglDepthFunc (GLenum func)
{
	if (nanoglState.depth_func == func)
	{
		return;
	}
	else
	{
		nanoglState.depth_func = func;
	}
	FlushOnStateChange();
	GLESIMPL glDepthFunc(func);
	CHECKGLERROR;
}

void nanoglFinish (void)
{
	FlushOnStateChange();
	GLESIMPL glFinish();
}

/* GL_OES_matrix_get */
#define GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES               0x898D
#define GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES              0x898E
#define GL_TEXTURE_MATRIX_FLOAT_AS_INT_BITS_OES                 0x898F


void nanoglGetFloatv (GLenum pname, GLfloat *params)
{
	FlushOnStateChange();

	GLESIMPL glGetFloatv(pname, params);
	CHECKGLERROR;

}

void nanoglGetDoublev (GLenum pname, GLdouble *params)
{
	FlushOnStateChange();

	GLfloat p[16];
	GLESIMPL glGetFloatv(pname, p);
	CHECKGLERROR;

	//TODO allow for more values
	if ((pname == GL_PROJECTION_MATRIX) ||
			(pname == GL_MODELVIEW_MATRIX))
	{
		for (int n=0;n<16;n++)
			params[n] = (GLdouble)p[n];
	}
	else
	{
		LOGI("EXITING nanoglGetDoublev %d",pname);
		*(int *)0 = 0;
		params[0] = p[0];
	}

}

void nanoglCullFace (GLenum mode)
{
	if (nanoglState.cullface == mode)
	{
		return;
	}
	else
	{
		nanoglState.cullface = mode;
	}
	FlushOnStateChange();
	GLESIMPL glCullFace(mode);
	CHECKGLERROR;
}

void nanoglFrustum ( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
//void nanoglFrustum (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
	FlushOnStateChange();
#if USE_GLES
	GLESIMPL glFrustumf(left,right,bottom,top,zNear,zFar);
	CHECKGLERROR;
#else
	GLESIMPL glFrustum(left,right,bottom,top,zNear,zFar);
	CHECKGLERROR;
#endif
}

void nanoglClear (GLbitfield mask)
{
	//LOGI("nanoglClear");
	//FlushOnStateChange();
	GLESIMPL glClear(mask);
	CHECKGLERROR;
}

void nanoglVertex3d( GLdouble x, GLdouble y, GLdouble z )
{
	nanoglVertex3f(x,y,z);

}

void nanoglVertex3f( GLfloat x, GLfloat y, GLfloat z )
{


	currentVertexAttrib.x = x;
	currentVertexAttrib.y = y;
	currentVertexAttrib.z = z;

	ptrVertexAttribArray->x = currentVertexAttrib.x;
	ptrVertexAttribArray->y = currentVertexAttrib.y;
	ptrVertexAttribArray->z = currentVertexAttrib.z;

	ptrVertexAttribArray->red = currentVertexAttrib.red;
	ptrVertexAttribArray->green = currentVertexAttrib.green;
	ptrVertexAttribArray->blue = currentVertexAttrib.blue;
	ptrVertexAttribArray->alpha = currentVertexAttrib.alpha;

	ptrVertexAttribArray->s = currentVertexAttrib.s;
	ptrVertexAttribArray->t = currentVertexAttrib.t;
#if defined(__MULTITEXTURE_SUPPORT__)
	ptrVertexAttribArray->s_multi = currentVertexAttrib.s_multi;
	ptrVertexAttribArray->t_multi = currentVertexAttrib.t_multi;
#endif
	ptrVertexAttribArray ++;
}

void nanoglColor4fv( const GLfloat *v )
{
	nanoglColor4f(  v[0], v[1], v[2], v[3] );
}



void nanoglColor4ub( GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha )
{
#if	COLOR_BYTE
	currentVertexAttrib.red = red;
	currentVertexAttrib.green = green;
	currentVertexAttrib.blue = blue;
	currentVertexAttrib.alpha = alpha;
#else
	/*static*/ float inv_255 = 1.0f/ 255.0f;
	currentVertexAttrib.red = (float)red * inv_255;
	currentVertexAttrib.green = (float)green * inv_255;
	currentVertexAttrib.blue = (float)blue * inv_255;
	currentVertexAttrib.alpha = (float)alpha * inv_255;
#endif

}

void nanoglColor3ub (GLubyte red, GLubyte green, GLubyte blue)
{
#if	COLOR_BYTE
	currentVertexAttrib.red = red;
	currentVertexAttrib.green = green;
	currentVertexAttrib.blue = blue;
	currentVertexAttrib.alpha = 1;
#else
	/*static*/ float inv_255 = 1.0f/ 255.0f;
	currentVertexAttrib.red = (float)red * inv_255;
	currentVertexAttrib.green = (float)green * inv_255;
	currentVertexAttrib.blue = (float)blue * inv_255;
	currentVertexAttrib.alpha = (float)1;
#endif
}

void nanoglColor3ubv( const GLubyte* v)
{
	nanoglColor4ub( v[0], v[1], v[2], 255 );
}

void nanoglColor4ubv( const GLubyte *v )
{
	nanoglColor4ub( v[0], v[1], v[2], v[3] );
}

void nanoglColor3fv( const GLfloat *v )
{
	nanoglColor3f( v[0], v[1], v[2] );
}



void nanoglHint (GLenum target, GLenum mode)
{
	FlushOnStateChange();
	GLESIMPL glHint(target, mode);
	CHECKGLERROR;
}

void nanoglBlendFunc (GLenum sfactor, GLenum dfactor)
{
	if ((nanoglState.sfactor == sfactor) && (nanoglState.dfactor == dfactor))
	{
		return;
	}
	nanoglState.sfactor = sfactor;
	nanoglState.dfactor = dfactor;
	FlushOnStateChange();
	GLESIMPL glBlendFunc(sfactor, dfactor);
	CHECKGLERROR;
}

void nanoglPopMatrix (void)
{
	FlushOnStateChange();
	GLESIMPL glPopMatrix();
}

void nanoglShadeModel (GLenum mode)
{
	if (nanoglState.shademodel == mode)
	{
		return;
	}
	nanoglState.shademodel = mode;
	FlushOnStateChange();
	GLESIMPL glShadeModel(mode);
	CHECKGLERROR;
}

void nanoglPushMatrix (void)
{
	FlushOnStateChange();
	GLESIMPL glPushMatrix();
}

void nanoglTexEnvf (GLenum target, GLenum pname, GLfloat param)
{
	if (target == GL_TEXTURE_ENV)
	{
		if (pname == GL_TEXTURE_ENV_MODE)
		{
			if (param == activetmuState->texture_env_mode.value)
			{
				return;
			}
			else
			{
				FlushOnStateChange();
				GLESIMPL glTexEnvf(target, pname, param);
				CHECKGLERROR;
				activetmuState->texture_env_mode.value = param;
				return;
			}
		}
	}
	FlushOnStateChange();
	GLESIMPL glTexEnvf(target, pname, param);
	CHECKGLERROR;
}
void nanoglTexEnvi (GLenum target, GLenum pname, GLint param)
{
	if (target == GL_TEXTURE_ENV)
	{
		if (pname == GL_TEXTURE_ENV_MODE)
		{
			if (param == activetmuState->texture_env_mode.value)
			{
				return;
			}
			else
			{
				FlushOnStateChange();
				GLESIMPL glTexEnvi(target, pname, param);
				CHECKGLERROR;
				activetmuState->texture_env_mode.value = param;
				return;
			}
		}
	}
	FlushOnStateChange();
	GLESIMPL glTexEnvi(target, pname, param);
	CHECKGLERROR;
}

void nanoglVertex3fv( const GLfloat *v )
{
	nanoglVertex3f( v[0], v[1], v[2] );
}


void nanoglDepthMask (GLboolean flag)
{
	if (nanoglState.depthmask == flag)
	{
		return;
	}
	nanoglState.depthmask = flag;
	FlushOnStateChange();
	GLESIMPL glDepthMask(flag);
	CHECKGLERROR;
}

void nanoglBindTexture (GLenum target, GLuint texture)
{
	if (activetmuState->boundtexture.value == texture)
	{
		return;
	}
	FlushOnStateChange();
	activetmuState->boundtexture.value = texture;
	GLESIMPL glBindTexture(target, texture);
}


void nanoglGetIntegerv (GLenum pname, GLint *params)
{
	FlushOnStateChange();
	GLESIMPL glGetIntegerv(pname, params);
	CHECKGLERROR;
}

GLubyte nano_extensions_string[4096];
const GLubyte* nanoglGetString (GLenum name)
{

	if (name == GL_EXTENSIONS)
	{
#if defined(__MULTITEXTURE_SUPPORT__)
		sprintf((char*)nano_extensions_string,"%s %s",GLESIMPL glGetString(name),"GL_ARB_multitexture EXT_texture_env_add");
#else
		sprintf((char*)nano_extensions_string,"%s %s",GLESIMPL glGetString(name),"EXT_texture_env_add");
#endif
		return nano_extensions_string;
	}
	return GLESIMPL glGetString(name);
}

void nanoglAlphaFunc (GLenum func, GLclampf ref)
{
	FlushOnStateChange();
	GLESIMPL glAlphaFunc(func,ref);
	CHECKGLERROR;
}

void nanoglFlush (void)
{
	FlushOnStateChange();
	GLESIMPL glFlush();
}

void nanoglReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)
{
	if (format == GL_DEPTH_COMPONENT)
	{
		// OpenglEs 1.1 does not support reading depth buffer without an extension
		memset(pixels, 0xff,4);
		return;
	}
	FlushOnStateChange();
	GLESIMPL glReadPixels(x,y,width,height,format,type,pixels);
	CHECKGLERROR;
}

void nanoglReadBuffer( GLenum mode )
{
	FatalError("nanoglReadBuffer not imp");
}

void nanoglLoadMatrixf (const GLfloat *m)
{
	FlushOnStateChange();
	GLESIMPL glLoadMatrixf(m);
	CHECKGLERROR;
}

void nanoglTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
	FlushOnStateChange();
	GLESIMPL glTexSubImage2D(target,level,xoffset,yoffset,width,height,format,type,pixels);
	CHECKGLERROR;
}

void nanoglClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	FlushOnStateChange();
	GLESIMPL glClearColor(red,green,blue,alpha);
}

GLenum nanoglGetError (void)
{
	FlushOnStateChange();
	return GL_NO_ERROR;//GLESIMPL glGetError();
}

void nanoglActiveTexture (GLenum texture)
{
	if (activetmu == texture)
	{
		return;
	}
	if (delayedttmuchange)
	{
		delayedttmuchange = GL_FALSE;
	}
	else
	{
		delayedttmuchange = GL_TRUE;
		delayedtmutarget = texture;
	}
	if (texture == GL_TEXTURE0)
	{
		activetmuState = &tmuState0;
	}
	else
	{
		activetmuState = &tmuState1;
	}
	activetmu = texture;
}

void nanoglClientActiveTexture (GLenum texture)
{
	clientactivetmu = texture;
}

void nanoglPolygonMode( GLenum face, GLenum mode )
{
	//FatalError("NO glPolygonMode");
	LOGI("NO glPolygonMode");
	//FlushOnStateChange();
	//GLESIMPL glPolygonMode(face,mode);
}

void nanoglDeleteTextures( GLsizei n, const GLuint *textures )
{
	FlushOnStateChange();
	GLESIMPL glDeleteTextures(n,textures);
	CHECKGLERROR;
}

//void nanoglClearDepth( GLclampf depth )
void nanoglClearDepth( GLclampd depth )
{
	FlushOnStateChange();
#if USE_GLES
	GLESIMPL glClearDepthf( depth );
#else
	GLESIMPL glClearDepth( depth );
#endif
}


void nanoglClipPlane( GLenum plane, const GLdouble *equation )
{

	FlushOnStateChange();
#if USE_GLES
	float array[4];
	array[0] = (GLfloat)(equation[0]);
	array[1] = (GLfloat)(equation[1]);
	array[2] = (GLfloat)(equation[2]);
	array[3] = (GLfloat)(equation[3]);
	GLESIMPL glClipPlanef( plane, array );
#else
	GLESIMPL glClipPlane( plane, equation );
#endif
	CHECKGLERROR;
}

void nanoglScissor( GLint x, GLint	y, GLsizei width, GLsizei height )
{
	FlushOnStateChange();
	GLESIMPL glScissor( x, y, width,height);
	CHECKGLERROR;
}

void nanoglPointSize( GLfloat size )
{
	FlushOnStateChange();
	GLESIMPL glPointSize( size );
	CHECKGLERROR;
}

void nanoglArrayElement(GLint i)
{
	FatalError("nanoglArrayElement not imp");
}
void nanoglLineWidth(GLfloat width)
{
	glLineWidth(width);
	//FatalError("nanoglLineWidth not imp");
}
void nanoglCallList( GLuint list )
{
	FatalError("nanoglCallList not imp");
}
void nanoglColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha )
{
	FlushOnStateChange();
	GLESIMPL glColorMask( red,green,blue,alpha );
	//GLESIMPL glColorMask( GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE );
	CHECKGLERROR;
}
void nanoglStencilFunc( GLenum func, GLint ref, GLuint mask )
{
	FlushOnStateChange();
	GLESIMPL glStencilFunc( func,ref,mask);
	CHECKGLERROR;
}

void nanoglStencilOp( GLenum fail, GLenum zfail, GLenum zpass )
{
	FlushOnStateChange();
	GLESIMPL glStencilOp( fail,zfail,zpass);
	CHECKGLERROR;
}

struct ptrstate vertex_array;
struct ptrstate color_array;
struct ptrstate texture_coord_array;

void nanoglDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices )
{
	// ensure that all primitives specified between glBegin/glEnd pairs
	// are rendered first, and that we have correct tmu in use..
	FlushOnStateChange();
	// setup correct vertex/color/texcoord pointers
	if (arraysValid ||
			tmuState0.vertex_array.changed ||
			tmuState0.color_array.changed ||
			tmuState0.texture_coord_array.changed)
	{
		GLESIMPL glClientActiveTexture(GL_TEXTURE0);
		CHECKGLERROR;
	}
	if (arraysValid || tmuState0.vertex_array.changed)
	{
		if (tmuState0.vertex_array.enabled)
		{
			GLESIMPL glEnableClientState(GL_VERTEX_ARRAY);
			CHECKGLERROR;
		}
		else
		{
			GLESIMPL glDisableClientState(GL_VERTEX_ARRAY);
			CHECKGLERROR;
		}
		GLESIMPL glVertexPointer(tmuState0.vertex_array.size,
				tmuState0.vertex_array.type,
				tmuState0.vertex_array.stride,
				tmuState0.vertex_array.ptr);
		tmuState0.vertex_array.changed = GL_FALSE;
	}
	if (arraysValid || tmuState0.color_array.changed)
	{
		if (tmuState0.color_array.enabled)
		{
			GLESIMPL glEnableClientState(GL_COLOR_ARRAY);
			CHECKGLERROR;
		}
		else
		{
			GLESIMPL glDisableClientState(GL_COLOR_ARRAY);
			CHECKGLERROR;
		}
		GLESIMPL glColorPointer(tmuState0.color_array.size,
				tmuState0.color_array.type,
				tmuState0.color_array.stride,
				tmuState0.color_array.ptr);
		CHECKGLERROR;
		tmuState0.color_array.changed = GL_FALSE;
	}
	if (arraysValid || tmuState0.texture_coord_array.changed)
	{
		tmuState0.texture_coord_array.changed = GL_FALSE;
		if (tmuState0.texture_coord_array.enabled)
		{
			GLESIMPL glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKGLERROR;
		}
		else
		{
			GLESIMPL glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKGLERROR;
		}
		GLESIMPL glTexCoordPointer(tmuState0.texture_coord_array.size,
				tmuState0.texture_coord_array.type,
				tmuState0.texture_coord_array.stride,
				tmuState0.texture_coord_array.ptr);
		CHECKGLERROR;
	}

	if (arraysValid || tmuState1.texture_coord_array.changed)
	{
		tmuState1.texture_coord_array.changed = GL_FALSE;
		GLESIMPL glClientActiveTexture(GL_TEXTURE1);
		CHECKGLERROR;
		if (tmuState1.texture_coord_array.enabled)
		{
			GLESIMPL glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKGLERROR;
		}
		else
		{
			GLESIMPL glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKGLERROR;
		}
		GLESIMPL glTexCoordPointer(tmuState1.texture_coord_array.size,
				tmuState1.texture_coord_array.type,
				tmuState1.texture_coord_array.stride,
				tmuState1.texture_coord_array.ptr);
		CHECKGLERROR;
	}

	arraysValid = GL_FALSE;

	GLESIMPL glDrawElements(mode, count, type, indices);
	CHECKGLERROR;
}

void nanoglEnableClientState(GLenum  array)
{
	nanotmuState* clientstate = NULL;
	if (clientactivetmu == GL_TEXTURE0)
	{
		clientstate = &tmuState0;
	}
	else if (clientactivetmu == GL_TEXTURE1)
	{
		clientstate = &tmuState1;
	}
	else
	{
		FatalError("nanoglEnableClientState clientactivetmu?");
		return;
	}
	switch (array)
	{
	case GL_VERTEX_ARRAY:
		if (clientstate->vertex_array.enabled)
		{
			return;
		}
		clientstate->vertex_array.enabled = GL_TRUE;
		clientstate->vertex_array.changed = GL_TRUE;
		break;
	case GL_COLOR_ARRAY:
		if (clientstate->color_array.enabled)
		{
			return;
		}
		clientstate->color_array.enabled = GL_TRUE;
		clientstate->color_array.changed = GL_TRUE;

		break;
	case GL_TEXTURE_COORD_ARRAY:
		if (clientstate->texture_coord_array.enabled)
		{
			return;
		}
		clientstate->texture_coord_array.enabled = GL_TRUE;
		clientstate->texture_coord_array.changed = GL_TRUE;
		break;
	default:
		break;
	}
}
void nanoglDisableClientState(GLenum  array)
{
	nanotmuState* clientstate = NULL;
	if (clientactivetmu == GL_TEXTURE0)
	{
		clientstate = &tmuState0;
	}
	else if (clientactivetmu == GL_TEXTURE1)
	{
		clientstate = &tmuState1;
	}
	else
	{
		return;
	}
	switch (array)
	{
	case GL_VERTEX_ARRAY:
		if (!clientstate->vertex_array.enabled)
		{
			return;
		}
		clientstate->vertex_array.enabled = GL_FALSE;
		clientstate->vertex_array.changed = GL_TRUE;
		break;
	case GL_COLOR_ARRAY:
		if (!clientstate->color_array.enabled)
		{
			return;
		}
		clientstate->color_array.enabled = GL_FALSE;
		clientstate->color_array.changed = GL_TRUE;

		break;
	case GL_TEXTURE_COORD_ARRAY:
		if (!clientstate->texture_coord_array.enabled)
		{
			return;
		}
		clientstate->texture_coord_array.enabled = GL_FALSE;
		clientstate->texture_coord_array.changed = GL_TRUE;
		break;
	default:
		break;
	}
}
void nanoglVertexPointer(	GLint size,	GLenum type,GLsizei	stride,	const GLvoid *pointer )
{
	if (tmuState0.vertex_array.size == size &&
			tmuState0.vertex_array.stride == stride &&
			tmuState0.vertex_array.type == type &&
			tmuState0.vertex_array.ptr == pointer)
	{
		return;
	}
	tmuState0.vertex_array.size = size;
	tmuState0.vertex_array.stride = stride;
	tmuState0.vertex_array.type = type;
	tmuState0.vertex_array.ptr  = (GLvoid*)pointer;
	tmuState0.vertex_array.changed = GL_TRUE;
}
void nanoglTexCoordPointer( GLint	size,  GLenum type,  GLsizei stride,  const	GLvoid *pointer	)
{
	nanotmuState* clientstate = NULL;
	if (clientactivetmu == GL_TEXTURE0)
	{
		clientstate = &tmuState0;
	}
	else if (clientactivetmu == GL_TEXTURE1)
	{
		clientstate = &tmuState1;
	}
	if (clientstate->texture_coord_array.size == size &&
			clientstate->texture_coord_array.stride == stride &&
			clientstate->texture_coord_array.type == type &&
			clientstate->texture_coord_array.ptr == pointer)
	{
		return;
	}
	clientstate->texture_coord_array.size = size;
	clientstate->texture_coord_array.stride = stride;
	clientstate->texture_coord_array.type = type;
	clientstate->texture_coord_array.ptr  = (GLvoid*)pointer;
	clientstate->texture_coord_array.changed = GL_TRUE;
}
void nanoglColorPointer( GLint size,  GLenum type,  GLsizei stride,  const GLvoid *pointer )
{
	if (tmuState0.color_array.size == size &&
			tmuState0.color_array.stride == stride &&
			tmuState0.color_array.type == type &&
			tmuState0.color_array.ptr == pointer)
	{
		return;
	}
	tmuState0.color_array.size = size;
	tmuState0.color_array.stride = stride;
	tmuState0.color_array.type = type;
	tmuState0.color_array.ptr  = (GLvoid*)pointer;
	tmuState0.color_array.changed = GL_TRUE;
}
void nanoglPolygonOffset( GLfloat factor, GLfloat units )
{
	FlushOnStateChange();
	GLESIMPL glPolygonOffset(factor, units);
	CHECKGLERROR;
}
void nanoglStencilMask( GLuint mask )
{
	FlushOnStateChange();
	glStencilMask(mask);
	CHECKGLERROR;
}

void nanoglClearStencil( GLint s )
{
	FlushOnStateChange();
	glClearStencil(s);
	CHECKGLERROR;
}


#if defined(__MULTITEXTURE_SUPPORT__)

void nanoglMultiTexCoord2fv ( GLenum target, const GLfloat* v)
{
	nanoglMultiTexCoord2fARB(target,v[0],v[1]);
}

void nanoglMultiTexCoord2dARB( GLenum target, GLdouble s, GLdouble t )
{
	nanoglMultiTexCoord2fARB(target,(GLfloat)s,(GLfloat)t);
}



void nanoglMultiTexCoord2fARB( GLenum target, GLfloat s, GLfloat t )
{
	if (target == GL_TEXTURE0)
	{
		nanoglTexCoord2f(s,t);
	}
	else
	{
		currentVertexAttrib.s_multi = s;
		currentVertexAttrib.t_multi = t;
	}
}
#else

void nanoglMultiTexCoord2fARB( GLenum target, GLfloat s, GLfloat t )
{
	FatalError("nanoglMultiTexCoord2fARB DISABLED");
}

#endif


void nanoglMultMatrixd (const GLdouble *m)
{

	FlushOnStateChange();

	GLfloat f[16];
	for (int n=0;n<16;n++)
		f[n] = (GLfloat)m[n];

	GLESIMPL glMultMatrixf (f);
	CHECKGLERROR;
}

void nanoglMultMatrixf (const GLfloat *m)
{
	FlushOnStateChange();
	GLESIMPL glMultMatrixf (m);
	CHECKGLERROR;
}

void nanoglDrawArrays (GLenum mode, GLint first, GLsizei count)
{

	FlushOnStateChange();
	// setup correct vertex/color/texcoord pointers
	if (arraysValid ||
			tmuState0.vertex_array.changed ||
			tmuState0.color_array.changed ||
			tmuState0.texture_coord_array.changed)
	{
		GLESIMPL glClientActiveTexture(GL_TEXTURE0);
		CHECKGLERROR;
	}
	if (arraysValid || tmuState0.vertex_array.changed)
	{
		if (tmuState0.vertex_array.enabled)
		{
			GLESIMPL glEnableClientState(GL_VERTEX_ARRAY);
			CHECKGLERROR;
		}
		else
		{
			GLESIMPL glDisableClientState(GL_VERTEX_ARRAY);
			CHECKGLERROR;
		}
		GLESIMPL glVertexPointer(tmuState0.vertex_array.size,
				tmuState0.vertex_array.type,
				tmuState0.vertex_array.stride,
				tmuState0.vertex_array.ptr);
		tmuState0.vertex_array.changed = GL_FALSE;
	}
	if (arraysValid || tmuState0.color_array.changed)
	{
		if (tmuState0.color_array.enabled)
		{
			GLESIMPL glEnableClientState(GL_COLOR_ARRAY);
			CHECKGLERROR;
		}
		else
		{
			GLESIMPL glDisableClientState(GL_COLOR_ARRAY);
			CHECKGLERROR;
		}

		GLESIMPL glColorPointer(tmuState0.color_array.size,
				tmuState0.color_array.type,
				tmuState0.color_array.stride,
				tmuState0.color_array.ptr);
		CHECKGLERROR;

		tmuState0.color_array.changed = GL_FALSE;
	}
	if (arraysValid || tmuState0.texture_coord_array.changed)
	{
		tmuState0.texture_coord_array.changed = GL_FALSE;
		if (tmuState0.texture_coord_array.enabled)
		{
			GLESIMPL glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKGLERROR;
		}
		else
		{
			GLESIMPL glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKGLERROR;
		}
		GLESIMPL glTexCoordPointer(tmuState0.texture_coord_array.size,
				tmuState0.texture_coord_array.type,
				tmuState0.texture_coord_array.stride,
				tmuState0.texture_coord_array.ptr);
		CHECKGLERROR;
	}

	if (arraysValid || tmuState1.texture_coord_array.changed)
	{
		tmuState1.texture_coord_array.changed = GL_FALSE;
		GLESIMPL glClientActiveTexture(GL_TEXTURE1);
		CHECKGLERROR;
		if (tmuState1.texture_coord_array.enabled)
		{
			GLESIMPL glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKGLERROR;
		}
		else
		{
			GLESIMPL glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKGLERROR;
		}
		GLESIMPL glTexCoordPointer(tmuState1.texture_coord_array.size,
				tmuState1.texture_coord_array.type,
				tmuState1.texture_coord_array.stride,
				tmuState1.texture_coord_array.ptr);
		CHECKGLERROR;
	}
	arraysValid  = GL_FALSE;

	glDrawArrays(mode,first,count);
	CHECKGLERROR;
}

void  nanoglRectd(	GLdouble  	x1,	GLdouble  	y1,	GLdouble  	x2,	GLdouble  	y2)
{
	GLfloat q3[] = {
			x1, y1,
			x2, y1,
			x2, y2,
			x1, y2
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, q3);
	glDrawArrays(GL_TRIANGLE_FAN,0,4);

	if (!tmuState0.vertex_array.enabled)
	{
		GLESIMPL glDisableClientState(GL_VERTEX_ARRAY);
		CHECKGLERROR;
	}

	if (tmuState0.color_array.enabled)
	{
		GLESIMPL glEnableClientState(GL_COLOR_ARRAY);
		CHECKGLERROR;
	}

	arraysValid = false;

}


void nanoglPixelStorei (GLenum pname, GLint param)
{
	glPixelStorei(pname,param);
	CHECKGLERROR;
}


void nanoglGenTextures (GLsizei n, GLuint *textures)
{
	glGenTextures(n,textures);
	CHECKGLERROR;
}

void nanoglFogf (GLenum pname, GLfloat param)
{
	FlushOnStateChange();

	glFogf(pname,param);
	CHECKGLERROR;
}
void nanoglFogfv(GLenum pname, const GLfloat *params)
{
	FlushOnStateChange();

	glFogfv(pname,params);
	CHECKGLERROR;
}
void nanoglFogi (GLenum pname, GLint param)
{
	FlushOnStateChange();

	glFogx(pname,param);
	CHECKGLERROR;
}

void nanoglCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	glCopyTexImage2D(target,level,internalFormat,x,y,width,height,border);
	CHECKGLERROR;
}
void nanoglCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	glCopyTexSubImage2D (target,level,xoffset,yoffset,x,y,width,height);
	CHECKGLERROR;
}

GLboolean nanoglIsEnabled (GLenum cap)
{
	switch(cap)
	{
	case GL_ALPHA_TEST:
	{
		return nanoglState.alpha_test;
	}
	case GL_BLEND:
	{
		return nanoglState.blend;
	}
	case GL_CLIP_PLANE0:
	{
		return nanoglState.clip_plane_0;
	}
	case GL_CLIP_PLANE0 + 1:
	{
		return nanoglState.clip_plane_1;
	}
	case GL_CLIP_PLANE0 + 2:
	{
		return nanoglState.clip_plane_2;
	}
	case GL_CLIP_PLANE0 + 3:
	{
		return nanoglState.clip_plane_3;
	}
	case GL_CLIP_PLANE0 + 4:
	{
		return nanoglState.clip_plane_4;
	}
	case GL_CLIP_PLANE0 + 5:
	{
		return nanoglState.clip_plane_5;
	}
	case GL_COLOR_LOGIC_OP:
	{
		return nanoglState.color_logic_op;
	}
	case GL_COLOR_MATERIAL:
	{
		return nanoglState.color_material;
	}
	case GL_CULL_FACE:
	{
		return nanoglState.cull_face;
	}
	case GL_DEPTH_TEST:
	{
		return nanoglState.depth_test;
	}
	case GL_DITHER:
	{
		return nanoglState.dither;
	}
	case GL_FOG:
	{
		return nanoglState.fog;
	}
	case GL_LIGHTING:
	{
		return nanoglState.lighting;
	}
	case GL_LINE_SMOOTH:
	{
		return nanoglState.line_smooth;
	}
	case GL_MULTISAMPLE:
	{
		return nanoglState.multisample;
	}
	case GL_NORMALIZE:
	{
		return nanoglState.normalize;
	}
	case GL_POLYGON_OFFSET_FILL:
	{
		return nanoglState.polygon_offset_fill;
	}
	case GL_RESCALE_NORMAL:
	{
		return nanoglState.rescale_normal;
	}
	case GL_SAMPLE_ALPHA_TO_COVERAGE:
	{
		return nanoglState.sample_alpha_to_coverage;
	}
	case GL_SAMPLE_ALPHA_TO_ONE:
	{
		return nanoglState.sample_alpha_to_one;
	}
	case GL_SAMPLE_COVERAGE:
	{
		return nanoglState.sample_coverage;
	}
	case GL_SCISSOR_TEST:
	{
		return nanoglState.scissor_test;
	}
	case GL_STENCIL_TEST:
	{
		return nanoglState.stencil_test;
	}
	case GL_TEXTURE_2D:
	{
		return activetmuState->texture_2d.value;
	}
	default:
		LOGI("IsEnabled 0x%0x",cap);
		FatalError("Unhandeled nanoglIsEnabled ");
		return 0;
		break;
	}
}

void nanoglFrontFace(	GLenum mode)
{
	FlushOnStateChange();
	glFrontFace(mode);
	CHECKGLERROR;
}




void nanoglBindBuffer (GLenum target, GLuint buffer)
{
	FlushOnStateChange();
	LOGI("nanoglBindBuffer %d %d",target,buffer);
	glBindBuffer(target,buffer);
}
void nanoglDeleteBuffers (GLsizei n, const GLuint *buffers)
{
	FlushOnStateChange();
	LOGI("nanoglDeleteBuffers %d %d",n,buffers);
	glDeleteBuffers(n,buffers);
}
void nanoglGenBuffers(GLsizei n, GLuint *buffers)
{
	FlushOnStateChange();
	LOGI("nanoglGenBuffers %d %d",n,buffers);
	glGenBuffers(n,buffers);
	LOGI("nanoglGenBuffers vbo = %d",n,*buffers);
}
void nanoglBufferData (GLenum target, GLsizei size, const GLvoid *data, GLenum usage)
{
	FlushOnStateChange();
	LOGI("nanoglBufferData %d %d %d %d",target,size,data,usage);
	glBufferData(target,size,data,usage);
}
void nanoglBufferSubData(GLenum target, GLint offset, GLsizei size, const GLvoid *data)
{
	FlushOnStateChange();
	LOGI("nanoglBufferSubData %d %d %d %d",target,offset,size,data);
	glBufferSubData(target,offset,size,data);
}
void *nanoglMapBuffer(GLenum target, GLenum access)
{
	FatalError("nanoglMapBuffer NOT HERE");
	//Not in OES 1?
	//return glMapBufferOES(target,access);
}
GLboolean nanoglUnmapBuffer (GLenum target)
{
	FatalError("nanoglUnmapBuffer NOT HERE");
	//return glUnmapBufferOES(target);
}


}
