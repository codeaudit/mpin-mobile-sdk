package com.certivox.fragments;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.certivox.interfaces.MPinController;
import com.example.mpinsdk.R;

public class IdentityCreatedFragment extends Fragment {

	private MPinController mMpinController;
	private TextView mInfoTextView;
	private View mView;
	private TextView mUserEmailTextView;
	private Button mSignInButton;
	private Button mBackButton;

	public void setController(MPinController controller) {
		mMpinController = controller;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.identity_created_layout, container,
				false);

		initViews();
		return mView;
	}

	@Override
	public void onResume() {
		mMpinController.setTooblarTitle(R.string.identity_created_title);
		super.onResume();
	}

	private void initViews() {
		mInfoTextView = (TextView) mView.findViewById(R.id.info_text);
		mInfoTextView.setText(String.format(
				getResources().getString(R.string.identity_created),
				mMpinController.getCurrentUser().getId()));
		mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
		mUserEmailTextView.setText(mMpinController.getCurrentUser().getId());

		mSignInButton = (Button) mView.findViewById(R.id.sign_in_button);
		mBackButton = (Button) mView.findViewById(R.id.back_button);

		mSignInButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				mMpinController.signIn();
			}
		});

		mBackButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				getActivity().onBackPressed();
			}
		});
	}
}
