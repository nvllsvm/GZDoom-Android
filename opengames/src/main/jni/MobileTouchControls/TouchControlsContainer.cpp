
#include "GLheader.h"

#include "TouchControlsContainer.h"




using namespace touchcontrols;


TouchControlsContainer::TouchControlsContainer()
{
	//Just for if we have an edit group
	editorButton = new touchcontrols::Button("edit_control",touchcontrols::RectF(11,0,13,2),"settings",0);
	editorButton->signal_button.connect(  sigc::mem_fun(this,&TouchControlsContainer::editorButtonPress) );
	editButtonAlpha = 0;
}

void TouchControlsContainer::resetDefaults()
{
	int size = controls.size();

	for (int n=0;n<size;n++)
	{
		TouchControls *cs = controls.at(n);
		cs->resetDefault();
	}
}

void TouchControlsContainer::addControlGroup(TouchControls *cntrl)
{
	controls.push_back((cntrl));
}

void TouchControlsContainer::editControls(TouchControls * ctrl)
{
	editingControls = ctrl;
	editingControls->edit();
}

bool TouchControlsContainer::isEditing()
{
	return (editingControls == 0)?false:true;
}

void TouchControlsContainer::finishEditing()
{
	if (editingControls)
	{
	    if( uiControls != NULL && uiControls->enabled )
        {
            uiControls->setEnabled( false );
        }
        else
        {
            editingControls->stopEdit();
            editingControls = 0;
            signal_settings.emit(0);
        }
	}
}

bool TouchControlsContainer::processPointer(int action, int pid, float x, float y)
{

    // Check if the UI is active
    if( uiControls != NULL )
	{
        if( uiControls->enabled )
        {
            uiControls->processPointer(action,pid, x, y);
            return false;
        }
	}

	if (drawEditButton)
		editorButton->processPointer(action,pid, x, y);

#ifdef USE_LIBROCKET
    touchGui->processPointer(action, pid, x, y);
#endif
    
	if (editingControls == 0)
	{
		int size = controls.size();
		bool under=false;
		for (int n=0;n<size;n++)
		{
			TouchControls *cs = controls.at(n);
			if (cs->enabled)
			{
				if ( cs->processPointer(action,pid, x, y) == true )
				{
				    // Don't pass touch if returns true
                    break;
				}
		    }
		}
		//if (!under) downInSpace = true;
		return false;
	}
	else
	{
		editingControls->processPointer(action,pid, x, y);

		return false;
	}
}

int TouchControlsContainer::draw ()
{
	int size = controls.size();

	//Do this first to stop flicker when switching
	for (int n=0;n<size;n++) //draw
	{
		TouchControls *c = controls.at(size-1-n);

		if (c->isEditing())
		{
			editingControls = c;
			signal_settings.emit(1); //Editing controls
			break;
		}
	}

#ifndef USE_GLES2
	glDisable(GL_ALPHA_TEST);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY );

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
#endif

	openGL_start.emit();

	if (editingControls == 0)
	{
		int drawEditButton_ = 0;

		for (int n=0;n<size;n++) //draw
		{
			TouchControls *c = controls.at(size-1-n);

			if (c->enabled)
			{
				c->draw();

				if (c->editGroup != -1)
					drawEditButton_ = 1;
			}

		}

		if (drawEditButton_ != drawEditButton)
			drawEditButton = drawEditButton_;

		if (drawEditButton)
		{
			if (editButtonAlpha != 0)
			{
				//LOGTOUCH("editButtonAlpha");
				glColor4f(1, 1, 1,editButtonAlpha);
			}

			glLoadIdentity();
			glScalef(GLScaleWidth, GLScaleHeight, 1);
		//	glColor4f(1.0, 0.7, 0.7, 1 );
			editorButton->drawGL();
		}

#ifdef USE_LIBROCKET
        touchGui->update();
#endif
	}
	else
	{

		if (!editingControls->drawEditor()) //Check if finished editing..
		{
			signal_settings.emit(0);
			editingControls = 0;
		}

		if (drawEditButton)
		{
			drawEditButton = true;

			glLoadIdentity();
			glScalef(GLScaleWidth, GLScaleHeight, 1);
			editorButton->drawGL();

		}
#ifdef USE_LIBROCKET
        touchGui->update();
#endif
	}

    if( uiControls != NULL )
	{
        if( uiControls->enabled )
        {
            //Grey out background
            GLRect rect;
            rect.resize(1,1);
            glLoadIdentity();
            glScalef(GLScaleWidth, GLScaleHeight, 1);
            drawRect((GLfloat)0, (GLfloat)0, (GLfloat)0, (GLfloat)0.7, 0.f, 0.f, rect );

            setFixAspect ( false );
            uiControls->draw();
            setFixAspect ( true );
        }
	}

    openGL_end.emit();

    return 0;
}

void TouchControlsContainer::initGL (const char * root_path)
{
	int size = controls.size();
	for (int n=0;n<size;n++) //draw
	{
		TouchControls *c = controls.at(size-1-n);
		c->initGL();
	}

	if (editorButton)
		editorButton->initGL();
    
#ifdef USE_LIBROCKET
    touchGui = new TouchGui(root_path,640,480);
#endif

}


void TouchControlsContainer::editorButtonPress(int state,int code)
{

	if (state)
	{
		if (editingControls == 0) //Not currently editing
		{
			int size = controls.size();
			for (int n=0;n<size;n++)
			{
				TouchControls *cs = controls.at(n);
				if (cs->enabled)
				{
					if (cs->editGroup != -1)
					{
						cs->edit();
						break;
					}
				}
			}
		}
		else //Find next in group
		{
			int curEditGroup = -1;
			int size = controls.size();
			for (int n=0;n<size;n++)
			{
				TouchControls *cs = controls.at(n);

				if (curEditGroup != -1)
				{
					if (cs->editGroup == curEditGroup)
					{
						cs->edit();
						break;
					}
				}

				if (editingControls == cs )
				{
					curEditGroup = cs->editGroup;
					cs->stopEdit();
				}
			}
		}
	}
}

TouchControls *TouchControlsContainer::getEditingControls()
{
    return editingControls;
}
