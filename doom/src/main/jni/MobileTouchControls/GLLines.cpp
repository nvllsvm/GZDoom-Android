#include "TouchControlsConfig.h"

#include "GLLines.h"
#include <string.h>

using namespace touchcontrols;



GLLines::GLLines(int nbr)
{
	len = nbr*2;
	vertices = new GLfloat[len * 3];
    for (int n=0; n < len * 3; n++)
        vertices[n] = 0;
    
	//int * p1 = new int[5];
}


