

#include <string>
#include <stdbool.h>
#include <vector>

#include "TouchControlsConfig.h"
#include "Button.h"
#include "ButtonExt.h"
#include "TouchJoy.h"
#include "Mouse.h"
#include "WheelSelect.h"
#include "MultitouchMouse.h"
#include "PolarSelect.h"
#include "JoyStick.h"

#include "UI_Window.h"
#include "UI_Slider.h"
#include "UI_TextBox.h"
#include "UI_Button.h"
#include "UI_Switch.h"


#ifndef _TouchControls_H_
#define _TouchControls_H_

//#define ScaleX
namespace touchcontrols
{

typedef enum {
    FADE_IN,FADE_OUT
} fadedir_t;

//const int ScaleX = 26;
//const int ScaleY = 16;
class ControlData
{

public:
	float left,top,right,bottom;
	float alpha;
	std::string tag;
	bool enabled;
};


class TouchControls
{

	std::vector<ControlSuper *> controls;

	void buttonDown(int code)
	{
		//LOGTOUCH("buttonDown %d",code);
		signal_buttonDown.emit(code);
	}

	void buttonUp(int code)
	{
		//LOGTOUCH("buttonUp %d",code);
		signal_buttonUp.emit(code);
	}

	void button(int state,int code)
	{
		signal_button.emit(state, code);
	}

	bool passThroughTouch; // set to true (default) to pass touches to controls underneath
	//EDITOR
	std::string xmlFilename;

	bool editing;
	GLLines *grid;
	ControlSuper *selectedCtrl;
	PointF finger1,finger2;
	PointF oldDist;

	//Vars to detect a long press
	int longPressTime;
	float totalFingerMove;

	void windowControl(ControlSuper *ctrl);
	void snapControl(ControlSuper *ctrl);

	Button *editorButton;
	Button *settingsButton;

	void editorButtonPress(int state,int code);
	void settingsButtonPress(int state,int code);

	//ANIMATIONS STUFF
	float slidePos; //current pos
	float slideMax; //post to slide to when animating out
	int slideDir; //0 = in, 1 = out

	float animateStep;
	bool animating;

	float fadePos; //current fade
	fadedir_t fadeDir;
	float fadeStep;
	bool fading;

	bool tapDeselect; //true if first press down in empty space
public:

	bool enabled;

	float alpha,r,g,b;

	int editGroup;

	std::string tag;


	sigc::signal<void, int> signal_buttonDown;
	sigc::signal<void, int> signal_buttonUp;
	sigc::signal<void, int, int> signal_button;

	sigc::signal<void, int> signal_settingsButton;

	TouchControls(std::string t,bool en, bool editable,int edit_group = -1,bool showExtraSettgins = true);

	void setColor(float r,float g, float b);

	void setPassThroughTouch(bool v);

	void animateIn(int steps);
	void animateOut(int steps);

	void fade(fadedir_t dir,int steps);


	void edit();
	void stopEdit();
	bool isEditing();

	void setEnabled(bool v);

	bool isEnabled();

	void setAlpha(float a);

	void addControl(Button *cntrl);

    void addControl(ButtonExt *cntrl);

	void addControl(ControlSuper *cntrl);

	int draw ();

	int drawEditor ();

	void initGL ();

	bool processPointer(int action, int pid, float x, float y);

	void saveXML(std::string filename);

	void loadXML(std::string filename);

	void setXMLFile(std::string file);

	void resetDefault(); //Reset control positions to init XML

    void resetOutput(); //Make all controls output the reset state

    //Enable or disable all BUTTONS in control group
    void setAllButtonsEnable(bool value);


    std::vector<ControlSuper *> * getControls();

    void *getControl(std::string name); //Get control by name, obviously you must cast to correct type!
};

}


#endif
