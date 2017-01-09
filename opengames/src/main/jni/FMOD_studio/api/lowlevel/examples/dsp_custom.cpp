/*==============================================================================
Custom DSP Example
Copyright (c), Firelight Technologies Pty, Ltd 2004-2015.

This example shows how to add a user created DSP callback to process audio 
data. The read callback is executed at runtime, and can be added anywhere in
the DSP network.
==============================================================================*/
#include "fmod.hpp"
#include "common.h"

FMOD_RESULT F_CALLBACK myDSPCallback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels) 
{
    FMOD_RESULT result;
    char name[256];
    unsigned int userdata;
    FMOD::DSP *thisdsp = (FMOD::DSP *)dsp_state->instance; 

    /* 
        This redundant call just shows using the instance parameter of FMOD_DSP_STATE to 
        call a DSP information function. 
    */
    result = thisdsp->getInfo(name, 0, 0, 0, 0);
    ERRCHECK(result);

    result = thisdsp->getUserData((void **)&userdata);
    ERRCHECK(result);

    /*
        This loop assumes inchannels = outchannels, which it will be if the DSP is created with '0' 
        as the number of channels in FMOD_DSP_DESCRIPTION.  
        Specifying an actual channel count will mean you have to take care of any number of channels coming in,
        but outputting the number of channels specified. Generally it is best to keep the channel 
        count at 0 for maximum compatibility.
    */
    for (unsigned int samp = 0; samp < length; samp++) 
    { 
        /*
            Feel free to unroll this.
        */
        for (int chan = 0; chan < *outchannels; chan++)
        {
            /* 
                This DSP filter just halves the volume! 
                Input is modified, and sent to output.
            */
            outbuffer[(samp * *outchannels) + chan] = inbuffer[(samp * inchannels) + chan] * 0.2f;
        }
    } 

    return FMOD_OK; 
} 


int FMOD_Main()
{
    FMOD::System       *system;
    FMOD::Sound        *sound;
    FMOD::Channel      *channel;
    FMOD::DSP          *mydsp;
    FMOD::ChannelGroup *mastergroup;
    FMOD_RESULT         result;
    unsigned int        version;
    void               *extradriverdata = 0;

    Common_Init(&extradriverdata);

    /*
        Create a System object and initialize.
    */
    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
    }

    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    ERRCHECK(result);

    result = system->createSound(Common_MediaPath("drumloop.wav"), FMOD_LOOP_NORMAL, 0, &sound);
    ERRCHECK(result);

    result = system->playSound(sound, 0, false, &channel);
    ERRCHECK(result);

    /*
        Create the DSP effect.
    */  
    { 
        FMOD_DSP_DESCRIPTION dspdesc; 
        memset(&dspdesc, 0, sizeof(dspdesc));
        
        strncpy(dspdesc.name, "My first DSP unit", sizeof(dspdesc.name));
        dspdesc.version = 0x00010000;
        dspdesc.numinputbuffers = 1;
        dspdesc.numoutputbuffers = 1;
        dspdesc.read = myDSPCallback; 
        dspdesc.userdata = (void *)0x12345678; 

        result = system->createDSP(&dspdesc, &mydsp); 
        ERRCHECK(result); 
    } 

    /*
        Attach the DSP, inactive by default.
    */
    result = mydsp->setBypass(true);
    ERRCHECK(result);

    result = system->getMasterChannelGroup(&mastergroup);
    ERRCHECK(result);

    result = mastergroup->addDSP(0, mydsp);
    ERRCHECK(result);

    /*
        Main loop.
    */
    do
    {
        bool bypass;

        Common_Update();

        result = mydsp->getBypass(&bypass);
        ERRCHECK(result);

        if (Common_BtnPress(BTN_ACTION1))
        {
            bypass = !bypass;
            
            result = mydsp->setBypass(bypass);
            ERRCHECK(result);
        }

        result = system->update();
        ERRCHECK(result);

        Common_Draw("==================================================");
        Common_Draw("Custom DSP Example.");
        Common_Draw("Copyright (c) Firelight Technologies 2004-2015.");
        Common_Draw("==================================================");
        Common_Draw("");
        Common_Draw("Press %s to toggle filter bypass", Common_BtnStr(BTN_ACTION1));
        Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
        Common_Draw("");
        Common_Draw("Filter is %s", bypass ? "inactive" : "active");

        Common_Sleep(50);
    } while (!Common_BtnPress(BTN_QUIT));

    /*
        Shut down
    */
    result = sound->release();
    ERRCHECK(result);

    result = mastergroup->removeDSP(mydsp);
    ERRCHECK(result);
    result = mydsp->release();
    ERRCHECK(result);

    result = system->close();
    ERRCHECK(result);
    result = system->release();
    ERRCHECK(result);

    Common_Close();

    return 0;
}
