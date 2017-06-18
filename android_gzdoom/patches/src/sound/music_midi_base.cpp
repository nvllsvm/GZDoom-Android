*** /home/draje/Code/gzdoom/src/sound/music_midi_base.cpp	2017-06-18 07:23:17.316807352 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/sound/music_midi_base.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 29,41 ****
  #endif
  	pair[p].Text = "GUS";
  	pair[p].Value = -4.0;
  	pair[p+1].Text = "OPL Synth Emulation";
  	pair[p+1].Value = -3.0;
  	pair[p+2].Text = "TiMidity++";
  	pair[p+2].Value = -2.0;
  	pair[p+3].Text = "FMOD";
  	pair[p+3].Value = -1.0;
! 
  }
  
  static void MIDIDeviceChanged(int newdev)
--- 29,42 ----
  #endif
  	pair[p].Text = "GUS";
  	pair[p].Value = -4.0;
+ #ifndef __ANDROID__
  	pair[p+1].Text = "OPL Synth Emulation";
  	pair[p+1].Value = -3.0;
  	pair[p+2].Text = "TiMidity++";
  	pair[p+2].Value = -2.0;
  	pair[p+3].Text = "FMOD";
  	pair[p+3].Value = -1.0;
! #endif
  }
  
  static void MIDIDeviceChanged(int newdev)
***************
*** 194,199 ****
--- 195,211 ----
  
  // Everything but Windows uses this code.
  
+ #ifdef __ANDROID__ //Only GUS and fluidsynth please
+ CUSTOM_CVAR(Int, snd_mididevice, -5, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
+ {
+ 	if (self < -5)
+ 		self = -5;
+ 	else if (self > -4)
+ 		self = -4;
+ 	else
+ 		MIDIDeviceChanged(self);
+ }
+ #else
  CUSTOM_CVAR(Int, snd_mididevice, -1, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  {
  	if (self < -5)
***************
*** 203,208 ****
--- 215,221 ----
  	else
  		MIDIDeviceChanged(self);
  }
+ #endif
  
  void I_BuildMIDIMenuList (FOptionValues *opt)
  {
