package com.certivox.fragments;

import android.app.AlertDialog;
import android.content.DialogInterface;
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

public class IdentityBlockedFragment extends MPinFragment implements
		OnClickListener {

	private View mView;

	private TextView mUserEmailTextView;
	private Button mRemoveIdentityButton;
	private Button mResetPinButton;
	private Button mBackButton;

	@Override
	public boolean handleMessage(Message msg) {
		return false;
	}

	@Override
	public void setData(Object data) {

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
	protected void initViews() {
		setTooblarTitle(R.string.identity_blocked_title);
		mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
		mUserEmailTextView
				.setText(getMPinController().getCurrentUser().getId());
		mRemoveIdentityButton = (Button) mView
				.findViewById(R.id.remove_identity_button);
		mResetPinButton = (Button) mView.findViewById(R.id.reset_pin_button);
		mBackButton = (Button) mView.findViewById(R.id.back_button);

		mRemoveIdentityButton.setOnClickListener(this);
		mResetPinButton.setOnClickListener(this);
		mBackButton.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.remove_identity_button:
			onDeleteIdentity();
			break;
		case R.id.reset_pin_button:
			getMPinController().handleMessage(MPinController.MESSAGE_RESET_PIN);
			break;
		case R.id.back_button:
			getMPinController().handleMessage(
					MPinController.MESSAGE_ON_SHOW_IDENTITY_LIST);
			break;
		default:
			return;
		}

	}

	private void onDeleteIdentity() {
		new AlertDialog.Builder(getActivity())
				.setTitle("Delete user")
				.setMessage(
						"Do you want to delete user "
								+ getMPinController().getCurrentUser().getId()
								+ "?")
				.setPositiveButton("Delete",
						new DialogInterface.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog,
									int which) {
								getMPinController()
										.handleMessage(
												MPinController.MESSAGE_ON_DELETE_IDENTITY);
							}
						}).setNegativeButton("Cancel", null).show();
	}
}
