package com.certivox.fragments;

import android.app.Fragment;
import android.os.Bundle;
import android.text.InputType;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;

import com.certivox.interfaces.MPinController;
import com.example.mpinsdk.R;

public class AccessNumberFragment extends Fragment {

	private MPinController mMpinController;
	private View mView;

	private TextView mUserEmail;
	private TextView mDigit0;
	private TextView mDigit1;
	private TextView mDigit2;
	private TextView mDigit3;
	private TextView mDigit4;
	private TextView mDigit5;
	private TextView mDigit6;
	private TextView mDigit7;
	private TextView mDigit8;
	private TextView mDigit9;
	private ImageButton mButtonLogin;
	private ImageButton mButtonClear;
	private EditText mPinEditText;

	private OnClickListener mOnDigitClickListener;
	private int mAccessNumberLength = 7;
	private final StringBuilder mInput = new StringBuilder();

	public void setController(MPinController controller) {
		mMpinController = controller;
	}

	public void setAccessNumberLength(int length) {
		mAccessNumberLength = length;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.access_number_activity, container,
				false);

		initViews();
		initAccessNumber();
		return mView;
	}

	@Override
	public void onResume() {
		mMpinController.disableContextToolbar();
		mMpinController.setTooblarTitle(R.string.access_number_title);
		super.onResume();
	}

	private void initViews() {
		mPinEditText = (EditText) mView.findViewById(R.id.pinpad_input);
		mPinEditText.setRawInputType(InputType.TYPE_CLASS_TEXT);
		mPinEditText.setTextIsSelectable(true);
		mPinEditText.setLongClickable(false);
		mPinEditText.setClickable(false);
		mPinEditText.requestFocus();

		mUserEmail = (TextView) mView.findViewById(R.id.user_email);
		if (mMpinController.getCurrentUser() != null) {
			mUserEmail.setText(mMpinController.getCurrentUser().getId());
		} else {
			mUserEmail.setText("");
		}

		mDigit0 = (TextView) mView.findViewById(R.id.pinpad_key_0);
		mDigit1 = (TextView) mView.findViewById(R.id.pinpad_key_1);
		mDigit2 = (TextView) mView.findViewById(R.id.pinpad_key_2);
		mDigit3 = (TextView) mView.findViewById(R.id.pinpad_key_3);
		mDigit4 = (TextView) mView.findViewById(R.id.pinpad_key_4);
		mDigit5 = (TextView) mView.findViewById(R.id.pinpad_key_5);
		mDigit6 = (TextView) mView.findViewById(R.id.pinpad_key_6);
		mDigit7 = (TextView) mView.findViewById(R.id.pinpad_key_7);
		mDigit8 = (TextView) mView.findViewById(R.id.pinpad_key_8);
		mDigit9 = (TextView) mView.findViewById(R.id.pinpad_key_9);

		mButtonLogin = (ImageButton) mView.findViewById(R.id.pinpad_key_login);
		mButtonClear = (ImageButton) mView.findViewById(R.id.pinpad_key_clear);
	}

	private OnClickListener getOnDigitClickListener() {
		if (mOnDigitClickListener == null) {
			initOnDigitClickListener();
		}

		return mOnDigitClickListener;
	}

	private void initOnDigitClickListener() {
		mOnDigitClickListener = new View.OnClickListener() {
			@Override
			public void onClick(View view) {
				if (mInput.length() >= mAccessNumberLength) {
					return;
				}
				final int id = view.getId();
				if (id == R.id.pinpad_key_0) {
					mInput.append('0');
				} else if (id == R.id.pinpad_key_1) {
					mInput.append('1');
				} else if (id == R.id.pinpad_key_2) {
					mInput.append('2');
				} else if (id == R.id.pinpad_key_3) {
					mInput.append('3');
				} else if (id == R.id.pinpad_key_4) {
					mInput.append('4');
				} else if (id == R.id.pinpad_key_5) {
					mInput.append('5');
				} else if (id == R.id.pinpad_key_6) {
					mInput.append('6');
				} else if (id == R.id.pinpad_key_7) {
					mInput.append('7');
				} else if (id == R.id.pinpad_key_8) {
					mInput.append('8');
				} else if (id == R.id.pinpad_key_9) {
					mInput.append('9');
				}
				updateInput();
			}
		};

	}

	private void setAccessNumberInput(String title) {
		mPinEditText.setText(title);
		mPinEditText.setSelection(mPinEditText.getText().length());
	}

	private String parseAccessNumber() {
		return mInput.toString();
	}

	private void initAccessNumber() {
		mInput.setLength(0);
		updateInput();
		mButtonLogin.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				synchronized (this) {
					this.notifyAll();
					String accessNumber = parseAccessNumber();
					if (mMpinController != null) {
						mMpinController.onAccessNumberEntered(accessNumber);
					}
				}
			}
		});

		mButtonClear.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				if (mInput.length() > 0) {
					mInput.setLength(mInput.length() - 1);
					updateInput();
				}
			}
		});

		mDigit0.setOnClickListener(getOnDigitClickListener());
		mDigit1.setOnClickListener(getOnDigitClickListener());
		mDigit2.setOnClickListener(getOnDigitClickListener());
		mDigit3.setOnClickListener(getOnDigitClickListener());
		mDigit4.setOnClickListener(getOnDigitClickListener());
		mDigit5.setOnClickListener(getOnDigitClickListener());
		mDigit6.setOnClickListener(getOnDigitClickListener());
		mDigit7.setOnClickListener(getOnDigitClickListener());
		mDigit8.setOnClickListener(getOnDigitClickListener());
		mDigit9.setOnClickListener(getOnDigitClickListener());
	}

	private void updateInput() {
		if (mInput.length() < 1) {
			setAccessNumberInput("");
		} else {
			String accessNumber = "";
			for (int i = 0; i < mInput.length(); ++i) {
				accessNumber += mInput.toString().charAt(i) + " ";
			}
			setAccessNumberInput(accessNumber);
		}

		mButtonLogin.setEnabled(mInput.length() == mAccessNumberLength);
		mButtonClear.setEnabled(mInput.length() > 0);
	}

}
