package com.certivox.fragments;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageButton;

import com.certivox.interfaces.MPinController;
import com.certivox.listeners.OnAddNewUserListener;
import com.example.mpinsdk.R;

public class AddUsersFragment extends Fragment {

	private MPinController mMpinController;
	private View mView;
	private Button mAddIdentityButton;
	private ImageButton mPlusAddIdentityButton;
	private OnAddNewUserListener mAddNewListener;

	public void setController(MPinController controller) {
		mMpinController = controller;
	}

	public void setOnAddNewListener(OnAddNewUserListener listener) {
		mAddNewListener = listener;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.add_user_layout, container, false);
		return mView;
	}

	@Override
	public void onResume() {
		mMpinController.setTooblarTitle(R.string.change_identity_title);
		super.onResume();
	}

	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		initViews();
		super.onActivityCreated(savedInstanceState);
	}

	private void initViews() {
		mPlusAddIdentityButton = (ImageButton) mView
				.findViewById(R.id.add_user_button);
		mPlusAddIdentityButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				addNewUser();
			}
		});
		mAddIdentityButton = (Button) mView.findViewById(R.id.add_identity);

		mAddIdentityButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				addNewUser();
			}
		});
	}

	private void addNewUser() {
		if (mAddNewListener != null) {
			mAddNewListener.onAddNewUser();
		}
	}
}
