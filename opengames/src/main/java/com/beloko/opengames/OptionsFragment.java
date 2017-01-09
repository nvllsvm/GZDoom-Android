package com.beloko.opengames;

import android.annotation.TargetApi;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Fragment;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Spinner;
import android.widget.TextView;

import com.beloko.opengames.GD.IDGame;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public class OptionsFragment extends Fragment{
	String LOG = "OptionsFragment";

	TextView basePathTextView;
	TextView musicPathTextView;
	TextView fpsTextView;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);


	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View mainView = inflater.inflate(R.layout.fragment_options, null);

		LinearLayout quakeExtra =  (LinearLayout)mainView.findViewById(R.id.quake_extra_layout);

		if ((AppSettings.game == IDGame.Doom) || (AppSettings.game == IDGame.Quake3)|| (AppSettings.game == IDGame.RTCW)
				|| (AppSettings.game == IDGame.Wolf3d)|| (AppSettings.game == IDGame.JK2)|| (AppSettings.game == IDGame.JK3)
				|| (AppSettings.game == IDGame.Hexen)|| (AppSettings.game == IDGame.Strife)|| (AppSettings.game == IDGame.Heretic)) //If doom, hide the music and other options, now alos Q3!
			quakeExtra.setVisibility(View.GONE);

		//Immersion mode for KitKat or above
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
			LinearLayout l =  (LinearLayout)mainView.findViewById(R.id.immersion_mode_layout);
			l.setVisibility(View.VISIBLE);

			CheckBox cb = (CheckBox)mainView.findViewById(R.id.immersion_mode_checkbox);
			cb.setChecked(AppSettings.immersionMode);

			cb.setOnCheckedChangeListener(new OnCheckedChangeListener() {

				@Override
				public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
					AppSettings.immersionMode = isChecked;
					AppSettings.setBoolOption(getActivity(), "immersion_mode", AppSettings.immersionMode);
				}
			});
		}


		basePathTextView = (TextView)mainView.findViewById(R.id.base_path_textview);
		musicPathTextView = (TextView)mainView.findViewById(R.id.music_path_textview);
		fpsTextView  = (TextView)mainView.findViewById(R.id.fps_textview);

		basePathTextView.setText(AppSettings.belokoBaseDir);
		musicPathTextView.setText(AppSettings.musicBaseDir);

		Button chooseDir = (Button)mainView.findViewById(R.id.choose_base_button);
		chooseDir.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				DirectoryChooserDialog directoryChooserDialog = 
						new DirectoryChooserDialog(getActivity(), 
								new DirectoryChooserDialog.ChosenDirectoryListener() 
						{
							@Override
							public void onChosenDir(String chosenDir) 
							{
								updateBaseDir(chosenDir);
							}
						}); 

				directoryChooserDialog.chooseDirectory(AppSettings.belokoBaseDir);
			}
		});


		Button resetDir = (Button)mainView.findViewById(R.id.reset_base_button);
		resetDir.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				AppSettings.resetBaseDir(getActivity());
				updateBaseDir(AppSettings.belokoBaseDir);
			}
		});



		Button sdcardDir = (Button)mainView.findViewById(R.id.sdcard_base_button);
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
			sdcardDir.setOnClickListener(new OnClickListener() {

				@TargetApi(Build.VERSION_CODES.KITKAT)
				@Override
				public void onClick(View v) {
					File[] files =getActivity().getExternalFilesDirs(null);
					
					if ((files.length < 2) || (files[1] == null))
					{
						showError("Can not find an external SD Card, is the card inserted?");
						return;
					}
					
					final String path = files[1].toString();
					
					AlertDialog.Builder dialogBuilder = new Builder(getActivity());
					dialogBuilder.setTitle("WARNING");
					dialogBuilder.setMessage("This will use the special location on the external SD Card which can be written to by this app, Android will DELETE this"
							+ " area when you uninstall the app and you will LOSE YOUR SAVEGAMES and game data!");
					dialogBuilder.setPositiveButton("OK", new android.content.DialogInterface.OnClickListener() 
					{
						@Override
						public void onClick(DialogInterface dialog, int which) 
						{
							updateBaseDir(path);
						}
					});
					dialogBuilder.setNegativeButton("Cancel",  new android.content.DialogInterface.OnClickListener() 
					{
						@Override
						public void onClick(DialogInterface dialog, int which) 
						{
						
						}
					});
					
					final AlertDialog errdialog = dialogBuilder.create();
					errdialog.show();
					
				}
			});
		}
		else
		{
			sdcardDir.setVisibility(View.GONE);
		}


		Button chooseMusicDir = (Button)mainView.findViewById(R.id.choose_music_button);
		chooseMusicDir.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				DirectoryChooserDialog directoryChooserDialog = 
						new DirectoryChooserDialog(getActivity(), 
								new DirectoryChooserDialog.ChosenDirectoryListener() 
						{
							@Override
							public void onChosenDir(String chosenDir) 
							{
								updateMusicDir(chosenDir);
							}
						}); 

				directoryChooserDialog.chooseDirectory(AppSettings.musicBaseDir);
			}
		});

		CheckBox vibrate = (CheckBox)mainView.findViewById(R.id.enable_vibrate_checkbox);
		vibrate.setChecked(AppSettings.vibrate);

		vibrate.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				AppSettings.vibrate = isChecked;
				AppSettings.setBoolOption(getActivity(), "vibrate", AppSettings.vibrate);
			}
		});

		SeekBar sb = (SeekBar)mainView.findViewById(R.id.fps_seekBar);
		sb.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub

			}

			@Override
			public void onStartTrackingTouch(SeekBar seekBar) {
				// TODO Auto-generated method stub

			}

			@Override
			public void onProgressChanged(SeekBar seekBar, int progress,
					boolean fromUser) {
				AppSettings.setIntOption(getActivity(), "max_fps", progress);
				updateFPS();
			}
		});
		sb.setProgress(AppSettings.getIntOption(getActivity(), "max_fps", 0));
		updateFPS();

		if (AppSettings.game == IDGame.Quake2)
		{
			LinearLayout quake2Extra =  (LinearLayout)mainView.findViewById(R.id.quake2_extra_layout);
			quake2Extra.setVisibility(View.VISIBLE);
			Spinner spinner = (Spinner) mainView.findViewById(R.id.quake2_hud_size_spinner);
			spinner.setSelection(AppSettings.getIntOption(getActivity(), "quake2_hud_size", 0));

			spinner.setOnItemSelectedListener(new OnItemSelectedListener() {

				@Override
				public void onItemSelected(AdapterView<?> arg0, View arg1,
						int pos, long arg3) {
					AppSettings.setIntOption(getActivity(), "quake2_hud_size", pos);
				}

				@Override
				public void onNothingSelected(AdapterView<?> arg0) {
					// TODO Auto-generated method stub

				}
			});
		}

		LinearLayout eglExtra =  (LinearLayout)mainView.findViewById(R.id.choose_egl_layout);
		if ((AppSettings.game == IDGame.RTCW) || (AppSettings.game == IDGame.JK2))
		{
			eglExtra.setVisibility(View.VISIBLE);
		}


		Button saveLogcat = (Button)mainView.findViewById(R.id.save_logcat_button);
		saveLogcat.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {

				new SimpleServerAccess(getActivity(),"http://beloko.com/quake_api/check_ver.php?pkg=" + getActivity().getPackageName())
				{
					void returnData(ByteArrayOutputStream data)
					{
						String ver = data.toString();
						int ver_int = 0;
						try{
							ver_int = Integer.parseInt(ver);
						}
						catch (Exception e){

						}
						if (GD.version < ver_int)
						{
							AlertDialog.Builder builder = new AlertDialog.Builder(ctx);
							builder.setMessage("Please first update app from Amazon to access support")
							.setCancelable(true)
							.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {

								}
							});

							builder.show();
						}
						else
						{
							SendDebugEmail();
						}
					}
				};
			}
		});
		return mainView;
	}

	@Override
	public void onHiddenChanged(boolean hidden) {
		updateEGLSpinner();
		super.onHiddenChanged(hidden);
	}

	private void updateEGLSpinner()
	{

		if (getView() == null)
			return;

		String eglOptions = AppSettings.getStringOption(getActivity(), "egl_configs", "Please launch game to populate");
		String[] items = eglOptions.split(",");

		int number = items.length;
		Spinner eglSpinnter = (Spinner)getView().findViewById(R.id.choose_egl_spinner);
		eglSpinnter.setAdapter(new ArrayAdapter<String> (getActivity(),android.R.layout.simple_dropdown_item_1line,items));

		int override = AppSettings.getIntOption(getActivity(), "egl_config_selected", 0);
		if (override < number)
			eglSpinnter.setSelection(override);


		eglSpinnter.setOnItemSelectedListener(new OnItemSelectedListener() {

			@Override
			public void onItemSelected(AdapterView<?> arg0, View arg1,
					int pos, long arg3) {
				AppSettings.setIntOption(getActivity(), "egl_config_selected", pos);
			}

			@Override
			public void onNothingSelected(AdapterView<?> arg0) {
				// TODO Auto-generated method stub

			}
		});
	}


	private void updateBaseDir(String dir)
	{
		File fdir = new File(dir);

		if (!fdir.isDirectory())
		{
			showError(dir + " is not a directory");
			return;
		}

		if (!fdir.canWrite())
		{
			showError(dir + " is not a writable");
			return;
		}


		//Test CAN actually write, the above canWrite can pass on KitKat SD cards WTF GOOGLE
		File test_write = new File (dir,"test_write");
		try {
			test_write.createNewFile();
			if (!test_write.exists())
			{
				showError(dir + " is not a writable");
				return;
			}
		} catch (IOException e) {
			showError(dir + " is not a writable");
			return;
		}
		test_write.delete();


		if (dir.contains(" "))
		{
			showError(dir + " must not contain any spaces");
			return;
		}


		AppSettings.belokoBaseDir = dir;
		AppSettings.setStringOption(getActivity(), "base_path", AppSettings.belokoBaseDir);
		AppSettings.createDirectories(getActivity());

		basePathTextView.setText(AppSettings.belokoBaseDir);
	}

	private void updateMusicDir(String dir)
	{
		File fdir = new File(dir);

		if (!fdir.isDirectory())
		{
			showError(dir + " is not a directory");
			return;
		}


		AppSettings.musicBaseDir = dir;
		AppSettings.setStringOption(getActivity(), "music_path", AppSettings.musicBaseDir);
		musicPathTextView.setText(AppSettings.musicBaseDir);
		CDAudioPlayer.initFiles(AppSettings.musicBaseDir);
	}

	private void updateFPS()
	{
		int fps = AppSettings.getIntOption(getActivity(), "max_fps", 0);
		if (fps == 0)
			fpsTextView.setText("No Limit");
		else
			fpsTextView.setText(fps + " FPS");
	}

	private void showError(String error)
	{
		AlertDialog.Builder dialogBuilder = new Builder(getActivity());
		dialogBuilder.setTitle(error);
		dialogBuilder.setPositiveButton("OK", new android.content.DialogInterface.OnClickListener() 
		{
			@Override
			public void onClick(DialogInterface dialog, int which) 
			{

			}
		});

		final AlertDialog errdialog = dialogBuilder.create();
		errdialog.show();
	}

	private void SendDebugEmail()
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
		builder.setMessage("Are you sure you want to email the debug log?\nIf yes, please give good information about the problem.\nPLEASE DO NOT send a log if the app was not downloaded from Amazon.")
		.setCancelable(true)
		.setPositiveButton("SEND EMAIL", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int id) {
				// TODO Auto-generated method stub
				PrintWriter printWriter = null;
				try {
					String filename = AppSettings.getBaseDir() + "/" +  AppSettings.game.toString() + "_logcat.txt";
					printWriter = new PrintWriter(new FileWriter(filename),true);

					String log = Utils.getLogCat();

					printWriter.print(log);

					printWriter.close();


					final Intent emailIntent = new Intent( android.content.Intent.ACTION_SEND);
					emailIntent.setType("plain/text");

					emailIntent.putExtra(android.content.Intent.EXTRA_SUBJECT, AppSettings.game.toString() + "_" + GD.version + " Logging file");
					emailIntent.putExtra(android.content.Intent.EXTRA_EMAIL,new String[]{"support@beloko.com"});

					emailIntent.putExtra(android.content.Intent.EXTRA_TEXT,"Enter description of issue:  ");

					Uri uri = Uri.parse("file://" + filename);
					emailIntent.putExtra(Intent.EXTRA_STREAM, uri);

					getActivity().startActivity(Intent.createChooser(emailIntent, "Send mail..."));

				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

			}
		});
		builder.setNegativeButton("CANCEL", new DialogInterface.OnClickListener() {

			@Override
			public void onClick(DialogInterface dialog, int which) {
				dialog.cancel();
			}
		});                                                   
		AlertDialog alert = builder.create();
		alert.show(); 

	}
}
