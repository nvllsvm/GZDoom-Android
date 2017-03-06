#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "OpenGLUtils.h"

#ifndef _UI_Window_H_
#define _UI_Window_H_


namespace touchcontrols
{

class UI_Window : public ControlSuper
{
	bool pressed;

	int id;

	GLuint glTex;

	GLRect glRect;

public:
	std::string image;

	UI_Window(std::string tag,RectF pos,std::string image_filename);

	bool processPointer(int action, int pid, float x, float y);

 	void resetOutput();

    void setScissor( void );

	bool drawGL(bool forEditor = false);

	bool initGL();

	void updateSize();

	void saveXML(TiXmlDocument &doc);

	void loadXML(TiXmlDocument &doc);
};

}

#endif
