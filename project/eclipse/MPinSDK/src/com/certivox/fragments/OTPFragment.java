package com.certivox.fragments;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.certivox.interfaces.MPinController;
import com.certivox.models.OTP;
import com.example.mpinsdk.R;

public class OTPFragment extends Fragment {

	private MPinController mMpinController;

	private View mView;
	private TextView mUserEmailTextView;
	private TextView mOTPTextView;
	private ProgressBar mOTPProgressBar;
	private TextView mTimeLeftTextView;
	private static OTP mOTP;

	public void setOTP(OTP otp) {
		mOTP = otp;
	}

	public void setController(MPinController controller) {
		mMpinController = controller;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.otp_layout, container, false);
		initViews();

		return mView;
	}

	@Override
	public void onResume() {
		mMpinController.setTooblarTitle(R.string.otp_title);
		super.onResume();
	}

	private void initViews() {
		mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
		if (mMpinController.getCurrentUser() != null) {
			mUserEmailTextView
					.setText(mMpinController.getCurrentUser().getId());
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
					mOTPProgressBar.setProgress(0);
					mOTPTextView.setText("EXPIRED");
					mTimeLeftTextView.setText("0 sec");
				}
			}
		});
	}

}
