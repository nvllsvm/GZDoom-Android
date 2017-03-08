#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "OpenGLUtils.h"

#ifndef _UI_Switch_H_
#define _UI_Switch_H_


namespace touchcontrols
{

class UI_Switch : public ControlSuper
{
    bool pressed;

    int touchId;

    GLuint glTexOn;
    GLuint glTexOff;

    GLRect glRect;

    std::string off_image;
    std::string on_image;

    bool isOn;

public:

    UI_Switch ( std::string tag, RectF pos, std::string on_image, std::string off_image );

    bool processPointer ( int action, int pid, float x, float y );

    void resetOutput();

    bool drawGL ( bool forEditor = false );

    bool initGL();

    void updateSize();

    void saveXML ( TiXmlDocument &doc );

    void loadXML ( TiXmlDocument &doc );
};

}

#endif
