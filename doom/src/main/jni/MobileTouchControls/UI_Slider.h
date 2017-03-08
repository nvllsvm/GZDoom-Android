#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "OpenGLUtils.h"

#ifndef _UI_Slider_H_
#define _UI_Slider_H_


namespace touchcontrols
{

class UI_Slider : public ControlSuper
{
	bool pressed;

	int touchId;

	GLuint glTex;
    GLuint glTexHandle;

	GLRect glRect;
    GLRect glRectHandle;

    float value;
    uint32_t uid;

    void updateValue( float x );
public:
	std::string bg_texture;
    std::string handle_texture;

	UI_Slider( std::string tag, RectF pos, std::string bg_texture, std::string handle_texture, uint32_t uid );

    sigc::signal<void, uint32_t, float> signal_valueChange;

	bool processPointer(int action, int pid, float x, float y);

 	void resetOutput();

	bool drawGL(bool forEditor = false);

	bool initGL();

	void updateSize();

	void saveXML(TiXmlDocument &doc);

	void loadXML(TiXmlDocument &doc);
};

}

#endif
