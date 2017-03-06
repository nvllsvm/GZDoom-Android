#include "UI_TextBox.h"
#include "TouchControlsConfig.h"


using namespace touchcontrols;

UI_TextBox::UI_TextBox( std::string tag, RectF pos, std::string font_filename, int fontSet, uint32_t params, std::string text, float textSize ): ControlSuper( TC_TYPE_UI_TEXTBOX, tag, pos )
{
    image = font_filename;
    this->textSize = textSize;
    this->text = text;

    glTex = 0;
    this->fontSet = fontSet;
    this->params = params;

    fontInfoVec.resize(256);

    updateSize();
}

void UI_TextBox::updateSize()
{
    float height = textSize;
    float width = height * ( -GLScaleHeight / GLScaleWidth );

    glRect.resize( width, height );

    charSpacing = glRect.height / 30.f;
}



bool UI_TextBox::processPointer( int action, int pid, float x, float y )
{
    return false;
}

void UI_TextBox::resetOutput()
{

}

bool UI_TextBox::initGL()
{
    int x, y;
    glTex = loadTextureFromPNG( image, x, y, &fontInfoVec );

    return false;
}


#define CHAR_TO_GLYPH( C, F ) ((C - 32) + 128 * F)
float UI_TextBox::getCharWidth( unsigned char c )
{
    if( c == ' ' )
    {
        return glRect.width / 4;
    }
    else
    {
        float leftPad = fontInfoVec[CHAR_TO_GLYPH( c, fontSet )].leftGap;
        float rightPad = fontInfoVec[CHAR_TO_GLYPH( c, fontSet )].rightGap;

        return  glRect.width * (1 - (leftPad + rightPad) );
    }
}

#define CHAR_TO_GLYPH( C, F ) ((C - 32) + 128 * F)
float UI_TextBox::getTotalWidth()
{
    float ret = 0;

    const char *textC = text.c_str();
    uint32_t pos = 0;

    while( textC[pos] )
    {
        ret += getCharWidth( textC[pos] ) + charSpacing;
        pos++;
    }

    return ret;
}

bool UI_TextBox::drawGL( bool forEditor )
{
    const char *textC = text.c_str();

    uint32_t pos = 0;

    float x = controlPos.left;

    if( params & UI_TEXT_CENTRE)
    {
        x = controlPos.left  + ( controlPos.width() - getTotalWidth() ) / 2;
    }
    else if( params & UI_TEXT_RIGHT)
    {
        x = controlPos.left  + ( controlPos.width() - getTotalWidth() );
    }

    float y =  controlPos.top + ( controlPos.height() - glRect.height ) / 2;

    while( textC[pos] )
    {
        char c = textC[pos];
        char g = CHAR_TO_GLYPH( c, fontSet );

        int32_t tx = g % 16;
        int32_t ty = ( g >> 4 );
        float fx = ( float )tx / 16.f;
        float fy = -( float )ty / 16.f;

        // Find how must space needs to be removed from the sides of the character
        float leftPad = fontInfoVec[g].leftGap;
        float rightPad = fontInfoVec[g].rightGap;

        // Create a new rect because the width needs to change
        GLRect glRectTemp;

        glRectTemp.texture[0] = fx + (leftPad  / 16.f);
        glRectTemp.texture[1] = fy - 0.0625f;
        glRectTemp.texture[2] = fx + (leftPad / 16.f);
        glRectTemp.texture[3] = fy;
        glRectTemp.texture[4] = fx + 0.0625f - (rightPad / 16.f);
        glRectTemp.texture[5] = fy - 0.0625f;
        glRectTemp.texture[6] = fx + 0.0625f - (rightPad / 16.f);
        glRectTemp.texture[7] = fy;


        glRectTemp.resize( getCharWidth ( c ), glRect.height);

        drawRect( glTex, x, y, glRectTemp );

        x += glRectTemp.width + charSpacing;

        pos++;
    }

    return false;
}

void UI_TextBox::saveXML( TiXmlDocument &doc )
{

}

void UI_TextBox::loadXML( TiXmlDocument &doc )
{

}
