package com.beloko.doom;


import android.app.Activity;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnDismissListener;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.Spinner;

import com.beloko.opengames.AboutDialog;
import com.beloko.opengames.AppSettings;

import java.util.ArrayList;
import java.util.List;

public class GzdoomOptionsDialog {
	String basePath;
	String extraPath = "";

	boolean PrBoomMode;

	ArrayList<String> filesArray = new ArrayList<String>();

	ArrayList<String> selectedArray = new ArrayList<String>();

	final Dialog dialog;
	Activity activity;
	CheckBox useDevCheckBox;


	GzdoomOptionsDialog(final Activity act,String path, boolean prboomMode)
	{
		basePath = path;
		activity = act;
		PrBoomMode = prboomMode;

		dialog = new Dialog(activity);
		dialog.setContentView(R.layout.dialog_gzdoom_options);
		dialog.setTitle("GZDoom Options");
		dialog.setCancelable(true);
		dialog.setOnDismissListener(new OnDismissListener() {

			@Override
			public void onDismiss(DialogInterface dialog) {
				resultResult();
			}
		});

		useDevCheckBox = (CheckBox)dialog.findViewById(R.id.use_gzdoom_dev_checkBox);
		boolean dev = AppSettings.getBoolOption(act, "gzdoom_dev", false);
		useDevCheckBox.setChecked(dev);
		useDevCheckBox.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
				AppSettings.setBoolOption(act, "gzdoom_dev", isChecked);
			}
		});

		Spinner resSpinnder = (Spinner)dialog.findViewById(R.id.resolution_div_spinner);
		List<String> list = new ArrayList<String>();
		list.add("1");
		list.add("2");
		list.add("3");
		list.add("4");
		list.add("5");
		list.add("6");
		list.add("7"); 
		list.add("8");
		
		ArrayAdapter<String> dataAdapter = new ArrayAdapter<String>(act, android.R.layout.simple_spinner_item,list);

		dataAdapter.setDropDownViewResource	(android.R.layout.simple_spinner_dropdown_item);
		
		resSpinnder.setAdapter(dataAdapter);
		resSpinnder.setOnItemSelectedListener(new OnItemSelectedListener() {

			@Override
			public void onItemSelected(AdapterView<?> parent, View view,
					int position, long id) {
				AppSettings.setIntOption(act, "gzdoom_res_div", position+1);
			}

			@Override
			public void onNothingSelected(AdapterView<?> parent) {
				// TODO Auto-generated method stub
				
			}
		});
		
		int selected = AppSettings.getIntOption(act,  "gzdoom_res_div", 1);
		resSpinnder.setSelection(selected-1);
		
		Button fluidDownload =  (Button)dialog.findViewById(R.id.fludisynth_button);
		fluidDownload.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				downloadFluidSynth();
			}
		});
		
		Button credits =  (Button)dialog.findViewById(R.id.fludisynth_license_button);
		credits.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				AboutDialog.show(act, R.raw.changes, R.raw.about);
			}
		});
		
		
		dialog.show();
	}

	public void resultResult() 
	{
		//override me 	
	}

	public void downloadFluidSynth()
	{
		//override me 	
	}
}
