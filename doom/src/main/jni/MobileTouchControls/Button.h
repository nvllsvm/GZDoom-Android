#include "sigc++/sigc++.h"
#include "ControlSuper.h"
#include "GLRect.h"
#include "OpenGLUtils.h"

#ifndef _Button_H_
#define _Button_H_


namespace touchcontrols
{

class Button : public ControlSuper
{
	bool pressed;

	int id;


	GLuint glTex;
	GLuint glTexHidden;

	GLRect glRect;

	int value;

	bool repeat;
	double repeatTime;

	bool flash;
	bool flashDir;
	int long long  flashCount;

	double getMS();

	int long long current_timestamp();

public:
	std::string image;
	sigc::signal<void, int> signal_buttonDown;
	sigc::signal<void, int> signal_buttonUp;
	sigc::signal<void, int, int> signal_button;

	Button(std::string tag,RectF pos,std::string image_filename,int value,bool repeat=false,bool hidden=false);
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
