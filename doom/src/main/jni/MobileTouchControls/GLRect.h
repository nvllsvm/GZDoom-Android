#include "GLheader.h"

#ifndef _GLRect_H_
#define _GLRect_H_

namespace touchcontrols
{

class GLRect
{
public:
	float width,height;

	GLfloat vertices[12];

	GLfloat texture[8];

	GLRect();

	void resize(float width, float height);
};

}
#endif
