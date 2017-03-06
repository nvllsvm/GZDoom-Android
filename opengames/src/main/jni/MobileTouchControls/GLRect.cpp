#include "TouchControlsConfig.h"

#include "GLRect.h"

using namespace touchcontrols;



GLRect::GLRect()
{
	for (int n=0;n<12;n++)
		vertices[n] = 0;

	GLfloat texture_[] = {
			// Mapping coordinates for the vertices
			0.0, -1.0,		// top left		(V2)
			0.0, 0.0,		// bottom left	(V1)
			1.0, -1.0,		// top right	(V4)
			1.0, 0.0		// bottom right	(V3)
	};

	for (int n=0;n<8;n++)
		texture[n] = texture_[n];
}

void GLRect::resize(float w, float h) {
	width = w;
	height = h;
#ifdef USE_GLES2
	float GLES2scaleX(float X);
	float GLES2scaleY(float Y);

	vertices[4] = -GLES2scaleY(height);
	vertices[6] = GLES2scaleX(width);
	vertices[7] = -GLES2scaleY(height);
	vertices[9] = GLES2scaleX(width);

#else
	vertices[1] = -1*height;
	vertices[6] = width;
	vertices[7] = -1*height;
	vertices[9] = width;
#endif
};
