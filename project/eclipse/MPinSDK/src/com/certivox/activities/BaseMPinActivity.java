package com.certivox.activities;

import net.hockeyapp.android.CrashManager;
import net.hockeyapp.android.FeedbackManager;
import net.hockeyapp.android.UpdateManager;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
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

import com.certivox.interfaces.MPinController;
import com.certivox.interfaces.PinPadController;
import com.example.mpinsdk.R;

public abstract class BaseMPinActivity extends ActionBarActivity implements
		MPinController, PinPadController {

	// Needed for Hockey App
	private static final String APP_ID = "40dc0524dbc338596640635c8c55dafb";

	private Activity mActivity;

	private boolean isSelectUserActive = true;

	private ActionBarDrawerToggle mDrawerToggle;
	protected DrawerLayout mDrawerLayout;
	private Toolbar mToolbar;
	private RelativeLayout mLoader;

	private TextView mDrawerSubtitle;

	private TextView mChangeIdentityButton;
	private TextView mChangeServiceButton;
	private TextView mAboutButton;

	private boolean isSelectUserContext;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.base_drawer_layout);
		mActivity = this;
		isSelectUserContext = false;

		initViews();
		initActionBar();
		initNavigationDrawer();

		// Needed for Hockey App
		checkForUpdates();
	}

	// Needed for Hockey App
	private void checkForCrashes() {
		CrashManager.register(this, APP_ID);
	}

	// Needed for Hockey App
	private void checkForUpdates() {
		// Remove this for store / production builds!
		UpdateManager.register(this, APP_ID);
	}

	// Needed for Hockey App
	public void showFeedbackActivity() {
		FeedbackManager.register(this, APP_ID, null);
		FeedbackManager.showFeedbackActivity(this);
	}

	@Override
	protected void onStop() {
		closeDrawer();
		super.onStop();
	}

	@Override
	public void setChosenConfiguration(String configTitle) {
		mDrawerSubtitle.setText(configTitle);
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

		mDrawerToggle.setDrawerIndicatorEnabled(true);
		mDrawerLayout.setDrawerListener(mDrawerToggle);
		initDrawerMenu();

	}

	public void showLoader() {
		Log.i("DEBUG", "SHOW LOADER!");
		mLoader.setVisibility(View.VISIBLE);
	}

	public void hideLoader() {
		Log.i("DEBUG", "HIDE LOADER!");
		mLoader.setVisibility(View.GONE);
	}

	protected void onChangeIdentityClicked() {
	}

	protected void onAboutClicked() {
	}

	private void initDrawerMenu() {
		mChangeIdentityButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				onChangeIdentityClicked();
			}
		});

		mChangeServiceButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				startActivity(new Intent(mActivity, PinpadConfigActivity.class));
			}
		});

		mAboutButton.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				onAboutClicked();
			}
		});
	}

	@Override
	public void enableContextToolbar() {
		if (!isSelectUserContext) {
			disableDrawer();
			mDrawerToggle
					.setToolbarNavigationClickListener(new OnClickListener() {
						@Override
						public void onClick(View v) {
							disableContextToolbar();
						}
					});
			isSelectUserContext = true;
			invalidateOptionsMenu();
		}
	}

	@Override
	public void disableContextToolbar() {
		enableDrawer();
		isSelectUserContext = false;
		invalidateOptionsMenu();
		deselectAllUsers();
	}

	public void disableDrawer() {
		// Disable the drawer from opening via swipe
		mDrawerLayout.setDrawerLockMode(DrawerLayout.LOCK_MODE_LOCKED_CLOSED);
		mDrawerToggle.setDrawerIndicatorEnabled(false);
		// Change the hamburger icon to up carret
		mDrawerToggle
				.setHomeAsUpIndicator(R.drawable.abc_ic_ab_back_mtrl_am_alpha);
	}

	public void setNavigationBack() {
		mDrawerToggle.setToolbarNavigationClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				onBackPressed();
			}
		});
	}

	public void enableDrawer() {
		mDrawerToggle.setDrawerIndicatorEnabled(true);
		mDrawerLayout.setDrawerLockMode(DrawerLayout.LOCK_MODE_UNLOCKED);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		if (isSelectUserContext) {
			enableSelectUser();
			getMenuInflater().inflate(R.menu.select_user_menu, menu);
			return true;
		}
		return false;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.select_user:
			if (isSelectUserActive) {
				userChosen();
			}
			return true;
		case R.id.delete_user:
			deleteCurrentUser();
			return true;
		case R.id.reset_pin:
			resetPin();
			return true;
		case android.R.id.home:
			onBackPressed();
			return true;
		default:
			return false;
		}
	}

	public void enableSelectUser() {
		isSelectUserActive = true;
	}

	public void disableSelectUser() {
		isSelectUserActive = false;
	}

	@Override
	protected void onPostCreate(Bundle savedInstanceState) {
		super.onPostCreate(savedInstanceState);
		mDrawerToggle.syncState();
	}

	public void setTooblarTitle(int resId) {
		getSupportActionBar().setTitle(resId);
	}

	@Override
	public void onBackPressed() {
		closeDrawer();
		if (getFragmentManager().getBackStackEntryCount() > 0) {
			getFragmentManager().popBackStack();
		}
		super.onBackPressed();
	}

	protected void closeDrawer() {
		if (mDrawerLayout != null) {
			mDrawerLayout.closeDrawers();
		}
	}
}
