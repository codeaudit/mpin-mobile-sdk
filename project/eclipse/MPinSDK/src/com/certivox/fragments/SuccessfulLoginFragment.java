package com.certivox.fragments;

import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TextView;

import com.certivox.controllers.MPinController;
import com.example.mpinsdk.R;

public class SuccessfulLoginFragment extends MPinFragment {
	private View mView;
	private ImageButton mLogoutButton;
	private TextView mUserEmailTextView;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.successful_login_layout, container,
				false);

		mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
		if (getMPinController().getCurrentUser() != null) {
			mUserEmailTextView.setText(getMPinController().getCurrentUser()
					.getId());
		}

		mLogoutButton = (ImageButton) mView.findViewById(R.id.logout_button);
		mLogoutButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				getMPinController().handleMessage(
						MPinController.MESSAGE_GO_BACK_REQUEST);
			}
		});

		initViews();

		return mView;
	}

	@Override
	public boolean handleMessage(Message msg) {
		return false;
	}

	@Override
	protected void initViews() {
		setTooblarTitle(R.string.account_summary);
	}

	@Override
	public void setData(Object data) {

	}
}
