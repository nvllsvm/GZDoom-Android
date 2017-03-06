#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "PointF.h"
#include "OpenGLUtils.h"

#ifndef _TouchJoy_H_
#define _TouchJoy_H_


namespace touchcontrols
{

class TouchJoy : public ControlSuper
{
	bool pressed;
	bool hideGraphics;

	int id;

	std::string image;

	GLuint glTex;

	GLRect glRect;

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


	TouchJoy(std::string tag,RectF pos,std::string image_filename);

	void setHideGraphics(bool v);

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
