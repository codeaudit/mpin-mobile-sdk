package com.certivox.fragments;

import android.app.AlertDialog;
import android.app.Fragment;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;

import com.certivox.interfaces.MPinController;
import com.example.mpinsdk.R;

public class CreateIdentityFragment extends Fragment {

	private MPinController mMpinController;
	private View mView;
	private EditText mEmailEditText;
	private Button mCreateIdentitiyButton;

	public void setController(MPinController controller) {
		mMpinController = controller;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.create_identity_layout, container, false);
		initViews();
		return mView;
	}

	private void initViews() {
		mEmailEditText = (EditText) mView.findViewById(R.id.email_input);
		mCreateIdentitiyButton = (Button) mView
				.findViewById(R.id.create_identity_button);

		mCreateIdentitiyButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				closeKeyBoard();
				String email = mEmailEditText.getText().toString();
				if (validateEmail(email)) {
					mMpinController.createNewUser(email);
				} else {
					showInvalidEmailDialog();
				}

			}
		});
	}

	@Override
	public void onResume() {
		mMpinController.disableContextToolbar();
		mMpinController.setTooblarTitle(R.string.add_identity_title);
		super.onResume();
	}

	private boolean validateEmail(String email) {
		if (TextUtils.isEmpty(email)) {
			return false;
		} else {
			return android.util.Patterns.EMAIL_ADDRESS.matcher(email).matches();
		}
	}

	private void showInvalidEmailDialog() {
		new AlertDialog.Builder(getActivity()).setTitle("Invalid email")
				.setMessage("Please, enter valid email address!")
				.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						dialog.cancel();
					}
				}).show();
	}

	private void closeKeyBoard() {
		View view = getActivity().getCurrentFocus();
		if (view != null) {
			InputMethodManager inputManager = (InputMethodManager) getActivity()
					.getSystemService(Context.INPUT_METHOD_SERVICE);
			inputManager.hideSoftInputFromWindow(view.getWindowToken(),
					InputMethodManager.HIDE_NOT_ALWAYS);
		}
	}
}
