package com.beloko.doom;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.Fragment;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.beloko.opengames.AboutDialog;
import com.beloko.opengames.AppSettings;
import com.beloko.opengames.GD;
import com.beloko.opengames.Utils;
import com.beloko.opengames.prboom.Game;
import com.beloko.opengames.prboom.NativeLib;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Iterator;


public class LaunchFragment extends Fragment{                           
                  
	String LOG = "LaunchFragment";           
	                                                  
	TextView gameArgsTextView;  
	EditText argsEditText;   
	ListView    listview; 
	LinearLayout  render_gl_layout;  
	LinearLayout render_soft_layout;
	TextView copyWadsTextView;    
                              
	GamesListAdapter listAdapter;          
            
	ArrayList<DoomWad> games = new ArrayList<DoomWad>(); 
	DoomWad selectedMod = null;
 
	String demoBaseDir;   
	String fullBaseDir;      

	ArrayList<String> argsHistory;
      
	int renderer;
  
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	
		AppSettings.setGame(GD.IDGame.Doom);
		demoBaseDir = AppSettings.getQuakeDemoDir();
		fullBaseDir = AppSettings.getQuakeFullDir();

		AppSettings.createDirectories(getActivity());

		loadArgs();
	}                                           


	@Override
	public void onHiddenChanged(boolean hidden) {
		if (GD.DEBUG) Log.d(LOG,"onHiddenChanged");
		demoBaseDir = AppSettings.getQuakeDemoDir();
		fullBaseDir = AppSettings.getQuakeFullDir();
		
		if (gameArgsTextView == null) //rare device call onHiddenchange before the view is created, detect this to prevent crash
			return;
		
		refreshGames();
		super.onHiddenChanged(hidden);
	}


	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View mainView = inflater.inflate(R.layout.fragment_launch, null);

		render_gl_layout = (LinearLayout)mainView.findViewById(R.id.render_gl_linearlayout);
		render_soft_layout = (LinearLayout)mainView.findViewById(R.id.render_soft_linearlayout);

		argsEditText = (EditText)mainView.findViewById(R.id.extra_args_edittext);
		gameArgsTextView = (TextView)mainView.findViewById(R.id.extra_args_textview);
		listview = (ListView)mainView.findViewById(R.id.listView);

//		listview.setBackgroundDrawable(new BitmapDrawable(getResources(),decodeSampledBitmapFromResource(getResources(),R.drawable.doom_face,200,100)));

		copyWadsTextView = (TextView)mainView.findViewById(R.id.copy_wads_textview);

		listAdapter = new GamesListAdapter(getActivity());
		listview.setAdapter(listAdapter);

		render_gl_layout.setClickable(true);
		render_gl_layout.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {

				selectRender(NativeLib.REND_GL);
			}
		});

		render_soft_layout.setClickable(true);
		render_soft_layout.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {

				selectRender(NativeLib.REND_SOFT);
			}
		});
		listview.setOnItemClickListener(new OnItemClickListener() {          

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int pos,
					long arg3) {
				selectGame(pos); 
			} 
		});

		Button startdemo = (Button)mainView.findViewById(R.id.start_demo);
		startdemo.setOnClickListener(new OnClickListener() {

			@Override 
			public void onClick(View v) {

				final Dialog dialog = new Dialog(getActivity());
				dialog.setContentView(R.layout.freedoom_dialog_view);
				dialog.setTitle("Select Freedoom episode");
				dialog.setCancelable(true); 
				
				Button credit = (Button) dialog.findViewById(R.id.freedoom_credits);
				credit.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						AboutDialog.show(getActivity(), R.raw.changes, R.raw.about);
					}
				});
				
				Button button = (Button) dialog.findViewById(R.id.freedoom1_button);
				button.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						if (Utils.checkFiles(demoBaseDir, new String[]{"freedoom1.wad"}) != null)
						{
							Utils.showDownloadDialog(getActivity(), "Download Freedoom Phase 1? (8MB)", 
									"", demoBaseDir, "freedoom1.zip");
						}
						else
						{
							selectGame(-1);
							startPrBoom(demoBaseDir," -iwad freedoom1.wad");
						}
					}
				});
				

				Button button2 = (Button) dialog.findViewById(R.id.freedoom2_button);
				button2.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						if (Utils.checkFiles(demoBaseDir,new String[] {"freedoom2.wad"}) != null)
						{
							Utils.showDownloadDialog(getActivity(), "Download Freedoom Phase 2? (11MB)", 
									"", demoBaseDir, "freedoom2.zip");
						}
						else
						{
							selectGame(-1);
							startPrBoom(demoBaseDir," -iwad freedoom2.wad");
						}
					}
				});
				
				dialog.show();
			}
		});
		Button startfull = (Button)mainView.findViewById(R.id.start_full);
		startfull.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {

				if (selectedMod == null)
				{
					AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
					builder.setMessage("Please select an IWAD file. Copy IWAD files to: " + fullBaseDir)
					.setCancelable(true)
					.setPositiveButton("OK", new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int id) {

						}
					});

					AlertDialog alert = builder.create();
					alert.show();

				}
				else
					startPrBoom(fullBaseDir,null);
			}
		});
		Button wad_button = (Button)mainView.findViewById(R.id.start_wads);
		wad_button.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				new ModSelectDialog(getActivity(), fullBaseDir,true){
					public void resultResult(String result) 
					{
						argsEditText.setText(result);	
					}
				};
			}
		});


		ImageView history = (ImageView)mainView.findViewById(R.id.args_history_imageview);
		history.setOnClickListener(new View.OnClickListener() {
			//@Override
			public void onClick(View v) {

				String[] servers = new String[ argsHistory.size()];
				for (int n=0;n<argsHistory.size();n++) servers[n] = argsHistory.get(n);

				AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
				builder.setTitle("Extra Args History");
				builder.setItems(servers, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						argsEditText.setText(argsHistory.get(which));
					}
				});
				builder.show();
			}        
		});

		int r = AppSettings.getIntOption(getActivity(), "last_renderer", NativeLib.REND_GL);
		selectRender(r);
		  
		return mainView;
	}


	void startPrBoom(String base,String moreArgs)
	{
		//Check prboom wad exists
		File prboomwad = new File(base + "/prboom-plus.wad"  );
		if (!prboomwad.exists())
		{
			Utils.copyAsset(getActivity(),"prboom-plus.wad",base);
		}

		File timiditycfg = new File(AppSettings.getBaseDir() + "/eawpats/timidity.cfg"  );
		File doomtimiditycfg = new File(AppSettings.getGameDir() + "/timidity.cfg"  );
 
		if (timiditycfg.exists() && !doomtimiditycfg.exists())
		{
			Log.d(LOG,"Copying timidity file");  
			try {
				Utils.copyFile(new FileInputStream(timiditycfg),new FileOutputStream(doomtimiditycfg));
			} catch (FileNotFoundException e) {
				Toast.makeText(getActivity(),"Error copying timidity.cfg " + e.toString(), Toast.LENGTH_LONG).show();
			} catch (IOException e) {
				Toast.makeText(getActivity(),"Error copying timidity.cfg " + e.toString(), Toast.LENGTH_LONG).show();
			}
		} 

		String extraArgs = argsEditText.getText().toString().trim();
    
		if (extraArgs.length() > 0)
		{
			Iterator<String> it = argsHistory.iterator();
			while (it.hasNext()) {
				String s = it.next();
				if (s.contentEquals(extraArgs))
					it.remove();
			}

			while (argsHistory.size()>50)  
				argsHistory.remove(argsHistory.size()-1);

			argsHistory.add(0, extraArgs);
			saveArgs();  
		} 
		
		AppSettings.setStringOption(getActivity(), "last_tab", "prboom");
		        
		String args =  gameArgsTextView.getText().toString() + " " + argsEditText.getText().toString();
 
		//Intent intent = new Intent(getActivity(), FTEDroidActivity.class);
		Intent intent = new Intent(getActivity(), Game.class);
		//Intent intent = new Intent(getActivity(), Server.class);
		intent.setAction(Intent.ACTION_MAIN);
		intent.addCategory(Intent.CATEGORY_LAUNCHER);


		intent.putExtra("doom_path",base);                         

		intent.putExtra("renderer", renderer);                 

		if (moreArgs != null)
			args = args + " " + moreArgs;
		
		intent.putExtra("args",args + " -devparm");                                                
		startActivity(intent);
	}                 

	private void selectRender(int r){
		renderer = r;
		AppSettings.setIntOption(getActivity(), "last_renderer", r);

		if (r == NativeLib.REND_GL)
		{
			render_soft_layout.setBackgroundResource(0);
			render_gl_layout.setBackgroundResource(R.drawable.layout_sel_background);
		}
		else 
		{
			render_gl_layout.setBackgroundResource(0);
			render_soft_layout.setBackgroundResource(R.drawable.layout_sel_background);
		}
	}

	void loadArgs()                      
	{ 
		File cacheDir = getActivity().getFilesDir();

		FileInputStream fis = null;
		ObjectInputStream in = null;
		try
		{
			fis = new FileInputStream(new File(cacheDir,"args_hist.dat"));
			in = new ObjectInputStream(fis);                
			argsHistory = (ArrayList<String>)in.readObject();
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
		argsHistory = new ArrayList<String>();
	}


	void saveArgs()
	{
		File cacheDir =  getActivity().getFilesDir();

		if (!cacheDir.exists())
			cacheDir.mkdirs();

		FileOutputStream fos = null;
		ObjectOutputStream out = null;
		try
		{
			fos = new FileOutputStream(new File(cacheDir,"args_hist.dat"));
			out = new ObjectOutputStream(fos);
			out.writeObject(argsHistory);
			out.close();
		}
		catch(IOException ex)         
		{
			Toast.makeText(getActivity(),"Error saving args History list: " + ex.toString(), Toast.LENGTH_LONG).show();
		}
	}              

	private void selectGame(int pos)
	{
		
		if ((pos == -1) || (pos >= games.size()))
		{
			selectedMod = null;
			gameArgsTextView.setText("");
			return;
		}

		DoomWad game =  games.get(pos);

		for (DoomWad g: games)
			g.selected = false;

		selectedMod = game;

		game.selected = true;

		gameArgsTextView.setText(game.getArgs());
		
		AppSettings.setIntOption(getActivity(), "last_iwad", pos);
		
		listAdapter.notifyDataSetChanged();
	}

	private void refreshGames()
	{
		games.clear();

		File files[] = new File(fullBaseDir).listFiles();

		if (files != null)
		{
			for(File f: files)
			{
				if (!f.isDirectory())
				{
					String file = f.getName().toLowerCase();
					if (file.endsWith(".wad") && !file.contentEquals("prboom-plus.wad"))
					{
						DoomWad game = new DoomWad(file, file);
						game.setArgs("-iwad " + file);
						game.setImage(DoomWad.getGameImage(file));
						
						games.add(game);			
					} 
				}
			}
		}
		if (games.size() > 0)
		{
			copyWadsTextView.setVisibility(View.GONE);  
		}
		else
			copyWadsTextView.setText("Copy your WAD files to here: \n" + fullBaseDir);

		if (listAdapter != null)
			listAdapter.notifyDataSetChanged();
		
		selectGame(AppSettings.getIntOption(getActivity(), "last_iwad", -1));
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
	class GamesListAdapter extends BaseAdapter{

		public GamesListAdapter(Activity context){

		}
		public void add(String string){

		}
		public int getCount() {
			return games.size();
		}

		public Object getItem(int arg0) {
			// TODO Auto-generated method stub
			return null;
		} 

		public long getItemId(int arg0) {
			// TODO Auto-generated method stub
			return 0;
		}
     
		public View getView (int position, View convertView, ViewGroup list)  {

			View view; 

			if (convertView == null)
				view = getActivity().getLayoutInflater().inflate(R.layout.games_listview_item, null);
			else
				view = convertView;

			ImageView iv = (ImageView)view.findViewById(R.id.imageview);
			DoomWad game = games.get(position);

			if (game.selected)
				view.setBackgroundResource(R.drawable.layout_sel_background);
			else
				view.setBackgroundResource(0);

			//iv.setImageResource(game.getImage());
			iv.setImageBitmap(decodeSampledBitmapFromResource(getResources(),game.getImage(),200,100));
			TextView title = (TextView)view.findViewById(R.id.title_textview);

			title.setText(game.getTitle());
			return view;
		} 
	}
}
