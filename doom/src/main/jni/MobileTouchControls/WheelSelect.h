#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "PointF.h"
#include "OpenGLUtils.h"

#ifndef _WheelSelect_H_
#define _WheelSelect_H_


namespace touchcontrols
{

class WheelSelect : public ControlSuper
{
    bool pressed;
    bool hideGraphics;

    int id;

    std::string image;

    GLuint glTex;
    GLuint glTexFade;

    GLRect glRect;
    GLRect glRectFade;


    PointF last;
    PointF fingerPos;
    PointF anchor;

    PointF centre;

    int nbrSegs;

    int selectedSeg;

    bool useFadeSegs; //default no
    int enabledSegs;
public:

    sigc::signal<void, int> signal_selected;
    sigc::signal<void, int> signal_enabled;
    sigc::signal<void, int> signal_scroll;

    WheelSelect ( std::string tag, RectF pos, std::string image_filename, int segments );

    void setSegmentEnabled ( int seg, bool v );

    void setHideGraphics ( bool v );

    void resetOutput();

    bool processPointer ( int action, int pid, float x, float y );

    bool drawGL ( bool forEditor = false );

    bool initGL();

    void updateSize();

    void saveXML ( TiXmlDocument &doc );

    void loadXML ( TiXmlDocument &doc );
private:

    void reset();
    void calcNewValue();
    void doUpdate();
    float distCentre ( float x, float y );
    bool inCentre ( float x, float y );
};

}

#endif
