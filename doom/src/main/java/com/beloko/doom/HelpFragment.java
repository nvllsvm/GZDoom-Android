package com.beloko.doom;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Fragment;
import android.content.DialogInterface;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;

import com.beloko.opengames.AppSettings;
import com.beloko.opengames.ServerAPI;

public class HelpFragment extends Fragment{
	String LOG = "HelpFragment";



	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View mainView = inflater.inflate(R.layout.fragment_help, null);

		Button downloadFonts = (Button)mainView.findViewById(R.id.download_fonts_button);


		downloadFonts.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
				builder.setMessage("Download Music Sound Fonts(31MB)?")
				.setCancelable(true)
				.setPositiveButton("OK", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
						//utils.downloadData("http://www.beloko.com/QUAKE/quake_demo.zip", demoBaseDir);
						ServerAPI.downloadFile(getActivity(), "eawpats.zip", AppSettings.getBaseDir());
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
		});

		EditText soundBuff  = (EditText)mainView.findViewById(R.id.sound_buff_editText);
		int soundBuffSize = AppSettings.getIntOption(getActivity(), "sound_buff_size", 1024);
		soundBuff.setText(Integer.toString(soundBuffSize));

		soundBuff.addTextChangedListener(new TextWatcher(){
			public void afterTextChanged(Editable s) {
				String t = s.toString();
				Log.d("TEST","t = " + t);
				
				if (t.length() > 2)
				{
					try{
						int b = Integer.parseInt(t);
						if (b > 100)
							AppSettings.setIntOption(getActivity(),  "sound_buff_size", b);
					}
					catch (Exception e)
					{
						
					}
				}
			}
			public void beforeTextChanged(CharSequence s, int start, int count, int after){}
			public void onTextChanged(CharSequence s, int start, int before, int count){}
		}); 
		return mainView;
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

}
