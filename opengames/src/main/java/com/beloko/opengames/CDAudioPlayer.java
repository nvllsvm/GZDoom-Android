package com.beloko.opengames;

import android.media.MediaPlayer;
import android.util.Log;

import com.beloko.touchcontrols.AlphanumComparator;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;

public class CDAudioPlayer {

	static final int DROID_STOP   =0;
	static final int DROID_PLAY   =1;
	static final int DROID_PAUSE  =2;
	static final int DROID_RESUME =3;


	static MediaPlayer mPlayer;



	public static void callback(int action, int p1, int p2)
	{
		Log.d("CD","callback " + action + " "+ p1 + " " + p2);
		switch(action)
		{
		case DROID_STOP:
			if (mPlayer!=null)
			{
				mPlayer.stop();
				mPlayer.reset();
			}
			break;
		case DROID_PLAY:
			int track = p1 - 2;
			if ((track < tracks.size()) && (track >= 0))
			{
				if (mPlayer == null)
					mPlayer = new MediaPlayer();
				else
				{
					mPlayer.stop();
					mPlayer.reset();
				}
				
				try {
					mPlayer.setDataSource(tracks.get(track));
					mPlayer.setLooping(p2!=0?true:false);
					mPlayer.prepare();
				} catch (IllegalArgumentException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IllegalStateException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				Log.d("Music", "Starting");
				mPlayer.start();
			}
			break;
		case DROID_PAUSE:
			if (mPlayer!=null)
				mPlayer.pause();
			break;
		case DROID_RESUME:
			if (mPlayer!=null)
				mPlayer.start();
			break;
		}
	}

	public static boolean checkHasAudioFiles(String path)
	{
		if (path == null)
			return false;
		
		if (!(new File(path).isDirectory()))
			return false;
		
		File files[] = new File(path).listFiles(new FileFilter() {

			@Override
			public boolean accept(File pathname) {
				return pathname.toString().toLowerCase().endsWith("mp3");
			}
		});

		if (files != null)
			return true;
		else return false;
	}
	
	static ArrayList<String> tracks;
	public static void initFiles(String path)
	{
		tracks = new ArrayList<String>();

		if (path == null)
			return;
		
		if (!(new File(path).isDirectory()))
			return;
		
		File files[] = new File(path).listFiles(new FileFilter() {

			@Override
			public boolean accept(File pathname) {
				return pathname.toString().toLowerCase().endsWith("mp3");
			}
		});

		if (files != null)
		{
			for (File f: files)
			{
				tracks.add(f.toString());
				Log.d("Music", "file = " + f.toString());
			}
		}

		//Try and sort in to correct order
		Collections.sort(tracks, new AlphanumComparator());
	}

	static boolean wasPause = false;;
	public static void onPause()
	{
		if (mPlayer != null)
		{
			if (mPlayer.isPlaying())
			{
				wasPause = true;
				mPlayer.pause();
			}
		}
	}
	
	public static void onResume()
	{
		if (mPlayer != null)
		{
			if (wasPause)
			{
				mPlayer.start();
				wasPause = false;
			}
		}
	}
}
