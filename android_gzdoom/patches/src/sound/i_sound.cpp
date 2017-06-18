*** /home/draje/Code/gzdoom/src/sound/i_sound.cpp	2017-06-18 07:23:17.316807352 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/sound/i_sound.cpp	2017-06-18 09:09:24.171730677 -0400
***************
*** 52,58 ****
--- 52,63 ----
  #include "doomtype.h"
  #include <math.h>
  
+ #ifdef __ANDROID__
+ #include "fmodsound_studio.h"
+ #else
  #include "fmodsound.h"
+ #endif
+ 
  
  #include "m_swap.h"
  #include "stats.h"
***************
*** 73,80 ****
--- 78,146 ----
  #include "doomdef.h"
  
  EXTERN_CVAR (Float, snd_sfxvolume)
+ 
+ #ifdef __ANDROID__
+ 
+ CVAR (Int, snd_buffersize, 1024, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
+ CVAR (Int, snd_buffercount, 4, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
+ 
+ 
+ static void setBufferFromSampleRate(int rate)
+ {
+ 	LOGI(("setBufferFromSampleRate"));
+ 	if (rate <= 22050)
+ 	{
+ 		snd_buffersize = 1024;
+ 		snd_buffercount = 4;
+ 	}
+ 	else if (rate <= 44100)
+ 	{
+ 		snd_buffersize = 1024;
+ 		snd_buffercount = 6;
+ 	}
+ 	else if(rate <= 48000)
+ 	{
+ 		snd_buffersize = 1024;
+ 		snd_buffercount = 8;
+ 	}
+ 	else //?
+ 	{
+ 		snd_buffersize = 1024;
+ 		snd_buffercount = 8;
+ 	}
+ }
+ 
+ extern int android_audio_rate;
+ static int sample_rate_last = 0;
+ CUSTOM_CVAR (Int, snd_samplerate, 0, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
+ {
+ 	int rate = self;
+ 	LOGI("samplerate 1 Custom %d (last = %d)", rate,sample_rate_last);
+ 
+ 	if (self == 0)
+ 	{
+ 		int rate = android_audio_rate;
+ 		setBufferFromSampleRate(rate);
+ 		sample_rate_last = self;
+ 		self = rate;
+ 	}
+ 	else if (sample_rate_last == 0)
+ 		sample_rate_last = self;
+ 
+ 
+ 	rate = self;
+ 	LOGI("samplerate 2 Custom %d (last = %d)", rate,sample_rate_last);
+ 
+ 	if (sample_rate_last != self)
+ 	{
+ 		setBufferFromSampleRate(self);
+ 		sample_rate_last = self;
+ 	}
+ }
+ #else
  CVAR (Int, snd_samplerate, 0, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  CVAR (Int, snd_buffersize, 0, CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
+ #endif
  CVAR (String, snd_output, "default", CVAR_ARCHIVE|CVAR_GLOBALCONFIG)
  
  // killough 2/21/98: optionally use varying pitched sounds
