package com.certivox.activities;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.FragmentTransaction;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Process;
import android.preference.PreferenceManager;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.CursorAdapter;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.certivox.db.ConfigsContract.ConfigEntry;
import com.certivox.db.ConfigsDao;
import com.certivox.fragments.ConfigDetailFragment;
import com.certivox.fragments.ConfigListFragment;
import com.certivox.interfaces.ConfigController;
import com.certivox.models.Config;
import com.certivox.models.Status;
import com.example.mpinsdk.R;

public class PinpadConfigActivity extends ActionBarActivity implements
		ConfigController {

	private Activity mActivity;
	private Toolbar mToolbar;
	private ActionBarDrawerToggle mDrawerToggle;
	private DrawerLayout mDrawerLayout;
	private ConfigsDao mConfigsDao;
	// Fragments
	private static final String FRAG_CONFIG_LIST = "FRAG_CONFIG_LIST";
	private static final String FRAG_CONFIG_DETAILS = "FRAG_CONFIG_EDIT";

	public static final String ACTION_CHANGING_CONFIG = "ACTION_CHANGING_CONFIG";
	public static final String ACTION_CONFIG_CHANGED = "ACTION_CONFIG_CHANGED";

	public static final String EXTRA_PREVIOUS_CONFIG = "EXTRA_PREVIOUS_CONFIG";
	public static final String EXTRA_CURRENT_CONFIG = "EXTRA_CURRENT_CONFIG";

	public static final String KEY_ACTIVE_CONFIG = "active_config";

	private static Config mLastConfig;

	private RelativeLayout mLoader;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.base_drawer_layout);
		mConfigsDao = new ConfigsDao(getApplicationContext());
		mActivity = this;
		mLastConfig = mConfigsDao.getActiveConfiguration();

		initViews();
		initActionBar();
		initNavigationDrawer();

		addConfigListFragment();
	}

	public void showLoader() {
		Log.i("DEBUG", "SHOW LOADER!");
		mLoader.setVisibility(View.VISIBLE);
	}

	public void hideLoader() {
		Log.i("DEBUG", "HIDE LOADER!");
		mLoader.setVisibility(View.GONE);
	}

	@Override
	protected void onStop() {
		Config activeConfiguration = mConfigsDao.getActiveConfiguration();
		if (activeConfiguration != null) {
			PreferenceManager
					.getDefaultSharedPreferences(this.getApplicationContext())
					.edit()
					.putLong(KEY_ACTIVE_CONFIG, activeConfiguration.getId())
					.commit();
		}

		super.onStop();
	}

	public void addConfigListFragment() {
		Log.d("CV", " + config list");
		if (getConfigListFragment() == null) {
			ConfigListFragment configListFragment = new ConfigListFragment();
			configListFragment.setController(this);

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, configListFragment,
					FRAG_CONFIG_LIST);
			transaction.commit();
			getFragmentManager().executePendingTransactions();

			setConfigListBackClickListener();
			mToolbar.setTitle(R.string.select_service_toolbar_title);
		}
	}

	public void removeConfigListFragment() {
		Log.d("CV", " - config list");
		if (getConfigListFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getConfigListFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	public void addConfigDetailsFragment(long configId) {
		Log.d("CV", " + config details");
		if (getConfigDetailsFragment() == null) {
			ConfigDetailFragment configDetailsFragment = new ConfigDetailFragment();
			configDetailsFragment.setController(this);
			configDetailsFragment.setConfigId(configId);

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, configDetailsFragment,
					FRAG_CONFIG_DETAILS);
			transaction.commit();
			getFragmentManager().executePendingTransactions();

			setConfigEditBackClickListener();
			if (configId != -1) {
				mToolbar.setTitle(R.string.edit_service_toolbar_title);
			} else {
				mToolbar.setTitle(R.string.add_service_toolbar_title);
			}
		}
	}

	public void removeConfigDetailsFragment() {
		Log.d("CV", " - config details");
		if (getConfigDetailsFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getConfigDetailsFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	// Fragments
	private ConfigListFragment getConfigListFragment() {
		return (ConfigListFragment) getFragmentManager().findFragmentByTag(
				FRAG_CONFIG_LIST);
	}

	private ConfigDetailFragment getConfigDetailsFragment() {
		return (ConfigDetailFragment) getFragmentManager().findFragmentByTag(
				FRAG_CONFIG_DETAILS);
	}

	private void initViews() {
		mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer);
		mToolbar = (Toolbar) findViewById(R.id.toolbar);
		mLoader = (RelativeLayout) findViewById(R.id.loader);
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
	}

	private void setConfigListBackClickListener() {
		mDrawerToggle.setToolbarNavigationClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				if (mLastConfig != null) {
					onBackPressed();
				} else {
					showNoActiveConfigurationDialog();
				}
			}
		});
	}

	private void showNoActiveConfigurationDialog() {
		new AlertDialog.Builder(mActivity).setTitle("No active configuration")
				.setMessage("Please, choose a configuration to continue")
				.setPositiveButton("OK", null).show();
	}

	private void showNoSelectedConfigurationDialog() {
		new AlertDialog.Builder(mActivity)
				.setTitle("No selected configuration")
				.setMessage("Please, choose a configuration")
				.setPositiveButton("OK", null).show();
	}

	private void showDeleteConfigurationDialog(final long configId) {
		new AlertDialog.Builder(mActivity)
				.setTitle("Delete configuration")
				.setMessage(
						"This action will also delete all identities, associated with this configuration.")
				.setPositiveButton("OK", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						Cursor cursor = deleteConfiguration(mActivity, configId);
						((CursorAdapter) getConfigListFragment()
								.getListAdapter()).changeCursor(cursor);
					}
				}).setNegativeButton("Cancel", null).show();
	}

	private void setConfigEditBackClickListener() {
		mDrawerToggle.setToolbarNavigationClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				addConfigListFragment();
			}
		});
	}

	@Override
	public void onBackPressed() {
		if (getConfigDetailsFragment() != null) {
			addConfigListFragment();
			return;
		}
		super.onBackPressed();
	}

	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
		mDrawerToggle.syncState();
	}

	public Cursor deleteConfiguration(Context context, long configId) {

		Cursor cursor = mConfigsDao.deleteConfigurationById(configId);

		if (cursor.moveToFirst()) {
			mConfigsDao.setActiveConfig(mConfigsDao.getConfigurationById(cursor
					.getLong(cursor.getColumnIndexOrThrow(ConfigEntry._ID))));
		}

		return cursor;
	}

	public void activateConfiguration(final Config config) {
		final long id = config != null ? config.getId() : -1;
		if (config != null) {
			showLoader();
			new Thread(new Runnable() {
				@Override
				public void run() {
					Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);

					final Status status = MPinActivity.sdk().SetBackend(
							config.getBackendUrl());
					if (status.getStatusCode() == Status.Code.OK) {
						PreferenceManager
								.getDefaultSharedPreferences(
										mActivity.getApplicationContext())
								.edit().putLong(KEY_ACTIVE_CONFIG, id).commit();
					}

					new Handler(Looper.getMainLooper()).post(new Runnable() {
						@Override
						public void run() {
							hideLoader();
							if (status.getStatusCode() == Status.Code.OK) {
								mLastConfig = mConfigsDao
										.getActiveConfiguration();
								Toast.makeText(mActivity,
										"Configuration changed successfully",
										Toast.LENGTH_SHORT).show();

								Intent intent = new Intent(mActivity,
										MPinActivity.class);

								// TODO revise these flags
								intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP
										| Intent.FLAG_ACTIVITY_SINGLE_TOP
										| Intent.FLAG_ACTIVITY_CLEAR_TASK
										| Intent.FLAG_ACTIVITY_NEW_TASK);
								startActivity(intent);
								finish();
							} else {
								Toast.makeText(mActivity,
										"Failed to activate configuration",
										Toast.LENGTH_SHORT).show();
							}

						}
					});
				}
			}).start();
		}
	}

	@Override
	public void configurationSelected(long mSelectedId) {
		activateConfiguration(mConfigsDao.getConfigurationById(mSelectedId));
	}

	@Override
	public void createNewConfiguration() {
		addConfigDetailsFragment(-1);
	}

	@Override
	public void editConfiguration(long configId) {
		if (configId != -1) {
			addConfigDetailsFragment(configId);
		} else {
			showNoSelectedConfigurationDialog();
		}
	}

	@Override
	public void onDeleteConfiguration(final long configId) {
		if (configId != -1) {
			showDeleteConfigurationDialog(configId);
		} else {
			showNoSelectedConfigurationDialog();
		}

	}

	@Override
	public void configurationSaved() {
		removeConfigDetailsFragment();
		addConfigListFragment();
	}
}
