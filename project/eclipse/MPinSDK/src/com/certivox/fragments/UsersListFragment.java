package com.certivox.fragments;

import java.util.List;

import android.os.Bundle;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ListView;

import com.certivox.adapters.UsersAdapter;
import com.certivox.models.User;
import com.example.mpinsdk.R;

public class UsersListFragment extends MPinFragment implements OnClickListener,
		AdapterView.OnItemClickListener {

	private final String TAG = UsersListFragment.class.getCanonicalName();

	private List<User> mUsersList;

	private View mView;
	private ListView mListView;
	private BaseAdapter mAdapter;
	private ImageButton mCreateIdentityFAButton;
	private Button mCreateIdentityButton;

	@Override
	public void setData(Object data) {
	};

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		mView = inflater.inflate(R.layout.users_list_layout, container, false);
		initViews();
		initScreen();

		return mView;
	}

	@Override
	public void onResume() {
		super.onResume();
	}

	@Override
	public boolean handleMessage(Message msg) {
		return false;
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.create_identity_button:
		case R.id.create_identity_fa_button:
			Log.i(TAG, "Create identity clicked");
			break;
		default:
			break;
		}
	}

	@Override
	public void onItemClick(AdapterView<?> parent, View view, int position,
			long id) {

	}

	@Override
	protected void initViews() {
		mListView = (ListView) mView.findViewById(android.R.id.list);
		mCreateIdentityButton = (Button) mView
				.findViewById(R.id.create_identity_button);
		mCreateIdentityFAButton = (ImageButton) mView
				.findViewById(R.id.create_identity_fa_button);

		mCreateIdentityButton.setOnClickListener(this);
		mCreateIdentityFAButton.setOnClickListener(this);
	}

	private void initScreen() {
		mUsersList = getMPinController().getUsersList();
		if (mUsersList.isEmpty()) {
			hideIdentitiesList();
			showCreateIdentityButton();
		} else {
			hideCreateIdentityButton();
			showIdentitiesList();
			initAdapter();
		}
	}

	private void initAdapter() {
		mAdapter = new UsersAdapter(getActivity().getApplicationContext(),
				mUsersList);

		mListView.setAdapter(mAdapter);
		mListView.setOnItemClickListener(this);
	}

	private void showIdentitiesList() {
		mListView.setVisibility(View.VISIBLE);
	}

	private void hideIdentitiesList() {
		mListView.setVisibility(View.GONE);
	}

	private void showCreateIdentityButton() {
		mCreateIdentityButton.setVisibility(View.VISIBLE);
	}

	private void hideCreateIdentityButton() {
		mCreateIdentityButton.setVisibility(View.GONE);
	}

}
