package com.beloko.opengames;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;


public class GD {
	public static boolean DEBUG = true;

	public enum IDGame{Quake,Quake2,Doom,Duke3d,Quake3,Hexen2,RTCW,Wolf3d,JK2,JK3,Heretic,Hexen,Strife,AVP,Shadow,Gish,Descent1,Descent2,Homeworld,BlakeStone,Noah,Doom3,TestPlatform,Abuse};
	
	public static String qc_fn = "quick_cmd.dat";

	public static int version;
	
	public static void init(Context ctx)
	{
		PackageInfo pInfo;
		try {
			pInfo = ctx.getPackageManager().getPackageInfo(ctx.getPackageName(), 0);
			version = pInfo.versionCode;
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
