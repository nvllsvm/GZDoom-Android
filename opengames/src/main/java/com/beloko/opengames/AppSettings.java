package com.beloko.opengames;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Environment;

import com.beloko.touchcontrols.TouchSettings;

import java.io.File;
import java.io.IOException;
import java.util.Random;

public class AppSettings {

	public static GD.IDGame game;

	public static String belokoBaseDir;

	public static String musicBaseDir;

	public static String graphicsDir = "";

	public static boolean vibrate;
	public static boolean immersionMode;

	public static long rid = 0x12345678abcdef12l;

	public static int rnlvl = 0;
	
	public static void setGame(GD.IDGame g)
	{
		game = g;
	}

	public static Context ctx;

	public static void resetBaseDir(Context ctx)
	{
		belokoBaseDir  =  Environment.getExternalStorageDirectory().toString() + "/Beloko";
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
			music  =  belokoBaseDir + "/" + game + "/Music";
			setStringOption(ctx, "music_path", music);
		}

		musicBaseDir =  music;

		graphicsDir = ctx.getFilesDir().toString() + "/";

		vibrate =  getBoolOption(ctx, "vibrate", true);

		immersionMode = getBoolOption(ctx, "immersion_mode", false);
		

		rid = getLongOption(ctx, "rid", 0);

		if (rid == 0)
		{
			Random randomGenerator = new Random();
			rid = randomGenerator.nextLong();
			setLongOption(ctx, "rid", rid);
		}
		
		Random randomGenerator = new Random();
		rnlvl = randomGenerator.nextInt();
		
		 
		com.beloko.opengames.CDAudioPlayer.initFiles(musicBaseDir);

	}

	public static String getBaseDir()
	{
		return  AppSettings.belokoBaseDir;
	}

	public static String getGameDir()
	{
		return  AppSettings.belokoBaseDir + "/" + game;
	}

	public static String getQuakeDemoDir()
	{
		String quakeFilesDir = AppSettings.belokoBaseDir + "/" + game;
		return  quakeFilesDir + "/DEMO";
	}

	public static String getQuakeFullDir()
	{
		String quakeFilesDir = AppSettings.belokoBaseDir + "/" + game;
		return quakeFilesDir + "/FULL";
	}

	public static void createDirectories(Context ctx)
	{
		boolean scan = false;

		if (!new File(getQuakeDemoDir()).exists())
			scan = true;

		new File(getQuakeDemoDir()).mkdirs();


		String d = "";
		if (game == GD.IDGame.Quake)
			d = "/id1/";
		else if (game == GD.IDGame.Quake2)
			d = "/baseq2/";
		else if (game == GD.IDGame.Quake3)
			d = "/baseq3/";
		else if (game ==  GD.IDGame.Hexen2)
			d = "/data1/";
		else if (game ==  GD.IDGame.RTCW)
			d = "/main/";
		else if (game ==  GD.IDGame.JK2)
			d = "/base/";
		else if (game ==  GD.IDGame.JK3)
			d = "/base/";

		if (!new File(getQuakeFullDir() + d).exists())
			scan = true;

		new File(getQuakeFullDir() + d).mkdirs();

		//This is totally stupid, need to do this so folder shows up in explorer!
		if (scan) 
		{
			File f = new File(getQuakeDemoDir() , "temp_");
			try {
				f.createNewFile();
				new SingleMediaScanner(ctx, false,  f.getAbsolutePath());
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			f = new File(getQuakeFullDir() + d , "temp_");
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
			new File(getQuakeDemoDir() , "temp_").delete();
			new File(getQuakeFullDir() + d , "temp_").delete();
		}



		/*//Crashes on kitkat 4.4!!
		if (scan)
		{
			Log.d("AppSettings","Doing media scan");
			ctx.sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED, Uri.parse("file://" + getBaseDir())));
		}
		 */
	}

	public static boolean showAbout(Context ctx)
	{
		try {
			int versionCode = ctx.getPackageManager().getPackageInfo(ctx.getPackageName(), 0).versionCode;

			SharedPreferences settings = ctx.getSharedPreferences("OPTIONS", 0);
			int last_ver = settings.getInt("last_opened_version", -1);
			//Log.d("test"," ver = " +  versionCode + " las=" + last_ver);
			if (versionCode != last_ver)
			{
				SharedPreferences.Editor editor = settings.edit();
				editor.putInt("last_opened_version", versionCode);
				editor.commit();
				return true;
			}
			else
				return false;

		} catch (NameNotFoundException e) {
			return false;
		}
	}

	public static float getFloatOption(Context ctx,String name, float def)
	{
		SharedPreferences settings = ctx.getSharedPreferences("OPTIONS", 	Context.MODE_MULTI_PROCESS);
		return settings.getFloat(name, def);
	}

	public static void setFloatOption(Context ctx,String name, float value)
	{
		SharedPreferences settings = ctx.getSharedPreferences("OPTIONS", 	Context.MODE_MULTI_PROCESS);
		SharedPreferences.Editor editor = settings.edit();
		editor.putFloat(name, value);
		editor.commit();
	}

	public static boolean getBoolOption(Context ctx,String name, boolean def)
	{
		SharedPreferences settings = ctx.getSharedPreferences("OPTIONS", 	Context.MODE_MULTI_PROCESS);
		return settings.getBoolean(name, def);
	}

	public static void setBoolOption(Context ctx,String name, boolean value)
	{
		SharedPreferences settings = ctx.getSharedPreferences("OPTIONS", 	Context.MODE_MULTI_PROCESS);
		SharedPreferences.Editor editor = settings.edit();
		editor.putBoolean(name, value);
		editor.commit();
	}

	public static int getIntOption(Context ctx,String name, int def)
	{
		SharedPreferences settings = ctx.getSharedPreferences("OPTIONS", 	Context.MODE_MULTI_PROCESS);
		return settings.getInt(name, def);
	}

	public static void setIntOption(Context ctx,String name, int value)
	{
		SharedPreferences settings = ctx.getSharedPreferences("OPTIONS", 	Context.MODE_MULTI_PROCESS);
		SharedPreferences.Editor editor = settings.edit();
		editor.putInt(name, value);
		editor.commit();
	}

	public static long getLongOption(Context ctx,String name, long def)
	{
		SharedPreferences settings = ctx.getSharedPreferences("OPTIONS", 	Context.MODE_MULTI_PROCESS);
		return settings.getLong(name, def);
	}

	public static void setLongOption(Context ctx,String name, long value)
	{
		SharedPreferences settings = ctx.getSharedPreferences("OPTIONS", 	Context.MODE_MULTI_PROCESS);
		SharedPreferences.Editor editor = settings.edit();
		editor.putLong(name, value);
		editor.commit();
	}

	public static String getStringOption(Context ctx,String name, String def)
	{
		SharedPreferences settings = ctx.getSharedPreferences("OPTIONS", 	Context.MODE_MULTI_PROCESS);
		return settings.getString(name, def);
	}

	public static void setStringOption(Context ctx,String name, String value)
	{
		SharedPreferences settings = ctx.getSharedPreferences("OPTIONS", 	Context.MODE_MULTI_PROCESS);
		SharedPreferences.Editor editor = settings.edit();
		editor.putString(name, value);
		editor.commit();
	}

	public static long getRID()
	{
		return rid;
	}
}
