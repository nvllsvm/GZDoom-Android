package net.nullsum.doom;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.Fragment;
import android.content.DialogInterface;
import android.content.Intent;
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
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import net.nullsum.doom.AppSettings;
import net.nullsum.doom.Utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;

public class LaunchFragmentGZdoom extends Fragment{

    String LOG = "LaunchFragment";


    TextView gameArgsTextView = null;
    EditText argsEditText;
    ListView listview;
    TextView copyWadsTextView;

    GamesListAdapter listAdapter;

    ArrayList<DoomWad> games = new ArrayList<DoomWad>();
    DoomWad selectedMod = null;


    String fullBaseDir;

    ArrayList<String> argsHistory = new ArrayList<String>();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        fullBaseDir = AppSettings.getQuakeFullDir();

        AppSettings.createDirectories(getActivity());

        Utils.loadArgs(getActivity(), argsHistory);
    }

    @Override
    public void onHiddenChanged(boolean hidden) {
        Log.d(LOG,"onHiddenChanged");
        fullBaseDir = AppSettings.getQuakeFullDir();

        if (gameArgsTextView == null) //rare device call onHiddenchange before the view is created, detect this to prevent crash
            return;

        refreshGames();
        super.onHiddenChanged(hidden);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        View mainView = inflater.inflate(R.layout.fragment_launch_gzdoom, null);


        argsEditText = (EditText)mainView.findViewById(R.id.extra_args_edittext);
        gameArgsTextView = (TextView)mainView.findViewById(R.id.extra_args_textview);
        listview = (ListView)mainView.findViewById(R.id.listView);

        //listview.setBackgroundDrawable(new BitmapDrawable(getResources(),Utils.decodeSampledBitmapFromResource(getResources(),R.drawable.chco_doom,635,284)));
        listAdapter = new GamesListAdapter(getActivity());
        listview.setAdapter(listAdapter);

        listview.setOnItemClickListener(new OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> arg0, View arg1, int pos,
                    long arg3) {
                selectGame(pos);
            }
        });
        copyWadsTextView = (TextView)mainView.findViewById(R.id.copy_wads_textview);

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
                    startGame(fullBaseDir,false,null);
            }
        });



        Button wad_button = (Button)mainView.findViewById(R.id.start_wads);
        wad_button.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                new ModSelectDialog(getActivity(), fullBaseDir,false){
                    public void resultResult(String result)
                    {
                        argsEditText.setText(result);
                    }
                };
            }
        });

        ImageView delete_args = (ImageView)mainView.findViewById(R.id.args_delete_imageview);
        delete_args.setOnClickListener(new View.OnClickListener() {
            //@Override
            public void onClick(View v) {
                argsEditText.setText("");
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

        refreshGames();

        return mainView;
    }

    void startGame(final String base,boolean ignoreMusic,final String moreArgs)
    {
        //Check gzdoom.pk3 wad exists
        //File extrawad = new File(base + "/gzdoom.pk3"  );
        //if (!extrawad.exists())
        {
            Utils.copyAsset(getActivity(),"gzdoom.pk3",base);
            Utils.copyAsset(getActivity(),"gzdoom.sf2",base);
            //Utils.copyAsset(getActivity(),"lights_dt.pk3",base);
            //Utils.copyAsset(getActivity(),"brightmaps_dt.pk3",base);
        }

        //File[] files = new File(basePath ).listFiles();

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
            Utils.saveArgs(getActivity(),argsHistory);
        }

        AppSettings.setStringOption(getActivity(), "last_tab", "gzdoom");

        String args =  gameArgsTextView.getText().toString() + " " + argsEditText.getText().toString();

        //Intent intent = new Intent(getActivity(), Game.class);
        Intent intent = new Intent(getActivity(), net.nullsum.doom.Game.class);
        intent.setAction(Intent.ACTION_MAIN);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);

        int resDiv = AppSettings.getIntOption(getActivity(),  "gzdoom_res_div", 1);
        intent.putExtra("res_div",resDiv);

        intent.putExtra("game_path",base);
        intent.putExtra("game", "doom");

        if (moreArgs != null)
            args = args + " " + moreArgs;
        String saveDir;
        saveDir = " -savedir " + base + "/gzdoom_saves";

        String fluidSynthFile = "gzdoom.sf2";

        intent.putExtra("args",args + saveDir + " +set fluid_patchset " + fluidSynthFile + " +set midi_dmxgus 0 ");

        startActivity(intent);
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
                    Log.d(LOG,"refreshGames " + file);
                    if ((file.endsWith(".wad") || file.endsWith(".pk3")|| file.endsWith(".pk7"))
                            && !file.contentEquals("prboom-plus.wad")
                            && !file.contentEquals("gzdoom.pk3")
                            && !file.contentEquals("gzdoom_dev.pk3")
                            && !file.contentEquals("lights_dt.pk3")
                            && !file.contentEquals("brightmaps_dt.pk3")
                            && !file.contentEquals("lights.pk3")
                            && !file.contentEquals("brightmaps.pk3")
                            )
                    {
                        DoomWad game = new DoomWad(file, file);
                        game.setArgs("-iwad " + file);
                        game.setImage(DoomWad.getGameImage(file));

                        games.add(game);
                    }
                }
            }
        }

        if (listAdapter != null)
            listAdapter.notifyDataSetChanged();

        selectGame(AppSettings.getIntOption(getActivity(), "last_iwad", -1));
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
            iv.setImageBitmap(Utils.decodeSampledBitmapFromResource(getResources(),game.getImage(),200,100));
            TextView title = (TextView)view.findViewById(R.id.title_textview);

            title.setText(game.getTitle());
            return view;
        }

    }

}
