package net.nullsum.doom;



import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;

import android.app.Activity;
import android.app.Dialog;
import android.content.DialogInterface;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

public class ModSelectDialog {
	String basePath;
	String extraPath = "";

	boolean PrBoomMode;

	ArrayList<String> filesArray = new ArrayList<String>();

	ArrayList<String> selectedArray = new ArrayList<String>();

	final Dialog dialog;
	Activity activity;
	ListView listView;
	TextView resultTextView;
	TextView infoTextView;

	ModsListAdapter listAdapter;

	ModSelectDialog(Activity act,String path, boolean prboomMode)
	{
		basePath = path;
		activity = act;
		PrBoomMode = prboomMode;

		dialog = new Dialog(activity);
		dialog.setContentView(R.layout.dialog_select_mods_wads);
		dialog.setTitle("Touch Control Sensitivity Settings");
		dialog.setCancelable(true);

		dialog.setOnKeyListener(new Dialog.OnKeyListener() {

			@Override
			public boolean onKey(DialogInterface arg0, int keyCode,
					KeyEvent event) {
				// TODO Auto-generated method stub
				if (keyCode == KeyEvent.KEYCODE_BACK) {
					if (extraPath.isEmpty() || !extraPath.contains("/"))
						return false;
					else
					{
						extraPath = extraPath.substring(0, extraPath.lastIndexOf("/"));
						populateList(extraPath);
						return true;
					}
				}
				return false;
			}
		});

		resultTextView = (TextView)dialog.findViewById(R.id.result_textView);
		infoTextView = (TextView)dialog.findViewById(R.id.info_textView);

		listView = (ListView)dialog.findViewById(R.id.listview);
		listAdapter = new ModsListAdapter(activity);
		listView.setAdapter(listAdapter);

		listView.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {

				if (filesArray.get(position).startsWith("/"))
				{
					populateList(extraPath + filesArray.get(position));
				}
				else //select/deselect
				{
					boolean removed = false;
					for (Iterator<String> iter = selectedArray.listIterator(); iter.hasNext(); ) {
						String s = iter.next();
						if (s.contentEquals(extraPath + "/" + filesArray.get(position)))
						{
							iter.remove();
							removed = true;
						}
					}

					if (!removed)
						selectedArray.add(extraPath + "/" + filesArray.get(position));

					//	Log.d("TEST","list size = " + selectedArray.size());

					listAdapter.notifyDataSetChanged();
					resultTextView.setText(getResult());

				}
			}
		});

		//Add folders on long press
		listView.setOnItemLongClickListener(new OnItemLongClickListener() {

			@Override
			public boolean onItemLongClick(AdapterView<?> parent, View view,
					int position, long id) {
				if (filesArray.get(position).startsWith("/"))
				{
					boolean removed = false;
					String name = filesArray.get(position).substring(1, filesArray.get(position).length());
					for (Iterator<String> iter = selectedArray.listIterator(); iter.hasNext(); ) {
						String s = iter.next();
						if (s.contentEquals(extraPath + "/" + name))
						{
							iter.remove();
							removed = true;
						}
					}

					if (!removed)
						selectedArray.add(extraPath + "/" + name);

					//	Log.d("TEST","list size = " + selectedArray.size());

					listAdapter.notifyDataSetChanged();
					resultTextView.setText(getResult());
					return true;
				}
				return false;
			}
		});

		Button wads_button = (Button)dialog.findViewById(R.id.wads_button);
		wads_button.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				populateList("wads");
			}
		});

		Button mods_button = (Button)dialog.findViewById(R.id.mods_button);
		mods_button.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				populateList("mods");
			}
		});

		Button ok_button = (Button)dialog.findViewById(R.id.ok_button);
		ok_button.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				dialog.dismiss();
				resultResult(getResult()); 
			}
		});

		populateList("wads");


		dialog.show();
	}

	public void resultResult(String result)
	{
 
	}

	public String getResult()
	{
		String result = "";

		if (PrBoomMode && selectedArray.size() > 0)
			result ="-file";

		for (int n=0;n<selectedArray.size();n++)
		{
			if (PrBoomMode)
				result += " " + selectedArray.get(n);
			else
			{
				if ((selectedArray.get(n).endsWith(".deh")) || (selectedArray.get(n).endsWith(".bex")))
					result += "-deh " + selectedArray.get(n) + " ";
				else
					result += "-file " + selectedArray.get(n) + " ";
			}
		}

		return result;
	}


	private void populateList(String path)
	{
		extraPath = path;
		dialog.setTitle(extraPath);
		String wad_dir = basePath + "/" + path;
		File files[] = new File(wad_dir).listFiles();
		filesArray.clear();
		if (files != null)
			for(File f: files)
			{
				if (!f.isDirectory())
				{
					String file = f.getName().toLowerCase();
					if (file.endsWith(".wad") || file.endsWith(".pk3") || file.endsWith(".pk7") || file.endsWith(".deh"))
					{
						filesArray.add(f.getName());
					}
				}
				else //Now also do directories
				{
					filesArray.add("/" + f.getName());
				}
			}

		Collections.sort(filesArray);

		if (filesArray.size() == 0)
			infoTextView.setText("Please copy wad/mods to here: \"" + basePath + "/" + path + "\"");
		else
			infoTextView.setText("");

		listAdapter.notifyDataSetChanged();
		resultTextView.setText(getResult());
	}

	class ModsListAdapter extends BaseAdapter{

		public ModsListAdapter(Activity context){

		}
		public void add(String string){

		}
		public int getCount() {
			return filesArray.size();
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
				view = activity.getLayoutInflater().inflate(R.layout.listview_item_mods_wads, null);
			else
				view = convertView;

			boolean selected = false;
			for (String s : selectedArray)
			{
				if (s.contentEquals(extraPath + "/" + filesArray.get(position)))
				{
					selected = true;
				}
			}


			if (selected)
				view.setBackgroundResource(R.drawable.layout_sel_background);
			else
				view.setBackgroundResource(0);

			ImageView iv = (ImageView)view.findViewById(R.id.imageview);

			//iv.setImageResource(game.getImage());

/*			if (filesArray.get(position).startsWith("/"))
				iv.setImageResource(R.drawable.file_folder);
			else if (filesArray.get(position).endsWith(".pk3"))
				iv.setImageResource(R.drawable.file_zip);
			else if (filesArray.get(position).endsWith(".pk7"))
				iv.setImageResource(R.drawable.file_zip);
			else
				iv.setImageResource(R.drawable.file_unknown);
                */


			TextView title = (TextView)view.findViewById(R.id.name_textview);

			title.setText(filesArray.get(position));
			return view;
		}

	}

}
