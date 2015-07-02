package com.certivox.fragments;

import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.certivox.controllers.MPinController;
import com.example.mpinsdk.R;

public class IdentityCreatedFragment extends MPinFragment implements
		OnClickListener {

	private TextView mInfoTextView;
	private View mView;
	private TextView mUserEmailTextView;
	private Button mSignInButton;
	private Button mBackButton;

	@Override
	public boolean handleMessage(Message msg) {
		return false;
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
	protected void initViews() {
		setTooblarTitle(R.string.identity_created_title);
		mInfoTextView = (TextView) mView.findViewById(R.id.info_text);
		mInfoTextView.setText(String.format(
				getResources().getString(R.string.identity_created),
				getMPinController().getCurrentUser().getId()));
		mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
		mUserEmailTextView
				.setText(getMPinController().getCurrentUser().getId());

		mSignInButton = (Button) mView.findViewById(R.id.sign_in_button);
		mBackButton = (Button) mView.findViewById(R.id.back_button);

		mSignInButton.setOnClickListener(this);
		mBackButton.setOnClickListener(this);

	}

	@Override
	public void setData(Object data) {

	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.sign_in_button:
			getMPinController()
					.handleMessage(MPinController.MESSAGE_ON_SIGN_IN);
			break;
		case R.id.back_button:
			getMPinController().handleMessage(
					MPinController.MESSAGE_ON_SHOW_IDENTITY_LIST);
			break;
		default:
			break;
		}
	}
}