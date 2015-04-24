package com.certivox.dialogs;
import android.app.Dialog;
import android.app.DialogFragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.certivox.listeners.OnButtonsClickListener;
import com.example.mpinsdk.R;

public class DialogAuthFailedFragment extends DialogFragment {

	private Dialog mDialog;
	private static OnButtonsClickListener sButtonsClickListener;

	private static final String TITLE = "title";
	private static final String STATUS = "status";
	private static final String POSITIVE_BUTTON_TEXT = "positive_button_text";
	private static final String NEGATIVE_BUTTON_TEXT = "negative_button_text";

	public static DialogAuthFailedFragment newInstance(
			OnButtonsClickListener buttonsClickListener, String title,
			String status, String positiveButtonText, String negativeButtonText) {
		DialogAuthFailedFragment instance = new DialogAuthFailedFragment();

		sButtonsClickListener = buttonsClickListener;

		Bundle args = new Bundle();
		args.putString(TITLE, title);
		args.putString(STATUS, status);
		args.putString(POSITIVE_BUTTON_TEXT, positiveButtonText);
		args.putString(NEGATIVE_BUTTON_TEXT, negativeButtonText);

		instance.setArguments(args);

		return instance;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.failed_authentication_dialog,
				container, false);

		mDialog = getDialog();
		String title = getArguments().getString(TITLE);
		mDialog.setTitle(title);

		TextView messageView = (TextView) view.findViewById(R.id.message);
		messageView.setText(getArguments().getString(STATUS));

		Button positiveButton = (Button) view
				.findViewById(R.id.positive_button);
		positiveButton.setText(getArguments().getString(POSITIVE_BUTTON_TEXT));

		positiveButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				sButtonsClickListener.onPositiveButtonClickListener();
				mDialog.dismiss();
			}
		});

		Button negativeButton = (Button) view
				.findViewById(R.id.negative_button);
		negativeButton.setText(getArguments().getString(NEGATIVE_BUTTON_TEXT));
		negativeButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				sButtonsClickListener.onNegativeButtonClickListener();
				mDialog.dismiss();
			}
		});
		return view;
	}
}
