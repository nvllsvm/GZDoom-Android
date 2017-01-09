void nanoPushState();

void nanoPopState();

void nanoglBlendEquation(GLenum mode);

void nanoglBegin(GLenum mode);
void nanoglEnd(void);
void nanoglEnable (GLenum cap);
void nanoglDisable (GLenum cap);
void nanoglVertex2f(GLfloat x, GLfloat y);
void nanoglVertex2d(GLdouble x, GLdouble y);
void nanoglVertex2i(GLint x, GLint y);
void nanoglColor3f(	GLfloat red, GLfloat green, GLfloat blue);
void nanoglTexCoord2f(GLfloat s, GLfloat t);
void nanoglTexCoord2d(GLdouble s, GLdouble t);
void nanoglViewport (GLint x, GLint y, GLsizei width, GLsizei height);
void nanoglLoadIdentity (void);
void nanoglColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void nanoglColor4b (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
void nanoglColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
void nanoglColor3ub (GLubyte red, GLubyte green, GLubyte blue);
void nanoglOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void nanoglMatrixMode (GLenum mode);
void nanoglTexParameterf (GLenum target, GLenum pname, GLfloat param);
void nanoglTexParameteri (GLenum target, GLenum pname, GLint param);
void nanoglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void nanoglDrawBuffer(GLenum mode);
void nanoglTranslatef (GLfloat x, GLfloat y, GLfloat z);
void nanoglRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void nanoglScalef (GLfloat x, GLfloat y, GLfloat z);
//void nanoglDepthRange(GLclampf zNear, GLclampf zFar);
void nanoglDepthRange(GLclampd zNear, GLclampd zFar);
void nanoglDepthFunc (GLenum func);
void nanoglFinish (void);
void nanoglGetFloatv (GLenum pname, GLfloat *params);
void nanoglGetDoublev (GLenum pname, GLdouble *params);
void nanoglGetIntegerv (GLenum pname, GLint *params);
void nanoglCullFace (GLenum mode);
//void nanoglFrustum (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
void nanoglFrustum ( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void nanoglClear (GLbitfield mask);
void nanoglVertex3f( GLfloat x, GLfloat y, GLfloat z );
void nanoglVertex3d( GLdouble x, GLdouble y, GLdouble z );
void nanoglColor4fv( const GLfloat *v );
void nanoglHint (GLenum target, GLenum mode);
void nanoglBlendFunc (GLenum sfactor, GLenum dfactor);
void nanoglPopMatrix (void);
void nanoglShadeModel (GLenum mode);
void nanoglPushMatrix (void);
void nanoglTexEnvf (GLenum target, GLenum pname, GLfloat param);
void nanoglTexEnvi (GLenum target, GLenum pname, GLint param);
void nanoglVertex3fv( const GLfloat *v );
void nanoglDepthMask (GLboolean flag);
void nanoglBindTexture (GLenum target, GLuint texture);
const GLubyte* nanoglGetString (GLenum name);
void nanoglAlphaFunc (GLenum func, GLclampf ref);
void nanoglFlush (void);
void nanoglReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void nanoglReadBuffer( GLenum mode );
void nanoglLoadMatrixf (const GLfloat *m);
void nanoglTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);

void nanoglCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void nanoglCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

void nanoglClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GLenum nanoglGetError (void);
void nanoglActiveTexture (GLenum texture);
void nanoglClientActiveTexture (GLenum texture);
void nanoglColor3ubv( const GLubyte* v);

void nanoglPolygonMode( GLenum face, GLenum mode );

GLboolean nanoglIsEnabled (GLenum cap);
void nanoglArrayElement(GLint i);
void nanoglLineWidth(GLfloat width);
void nanoglCallList( GLuint list );
void nanoglTexCoord2fv( const GLfloat *v );
void nanoglColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
void nanoglStencilFunc( GLenum func, GLint ref, GLuint mask );
void nanoglStencilOp( GLenum fail, GLenum zfail, GLenum zpass );
void nanoglColor4ubv( const GLubyte *v );
void nanoglDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid *indices );
void nanoglEnableClientState( GLenum array );
void nanoglDisableClientState(GLenum  array);
void nanoglVertexPointer(	GLint size,	GLenum type,GLsizei	stride,	const GLvoid *pointer );
void nanoglTexCoordPointer( GLint	size,  GLenum type,  GLsizei stride,  const	GLvoid *pointer	);
void nanoglColorPointer( GLint size,  GLenum type,  GLsizei stride,  const GLvoid *pointer );
void nanoglPolygonOffset(	GLfloat	factor,	GLfloat	units );
//void nanoglClearDepth( GLclampf depth );
void nanoglClearDepth( GLclampd depth );
void nanoglDeleteTextures( GLsizei n,	 const GLuint *textures	);
void nanoglTexParameterfv(	GLenum  	target, GLenum  	pname, 	const GLfloat *  	params);
void nanoglStencilMask( GLuint mask );
void nanoglClearStencil( GLint s );
void nanoglScissor( GLint	x,  GLint	y,  GLsizei width,  GLsizei height );
void nanoglClipPlane( GLenum plane, const GLdouble *equation );
void nanoglColor3fv( const GLfloat *v );
void nanoglPointSize( GLfloat size );

void nanoglMultiTexCoord2fARB( GLenum target, GLfloat s, GLfloat t );
void nanoglMultiTexCoord2dARB( GLenum target, GLdouble s, GLdouble t );
void nanoglMultiTexCoord2fv ( GLenum target, const GLfloat* v);


void nanoglMultMatrixd (const GLdouble *m);
void nanoglMultMatrixf (const GLfloat *m);

void nanoglGetIntegerv(GLenum pname, GLint *params);
void nanoglDrawArrays (GLenum mode, GLint first, GLsizei count);
void nanoglPixelStorei (GLenum pname, GLint param);

void nanoglFogf (GLenum pname, GLfloat param);
void nanoglFogfv(GLenum pname, const GLfloat *params);
void nanoglFogi (GLenum pname, GLint param);

void nanoglGenTextures (GLsizei n, GLuint *textures);

void nanoglRectd(	GLdouble  	x1,	GLdouble  	y1,	GLdouble  	x2,	GLdouble  	y2);

void nanoglFrontFace(	GLenum mode);

void nanoglBindBuffer (GLenum target, GLuint buffer);
void nanoglDeleteBuffers (GLsizei n, const GLuint *buffers);
void nanoglGenBuffers(GLsizei n, GLuint *buffers);
void nanoglBufferData (GLenum target, GLsizei size, const GLvoid *data, GLenum usage);
void nanoglBufferSubData(GLenum target, GLint offset, GLsizei size, const GLvoid *data);
void *nanoglMapBuffer(GLenum target, GLenum access);
GLboolean nanoglUnmapBuffer (GLenum target);
//VBO stuff





