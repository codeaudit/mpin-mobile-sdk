package com.certivox.fragments;

import android.app.Fragment;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.text.method.LinkMovementMethod;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.certivox.interfaces.MPinController;
import com.example.mpinsdk.R;

public class AboutFragment extends Fragment {

	private MPinController mMpinController;
	private View mView;
	private TextView mLinkTextView;
	private TextView mVersionTextView;
	private TextView mBuildTextView;

	public void setController(MPinController controller) {
		mMpinController = controller;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.about_layout, container, false);
		mLinkTextView = (TextView) mView
				.findViewById(R.id.terms_and_conditions_link);
		mLinkTextView.setMovementMethod(LinkMovementMethod.getInstance());
		mVersionTextView = (TextView) mView.findViewById(R.id.about_version);
		mBuildTextView = (TextView) mView.findViewById(R.id.about_build);
		setVersion();

		return mView;

	}

	@Override
	public void onResume() {
		mMpinController.disableContextToolbar();
		mMpinController.setTooblarTitle(R.string.about_title);
		super.onResume();
	}

	private void setVersion() {
		PackageInfo pInfo;
		try {
			pInfo = getActivity().getPackageManager().getPackageInfo(
					getActivity().getPackageName(), 0);

			String versionName = pInfo.versionName;
			int versionCode = pInfo.versionCode;

			mVersionTextView.setText(versionName);
			mBuildTextView.setText(versionCode + "");

		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
}
