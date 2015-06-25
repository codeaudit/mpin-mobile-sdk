package com.certivox.fragments;

import android.app.AlertDialog;
import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.certivox.controllers.MPinController;
import com.example.mpinsdk.R;

public class ConfirmEmailFragment extends MPinFragment implements
		OnClickListener {

	private View mView;
	private TextView mUserEmailTextView;
	private TextView mInfoTextView;
	private Button mEmailConfirmedButton;
	private Button mResendMailButton;
	private Button mBackButton;

	@Override
	public boolean handleMessage(Message msg) {
		switch (msg.what) {
		case MPinController.MESSAGE_EMAIL_NOT_CONFIRMED:
			showEmailNotConfirmedDialog();
			return true;
		case MPinController.MESSAGE_EMAIL_SENT:
			Toast.makeText(getActivity(), "Email sent", Toast.LENGTH_LONG)
					.show();
			return true;
		default:
			return false;
		}
	}

	@Override
	public void setData(Object data) {

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
	protected void initViews() {
		setTooblarTitle(R.string.confirm_email_title);
		mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
		mUserEmailTextView
				.setText(getMPinController().getCurrentUser().getId());

		mInfoTextView = (TextView) mView.findViewById(R.id.info_text);
		mInfoTextView.setText(String.format(
				getResources().getString(R.string.confirm_new_identitiy),
				getMPinController().getCurrentUser().getId()));
		mEmailConfirmedButton = (Button) mView
				.findViewById(R.id.email_confirmed_button);
		mResendMailButton = (Button) mView
				.findViewById(R.id.resend_email_button);
		mBackButton = (Button) mView.findViewById(R.id.back_button);

		mEmailConfirmedButton.setOnClickListener(this);
		mResendMailButton.setOnClickListener(this);
		mBackButton.setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.email_confirmed_button:
			getMPinController().handleMessage(
					MPinController.MESSAGE_EMAIL_CONFIRMED);
			break;
		case R.id.resend_email_button:
			getMPinController().handleMessage(
					MPinController.MESSAGE_RESEND_EMAIL);
			break;
		case R.id.back_button:
			getMPinController().handleMessage(
					MPinController.MESSAGE_GO_BACK_REQUEST);
			break;
		default:
			break;
		}
	}

	private void showEmailNotConfirmedDialog() {
		new AlertDialog.Builder(getActivity())
				.setTitle("Email not confirmed")
				.setMessage(
						"Please, click the link in the email, to confirm your identity and proceed.")
				.setPositiveButton("OK", null).show();
	}
}
