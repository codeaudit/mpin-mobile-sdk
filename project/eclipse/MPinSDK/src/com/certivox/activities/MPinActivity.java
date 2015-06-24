package com.certivox.activities;

import java.lang.reflect.InvocationTargetException;

import android.app.FragmentTransaction;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.certivox.controllers.MPinController;
import com.certivox.fragments.ConfigurationsListFragment;
import com.certivox.fragments.MPinFragment;
import com.example.mpinsdk.R;

public class MPinActivity extends ActionBarActivity implements OnClickListener,
		Handler.Callback {

	private static final String TAG = MPinActivity.class.getSimpleName();

	// Controller
	private MPinController mController;

	// Views
	private DrawerLayout mDrawerLayout;
	private ActionBarDrawerToggle mDrawerToggle;
	private Toolbar mToolbar;
	private RelativeLayout mLoader;
	private TextView mDrawerSubtitle;
	private TextView mChangeIdentityButton;
	private TextView mChangeServiceButton;
	private TextView mAboutButton;

	// Fragments
	private static final String FRAGMENT_CONFIGURATIONS_LIST = "FRAGMENT_CONFIGURATIONS_LIST";
	private static final String FRAGMENT_PINPAD = "FRAGMENT_PINPAD";
	private static final String FRAGMENT_ACCESS_NUMBER = "FRAGMENT_ACCESS_NUMBER";
	private static final String FRAGMENT_EMPTY_USERS_LIST = "FRAGMENT_EMPTY_USERS_LIST";
	private static final String FRAGMENT_USERS_LIST = "FRAGMENT_USERS_LIST";
	private static final String FRAGMENT_NEW_USER = "FRAGMENT_NEW_USER";
	private static final String FRAGMENT_CONFIRM_EMAIL = "FRAGMENT_CONFIRM_EMAIL";
	private static final String FRAGMENT_IDENTITY_CREATED = "FRAGMENT_IDENTITY_CREATED";
	private static final String FRAGMENT_OTP = "FRAGMENT_OTP";
	private static final String FRAGMENT_SUCCESSFUL_LOGIN = "FRAGMENT_SUCCESSFUL_LOGIN";
	private static final String FRAGMENT_IDENTITY_BLOCKED = "FRAGMENT_IDENTITY_BLOCKED";
	private static final String FRAGMENT_ABOUT = "FRAGMENT_ABOUT";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_mpin);

		initialize();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		mController.handleMessage(MPinController.MESSAGE_ON_DESTROY);
		freeResources();
	}

	@Override
	protected void onStart() {
		super.onStart();
		mController.handleMessage(MPinController.MESSAGE_ON_START);
	};

	@Override
	protected void onStop() {
		super.onStop();
		mController.handleMessage(MPinController.MESSAGE_ON_STOP);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// getMenuInflater().inflate(R.menu.select_user_menu, menu);
		return false;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.select_user:
			Log.i(TAG, "Select user pressed");
			return true;
		case R.id.delete_user:
			Log.i(TAG, "Delete user pressed");
			return true;
		case R.id.reset_pin:
			return true;
		case android.R.id.home:
			Log.i(TAG, "Home pressed");
			return true;
		default:
			return false;
		}
	}

	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.change_identitiy:
			Log.i(TAG, "Change identity pressed");
			break;
		case R.id.change_service:
			Log.i(TAG, "Change service pressed");
			break;
		case R.id.about:
			Log.i(TAG, "About pressed");
			break;
		default:
			return;
		}
	}

	@Override
	public void onBackPressed() {
		Log.i(TAG, "onBackPressed");
		mController.handleMessage(MPinController.MESSAGE_ON_BACK_PRESSED);
	}

	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
		if (mDrawerToggle != null) {
			mDrawerToggle.syncState();
		}
	}

	@Override
	public boolean handleMessage(Message msg) {
		switch (msg.what) {
		case MPinController.MESSAGE_START_WORK_IN_PROGRESS:
			showLoader();
			return true;
		case MPinController.MESSAGE_STOP_WORK_IN_PROGRESS:
			hideLoader();
			return true;
		case MPinController.MESSAGE_SHOW_CONFIGURATIONS_LIST_FRAGMENT:
			createAndAddFragment(FRAGMENT_CONFIGURATIONS_LIST,
					ConfigurationsListFragment.class, false);
			return true;
		}
		return false;
	}

	/** Called to do the initialization of the view */
	private void initialize() {
		initViews();
		initActionBar();
		initNavigationDrawer();

		// Init the controller
		mController = new MPinController(getApplicationContext());
		mController.addOutboxHandler(new Handler(this));
		mController.handleMessage(MPinController.MESSAGE_ON_CREATE);
	}

	/** Called when activity is being destroyed to free up memory */
	private void freeResources() {
		mController = null;
		mDrawerSubtitle = null;
		mDrawerToggle = null;
		mDrawerLayout = null;
		mToolbar = null;
		mChangeIdentityButton = null;
		mChangeServiceButton = null;
		mAboutButton = null;
		mLoader = null;
	}

	private void initViews() {
		mDrawerSubtitle = (TextView) findViewById(R.id.drawer_subtitle);
		mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer);
		mToolbar = (Toolbar) findViewById(R.id.toolbar);
		mChangeIdentityButton = (TextView) findViewById(R.id.change_identitiy);
		mChangeServiceButton = (TextView) findViewById(R.id.change_service);
		mAboutButton = (TextView) findViewById(R.id.about);
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

	private void setTooblarTitle(int resId) {
		getSupportActionBar().setTitle(resId);
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

		mDrawerToggle.setDrawerIndicatorEnabled(true);
		mDrawerLayout.setDrawerListener(mDrawerToggle);
		initDrawerMenu();
	}

	private void initDrawerMenu() {
		if (mChangeIdentityButton != null) {
			mChangeIdentityButton.setOnClickListener(this);
		}
		if (mChangeServiceButton != null) {
			mChangeServiceButton.setOnClickListener(this);
		}
		if (mAboutButton != null) {
			mAboutButton.setOnClickListener(this);
		}
	}

	private void setDrawerTitle(String title) {
		if (mDrawerSubtitle != null) {
			mDrawerSubtitle.setText(title);
		}
	}

	private void closeDrawer() {
		if (mDrawerLayout != null) {
			mDrawerLayout.closeDrawers();
		}
	}

	private void showLoader() {
		Log.i(TAG, "SHOW LOADER!");
		if (mLoader != null) {
			mLoader.setVisibility(View.VISIBLE);
		}
	}

	private void hideLoader() {
		Log.i(TAG, "HIDE LOADER!");
		if (mLoader != null) {
			mLoader.setVisibility(View.GONE);
		}
	}

	private void createAndAddFragment(String tag,
			Class<? extends MPinFragment> fragmentClass, boolean addToBackStack) {

		MPinFragment fragment = (MPinFragment) getFragmentManager()
				.findFragmentByTag(tag);

		if (fragment == null) {
			try {

				fragment = fragmentClass.getConstructor().newInstance();
				fragment.setMPinController(mController);

				FragmentTransaction transaction = getFragmentManager()
						.beginTransaction();
				transaction.replace(R.id.content, fragment, tag);
				transaction.commit();
				getFragmentManager().executePendingTransactions();

			} catch (InstantiationException e) {
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				e.printStackTrace();
			} catch (NoSuchMethodException e) {
				e.printStackTrace();
			}
		}
	}
}
