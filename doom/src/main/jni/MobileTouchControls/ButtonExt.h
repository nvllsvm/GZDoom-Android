#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "OpenGLUtils.h"
#include "PointF.h"

#ifndef _ButtonExt_H_
#define _ButtonExt_H_

#define BUTTONEXT_MOVE 0
#define BUTTONEXT_TAP  1
#define BUTTONEXT_DOWN 2
#define BUTTONEXT_UP   3
#define BUTTONEXT_DOUBLE   4

namespace touchcontrols
{

class ButtonExt : public ControlSuper
{
	bool pressed;

	int id;

	GLuint glTex;

	GLRect glRect;

	int value;

	bool repeat;
	double repeatTime;

	bool flash;
	bool flashDir;
	int long long  flashCount;

	double getMS();

	int long long current_timestamp();

	int doubleTapState; //0 = waiting for first press, 1 = waiting for first lift,
	int long long doubleTapCounter;
	PointF doubleTapPos;

public:
	std::string image;
	sigc::signal<void, int, int> signal_button;

	ButtonExt(std::string tag,RectF pos,std::string image_filename,int value,bool repeat=false,bool hidden=false);
	//Button(std::string tag,RectF pos,std::string image_filename,int value,bool repeat,bool hidden);
	//Button(std::string tag,RectF pos,std::string image_filename,int value,bool repeat,bool hidden);

	void setRepeat(bool v);

	void setFlash(bool v);

	void resetOutput();

	bool processPointer(int action, int pid, float x, float y);

	bool drawGL(bool forEditor = false);

	bool initGL();

	void updateSize();

	void saveXML(TiXmlDocument &doc);

	void loadXML(TiXmlDocument &doc);
};

}

#endif
