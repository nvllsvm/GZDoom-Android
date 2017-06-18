*** /home/draje/Code/gzdoom/src/sound/music_fluidsynth_mididevice.cpp	2017-06-18 07:23:17.316807352 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/sound/music_fluidsynth_mididevice.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 428,434 ****
--- 428,436 ----
  {
  	if (len > 1 && (data[0] == 0xF0 || data[0] == 0xF7))
  	{
+ #ifndef __ANDROID__
  		fluid_synth_sysex(FluidSynth, (const char *)data + 1, len - 1, NULL, NULL, NULL, 0);
+ #endif
  	}
  }
  
***************
*** 611,617 ****
--- 613,623 ----
  
  	CritSec.Enter();
  	int polyphony = fluid_synth_get_polyphony(FluidSynth);
+ #ifdef __ANDROID__
+ 	int voices = 1;
+ #else
  	int voices = fluid_synth_get_active_voice_count(FluidSynth);
+ #endif
  	double load = fluid_synth_get_cpu_load(FluidSynth);
  	char *chorus, *reverb;
  	int maxpoly;
