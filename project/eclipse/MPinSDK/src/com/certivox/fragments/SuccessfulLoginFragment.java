package com.certivox.fragments;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TextView;

import com.certivox.interfaces.MPinController;
import com.example.mpinsdk.R;

public class SuccessfulLoginFragment extends Fragment {
	private MPinController mMpinController;

	private View mView;
	private ImageButton mLogoutButton;
	private TextView mUserEmailTextView;

	public void setController(MPinController controller) {
		mMpinController = controller;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.successful_login_layout, container,
				false);

		mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
		if (mMpinController.getCurrentUser() != null) {
			mUserEmailTextView
					.setText(mMpinController.getCurrentUser().getId());
		}

		mLogoutButton = (ImageButton) mView.findViewById(R.id.logout_button);
		mLogoutButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				getActivity().onBackPressed();
			}
		});
		return mView;
	}

	@Override
	public void onResume() {
		mMpinController.setTooblarTitle(R.string.account_summary);
		super.onResume();
	}
}
