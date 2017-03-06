#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "OpenGLUtils.h"

#ifndef _UI_TextBox_H_
#define _UI_TextBox_H_

#define UI_TEXT_LEFT  0x1
#define UI_TEXT_CENTRE 0x2
#define UI_TEXT_RIGHT 0x4

namespace touchcontrols
{

class UI_TextBox : public ControlSuper
{
	bool pressed;

	int id;

	GLuint glTex;

	GLRect glRect;

    std::string image;
    std::string text;
    float textSize;
    float charSpacing;
    int fontSet; //0 or 1
    uint32_t params;

    std::vector< FontInfo > fontInfoVec;

    float getCharWidth(unsigned char c);

    float getTotalWidth();

public:

	UI_TextBox( std::string tag, RectF pos, std::string font_filename, int fontSet, uint32_t params, std::string text, float textSize );

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
