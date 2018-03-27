package net.nullsum.doom;

import android.content.Context;
import android.opengl.GLSurfaceView.EGLConfigChooser;
import android.util.Log;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;

public class BestEglChooser implements EGLConfigChooser {
    String LOG = "BestEglChooser";

    Context ctx;

    public BestEglChooser(Context ctx) {
        this.ctx = ctx;
    }


    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

        Log.i(LOG, "chooseConfig");


        int[][] mConfigSpec = {
                {
                        EGL10.EGL_RED_SIZE, 8,
                        EGL10.EGL_GREEN_SIZE, 8,
                        EGL10.EGL_BLUE_SIZE, 8,
                        EGL10.EGL_ALPHA_SIZE, 8,
                        EGL10.EGL_DEPTH_SIZE, 24,
                        EGL10.EGL_STENCIL_SIZE, 8,
                        EGL10.EGL_NONE
                }, {
                EGL10.EGL_RED_SIZE, 8,
                EGL10.EGL_GREEN_SIZE, 8,
                EGL10.EGL_BLUE_SIZE, 8,
                EGL10.EGL_ALPHA_SIZE, 8,
                EGL10.EGL_DEPTH_SIZE, 24,
                EGL10.EGL_STENCIL_SIZE, 0,
                EGL10.EGL_NONE
        },
                {
                        EGL10.EGL_RED_SIZE, 8,
                        EGL10.EGL_GREEN_SIZE, 8,
                        EGL10.EGL_BLUE_SIZE, 8,
                        EGL10.EGL_ALPHA_SIZE, 8,
                        EGL10.EGL_DEPTH_SIZE, 16,
                        EGL10.EGL_STENCIL_SIZE, 8,
                        EGL10.EGL_NONE
                }, {
                EGL10.EGL_RED_SIZE, 8,
                EGL10.EGL_GREEN_SIZE, 8,
                EGL10.EGL_BLUE_SIZE, 8,
                EGL10.EGL_ALPHA_SIZE, 8,
                EGL10.EGL_DEPTH_SIZE, 16,
                EGL10.EGL_STENCIL_SIZE, 0,
                EGL10.EGL_NONE
        },
        };

        Log.i(LOG, "Number of specs to test: " + mConfigSpec.length);

        int specChosen;
        int[] num_config = new int[1];
        int numConfigs = 0;
        for (specChosen = 0; specChosen < mConfigSpec.length; specChosen++) {
            egl.eglChooseConfig(display, mConfigSpec[specChosen], null, 0, num_config);
            if (num_config[0] > 0) {
                numConfigs = num_config[0];
                break;
            }
        }

        if (specChosen == mConfigSpec.length) {
            throw new IllegalArgumentException(
                    "No EGL configs match configSpec");
        }

        EGLConfig[] configs = new EGLConfig[numConfigs];
        egl.eglChooseConfig(display, mConfigSpec[specChosen], configs, numConfigs, num_config);

        StringBuilder eglConfigsString = new StringBuilder();
        for (int n = 0; n < configs.length; n++) {
            Log.i(LOG, "found EGL config : " + printConfig(egl, display, configs[n]));
            eglConfigsString.append(n).append(": ").append(printConfig(egl, display, configs[n])).append(",");
        }
        Log.i(LOG, eglConfigsString.toString());
        AppSettings.setStringOption(ctx, "egl_configs", eglConfigsString.toString());


        int selected = 0;

        int override = AppSettings.getIntOption(ctx, "egl_config_selected", 0);
        if (override < configs.length) {
            selected = override;
        }

        // best choice : select first config
        Log.i(LOG, "selected EGL config[" + selected + "]: " + printConfig(egl, display, configs[selected]));

        return configs[selected];
    }


    private String printConfig(EGL10 egl, EGLDisplay display,
                               EGLConfig config) {

        int r = findConfigAttrib(egl, display, config,
                EGL10.EGL_RED_SIZE);
        int g = findConfigAttrib(egl, display, config,
                EGL10.EGL_GREEN_SIZE);
        int b = findConfigAttrib(egl, display, config,
                EGL10.EGL_BLUE_SIZE);
        int a = findConfigAttrib(egl, display, config,
                EGL10.EGL_ALPHA_SIZE);
        int d = findConfigAttrib(egl, display, config,
                EGL10.EGL_DEPTH_SIZE);
        int s = findConfigAttrib(egl, display, config,
                EGL10.EGL_STENCIL_SIZE);

		/*
         *
		 * EGL_CONFIG_CAVEAT value 

     #define EGL_NONE		       0x3038	
     #define EGL_SLOW_CONFIG		       0x3050	
     #define EGL_NON_CONFORMANT_CONFIG      0x3051	
		 */

        return String.format("rgba=%d%d%d%d z=%d sten=%d", r, g, b, a, d, s)
                + " n=" + findConfigAttrib(egl, display, config, EGL10.EGL_NATIVE_RENDERABLE)
                + " b=" + findConfigAttrib(egl, display, config, EGL10.EGL_BUFFER_SIZE)
                + String.format(" c=0x%04x", findConfigAttrib(egl, display, config, EGL10.EGL_CONFIG_CAVEAT))
                ;

    }

    private int findConfigAttrib(EGL10 egl, EGLDisplay display,
                                 EGLConfig config, int attribute) {

        int[] mValue = new int[1];
        if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
            return mValue[0];
        }
        return 0;
    }

}
