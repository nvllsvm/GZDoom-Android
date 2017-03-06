#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "PointF.h"
#include "OpenGLUtils.h"

#ifndef _JoyStick_H_
#define _JoyStick_H_


namespace touchcontrols
{

class JoyStick : public ControlSuper
{
	bool pressed;
	bool hideGraphics;
	bool centreAnchor;

	int id;

	std::string image;
	std::string imageBg;

	GLuint glTex;
	GLuint glTexBg;

	GLRect glRect;
	GLRect glRectBg;

	PointF valueTouch;
	PointF valueJoy;

	PointF last;
	PointF fingerPos;
	PointF anchor;
	int glitchFix;


	//Double tap stuff
	int doubleTapState; //0 = waiting for first press, 1 = waiting for first lift,
	double doubleTapCounter;
public:
	sigc::signal<void, float,float,float,float> signal_move;

	sigc::signal<void, int> signal_double_tap;


	JoyStick(std::string tag,RectF pos,std::string image_filename);

	void setHideGraphics(bool v);
	void setCentreAnchor(bool v);

	void setBackground(std::string image_filename);

    void resetOutput();

	bool processPointer(int action, int pid, float x, float y);

	bool drawGL(bool forEditor = false);

	bool initGL();

	void updateSize();

	void saveXML(TiXmlDocument &doc);

	void loadXML(TiXmlDocument &doc);
private:

	void reset();
	void calcNewValue();
	void doUpdate();
	double getMS();
};

}

#endif
