#include "GLRect.h"
#include "GLLines.h"
#include <string>
#include <vector>
#include "TouchControlsConfig.h"
extern "C" {
#include <libpng/png.h>
}


#ifndef _OpenGLUtils_H_
#define _OpenGLUtils_H_

#define TEXTURE_LOAD_ERROR -1

namespace touchcontrols
{

struct FontInfo
{
    float leftGap;
    float rightGap;
};

extern float GLScaleWidth ;
extern float GLScaleHeight ;

void drawRect(GLuint texture, float x, float y, GLRect &rect);
void drawRect(GLfloat r,GLfloat g,GLfloat b,GLfloat a, float x, float y, GLRect &rect);

void drawLines(float x, float y, GLLines &lines);

void setGraphicsBasePath(std::string path);

GLuint loadTextureFromPNG(std::string filename, int &width, int &height, std::vector< FontInfo >* = NULL );

void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length);

void setFixAspect( bool v );
}


#endif
