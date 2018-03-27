package net.nullsum.doom;


import android.content.Context;
import android.media.MediaScannerConnection;
import android.media.MediaScannerConnection.MediaScannerConnectionClient;
import android.net.Uri;
import android.util.Log;

class SingleMediaScanner implements MediaScannerConnectionClient {

    private final MediaScannerConnection mMs;
    private final String mFile;

    public SingleMediaScanner(Context context, String f) {
        Log.d("SingleMediaScanner", "path = " + false + ", f = " + f);
        mFile = f;
        mMs = new MediaScannerConnection(context, this);
        mMs.connect();
    }

    @Override
    public void onMediaScannerConnected() {
        mMs.scanFile(mFile, null);
    }

    @Override
    public void onScanCompleted(String path, Uri uri) {
        mMs.disconnect();
    }

}
