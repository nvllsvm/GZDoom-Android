#include "Mouse.h"
#include "TouchControlsConfig.h"
#include <sys/time.h>

#define TAP_SPEED 10
using namespace touchcontrols;

Mouse::Mouse(std::string tag,RectF pos,std::string image_filename):
		ControlSuper(TC_TYPE_MOUSE,tag,pos)
{
	image = image_filename;
	id = -1;
	id2 = -1;
	glLines = new GLLines(2);
	hideGraphics = false;
	updateSize();
	glitchFix = 0;
};

void Mouse::setHideGraphics(bool v)
{
	hideGraphics = v;
}

void Mouse::resetOutput(){
    reset();
}

void Mouse::updateSize()
{

	//glRect.resize(controlPos.right - controlPos.left, controlPos.bottom - controlPos.top);
	glRect.resize(0.1, 0.16);


	glLines->vertices[0] = controlPos.left;
	glLines->vertices[1] = -controlPos.top;
	glLines->vertices[3] = controlPos.right;
	glLines->vertices[4] = -controlPos.top;


	glLines->vertices[6] =  controlPos.right ;
	glLines->vertices[7] =  -controlPos.top;
	glLines->vertices[9] =  controlPos.right;
	glLines->vertices[10] = -controlPos.bottom ;
}

double Mouse::getMS()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return  (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
}
bool Mouse::processPointer(int action, int pid, float x, float y)
{
	if (action == P_DOWN)
	{
		if (id == -1) //Only process if not active
		{
			if (controlPos.contains(x, y))
			{
				id = pid;

				glitchFix = 2; //Fix jumpy initial move

				last.x = x;
				last.y = y;
				anchor.x = x;
				anchor.y = y;
				fingerPos.x = x;
				fingerPos.y = y;
				tapCounter = 0;

				signal_action.emit(TOUCHMOUSE_DOWN,fingerPos.x,fingerPos.y,0,0);
				return true;
			}
		}
		else //second finger down
		{
			if (controlPos.contains(x, y))
			{
				id2 = pid;
				signal_action.emit(TOUCHMOUSE_UP,fingerPos.x,fingerPos.y,0,0); //This also up?...
				signal_action.emit(TOUCHMOUSE_2_DOWN,fingerPos.x,fingerPos.y,0,0);
			}
		}
		return false;
	}
	else if (action == P_UP)
	{
		if (id == pid)
		{

			//Simple check to see if finger moved very much
			if ((tapCounter < TAP_SPEED) &&
					(((abs(anchor.x - fingerPos.x) + abs(anchor.y - fingerPos.y))) < 0.05))
			{
				signal_action.emit(TOUCHMOUSE_TAP,fingerPos.x,fingerPos.y,0,0);
			}
			signal_action.emit(TOUCHMOUSE_UP,fingerPos.x,fingerPos.y,0,0);
			signal_action.emit(TOUCHMOUSE_2_UP,fingerPos.x,fingerPos.y,0,0);
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
				//LOGTOUCH("glitchFix %d",glitchFix);
				if ((last.x != x) || (last.y != y))
				{
					last.x = x;
					last.y = y;
					anchor.x = x;
					anchor.y = y;
					fingerPos.x = x;
					fingerPos.y = y;
					glitchFix --;
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

bool Mouse::initGL()
{
	int x,y;
	glTex = loadTextureFromPNG(image,x,y);
    
    return false;
}

bool Mouse::drawGL(bool editor)
{
	//drawLines(0,0,*glLines);

	//drawRect(glTex,controlPos.left,controlPos.top,glRect);
	if (!hideGraphics)
	{
		if (id != -1)
			drawRect(glTex,fingerPos.x-glRect.width/2,fingerPos.y-glRect.height/2,glRect);
		else
			drawRect(glTex,controlPos.left+controlPos.width()/2-glRect.width/2,controlPos.top+controlPos.height()/2-glRect.height/2,glRect);

	}

	tapCounter++;
	//LOGTOUCH("state = %d, counter = %d",doubleTapState,doubleTapCounter);
    return false;
}

void Mouse::reset()
{
	id = -1;
	id2 = -1;
	valueRel.x = 0;
	valueRel.y = 0;

	doUpdate();

}

void Mouse::calcNewValue()
{
	float dx = last.x - fingerPos.x;
	float dy = last.y - fingerPos.y;
	valueRel.x = dx;
	valueRel.y = dy;
	last.x =  fingerPos.x;
	last.y = fingerPos.y;


	dx = anchor.x - fingerPos.x;
	dy = anchor.y - fingerPos.y;


	doUpdate();

}

void Mouse::doUpdate()
{
	//LOGTOUCH("xT = %f yT = %f,xJ = %f yJ = %f",valueTouch.x,valueTouch.y,valueJoy.x ,valueJoy.y);
	signal_action.emit(TOUCHMOUSE_MOVE,fingerPos.x,fingerPos.y,valueRel.x,valueRel.y);
}

void Mouse::saveXML(TiXmlDocument &doc)
{
	TiXmlElement * root = new TiXmlElement(tag.c_str());
	doc.LinkEndChild( root );

	ControlSuper::saveXML(*root);
}

void Mouse::loadXML(TiXmlDocument &doc)
{
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem=hDoc.FirstChild( tag ).Element();

	if (!pElem) //Check exists, if not just leave as default
		return;

	ControlSuper::loadXML(*pElem);
}
