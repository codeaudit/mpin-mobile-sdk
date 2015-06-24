package com.certivox.fragments;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import com.certivox.adapters.ConfigurationListAdapter;
import com.example.mpinsdk.R;

public class ConfigurationsListFragment extends MPinFragment implements
		Handler.Callback {

	private String TAG = ConfigurationsListFragment.class.getCanonicalName();

	// View
	private ListView mView;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setHasOptionsMenu(true);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = (ListView) inflater.inflate(
				R.layout.fragment_configurations_list, container, false);

		mView.setAdapter(new ConfigurationListAdapter(getActivity()
				.getApplicationContext(), getMPinController()
				.getConfigurationsList(), getMPinController()
				.getActiveConfigurationId()));

		return mView;
	}

	@Override
	public boolean handleMessage(Message msg) {
		// TODO Auto-generated method stub
		return false;
	}
}
