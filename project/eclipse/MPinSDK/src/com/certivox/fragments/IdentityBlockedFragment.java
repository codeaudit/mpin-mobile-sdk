package com.certivox.fragments;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import com.certivox.interfaces.MPinController;
import com.example.mpinsdk.R;

public class IdentityBlockedFragment extends Fragment {

	private MPinController mMpinController;
	private View mView;

	private TextView mUserEmailTextView;
	private Button mRemoveIdentityButton;
	private Button mResetPinButton;
	private Button mBackButton;

	public void setController(MPinController controller) {
		mMpinController = controller;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.identity_blocked_layout, container,
				false);

		initViews();
		return mView;
	}

	@Override
	public void onResume() {
		mMpinController.disableContextToolbar();
		mMpinController.setTooblarTitle(R.string.identity_blocked_title);
		super.onResume();
	}

	private void initViews() {
		mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
		mUserEmailTextView.setText(mMpinController.getCurrentUser().getId());
		mRemoveIdentityButton = (Button) mView
				.findViewById(R.id.remove_identity_button);
		mResetPinButton = (Button) mView.findViewById(R.id.reset_pin_button);
		mBackButton = (Button) mView.findViewById(R.id.back_button);

		mRemoveIdentityButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				mMpinController.deleteCurrentUser();
			}
		});

		mResetPinButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				mMpinController.reRegisterUser(mMpinController.getCurrentUser());
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
