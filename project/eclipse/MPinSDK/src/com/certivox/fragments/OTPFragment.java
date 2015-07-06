package com.certivox.fragments;

import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.certivox.constants.FragmentTags;
import com.certivox.controllers.MPinController;
import com.certivox.models.OTP;
import com.example.mpinsdk.R;

public class OTPFragment extends MPinFragment {

	private View mView;
	private TextView mUserEmailTextView;
	private TextView mOTPTextView;
	private ProgressBar mOTPProgressBar;
	private TextView mTimeLeftTextView;
	private static OTP mOTP;

	@Override
	public void setData(Object otp) {
		mOTP = (OTP) otp;

	}

	@Override
	protected String getFragmentTag() {
		return FragmentTags.FRAGMENT_OTP;
	}

	@Override
	protected OnClickListener getDrawerBackClickListener() {
		return null;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.otp_layout, container, false);
		initViews();

		return mView;
	}

	private void initProgressBar() {
		final long start = System.currentTimeMillis();
		getActivity().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				int remaining = mOTP.ttlSeconds
						- (int) ((System.currentTimeMillis() - start) / 1000L);
				if (remaining < 0) {
					remaining = 0;
				}
				if (remaining > 0) {
					mTimeLeftTextView.setText(remaining + " sec");
					double prog = 1 - (System.currentTimeMillis() - start)
							/ (mOTP.ttlSeconds * 1000.0);
					mOTPProgressBar.setProgress((int) (mOTPProgressBar.getMax() * prog));
					mOTPProgressBar.postDelayed(this, 100);
				} else {
					getMPinController().handleMessage(
							MPinController.MESSAGE_OTP_EXPIRED);
				}
			}
		});
	}

	@Override
	public boolean handleMessage(Message msg) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	protected void initViews() {
		setTooblarTitle(R.string.otp_title);
		mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
		if (getMPinController().getCurrentUser() != null) {
			mUserEmailTextView.setText(getMPinController().getCurrentUser()
					.getId());
		}

		mOTPTextView = (TextView) mView.findViewById(R.id.otp);
		String otp = "";
		for (int i = 0; i < mOTP.otp.length(); i++) {
			otp += mOTP.otp.charAt(i) + " ";
		}
		mOTPTextView.setText(otp);
		mOTPProgressBar = (ProgressBar) mView.findViewById(R.id.otp_progress);
		mTimeLeftTextView = (TextView) mView.findViewById(R.id.otp_time_left);

		initProgressBar();

	}
}