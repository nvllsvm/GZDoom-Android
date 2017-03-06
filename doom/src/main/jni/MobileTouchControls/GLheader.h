//
//  GLheader.h
//  touchcontrols
//
//  Created by Emile on 05/12/2015.
//  Copyright © 2015 Beloko games. All rights reserved.
//

#ifndef GLheader_h
#define GLheader_h


#ifdef __ANDROID__

#ifdef USE_GLES2
#include <GLES2/gl2.h>
#else
#include <GLES/gl.h>
#endif

#endif



#ifdef __IOS__


#ifdef USE_GLES2
#include "OpenGLES/ES2/gl.h"

//These are emulated in OpenGLUtils
void glColor4f(GLfloat r,GLfloat g,GLfloat b,GLfloat a);

void glLoadIdentity();

void glScalef(GLfloat x,GLfloat y,GLfloat z);

void glTranslatef(GLfloat x,GLfloat y,GLfloat z);

#else
#include "OpenGLES/ES1/gl.h"
#endif

#endif

#endif /* GLheader_h */
