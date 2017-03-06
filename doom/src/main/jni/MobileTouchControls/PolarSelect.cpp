#include "PolarSelect.h"
#include "TouchControlsConfig.h"
#include <math.h>       /* sin */

#define PI 3.14159265

#define CENTRE_SIZE  0.1

using namespace touchcontrols;

PolarSelect::PolarSelect(std::string tag,RectF pos,std::string image_filename):
						ControlSuper(TC_TYPE_WHEELSEL,tag,pos)
{
	image = image_filename;
	id = -1;

	hideGraphics = false;
	updateSize();

};

void PolarSelect::setHideGraphics(bool v)
{
	hideGraphics = v;
}

void PolarSelect::resetOutput(){
	reset();
}


void PolarSelect::updateSize()
{

	glRect.resize(controlPos.right - controlPos.left, controlPos.bottom - controlPos.top);

	centre.x = controlPos.left + (controlPos.right - controlPos.left)/2;
	centre.y = controlPos.top + (controlPos.bottom - controlPos.top)/2;
}

float PolarSelect::distCentre(float x, float y) //also scales so 1 is half the width
{

	float dist = ((centre.x - x) * (centre.x - x)) + ((centre.y - y) * (centre.y - y));
	dist = sqrt(dist);

	dist = dist * 1/(controlPos.width()/2);
	return dist;
}

float PolarSelect::getAngle(float x, float y)
{

	float a = x  - centre.x;
	float o = y  - centre.y;

	a = a * ((float)touchcontrols::ScaleX / (float)touchcontrols::ScaleY);

	float angle = atan2(o,a);

	angle += PI/2;

	if (angle < 0)
		angle = (2*PI) + angle;

	angle *= 180/PI;

	return angle;
}

bool PolarSelect::processPointer(int action, int pid, float x, float y)
{
	if (action == P_DOWN)
	{
		if (id == -1) //Only process if not active
		{
			if (controlPos.contains(x, y))
			{
				id = pid;

				fingerPos.x = x;
				fingerPos.y = y;

				signal_action.emit(POLARSELECT_DOWN,getAngle(x,y),distCentre(x,y));

				return true;
			}

		}
		return false;
	}
	else if (action == P_UP)
	{
		if (id == pid)
		{
			signal_action.emit(POLARSELECT_UP,getAngle(x,y),distCentre(x,y));
			reset();
			return true;
		}
		return false;
	}
	/*
	else if (action == P_ALLUP)
	{
		if (id != -1) //Should not get this, but could be buggy drivers
		{
			signal_action.emit(POLARSELECT_UP,getAngle(x,y),distCentre(x,y));
			reset();
			return true;
		}
		return false;
	}
	*/
	else if(action == P_MOVE)
	{
		if (pid == id) //Finger already down
		{
			//LOGTOUCH("centre: %f   %f",centre.x,centre.y);
			fingerPos.x = x;
			fingerPos.y = y;

			signal_action.emit(POLARSELECT_MOVE,getAngle(x,y),distCentre(x,y));

			return true;
		}
		return false;
	}
    
    return false;
}

bool PolarSelect::initGL()
{
	int x,y;
	glTex = loadTextureFromPNG(image,x,y);
    
    return false;
}

bool PolarSelect::drawGL(bool forEditor)
{
	//drawLines(0,0,*glLines);
#ifndef USE_GLES2
	//glRotatef(50,0,0,1);
#endif

	drawRect(glTex,controlPos.left,controlPos.top,glRect);

    //LOGTOUCH("state = %d, counter = %d",doubleTapState,doubleTapCounter);
    return false;
}

void PolarSelect::reset()
{
	id = -1;
	signal_action.emit(POLARSELECT_UP,0,0);
	doUpdate();
}

void PolarSelect::calcNewValue()
{


}

void PolarSelect::doUpdate()
{

}

void PolarSelect::saveXML(TiXmlDocument &doc)
{
	TiXmlElement * root = new TiXmlElement(tag.c_str());
	doc.LinkEndChild( root );

	ControlSuper::saveXML(*root);
}

void PolarSelect::loadXML(TiXmlDocument &doc)
{
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem=hDoc.FirstChild( tag ).Element();

	if (!pElem) //Check exists, if not just leave as default
		return;

	ControlSuper::loadXML(*pElem);
}
