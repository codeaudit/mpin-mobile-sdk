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

import com.certivox.interfaces.PinPadController;
import com.certivox.models.User;
import com.certivox.mpin.R;

public class PinPadFragment extends Fragment {

	private PinPadController mMpinController;

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
	private TextView mWrongPinTextView;

	private OnClickListener mOnDigitClickListener;
	private int mPinLength = 4;
	private final StringBuilder mInput = new StringBuilder();
	private volatile boolean mIsPinSet;

	public void setController(PinPadController controller) {
		mMpinController = controller;
	}

	public void setPinLength(int length) {
		mPinLength = length;
	}

	public TextView getTitle() {
		return mPinEditText;
	}

	private void setPinInput(String input) {
		mPinEditText.setText(input);
		mPinEditText.setSelection(mPinEditText.getText().length());
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.pinpad_layout, container, false);
		initViews();
		initPinPad();
		return mView;
	}

	@Override
	public void onResume() {
		mMpinController.disableContextToolbar();
		if (mMpinController.getCurrentUser().getState()
				.equals(User.State.REGISTERED)) {
			mMpinController.setTooblarTitle(R.string.enter_pin_title);
		} else {
			mMpinController.setTooblarTitle(R.string.setup_pin_title);
		}
		super.onResume();
	}

	private void initViews() {
		mUserEmail = (TextView) mView.findViewById(R.id.user_email);
		mUserEmail.setText(mMpinController.getCurrentUser().getId());

		mPinEditText = (EditText) mView.findViewById(R.id.pinpad_input);
		mPinEditText.setRawInputType(InputType.TYPE_CLASS_TEXT);
		mPinEditText.setTextIsSelectable(true);
		mPinEditText.setLongClickable(false);
		mPinEditText.setClickable(false);
		mPinEditText.requestFocus();

		mWrongPinTextView = (TextView) mView.findViewById(R.id.wrong_pin);
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

	public void showWrongPin() {
		mIsPinSet = false;
		mInput.setLength(0);
		updateInput();
		mWrongPinTextView.setVisibility(View.VISIBLE);
	}

	public void hideWrongPin() {
		mWrongPinTextView.setVisibility(View.INVISIBLE);
	}

	private void setEmptyPin() {
		mIsPinSet = false;
		mInput.setLength(0);
		updateInput();
	}

	private void initPinPad() {
		setEmptyPin();
		mButtonLogin.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				synchronized (PinPadFragment.this) {
					if (!mIsPinSet) {
						mIsPinSet = true;
						if (mMpinController != null) {
							mMpinController.onPinEntered(parsePin());
						}
						PinPadFragment.this.notifyAll();
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
				if (mInput.length() >= mPinLength) {
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

	private String parsePin() {
		return mInput.toString();
	}

	@Override
	public void onDetach() {
		synchronized (this) {
			mInput.setLength(0);
			mIsPinSet = true;
			notifyAll();
		}

		super.onDetach();
	}

	public String getPin() {
		synchronized (this) {
			try {
				while (!mIsPinSet) {
					wait();
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		String pin = parsePin();
		return pin;
	}

	private void updateInput() {
		if (mInput.length() < 1) {
			setPinInput("");
		} else {
			hideWrongPin();
			String pin = "";
			for (int i = 0; i < mInput.length(); ++i) {
				pin += '*';
			}
			setPinInput(pin);
		}

		mButtonLogin.setEnabled(mInput.length() == mPinLength);
		mButtonClear.setEnabled(mInput.length() > 0);
	}
}
