package com.certivox.fragments;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.mpinsdk.R;

public class ManageConfigurationsFragment extends Fragment {

	private View mView;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.manage_configurations_layout,
				container, false);

		return mView;
	}
}