package com.beloko.opengames;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.animation.Animation;
import android.view.animation.Transformation;
import android.widget.Toast;

import com.beloko.opengames.GD.IDGame;
import com.beloko.touchcontrols.ActionInput;
import com.beloko.touchcontrols.ControlConfig;
import com.beloko.touchcontrols.ControlConfig.Type;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;
import java.util.zip.ZipInputStream;

;

public class Utils {
	static String LOG = "Utils";

	static public void copyFile(InputStream in, OutputStream out) throws IOException {
		byte[] buffer = new byte[1024];
		int read;
		while((read = in.read(buffer)) != -1){
			out.write(buffer, 0, read);
		}
		out.close(); 
	}

	static public void copyFile(InputStream in, OutputStream out,ProgressDialog pb) throws IOException {
		byte[] buffer = new byte[1024];
		int read;
		while((read = in.read(buffer)) != -1){
			out.write(buffer, 0, read);
			pb.setProgress(pb.getProgress() + 1024);
		}
		out.close(); 
	}
	static public  void showDownloadDialog(final Activity act,String title,final String KEY,final String directory,final String file)
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(act);
		builder.setMessage(title)
		.setCancelable(true)
		.setPositiveButton("OK", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int id) {
				ServerAPI.downloadFile(act,file,directory);
			}
		});
		builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int which) {
				dialog.cancel();
			}
		});                                                   
		AlertDialog alert = builder.create();
		alert.show(); 
	}
	
	public static String checkFiles(String basePath,String[] files_to_ceck)
	{
		File[] files = new File(basePath ).listFiles();
		boolean ok=true;

		String filesNotFound = "";

		String[] expected;
		expected = files_to_ceck;

		if (files == null)
			files = new File[0];

		if (files!=null)
		{
			for (File f: files)
			{
				Log.d(LOG,"FILES: " + f.toString());

			}

			for (String e: expected)
			{
				boolean found=false;
				for (File f: files)
				{
					if (f.toString().toLowerCase().endsWith(e.toLowerCase()))
						found = true;
				}
				if (!found)
				{
					Log.d(LOG,"Didnt find " + e);
					filesNotFound +=  e + "\n";
					ok = false;
				}
			}
		}

		if (filesNotFound.contentEquals(""))
			return null;
		else
			return filesNotFound;

	}
	static public void copyPNGAssets(Context ctx,String dir) {
		copyPNGAssets(ctx,dir,"");
	}

	static public void copyPNGAssets(Context ctx,String dir,String prefix) {

		if (prefix == null)
			prefix = "";

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
		for(String filename : files) {
			if (filename.endsWith("png") && filename.startsWith(prefix)){
				InputStream in = null;
				OutputStream out = null;
				//Log.d("test","file = " + filename);
				try {
					in = assetManager.open(filename);
					out = new FileOutputStream(dir + "/" + filename.substring(prefix.length()));
					copyFile(in, out);
					in.close();
					in = null;
					out.flush();
					out.close();
					out = null;
				} catch(IOException e) {
					Log.e("tag", "Failed to copy asset file: " + filename, e);
				}       
			}
		}
	}

	public static void ExtractAsset(Context ctx,String file, String dest)
	{
		ExtractAsset.ctx = ctx;
		new ExtractAsset().execute(file,dest);
	}

	static private class ExtractAsset extends AsyncTask<String, Integer, Long> {

		private ProgressDialog progressBar;
		String errorstring= null;
		static Context ctx;

		@Override
		protected void onPreExecute() {
			progressBar = new ProgressDialog(ctx);
			progressBar.setMessage("Extracting files..");
			progressBar.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
			progressBar.setCancelable(false);
			progressBar.show();
		}

		int getTotalZipSize(String file)
		{
			int ret = 0;
			try {
				ZipFile zf = new ZipFile(file);
				Enumeration e = zf.entries();
				while (e.hasMoreElements()) {
					ZipEntry ze = (ZipEntry) e.nextElement();
					String name = ze.getName();

					ret += ze.getSize();
					long compressedSize = ze.getCompressedSize();
				}
			} catch (IOException ex) {
				System.err.println(ex);
			}
			return ret;
		}

		protected Long doInBackground(String... info) {

			String file = info[0];
			String basePath = info[1];

			boolean isLocal = false;

			progressBar.setProgress(0);

            try
            {

                BufferedInputStream in = null;
                FileOutputStream fout = null;


                AssetManager assetManager = ctx.getAssets();
                InputStream ins = assetManager.open(file);

                progressBar.setMax(1024*1024*5); //TODO FIX ME

                in = new BufferedInputStream(ins);

                if (file.endsWith(".zip"))
                {
                    ZipInputStream zis = new ZipInputStream(new BufferedInputStream(in));
                    ZipEntry entry;
                    while ((entry = zis.getNextEntry()) != null) {
                        if(entry.isDirectory()) {
                            // Assume directories are stored parents first then children.
                            System.err.println("Extracting directory: " + entry.getName());
                            // This is not robust, just for demonstration purposes.
                            (new File(basePath, entry.getName())).mkdirs();
                            continue;
                        }
                        if (GD.DEBUG) Log.d(LOG,"Extracting file: " + entry.getName());
                        (new File(basePath, entry.getName())).getParentFile().mkdirs();
                        BufferedInputStream zin = new BufferedInputStream(zis);
                        OutputStream out =  new FileOutputStream(new File(basePath, entry.getName()));
                        Utils.copyFile(zin,out,progressBar);
                    }
                }
                else
                {
                    File outZipFile = new File(basePath,"temp.zip");

                    fout = new FileOutputStream(outZipFile);
                    byte data[] = new byte[1024];
                    int count;
                    while ((count = in.read(data, 0, 1024)) != -1)
                    {
                        fout.write(data, 0, count);
                        progressBar.setProgress(progressBar.getProgress() + count);
                    }
                    in.close();
                    fout.close();
                    //AppSettings.setBoolOption(ctx,"DLF" + ServerAPI.file, true);

                    outZipFile.renameTo(new File(basePath , file));
                    return 0l;
                }

            } catch (IOException e) {
                errorstring = e.toString();
                return 1l;
            }

			return 0l;
		}

		protected void onProgressUpdate(Integer... progress) {

		}

		protected void onPostExecute(Long result) {
			progressBar.dismiss();
			if (errorstring!=null)
			{
				AlertDialog.Builder builder = new AlertDialog.Builder(ctx);
				builder.setMessage("Error accessing server: " + errorstring)
						.setCancelable(true)
						.setPositiveButton("OK", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {

							}
						});

				builder.show();
			}
		}
	}

	static public String[] creatArgs(String appArgs)
	{
		ArrayList<String> a = new ArrayList<String>(Arrays.asList(appArgs.split(" ")));

		Iterator<String> iter = a.iterator();
		while (iter.hasNext()) {
			if (iter.next().contentEquals("")) {
				iter.remove();
			}
		}

		return a.toArray(new String[a.size()]);
	}


	public static void expand(final View v) {
		v.measure(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT);
		final int targtetHeight = v.getMeasuredHeight();

		v.getLayoutParams().height = 0;
		v.setVisibility(View.VISIBLE);
		Animation a = new Animation()
		{
			@Override
			protected void applyTransformation(float interpolatedTime, Transformation t) {
				v.getLayoutParams().height = interpolatedTime == 1
						? LayoutParams.WRAP_CONTENT
								: (int)(targtetHeight * interpolatedTime);
				v.requestLayout();
			}

			@Override
			public boolean willChangeBounds() {
				return true;
			}
		};

		// 1dp/ms
		a.setDuration((int)(targtetHeight / v.getContext().getResources().getDisplayMetrics().density));
		v.startAnimation(a);
	}

	public static void collapse(final View v) {
		final int initialHeight = v.getMeasuredHeight();

		Animation a = new Animation()
		{
			@Override
			protected void applyTransformation(float interpolatedTime, Transformation t) {
				if(interpolatedTime == 1){
					v.setVisibility(View.GONE);
				}else{
					v.getLayoutParams().height = initialHeight - (int)(initialHeight * interpolatedTime);
					v.requestLayout();
				}
			}

			@Override
			public boolean willChangeBounds() {
				return true;
			}
		};

		// 1dp/ms
		a.setDuration((int)(initialHeight / v.getContext().getResources().getDisplayMetrics().density));
		v.startAnimation(a);
	}


	static final int BUFFER_SIZE = 1024;

	static public String getLogCat() {
		String[] logcatArgs = new String[] {"logcat", "-d", "-v", "time"};

		Process logcatProc = null;
		try {
			logcatProc = Runtime.getRuntime().exec(logcatArgs);
		}
		catch (IOException e) {
			return null;
		}

		BufferedReader reader = null;
		String response = null;
		try {
			String separator = System.getProperty("line.separator");
			StringBuilder sb = new StringBuilder();
			reader = new BufferedReader(new InputStreamReader(logcatProc.getInputStream()), BUFFER_SIZE);
			String line;
			while ((line = reader.readLine()) != null) {
				sb.append(line);
				sb.append(separator);
			}
			response = sb.toString();
		}
		catch (IOException e) {
		}
		finally {
			if (reader != null) {
				try {
					reader.close();
				}
				catch (IOException e) {}
			}
		}

		return response;
	}


	static public void copyAsset(Context ctx,String file,String destdir) {
		AssetManager assetManager = ctx.getAssets();

		InputStream in = null;
		OutputStream out = null;

		try {
			in = assetManager.open(file);
			out = new FileOutputStream(destdir + "/" + file);
			copyFile(in, out);
			in.close();
			in = null;
			out.flush();
			out.close();
			out = null;
		} catch(IOException e) {
			Log.e("tag", "Failed to copy asset file: " + file);
		}       
	}

	public static int calculateInSampleSize(
			BitmapFactory.Options options, int reqWidth, int reqHeight) {
		// Raw height and width of image
		final int height = options.outHeight;
		final int width = options.outWidth;  
		int inSampleSize = 1;

		if (height > reqHeight || width > reqWidth) {

			// Calculate ratios of height and width to requested height and width
			final int heightRatio = Math.round((float) height / (float) reqHeight);
			final int widthRatio = Math.round((float) width / (float) reqWidth);

			// Choose the smallest ratio as inSampleSize value, this will guarantee
			// a final image with both dimensions larger than or equal to the
			// requested height and width.
			inSampleSize = heightRatio < widthRatio ? heightRatio : widthRatio;
		}

		return inSampleSize;
	}

	public static Bitmap decodeSampledBitmapFromResource(Resources res, int resId,
			int reqWidth, int reqHeight) {

		// First decode with inJustDecodeBounds=true to check dimensions
		final BitmapFactory.Options options = new BitmapFactory.Options();
		options.inJustDecodeBounds = true;
		BitmapFactory.decodeResource(res, resId, options);

		// Calculate inSampleSize
		options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight);

		// Decode bitmap with inSampleSize set
		options.inJustDecodeBounds = false;
		return BitmapFactory.decodeResource(res, resId, options);
	}

	public static void loadArgs(Context ctx,ArrayList<String> args)                         
	{ 
		File cacheDir = ctx.getFilesDir();

		FileInputStream fis = null;
		ObjectInputStream in = null;
		try
		{
			fis = new FileInputStream(new File(cacheDir,"args_hist.dat"));
			in = new ObjectInputStream(fis);                
			ArrayList<String> argsHistory = (ArrayList<String>)in.readObject();
			args.clear();
			args.addAll(argsHistory);
			in.close();
			return;
		}
		catch(IOException ex)
		{

		}  
		catch(ClassNotFoundException ex)
		{

		}
		//failed load, load default
		args.clear();
	}  


	public static void saveArgs(Context ctx,ArrayList<String> args)
	{
		File cacheDir = ctx.getFilesDir();

		if (!cacheDir.exists())
			cacheDir.mkdirs();

		FileOutputStream fos = null;
		ObjectOutputStream out = null;
		try
		{
			fos = new FileOutputStream(new File(cacheDir,"args_hist.dat"));
			out = new ObjectOutputStream(fos);
			out.writeObject(args);
			out.close();
		}
		catch(IOException ex)         
		{
			Toast.makeText(ctx,"Error saving args History list: " + ex.toString(), Toast.LENGTH_LONG).show();
		}
	}              

	public static void copyTimidityFile(Activity act)
	{
		File timiditycfg = new File(com.beloko.opengames.AppSettings.getBaseDir() + "/eawpats/timidity.cfg"  );
		File doomtimiditycfg = new File(com.beloko.opengames.AppSettings.getGameDir() + "/timidity.cfg"  );

		if (timiditycfg.exists() && !doomtimiditycfg.exists())
		{
			Log.d(LOG,"Copying timidity file");
			try {
				Utils.copyFile(new FileInputStream(timiditycfg),new FileOutputStream(doomtimiditycfg));
			} catch (FileNotFoundException e) {
				Toast.makeText(act,"Error copying timidity.cfg " + e.toString(), Toast.LENGTH_LONG).show();
			} catch (IOException e) {
				Toast.makeText(act,"Error copying timidity.cfg " + e.toString(), Toast.LENGTH_LONG).show();
			}
		}
	}


	public static void setImmersionMode(final Activity act)
	{
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {

			if (AppSettings.immersionMode)
			{
				act.getWindow().getDecorView().setSystemUiVisibility(
						View.SYSTEM_UI_FLAG_LAYOUT_STABLE
						| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
						| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
						| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
						| View.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
						| View.SYSTEM_UI_FLAG_IMMERSIVE
						| View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY );

				View decorView = act.getWindow().getDecorView();
				decorView.setOnSystemUiVisibilityChangeListener
				(new View.OnSystemUiVisibilityChangeListener() {
					@Override
					public void onSystemUiVisibilityChange(int visibility) {
						Log.d(LOG,"onSystemUiVisibilityChange");

						act.getWindow().getDecorView().setSystemUiVisibility(
								View.SYSTEM_UI_FLAG_LAYOUT_STABLE
								| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
								| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
								| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
								| View.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
								| View.SYSTEM_UI_FLAG_IMMERSIVE
								| View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY );

					}
				});
			}
		}
	}

	public static void onWindowFocusChanged(final Activity act,final boolean hasFocus)
	{
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {

			if (AppSettings.immersionMode)
			{
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
	}

	public static ArrayList<ActionInput> getGameGamepadConfig(GD.IDGame game)
	{
		ArrayList<ActionInput> actions = new ArrayList<ActionInput>();

		actions.add(new ActionInput("analog_move_fwd","Forward/Back", ControlConfig.ACTION_ANALOG_FWD, Type.ANALOG));
		actions.add(new ActionInput("analog_move_strafe","Strafe",ControlConfig.ACTION_ANALOG_STRAFE,Type.ANALOG));


		if (game != IDGame.Wolf3d)
			actions.add(new ActionInput("analog_look_pitch","Look Up/Look Down",ControlConfig.ACTION_ANALOG_PITCH,Type.ANALOG));

		actions.add(new ActionInput("analog_look_yaw","Look Left/Look Right",ControlConfig.ACTION_ANALOG_YAW,Type.ANALOG));

		actions.add(new ActionInput("attack","Attack",ControlConfig.PORT_ACT_ATTACK,Type.BUTTON));

		if ((game == IDGame.Doom) || (game == IDGame.Wolf3d)|| (game == IDGame.Hexen)|| (game == IDGame.Strife)|| (game == IDGame.Heretic))
			actions.add(new ActionInput("use","Use/Open",ControlConfig.PORT_ACT_USE,Type.BUTTON));

		if (game == IDGame.RTCW)
		{
			actions.add(new ActionInput("use","Use/Open",ControlConfig.PORT_ACT_USE,Type.BUTTON));
			actions.add(new ActionInput("reload","Reload",ControlConfig.PORT_ACT_RELOAD,Type.BUTTON));
			actions.add(new ActionInput("alt_fire","Alt Weapon",ControlConfig.PORT_ACT_ALT_FIRE,Type.BUTTON));
			actions.add(new ActionInput("binocular","Binocuar",ControlConfig.PORT_ACT_ZOOM_IN,Type.BUTTON));
			actions.add(new ActionInput("quick_kick","Kick",ControlConfig.PORT_ACT_KICK,Type.BUTTON));
			actions.add(new ActionInput("lean_left","Lean Left",ControlConfig.PORT_ACT_LEAN_LEFT,Type.BUTTON));
			actions.add(new ActionInput("lean_right","Lean Right",ControlConfig.PORT_ACT_LEAN_RIGHT,Type.BUTTON));
		}

		if (game == IDGame.Quake3)
		{
			actions.add(new ActionInput("zoomin","Zoom in/out",ControlConfig.PORT_ACT_ZOOM_IN,Type.BUTTON));
			actions.add(new ActionInput("custom_0","Custom F1",ControlConfig.PORT_ACT_CUSTOM_0,Type.BUTTON));
			actions.add(new ActionInput("custom_1","Custom F2",ControlConfig.PORT_ACT_CUSTOM_1,Type.BUTTON));
			actions.add(new ActionInput("custom_2","Custom F3",ControlConfig.PORT_ACT_CUSTOM_2,Type.BUTTON));
			actions.add(new ActionInput("custom_3","Custom F4",ControlConfig.PORT_ACT_CUSTOM_3,Type.BUTTON));
		}

		if ((game == IDGame.JK2) || (game == IDGame.JK3))
		{
			actions.add(new ActionInput("attack_alt","Alt Attack",ControlConfig.PORT_ACT_ALT_ATTACK,Type.BUTTON));
			actions.add(new ActionInput("use_force","Use Force",ControlConfig.PORT_ACT_FORCE_USE,Type.BUTTON));
			actions.add(new ActionInput("saber_style","Saber Style",ControlConfig.PORT_ACT_SABER_STYLE,Type.BUTTON));
			actions.add(new ActionInput("saber_show_hide","Saber Sheath/Unsheath",ControlConfig.PORT_ACT_SABER_SEL,Type.BUTTON));
			actions.add(new ActionInput("use","Use/Open",ControlConfig.PORT_ACT_USE,Type.BUTTON));
		}

		if ((game != IDGame.Doom) && (game != IDGame.Wolf3d))
			actions.add(new ActionInput("jump","Jump",ControlConfig.PORT_ACT_JUMP,Type.BUTTON));

		if ((game == IDGame.Quake2) || (game == IDGame.Quake3)|| (game == IDGame.Hexen2)|| (game == IDGame.RTCW)|| (game == IDGame.JK2) || (game == IDGame.JK3))
			actions.add(new ActionInput("crouch","Crouch",ControlConfig.PORT_ACT_DOWN,Type.BUTTON));

		//Add GZDoom specific actions
		if (game == IDGame.Doom)
		{
			actions.add(new ActionInput("attack_alt","Alt Attack (GZ)",ControlConfig.PORT_ACT_ALT_ATTACK,Type.BUTTON));
			actions.add(new ActionInput("jump","Jump (GZ)",ControlConfig.PORT_ACT_JUMP,Type.BUTTON));
			actions.add(new ActionInput("crouch","Crouch (GZ)",ControlConfig.PORT_ACT_DOWN,Type.BUTTON));
			actions.add(new ActionInput("custom_0","Custom A (GZ)",ControlConfig.PORT_ACT_CUSTOM_0,Type.BUTTON));
			actions.add(new ActionInput("custom_1","Custom B (GZ)",ControlConfig.PORT_ACT_CUSTOM_1,Type.BUTTON));
			actions.add(new ActionInput("custom_2","Custom C (GZ)",ControlConfig.PORT_ACT_CUSTOM_2,Type.BUTTON));
			actions.add(new ActionInput("custom_3","Custom D (GZ)",ControlConfig.PORT_ACT_CUSTOM_3,Type.BUTTON));
			actions.add(new ActionInput("custom_4","Custom E (GZ)",ControlConfig.PORT_ACT_CUSTOM_4,Type.BUTTON));
			actions.add(new ActionInput("custom_5","Custom F (GZ)",ControlConfig.PORT_ACT_CUSTOM_5,Type.BUTTON));
			actions.add(new ActionInput("quick_save","Quick Save (GZ)",ControlConfig.PORT_ACT_QUICKSAVE,Type.BUTTON));
			actions.add(new ActionInput("quick_load","Quick Load (GZ)",ControlConfig.PORT_ACT_QUICKLOAD,Type.BUTTON));
		}

		actions.add(new ActionInput("fwd","Move Forward",ControlConfig.PORT_ACT_FWD,Type.BUTTON));
		actions.add(new ActionInput("back","Move Backwards",ControlConfig.PORT_ACT_BACK,Type.BUTTON));
		actions.add(new ActionInput("left","Strafe Left",ControlConfig.PORT_ACT_MOVE_LEFT,Type.BUTTON));
		actions.add(new ActionInput("right","Strafe Right",ControlConfig.PORT_ACT_MOVE_RIGHT,Type.BUTTON));

		if ((game != IDGame.Doom) && (game != IDGame.Wolf3d))
		{
			actions.add(new ActionInput("look_up","Look Up",ControlConfig.PORT_ACT_LOOK_UP,Type.BUTTON));
			actions.add(new ActionInput("look_down","Look Down",ControlConfig.PORT_ACT_LOOK_DOWN,Type.BUTTON));
		}

		actions.add(new ActionInput("look_left","Look Left",ControlConfig.PORT_ACT_LEFT,Type.BUTTON));
		actions.add(new ActionInput("look_right","Look Right",ControlConfig.PORT_ACT_RIGHT,Type.BUTTON));

		if ((game != IDGame.Wolf3d) && (game != IDGame.JK2) || (game != IDGame.JK3))
		{
			actions.add(new ActionInput("strafe_on","Strafe On",ControlConfig.PORT_ACT_STRAFE,Type.BUTTON));
			actions.add(new ActionInput("speed","Run On",ControlConfig.PORT_ACT_SPEED,Type.BUTTON));
		}
		actions.add(new ActionInput("next_weapon","Next Weapon",ControlConfig.PORT_ACT_NEXT_WEP,Type.BUTTON));
		actions.add(new ActionInput("prev_weapon","Previous Weapon",ControlConfig.PORT_ACT_PREV_WEP,Type.BUTTON));

		if ((game == IDGame.JK2)|| (game == IDGame.JK3))
		{
			actions.add(new ActionInput("next_force","Next Force",ControlConfig.PORT_ACT_NEXT_FORCE,Type.BUTTON));
			actions.add(new ActionInput("prev_force","Previous Force",ControlConfig.PORT_ACT_PREV_FORCE,Type.BUTTON));
			actions.add(new ActionInput("force_pull","Force Pull",ControlConfig.PORT_ACT_FORCE_PULL,Type.BUTTON));
			actions.add(new ActionInput("force_push","Force Push",ControlConfig.PORT_ACT_FORCE_PUSH,Type.BUTTON));
			actions.add(new ActionInput("force_speed","Force Speed",ControlConfig.PORT_ACT_FORCE_SPEED,Type.BUTTON));
			actions.add(new ActionInput("force_heal","Force Heal",ControlConfig.PORT_ACT_FORCE_HEAL,Type.BUTTON));
			actions.add(new ActionInput("force_mind","Force Mind",ControlConfig.PORT_ACT_FORCE_MIND,Type.BUTTON));
			actions.add(new ActionInput("force_grip","Force Grip",ControlConfig.PORT_ACT_FORCE_GRIP,Type.BUTTON));
			actions.add(new ActionInput("force_lightning","Force Lightning",ControlConfig.PORT_ACT_FORCE_LIGHT,Type.BUTTON));

		}

		if ((game == IDGame.Quake2) || (game == IDGame.Hexen2)|| (game == IDGame.RTCW))
		{
			actions.add(new ActionInput("help_comp","Show Objectives",ControlConfig.PORT_ACT_HELPCOMP,Type.BUTTON));
			actions.add(new ActionInput("inv_show","Show Inventory",ControlConfig.PORT_ACT_INVEN,Type.BUTTON));
			actions.add(new ActionInput("inv_use","Use Item",ControlConfig.PORT_ACT_INVUSE,Type.BUTTON));
			actions.add(new ActionInput("inv_next","Next Item",ControlConfig.PORT_ACT_INVNEXT,Type.BUTTON));
			actions.add(new ActionInput("inv_prev","Prev Item",ControlConfig.PORT_ACT_INVPREV,Type.BUTTON));
		}

		if (game == IDGame.JK2)
		{
			actions.add(new ActionInput("help_comp","Show Data Pad",ControlConfig.PORT_ACT_DATAPAD,Type.BUTTON));
			actions.add(new ActionInput("inv_use","Use Item",ControlConfig.PORT_ACT_INVUSE,Type.BUTTON));
			actions.add(new ActionInput("inv_next","Next Item",ControlConfig.PORT_ACT_INVNEXT,Type.BUTTON));
			actions.add(new ActionInput("inv_prev","Prev Item",ControlConfig.PORT_ACT_INVPREV,Type.BUTTON));
		}

		if (game == IDGame.Hexen)
		{
			actions.add(new ActionInput("inv_use","Use Item",ControlConfig.PORT_ACT_INVUSE,Type.BUTTON));
			actions.add(new ActionInput("inv_next","Next Item",ControlConfig.PORT_ACT_INVNEXT,Type.BUTTON));
			actions.add(new ActionInput("inv_prev","Prev Item",ControlConfig.PORT_ACT_INVPREV,Type.BUTTON));
			actions.add(new ActionInput("fly_up","Fly Up",ControlConfig.PORT_ACT_FLY_UP,Type.BUTTON));
			actions.add(new ActionInput("fly_down","Fly Down",ControlConfig.PORT_ACT_FLY_DOWN,Type.BUTTON));

		}

		if (game == IDGame.Strife)
		{
			actions.add(new ActionInput("inv_use","Use Item",ControlConfig.PORT_ACT_INVUSE,Type.BUTTON));
			actions.add(new ActionInput("inv_drop","Drop Item",ControlConfig.PORT_ACT_INVDROP,Type.BUTTON));
			actions.add(new ActionInput("inv_next","Next Item",ControlConfig.PORT_ACT_INVNEXT,Type.BUTTON));
			actions.add(new ActionInput("inv_prev","Prev Item",ControlConfig.PORT_ACT_INVPREV,Type.BUTTON));
			actions.add(new ActionInput("show_weap","Show Stats/Weapons",ControlConfig.PORT_ACT_SHOW_WEAPONS,Type.BUTTON));
			actions.add(new ActionInput("show_keys","Show Keys",ControlConfig.PORT_ACT_SHOW_KEYS,Type.BUTTON));

		}

		if (game == IDGame.Heretic)
		{
			actions.add(new ActionInput("inv_use","Use Item",ControlConfig.PORT_ACT_INVUSE,Type.BUTTON));
			actions.add(new ActionInput("inv_next","Next Item",ControlConfig.PORT_ACT_INVNEXT,Type.BUTTON));
			actions.add(new ActionInput("inv_prev","Prev Item",ControlConfig.PORT_ACT_INVPREV,Type.BUTTON));
			actions.add(new ActionInput("fly_up","Fly Up",ControlConfig.PORT_ACT_FLY_UP,Type.BUTTON));
			actions.add(new ActionInput("fly_down","Fly Down",ControlConfig.PORT_ACT_FLY_DOWN,Type.BUTTON));
		}

		if (game == IDGame.Quake3)
		{
			actions.add(new ActionInput("inv_use","Use Item",ControlConfig.PORT_ACT_USE,Type.BUTTON));
		}

		if (game == IDGame.Doom)
		{
			actions.add(new ActionInput("map_show","Show Automap",ControlConfig.PORT_ACT_MAP,Type.BUTTON));
			actions.add(new ActionInput("map_up","Automap Up",ControlConfig.PORT_ACT_MAP_UP,Type.BUTTON));
			actions.add(new ActionInput("map_down","Automap Down",ControlConfig.PORT_ACT_MAP_DOWN,Type.BUTTON));
			actions.add(new ActionInput("map_left","Automap Left",ControlConfig.PORT_ACT_MAP_LEFT,Type.BUTTON));
			actions.add(new ActionInput("map_right","Automap Right",ControlConfig.PORT_ACT_MAP_RIGHT,Type.BUTTON));
			actions.add(new ActionInput("map_zoomin","Automap Zoomin",ControlConfig.PORT_ACT_MAP_ZOOM_IN,Type.BUTTON));
			actions.add(new ActionInput("map_zoomout","Automap Zoomout",ControlConfig.PORT_ACT_MAP_ZOOM_OUT,Type.BUTTON));
		}

		if ((game == IDGame.RTCW) || (game == IDGame.JK2) || (game == IDGame.JK3))
		{
			actions.add(new ActionInput("quick_save","Quick Save",ControlConfig.PORT_ACT_QUICKSAVE,Type.BUTTON));
			actions.add(new ActionInput("quick_load","Quick Load",ControlConfig.PORT_ACT_QUICKLOAD,Type.BUTTON));
		}

		if ((game == IDGame.Doom) || (game == IDGame.Heretic)  || (game == IDGame.Hexen)
				|| (game == IDGame.Strife)|| (game == IDGame.Quake)|| (game == IDGame.Quake2)
				|| (game == IDGame.Hexen2)
				|| (game == IDGame.JK2)  || (game == IDGame.JK3) || (game == IDGame.TestPlatform))
		{
			actions.add(new ActionInput("menu_up","Menu Up",ControlConfig.MENU_UP,Type.MENU));
			actions.add(new ActionInput("menu_down","Menu Down",ControlConfig.MENU_DOWN,Type.MENU));
			actions.add(new ActionInput("menu_left","Menu Left",ControlConfig.MENU_LEFT,Type.MENU));
			actions.add(new ActionInput("menu_right","Menu Right",ControlConfig.MENU_RIGHT,Type.MENU));
			actions.add(new ActionInput("menu_select","Menu Select",ControlConfig.MENU_SELECT,Type.MENU));
			actions.add(new ActionInput("menu_back","Menu Back",ControlConfig.MENU_BACK,Type.MENU));
		}

		return actions;
	}
}
