#include "gl.h"
#include "glext.h"

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"JNI", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "JNI", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,"JNI", __VA_ARGS__))



static void __gluMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

GLAPI void APIENTRY
gluOrtho2D(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)
{
    glOrthof(left, right, bottom, top, -1, 1);
}

#define __glPi 3.14159265358979323846

GLAPI void APIENTRY
gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLfloat m[4][4];
    GLfloat sine, cotangent, deltaZ;
    GLfloat radians=(GLfloat)(fovy/2.0f*__glPi/180.0f);

    deltaZ=zFar-zNear;
    sine=(GLfloat)sin(radians);
    if ((deltaZ==0.0f) || (sine==0.0f) || (aspect==0.0f))
    {
        return;
    }
    cotangent=(GLfloat)(cos(radians)/sine);

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -1.0f;
    m[3][2] = -2.0f * zNear * zFar / deltaZ;
    m[3][3] = 0;
    glMultMatrixf(&m[0][0]);
}

#define M_PI 3.14159265358979323846

void BuildPerspProjMat(float *m, float fov, float aspect, float znear, float zfar)
 {
  float ymax = znear * tan(fov * M_PI / 360.0);
  float ymin = -ymax;
  float xmax = ymax * aspect;
  float xmin = ymin * aspect;

  float width = xmax - xmin;
  float height = ymax - ymin;

  float depth = zfar - znear;
  float q = -(zfar + znear) / depth;
  float qn = -2 * (zfar * znear) / depth;

  float w = 2 * znear / width;
  w = w / aspect;
  float h = 2 * znear / height;

  m[0]  = w;
  m[1]  = 0;
  m[2]  = 0;
  m[3]  = 0;

  m[4]  = 0;
  m[5]  = h;
  m[6]  = 0;
  m[7]  = 0;

  m[8]  = 0;
  m[9]  = 0;
  m[10] = q;
  m[11] = -1;

  m[12] = 0;
  m[13] = 0;
  m[14] = qn;
  m[15] = 0;
 }


void __gluMakeIdentityf2(GLfloat m[16])
{
	m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
	m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
	m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
	m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

//void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
void gluPerspective2(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	//LOGI("gluPerspective fox = %f, aspect = %f, near = %f, far = %f",fovy,aspect,zNear,zFar);
/*
	GLfloat xform[16];
	BuildPerspProjMat(&xform[0],fovy,aspect,zNear,zFar);
	glMultMatrixf(&xform[0]);
	return;
*/
/*
	GLfloat f = 1.0 / tan(fovy * M_PI / 360.0);  // convert degrees to radians and divide by 2
	GLfloat xform[16] =
	{
			f / aspect, 0, 0, 0,
			0,          f, 0, 0,
			0,          0, (zFar + zNear)/(zFar - zNear), -1,
			0,          0, 2*zFar*zNear/(zFar - zNear), 0
	};

	LOGI("1");
	LOGI("%f  %f  %f  %f\n%f  %f  %f  %f\n%f  %f  %f  %f\n%f  %f  %f  %f\n",
			xform[0],			xform[1],xform[2],xform[3],
			xform[4],			xform[5],xform[6],xform[7],
			xform[8],			xform[9],xform[10],xform[11],
			xform[12],			xform[13],xform[14],xform[15]);

	glMultMatrixf(xform);
	return;
*/
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	GLfloat m[4][4];
	GLfloat sine, cotangent, deltaZ;
	GLfloat radians = fovy / 2 * 3.14 / 180;

	deltaZ = zFar - zNear;
	sine = sin(radians);
	if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
	{
		return;
	}
	cotangent = cos(radians) / sine;

	__gluMakeIdentityf(&m[0][0]);

	m[0][0] = cotangent / aspect;
	m[1][1] = cotangent;
	m[2][2] = -(zFar + zNear) / deltaZ;
	m[2][3] = -1;
	//m[3][2] = -2 * zNear * zFar / deltaZ;
	m[3][2] = -2 * zNear * zFar / deltaZ;
	m[3][3] = 0;

	GLfloat * xform1 = (GLfloat*)(&m[0][0]);
/*
	LOGI("2");
	LOGI("%f  %f  %f  %f\n%f  %f  %f  %f\n%f  %f  %f  %f\n%f  %f  %f  %f\n",
			xform1[0],			xform1[1],xform1[2],xform1[3],
			xform1[4],			xform1[5],xform1[6],xform1[7],
			xform1[8],			xform1[9],xform1[10],xform1[11],
			xform1[12],			xform1[13],xform1[14],xform1[15]);
*/

	glMultMatrixf(&m[0][0]);

	//glMatrixMode(GL_MODELVIEW);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//glDepthMask(GL_TRUE);
}

/*
static void __gluMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

GLAPI void APIENTRY
gluOrtho2D(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)
{
    glOrthof(left, right, bottom, top, -1, 1);
}

#define __glPi 3.14159265358979323846

GLAPI void APIENTRY
gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
    GLfloat m[4][4];
    GLfloat sine, cotangent, deltaZ;
    GLfloat radians=(GLfloat)(fovy/2.0f*__glPi/180.0f);

    deltaZ=zFar-zNear;
    sine=(GLfloat)sin(radians);
    if ((deltaZ==0.0f) || (sine==0.0f) || (aspect==0.0f))
    {
        return;
    }
    cotangent=(GLfloat)(cos(radians)/sine);

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -1.0f;
    m[3][2] = -2.0f * zNear * zFar / deltaZ;
    m[3][3] = 0;
    glMultMatrixf(&m[0][0]);
}
 */
GLAPI void APIENTRY gluPerspective1 ( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	LOGI("MY gluPerspective");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	double xmin, xmax, ymin, ymax;

	ymax = zNear * tan(fovY * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;


	glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glDepthMask(GL_TRUE);

	/*
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLfloat fW, fH;
	fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan( fovY / 360 * pi ) * zNear;
	fW = fH * aspect;

	glFrustumf( -fW, fW, -fH, fH, (GLfloat)zNear, (GLfloat)zFar );
	 */

	/*
	// Start in projection mode.
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 double xmin, xmax, ymin, ymax;
	 ymax = zNear * tan(fovy * 3.141592 / 360.0);
	 ymin = -ymax;
	 xmin = ymin * aspect;
	 xmax = ymax * aspect;
	 glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
	 */
	/*
	GLfloat range=zNear*tan(fovy*0.008841941282883074);
	aspect*=range;
	glFrustumf(-aspect,aspect,-range,range,zNear,zFar);
	 */
}

#define GLU_INVALID_ENUM  100900
#define GLU_OUT_OF_MEMORY 100902

#define FIXED(x)	((int)((x) * 65536))
#define INT(x)		((x) / 65536)
#define FRAC(x)		((x) & 0xffff)
#define RGBA(r,g,b,a)	((((a)&0xff) << 24) | \
		(((b)&0xff) << 16) | \
		(((g)&0xff) <<  8) | \
		(((r)&0xff) <<  0))

#define R(rgba)	(((rgba) & 0x000000ff) >> 0)
#define G(rgba)	(((rgba) & 0x0000ff00) >> 8)
#define B(rgba)	(((rgba) & 0x00ff0000) >>16)
#define A(rgba)	(((rgba) & 0xff000000) >>24)


struct format;
typedef unsigned pix_t;

typedef void (*unpack_t)(const struct format *fmt, const void *src, pix_t *dst, int count);
typedef void (*pack_t)(const struct format *fmt, const pix_t *src, void *dst, int count);

struct format {
	GLenum format;
	GLenum type;

	GLubyte channels;
	GLubyte size;

	unpack_t unpack;
	pack_t pack;
};
/* weight is a 1.16 fixed-point fraction */
static inline pix_t lerp(pix_t a, pix_t b, unsigned weight)
{
	unsigned omw = FIXED(1) - weight;

	return RGBA(INT(R(a) * omw + R(b) * weight),
			INT(G(a) * omw + G(b) * weight),
			INT(B(a) * omw + B(b) * weight),
			INT(A(a) * omw + A(b) * weight));
}

static void rescale_horiz_up(const pix_t *image, pix_t *ret,
		GLsizei src_width, GLsizei dst_width, GLsizei height)
{
	unsigned scale = FIXED(src_width) / dst_width;
	unsigned y;

	if (0)
		printf("\nscale %dx%d -> %dx%d, scale=%g 1/scale=%g\n",
				src_width, height, dst_width, height,
				scale / 65536., 65536. / scale);

	for(y = 0; y < height; y++, image += src_width) {
		unsigned src_x, dst_x;

		for(src_x = dst_x = 0; dst_x < dst_width; dst_x++, src_x += scale) {
			unsigned frac_src_x = FRAC(src_x);
			const pix_t *pix = &image[INT(src_x)];
			int next = 0;
			if (INT(src_x) + 1 < src_width)
				next = 1;

			*ret++ = lerp(pix[0], pix[next], frac_src_x);
		}
	}
}

static void rescale_horiz_down(const pix_t *image, pix_t *ret,
		GLsizei src_width, GLsizei dst_width, GLsizei height)
{
	unsigned scale = FIXED(src_width) / dst_width;
	unsigned y;

	for(y = 0; y < height; y++, image += src_width) {
		unsigned src_x, dst_x;

		for(src_x = dst_x = 0; dst_x < dst_width; dst_x++, src_x += scale) {
			unsigned r,g,b,a;
			unsigned int_src_x = INT(src_x);
			int i, count;

			r=g=b=a = 0;

			count = 0;
			for(i = int_src_x; i < INT(src_x + scale + 65535); i++) {
				int c = (i >= src_width) ? src_width-1 : i;

				count++;
				r += R(image[c]);
				g += G(image[c]);
				b += B(image[c]);
				a += A(image[c]);
			}

			*ret++ = RGBA(r / count,
					g / count,
					b / count,
					a / count);
		}
	}
}

static pix_t *rescale_horiz(pix_t *image,
		GLsizei src_width, GLsizei dst_width, GLsizei height)
{
	pix_t *ret;

	if (image == NULL)
		return NULL;

	ret = malloc(dst_width * height * sizeof(pix_t));

	if (ret == NULL)
		goto out;

	if (dst_width < src_width)
		rescale_horiz_down(image, ret, src_width, dst_width, height);
	else
		rescale_horiz_up(image, ret, src_width, dst_width, height);

	out:
	free(image);
	return ret;
}

static void rescale_vert_up(const pix_t *image, pix_t *ret,
		GLsizei width, GLsizei src_height, GLsizei dst_height)
{
	unsigned scale = FIXED(src_height) / dst_height;
	unsigned src_y, dst_y;

	for(dst_y = src_y = 0; dst_y < dst_height; dst_y++, src_y += scale) {
		unsigned int_src_y = INT(src_y);
		unsigned frac_src_y = FRAC(src_y);
		unsigned x;
		const pix_t *row, *next;

		row = &image[int_src_y * width];
		next = row;
		if (int_src_y+1 < src_height)
			next = row+width;

		for(x = 0; x < width; x++)
			*ret++ = lerp(*row++, *next++, frac_src_y);
	}
}

static void rescale_vert_down(const pix_t *image, pix_t *ret,
		GLsizei width, GLsizei src_height, GLsizei dst_height)
{
	unsigned scale = FIXED(src_height) / dst_height;
	unsigned r_scale = (FIXED(dst_height) / src_height) / 256;
	unsigned src_y, dst_y;

	if (0)
		printf("\nscale %dx%d -> %dx%d, scale=%g 1/scale=%g\n",
				width, src_height, width, dst_height,
				scale / 65536., 65536. / scale);

	for(dst_y = src_y = 0;
			dst_y < dst_height;
			dst_y++, src_y += scale) {
		int x;
		unsigned box_start = FIXED(1) - FRAC(src_y);
		unsigned box_middle = INT(src_y + scale) - INT(src_y + FIXED(1));
		unsigned box_end = FRAC(src_y + scale);

		if (0)
			printf("dst_y=%d src_y=%g - %g; box start=%g middle=%d end=%g sum=%g\n",
					dst_y,
					src_y / 65536., (src_y + scale) / 65536.,
					box_start / 65536.,
					box_middle,
					box_end / 65536.,
					(box_start + FIXED(box_middle) + box_end) / 65536.);

		for(x = 0; x < width; x++) {
			unsigned r,g,b,a;
			const pix_t *row;
			int i;

			r=g=b=a = 0;

			/*
			               s-----e
			   src   |----|----|----|----|----|----|
			   dst   |-----|-----|-----|-----|-----

			                   s--mm-mm--e
			   src   |--|--|--|--|--|--|--|--|--|--|
			   dst   |---------|---------|---------

			   dst = box filter over src pixels
			 */

			row = &image[(INT(src_y) * width) + x];

			if (box_start) {
				r += R(*row) * box_start;
				g += G(*row) * box_start;
				b += B(*row) * box_start;
				a += A(*row) * box_start;
				row += width;
			}

			for(i = 0; i < box_middle; i++) {
				r += FIXED(R(*row));
				g += FIXED(G(*row));
				b += FIXED(B(*row));
				a += FIXED(A(*row));
				row += width;
			}

			if (box_end) {
				r += R(*row) * box_end;
				g += G(*row) * box_end;
				b += B(*row) * box_end;
				a += A(*row) * box_end;
			}

			*ret++ = RGBA(INT((r / 256) * r_scale),
					INT((g / 256) * r_scale),
					INT((b / 256) * r_scale),
					INT((a / 256) * r_scale));

		}
	}
}

static pix_t *rescale_vert(pix_t *image,
		GLsizei width, GLsizei src_height, GLsizei dst_height)
{
	pix_t *ret;

	if (image == NULL)
		return NULL;

	ret = malloc(width * dst_height * sizeof(pix_t));

	if (ret == NULL)
		goto out;

	if (dst_height < src_height)
		rescale_vert_down(image, ret, width, src_height, dst_height);
	else
		rescale_vert_up(image, ret, width, src_height, dst_height);

	out:
	free(image);
	return ret;
}


GLint gluScaleImage(GLenum format,
		GLsizei src_width, GLsizei src_height,
		GLenum src_type, const GLvoid *src,
		GLsizei dst_width, GLsizei dst_height,
		GLenum dst_type, GLvoid *dst)
{
	LOGI("My gluScaleImage");
	memcpy(dst, src, ((dst_width * dst_height * 4) < (src_width * src_height * 4))?(dst_width * dst_height * 4): (src_width * src_height * 4));
	return;
	//const struct format *srcfmt = __pspglu_getformat(format, src_type);
	//const struct format *dstfmt = __pspglu_getformat(format, dst_type);
	pix_t *image = src;
	int i;

	//	if (srcfmt == NULL || dstfmt == NULL)
	//		return GLU_INVALID_ENUM;

	if (src_width == dst_width &&
			src_height == dst_height &&
			src_type == dst_type) {
		memcpy(dst, src, dst_width * dst_height * 4);
		LOGI("My gluScaleImage DONE 1");
		return 0;
	}

	image = malloc(src_width * src_height * sizeof(pix_t));
	if (image == NULL)
	{
		LOGI("My gluScaleImage GL_OUT_OF_MEMORY");
		return GLU_OUT_OF_MEMORY;
	}
	/*
	for(i = 0; i < src_height; i++)
		(*srcfmt->unpack)(srcfmt,
				  src + i * src_width * srcfmt->size,
				  &image[i * src_width],
				  src_width);
	 */

	if (src_width != dst_width)
		image = rescale_horiz(image, src_width, dst_width, src_height);

	if (src_height != dst_height)
		image = rescale_vert(image, dst_width, src_height, dst_height);

	if (image == NULL)
	{
		LOGI("My gluScaleImage GL_OUT_OF_MEMORY");
		return GL_OUT_OF_MEMORY;
	}
	/*
	for(i = 0; i < dst_height; i++)
		(*dstfmt->pack)(dstfmt,
				&image[i * dst_width],
				dst + i * dst_width * dstfmt->size,
				dst_width);
	 */
	memcpy(dst, image, dst_width * dst_height * 4);
	free(image);
	LOGI("My gluScaleImage DONE");
	return 0;
}

GLvoid* APIENTRY glMapBufferRange (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{


}

GLAPI void APIENTRY glFlushMappedBufferRange (GLenum target, GLintptr offset, GLsizeiptr length){}



GLAPI void APIENTRY glBlendEquation (GLenum mode)
{

}
GLAPI void APIENTRY glGetObjectParameteriv(GLhandleARB obj, GLenum pname, GLint *params)
{

}
GLAPI void APIENTRY glBlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha){}
GLAPI void APIENTRY glDrawBuffers (GLsizei n, const GLenum *bufs){}
GLAPI void APIENTRY glStencilOpSeparate (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass){}
GLAPI void APIENTRY glStencilFuncSeparate (GLenum face, GLenum func, GLint ref, GLuint mask){}
GLAPI void APIENTRY glStencilMaskSeparate (GLenum face, GLuint mask){}
GLAPI void APIENTRY glAttachShader (GLuint program, GLuint shader){}
GLAPI void APIENTRY glBindAttribLocation (GLuint program, GLuint index, const GLchar *name){}
GLAPI void APIENTRY glCompileShader (GLuint shader){}
GLAPI GLuint APIENTRY glCreateProgram (void){}
GLAPI GLuint APIENTRY glCreateShader (GLenum type){}
GLAPI void APIENTRY glDeleteProgram (GLuint program){}
GLAPI void APIENTRY glDeleteShader (GLuint shader){}
GLAPI void APIENTRY glDetachShader (GLuint program, GLuint shader){}
GLAPI void APIENTRY glDisableVertexAttribArray (GLuint index){}
GLAPI void APIENTRY glEnableVertexAttribArray (GLuint index){}
GLAPI void APIENTRY glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name){}
GLAPI void APIENTRY glGetActiveUniform (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name){}
GLAPI void APIENTRY glGetAttachedShaders (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj){}
GLAPI GLint APIENTRY glGetAttribLocation (GLuint program, const GLchar *name){}
GLAPI void APIENTRY glGetProgramiv (GLuint program, GLenum pname, GLint *params){}
GLAPI void APIENTRY glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog){}
GLAPI void APIENTRY glGetShaderiv (GLuint shader, GLenum pname, GLint *params){}
GLAPI void APIENTRY glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog){}
GLAPI void APIENTRY glGetShaderSource (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source){}
GLAPI GLint APIENTRY glGetUniformLocation (GLuint program, const GLchar *name){}
GLAPI void APIENTRY glGetUniformfv (GLuint program, GLint location, GLfloat *params){}
GLAPI void APIENTRY glGetUniformiv (GLuint program, GLint location, GLint *params){}
GLAPI void APIENTRY glGetVertexAttribdv (GLuint index, GLenum pname, GLdouble *params){}
GLAPI void APIENTRY glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat *params){}
GLAPI void APIENTRY glGetVertexAttribiv (GLuint index, GLenum pname, GLint *params){}
GLAPI void APIENTRY glGetVertexAttribPointerv (GLuint index, GLenum pname, GLvoid* *pointer){}
GLAPI GLboolean APIENTRY glIsProgram (GLuint program){}
GLAPI GLboolean APIENTRY glIsShader (GLuint shader){}
GLAPI void APIENTRY glLinkProgram (GLuint program){}
GLAPI void APIENTRY glShaderSource (GLuint shader, GLsizei count, const GLchar* *string, const GLint *length){}
GLAPI void APIENTRY glUseProgram (GLuint program){}
GLAPI void APIENTRY glUniform1f (GLint location, GLfloat v0){}
GLAPI void APIENTRY glUniform2f (GLint location, GLfloat v0, GLfloat v1){}
GLAPI void APIENTRY glUniform3f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2){}
GLAPI void APIENTRY glUniform4f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){}
GLAPI void APIENTRY glUniform1i (GLint location, GLint v0){}
GLAPI void APIENTRY glUniform2i (GLint location, GLint v0, GLint v1){}
GLAPI void APIENTRY glUniform3i (GLint location, GLint v0, GLint v1, GLint v2){}
GLAPI void APIENTRY glUniform4i (GLint location, GLint v0, GLint v1, GLint v2, GLint v3){}
GLAPI void APIENTRY glUniform1fv (GLint location, GLsizei count, const GLfloat *value){}
GLAPI void APIENTRY glUniform2fv (GLint location, GLsizei count, const GLfloat *value){}
GLAPI void APIENTRY glUniform3fv (GLint location, GLsizei count, const GLfloat *value){}
GLAPI void APIENTRY glUniform4fv (GLint location, GLsizei count, const GLfloat *value){}
GLAPI void APIENTRY glUniform1iv (GLint location, GLsizei count, const GLint *value){}
GLAPI void APIENTRY glUniform2iv (GLint location, GLsizei count, const GLint *value){}
GLAPI void APIENTRY glUniform3iv (GLint location, GLsizei count, const GLint *value){}
GLAPI void APIENTRY glUniform4iv (GLint location, GLsizei count, const GLint *value){}
GLAPI void APIENTRY glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){}
GLAPI void APIENTRY glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){}
GLAPI void APIENTRY glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){}
GLAPI void APIENTRY glValidateProgram (GLuint program){}
GLAPI void APIENTRY glVertexAttrib1d (GLuint index, GLdouble x){}
GLAPI void APIENTRY glVertexAttrib1dv (GLuint index, const GLdouble *v){}
GLAPI void APIENTRY glVertexAttrib1f (GLuint index, GLfloat x){}
GLAPI void APIENTRY glVertexAttrib1fv (GLuint index, const GLfloat *v){}
GLAPI void APIENTRY glVertexAttrib1s (GLuint index, GLshort x){}
GLAPI void APIENTRY glVertexAttrib1sv (GLuint index, const GLshort *v){}
GLAPI void APIENTRY glVertexAttrib2d (GLuint index, GLdouble x, GLdouble y){}
GLAPI void APIENTRY glVertexAttrib2dv (GLuint index, const GLdouble *v){}
GLAPI void APIENTRY glVertexAttrib2f (GLuint index, GLfloat x, GLfloat y){}
GLAPI void APIENTRY glVertexAttrib2fv (GLuint index, const GLfloat *v){}
GLAPI void APIENTRY glVertexAttrib2s (GLuint index, GLshort x, GLshort y){}
GLAPI void APIENTRY glVertexAttrib2sv (GLuint index, const GLshort *v){}
GLAPI void APIENTRY glVertexAttrib3d (GLuint index, GLdouble x, GLdouble y, GLdouble z){}
GLAPI void APIENTRY glVertexAttrib3dv (GLuint index, const GLdouble *v){}
GLAPI void APIENTRY glVertexAttrib3f (GLuint index, GLfloat x, GLfloat y, GLfloat z){}
GLAPI void APIENTRY glVertexAttrib3fv (GLuint index, const GLfloat *v){}
GLAPI void APIENTRY glVertexAttrib3s (GLuint index, GLshort x, GLshort y, GLshort z){}
GLAPI void APIENTRY glVertexAttrib3sv (GLuint index, const GLshort *v){}
GLAPI void APIENTRY glVertexAttrib4Nbv (GLuint index, const GLbyte *v){}
GLAPI void APIENTRY glVertexAttrib4Niv (GLuint index, const GLint *v){}
GLAPI void APIENTRY glVertexAttrib4Nsv (GLuint index, const GLshort *v){}
GLAPI void APIENTRY glVertexAttrib4Nub (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w){}
GLAPI void APIENTRY glVertexAttrib4Nubv (GLuint index, const GLubyte *v){}
GLAPI void APIENTRY glVertexAttrib4Nuiv (GLuint index, const GLuint *v){}
GLAPI void APIENTRY glVertexAttrib4Nusv (GLuint index, const GLushort *v){}
GLAPI void APIENTRY glVertexAttrib4bv (GLuint index, const GLbyte *v){}
GLAPI void APIENTRY glVertexAttrib4d (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w){}
GLAPI void APIENTRY glVertexAttrib4dv (GLuint index, const GLdouble *v){}
GLAPI void APIENTRY glVertexAttrib4f (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w){}
GLAPI void APIENTRY glVertexAttrib4fv (GLuint index, const GLfloat *v){}
GLAPI void APIENTRY glVertexAttrib4iv (GLuint index, const GLint *v){}
GLAPI void APIENTRY glVertexAttrib4s (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w){}
GLAPI void APIENTRY glVertexAttrib4sv (GLuint index, const GLshort *v){}
GLAPI void APIENTRY glVertexAttrib4ubv (GLuint index, const GLubyte *v){}
GLAPI void APIENTRY glVertexAttrib4uiv (GLuint index, const GLuint *v){}
GLAPI void APIENTRY glVertexAttrib4usv (GLuint index, const GLushort *v){}
GLAPI void APIENTRY glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer){}


GLAPI void APIENTRY glGenQueries (GLsizei n, GLuint *ids){}
GLAPI void APIENTRY glDeleteQueries (GLsizei n, const GLuint *ids){}
GLAPI GLboolean APIENTRY glIsQuery (GLuint id){}
GLAPI void APIENTRY glBeginQuery (GLenum target, GLuint id){}
GLAPI void APIENTRY glEndQuery (GLenum target){}
GLAPI void APIENTRY glGetQueryiv (GLenum target, GLenum pname, GLint *params){}
GLAPI void APIENTRY glGetQueryObjectiv (GLuint id, GLenum pname, GLint *params){}
GLAPI void APIENTRY glGetQueryObjectuiv (GLuint id, GLenum pname, GLuint *params){}
GLAPI void APIENTRY glBindBuffer (GLenum target, GLuint buffer){}
GLAPI void APIENTRY glDeleteBuffers (GLsizei n, const GLuint *buffers){}
GLAPI void APIENTRY glGenBuffers (GLsizei n, GLuint *buffers){}
GLAPI GLboolean APIENTRY glIsBuffer (GLuint buffer){}
GLAPI void APIENTRY glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage){}
GLAPI void APIENTRY glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data){}
GLAPI void APIENTRY glGetBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data){}
GLAPI GLvoid* APIENTRY glMapBuffer (GLenum target, GLenum access){}
GLAPI GLboolean APIENTRY glUnmapBuffer (GLenum target){}
GLAPI void APIENTRY glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params){}
GLAPI void APIENTRY glGetBufferPointerv (GLenum target, GLenum pname, GLvoid* *params){}


GLAPI GLboolean APIENTRY glIsRenderbuffer (GLuint renderbuffer){}
GLAPI void APIENTRY glBindRenderbuffer (GLenum target, GLuint renderbuffer){}
GLAPI void APIENTRY glDeleteRenderbuffers (GLsizei n, const GLuint *renderbuffers){}
GLAPI void APIENTRY glGenRenderbuffers (GLsizei n, GLuint *renderbuffers){}
GLAPI void APIENTRY glRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height){}
GLAPI void APIENTRY glGetRenderbufferParameteriv (GLenum target, GLenum pname, GLint *params){}
GLAPI GLboolean APIENTRY glIsFramebuffer (GLuint framebuffer){}
GLAPI void APIENTRY glBindFramebuffer (GLenum target, GLuint framebuffer){}
GLAPI void APIENTRY glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers){}
GLAPI void APIENTRY glGenFramebuffers (GLsizei n, GLuint *framebuffers){}
GLAPI GLenum APIENTRY glCheckFramebufferStatus (GLenum target){}
GLAPI void APIENTRY glFramebufferTexture1D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){}
GLAPI void APIENTRY glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level){}
GLAPI void APIENTRY glFramebufferTexture3D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset){}
GLAPI void APIENTRY glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer){}
GLAPI void APIENTRY glGetFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint *params){}
GLAPI void APIENTRY glGenerateMipmap (GLenum target){}
GLAPI void APIENTRY glBlitFramebuffer (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter){}
GLAPI void APIENTRY glRenderbufferStorageMultisample (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height){}
GLAPI void APIENTRY glFramebufferTextureLayer (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer){}

