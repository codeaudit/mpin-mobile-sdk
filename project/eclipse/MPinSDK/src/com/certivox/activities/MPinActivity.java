package com.certivox.activities;

import java.lang.reflect.InvocationTargetException;

import android.app.Activity;
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
import com.certivox.fragments.AboutFragment;
import com.certivox.fragments.AccessNumberFragment;
import com.certivox.fragments.ConfigDetailFragment;
import com.certivox.fragments.ConfigsListFragment;
import com.certivox.fragments.ConfirmEmailFragment;
import com.certivox.fragments.CreateIdentityFragment;
import com.certivox.fragments.IdentityBlockedFragment;
import com.certivox.fragments.IdentityCreatedFragment;
import com.certivox.fragments.MPinFragment;
import com.certivox.fragments.PinPadFragment;
import com.certivox.fragments.SuccessfulLoginFragment;
import com.certivox.fragments.UsersListFragment;
import com.example.mpinsdk.R;

public class MPinActivity extends ActionBarActivity implements OnClickListener,
		Handler.Callback {

	private static final String TAG = MPinActivity.class.getSimpleName();

	// Controller
	private MPinController mController;

	private static MPinActivity mActivity;

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
	private static final String FRAGMENT_CONFIGURATION_EDIT = "FRAGMENT_CONFIGURATION_EDIT";
	private static final String FRAGMENT_PINPAD = "FRAGMENT_PINPAD";
	private static final String FRAGMENT_ACCESS_NUMBER = "FRAGMENT_ACCESS_NUMBER";
	private static final String FRAGMENT_USERS_LIST = "FRAGMENT_USERS_LIST";
	private static final String FRAGMENT_CREATE_IDENTITY = "FRAGMENT_NEW_USER";
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
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.change_identitiy:
			mController
					.handleMessage(MPinController.MESSAGE_ON_CHANGE_IDENTITY);
			break;
		case R.id.change_service:
			mController.handleMessage(MPinController.MESSAGE_ON_CHANGE_SERVICE);
			break;
		case R.id.about:
			mController.handleMessage(MPinController.MESSAGE_ON_ABOUT);
			break;
		default:
			return;
		}
	}

	@Override
	public void onBackPressed() {
		Log.i(TAG, "onBackPressed");
		mController.handleMessage(MPinController.MESSAGE_GO_BACK_REQUEST);
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
		case MPinController.MESSAGE_GO_BACK:
			goBack();
			return true;
		case MPinController.MESSAGE_CONFIGURATION_CHANGED:
			// TODO: This should be also called on initial time
			setDrawerTitle(mController.getActiveConfiguration().getTitle());
			return true;
		case MPinController.MESSAGE_INCORRECT_PIN:
			// TODO: this is not clean
			PinPadFragment pinPadFragment = getPinPadFragment();
			if (pinPadFragment != null) {
				pinPadFragment.showWrongPin();
			}
			return true;
		case MPinController.MESSAGE_SHOW_CONFIGURATIONS_LIST:
			// TODO: Check if this could be done in the fragment
			disableDrawer();
			createAndAddFragment(FRAGMENT_CONFIGURATIONS_LIST,
					ConfigsListFragment.class, false, null);
			return true;
		case MPinController.MESSAGE_SHOW_CONFIGURATION_EDIT:
			createAndAddFragment(FRAGMENT_ABOUT, ConfigDetailFragment.class,
					false, msg.arg1);
			return true;
		case MPinController.MESSAGE_SHOW_ABOUT:
			// TODO: Check if this could be done in the fragment
			disableDrawer();
			createAndAddFragment(FRAGMENT_ABOUT, AboutFragment.class, false,
					null);
			return true;
		case MPinController.MESSAGE_SHOW_USERS_LIST:
			// TODO: Check if this could be done in the fragment
			enableDrawer();
			createAndAddFragment(FRAGMENT_USERS_LIST, UsersListFragment.class,
					false, null);
			return true;
		case MPinController.MESSAGE_SHOW_CREATE_IDENTITY:
			createAndAddFragment(FRAGMENT_CREATE_IDENTITY,
					CreateIdentityFragment.class, false, null);
			return true;
		case MPinController.MESSAGE_SHOW_CONFIRM_EMAIL:
			createAndAddFragment(FRAGMENT_CONFIRM_EMAIL,
					ConfirmEmailFragment.class, false, null);
			return true;
		case MPinController.MESSAGE_SHOW_IDENTITY_CREATED:
			createAndAddFragment(FRAGMENT_IDENTITY_CREATED,
					IdentityCreatedFragment.class, false, null);
			return true;
		case MPinController.MESSAGE_SHOW_ACCESS_NUMBER:
			createAndAddFragment(FRAGMENT_ACCESS_NUMBER,
					AccessNumberFragment.class, false, null);
			return true;
		case MPinController.MESSAGE_SHOW_USER_BLOCKED:
			createAndAddFragment(FRAGMENT_IDENTITY_BLOCKED,
					IdentityBlockedFragment.class, false, null);
			return true;
		case MPinController.MESSAGE_SHOW_LOGGED_IN:
			createAndAddFragment(FRAGMENT_SUCCESSFUL_LOGIN,
					SuccessfulLoginFragment.class, false, null);
			return true;
		}
		return false;
	}

	/** Called to do the initialization of the view */
	private void initialize() {
		mActivity = this;
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
		mActivity = null;
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

		enableDrawer();
		mDrawerLayout.setDrawerListener(mDrawerToggle);
		initDrawerMenu();
	}

	private void enableDrawer() {
		mDrawerToggle.setDrawerIndicatorEnabled(true);
		mDrawerLayout.setDrawerLockMode(DrawerLayout.LOCK_MODE_UNLOCKED);
	}

	private void disableDrawer() {
		// Disable the drawer from opening via swipe
		mDrawerLayout.setDrawerLockMode(DrawerLayout.LOCK_MODE_LOCKED_CLOSED);
		mDrawerToggle.setDrawerIndicatorEnabled(false);
		// Change the hamburger icon to up carret
		mDrawerToggle
				.setHomeAsUpIndicator(R.drawable.abc_ic_ab_back_mtrl_am_alpha);

		mDrawerToggle.setToolbarNavigationClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				mController
						.handleMessage(MPinController.MESSAGE_ON_DRAWER_BACK);
			}
		});
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
		if (mLoader != null) {
			mLoader.setVisibility(View.VISIBLE);
		}
	}

	private void hideLoader() {
		if (mLoader != null) {
			mLoader.setVisibility(View.GONE);
		}
	}

	private void createAndAddFragment(String tag,
			Class<? extends MPinFragment> fragmentClass,
			boolean addToBackStack, Object data) {

		MPinFragment fragment = (MPinFragment) getFragmentManager()
				.findFragmentByTag(tag);

		if (fragment == null) {
			try {
				fragment = fragmentClass.getConstructor().newInstance();
				fragment.setMPinController(mController);
				fragment.setData(data);

				FragmentTransaction transaction = getFragmentManager()
						.beginTransaction();
				transaction.replace(R.id.content, fragment, tag);
				// TODO: Check when to add to backstack
				// transaction.addToBackStack(tag);
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
			// Close the drawer if opened
			closeDrawer();
		}
	}

	private void goBack() {
		// TODO: This doesn't update the Toolbar "back" button
		if (getFragmentManager().getBackStackEntryCount() > 0) {
			getFragmentManager().popBackStack();
			return;
		}
		super.onBackPressed();
	}

	// TODO: This is not done right, should be refactored
	public static String show() {
		Log.i(TAG, "SHOW PINPAD CALLED");
		// TODO This seems not thread-safe
		mActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				mActivity.addPinPadFragment();
			}
		});

		synchronized (MPinActivity.class) {
			while (mActivity.getPinPadFragment() == null) {
				try {
					MPinActivity.class.wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
		if (mActivity != null && mActivity.getPinPadFragment() != null) {
			return mActivity.getPinPadFragment().getPin();
		}
		return "";
	}

	// TODO: This is not done right, should be refactored
	public void addPinPadFragment() {
		if (getPinPadFragment() == null) {
			PinPadFragment pinPadFragment = new PinPadFragment();
			pinPadFragment.setUser(mController.getCurrentUser());

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, pinPadFragment, FRAGMENT_PINPAD);
			transaction.commit();
			getFragmentManager().executePendingTransactions();
			enableDrawer();
		}

		synchronized (MPinActivity.class) {
			MPinActivity.class.notifyAll();
		}
	}

	// TODO: This is not done right, should be refactored
	private PinPadFragment getPinPadFragment() {
		return (PinPadFragment) getFragmentManager().findFragmentByTag(
				FRAGMENT_PINPAD);
	}
}
