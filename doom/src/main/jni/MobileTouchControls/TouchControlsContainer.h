

#include <string>
#include <stdbool.h>
#include <vector>

#include "TouchControlsInterface.h"
#include "TouchControlsConfig.h"
#include "TouchControls.h"

#ifdef USE_LIBROCKET
#include "TouchGui.h"
#endif

#ifndef _TouchControlsContainer_H_
#define _TouchControlsContainer_H_

//#define ScaleX
namespace touchcontrols
{


class TouchControlsContainer: public TouchControlsInterface
{

	std::vector<TouchControls *> controls;

	TouchControls * editingControls = NULL;


	//For when we have an edit group
	Button *editorButton;
	void editorButtonPress(int state,int code);
	int drawEditButton;
    


public:

	float editButtonAlpha;

   // Set to UI control group
	TouchControls * uiControls = NULL;

	sigc::signal<void,int> signal_settings;

    sigc::signal<void> openGL_start;
    sigc::signal<void> openGL_end;
    
	TouchControlsContainer();

	void addControlGroup(TouchControls *cntrl);

	void editControls(TouchControls * ctrl);

	int draw ();

	void initGL (const char * root_path = NULL);

	bool processPointer(int action, int pid, float x, float y);

	bool isEditing();
	void finishEditing();

	void resetDefaults();

	TouchControls* getEditingControls();
    
    //Needs to be here for IOS, breaks otherwise. Memory or compiler bug
#ifdef USE_LIBROCKET
    TouchGui *touchGui;
#endif

};

}


#endif
