
LOCAL_PATH := $(call my-dir)/game-music-emu


include $(CLEAR_VARS)


LOCAL_MODULE    := gme_dev

LOCAL_CFLAGS :=   -O2  

LOCAL_LDLIBS += -llog

LOCAL_CFLAGS += -march=armv7-a -mfloat-abi=softfp
LOCAL_LDLIBS += -Wl,--fix-cortex-a8

LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/gme/



LOCAL_SRC_FILES =  	\
   gme/Blip_Buffer.cpp \ \
    gme/Classic_Emu.cpp \
    gme/Data_Reader.cpp \
    gme/Dual_Resampler.cpp \
    gme/Effects_Buffer.cpp \
    gme/Fir_Resampler.cpp \
    gme/gme.cpp \
    gme/Gme_File.cpp \
    gme/M3u_Playlist.cpp \
    gme/Multi_Buffer.cpp \
    gme/Music_Emu.cpp \
    gme/Ay_Apu.cpp \
    gme/Ay_Cpu.cpp \
    gme/Ay_Emu.cpp \
    gme/Gb_Apu.cpp \
    gme/Gb_Cpu.cpp \
    gme/Gb_Oscs.cpp \
    gme/Gbs_Emu.cpp \
    gme/Gym_Emu.cpp \
    gme/Hes_Apu.cpp \
    gme/Hes_Cpu.cpp \
    gme/Hes_Emu.cpp \
    gme/Kss_Cpu.cpp \
    gme/Kss_Emu.cpp \
    gme/Kss_Scc_Apu.cpp \
    gme/Nes_Apu.cpp \
    gme/Nes_Cpu.cpp \
    gme/Nes_Fme7_Apu.cpp \
    gme/Nes_Namco_Apu.cpp \
    gme/Nes_Oscs.cpp \
    gme/Nes_Vrc6_Apu.cpp \
    gme/Nsf_Emu.cpp \
    gme/Nsfe_Emu.cpp \
    gme/Sap_Apu.cpp \
    gme/Sap_Cpu.cpp \
    gme/Sap_Emu.cpp \
    gme/Sms_Apu.cpp \
    gme/Snes_Spc.cpp \
    gme/Spc_Cpu.cpp \
    gme/Spc_Dsp.cpp \
    gme/Spc_Emu.cpp \
      gme/Spc_Filter.cpp \
    gme/Vgm_Emu.cpp \
    gme/Vgm_Emu_Impl.cpp \
    gme/Ym2413_Emu.cpp \
    gme/Ym2612_Emu.cpp \

#strtof.c \

LOCAL_LDLIBS :=  -ldl -llog

include $(BUILD_STATIC_LIBRARY)








