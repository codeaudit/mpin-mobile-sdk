package com.certivox.fragments;

import android.os.Bundle;
import android.os.Message;
import android.text.InputType;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;

import com.certivox.constants.FragmentTags;
import com.example.mpinsdk.R;

public class AccessNumberFragment extends MPinFragment {

	private static final String TAG = AccessNumberFragment.class
			.getCanonicalName();

	private View mView;
	private TextView mAccessNumberTitle;

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
	private EditText mAccessNumberEditText;

	private OnClickListener mOnDigitClickListener;
	private int mAccessNumberLength;
	private final StringBuilder mInput = new StringBuilder();

	private void setAccessNumberLength() {
		mAccessNumberLength = getMPinController().getAccessNumberLength();
	}

	@Override
	protected String getFragmentTag() {
		return FragmentTags.FRAGMENT_ACCESS_NUMBER;
	}

	@Override
	protected OnClickListener getDrawerBackClickListener() {
		return null;
	}

	@Override
	public boolean handleMessage(Message msg) {
		return true;
	}

	@Override
	public void setData(Object data) {

	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		mView = inflater.inflate(R.layout.fragment_access_number, container,
				false);
		setAccessNumberLength();
		initViews();
		initAccessNumber();
		return mView;
	}

	@Override
	public void onStart() {
		super.onStart();
		// This is needed because when restarting the fragment the keyboard
		// appears automatically on some devices
		getActivity().getWindow().setSoftInputMode(
				WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN);
	}

	@Override
	protected void initViews() {
		setTooblarTitle(R.string.access_number_title);
		mAccessNumberTitle = (TextView) mView
				.findViewById(R.id.access_number_title);

		mAccessNumberTitle.setText(String.format(
				getResources().getString(R.string.access_number_text),
				mAccessNumberLength));

		mAccessNumberEditText = (EditText) mView
				.findViewById(R.id.pinpad_input);
		mAccessNumberEditText.setRawInputType(InputType.TYPE_CLASS_TEXT);
		mAccessNumberEditText.setTextIsSelectable(true);
		mAccessNumberEditText.setLongClickable(false);
		mAccessNumberEditText.setClickable(false);
		mAccessNumberEditText.requestFocus();

		mUserEmail = (TextView) mView.findViewById(R.id.user_email);
		if (getMPinController().getCurrentUser() != null) {
			mUserEmail.setText(getMPinController().getCurrentUser().getId());
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
		mAccessNumberEditText.setText(title);
		mAccessNumberEditText.setSelection(mAccessNumberEditText.getText()
				.length());
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
				String accessNumber = parseAccessNumber();
				getMPinController().onAccessNumberEntered(accessNumber);
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