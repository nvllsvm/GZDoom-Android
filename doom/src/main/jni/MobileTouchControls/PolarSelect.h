#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "PointF.h"
#include "OpenGLUtils.h"

#ifndef _PolarSelect_H_
#define _PolarSelect_H_

#define POLARSELECT_DOWN 0
#define POLARSELECT_UP   1
#define POLARSELECT_MOVE 2

namespace touchcontrols
{

class PolarSelect : public ControlSuper
{
	bool pressed;
	bool hideGraphics;

	int id;

	std::string image;
	GLuint glTex;
	GLRect glRect;

	PointF fingerPos;

	PointF centre;


public:

	sigc::signal<void, int, float,float> signal_action;

	PolarSelect(std::string tag,RectF pos,std::string image_filename);


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
	float distCentre(float x, float y);
	float getAngle(float x, float y);
};

}

#endif
