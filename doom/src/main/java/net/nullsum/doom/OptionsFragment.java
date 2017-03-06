package net.nullsum.doom;

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

import net.nullsum.doom.GD.IDGame;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public class OptionsFragment extends Fragment{
	String LOG = "OptionsFragment";

	TextView basePathTextView;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);


	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View mainView = inflater.inflate(R.layout.fragment_options, null);

		basePathTextView = (TextView)mainView.findViewById(R.id.base_path_textview);

		basePathTextView.setText(AppSettings.belokoBaseDir);

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

		return mainView;
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
