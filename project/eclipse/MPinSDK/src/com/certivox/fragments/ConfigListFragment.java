package com.certivox.fragments;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ListFragment;
import android.content.DialogInterface;
import android.content.Intent;
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

import com.certivox.activities.ConfigDetailActivity;
import com.certivox.activities.PinpadConfigActivity;
import com.certivox.adapters.ConfigAdapter;
import com.certivox.db.ConfigsContract.ConfigEntry;
import com.certivox.db.ConfigsDbHelper;
import com.certivox.mpinsdk.Config;
import com.example.mpinsdk.R;

public class ConfigListFragment extends ListFragment {

	private long mSelectedId;

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
		PinpadConfigActivity.setActive(getActivity(),
				PinpadConfigActivity.get(getActivity(), mSelectedId));
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
		final Config active = PinpadConfigActivity.getActiveConfiguration(getActivity());
		switch (item.getItemId()) {
		case R.id.select_config: {
			((PinpadConfigActivity) getActivity()).activateConfiguration(
					getActivity(),
					PinpadConfigActivity.get(getActivity(), mSelectedId));
			return true;
		}
		case R.id.configs_list_new: {
			Intent intent = new Intent(getActivity(),
					ConfigDetailActivity.class);
			intent.putExtra(ConfigDetailActivity.EXTRA_ID, -1);
			getActivity().startActivity(intent);
			return true;
		}
		case R.id.configs_list_edit: {
			if (active == null)
				return true;
			Intent intent = new Intent(getActivity(),
					ConfigDetailActivity.class);
			intent.putExtra(ConfigDetailActivity.EXTRA_ID, active.getId());
			getActivity().startActivity(intent);
			return true;
		}
		case R.id.configs_list_delete: {
			if (active == null)
				return true;
			new AlertDialog.Builder(getActivity())
					.setTitle("Delete configuration")
					.setMessage(
							"Do you want to delete configuration '"
									+ active.getTitle() + "'?")
					.setPositiveButton("OK",
							new DialogInterface.OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog,
										int which) {
									SQLiteDatabase db = new ConfigsDbHelper(
											getActivity())
											.getWritableDatabase();
									db.delete(ConfigEntry.TABLE_NAME,
											ConfigEntry._ID + " LIKE ?",
											new String[] { String
													.valueOf(active.getId()) });

									Cursor c = db.query(ConfigEntry.TABLE_NAME,
											ConfigEntry.getFullProjection(),
											null, null, null, null, null);
									if (c.moveToFirst()) {
										PinpadConfigActivity
												.setActive(
														getActivity(),
														PinpadConfigActivity
																.get(getActivity(),
																		c.getLong(c
																				.getColumnIndexOrThrow(ConfigEntry._ID))));
									}

									((ConfigAdapter) getListAdapter())
											.changeCursor(c);

								}
							}).setNegativeButton("Cancel", null).show();
			return true;
		}
		}
		return super.onOptionsItemSelected(item);
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
