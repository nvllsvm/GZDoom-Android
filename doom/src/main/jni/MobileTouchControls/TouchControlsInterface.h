//
//  TouchControlsInterface.h
//  Postal
//
//  Created by Emile on 01/04/2015.
//
//

#ifndef Postal_TouchControlsInterface_h
#define Postal_TouchControlsInterface_h

#include "TouchControlsConfig.h"

class TouchControlsInterface{
public:
    virtual bool processPointer(int action, int pid, float x, float y) = 0;
};

#endif
