package com.beloko.opengames;

import android.app.Dialog;
import android.content.Context;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class NoticeDialog {

	public static void show(final Context ctx,String title, String text)
	{
		final Dialog dialog = new Dialog(ctx);
		dialog.setContentView(R.layout.notice);
		dialog.setTitle(title);
		dialog.setCancelable(true);
		//there are a lot of settings, for dialog, check them all out!

		final TextView textView = (TextView) dialog.findViewById(R.id.textView1);
		textView.setText(text);

		//set up button
		Button button = (Button) dialog.findViewById(R.id.button1);
		button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				dialog.dismiss();    
			}
		});

		//now that the dialog is set up, it's time to show it    
		dialog.show();
		
	}
	
	public static void show(final Context ctx,String title, int textid)
	{
		show(ctx,title, readTxt(ctx,textid));
	}


	private static String readTxt(Context ctx, int id){

		InputStream inputStream = ctx.getResources().openRawResource(id);

		ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();

		int i;
		try {
			i = inputStream.read();
			while (i != -1)
			{
				byteArrayOutputStream.write(i);
				i = inputStream.read();
			}
			inputStream.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return byteArrayOutputStream.toString();
	}

}
