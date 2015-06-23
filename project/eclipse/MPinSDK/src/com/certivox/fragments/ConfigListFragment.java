package com.certivox.fragments;

import android.app.Activity;
import android.app.ListFragment;
import android.database.Cursor;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.ListView;

import com.certivox.adapters.ConfigAdapter;
import com.certivox.dal.ConfigsDao;
import com.certivox.interfaces.ConfigController;
import com.certivox.models.Config;
import com.example.mpinsdk.R;

public class ConfigListFragment extends ListFragment {

	public static long sSelectedId;

	private ConfigController controller;
	private ImageButton addServiceImageButton;
	private ConfigsDao mConfigsDao;

	public void setController(ConfigController controller) {
		this.controller = controller;
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setHasOptionsMenu(true);
	}

	@Override
	public void onAttach(Activity activity) {
		super.onAttach(activity);
		mConfigsDao = new ConfigsDao(getActivity().getApplicationContext());
		Cursor cursor = mConfigsDao.getConfigs();
		setListAdapter(new ConfigAdapter(getActivity(), cursor));
	}

	@Override
	public void onStart() {
		super.onStart();
		sSelectedId = -1;
		Config activeConfig = mConfigsDao.getActiveConfiguration();

		if (activeConfig != null) {
			sSelectedId = activeConfig.getId();
		}

		Cursor configsCursor = mConfigsDao.getConfigs();
		((ConfigAdapter) getListAdapter()).changeCursor(configsCursor);
	}

	private void initViews() {
		addServiceImageButton = (ImageButton) getActivity().findViewById(
				R.id.add_service_button);
		addServiceImageButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				if (controller != null) {
					controller.createNewConfiguration();
				}
			}
		});
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		return inflater.inflate(R.layout.config_list_layout, container, false);
	}

	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		initViews();
	}

	@Override
	public void onListItemClick(ListView l, View v, int position, long id) {
		sSelectedId = id;
		((ConfigAdapter) getListAdapter()).notifyDataSetChanged();
	}

	@Override
	public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
		inflater.inflate(R.menu.configs_list, menu);
		super.onCreateOptionsMenu(menu, inflater);
	}

	@Override
	public void onPrepareOptionsMenu(Menu menu) {
		super.onPrepareOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.select_config: {
			controller.configurationSelected(sSelectedId);
			return true;
		}
		case R.id.configs_list_new: {
			controller.createNewConfiguration();
			return true;
		}
		case R.id.configs_list_edit: {
			controller.editConfiguration(sSelectedId);
			return true;
		}
		case R.id.configs_list_delete: {
			controller.onDeleteConfiguration(sSelectedId);
			return true;
		}
		default:
			return false;
		}
	}
}
