#include "UI_Button.h"
#include "TouchControlsConfig.h"


using namespace touchcontrols;

UI_Button::UI_Button ( std::string tag, RectF pos, uint32_t uid, std::string font_filename, int fontSet, uint32_t params, std::string text, float textSize, std::string bg_image ) :
    UI_TextBox ( tag, pos, font_filename, fontSet, params, text, textSize )
{
    touchId = -1;
    this->bg_image = bg_image;
    this->uid = uid;
    glTex = 0;

    updateSize();
}

void UI_Button::updateSize()
{
    glRect.resize ( controlPos.right - controlPos.left, controlPos.bottom - controlPos.top );
}



bool UI_Button::processPointer ( int action, int pid, float x, float y )
{
    if ( action == P_DOWN )
    {
        if ( touchId == -1 ) //Only process if not active
        {
            if ( controlPos.contains ( x, y ) )
            {
                touchId = pid;

                return true;
            }
        }
        return false;
    }
    else if ( action == P_UP )
    {
        if ( pid == touchId )
        {
            signal.emit(1 , uid );
            touchId = -1;
            return true;
        }
        return false;
    }
    else if ( action == P_MOVE )
    {
        //If the finger moves out of the button cancel
        if ( !controlPos.contains ( x, y ) )
        {
            touchId = -1;
        }
        return false;
    }

    return false;
}

void UI_Button::resetOutput()
{
    touchId = -1;
}

bool UI_Button::initGL()
{
    int x, y;
    glTex = loadTextureFromPNG ( bg_image, x, y );

    UI_TextBox::initGL();

    return false;
}



bool UI_Button::drawGL ( bool forEditor )
{
    drawRect ( glTex, controlPos.left, controlPos.top, glRect );

    UI_TextBox::drawGL ( forEditor );

    if ( touchId != -1 ) // Finger down, change colour
    {
        drawRect ( 0.9, 1, 0, 0.2, controlPos.left, controlPos.top, glRect );
    }

    return false;
}

void UI_Button::saveXML ( TiXmlDocument &doc )
{
    TiXmlElement * root = new TiXmlElement ( tag.c_str() );
    doc.LinkEndChild ( root );

    ControlSuper::saveXML ( *root );
}

void UI_Button::loadXML ( TiXmlDocument &doc )
{
    TiXmlHandle hDoc ( &doc );
    TiXmlElement* pElem = hDoc.FirstChild ( tag ).Element();

    if ( !pElem ) //Check exists, if not just leave as default
    {
        return;
    }

    ControlSuper::loadXML ( *pElem );
}
