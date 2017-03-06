package net.nullsum.doom;

import android.app.Fragment;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;

public class OnlineFragment extends Fragment{
	String LOG = "OnlineFragment";


	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);


	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View mainView = inflater.inflate(R.layout.fragment_online, null);

		return mainView;
	}

	@Override
	public void onHiddenChanged(boolean hidden) {

		super.onHiddenChanged(hidden);
	}




}
