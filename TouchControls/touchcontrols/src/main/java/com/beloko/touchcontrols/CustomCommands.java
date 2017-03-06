package com.beloko.touchcontrols;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.mobeta.android.dslv.DragSortListView;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;

public class CustomCommands {
	static String LOG = "QuakeCustomCommands";
	static Activity activity;
	static ControlInterface quakeIf;

	static String mainCmdsPath;
	static String modCmdsPath;

	ArrayList<QuickCommand> commands;
	QuickCommandsAdapter adapter;

	LinearLayout editView;
	EditText nameEditText;
	EditText commandEditText;


	DragSortListView listView;

	enum QuickCmdList {MAIN,MOD};

	static QuickCmdList currentList = QuickCmdList.MAIN;

	public static void setup(Activity a,ControlInterface qif,String main, String mod)
	{
		activity = a;
		quakeIf = qif;
		mainCmdsPath = main;
		modCmdsPath = mod;
		if (TouchSettings.DEBUG) Log.d(LOG,"main = " + main + ", mod = " + mod);
	}

	CustomCommands()
	{	
		loadQuickCommands(currentList);

		final Dialog dialog = new Dialog(activity);
		dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
		dialog.setContentView(R.layout.quick_commands);
		dialog.setCancelable(true);

		ImageView add = (ImageView)dialog.findViewById(R.id.add_quick_command_image);

		add.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				editView.setVisibility(View.VISIBLE);

			}
		});

		Button main = (Button)dialog.findViewById(R.id.main_button);
		main.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				loadQuickCommands(QuickCmdList.MAIN);
				adapter.notifyDataSetChanged();
			}
		});

		Button mod = (Button)dialog.findViewById(R.id.mod_button);
		mod.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				loadQuickCommands(QuickCmdList.MOD);
				adapter.notifyDataSetChanged();
			}
		});

		if (modCmdsPath == null)
		{
			mod.setVisibility(View.GONE);
		}

		editView = (LinearLayout)dialog.findViewById(R.id.edit_qc_view);
		nameEditText = (EditText)dialog.findViewById(R.id.name_edittext);
		commandEditText =  (EditText)dialog.findViewById(R.id.command_edittext);

		Button cancel = (Button)dialog.findViewById(R.id.cancel_button);
		cancel.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				editView.setVisibility(View.GONE);
			}
		});

		Button save = (Button)dialog.findViewById(R.id.save_button);
		save.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				QuickCommand qc = new QuickCommand(nameEditText.getText().toString(),commandEditText.getText().toString());
				commands.add(qc);
				saveQuickCommands();
				nameEditText.setText("");
				commandEditText.setText("");

				editView.setVisibility(View.GONE);
			}
		});


		listView =  (DragSortListView)dialog.findViewById(R.id.list);
		listView.setDragEnabled(true);
		listView.setDropListener(onDrop);

		adapter = new QuickCommandsAdapter(activity);

		listView.setAdapter(adapter);

		listView.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int pos,
					long arg3) {
				quakeIf.quickCommand_if(commands.get(pos).getCommand());
				dialog.dismiss();
			}
		});

		listView.setOnItemLongClickListener(new OnItemLongClickListener() {

			@Override
			public boolean onItemLongClick(AdapterView<?> arg0, View arg1,
					final int pos, long arg3) {
				AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(
						activity);

				// set title
				alertDialogBuilder.setTitle("Delete Command?");

				// set dialog message
				alertDialogBuilder
				.setCancelable(false)
				.setPositiveButton("Yes",new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog,int id) {
						commands.remove(pos);
						saveQuickCommands();
					}
				})
				.setNegativeButton("No",new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog,int id) {
						dialog.cancel();
					}
				});

				// create alert dialog
				AlertDialog alertDialog = alertDialogBuilder.create();

				// show it
				alertDialog.show();
				return true;
			}
		});

		adapter.notifyDataSetChanged();

		dialog.show();
	}


	public static void showCommands()
	{
		if (TouchSettings.DEBUG) Log.d(LOG,"showCommands");
		new CustomCommands();
	}

	private DragSortListView.DropListener onDrop =
			new DragSortListView.DropListener() {
		@Override
		public void drop(int from, int to) {
			if (TouchSettings.DEBUG) Log.d(LOG,"drop " + from + " to " + to);
			if (from != to) {
				//Collections.swap(commands, from, to);
				QuickCommand f = commands.remove(from);
				commands.add(to, f);

				saveQuickCommands();
			}
		}
	};

	private void loadQuickCommands(QuickCmdList m)
	{
		currentList = m;
		String filename;
		if (currentList == QuickCmdList.MAIN)
		{
			filename = mainCmdsPath;
		}
		else
		{
			if (modCmdsPath == null)
			{
				filename = mainCmdsPath;
				currentList = QuickCmdList.MAIN;
			}
			else
				filename = modCmdsPath;
		}

		FileInputStream fis = null;
		ObjectInputStream in = null;
		try
		{
			fis = new FileInputStream(filename);
			in = new ObjectInputStream(fis);
			commands = (ArrayList<QuickCommand> )in.readObject();
			if (TouchSettings.DEBUG) Log.d(LOG,"Read commands");
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
		commands = new ArrayList<QuickCommand> ();
	}

	private void saveQuickCommands()
	{
		String filename;
		if (currentList == QuickCmdList.MAIN)
		{
			filename = mainCmdsPath;
		}
		else
		{
			filename = modCmdsPath;
		}


		FileOutputStream fos = null;
		ObjectOutputStream out = null;
		try
		{
			fos = new FileOutputStream(filename);
			out = new ObjectOutputStream(fos);
			out.writeObject(commands);
			out.close();
		}
		catch(IOException ex)
		{
			Toast.makeText(activity,"Error saving commands " + ex.toString(), Toast.LENGTH_LONG).show();
		}
		adapter.notifyDataSetChanged();
	}


	class QuickCommandsAdapter extends BaseAdapter{
		private Activity context;

		public QuickCommandsAdapter(Activity context){
			this.context=context;

		}
		public void add(String string){

		}
		public int getCount() {
			return commands.size();
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
			View view = activity.getLayoutInflater().inflate(R.layout.quick_command_listview_item, null);
			ImageView image = (ImageView)view.findViewById(R.id.imageView);
			TextView title = (TextView)view.findViewById(R.id.title_textview);
			TextView command = (TextView)view.findViewById(R.id.command_textview);
			title.setText(commands.get(position).getTitle());
			command.setText(commands.get(position).getCommand());
			return view;
		}

	}

	
	
	
}
