#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "PointF.h"
#include "OpenGLUtils.h"

#ifndef _MultitouchMouse_H_
#define _MultitouchMouse_H_

#define MULTITOUCHMOUSE_MOVE 0
#define MULTITOUCHMOUSE_TAP  1
#define MULTITOUCHMOUSE_DOWN 2
#define MULTITOUCHMOUSE_UP   3

#define MULTITOUCHMOUSE_2_DOWN 4
#define MULTITOUCHMOUSE_2_UP   5

#define MULTITOUCHMOUSE_ZOOM   6
#define MULTITOUCHMOUSE_LONG_PRESS   7

namespace touchcontrols
{

class MultitouchMouse : public ControlSuper
{
	bool pressed;
	bool hideGraphics;

	int id;

	int id2;

	std::string image;

	GLuint glTex;

	GLRect glRect;
	GLLines *glLines;


	PointF last,last2;

	PointF anchor;


	//Double tap stuff
	int tapState; //0 = waiting for first press, 1 = waiting for first lift,
	int tapCounter;

	//enum mode{
public:
	sigc::signal<void,int, float,float,float,float> signal_action;

	sigc::signal<void, int> signal_double_tap;


	MultitouchMouse(std::string tag,RectF pos,std::string image_filename);

	void setHideGraphics(bool v);

	void resetOutput();

	bool processPointer(int action, int pid, float x, float y);

	bool drawGL(bool);

	bool initGL();

	void updateSize();

	void saveXML(TiXmlDocument &doc);

	void loadXML(TiXmlDocument &doc);
private:

	void reset();
	float distancePoints(PointF p1,PointF p2);

};

}

#endif
