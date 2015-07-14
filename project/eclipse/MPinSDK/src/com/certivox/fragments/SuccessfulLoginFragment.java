package com.certivox.fragments;

import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TextView;

import com.certivox.constants.FragmentTags;
import com.certivox.controllers.MPinController;
import com.example.mpinsdk.R;

public class SuccessfulLoginFragment extends MPinFragment implements
		OnClickListener {
	private View mView;
	private ImageButton mLogoutButton;
	private TextView mUserEmailTextView;

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.fragment_successful_login, container,
				false);

		mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
		if (getMPinController().getCurrentUser() != null) {
			mUserEmailTextView.setText(getMPinController().getCurrentUser()
					.getId());
		}
		mLogoutButton = (ImageButton) mView.findViewById(R.id.logout_button);
		mLogoutButton.setOnClickListener(this);

		initViews();

		return mView;
	}

	@Override
	protected OnClickListener getDrawerBackClickListener() {
		return null;
	}

	@Override
	protected String getFragmentTag() {
		return FragmentTags.FRAGMENT_SUCCESSFUL_LOGIN;
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

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.logout_button:
			getMPinController().handleMessage(
					MPinController.MESSAGE_ON_SHOW_IDENTITY_LIST);
			return;
		}
	}
}
