package com.beloko.opengames;


import android.content.Context;
import android.media.MediaScannerConnection;
import android.media.MediaScannerConnection.MediaScannerConnectionClient;
import android.net.Uri;
import android.util.Log;

import java.io.File;

public class SingleMediaScanner implements MediaScannerConnectionClient {

	private MediaScannerConnection mMs;
	private String mFile;
	private boolean path;
	
	public SingleMediaScanner(Context context,boolean path, String f) {
		if (com.beloko.opengames.GD.DEBUG) Log.d("SingleMediaScanner","path = " + path + ", f = " + f);
		mFile = f;
		mMs = new MediaScannerConnection(context, this);
		mMs.connect();
	}

	@Override
	public void onMediaScannerConnected() {
		if (path)
		{
			File p = new File(mFile);
			File[] files = p.listFiles();
			if (files != null)
			{
				for (File f: files)
				{
					mMs.scanFile(f.getAbsolutePath(), null);
				}
			}
		}
		else
			mMs.scanFile(mFile, null);
	}

	@Override
	public void onScanCompleted(String path, Uri uri) {
		mMs.disconnect();
	}

}
