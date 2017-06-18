*** /home/draje/Code/gzdoom/src/sound/music_midistream.cpp	2017-06-18 07:52:50.706347079 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/sound/music_midistream.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 38,43 ****
--- 38,44 ----
  #include "templates.h"
  #include "doomdef.h"
  #include "m_swap.h"
+ #include "doomerrors.h"
  
  // MACROS ------------------------------------------------------------------
  
***************
*** 275,284 ****
  
  	case MDEV_GUS:
  		return new TimidityMIDIDevice;
! 
  	case MDEV_OPL:
! 		return new OPLMIDIDevice;
! 
  	case MDEV_TIMIDITY:
  		return new TimidityPPMIDIDevice;
  
--- 276,294 ----
  
  	case MDEV_GUS:
  		return new TimidityMIDIDevice;
! #ifndef ONLY_GPL
  	case MDEV_OPL:
! 		try
! 		{
! 			return new OPLMIDIDevice;
! 		}
! 		catch (CRecoverableError &err)
! 		{
! 			// The creation of an OPL MIDI device can abort with an error if no GENMIDI lump can be found.
! 			Printf("Unable to create OPL MIDI device: %s\nFalling back to FModEx playback", err.GetMessage());
! 			return new FMODMIDIDevice;
! 		}
! #endif
  	case MDEV_TIMIDITY:
  		return new TimidityPPMIDIDevice;
  
***************
*** 309,322 ****
  	devtype = SelectMIDIDevice(DeviceType);
  	if (DumpFilename.IsNotEmpty())
  	{
! 		if (devtype == MDEV_OPL)
  		{
! 			MIDI = new OPLDumperMIDIDevice(DumpFilename);
  		}
! 		else if (devtype == MDEV_GUS)
  		{
! 			MIDI = new TimidityWaveWriterMIDIDevice(DumpFilename, 0);
  		}
  	}
  	else
  	{
--- 319,335 ----
  	devtype = SelectMIDIDevice(DeviceType);
  	if (DumpFilename.IsNotEmpty())
  	{
! 
! 		if (devtype == MDEV_GUS)
  		{
! 			MIDI = new TimidityWaveWriterMIDIDevice(DumpFilename, 0);
  		}
! #ifndef ONLY_GPL
! 		else if (devtype == MDEV_OPL)
  		{
! 			MIDI = new OPLDumperMIDIDevice(DumpFilename);
  		}
+ #endif
  	}
  	else
  	{
