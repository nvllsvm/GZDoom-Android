//
//  TouchGui.cpp
//  touchcontrols
//
//  Created by Emile Belanger on 02/01/2016.
//  Copyright © 2016 Beloko games. All rights reserved.
//

#include "TouchGui.h"
#include <TouchControls.h>
#include <Input.h>
#include <Shell.h>


TouchGui::TouchGui(const char * root_path,int width,int height)
{

    LOGTOUCH("TouchGui::TouchGui path = %s %dx%d\n",root_path,width,height);
    this->width = width;
    this->height = height;
    
    file_interface = new ShellFileInterface(root_path);
    
    Rocket::Core::SetFileInterface( file_interface);

    opengl_renderer = new ShellRenderInterfaceOpenGL();
    system_interface = new ShellSystemInterface();
    
    Rocket::Core::SetRenderInterface(opengl_renderer);
  
    Rocket::Core::SetSystemInterface(system_interface);
    
    Rocket::Core::Initialise();
    
    // Create the main Rocket context and set it on the shell's input layer.
    context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(WIDTH, HEIGHT));
    if (context == NULL)
    {
        LOGTOUCH("Error creating context\n");
        Rocket::Core::Shutdown();
        return;
    }
    
    //Rocket::Debugger::Initialise(context);
    Input::SetContext(context);
    
  
    Rocket::Controls::Initialise();

    Rocket::Core::FreeType::FontProvider::Initialise();
    Shell::LoadFonts("gui_assets/");
    
    // Load and show the demo document.
    Rocket::Core::ElementDocument* document = context->LoadDocument("gui_assets/demo.rml");
    if (document != NULL)
    {
        document->Show();
        document->RemoveReference();
    }
    

    
}
    
void TouchGui::update()
{
   // LOGTOUCH("gui update\n");
    
    glClearColor(0, 0, 0, 1);
    glColor4f(1, 1, 1, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof(0, WIDTH, HEIGHT, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    context->Update();
    context->Render();
}

void TouchGui::processMouseMove(int x, int y)
{
    context->ProcessMouseMove(x, y, 0);
}

void TouchGui::processMouseButton(int state, int button)
{
    if (state)
        context->ProcessMouseButtonDown(button, 0);
    else
        context->ProcessMouseButtonUp(button, 0);
}

void TouchGui::processPointer(int action, int pid, float x, float y)
{
    if (action == P_DOWN)
    {
        processMouseMove(x * WIDTH, y * HEIGHT);
        processMouseButton(1, pid);
    }
    else if (action == P_UP)
    {
        processMouseMove(x * WIDTH, y * HEIGHT);
        processMouseButton(0, pid);
    }
    else if(action == P_MOVE)
    {
        processMouseMove(x * WIDTH, y * HEIGHT);
    }
    
}
