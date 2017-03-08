#include <string>

#include "RectF.h"
#define TIXML_USE_STL
#include "TinyXML/tinyxml.h"

#ifndef _ControlSuper_H_
#define _ControlSuper_H_

#define TC_TYPE_BUTTON          1
#define TC_TYPE_TOUCHJOY        2
#define TC_TYPE_MOUSE           3
#define TC_TYPE_WHEELSEL        4
#define TC_TYPE_MULTITOUCHMOUSE 5
#define TC_TYPE_JOYSTICK        6
#define TC_TYPE_BUTTONEXT       7

#define TC_TYPE_UI_WINDOW       20
#define TC_TYPE_UI_SLIDER       21
#define TC_TYPE_UI_TEXTBOX      22
#define TC_TYPE_UI_BUTTON       23
#define TC_TYPE_UI_SWITCH       24


namespace touchcontrols
{
class ControlSuper
{
protected:
	bool enabled;

	bool hidden; //Can hide control from view, but still enabled

public:
	RectF controlPos;

	void setEnabled(bool v);
	void setHidden(bool v);

	bool isEnabled();
	bool isHidden();

	std::string description;

	std::string tag;

	int type;

	ControlSuper(int type,std::string t,RectF pos);

	virtual void resetOutput() = 0; //This outputs a reset

	virtual bool processPointer(int action, int pid, float x, float y) = 0;

	virtual bool drawGL(bool forEditor = false) = 0;

	virtual bool initGL() = 0;

	virtual void updateSize() = 0;

	virtual void saveXML(TiXmlDocument &doc) = 0;

	virtual void loadXML(TiXmlDocument &doc) = 0;

	void saveXML(TiXmlElement &root);

	void loadXML(TiXmlElement &root);

};

}

#endif
