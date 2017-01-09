/*==============================================================================
Event Parameter Example
Copyright (c), Firelight Technologies Pty, Ltd 2012-2015.

This example demonstrates how to control event playback using game parameters.
==============================================================================*/
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "common.h"

int FMOD_Main()
{
    void *extraDriverData = NULL;
    Common_Init(&extraDriverData);

    FMOD::Studio::System* system = NULL;
    ERRCHECK( FMOD::Studio::System::create(&system) );
    ERRCHECK( system->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData) );

    FMOD::Studio::Bank* masterBank = NULL;
    ERRCHECK( system->loadBankFile(Common_MediaPath("Master Bank.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );

    FMOD::Studio::Bank* stringsBank = NULL;
    ERRCHECK( system->loadBankFile(Common_MediaPath("Master Bank.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );

    FMOD::Studio::Bank* ambienceBank = NULL;
    ERRCHECK( system->loadBankFile(Common_MediaPath("Character.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &ambienceBank) );

    FMOD::Studio::EventDescription* eventDescription = NULL;
    ERRCHECK( system->getEvent("event:/Character/Footsteps/Footsteps", &eventDescription) );

    FMOD::Studio::EventInstance* eventInstance = NULL;
    ERRCHECK( eventDescription->createInstance(&eventInstance) );

    FMOD::Studio::ParameterInstance* surfaceParameter = NULL;
    ERRCHECK( eventInstance->getParameter("Surface", &surfaceParameter) );

    // Make the event audible to start with
    ERRCHECK( surfaceParameter->setValue(1.0f) );

    float surfaceParameterValue = 0;
    ERRCHECK( surfaceParameter->getValue(&surfaceParameterValue) );
    
    ERRCHECK( eventInstance->start() );

    do
    {
        Common_Update();

        if (Common_BtnPress(BTN_ACTION1))
        {
            surfaceParameterValue -= 1.0f;
            ERRCHECK( surfaceParameter->setValue(surfaceParameterValue) );
            ERRCHECK( surfaceParameter->getValue(&surfaceParameterValue) );
        }

        if (Common_BtnPress(BTN_ACTION2))
        {
            surfaceParameterValue += 1.0f;
            ERRCHECK( surfaceParameter->setValue(surfaceParameterValue) );
            ERRCHECK( surfaceParameter->getValue(&surfaceParameterValue) );
        }

        ERRCHECK( system->update() );

        Common_Draw("==================================================");
        Common_Draw("Event Parameter Example.");
        Common_Draw("Copyright (c) Firelight Technologies 2015-2015.");
        Common_Draw("==================================================");
        Common_Draw("");
        Common_Draw("Surface Parameter = %1.1f", surfaceParameterValue);
        Common_Draw("");
        Common_Draw("Surface Parameter:");
        Common_Draw("Press %s to decrease value", Common_BtnStr(BTN_ACTION1));
        Common_Draw("Press %s to increase value", Common_BtnStr(BTN_ACTION2));
        Common_Draw("");
        Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));

        Common_Sleep(50);
    } while (!Common_BtnPress(BTN_QUIT));

    ERRCHECK( system->release() );

    Common_Close();

    return 0;
}
