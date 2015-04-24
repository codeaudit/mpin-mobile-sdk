package com.certivox.activities;

import java.util.HashMap;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Process;
import android.preference.PreferenceManager;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Toast;

import com.certivox.db.ConfigsContract.ConfigEntry;
import com.certivox.db.ConfigsDbHelper;
import com.certivox.fragments.ConfigListFragment;
import com.certivox.models.Status;
import com.certivox.mpinsdk.Config;
import com.example.mpinsdk.R;

public class PinpadConfigActivity extends ActionBarActivity {

	private Activity mActivity;
	private Toolbar mToolbar;
	private ActionBarDrawerToggle mDrawerToggle;
	private DrawerLayout mDrawerLayout;

	private ConfigListFragment mConfigListFragment;

	public static final String ACTION_CHANGING_CONFIG = "ACTION_CHANGING_CONFIG";
	public static final String ACTION_CONFIG_CHANGED = "ACTION_CONFIG_CHANGED";

	public static final String EXTRA_PREVIOUS_CONFIG = "EXTRA_PREVIOUS_CONFIG";
	public static final String EXTRA_CURRENT_CONFIG = "EXTRA_CURRENT_CONFIG";

	public static final String KEY_ACTIVE_CONFIG = "active_config";

	private static Config mLastConfig;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.base_drawer_layout);
		mActivity = this;
		mConfigListFragment = new ConfigListFragment();
		getFragmentManager().beginTransaction()
				.replace(R.id.content, mConfigListFragment).commit();
		mLastConfig = getActiveConfiguration(this);
		initViews();
		initActionBar();
		initNavigationDrawer();
	}

	private void initViews() {
		mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer);
		mToolbar = (Toolbar) findViewById(R.id.toolbar);
	}

	private void initActionBar() {
		if (mToolbar != null) {
			mToolbar.setTitle("");
			setSupportActionBar(mToolbar);
			getSupportActionBar().setDisplayHomeAsUpEnabled(true);
			getSupportActionBar().setDisplayShowHomeEnabled(true);
		}
	}

	private void initNavigationDrawer() {
		mDrawerToggle = new ActionBarDrawerToggle(this, mDrawerLayout,
				mToolbar, R.string.drawer_open, R.string.drawer_closed) {

			/** Called when a drawer has settled in a completely closed state. */
			public void onDrawerClosed(View view) {
				super.onDrawerClosed(view);
			}

			/** Called when a drawer has settled in a completely open state. */
			public void onDrawerOpened(View drawerView) {
				super.onDrawerOpened(drawerView);
			}
		};

		mDrawerLayout.setDrawerListener(mDrawerToggle);
		mDrawerLayout.setDrawerLockMode(DrawerLayout.LOCK_MODE_LOCKED_CLOSED);
		mDrawerToggle.setDrawerIndicatorEnabled(false);
		mDrawerToggle
				.setHomeAsUpIndicator(R.drawable.abc_ic_ab_back_mtrl_am_alpha);
		mDrawerToggle.setToolbarNavigationClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				if (mLastConfig != null) {
					onBackPressed();
				} else {
					new AlertDialog.Builder(mActivity)
							.setTitle("No active configuration")
							.setMessage(
									"Please, choose a configuration to continue.")
							.setPositiveButton("OK", null).show();
				}
			}
		});
	}

	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
		mDrawerToggle.syncState();
	}

	@Override
	protected void onStop() {
		Config activeConfiguration = getActiveConfiguration(this);
		if (activeConfiguration != null && mLastConfig != null
				&& activeConfiguration.getId() != mLastConfig.getId()) {
			PreferenceManager
					.getDefaultSharedPreferences(this.getApplicationContext())
					.edit().putLong(KEY_ACTIVE_CONFIG, mLastConfig.getId())
					.commit();
		}

		super.onStop();
	}

	public static Config get(Context context, long id) {
		if (id == -1 || context == null)
			return null;
		SQLiteDatabase db = new ConfigsDbHelper(context).getReadableDatabase();
		Cursor c = null;
		try {
			c = db.query(ConfigEntry.TABLE_NAME,
					ConfigEntry.getFullProjection(), ConfigEntry._ID
							+ " LIKE ?", new String[] { String.valueOf(id) },
					null, null, null);
			if (c.moveToFirst()) {
				Config config = new Config();
				config.formCursor(c);
				return config;
			}
		} finally {
			if (c != null)
				c.close();
		}
		return null;
	}

	public static Config getActiveConfiguration(Context context) {
		if (context == null)
			return null;
		long id = PreferenceManager.getDefaultSharedPreferences(
				context.getApplicationContext()).getLong(KEY_ACTIVE_CONFIG, -1);
		return get(context, id);
	}

	public static void setActive(final Context context, final Config config) {
		final long id = config != null ? config.getId() : -1;
		mLastConfig = getActiveConfiguration(context);
		if ((mLastConfig != null ? mLastConfig.getId() : -1) == id) {
			return;
		}
		PreferenceManager
				.getDefaultSharedPreferences(context.getApplicationContext())
				.edit().putLong(KEY_ACTIVE_CONFIG, id).commit();
	}

	public void activateConfiguration(final Context context, final Config config) {
		final long id = config != null ? config.getId() : -1;
		if (config != null) {
			new Thread(new Runnable() {
				@Override
				public void run() {
					Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);

					LocalBroadcastManager lbm = LocalBroadcastManager
							.getInstance(context);
					Intent intent = new Intent(ACTION_CHANGING_CONFIG);
					intent.putExtra(EXTRA_PREVIOUS_CONFIG,
							mLastConfig != null ? mLastConfig.getId() : -1);
					lbm.sendBroadcast(intent);

					HashMap<String, String> cfg = new HashMap<String, String>();
					cfg.put("RPA_server", config.getBackendUrl());
					MPinActivity.init(context, cfg);

					final Status status = MPinActivity.sdk().SetBackend(
							config.getBackendUrl());
					if (status.getStatusCode() == Status.Code.OK) {

						PreferenceManager
								.getDefaultSharedPreferences(
										context.getApplicationContext()).edit()
								.putLong(KEY_ACTIVE_CONFIG, id).commit();

						intent = new Intent(ACTION_CONFIG_CHANGED);
						intent.putExtra(EXTRA_PREVIOUS_CONFIG,
								mLastConfig != null ? mLastConfig.getId() : -1);
						intent.putExtra(EXTRA_CURRENT_CONFIG, id);
						lbm.sendBroadcast(intent);
					} else {
						intent = new Intent(ACTION_CONFIG_CHANGED);
						intent.putExtra(EXTRA_PREVIOUS_CONFIG,
								mLastConfig != null ? mLastConfig.getId() : -1);
						intent.putExtra(EXTRA_CURRENT_CONFIG,
								mLastConfig != null ? mLastConfig.getId() : -1);
						LocalBroadcastManager.getInstance(context)
								.sendBroadcast(intent);
					}

					new Handler(Looper.getMainLooper()).post(new Runnable() {
						@Override
						public void run() {
							if (status.getStatusCode() == Status.Code.OK) {
								mLastConfig = getActiveConfiguration(PinpadConfigActivity.this);
								Toast.makeText(context,
										"Configuration changed successfully",
										Toast.LENGTH_SHORT).show();
								finish();
							} else {
								Toast.makeText(context,
										"Failed to activate configuration",
										Toast.LENGTH_SHORT).show();
							}
						}
					});
				}
			}).start();
		}
	}

}
