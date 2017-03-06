//
//  TouchGui.hpp
//  touchcontrols
//
//  Created by Emile Belanger on 02/01/2016.
//  Copyright © 2016 Beloko games. All rights reserved.
//

#ifndef TouchGui_hpp
#define TouchGui_hpp

#include <Rocket/Core.h>
#include <Rocket/Core/SystemInterface.h>
#include <Rocket/Core/FreeType/FontProvider.h>

#include <Rocket/Core/FontDatabase.h>

#include <Rocket/Debugger.h>
#include <Rocket/Controls.h>

#include "ShellSystemInterface.h"
#include "ShellRenderInterfaceOpenGL.h"
#include "ShellFileInterface.h"
#include "ShellOpenGL.h"

class TouchGui
{
public:
    static const int WIDTH = 1024;
    static const int HEIGHT = 768;
    
    TouchGui(const char * root_path,int width,int height);
    
    void update();
    
    void processMouseMove(int x, int y);
    void processMouseButton(int state, int button);
    
    void processPointer(int action, int pid, float x, float y);
private:
    int width,height;
    ShellRenderInterfaceOpenGL * opengl_renderer;
   
    ShellSystemInterface * system_interface;
    Rocket::Core::Context * context;
    ShellFileInterface * file_interface;

};

#endif /* TouchGui_hpp */
