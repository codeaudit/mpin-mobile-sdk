package com.certivox.fragments;

import java.util.List;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.Toast;

import com.certivox.adapters.ConfigurationListAdapter;
import com.certivox.controllers.MPinController;
import com.certivox.models.Config;
import com.example.mpinsdk.R;

public class ConfigurationsListFragment extends MPinFragment implements
		OnClickListener, AdapterView.OnItemClickListener, Handler.Callback {

	private String TAG = ConfigurationsListFragment.class.getCanonicalName();

	private View mView;
	private ListView mListView;
	private ConfigurationListAdapter mAdapter;
	private ImageButton mAddServiceButton;
	private long mSelectedConfiguraionId;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setHasOptionsMenu(true);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {

		mView = inflater.inflate(R.layout.fragment_configurations_list,
				container, false);

		getMPinController().addOutboxHandler(new Handler(this));

		initViews();
		initAdapter();

		return mView;
	}

	@Override
	public boolean handleMessage(Message msg) {
		switch (msg.what) {
		case MPinController.MESSAGE_CONFIGURATION_DELETED:
			mAdapter.updateConfigsList(getMPinController()
					.getConfigurationsList());
			return true;
		case MPinController.MESSAGE_CONFIGURATION_CHANGED:
			Toast.makeText(getActivity(), "Configuration activated!",
					Toast.LENGTH_SHORT).show();
			return true;
		case MPinController.MESSAGE_CONFIGURATION_CHANGE_ERROR:
			Toast.makeText(getActivity(), "Failed to activate configuration",
					Toast.LENGTH_SHORT).show();
			return true;
		default:
			return false;
		}
	}

	@Override
	public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
		inflater.inflate(R.menu.configs_list, menu);
		super.onCreateOptionsMenu(menu, inflater);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.select_config:
			getMPinController().handleMessage(
					MPinController.MESSAGE_ON_SELECT_CONFIGURATION,
					mSelectedConfiguraionId);
			return true;
		case R.id.configs_list_new:
			getMPinController().handleMessage(
					MPinController.MESSAGE_ON_NEW_CONFIGURATION);
			return true;
		case R.id.configs_list_edit:
			getMPinController().handleMessage(
					MPinController.MESSAGE_ON_EDIT_CONFIGURATION,
					mSelectedConfiguraionId);
			return true;
		case R.id.configs_list_delete:
			onDeleteConfiguration();
			return true;
		default:
			return false;
		}
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.add_service_button:
			getMPinController().handleMessage(
					MPinController.MESSAGE_ON_NEW_CONFIGURATION);
			break;
		default:
		}
	}

	@Override
	public void onItemClick(AdapterView<?> parent, View view, int position,
			long id) {
		mSelectedConfiguraionId = view.getId();
		mAdapter.setSelectedfigurationId(mSelectedConfiguraionId);
	}

	@Override
	protected void initViews() {
		mListView = (ListView) mView.findViewById(android.R.id.list);
		mAddServiceButton = (ImageButton) mView
				.findViewById(R.id.add_service_button);
		mAddServiceButton.setOnClickListener(this);
	}

	private void initAdapter() {
		List<Config> listConfigurations = getMPinController()
				.getConfigurationsList();
		mSelectedConfiguraionId = getMPinController()
				.getActiveConfigurationId();

		mAdapter = new ConfigurationListAdapter(getActivity()
				.getApplicationContext(), listConfigurations,
				mSelectedConfiguraionId);

		mListView.setAdapter(mAdapter);
		mListView.setOnItemClickListener(this);
	}

	private void onDeleteConfiguration() {
		new AlertDialog.Builder(getActivity())
				.setTitle("Delete configuration")
				.setMessage(
						"This action will also delete all identities, associated with this configuration.")
				.setPositiveButton("OK", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						getMPinController().handleMessage(
								MPinController.MESSAGE_DELETE_CONFIGURATION,
								mSelectedConfiguraionId);
					}
				}).setNegativeButton("Cancel", null).show();
	}
}
