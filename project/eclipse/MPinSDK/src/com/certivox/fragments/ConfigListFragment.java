package com.certivox.fragments;

import android.app.Activity;
import android.app.ListFragment;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import com.certivox.activities.PinpadConfigActivity;
import com.certivox.adapters.ConfigAdapter;
import com.certivox.db.ConfigsContract.ConfigEntry;
import com.certivox.db.ConfigsDbHelper;
import com.certivox.interfaces.ConfigController;
import com.certivox.mpinsdk.Config;
import com.example.mpinsdk.R;

public class ConfigListFragment extends ListFragment {

	private long mSelectedId;

	private ConfigController controller;

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

		SQLiteDatabase db = new ConfigsDbHelper(activity).getReadableDatabase();
		Cursor c = db.query(ConfigEntry.TABLE_NAME,
				ConfigEntry.getFullProjection(), null, null, null, null, null);
		setListAdapter(new ConfigAdapter(getActivity(), c));

	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		return inflater.inflate(R.layout.config_list_layout, container, false);
	}

	@Override
	public void onListItemClick(ListView l, View v, int position, long id) {
		mSelectedId = id;
		PinpadConfigActivity.setActiveConfig(getActivity(), PinpadConfigActivity
				.getConfigurationById(getActivity(), mSelectedId));
		((ConfigAdapter) getListAdapter()).notifyDataSetChanged();
	}

	@Override
	public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
		inflater.inflate(R.menu.configs_list, menu);
		super.onCreateOptionsMenu(menu, inflater);
	}

	@Override
	public void onPrepareOptionsMenu(Menu menu) {
		// menu.findItem(R.id.configs_list_delete).setEnabled(
		// getListAdapter().getCount() > 1);
		super.onPrepareOptionsMenu(menu);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		final Config activeConfig = PinpadConfigActivity
				.getActiveConfiguration(getActivity());
		switch (item.getItemId()) {
		case R.id.select_config: {
			controller.configurationSelected(mSelectedId);
			return true;
		}
		case R.id.configs_list_new: {
			controller.createNewConfiguration();
			return true;
		}
		case R.id.configs_list_edit: {
			if (activeConfig == null) {
				return true;
			}
			controller.editConfiguration(activeConfig);
			return true;
		}
		case R.id.configs_list_delete: {
			if (activeConfig == null) {
				return true;
			}

			controller.onDeleteConfiguration(activeConfig);
			return true;
		}
		default:
			return false;
		}
	}

	@Override
	public void onStart() {
		super.onStart();
		SQLiteDatabase db = new ConfigsDbHelper(getActivity())
				.getWritableDatabase();
		Cursor c = db.query(ConfigEntry.TABLE_NAME,
				ConfigEntry.getFullProjection(), null, null, null, null, null);
		((ConfigAdapter) getListAdapter()).changeCursor(c);
	}

}
