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
}
