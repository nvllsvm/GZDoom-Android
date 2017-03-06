#include "Button.h"
#include "TouchControlsConfig.h"

#ifdef __IOS__
#include <sys/time.h>
#endif

#define REPEAT_START_TIME 500
#define REPEAT_INTERVAL   150


using namespace touchcontrols;

Button::Button(std::string tag,RectF pos,std::string image_filename,int value_,bool repeat_,bool hidden_):
																						ControlSuper(TC_TYPE_BUTTON,tag,pos)
{
	value = value_;
	image = image_filename;
	glTex = 0;
	id = -1;
	repeat = repeat_;
	hidden = hidden_;
	updateSize();

	flash = false;
	flashDir = false;
	flashCount = 0;

}

double Button::getMS()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return  (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

int long long Button::current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

void Button::updateSize()
{
	glRect.resize(controlPos.right - controlPos.left, controlPos.bottom - controlPos.top);
}

void Button::setFlash(bool v)
{
	flash = v;
}

void Button::resetOutput()
{
    id = -1;
    signal_buttonUp.emit(value);
    signal_button.emit(0,value);
}

bool Button::processPointer(int action, int pid, float x, float y)
{
	if (hidden) //Hidden controls do not respond to inputs
		return false;

	if (action == P_DOWN)
	{
		if (controlPos.contains(x,y))
		{
			id = pid;
			signal_buttonDown.emit(value);
			signal_button.emit(1,value);
			repeatTime = getMS() +  REPEAT_START_TIME;//Wait before repeating
			return true;
		}
		return false;
	}
	else if (action == P_UP)
	{
		if (pid == id)
		{
			id = -1;
			signal_buttonUp.emit(value);
			signal_button.emit(0,value);
			return true;
		}
		return false;
	}
	else if(action == P_MOVE)
	{
		return false;
	}
    
    return false;
}


bool Button::initGL()
{
	int x,y;
	glTex = loadTextureFromPNG(image,x,y);
	
    return false;
}

bool Button::drawGL(bool forEditor)
{
	if (forEditor)
	{
        if (!hidden)
        {
		drawRect(glTex,controlPos.left,controlPos.top,glRect);
		}
	}
	else //Draw normal in game
	{
		if (!hidden)
		{

			if (flash)
			{
				//LOGTOUCH("fc = %lld",flashCount);
				//LOGTOUCH("flashDir = %d",flashDir);

				if (current_timestamp() > flashCount)
				{
					flashCount = current_timestamp() + 300;
					flashDir = !flashDir;
				}

				if (flashDir)
					return false;
			}

			if (id==-1)
				drawRect(glTex,controlPos.left,controlPos.top,glRect);
			else //Highlight buttons if pressed
			{
				//glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_SRC_ALPHA);
				drawRect(glTex,controlPos.left,controlPos.top,glRect);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
		}
	}

	if (repeat)
	{
		if (id != -1)
		{
			if (getMS() > repeatTime)
			{
				repeatTime = getMS() + REPEAT_INTERVAL;

				signal_buttonUp.emit(value);
				signal_button.emit(0,value);

				signal_buttonDown.emit(value);
				signal_button.emit(1,value);
			}
		}
	}
    
    return false;
}

void Button::saveXML(TiXmlDocument &doc)
{
	TiXmlElement * root = new TiXmlElement(tag.c_str());
	doc.LinkEndChild( root );

	ControlSuper::saveXML(*root);
}

void Button::loadXML(TiXmlDocument &doc)
{
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem=hDoc.FirstChild( tag ).Element();

	if (!pElem) //Check exists, if not just leave as default
		return;

	ControlSuper::loadXML(*pElem);
}
