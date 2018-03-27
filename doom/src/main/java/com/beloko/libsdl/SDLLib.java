package com.beloko.libsdl;

import android.graphics.PixelFormat;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;


/**
 * SDL Activity
 */
public class SDLLib {

    private static final Object threadLock = new Object();
    public static boolean resumed = false;
    // Audio
    private static Thread mAudioThread;
    private static AudioTrack mAudioTrack;
    // EGL private objects
    private static EGLContext mEGLContext;
    private static EGLSurface mEGLSurface;
    private static EGLDisplay mEGLDisplay;
    private static EGLConfig mEGLConfig;
    private static int mGLMajor, mGLMinor;

    public static void loadSDL() {

        try {
            Log.i("JNI", "Trying to load SDL.so");

            System.loadLibrary("SDL");
            System.loadLibrary("SDL_mixer");
            System.loadLibrary("SDL_image");
        } catch (UnsatisfiedLinkError ule) {
            Log.e("JNI", "WARNING: Could not load SDL.so: " + ule.toString());
        }
    }

    // C functions we call
    public static native void nativeInit(boolean launch);

    public static native void nativeQuit();

    public static native void nativePause();

    public static native void nativeResume();

    public static native void onNativeResize(int x, int y, int format);

    public static native void onNativeKeyDown(int keycode);

    public static native void onNativeKeyUp(int keycode);

    public static native void onNativeTouch(int touchDevId, int pointerFingerId,
                                            int action, float x,
                                            float y, float p);

    public static native void onNativeAccel(float x, float y, float z);


    // Java functions called from C

    public static native void nativeRunAudioThread();

    public static boolean createGLContext(int majorVersion, int minorVersion) {
        return true;
    }

    public static void flipBuffers() {

    }


    //    // EGL functions
    //    public static boolean initEGL(int majorVersion, int minorVersion) {
    //        if (SDLLib.mEGLDisplay == null) {
    //            //Log.v("SDL", "Starting up OpenGL ES " + majorVersion + "." + minorVersion);
    //
    //            try {
    //                EGL10 egl = (EGL10)EGLContext.getEGL();
    //
    //                EGLDisplay dpy = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
    //
    //                int[] version = new int[2];
    //                egl.eglInitialize(dpy, version);
    //
    //                int EGL_OPENGL_ES_BIT = 1;
    //                int EGL_OPENGL_ES2_BIT = 4;
    //                int renderableType = 0;
    //                if (majorVersion == 2) {
    //                    renderableType = EGL_OPENGL_ES2_BIT;
    //                } else if (majorVersion == 1) {
    //                    renderableType = EGL_OPENGL_ES_BIT;
    //                }
    //                int[] configSpec = {
    //                    //EGL10.EGL_DEPTH_SIZE,   16,
    //                    EGL10.EGL_RENDERABLE_TYPE, renderableType,
    //                    EGL10.EGL_NONE
    //                };
    //                EGLConfig[] configs = new EGLConfig[1];
    //                int[] num_config = new int[1];
    //                if (!egl.eglChooseConfig(dpy, configSpec, configs, 1, num_config) || num_config[0] == 0) {
    //                    Log.e("SDL", "No EGL config available");
    //                    return false;
    //                }
    //                EGLConfig config = configs[0];
    //
    //                /*int EGL_CONTEXT_CLIENT_VERSION=0x3098;
    //                int contextAttrs[] = new int[] { EGL_CONTEXT_CLIENT_VERSION, majorVersion, EGL10.EGL_NONE };
    //                EGLContext ctx = egl.eglCreateContext(dpy, config, EGL10.EGL_NO_CONTEXT, contextAttrs);
    //
    //                if (ctx == EGL10.EGL_NO_CONTEXT) {
    //                    Log.e("SDL", "Couldn't create context");
    //                    return false;
    //                }
    //                SDLActivity.mEGLContext = ctx;*/
    //                SDLLib.mEGLDisplay = dpy;
    //                SDLLib.mEGLConfig = config;
    //                SDLLib.mGLMajor = majorVersion;
    //                SDLLib.mGLMinor = minorVersion;
    //
    //                SDLLib.createEGLSurface();
    //            } catch(Exception e) {
    //                Log.v("SDL", e + "");
    //                for (StackTraceElement s : e.getStackTrace()) {
    //                    Log.v("SDL", s.toString());
    //                }
    //            }
    //        }
    //        else SDLLib.createEGLSurface();
    //
    //        return true;
    //    }
    //
    //    public static boolean createEGLContext() {
    //        EGL10 egl = (EGL10)EGLContext.getEGL();
    //        int EGL_CONTEXT_CLIENT_VERSION=0x3098;
    //        int contextAttrs[] = new int[] { EGL_CONTEXT_CLIENT_VERSION, SDLLib.mGLMajor, EGL10.EGL_NONE };
    //        SDLLib.mEGLContext = egl.eglCreateContext(SDLLib.mEGLDisplay, SDLLib.mEGLConfig, EGL10.EGL_NO_CONTEXT, contextAttrs);
    //        if (SDLLib.mEGLContext == EGL10.EGL_NO_CONTEXT) {
    //            Log.e("SDL", "Couldn't create context");
    //            return false;
    //        }
    //        return true;
    //    }
    //
    //    public static boolean createEGLSurface() {
    //        if (SDLLib.mEGLDisplay != null && SDLLib.mEGLConfig != null) {
    //            EGL10 egl = (EGL10)EGLContext.getEGL();
    //            if (SDLLib.mEGLContext == null) createEGLContext();
    //
    //            Log.v("SDL", "Creating new EGL Surface");
    //            EGLSurface surface = egl.eglCreateWindowSurface(SDLLib.mEGLDisplay, SDLLib.mEGLConfig, SDLLib.mSurface, null);
    //            if (surface == EGL10.EGL_NO_SURFACE) {
    //                Log.e("SDL", "Couldn't create surface");
    //                return false;
    //            }
    //
    //            if (!egl.eglMakeCurrent(SDLLib.mEGLDisplay, surface, surface, SDLLib.mEGLContext)) {
    //                Log.e("SDL", "Old EGL Context doesnt work, trying with a new one");
    //                createEGLContext();
    //                if (!egl.eglMakeCurrent(SDLLib.mEGLDisplay, surface, surface, SDLLib.mEGLContext)) {
    //                    Log.e("SDL", "Failed making EGL Context current");
    //                    return false;
    //                }
    //            }
    //            SDLLib.mEGLSurface = surface;
    //            return true;
    //        }
    //        return false;
    //    }
    //
    //    // EGL buffer flip
    //    public static void flipEGL() {
    //        try {
    //            EGL10 egl = (EGL10)EGLContext.getEGL();
    //
    //            egl.eglWaitNative(EGL10.EGL_CORE_NATIVE_ENGINE, null);
    //
    //            // drawing here
    //
    //            egl.eglWaitGL();
    //
    //            egl.eglSwapBuffers(SDLLib.mEGLDisplay, SDLLib.mEGLSurface);
    //
    //
    //        } catch(Exception e) {
    //            Log.v("SDL", "flipEGL(): " + e);
    //            for (StackTraceElement s : e.getStackTrace()) {
    //                Log.v("SDL", s.toString());
    //            }
    //        }
    //    }

    public static void setActivityTitle(String title) {

    }

    // Called when the surface is resized
    public static void surfaceChanged(int format, int width, int height) {
        Log.v("SDL", "surfaceChanged()");

        int sdlFormat = 0x85151002; // SDL_PIXELFORMAT_RGB565 by default
        switch (format) {
            case PixelFormat.A_8:
                Log.v("SDL", "pixel format A_8");
                break;
            case PixelFormat.LA_88:
                Log.v("SDL", "pixel format LA_88");
                break;
            case PixelFormat.L_8:
                Log.v("SDL", "pixel format L_8");
                break;
            case PixelFormat.RGBA_4444:
                Log.v("SDL", "pixel format RGBA_4444");
                sdlFormat = 0x85421002; // SDL_PIXELFORMAT_RGBA4444
                break;
            case PixelFormat.RGBA_5551:
                Log.v("SDL", "pixel format RGBA_5551");
                sdlFormat = 0x85441002; // SDL_PIXELFORMAT_RGBA5551
                break;
            case PixelFormat.RGBA_8888:
                Log.v("SDL", "pixel format RGBA_8888");
                sdlFormat = 0x86462004; // SDL_PIXELFORMAT_RGBA8888
                break;
            case PixelFormat.RGBX_8888:
                Log.v("SDL", "pixel format RGBX_8888");
                sdlFormat = 0x86262004; // SDL_PIXELFORMAT_RGBX8888
                break;
            case PixelFormat.RGB_332:
                Log.v("SDL", "pixel format RGB_332");
                sdlFormat = 0x84110801; // SDL_PIXELFORMAT_RGB332
                break;
            case PixelFormat.RGB_565:
                Log.v("SDL", "pixel format RGB_565");
                sdlFormat = 0x85151002; // SDL_PIXELFORMAT_RGB565
                break;
            case PixelFormat.RGB_888:
                Log.v("SDL", "pixel format RGB_888");
                // Not sure this is right, maybe SDL_PIXELFORMAT_RGB24 instead?
                sdlFormat = 0x86161804; // SDL_PIXELFORMAT_RGB888
                break;
            default:
                Log.v("SDL", "pixel format unknown " + format);
                break;
        }
        SDLLib.onNativeResize(width, height, sdlFormat);
        Log.v("SDL", "Window size:" + width + "x" + height);
    }

    // Audio
    public static void onPause() {
        resumed = false;
    }

    public static void onResume() {
        resumed = true;
        synchronized (threadLock) {
            threadLock.notifyAll();
        }
    }

    public static Object audioInit(int sampleRate, boolean is16Bit, boolean isStereo, int desiredFrames) {
        int channelConfig = isStereo ? AudioFormat.CHANNEL_CONFIGURATION_STEREO : AudioFormat.CHANNEL_CONFIGURATION_MONO;
        int audioFormat = is16Bit ? AudioFormat.ENCODING_PCM_16BIT : AudioFormat.ENCODING_PCM_8BIT;
        int frameSize = (isStereo ? 2 : 1) * (is16Bit ? 2 : 1);

        Log.v("SDL", "SDL audio: wanted " + (isStereo ? "stereo" : "mono") + " " + (is16Bit ? "16-bit" : "8-bit") + " " + ((float) sampleRate / 1000f) + "kHz, " + desiredFrames + " frames buffer");

        // Let the user pick a larger buffer if they really want -- but ye
        // gods they probably shouldn't, the minimums are horrifyingly high
        // latency already
        desiredFrames = Math.max(desiredFrames, (AudioTrack.getMinBufferSize(sampleRate, channelConfig, audioFormat) + frameSize - 1) / frameSize);

        mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate,
                channelConfig, audioFormat, desiredFrames * frameSize, AudioTrack.MODE_STREAM);

        audioStartThread();

        Log.v("SDL", "SDL audio: got " + ((mAudioTrack.getChannelCount() >= 2) ? "stereo" : "mono") + " " + ((mAudioTrack.getAudioFormat() == AudioFormat.ENCODING_PCM_16BIT) ? "16-bit" : "8-bit") + " " + ((float) mAudioTrack.getSampleRate() / 1000f) + "kHz, " + desiredFrames + " frames buffer");

        Object buf;
        if (is16Bit) {
            buf = new short[desiredFrames * (isStereo ? 2 : 1)];
        } else {
            buf = new byte[desiredFrames * (isStereo ? 2 : 1)];
        }
        return buf;
    }

    public static void audioStartThread() {
        mAudioThread = new Thread(new Runnable() {
            public void run() {
                mAudioTrack.play();
                nativeRunAudioThread();
            }
        });

        // I'd take REALTIME if I could get it!
        mAudioThread.setPriority(Thread.MAX_PRIORITY);
        mAudioThread.start();
    }

    public static void audioWriteShortBuffer(short[] buffer) {
        if (!resumed)
            synchronized (threadLock) {
                try {
                    if (mAudioTrack != null)
                        mAudioTrack.pause();
                    threadLock.wait();
                    if (mAudioTrack != null)
                        mAudioTrack.play();
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }


        for (int i = 0; i < buffer.length; ) {
            int result = mAudioTrack.write(buffer, i, buffer.length - i);
            if (result > 0) {
                i += result;
            } else if (result == 0) {
                try {
                    Thread.sleep(1);
                } catch (InterruptedException e) {
                    // Nom nom
                }
            } else {
                Log.w("SDL", "SDL audio: error return from write(short)");
                return;
            }
        }
    }

    public static void audioWriteByteBuffer(byte[] buffer) {
        if (!resumed)
            synchronized (threadLock) {
                try {
                    if (mAudioTrack != null)
                        mAudioTrack.pause();
                    threadLock.wait();
                    if (mAudioTrack != null)
                        mAudioTrack.play();
                } catch (InterruptedException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }

        for (int i = 0; i < buffer.length; ) {
            int result = mAudioTrack.write(buffer, i, buffer.length - i);
            if (result > 0) {
                i += result;
            } else if (result == 0) {
                try {
                    Thread.sleep(1);
                } catch (InterruptedException e) {
                    // Nom nom
                }
            } else {
                Log.w("SDL", "SDL audio: error return from write(short)");
                return;
            }
        }
    }

    public static void audioQuit() {
        if (mAudioThread != null) {
            try {
                mAudioThread.join();
            } catch (Exception e) {
                Log.v("SDL", "Problem stopping audio thread: " + e);
            }
            mAudioThread = null;

            //Log.v("SDL", "Finished waiting for audio thread");
        }

        if (mAudioTrack != null) {
            mAudioTrack.stop();
            mAudioTrack = null;
        }
    }
}




