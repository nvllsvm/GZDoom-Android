#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "OpenGLUtils.h"
#include "UI_TextBox.h"

#ifndef _UI_Button_H_
#define _UI_Button_H_


namespace touchcontrols
{

class UI_Button : public UI_TextBox
{
	bool pressed;

	int touchId;

	GLuint glTex;

	GLRect glRect;

    std::string bg_image;

    uint32_t uid;
public:

    sigc::signal<void, uint32_t, uint32_t> signal;


	UI_Button( std::string tag, RectF pos, uint32_t uid, std::string font_filename, int fontSet,
	            uint32_t params, std::string text, float textSize, std::string bg_image );

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
