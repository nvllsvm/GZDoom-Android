package com.beloko.opengames;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.AsyncTask;
import android.util.Log;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.net.URLConnection;
import java.net.URLEncoder;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class ServerAPI {
	static final String LOG = "ServerAPI";


	static Activity ctx;

	public static void downloadFile(Activity ctx,String file,String path)
	{
		ServerAPI.ctx = ctx;
		new DLFileThread().execute(file,path);
	}


	static ProgressDialog checkLicPd = null;
	static Object checkLicPdLock = new Object();
	static String file,path;


	static private class DLFileThread extends AsyncTask<String, Integer, Long> {

		private ProgressDialog progressBar;
		String errorstring= null;

		@Override
		protected void onPreExecute() {
			progressBar = new ProgressDialog(ctx); 
			progressBar.setMessage("Downloading/Extracting files..");
			progressBar.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
			progressBar.setCancelable(false);
			progressBar.show();
		}

		int getTotalZipSize(String file)
		{
			int ret = 0;
			try {
				ZipFile zf = new ZipFile(file);
				Enumeration e = zf.entries();
				while (e.hasMoreElements()) {
					ZipEntry ze = (ZipEntry) e.nextElement();
					String name = ze.getName();

					ret += ze.getSize();
					long compressedSize = ze.getCompressedSize();


				}
			} catch (IOException ex) {
				System.err.println(ex);
			}
			return ret;
		}
		
		protected Long doInBackground(String... info) {

			String file = info[0];
			String basePath = info[1];

			boolean isLocal = false;

			progressBar.setProgress(0);

			File outZipFile = new File(basePath,"temp.zip");

			try
			{
				String challenge = "";

				try {

					challenge = URLEncoder.encode(AES256Cipher.AES_Encode(getVerifCode(), BelokoSecure.server_key));
				} catch (Exception e )
				{
					if (GD.DEBUG) Log.d(LOG,"Error encrypting: " + e.toString());
					errorstring = e.toString();
					return 1l;
				}

				String url_full = "http://beloko.com/quake_api/api.php?verif=" + challenge +
						"&download=" + file;

				if (GD.DEBUG) Log.d(LOG,url_full);

				HttpClient httpclient = new DefaultHttpClient();


				HttpPost httppost = new HttpPost(url_full);

				HttpResponse httpResponse = null;
				httpResponse = httpclient.execute(httppost);

				int code = httpResponse.getStatusLine().getStatusCode();

				if (GD.DEBUG) Log.d(LOG,"code = " + code);
				if (GD.DEBUG)Log.d(LOG,"reason = " +httpResponse.getStatusLine().getReasonPhrase());

				if (code != 200)
				{
					errorstring = httpResponse.getStatusLine().getReasonPhrase();
					return 1L;
				}

				int dlSize = (int)httpResponse.getEntity().getContentLength();

				BufferedInputStream in = null;
				FileOutputStream fout = null;
				InputStream ins = httpResponse.getEntity().getContent();

				progressBar.setMax(dlSize);
				if (GD.DEBUG) Log.d(LOG,"File size = " + dlSize);

				in = new BufferedInputStream(ins);
				fout = new FileOutputStream(outZipFile);

				byte data[] = new byte[1024];
				int count;
				while ((count = in.read(data, 0, 1024)) != -1)
				{
					fout.write(data, 0, count);
					progressBar.setProgress(progressBar.getProgress() + count);
				}
				in.close();
				fout.close();
				//AppSettings.setBoolOption(ctx,"DLF" + ServerAPI.file, true);

			} catch (IOException e) {
				errorstring = e.toString();
				return 1l;
			}

			
			if (!file.endsWith(".zip")) //If not a zip file, then just rename
			{
				Log.d(LOG,"Not a zip file, renaming only");
				if (errorstring == null)
					outZipFile.renameTo(new File(basePath , file));
				else
					Log.d(LOG,"Not renaming file as error downloading: " + errorstring);
				return 0l;
			}
			
			InputStream in = null;
			OutputStream out = null;

			try {
				ZipFile zipFile = new ZipFile(outZipFile);

				progressBar.setMax(getTotalZipSize(outZipFile.getPath()));
				progressBar.setProgress(0);

				Enumeration entries = zipFile.entries();
				while(entries.hasMoreElements()) {
					ZipEntry entry = (ZipEntry)entries.nextElement();
					if(entry.isDirectory()) {
						// Assume directories are stored parents first then children.
						System.err.println("Extracting directory: " + entry.getName());
						// This is not robust, just for demonstration purposes.
						(new File(basePath, entry.getName())).mkdirs();
						continue;
					}
					if (GD.DEBUG) Log.d(LOG,"Extracting file: " + entry.getName());
					(new File(basePath, entry.getName())).getParentFile().mkdirs();
					in = zipFile.getInputStream(entry);
					out =  new FileOutputStream(new File(basePath, entry.getName()));
					Utils.copyFile(in, out, progressBar);
				}
				//zipFile.close();
			} catch (IOException ioe) {
				if (GD.DEBUG) Log.d(LOG, "Error reading zip " + out + " - " + ioe.toString());
				outZipFile.delete();
				errorstring = ioe.toString();
				return 1l;
			}


			outZipFile.delete();

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
		}
	}

	static public byte[] fetchChallengeCode() throws IOException
	{
		BufferedInputStream in = null;

		URL url = new URL("http://www.beloko.com/quake_api/challenge_key.txt");
		URLConnection con;
		con = url.openConnection();
		con.setConnectTimeout(4000);
		con.setReadTimeout(8000);
		InputStream ins = con.getInputStream();

		in = new BufferedInputStream(ins);

		byte data[] = new byte[16];

		int len = in.read(data, 0, 16) ;
		if (len != 16)
			throw new IOException("Challenge code not 16 (" + len + ")");

		return data;
	}


	static public byte[] getVerifCode() throws IOException
	{
		byte[] code = new byte[16 + 16 + 8];

		byte[] challenge = fetchChallengeCode();


		for (int n=0;n<16;n++)
			code[n] = challenge[n];

		//code[3] = 3;
		int version = 0;
		PackageInfo pInfo;
		try {
			pInfo = ctx.getPackageManager().getPackageInfo(ctx.getPackageName(), 0);
			version = pInfo.versionCode;
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		for (int n=0;n<16;n++)
			code[n + 16] = (byte)n;

		code[32] = (byte)version;
		code[33] = 0;
		code[34] = (byte)AppSettings.game.ordinal();


		return code;
	}
}
