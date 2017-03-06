#include "UI_TouchDefaultSettings.h"

namespace touchcontrols
{

#define BUTTON_CLOSE 1
#define BUTTON_RESET 2
#define BUTTON_ADDCTRL 3

static TouchControls *controls = NULL;

static void buttonPress( int state, int code )
{
    if( state == 0 && code == BUTTON_CLOSE )
        controls->setEnabled( false );

    if( code == BUTTON_RESET)
    {


    }
}

static void sliderChange( uint32_t uid, float value )
{

}

TouchControls *createDefaultSettingsUI()
{
    if ( controls == NULL )
    {
        controls = new TouchControls( "ui_settings", false, false );

        float textSize = 0.07f;
        uint32_t windownLeft = 4;
        uint32_t windowRight = 23;

        controls->addControl( new UI_TextBox( "text",         touchcontrols::RectF( windownLeft, 2, 22, 4 ), "font_dual", 1, UI_TEXT_CENTRE,"Settings", 0.09 ));
        controls->addControl( new Button( "close", touchcontrols::RectF( windownLeft, 2, windownLeft+2, 4 ), "ui_back_arrow", BUTTON_CLOSE ));

        controls->addControl( new UI_TextBox( "text",         touchcontrols::RectF( windownLeft, 4, 12, 6 ), "font_dual", 0, UI_TEXT_RIGHT, "Transparency:", textSize ));
        UI_Slider *slider =   new UI_Slider( "slider_alpha",  touchcontrols::RectF( 13, 4, windowRight-1, 6 ), "ui_slider_bg1", "ui_slider_handle", 0 );
        slider->signal_valueChange.connect(sigc::ptr_fun(&sliderChange) );
        controls->addControl( slider );

        controls->addControl( new UI_TextBox( "text",         touchcontrols::RectF( windownLeft, 6, 12, 8 ), "font_dual", 0, UI_TEXT_RIGHT, "Look sensitivity:", textSize ));
        slider =              new UI_Slider( "slider_pitch",  touchcontrols::RectF( 13, 6, windowRight-1, 8 ), "ui_slider_bg1", "ui_slider_handle", 0 );
        slider->signal_valueChange.connect(sigc::ptr_fun(&sliderChange) );
        controls->addControl( slider );

        controls->addControl( new UI_TextBox( "text",         touchcontrols::RectF( windownLeft, 8, 12, 10 ), "font_dual", 0, UI_TEXT_RIGHT, "Move sensitivity:", textSize ));
        slider =              new UI_Slider( "slider_yaw",    touchcontrols::RectF( 13, 8, windowRight-1, 10 ), "ui_slider_bg1", "ui_slider_handle", 0 );
        slider->signal_valueChange.connect(sigc::ptr_fun(&sliderChange) );
        controls->addControl( slider );

        controls->addControl( new UI_TextBox( "text",         touchcontrols::RectF( windownLeft, 10, 9.5, 12 ), "font_dual", 0, UI_TEXT_RIGHT, "Invert look:", textSize ));
        controls->addControl( new UI_Switch( "invert_switch", touchcontrols::RectF( 10, 10.3, 13, 11.7 ),"ui_switch_on", "ui_switch_off"  ));

        controls->addControl( new UI_TextBox( "text",         touchcontrols::RectF( 13, 10, windowRight-3.5, 12 ), "font_dual", 0, UI_TEXT_RIGHT, "Another:", textSize ));
        controls->addControl( new UI_Switch( "invert_switch", touchcontrols::RectF( windowRight-3, 10.3, windowRight, 11.7 ),"ui_switch_on", "ui_switch_off"  ));

        UI_Button *button =   new UI_Button( "reset_button",  touchcontrols::RectF( 13, 12, windowRight, 14 ),BUTTON_RESET, "font_dual", 0, UI_TEXT_CENTRE, "Reset", textSize, "ui_button_bg" );
        button->signal.connect(sigc::ptr_fun(&buttonPress) );
        controls->addControl(button);

        button =              new UI_Button( "add_remove",    touchcontrols::RectF( windownLeft, 12, 13, 14 ),BUTTON_ADDCTRL, "font_dual", 0, UI_TEXT_CENTRE, "Add/del controls", textSize, "ui_button_bg" );
        controls->addControl( button );


        // Draws backwards so need background last
        controls->addControl( new UI_Window( "bg_window", touchcontrols::RectF( windownLeft, 2, windowRight, 14.2 ), "ui_background" ));

        controls->setPassThroughTouch( false );

        controls->signal_button.connect(  sigc::ptr_fun(&buttonPress) );
    }

    return controls;
}


}