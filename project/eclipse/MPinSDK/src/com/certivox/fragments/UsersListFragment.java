package com.certivox.fragments;

import java.util.List;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ListView;

import com.certivox.adapters.UsersAdapter;
import com.certivox.constants.FragmentTags;
import com.certivox.controllers.MPinController;
import com.certivox.models.User;
import com.example.mpinsdk.R;

public class UsersListFragment extends MPinFragment implements OnClickListener,
		AdapterView.OnItemClickListener {

	private final String TAG = UsersListFragment.class.getCanonicalName();
	private User mSelectedIdentity;
	private List<User> mUsersList;
	private boolean mShowOptionsMenu;
	private View mView;
	private ListView mListView;
	private UsersAdapter mAdapter;
	private ImageButton mCreateIdentityFAButton;
	private Button mCreateIdentityButton;

	@Override
	public void setData(Object data) {
	};

	@Override
	protected OnClickListener getDrawerBackClickListener() {
		return null;
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		setHasOptionsMenu(true);
		enableDrawer();
		mView = inflater
				.inflate(R.layout.fragment_users_list, container, false);
		initViews();
		initScreen();

		return mView;
	}

	@Override
	protected String getFragmentTag() {
		return FragmentTags.FRAGMENT_USERS_LIST;
	}

	@Override
	public boolean handleMessage(Message msg) {
		switch (msg.what) {
		case MPinController.MESSAGE_IDENTITY_DELETED:
			initScreen();
			break;
		default:
			break;
		}
		return false;
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.create_identity_button:
		case R.id.create_identity_fa_button:
			getMPinController().handleMessage(
					MPinController.MESSAGE_ON_CREATE_IDENTITY);
			break;
		default:
			break;
		}
	}

	@Override
	public void onItemClick(AdapterView<?> parent, View view, int position,
			long id) {
		mAdapter.setActiveUser(position);
		mSelectedIdentity = mAdapter.getItem(position);
		mShowOptionsMenu = true;
		getActivity().invalidateOptionsMenu();
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
		mShowOptionsMenu = false;
		getActivity().invalidateOptionsMenu();
		mUsersList = getMPinController().getUsersList();
		if (mUsersList.isEmpty()) {
			setTooblarTitle(R.string.change_identity_title);
			hideIdentitiesList();
			showCreateIdentityButton();
		} else {
			setTooblarTitle(R.string.select_identity_title);
			hideCreateIdentityButton();
			showIdentitiesList();
			initAdapter();
		}
	}

	private void initAdapter() {
		mAdapter = new UsersAdapter(getActivity().getApplicationContext(),
				mUsersList);

		// TODO: Check if this could be done better
		mAdapter.deselectAllUsers();

		mListView.setAdapter(mAdapter);
		mListView.setOnItemClickListener(this);
	}

	@Override
	public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
		if (mShowOptionsMenu) {
			inflater.inflate(R.menu.select_user_menu, menu);
			super.onCreateOptionsMenu(menu, inflater);
		}
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.select_identity:
			getMPinController().onIdentitySelected(mSelectedIdentity);
			return true;
		case R.id.reset_pin:
			showResetPinDialog();
			return true;
		case R.id.delete_identity:
			showDeleteUserDialog();
			return true;
		default:
			return false;
		}
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

	private void showDeleteUserDialog() {
		new AlertDialog.Builder(getActivity())
				.setTitle("Delete user")
				.setMessage(
						"Do you want to delete user "
								+ mSelectedIdentity.getId() + "?")
				.setPositiveButton("Delete",
						new DialogInterface.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog,
									int which) {
								getMPinController().onDeleteIdentity(
										mSelectedIdentity);
							}
						}).setNegativeButton("Cancel", null).show();
	}

	private void showResetPinDialog() {
		new AlertDialog.Builder(getActivity())
				.setTitle("Reset Pin")
				.setMessage(
						"Are you sure you would like to reset the PIN for "
								+ mSelectedIdentity.getId() + "?")
				.setPositiveButton("Reset",
						new DialogInterface.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog,
									int which) {
								getMPinController().onResetPin(
										mSelectedIdentity);
							}
						}).setNegativeButton("Cancel", null).show();
	}

}
