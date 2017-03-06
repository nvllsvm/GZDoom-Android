#include "UI_Window.h"
#include "TouchControlsConfig.h"


using namespace touchcontrols;

UI_Window::UI_Window ( std::string tag, RectF pos, std::string image_filename ) : ControlSuper ( TC_TYPE_UI_WINDOW, tag, pos )
{
    image = image_filename;
    glTex = 0;

    updateSize();
}

void UI_Window::updateSize()
{
    glRect.resize ( controlPos.right - controlPos.left, controlPos.bottom - controlPos.top );
}



bool UI_Window::processPointer ( int action, int pid, float x, float y )
{
    return false;
}

void UI_Window::resetOutput()
{

}

bool UI_Window::initGL()
{
    int x, y;
    glTex = loadTextureFromPNG ( image, x, y );

    return false;
}

void UI_Window::setScissor ( void )
{
    glScissor ( controlPos.left * GLScaleWidth, controlPos.top * -GLScaleHeight,
                controlPos.width() * GLScaleWidth, controlPos.height() *  -GLScaleHeight );
    glEnable ( GL_SCISSOR_TEST );
}

bool UI_Window::drawGL ( bool forEditor )
{
    //setScissor();

    drawRect ( glTex, controlPos.left, controlPos.top, glRect );

    return false;
}

void UI_Window::saveXML ( TiXmlDocument &doc )
{

}

void UI_Window::loadXML ( TiXmlDocument &doc )
{

}
