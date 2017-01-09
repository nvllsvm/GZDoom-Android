/*==============================================================================
Load Banks Example
Copyright (c), Firelight Technologies Pty, Ltd 2012-2015.

This example demonstrates loading banks and then loading sample data.
It also hooks into the error callback to visually list any error codes that
are generated from bank loading.

It tries to load the following banks:

* Master Bank
* Weapons Bank
* "nope" Bank, which does not exist.  Opening this bank will fail and log errors.

The sample loading of banks can be controlled independently of the bank loading
itself.  Sample loading is asynchronous and the sample loading state can be 
polled to check whether the loading has finished.

A define can be enabled to demonstrate extra errors due to bad banks:

    #define INCLUDE_BAD_BANKS

### See Also ###
* Studio::System::loadBankFile
* Studio::Bank::loadSampleData
* Studio::Bank::getSampleLoadingState
* System::setCallback
==============================================================================*/
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "common.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <stdarg.h>

// Can include more failures
//#define INCLUDE_BAD_BANKS

static const char* BANK_NAMES[] =
{
    "Master Bank.bank",
    "Weapons.bank",
    "nope.bank",            // Deliberate missing bank
#ifdef INCLUDE_BAD_BANKS
    "Weapons.bank",         // Deliberate duplicate bank
    "**%$%^.bank",          // Deliberate badly formed filename
#endif
};

static const int BANK_COUNT = sizeof(BANK_NAMES)/sizeof(BANK_NAMES[0]);

const char* getLoadingStateString(FMOD_STUDIO_LOADING_STATE state)
{
    switch (state)
    {
        case FMOD_STUDIO_LOADING_STATE_UNLOADING:
            return "unloading";
        case FMOD_STUDIO_LOADING_STATE_UNLOADED:
            return "unloaded ";
        case FMOD_STUDIO_LOADING_STATE_LOADING:
            return "loading  ";
        case FMOD_STUDIO_LOADING_STATE_LOADED:
            return "loaded   ";
        default:
            return "???";
    };
}
const char* getHandleStateString(FMOD::Studio::Bank* bank)
{
    if (bank == NULL)
    {
        return "null   ";
    }
    else if (!bank->isValid())
    {
        return "invalid";
    }
    else
    {
        return "valid  ";
    }
}

struct RunData
{
    Common_Mutex criticalSection;
    FMOD::Studio::Bank* banks[BANK_COUNT];
    std::vector<std::string> errorStrings;
};

FMOD_RESULT F_CALLBACK SystemCallback(FMOD_SYSTEM *system, FMOD_SYSTEM_CALLBACK_TYPE type, void *commanddata1, void *commanddata2, void* userdata)
{
    if (type == FMOD_SYSTEM_CALLBACK_ERROR)
    {
        FMOD_ERRORCALLBACK_INFO* errorInfo = (FMOD_ERRORCALLBACK_INFO*)commanddata1;
        RunData* data = (RunData*)userdata;
        Common_Mutex_Enter(&data->criticalSection);
        char buffer[512];
        Common_snprintf(buffer, 512, "%s(%s)[%d]", errorInfo->functionname, errorInfo->functionparams, errorInfo->result);
        buffer[511] = '\0';
        data->errorStrings.push_back(buffer);
        Common_Mutex_Leave(&data->criticalSection);
    }
    return FMOD_OK;
}

int FMOD_Main()
{
    void *extraDriverData = 0;
    Common_Init(&extraDriverData);

    RunData data;
    Common_Mutex_Create(&data.criticalSection);

    FMOD::Studio::System* system;
    ERRCHECK( FMOD::Studio::System::create(&system) );
    FMOD::System* lowLevelSystem;
    ERRCHECK( system->getLowLevelSystem(&lowLevelSystem) );
    ERRCHECK( lowLevelSystem->setCallback(SystemCallback) );
    ERRCHECK( lowLevelSystem->setUserData(&data) );

    ERRCHECK( system->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData) );

    for (int i=0; i<BANK_COUNT; ++i)
    {
        data.banks[i] = NULL;
    }

    bool wantSampleLoad = false;

    do
    {
        Common_Update();
        
        if (Common_BtnPress(BTN_ACTION1))
        {
            // Start loading all the banks
            for (int i=0; i<BANK_COUNT; ++i)
            {
                if (data.banks[i] == NULL || !data.banks[i]->isValid())
                {
                    FMOD_RESULT result = system->loadBankFile(Common_MediaPath(BANK_NAMES[i]), FMOD_STUDIO_LOAD_BANK_NONBLOCKING, &data.banks[i]);
                    if (result != FMOD_OK)
                    {
                    }
                }
            }
        }
        if (Common_BtnPress(BTN_ACTION2))
        {
            // Unload all banks
            for (int i=0; i<BANK_COUNT; ++i)
            {
                FMOD_RESULT result = data.banks[i]->unload();
                if (result != FMOD_OK)
                {
                }
            }
        }
        if (Common_BtnPress(BTN_MORE))
        {
            wantSampleLoad = !wantSampleLoad;
        }
        // Load bank sample data

        for (int i=0; i<BANK_COUNT; ++i)
        {
            FMOD_STUDIO_LOADING_STATE bankLoadState = FMOD_STUDIO_LOADING_STATE_UNLOADED;
            FMOD_STUDIO_LOADING_STATE sampleLoadState = FMOD_STUDIO_LOADING_STATE_UNLOADED;
            if (data.banks[i] && data.banks[i]->isValid())
            {
                data.banks[i]->getLoadingState(&bankLoadState);
                data.banks[i]->getSampleLoadingState(&sampleLoadState);
            }
            if (bankLoadState == FMOD_STUDIO_LOADING_STATE_LOADED)
            {
                if (wantSampleLoad && sampleLoadState == FMOD_STUDIO_LOADING_STATE_UNLOADED)
                {
                    ERRCHECK(data.banks[i]->loadSampleData());
                }
                else if (!wantSampleLoad && (sampleLoadState == FMOD_STUDIO_LOADING_STATE_LOADING || sampleLoadState == FMOD_STUDIO_LOADING_STATE_LOADED))
                {
                    ERRCHECK(data.banks[i]->unloadSampleData());
                }
            }
        }


        ERRCHECK( system->update() );

        Common_Draw("==================================================");
        Common_Draw("Bank Load Example.");
        Common_Draw("Copyright (c) Firelight Technologies 2015-2015.");
        Common_Draw("==================================================");
        Common_Draw("Name              Handle  Bank-State  Sample-State");

        for (int i=0; i<BANK_COUNT; ++i)
        {
            FMOD_STUDIO_LOADING_STATE bankLoadState = FMOD_STUDIO_LOADING_STATE_UNLOADED;
            FMOD_STUDIO_LOADING_STATE sampleLoadState = FMOD_STUDIO_LOADING_STATE_UNLOADED;
            if (data.banks[i] && data.banks[i]->isValid())
            {
                data.banks[i]->getLoadingState(&bankLoadState);
                data.banks[i]->getSampleLoadingState(&sampleLoadState);
            }
            char namePad[64];
            int bankNameLen = strlen(BANK_NAMES[i]);
            memset(namePad, ' ', 63);
            namePad[16] = '\0';
            strncpy(namePad, BANK_NAMES[i], bankNameLen);

            Common_Draw("%s  %s %s   %s",
                    namePad, getHandleStateString(data.banks[i]), getLoadingStateString(bankLoadState), getLoadingStateString(sampleLoadState));
        }
        Common_Draw("");
        Common_Draw("Press %s to load banks, %s to unload banks", Common_BtnStr(BTN_ACTION1), Common_BtnStr(BTN_ACTION2));
        Common_Draw("Press %s to toggle sample data: %s", Common_BtnStr(BTN_MORE), wantSampleLoad ? "loaded" : "unloaded");
        Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));

        {
            Common_Mutex_Enter(&data.criticalSection);
            int errorCount = (int)data.errorStrings.size();
            Common_Draw("Errors (%d):", errorCount);
			int startIndex = errorCount - 6;
			if (startIndex < 0) startIndex = 0;
            for (int i=startIndex; i<errorCount; ++i)
            {
                Common_Draw(" %s", data.errorStrings[i].c_str());
            }
            Common_Mutex_Leave(&data.criticalSection);
        }

        Common_Sleep(50);
    } while (!Common_BtnPress(BTN_QUIT));
    
    for (int i=0; i<BANK_COUNT; ++i)
    {
        if (data.banks[i] != NULL && data.banks[i]->isValid())
        {
            data.banks[i]->unload();
        }
    }

    ERRCHECK( system->release() );

    Common_Mutex_Destroy(&data.criticalSection);
    Common_Close();

    return 0;
}
