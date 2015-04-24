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

public class ConfirmEmailFragment extends Fragment {

	private MPinController mMpinController;
	private View mView;
	private TextView mUserEmailTextView;
	private TextView mInfoTextView;
	private Button mEmailConfirmedButton;
	private Button mResendMailButton;
	private Button mBackButton;

	public void setController(MPinController controller) {
		mMpinController = controller;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.confirm_email_layout, container,
				false);

		initViews();
		return mView;
	}

	@Override
	public void onResume() {
		mMpinController.disableContextToolbar();
		mMpinController.setTooblarTitle(R.string.confirm_email_title);
		super.onResume();
	}

	private void initViews() {
		mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
		mUserEmailTextView.setText(mMpinController.getCurrentUser().getId());

		mInfoTextView = (TextView) mView.findViewById(R.id.info_text);
		mInfoTextView.setText(String.format(
				getResources().getString(R.string.confirm_new_identitiy),
				mMpinController.getCurrentUser().getId()));
		mEmailConfirmedButton = (Button) mView
				.findViewById(R.id.email_confirmed_button);
		mResendMailButton = (Button) mView
				.findViewById(R.id.resend_email_button);
		mBackButton = (Button) mView.findViewById(R.id.back_button);

		mEmailConfirmedButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				mMpinController.emailConfirmed();
			}
		});

		mResendMailButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				mMpinController.resendEmail();
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
