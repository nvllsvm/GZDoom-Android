package com.beloko.opengames;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.AsyncTask;
import android.util.Log;

import com.beloko.opengames.GD;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class SimpleServerAccess {
	String LOG = "SimpleServerAccess";
	Context ctx;
	void returnData(ByteArrayOutputStream data)
	{
		//OVerride me
	}


	SimpleServerAccess(Context context,String url)
	{
		ctx = context;
		new ServerAccessThread().execute(url);
	}

	private class ServerAccessThread extends AsyncTask<String, Integer, Long> {

		private ProgressDialog progressBar;
		String errorstring= null;
		ByteArrayOutputStream data_out = new ByteArrayOutputStream();

		@Override
		protected void onPreExecute() {
			progressBar = new ProgressDialog(ctx); 
			progressBar.setMessage("Accessing Server..");
			progressBar.setProgressStyle(ProgressDialog.STYLE_SPINNER);
			progressBar.setCancelable(false);
			progressBar.show();
		}

		protected Long doInBackground(String... url) {

			String url_full = url[0];

			
			try
			{

				if (GD.DEBUG) Log.d(LOG,url_full);

				HttpClient httpclient = new DefaultHttpClient();


				HttpPost httppost = new HttpPost(url_full);

				HttpResponse httpResponse = null;
				httpResponse = httpclient.execute(httppost);

				int code = httpResponse.getStatusLine().getStatusCode();

				if (GD.DEBUG) Log.d(LOG,"code = " + code);
				if (GD.DEBUG)Log.d(LOG,"reason = " + httpResponse.getStatusLine().getReasonPhrase());

				if (code != 200)
				{
					errorstring = httpResponse.getStatusLine().getReasonPhrase();
					return 1L;
				}

				int dlSize = (int)httpResponse.getEntity().getContentLength();

				BufferedInputStream in = null;

				InputStream ins = httpResponse.getEntity().getContent();

				progressBar.setMax(dlSize);
				if (GD.DEBUG) Log.d(LOG,"File size = " + dlSize);

				in = new BufferedInputStream(ins);

				byte data[] = new byte[1024];
				int count;
				while ((count = in.read(data, 0, 1024)) != -1)
				{
					data_out.write(data, 0, count);
				}
				in.close();


			} catch (IOException e) {
				errorstring = e.toString();
				return 1l;
			}

			return 0l;
		}

		protected void onProgressUpdate(Integer... progress) {

		}

		protected void onPostExecute(Long result) {
			progressBar.dismiss();
			if (errorstring!=null)
			{
				AlertDialog.Builder builder = new AlertDialog.Builder(ctx);
				builder.setMessage("Error accessing server: " + errorstring)
				.setCancelable(true)
				.setPositiveButton("OK", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {

					}
				});

				builder.show();
			}
			else
			{
				returnData(data_out);
			}
		}
	}
}
