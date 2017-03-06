#include "GLheader.h"

#ifndef _GLLines_H_
#define _GLLines_H_

namespace touchcontrols
{

class GLLines
{
public:
	GLfloat *vertices;

	GLuint len;

	GLLines(int nbr);

};

}
#endif
