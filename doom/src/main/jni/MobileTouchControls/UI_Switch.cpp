#include "UI_Switch.h"
#include "TouchControlsConfig.h"


using namespace touchcontrols;

UI_Switch::UI_Switch ( std::string tag, RectF pos, std::string on_image, std::string off_image ) : ControlSuper ( TC_TYPE_UI_SWITCH, tag, pos )
{
    this->on_image = on_image;
    this->off_image = off_image;

    glTexOn = glTexOff = 0;

    isOn = false;

    updateSize();
}

void UI_Switch::updateSize()
{
    glRect.resize ( controlPos.right - controlPos.left, controlPos.bottom - controlPos.top );
}



bool UI_Switch::processPointer ( int action, int pid, float x, float y )
{
 if ( action == P_DOWN )
    {
        if ( controlPos.contains ( x, y ) )
        {
            // Toggle switch
            isOn = !isOn;

            return true;
        }
    }
    else if ( action == P_UP )
    {

    }
    else if ( action == P_MOVE )
    {

    }

    return false;
}

void UI_Switch::resetOutput()
{

}

bool UI_Switch::initGL()
{
    int x, y;
    glTexOn = loadTextureFromPNG ( on_image, x, y );
    glTexOff = loadTextureFromPNG ( off_image, x, y );


    return false;
}


bool UI_Switch::drawGL ( bool forEditor )
{
    //setScissor();
    if( isOn )
        drawRect ( glTexOn, controlPos.left, controlPos.top, glRect );
    else
        drawRect ( glTexOff, controlPos.left, controlPos.top, glRect );

    return false;
}

void UI_Switch::saveXML ( TiXmlDocument &doc )
{

}

void UI_Switch::loadXML ( TiXmlDocument &doc )
{

}
