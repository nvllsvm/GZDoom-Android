#include "ButtonExt.h"
#include "TouchControlsConfig.h"

#ifdef __IOS__
#include <sys/time.h>
#endif

#define REPEAT_START_TIME 500
#define REPEAT_INTERVAL   150

#define DOUBLE_TAP_SPEED 200
#define DOUBLE_TAP_DRIFT 0.05

using namespace touchcontrols;

ButtonExt::ButtonExt(std::string tag,RectF pos,std::string image_filename,int value_,bool repeat_,bool hidden_):
																						ControlSuper(TC_TYPE_BUTTONEXT,tag,pos)
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

double ButtonExt::getMS()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return  (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

int long long ButtonExt::current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

void ButtonExt::updateSize()
{
	glRect.resize(controlPos.right - controlPos.left, controlPos.bottom - controlPos.top);
}

void ButtonExt::setFlash(bool v)
{
	flash = v;
}

void ButtonExt::resetOutput()
{
    id = -1;
    signal_button.emit(BUTTONEXT_UP,value);
}

bool ButtonExt::processPointer(int action, int pid, float x, float y)
{
	if (hidden) //Hidden controls do not respond to inputs
		return false;
    LOGTOUCH("state = %d, count = %lld",doubleTapState,doubleTapCounter);
	if (action == P_DOWN)
	{
		if (controlPos.contains(x,y))
		{
			id = pid;
			signal_button.emit(BUTTONEXT_DOWN,value);
			repeatTime = getMS() +  REPEAT_START_TIME;//Wait before repeating


             if(doubleTapState == 2) //Second down of double tap
            {
                if (((current_timestamp() - doubleTapCounter) < DOUBLE_TAP_SPEED)&&
                    (((abs(doubleTapPos.x - x) + abs(doubleTapPos.y - y))) < DOUBLE_TAP_DRIFT))
                {
                    doubleTapState = 3;
                }
                else
                    doubleTapState = 0;
            }

            if (doubleTapState == 0) //First tap of double tap
            {
                            doubleTapState = 1;
                            doubleTapCounter = current_timestamp();
                            doubleTapPos.x = x;
                            doubleTapPos.y = y;
             }

			return true;
		}
		return false;
	}
	else if (action == P_UP)
	{
		if (pid == id)
		{
			id = -1;
			signal_button.emit(BUTTONEXT_UP,value);

			if (controlPos.contains(x,y))
            {
                if (doubleTapState == 1) //First up of double tap
                {
                    //Simple check to see if finger moved very much
                    if (((current_timestamp() - doubleTapCounter) < DOUBLE_TAP_SPEED) &&
                            (((abs(doubleTapPos.x - x) + abs(doubleTapPos.y - y))) < DOUBLE_TAP_DRIFT))
                    {
                        doubleTapState = 2;
                        doubleTapCounter = current_timestamp();
                    }
                    else
                        doubleTapState = 0;
                }
                else if (doubleTapState == 3) //Finger up, finished double tap
                {
                    if ((((abs(doubleTapPos.x - x) + abs(doubleTapPos.y - y))) < DOUBLE_TAP_DRIFT))
                    {
                        LOGTOUCH("emmit");
                       signal_button.emit(BUTTONEXT_DOUBLE,value);
                    }

                    doubleTapState = 0;
                    doubleTapCounter = 0;
                }
			}

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


bool ButtonExt::initGL()
{
	int x,y;
	glTex = loadTextureFromPNG(image,x,y);
	
    return false;
}

bool ButtonExt::drawGL(bool forEditor)
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


				signal_button.emit(BUTTONEXT_DOWN,value);
				signal_button.emit(BUTTONEXT_UP,value);
			}
		}
	}
    
    return false;
}

void ButtonExt::saveXML(TiXmlDocument &doc)
{
	TiXmlElement * root = new TiXmlElement(tag.c_str());
	doc.LinkEndChild( root );

	ControlSuper::saveXML(*root);
}

void ButtonExt::loadXML(TiXmlDocument &doc)
{
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem=hDoc.FirstChild( tag ).Element();

	if (!pElem) //Check exists, if not just leave as default
		return;

	ControlSuper::loadXML(*pElem);
}
