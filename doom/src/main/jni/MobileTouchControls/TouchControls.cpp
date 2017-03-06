#include "GLheader.h"

#include "TouchControls.h"
#include <math.h>

extern int android_app_is_shutting_down;

using namespace touchcontrols;


TouchControls::TouchControls(std::string t,bool en,bool editable, int edit_group,bool showExtraSettings)
{
	tag = t;
	enabled = en;
	animating = false;
	fading = false;
	editing = false;
	alpha = 0.5;
	r = g = b = 1.0f;
	editGroup = edit_group;
	passThroughTouch = true;


	int lines = ScaleX+1 + ScaleY+1;

#ifdef USE_GLES2
	float GLES2scaleX(float X);
	float GLES2scaleY(float Y);
#endif

	editorButton = 0;
	settingsButton  = 0;

	if (editable)
	{
		grid = new GLLines(lines);

		int l=0;
		for (int n=0;n<ScaleX;n++)
		{
			grid->vertices[l+0] = (float)n/(float)ScaleX;
			grid->vertices[l+1] = 0;
#ifdef USE_GLES2
			grid->vertices[l+0] = GLES2scaleX((float)n/(float)ScaleX);
#endif
			grid->vertices[l+3] = (float)n/(float)ScaleX;
			grid->vertices[l+4] = -1;
#ifdef USE_GLES2
			grid->vertices[l+3] = GLES2scaleX((float)n/(float)ScaleX);
			grid->vertices[l+4] = -GLES2scaleX(1);
#endif
			l +=6;
		}
		for (int n=1;n<ScaleY+1;n++)
		{
			grid->vertices[l+0] = 0;
			grid->vertices[l+1] = -(float)n/(float)ScaleY;
#ifdef USE_GLES2
			grid->vertices[l+1] = -GLES2scaleY((float)n/(float)ScaleY);
#endif

			grid->vertices[l+3] = 1;
			grid->vertices[l+4] = -(float)n/(float)ScaleY;
#ifdef USE_GLES2
			grid->vertices[l+3] = GLES2scaleY(1);
			grid->vertices[l+4] = -GLES2scaleY((float)n/(float)ScaleY);
#endif
			l +=6;
		}
		if (editGroup == -1)
		{
			editorButton = new touchcontrols::Button("edit_control",touchcontrols::RectF(11,0,13,2),"settings",0);
			editorButton->signal_button.connect(  sigc::mem_fun(this,&TouchControls::editorButtonPress) );
		}
		if (showExtraSettings)
		{
			settingsButton = new touchcontrols::Button("settings_control",touchcontrols::RectF(0,0,2,2),"settings_bars",0);
			settingsButton->signal_button.connect(  sigc::mem_fun(this,&TouchControls::settingsButtonPress) );
		}

	}

}

void TouchControls::setColor(float r_,float g_, float b_)
{
	r = r_;
	g = g_;
	b = b_;
}

void TouchControls::setPassThroughTouch(bool v)
{
	passThroughTouch = v;
}

void  TouchControls::resetDefault()
{
	if (xmlFilename.length() > 0)
		loadXML(xmlFilename + ".default");

}

void TouchControls::setAllButtonsEnable(bool value)
{
	for (int n=0;n<controls.size();n++)
	{
		ControlSuper *c = controls.at(n);
		if (c->type == TC_TYPE_BUTTON)
		{
			c->setEnabled(value);
		}
	}
}

void TouchControls::animateIn(int steps)
{
	float top = 1;
	for (int n=0;n<controls.size();n++)// Find lowest number (nearest top of screen) edge
	{
		ControlSuper *c = controls.at(n);
		if (c->isEnabled())
		{
			if (c->controlPos.top < top)
				top = c->controlPos.top;
		}
	}

	animating = true;
	slideDir = 0;
	slidePos = 1 - top; //So we want it to slide in from the bottom
	animateStep = slidePos/(float)steps;
	setEnabled(true);
}

void TouchControls::animateOut(int steps)
{
	float top = 1;
	for (int n=0;n<controls.size();n++)// Find lowest number (nearest top of screen) edge
	{
		ControlSuper *c = controls.at(n);
		if (c->isEnabled())
		{
			if (c->controlPos.top < top)
				top = c->controlPos.top;
		}
	}

	animating = true;
	slideDir = 1;
	slidePos = 0;
	slideMax = (1-top);
	animateStep = slideMax/(float)steps;
	//setEnabled(false);
}

void TouchControls::fade(fadedir_t dir,int steps)
{
	//LOGTOUCH("fade %d  %d",in,steps);
	if (dir == FADE_IN) //Fade in
	{
		fadePos = 0;
	}
	else //Fade out
	{
		fadePos = 1;
	}
	fadeDir = dir;
	fadeStep = (float)1/(float)steps;
	fading = true;
}


void TouchControls::edit()
{
	editing = true;
	selectedCtrl = 0;
	longPressTime = -1;
	finger1.enabled = false;
	finger2.enabled = false;
}

void TouchControls::stopEdit()
{
	saveXML(xmlFilename);
	editing = false;
}

bool TouchControls::isEditing()
{
	return editing;
}

void TouchControls::setEnabled(bool v)
{
	enabled = v;
}

void TouchControls::resetOutput()
{
	int size = controls.size();

	for (int n=0;n<size;n++)
	{
		ControlSuper *cs = controls.at(n);
		// LOGTOUCH("setEnabled reset to %s",cs->tag.c_str());
		cs->resetOutput();
	}
}

void TouchControls::setAlpha(float a)
{
	alpha = a;
}

bool TouchControls::isEnabled()
{
	return enabled && !animating;
}


void TouchControls::addControl(Button *cntrl)
{
	cntrl->signal_buttonDown.connect(  sigc::mem_fun(this,&TouchControls::buttonDown) );
	cntrl->signal_buttonUp.connect(  sigc::mem_fun(this,&TouchControls::buttonUp) );
	cntrl->signal_button.connect(  sigc::mem_fun(this,&TouchControls::button) );
	controls.push_back((cntrl));
}

void TouchControls::addControl(ButtonExt *cntrl)
{
	cntrl->signal_button.connect(  sigc::mem_fun(this,&TouchControls::button) );
	controls.push_back((cntrl));
}


void TouchControls::addControl(ControlSuper *cntrl)
{
	controls.push_back((cntrl));
}

bool TouchControls::processPointer(int action, int pid, float x, float y)
{
#ifdef __ANDROID__
	if (android_app_is_shutting_down)
		return 0;
#endif
	if (editorButton)
		editorButton->processPointer(action,pid, x, y);

	if (!editing)
	{
		int size = controls.size();

		for (int n=0;n<size;n++)
		{
			ControlSuper *cs = controls.at(n);
			if (cs->isEnabled())
				if (cs->processPointer(action,pid, x, y))
				{
					//If it is a touch pad or mouse, break out so nothing under it gets data
					if (( cs->type == TC_TYPE_TOUCHJOY ) || ( cs->type == TC_TYPE_MOUSE ))
						break;
				}
		}

		if ( !passThroughTouch ) // If not passing through return true
			return true;
	    else
		    return false;
	}
	else
	{
		if (settingsButton)
			settingsButton->processPointer(action,pid, x, y);

		if (action == P_DOWN)
		{
			if (pid < 2)
			{
				if ((!finger1.enabled) && (!finger1.enabled) && (pid == 0))
				{
					//selectedCtrl = 0;
					tapDeselect = true;
					for (int n=0;n<controls.size();n++)
					{
						ControlSuper *cs = controls.at(n);
						if (cs->isEnabled() && !cs->isHidden() &&  (cs->type != TC_TYPE_MOUSE))
							if (cs->controlPos.contains(x, y))
							{
								selectedCtrl = cs;
								tapDeselect = false;
								break;
							}
					}

					longPressTime = 0;
					totalFingerMove = 0;
					finger1.enabled = true;
					finger1.x = x;
					finger1.y = y;
				}
				else if ((finger1.enabled) && (pid == 1))
				{
					longPressTime = -1; //Disable long press count
					finger2.enabled = true;
					finger2.x = x;
					finger2.y = y;
					oldDist.x = fabs(finger1.x - finger2.x);
					oldDist.y = fabs(finger1.y - finger2.y);
				}
			}
		}
		else if (action == P_UP)
		{
			if (pid < 2)
			{
				//This is to deselect all controls if you tap in a clear space
				if ( tapDeselect )
				{
					if ((pid == 0) && (totalFingerMove <  0.03))
					{
					//	selectedCtrl = 0;
					}
				}

				if (selectedCtrl != 0)
					snapControl(selectedCtrl);

				if (pid == 0)
					finger1.enabled = false;
				else if (pid == 1)
					finger2.enabled = false;

				longPressTime = -1; //Disable long press count
			}
		}
		else if(action == P_MOVE)
		{
			if ((finger1.enabled) && (!finger2.enabled)) //drag
			{
				totalFingerMove += fabs(x - finger1.x) + fabs(y - finger1.y);

				if (selectedCtrl != 0)
				{
					selectedCtrl->controlPos.offset(x - finger1.x, y - finger1.y);
					windowControl(selectedCtrl);

					finger1.x = x;
					finger1.y = y;

				}
			}
			else if ((finger1.enabled) && (finger2.enabled)) //zoom
			{
				if (selectedCtrl != 0)
				{
					float newDistX = fabs(finger1.x - finger2.x);
					if (fabs(newDistX - oldDist.x)>((float)1/(float)ScaleX))
					{
						selectedCtrl->controlPos.right += (newDistX - oldDist.x);
						oldDist.x = newDistX;

						if (selectedCtrl->controlPos.width()<(1/(float)ScaleX))
						{
							selectedCtrl->controlPos.right = selectedCtrl->controlPos.left + (1/(float)ScaleX);
						}
						else if  (selectedCtrl->controlPos.width()>0.5)
						{
							selectedCtrl->controlPos.right = selectedCtrl->controlPos.left + 0.5;
						}
					}

					float newDistY = fabs(finger1.y - finger2.y);
					if (fabs(newDistY - oldDist.y)>((float)1/(float)ScaleY))
					{
						selectedCtrl->controlPos.bottom += (newDistY - oldDist.y);
						oldDist.y = newDistY;

						if (selectedCtrl->controlPos.height()<(1/(float)ScaleY))
						{
							selectedCtrl->controlPos.bottom = selectedCtrl->controlPos.top + (1/(float)ScaleY);
						}
						else if  (selectedCtrl->controlPos.width()>0.5)
						{
							selectedCtrl->controlPos.bottom = selectedCtrl->controlPos.top + 0.5;
						}

					}

					snapControl(selectedCtrl);
					windowControl(selectedCtrl);
				}

				if (pid == 0)
				{
					finger1.x = x;
					finger1.y = y;
				}
				else if (pid == 1)
				{
					finger2.x = x;
					finger2.y = y;
				}
			}
		}
	}

    return false;
}

void TouchControls::editorButtonPress(int state,int code)
{

	if (state == 1)
	{
		if (!editing)
			edit();
		else
			stopEdit();
	}
}

void TouchControls::settingsButtonPress(int state,int code)
{
	signal_settingsButton.emit(state);
}

void TouchControls::windowControl(ControlSuper *ctrl)
{
	if (ctrl->controlPos.left<0)
		ctrl->controlPos.offsetTo(0, ctrl->controlPos.top);
	else if (ctrl->controlPos.right>1)
		ctrl->controlPos.offsetTo(1-(ctrl->controlPos.right-ctrl->controlPos.left),
				ctrl->controlPos.top);

	if (ctrl->controlPos.top<0)
		ctrl->controlPos.offsetTo( ctrl->controlPos.left,0);
	else if (ctrl->controlPos.bottom>1)
		ctrl->controlPos.offsetTo( ctrl->controlPos.left,
				1-(ctrl->controlPos.bottom-ctrl->controlPos.top));
	ctrl->updateSize();

}

void  TouchControls::snapControl(ControlSuper *ctrl)
{
	int t = floor((ctrl->controlPos.left * (float)ScaleX) + 0.5);
	ctrl->controlPos.left = (float)t/(float)ScaleX;
	t = floor((ctrl->controlPos.right * (float)ScaleX) + 0.5);
	ctrl->controlPos.right = (float)t/(float)ScaleX;
	t = floor((ctrl->controlPos.top * (float)ScaleY) + 0.5);
	ctrl->controlPos.top = (float)t/(float)ScaleY;
	t = floor((ctrl->controlPos.bottom * (float)ScaleY) + 0.5);
	ctrl->controlPos.bottom = (float)t/(float)ScaleY;
	ctrl->updateSize();
}


int TouchControls::draw ()
{
	if (fading)
	{
		if (fadeDir == FADE_IN) //Fading in
		{
			fadePos += fadeStep;
			if (fadePos >= 1)
				fading = false;

		}
		else //Fading out
		{
			fadePos -= fadeStep;
			if (fadePos <= 0)
			{
				fading = false;
				setEnabled(false); //now also disable the control
			}
		}
		//LOGTOUCH("fadePos = %f",fadePos);

		glColor4f(r, g, b,alpha * fadePos);

	}
	else
		glColor4f(r, g, b,alpha);

	int size = controls.size();
	for (int n=0;n<size;n++) //draw
	{
		ControlSuper *c = controls.at(size-1-n);
		if (c->isEnabled())
		{
			glLoadIdentity();
			glScalef(GLScaleWidth, GLScaleHeight, 1);
            
			if (animating)
				glTranslatef(0, -slidePos, 0);
            
			c->drawGL();
		}
	}

    // This is here is disable scissor test if UI_Window enabled it..will this be OK??
    glDisable( GL_SCISSOR_TEST );

	if (editorButton)
	{
		glLoadIdentity();
		glScalef(GLScaleWidth, GLScaleHeight, 1);
		editorButton->drawGL();
	}

	if (animating)
	{
		if (slideDir == 0)
		{
			slidePos -= animateStep;
			if (slidePos <= 0)
				animating = false;

		}
		else //Animate out
		{
			slidePos += animateStep;
			if (slidePos >= slideMax)
			{
				animating = false;
				setEnabled(false);
			}
		}
	}

	if (editing)
		return 1;
	else
		return 0;
}

int  TouchControls::drawEditor ()
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glScalef(GLScaleWidth, GLScaleHeight, 1);
	drawLines(0,0,*grid);

	int size = controls.size();
	for (int n=0;n<size;n++) //draw
	{
		ControlSuper *c = controls.at(size-1-n);
		if (c->isEnabled() &&  (c->type != TC_TYPE_MOUSE))
		{

			GLRect rect;
			rect.resize(c->controlPos.right - c->controlPos.left, c->controlPos.bottom - c->controlPos.top);
			glLoadIdentity();
			glScalef(GLScaleWidth, GLScaleHeight, 1);

			if (!c->isHidden()) //Not hidden control
				drawRect((GLfloat)1,(GLfloat)0.5,(GLfloat)0,(GLfloat)0.2,c->controlPos.left,c->controlPos.top,rect);

			glLoadIdentity();
			glScalef(GLScaleWidth, GLScaleHeight, 1);
			glColor4f(r, g, b, 1 );

			c->drawGL(true);

			glLoadIdentity();
			glScalef(GLScaleWidth, GLScaleHeight, 1);

			//  if (c->isHidden()) //Hidden controls over color
			//     drawRect((GLfloat)0.5,(GLfloat)0.5,(GLfloat)0.5,(GLfloat)0.5,c->controlPos.left,c->controlPos.top,rect);

		}
	}

	ControlSuper * sel = selectedCtrl; //Save pointer because drawn of different thread

	if (sel != 0)
	{
		GLRect rect;
		rect.resize(sel->controlPos.right - sel->controlPos.left, sel->controlPos.bottom - sel->controlPos.top);
		glLoadIdentity();
		glScalef(GLScaleWidth, GLScaleHeight, 1);
		drawRect((GLfloat)0.5,(GLfloat)0.3,(GLfloat)0.8,(GLfloat)0.5,sel->controlPos.left,sel->controlPos.top,rect);
	}


	if (editorButton)
	{
		glLoadIdentity();
		glScalef(GLScaleWidth, GLScaleHeight, 1);
	//	glColor4f(1.0, 0.7, 0.7, 1 );
		editorButton->drawGL();
	}

	if (editing)
	{
		if (settingsButton)
		{
			glLoadIdentity();
			glScalef(GLScaleWidth, GLScaleHeight, 1);
		//	glColor4f(1.0, 0.7, 0.7, 1 );
			settingsButton->drawGL();
		}
	}

	/* Don't do this any more, use the list to hide/show!
	if (editing)
	{
		if (longPressTime>=0)
		{
			longPressTime++;
			if (longPressTime > 60) //About 1 second @ 60fpd
			{
				if (totalFingerMove < 0.03) //Finger must stay still for a long press
				{
					LOGTOUCH("Long press active");
                    if ((selectedCtrl != 0) && (selectedCtrl->type == TC_TYPE_BUTTON)) //Only can hide buttons
					{
						selectedCtrl->setHidden(!(selectedCtrl->isHidden())); //toggle hidden state
					}
				}

				longPressTime = -1; //cancel longpress
			}
		}
	}
	 */
	if (editing)
		return 1;
	else
		return 0;
}

void  TouchControls::initGL ()
{
	int size = controls.size();
	for (int n=0;n<size;n++) //draw
	{
		ControlSuper *c = controls.at(size-1-n);
		c->initGL();
	}

	if (editorButton)
		editorButton->initGL();

	if (settingsButton)
		settingsButton->initGL();
}

void TouchControls::setXMLFile(std::string file)
{
	xmlFilename = file;
	saveXML(xmlFilename + ".default"); //Save incase we want to reset positions
	loadXML(xmlFilename);
}

void TouchControls::saveXML(std::string filename)
{
	if (filename.length() == 0)
		return;

	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	for (int n=0;n< controls.size();n++) //draw
	{
		ControlSuper *c = controls.at(n);
		c->saveXML(doc);
	}

	doc.SaveFile(filename);
}

void TouchControls::loadXML(std::string filename)
{

    //The hell..crashes on destructor when on stack..
	//TiXmlDocument doc(filename.c_str());
    TiXmlDocument *doc = new TiXmlDocument(filename.c_str());
    

	if (!doc->LoadFile()) return;

	for (int n=0; n < controls.size();n++) //draw
	{
		ControlSuper *c = controls.at(n);
		c->loadXML(*doc);
		c->updateSize();
	}
    
    delete doc;
}

std::vector<ControlSuper *> * TouchControls::getControls()
{
	return &controls;

}

void *TouchControls::getControl(std::string name)
{
	for (int n=0;n< controls.size();n++) //draw
	{
		ControlSuper *c = controls.at(n);
		if (c->tag == name)
			return c;
	}

	return NULL;
}

