package net.nullsum.doom;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.beloko.touchcontrols.ActionInput;
import com.beloko.touchcontrols.ControlConfig;
import com.beloko.touchcontrols.ControlConfig.Type;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;

class Utils {
    private static final String LOG = "Utils";

    private static void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while ((read = in.read(buffer)) != -1) {
            out.write(buffer, 0, read);
        }
        out.close();
    }

    static public void copyPNGAssets(Context ctx, String dir) {
        String prefix = "";

        File d = new File(dir);
        if (!d.exists())
            d.mkdirs();

        AssetManager assetManager = ctx.getAssets();
        String[] files = null;
        try {
            files = assetManager.list("");
        } catch (IOException e) {
            Log.e("tag", "Failed to get asset file list.", e);
        }
        for (String filename : files) {
            if (filename.endsWith("png") && filename.startsWith(prefix)) {
                InputStream in;
                OutputStream out;
                //Log.d("test","file = " + filename);
                try {
                    in = assetManager.open(filename);
                    out = new FileOutputStream(dir + "/" + filename.substring(prefix.length()));
                    copyFile(in, out);
                    in.close();
                    out.flush();
                    out.close();
                } catch (IOException e) {
                    Log.e("tag", "Failed to copy asset file: " + filename, e);
                }
            }
        }
    }

    static public String[] creatArgs(String appArgs) {
        ArrayList<String> a = new ArrayList<>(Arrays.asList(appArgs.split(" ")));

        Iterator<String> iter = a.iterator();
        while (iter.hasNext()) {
            if (iter.next().contentEquals("")) {
                iter.remove();
            }
        }

        return a.toArray(new String[a.size()]);
    }


    static public void copyAsset(Context ctx, String file, String destdir) {
        AssetManager assetManager = ctx.getAssets();

        InputStream in;
        OutputStream out;

        try {
            in = assetManager.open(file);
            out = new FileOutputStream(destdir + "/" + file);
            copyFile(in, out);
            in.close();
            out.flush();
            out.close();
        } catch (IOException e) {
            Log.e("tag", "Failed to copy asset file: " + file);
        }
    }

    public static void loadArgs(Context ctx, ArrayList<String> args) {
        File cacheDir = ctx.getFilesDir();

        FileInputStream fis;
        ObjectInputStream in;
        try {
            fis = new FileInputStream(new File(cacheDir, "args_hist.dat"));
            in = new ObjectInputStream(fis);
            ArrayList<String> argsHistory = (ArrayList<String>) in.readObject();
            args.clear();
            args.addAll(argsHistory);
            in.close();
            return;
        } catch (IOException | ClassNotFoundException ex) {

        }
        //failed load, load default
        args.clear();
    }

    public static void saveArgs(Context ctx, ArrayList<String> args) {
        File cacheDir = ctx.getFilesDir();

        if (!cacheDir.exists())
            cacheDir.mkdirs();

        FileOutputStream fos;
        ObjectOutputStream out;
        try {
            fos = new FileOutputStream(new File(cacheDir, "args_hist.dat"));
            out = new ObjectOutputStream(fos);
            out.writeObject(args);
            out.close();
        } catch (IOException ex) {
            Toast.makeText(ctx, "Error saving args History list: " + ex.toString(), Toast.LENGTH_LONG).show();
        }
    }

    public static void setImmersionMode(final Activity act) {

        if (AppSettings.immersionMode) {
            act.getWindow().getDecorView().setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
                            | View.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
                            | View.SYSTEM_UI_FLAG_IMMERSIVE
                            | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);

            View decorView = act.getWindow().getDecorView();
            decorView.setOnSystemUiVisibilityChangeListener
                    (new View.OnSystemUiVisibilityChangeListener() {
                        @Override
                        public void onSystemUiVisibilityChange(int visibility) {
                            Log.d(LOG, "onSystemUiVisibilityChange");

                            act.getWindow().getDecorView().setSystemUiVisibility(
                                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
                                            | View.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
                                            | View.SYSTEM_UI_FLAG_IMMERSIVE
                                            | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);

                        }
                    });
        }
    }

    public static void onWindowFocusChanged(final Activity act, final boolean hasFocus) {

        if (AppSettings.immersionMode) {
            Handler handler = new Handler();

            handler.postDelayed(new Runnable() {
                public void run() {

                    if (hasFocus) {
                        act.getWindow().getDecorView().setSystemUiVisibility(
                                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                                        | View.SYSTEM_UI_FLAG_IMMERSIVE
                                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
                    }
                }

            }, 2000);
        }
    }

    public static ArrayList<ActionInput> getGameGamepadConfig() {
        ArrayList<ActionInput> actions = new ArrayList<>();

        actions.add(new ActionInput("analog_look_pitch", "Look Up/Look Down", ControlConfig.ACTION_ANALOG_PITCH, Type.ANALOG));
        actions.add(new ActionInput("analog_look_yaw", "Look Left/Look Right", ControlConfig.ACTION_ANALOG_YAW, Type.ANALOG));
        actions.add(new ActionInput("analog_move_fwd", "Forward/Back", ControlConfig.ACTION_ANALOG_FWD, Type.ANALOG));
        actions.add(new ActionInput("analog_move_strafe", "Strafe", ControlConfig.ACTION_ANALOG_STRAFE, Type.ANALOG));
        actions.add(new ActionInput("attack", "Attack", ControlConfig.PORT_ACT_ATTACK, Type.BUTTON));
        actions.add(new ActionInput("attack_alt", "Alt Attack (GZ)", ControlConfig.PORT_ACT_ALT_ATTACK, Type.BUTTON));
        actions.add(new ActionInput("back", "Move Backwards", ControlConfig.PORT_ACT_BACK, Type.BUTTON));
        actions.add(new ActionInput("crouch", "Crouch (GZ)", ControlConfig.PORT_ACT_DOWN, Type.BUTTON));
        actions.add(new ActionInput("custom_0", "Custom A (GZ)", ControlConfig.PORT_ACT_CUSTOM_0, Type.BUTTON));
        actions.add(new ActionInput("custom_1", "Custom B (GZ)", ControlConfig.PORT_ACT_CUSTOM_1, Type.BUTTON));
        actions.add(new ActionInput("custom_2", "Custom C (GZ)", ControlConfig.PORT_ACT_CUSTOM_2, Type.BUTTON));
        actions.add(new ActionInput("custom_3", "Custom D (GZ)", ControlConfig.PORT_ACT_CUSTOM_3, Type.BUTTON));
        actions.add(new ActionInput("custom_4", "Custom E (GZ)", ControlConfig.PORT_ACT_CUSTOM_4, Type.BUTTON));
        actions.add(new ActionInput("custom_5", "Custom F (GZ)", ControlConfig.PORT_ACT_CUSTOM_5, Type.BUTTON));
        actions.add(new ActionInput("fly_down", "Fly Down", ControlConfig.PORT_ACT_FLY_DOWN, Type.BUTTON));
        actions.add(new ActionInput("fly_up", "Fly Up", ControlConfig.PORT_ACT_FLY_UP, Type.BUTTON));
        actions.add(new ActionInput("fwd", "Move Forward", ControlConfig.PORT_ACT_FWD, Type.BUTTON));
        actions.add(new ActionInput("inv_drop", "Drop Item", ControlConfig.PORT_ACT_INVDROP, Type.BUTTON));
        actions.add(new ActionInput("inv_next", "Next Item", ControlConfig.PORT_ACT_INVNEXT, Type.BUTTON));
        actions.add(new ActionInput("inv_prev", "Prev Item", ControlConfig.PORT_ACT_INVPREV, Type.BUTTON));
        actions.add(new ActionInput("inv_use", "Use Item", ControlConfig.PORT_ACT_INVUSE, Type.BUTTON));
        actions.add(new ActionInput("jump", "Jump (GZ)", ControlConfig.PORT_ACT_JUMP, Type.BUTTON));
        actions.add(new ActionInput("left", "Strafe Left", ControlConfig.PORT_ACT_MOVE_LEFT, Type.BUTTON));
        actions.add(new ActionInput("look_left", "Look Left", ControlConfig.PORT_ACT_LEFT, Type.BUTTON));
        actions.add(new ActionInput("look_right", "Look Right", ControlConfig.PORT_ACT_RIGHT, Type.BUTTON));
        actions.add(new ActionInput("map_down", "Automap Down", ControlConfig.PORT_ACT_MAP_DOWN, Type.BUTTON));
        actions.add(new ActionInput("map_left", "Automap Left", ControlConfig.PORT_ACT_MAP_LEFT, Type.BUTTON));
        actions.add(new ActionInput("map_right", "Automap Right", ControlConfig.PORT_ACT_MAP_RIGHT, Type.BUTTON));
        actions.add(new ActionInput("map_show", "Show Automap", ControlConfig.PORT_ACT_MAP, Type.BUTTON));
        actions.add(new ActionInput("map_up", "Automap Up", ControlConfig.PORT_ACT_MAP_UP, Type.BUTTON));
        actions.add(new ActionInput("map_zoomin", "Automap Zoomin", ControlConfig.PORT_ACT_MAP_ZOOM_IN, Type.BUTTON));
        actions.add(new ActionInput("map_zoomout", "Automap Zoomout", ControlConfig.PORT_ACT_MAP_ZOOM_OUT, Type.BUTTON));
        actions.add(new ActionInput("menu_back", "Menu Back", ControlConfig.MENU_BACK, Type.MENU));
        actions.add(new ActionInput("menu_down", "Menu Down", ControlConfig.MENU_DOWN, Type.MENU));
        actions.add(new ActionInput("menu_left", "Menu Left", ControlConfig.MENU_LEFT, Type.MENU));
        actions.add(new ActionInput("menu_right", "Menu Right", ControlConfig.MENU_RIGHT, Type.MENU));
        actions.add(new ActionInput("menu_select", "Menu Select", ControlConfig.MENU_SELECT, Type.MENU));
        actions.add(new ActionInput("menu_up", "Menu Up", ControlConfig.MENU_UP, Type.MENU));
        actions.add(new ActionInput("next_weapon", "Next Weapon", ControlConfig.PORT_ACT_NEXT_WEP, Type.BUTTON));
        actions.add(new ActionInput("prev_weapon", "Previous Weapon", ControlConfig.PORT_ACT_PREV_WEP, Type.BUTTON));
        actions.add(new ActionInput("quick_load", "Quick Load (GZ)", ControlConfig.PORT_ACT_QUICKLOAD, Type.BUTTON));
        actions.add(new ActionInput("quick_save", "Quick Save (GZ)", ControlConfig.PORT_ACT_QUICKSAVE, Type.BUTTON));
        actions.add(new ActionInput("right", "Strafe Right", ControlConfig.PORT_ACT_MOVE_RIGHT, Type.BUTTON));
        actions.add(new ActionInput("show_keys", "Show Keys", ControlConfig.PORT_ACT_SHOW_KEYS, Type.BUTTON));
        actions.add(new ActionInput("show_weap", "Show Stats/Weapons", ControlConfig.PORT_ACT_SHOW_WEAPONS, Type.BUTTON));
        actions.add(new ActionInput("speed", "Run On", ControlConfig.PORT_ACT_SPEED, Type.BUTTON));
        actions.add(new ActionInput("strafe_on", "Strafe On", ControlConfig.PORT_ACT_STRAFE, Type.BUTTON));
        actions.add(new ActionInput("use", "Use/Open", ControlConfig.PORT_ACT_USE, Type.BUTTON));

        return actions;
    }

}
