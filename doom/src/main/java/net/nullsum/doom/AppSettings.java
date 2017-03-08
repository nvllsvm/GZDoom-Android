package net.nullsum.doom;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Environment;

import com.beloko.touchcontrols.TouchSettings;

import java.io.File;
import java.io.IOException;
import java.util.Random;

public class AppSettings {

    public static String belokoBaseDir;

    public static String musicBaseDir;

    public static String graphicsDir = "";

    public static boolean vibrate;
    public static boolean immersionMode;

    public static Context ctx;

    public static void resetBaseDir(Context ctx)
    {
        belokoBaseDir  =  Environment.getExternalStorageDirectory().toString() + "/GZDoom";
        setStringOption(ctx, "base_path", belokoBaseDir);
    }

    public static void reloadSettings(Context ctx)
    {
        AppSettings.ctx = ctx;

        TouchSettings.reloadSettings(ctx);

        belokoBaseDir = getStringOption(ctx, "base_path", null);
        if (belokoBaseDir == null)
        {
            resetBaseDir(ctx);
        }

        String music = getStringOption(ctx, "music_path", null);
        if (music == null)
        {
            music  =  belokoBaseDir + "/doom/Music";
            setStringOption(ctx, "music_path", music);
        }

        musicBaseDir =  music;

        graphicsDir = ctx.getFilesDir().toString() + "/";

        vibrate =  getBoolOption(ctx, "vibrate", true);

        immersionMode = getBoolOption(ctx, "immersion_mode", true);

        net.nullsum.doom.CDAudioPlayer.initFiles(musicBaseDir);
    }

    public static String getBaseDir()
    {
        return  AppSettings.belokoBaseDir;
    }

    public static String getGameDir()
    {
        return  AppSettings.belokoBaseDir;
    }

    public static String getQuakeFullDir()
    {
        String quakeFilesDir = AppSettings.belokoBaseDir;
        return quakeFilesDir + "/config";
    }

    public static void createDirectories(Context ctx)
    {
        boolean scan = false;

        String d = "";
        if (!new File(getQuakeFullDir() + d).exists())
            scan = true;

        new File(getQuakeFullDir() + d).mkdirs();

        //This is totally stupid, need to do this so folder shows up in explorer!
        if (scan)
        {
            File f = new File(getQuakeFullDir() + d , "temp_");
            try {
                f.createNewFile();
                new SingleMediaScanner(ctx, false,  f.getAbsolutePath());
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        else
        {
            new File(getQuakeFullDir() + d , "temp_").delete();
        }
    }

    public static void setFloatOption(Context ctx,String name, float value)
    {
        SharedPreferences settings = ctx.getSharedPreferences("OPTIONS",    Context.MODE_MULTI_PROCESS);
        SharedPreferences.Editor editor = settings.edit();
        editor.putFloat(name, value);
        editor.apply();
    }

    public static boolean getBoolOption(Context ctx,String name, boolean def)
    {
        SharedPreferences settings = ctx.getSharedPreferences("OPTIONS",    Context.MODE_MULTI_PROCESS);
        return settings.getBoolean(name, def);
    }

    public static void setBoolOption(Context ctx,String name, boolean value)
    {
        SharedPreferences settings = ctx.getSharedPreferences("OPTIONS",    Context.MODE_MULTI_PROCESS);
        SharedPreferences.Editor editor = settings.edit();
        editor.putBoolean(name, value);
        editor.apply();
    }

    public static int getIntOption(Context ctx,String name, int def)
    {
        SharedPreferences settings = ctx.getSharedPreferences("OPTIONS",    Context.MODE_MULTI_PROCESS);
        return settings.getInt(name, def);
    }

    public static void setIntOption(Context ctx,String name, int value)
    {
        SharedPreferences settings = ctx.getSharedPreferences("OPTIONS",    Context.MODE_MULTI_PROCESS);
        SharedPreferences.Editor editor = settings.edit();
        editor.putInt(name, value);
        editor.apply();
    }

    public static long getLongOption(Context ctx,String name, long def)
    {
        SharedPreferences settings = ctx.getSharedPreferences("OPTIONS",    Context.MODE_MULTI_PROCESS);
        return settings.getLong(name, def);
    }

    public static void setLongOption(Context ctx,String name, long value)
    {
        SharedPreferences settings = ctx.getSharedPreferences("OPTIONS",    Context.MODE_MULTI_PROCESS);
        SharedPreferences.Editor editor = settings.edit();
        editor.putLong(name, value);
        editor.apply();
    }

    public static String getStringOption(Context ctx,String name, String def)
    {
        SharedPreferences settings = ctx.getSharedPreferences("OPTIONS",    Context.MODE_MULTI_PROCESS);
        return settings.getString(name, def);
    }

    public static void setStringOption(Context ctx,String name, String value)
    {
        SharedPreferences settings = ctx.getSharedPreferences("OPTIONS",    Context.MODE_MULTI_PROCESS);
        SharedPreferences.Editor editor = settings.edit();
        editor.putString(name, value);
        editor.apply();
    }
}
