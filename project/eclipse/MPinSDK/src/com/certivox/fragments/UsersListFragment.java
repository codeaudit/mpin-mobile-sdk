package com.certivox.fragments;

import android.app.ListFragment;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListAdapter;
import android.widget.ListView;

import com.certivox.adapters.UsersAdapter;
import com.certivox.interfaces.MPinController;
import com.certivox.listeners.OnAddNewUserListener;
import com.certivox.listeners.OnUserSelectedListener;
import com.certivox.models.User;
import com.example.mpinsdk.R;

public class UsersListFragment extends ListFragment {

	private MPinController mMpinController;

	private OnUserSelectedListener mSelectedUserListener;
	private OnAddNewUserListener mAddNewListener;

	public void setController(MPinController controller) {
		mMpinController = controller;
	}

	public void setOnUserSelectedListener(OnUserSelectedListener listener) {
		mSelectedUserListener = listener;
	}

	public void setOnAddNewListener(OnAddNewUserListener listener) {
		mAddNewListener = listener;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View view = inflater.inflate(R.layout.users_list_layout, container,
				false);
		view.findViewById(R.id.add_user_button).setOnClickListener(
				new View.OnClickListener() {
					@Override
					public void onClick(View v) {
						if (mAddNewListener != null) {
							mAddNewListener.onAddNewUser();
						}
					}
				});

		return view;
	}

	@Override
	public void onResume() {
		mMpinController.setTooblarTitle(R.string.select_identity_title);
		super.onResume();
	}

	@Override
	public void setListAdapter(ListAdapter adapter) {
		if (!(adapter instanceof UsersAdapter))
			throw new IllegalArgumentException("Only UsersAdapter is supported");
		super.setListAdapter(adapter);
	}

	@Override
	public UsersAdapter getListAdapter() {
		return (UsersAdapter) super.getListAdapter();
	}

	@Override
	public void onListItemClick(ListView l, View v, int position, long id) {
		UsersAdapter adapter = getListAdapter();
		if (adapter != null) {
			User user = adapter.getItem(position);
			if (mSelectedUserListener != null) {
				mSelectedUserListener.onUserSelected(user);
			}
		}
	}

	public void setSelectedUser(User user) {
		UsersAdapter adapter = getListAdapter();
		if (adapter != null) {
			deselectAllUsers();
			if (user != null) {
				user.setUserSelected(true);
				mMpinController.enableContextToolbar();
			}
			adapter.notifyDataSetChanged();
		}
	}

	public void deselectAllUsers() {
		UsersAdapter adapter = getListAdapter();
		adapter.deselectAllUsers();
	}

}
