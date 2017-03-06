#include "WheelSelect.h"
#include "TouchControlsConfig.h"
#include <math.h>       /* sin */

#define PI 3.14159265

#define CENTRE_SIZE  0.1

using namespace touchcontrols;

WheelSelect::WheelSelect(std::string tag,RectF pos,std::string image_filename, int segments):
ControlSuper(TC_TYPE_WHEELSEL,tag,pos)
{
	image = image_filename;
	id = -1;
	nbrSegs = segments;

	hideGraphics = false;
	updateSize();
	selectedSeg = -1;
	enabledSegs = 0;
	useFadeSegs = false;
};

void WheelSelect::setHideGraphics(bool v)
{
	hideGraphics = v;
}

void WheelSelect::resetOutput(){
    reset();
}

void WheelSelect::setSegmentEnabled(int seg, bool v)
{
	if (v)
		enabledSegs |= 1<<seg;
	else
		enabledSegs &= ~(1<<seg);

	useFadeSegs = true;
}


void WheelSelect::updateSize()
{

	glRect.resize(controlPos.right - controlPos.left, controlPos.bottom - controlPos.top);

	glRectFade.resize(glRect.width/5,glRect.height/5);

	centre.x = controlPos.left + (controlPos.right - controlPos.left)/2;
	centre.y = controlPos.top + (controlPos.bottom - controlPos.top)/2;
}
float WheelSelect::distCentre(float x, float y)
{

	float dist = ((centre.x - x) * (centre.x - x)) + ((centre.y - y) * (centre.y - y));
	dist = sqrt(dist);
	return dist;

}

bool WheelSelect::inCentre(float x, float y)
{
	float dist = distCentre(x,y);

	if (dist < CENTRE_SIZE)
		return true;
	else
		return false;
}

bool WheelSelect::processPointer(int action, int pid, float x, float y)
{
	if (action == P_DOWN)
	{
		if (id == -1) //Only process if not active
		{
			if (inCentre(x, y))
			{
				id = pid;


				last.x = x;
				last.y = y;
				anchor.x = x;
				anchor.y = y;
				fingerPos.x = x;
				fingerPos.y = y;
				selectedSeg = -1;
				signal_enabled.emit(1);

				return true;
			}
		}
		return false;
	}
	else if (action == P_UP)
	{
		if (id == pid)
		{

			signal_selected.emit(selectedSeg);
			signal_enabled.emit(0);

			reset();
			return true;
		}
		return false;
	}
	else if(action == P_MOVE)
	{
		if (pid == id) //Finger already down
		{
			//LOGTOUCH("centre: %f   %f",centre.x,centre.y);
			fingerPos.x = x;
			fingerPos.y = y;

			float a = fingerPos.x  - centre.x;
			float o = fingerPos.y  - centre.y;

			a = a * ((float)touchcontrols::ScaleX / (float)touchcontrols::ScaleY);
			//LOGTOUCH("len: %f   %f",o,a);

			//float angle = asin(o/a);
			float angle = atan2(o,a);

			angle += PI/2;


			if (angle < 0)
				angle = (2*PI) + angle;

			if (distCentre(x,y) > CENTRE_SIZE) //Only update if moved out of circle
			{
				selectedSeg =  angle * nbrSegs/(2*PI) ;
				signal_scroll.emit(selectedSeg);
			}
			return true;
		}
		return false;
	}
    
    return false;
}

bool WheelSelect::initGL()
{
	int x,y;
	glTex = loadTextureFromPNG(image,x,y);

	glTexFade  = loadTextureFromPNG("red_cross",x,y);
    
    return true;
}

bool WheelSelect::drawGL(bool forEditor)
{
	//drawLines(0,0,*glLines);
#ifndef USE_GLES2
	//glRotatef(50,0,0,1);
#endif
	if ((id != -1) || forEditor)
		drawRect(glTex,controlPos.left,controlPos.top,glRect);

	if (useFadeSegs)
	{
		if ((id != -1))
		{

			float ang = 360/nbrSegs/2;

			for (int n=0;n<nbrSegs;n++)
			{
				//Work out presuming square
				float h_len = (glRect.height/2) * 0.7;

				float a = cos(ang * PI / 180.0 ) * h_len;

				float o = sin(ang * PI / 180.0 ) * h_len;

				//Now scale as prob not square..
				o = o * (glRect.width/glRect.height);
				if (!(enabledSegs & 1<<n))
					drawRect(glTexFade,centre.x + o - glRectFade.width/2 ,centre.y - a -glRectFade.height/2 ,glRectFade);

				ang += 360/nbrSegs;
			}
		}
	}
    
    return false;
	//LOGTOUCH("state = %d, counter = %d",doubleTapState,doubleTapCounter);
}

void WheelSelect::reset()
{
	id = -1;


	doUpdate();

}

void WheelSelect::calcNewValue()
{


}

void WheelSelect::doUpdate()
{

}

void WheelSelect::saveXML(TiXmlDocument &doc)
{
	TiXmlElement * root = new TiXmlElement(tag.c_str());
	doc.LinkEndChild( root );

	ControlSuper::saveXML(*root);
}

void WheelSelect::loadXML(TiXmlDocument &doc)
{
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem=hDoc.FirstChild( tag ).Element();

	if (!pElem) //Check exists, if not just leave as default
		return;

	ControlSuper::loadXML(*pElem);
}
