#include "TouchJoy.h"
#include "TouchControlsConfig.h"
#include <sys/time.h>

#define DOUBLE_TAP_SPEED 200

using namespace touchcontrols;

TouchJoy::TouchJoy(std::string tag,RectF pos,std::string image_filename):
		ControlSuper(TC_TYPE_TOUCHJOY,tag,pos)
{
	image = image_filename;
	id = -1;
	doubleTapState = 0;
	hideGraphics = false;
	updateSize();
	glitchFix = 0;
};


void TouchJoy::setHideGraphics(bool v)
{
	hideGraphics = v;
	//LOGTOUCH("setHideGraphics enabled = %d",enabled);
}

void TouchJoy::resetOutput(){
    reset();
}

void TouchJoy::updateSize()
{
	//glRect.resize(controlPos.right - controlPos.left, controlPos.bottom - controlPos.top);
	glRect.resize(0.1, 0.16);
}

double TouchJoy::getMS()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return  (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}

bool TouchJoy::processPointer(int action, int pid, float x, float y)
{
	if (action == P_DOWN)
	{
		//if (id == -1) //Only process if not active
		if (1) //Testing this, try to fix random pointer swap 08/12/13
		{
			if (controlPos.contains(x, y))
			{
				id = pid;

				glitchFix = 1; //Fix jumpy initial move

				last.x = x;
				last.y = y;
				anchor.x = x;
				anchor.y = y;
				fingerPos.x = x;
				fingerPos.y = y;

				if (doubleTapState == 0) //First tap of double tap
				{
					doubleTapState = 1;
                    doubleTapCounter = getMS();
				}
				else if(doubleTapState == 2) //Second down of double tap
				{
                    if ((getMS() - doubleTapCounter) < DOUBLE_TAP_SPEED)
					{
						signal_double_tap.emit(1);
						doubleTapState = 3;
					}
					else
						doubleTapState = 0;
				}
				return true;
			}
		}
		return false;
	}
	else if (action == P_UP)
	{
		if (id == pid)
		{
			if (doubleTapState == 1) //First up of double tap
			{
				//Simple check to see if finger moved very much
                if (((getMS() - doubleTapCounter) < DOUBLE_TAP_SPEED) &&
						(((abs(anchor.x - fingerPos.x) + abs(anchor.y - fingerPos.y))) < 0.05))
				{
					doubleTapState = 2;
                    doubleTapCounter = getMS();
				}
				else
					doubleTapState = 0;
			}
			else if (doubleTapState == 3) //Finger up, finished double tap
			{
				signal_double_tap.emit(0);
				doubleTapState = 0;
				doubleTapCounter = 0;
			}


			reset();
			return true;
		}
		return false;
	}
	else if(action == P_MOVE)
	{
		if (pid == id) //Finger already down
		{

			if (glitchFix) //Need to wait untill the values have changed at least once, otherwise inital jump
			{
				if ((last.x != x) || (last.y != y))
				{
					last.x = x;
					last.y = y;
					anchor.x = x;
					anchor.y = y;
					fingerPos.x = x;
					fingerPos.y = y;
					glitchFix--;
				}
			}


			if (!glitchFix)
			{
				fingerPos.x = x;
				fingerPos.y = y;
				calcNewValue();
			}
			return true;
		}
		return false;
	}
    
    return false;
}

bool TouchJoy::initGL()
{
	int x,y;
	glTex = loadTextureFromPNG(image,x,y);
    
    return false;
}

bool TouchJoy::drawGL(bool forEditor)
{

	if (!enabled)
		return false;
	//drawLines(0,0,*glLines);

	//drawRect(glTex,controlPos.left,controlPos.top,glRect);
    if (!hideGraphics)
	{
		if (id != -1)
			drawRect(glTex,fingerPos.x-glRect.width/2,fingerPos.y-glRect.height/2,glRect);
		else
			drawRect(glTex,controlPos.left+controlPos.width()/2-glRect.width/2,controlPos.top+controlPos.height()/2-glRect.height/2,glRect);

	}

	//LOGTOUCH("state = %d, counter = %d",doubleTapState,doubleTapCounter);
    
    return false;
}

void TouchJoy::reset()
{
	id = -1;
	valueTouch.x = 0;
	valueTouch.y = 0;
	valueJoy.x = 0;
	valueJoy.y = 0;

	doUpdate();

}

void TouchJoy::calcNewValue()
{
	float dx = last.x - fingerPos.x;
	float dy = last.y - fingerPos.y;
	valueTouch.x = dx;
	valueTouch.y = dy;
	last.x =  fingerPos.x;
	last.y = fingerPos.y;


	dx = anchor.x - fingerPos.x;
	dy = anchor.y - fingerPos.y;
	valueJoy.x = dx;
	valueJoy.y = dy;
	if (valueJoy.x > 1)
		valueJoy.x = 1;
	else if (valueJoy.x < -1)
		valueJoy.x = -1;

	if (valueJoy.y > 1)
		valueJoy.y = 1;
	else if (valueJoy.y < -1)
		valueJoy.y = -1;

	doUpdate();

}

void TouchJoy::doUpdate()
{
	//LOGTOUCH("xT = %f yT = %f,xJ = %f yJ = %f",valueTouch.x,valueTouch.y,valueJoy.x ,valueJoy.y);
	signal_move.emit(valueJoy.x,valueJoy.y,valueTouch.x,valueTouch.y);
}

void TouchJoy::saveXML(TiXmlDocument &doc)
{
	TiXmlElement * root = new TiXmlElement(tag.c_str());
	doc.LinkEndChild( root );

	ControlSuper::saveXML(*root);
}

void TouchJoy::loadXML(TiXmlDocument &doc)
{
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem=hDoc.FirstChild( tag ).Element();

	if (!pElem) //Check exists, if not just leave as default
		return;

	ControlSuper::loadXML(*pElem);
}
