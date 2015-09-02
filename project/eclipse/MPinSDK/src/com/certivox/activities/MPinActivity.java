/*******************************************************************************
 * Copyright (c) 2012-2015, Certivox All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * For full details regarding our CertiVox terms of service please refer to the following links:
 * 
 * * Our Terms and Conditions - http://www.certivox.com/about-certivox/terms-and-conditions/
 * 
 * * Our Security and Privacy - http://www.certivox.com/about-certivox/security-privacy/
 * 
 * * Our Statement of Position and Our Promise on Software Patents - http://www.certivox.com/about-certivox/patents/
 ******************************************************************************/
package com.certivox.activities;


import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;

import com.certivox.constants.FragmentTags;
import com.certivox.constants.IntentConstants;
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
import com.certivox.fragments.NoInternetConnectionFragment;
import com.certivox.fragments.OTPFragment;
import com.certivox.fragments.PinPadFragment;
import com.certivox.fragments.SuccessfulLoginFragment;
import com.certivox.fragments.UsersListFragment;
import com.certivox.models.Config;
import com.certivox.models.OTP;
import com.certivox.mpinsdk.R;

import android.app.AlertDialog;
import android.app.FragmentTransaction;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.InputMethodManager;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;
import net.hockeyapp.android.CrashManager;
import net.hockeyapp.android.FeedbackManager;
import net.hockeyapp.android.UpdateManager;


public class MPinActivity extends ActionBarActivity implements OnClickListener, Handler.Callback {

    private static final String TAG = MPinActivity.class.getSimpleName();

    // Needed for Hockey App
    private static final String APP_ID = "08b0417545be2304b7ce45ef43e30daf";

    // Controller
    private MPinController      mController;
    private Handler             mControllerHandler;
    private static MPinActivity mActivity;

    private enum ActivityStates {
        ON_CREATE, ON_STOP, ON_POST_RESUME, ON_DESTROY;
    };

    private ActivityStates mActivityLifecycleState;

    // Views
    private Toolbar               mToolbar;
    private RelativeLayout        mLoader;
    private DrawerLayout          mDrawerLayout;
    private ActionBarDrawerToggle mDrawerToggle;
    private TextView              mDrawerActiveServiceTextView;
    private TextView              mDrawerActiveServiceUrlTextView;
    private TextView              mChangeIdentityButton;
    private TextView              mChangeServiceButton;
    private TextView              mAboutButton;
    private TextView              mQuickStartGuideButton;
    private TextView              mMPinServerGuideButton;
    private TextView              mNoInternetConnectionTitle;
    private Toast                 mNoInternetToast;
    private BroadcastReceiver     mNetworkConectivityReceiver;
    private static final String   CONNECTIVITY_CHANGE = "android.net.conn.CONNECTIVITY_CHANGE";


    public MPinController getController() {
        return mController;
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mpin);
        mActivityLifecycleState = ActivityStates.ON_CREATE;

        initialize();
        registerNetworkConectivityReceiver();

        // Needed for Hockey App
        checkForUpdates();
        checkForCrashes();
    }


    private void registerNetworkConectivityReceiver() {
        mNetworkConectivityReceiver = new BroadcastReceiver() {

            public void onReceive(Context context, Intent intent) {
                mController.handleMessage(MPinController.MESSAGE_NETWORK_CONNECTION_CHANGE);
            }
        };

        registerReceiver(mNetworkConectivityReceiver, new IntentFilter(CONNECTIVITY_CHANGE));
    }


    private void unregisterNetworkConectivityReceiver() {
        if (mNetworkConectivityReceiver != null) {
            unregisterReceiver(mNetworkConectivityReceiver);
        }
    }


    @Override
    protected void onPostResume() {
        super.onPostResume();
        mActivityLifecycleState = ActivityStates.ON_POST_RESUME;
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        mActivityLifecycleState = ActivityStates.ON_DESTROY;
        mController.handleMessage(MPinController.MESSAGE_ON_DESTROY);
        unregisterNetworkConectivityReceiver();
        mController.removeOutboxHandler(mControllerHandler);
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
        mActivityLifecycleState = ActivityStates.ON_STOP;
        mController.handleMessage(MPinController.MESSAGE_ON_STOP);
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
        case R.id.change_identitiy:
            mController.handleMessage(MPinController.MESSAGE_ON_SHOW_IDENTITY_LIST);
            break;
        case R.id.change_service:
            mController.handleMessage(MPinController.MESSAGE_ON_CHANGE_SERVICE);
            break;
        case R.id.about:
            mController.handleMessage(MPinController.MESSAGE_ON_ABOUT);
            break;
        case R.id.quick_start_guide:
            mController.handleMessage(MPinController.MESSAGE_ON_QUICK_START_GUIDE);
            break;
        case R.id.m_pin_server_guide:
            mController.handleMessage(MPinController.MESSAGE_ON_MPIN_SERVER_GUIDE);
            break;
        default:
            return;
        }
    }


    @Override
    public void onBackPressed() {
        mController.handleMessage(MPinController.MESSAGE_ON_BACK);
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
        case MPinController.MESSAGE_INTERNET_CONNECTION_AVAILABLE:
            onInternetConnectionAvailable();
            return true;
        case MPinController.MESSAGE_NO_INTERNET_CONNECTION_AVAILABLE:
            onNoInternetConnectionAvailable();
            return true;
        case MPinController.MESSAGE_GO_BACK:
            goBack();
            return true;
        case MPinController.MESSAGE_CONFIGURATION_CHANGED:
        case MPinController.MESSAGE_SDK_INITIALIZED:
            setDrawerTitle();
            return true;
        case MPinController.MESSAGE_INCORRECT_PIN:
            // TODO: this is not clean
            PinPadFragment pinPadFragment = getPinPadFragment();
            if (pinPadFragment != null) {
                pinPadFragment.showWrongPin();
            }
            return true;
        case MPinController.MESSAGE_SHOW_CONFIGURATIONS_LIST:
            createAndAddFragment(FragmentTags.FRAGMENT_CONFIGURATIONS_LIST, ConfigsListFragment.class, false, null);
            return true;
        case MPinController.MESSAGE_SHOW_CONFIGURATION_EDIT:
            createAndAddFragment(FragmentTags.FRAGMENT_CONFIGURATION_EDIT, ConfigDetailFragment.class, false, msg.arg1);
            return true;
        case MPinController.MESSAGE_SHOW_ABOUT:
            createAndAddFragment(FragmentTags.FRAGMENT_ABOUT, AboutFragment.class, false, null);
            return true;
        case MPinController.MESSAGE_SHOW_IDENTITIES_LIST:
            createAndAddFragment(FragmentTags.FRAGMENT_USERS_LIST, UsersListFragment.class, false, null);
            return true;
        case MPinController.MESSAGE_SHOW_CREATE_IDENTITY:
            createAndAddFragment(FragmentTags.FRAGMENT_CREATE_IDENTITY, CreateIdentityFragment.class, false, msg.obj);
            return true;
        case MPinController.MESSAGE_SHOW_CONFIRM_EMAIL:
            createAndAddFragment(FragmentTags.FRAGMENT_CONFIRM_EMAIL, ConfirmEmailFragment.class, false, null);
            return true;
        case MPinController.MESSAGE_SHOW_IDENTITY_CREATED:
            createAndAddFragment(FragmentTags.FRAGMENT_IDENTITY_CREATED, IdentityCreatedFragment.class, false, null);
            return true;
        case MPinController.MESSAGE_SHOW_ACCESS_NUMBER:
            createAndAddFragment(FragmentTags.FRAGMENT_ACCESS_NUMBER, AccessNumberFragment.class, false, null);
            return true;
        case MPinController.MESSAGE_SHOW_USER_BLOCKED:
            createAndAddFragment(FragmentTags.FRAGMENT_IDENTITY_BLOCKED, IdentityBlockedFragment.class, false, null);
            return true;
        case MPinController.MESSAGE_SHOW_LOGGED_IN:
            createAndAddFragment(FragmentTags.FRAGMENT_SUCCESSFUL_LOGIN, SuccessfulLoginFragment.class, false, null);
            return true;
        case MPinController.MESSAGE_SHOW_OTP:
            OTP otp = (OTP) msg.obj;
            createAndAddFragment(FragmentTags.FRAGMENT_OTP, OTPFragment.class, false, otp);
            return true;
        case MPinController.MESSAGE_SHOW_NO_INTERNET_CONNECTION:
            createAndAddFragment(FragmentTags.FRAGMENT_NO_INTERNET_CONNECTION, NoInternetConnectionFragment.class,
                    false, null);
            return true;
        case MPinController.MESSAGE_INCORRECT_PIN_AN:
            showWrongPinDialog();
            return true;
        case MPinController.MESSAGE_AUTH_SUCCESS:
            showAuthSuccessDialog();
            return true;
        case MPinController.MESSAGE_OTP_NOT_SUPPORTED:
            showOtpNotSupportedDialog();
            return true;
        case MPinController.MESSAGE_INCORRECT_ACCESS_NUMBER:
            showIncorrectANDialog();
            return true;
        case MPinController.MESSAGE_NETWORK_ERROR:
            showNetworkErrorDialog();
            return true;
        case MPinController.MESSAGE_IDENTITY_NOT_AUTHORIZED:
            showInvalidUserDialog();
            return true;
        case MPinController.MESSAGE_NO_INTERNET_ACCESS:
            showNoInternetAccessToast();
            return true;
        case MPinController.MESSAGE_IMPORT_NEW_CONFIGURATIONS:
            Intent startIntent = new Intent(this, SelectConfigsActivity.class);
            startIntent.setAction(Intent.ACTION_PICK);
            startIntent.putExtra(IntentConstants.EXTRA_CONFIGS_LIST, (ArrayList<Config>) msg.obj);
            startActivity(startIntent);
            return true;
        }
        return false;
    }


    /** Called to do the initialization of the view */
    private void initialize() {
        mActivity = this;
        initController();
        initViews();
        initActionBar();
        initNavigationDrawer();

        mController.handleMessage(MPinController.MESSAGE_ON_CREATE);
    }


    private void initController() {
        mControllerHandler = new Handler(this);
        mController = new MPinController(getApplicationContext(), mControllerHandler);
    }


    /** Called when activity is being destroyed to free up memory */
    private void freeResources() {
        mActivity = null;
        mController = null;
        mDrawerActiveServiceTextView = null;
        mDrawerActiveServiceUrlTextView = null;
        mDrawerToggle = null;
        mDrawerLayout = null;
        mToolbar = null;
        mChangeIdentityButton = null;
        mChangeServiceButton = null;
        mAboutButton = null;
        mQuickStartGuideButton = null;
        mMPinServerGuideButton = null;
        mLoader = null;
        mControllerHandler = null;
    }


    private void initViews() {
        mDrawerActiveServiceTextView = (TextView) findViewById(R.id.active_service_id);
        mDrawerActiveServiceUrlTextView = (TextView) findViewById(R.id.active_service_url_id);
        mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer);
        mToolbar = (Toolbar) findViewById(R.id.toolbar);
        mChangeIdentityButton = (TextView) findViewById(R.id.change_identitiy);
        mChangeServiceButton = (TextView) findViewById(R.id.change_service);
        mAboutButton = (TextView) findViewById(R.id.about);
        mQuickStartGuideButton = (TextView) findViewById(R.id.quick_start_guide);
        mMPinServerGuideButton = (TextView) findViewById(R.id.m_pin_server_guide);
        mLoader = (RelativeLayout) findViewById(R.id.loader);
        mNoInternetConnectionTitle = (TextView) findViewById(R.id.no_network_connection_message_id);
    }


    private void initActionBar() {
        if (mToolbar != null) {
            mToolbar.setTitle("");
            setSupportActionBar(mToolbar);
        }
    }


    private void initNavigationDrawer() {
        mDrawerToggle = new ActionBarDrawerToggle(this, mDrawerLayout, mToolbar, R.string.drawer_open,
                R.string.drawer_closed) {

            /** Called when a drawer has settled in a completely closed state. */
            public void onDrawerClosed(View view) {
                super.onDrawerClosed(view);
            }


            /** Called when a drawer has settled in a completely open state. */
            public void onDrawerOpened(View drawerView) {
                hideKeyboard();
                super.onDrawerOpened(drawerView);
            }
        };

        mDrawerLayout.setDrawerListener(mDrawerToggle);
        initDrawerMenu();
    }


    public void enableDrawer() {
        mDrawerToggle.setDrawerIndicatorEnabled(true);
        mDrawerLayout.setDrawerLockMode(DrawerLayout.LOCK_MODE_UNLOCKED);
    }


    public void disableDrawer(OnClickListener drawerBackClickListener) {
        // Disable the drawer from opening via swipe
        mDrawerLayout.setDrawerLockMode(DrawerLayout.LOCK_MODE_LOCKED_CLOSED);
        mDrawerToggle.setDrawerIndicatorEnabled(false);
        // Change the hamburger icon to up carret
        if (drawerBackClickListener != null) {
            mDrawerToggle.setHomeAsUpIndicator(R.drawable.abc_ic_ab_back_mtrl_am_alpha);
            mDrawerToggle.setToolbarNavigationClickListener(drawerBackClickListener);
        }
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
        if (mQuickStartGuideButton != null) {
            mQuickStartGuideButton.setOnClickListener(this);
        }
        if ((mMPinServerGuideButton != null)) {
            mMPinServerGuideButton.setOnClickListener(this);
        }
    }


    private void setDrawerTitle() {
        Config config = mController.getActiveConfiguration();
        if (config != null) {
            if (mDrawerActiveServiceTextView != null) {
                mDrawerActiveServiceTextView.setText(config.getTitle());
            }
            if (mDrawerActiveServiceUrlTextView != null) {
                mDrawerActiveServiceUrlTextView.setText(config.getBackendUrl());
            }
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


    private void onNoInternetConnectionAvailable() {
        mNoInternetConnectionTitle.setVisibility(View.VISIBLE);
    }


    private void onInternetConnectionAvailable() {
        mNoInternetConnectionTitle.setVisibility(View.GONE);
    }


    private void createAndAddFragment(String tag, Class<? extends MPinFragment> fragmentClass, boolean addToBackStack,
            Object data) {

        // Need to check if the activity is in proper state for switching fragments, otherwise exception is thrown
        switch (mActivityLifecycleState) {
        case ON_CREATE:
        case ON_POST_RESUME:
        case ON_STOP:
            MPinFragment fragment = (MPinFragment) getFragmentManager().findFragmentByTag(tag);

            if (fragment == null) {
                fragment = getFragmentByClass(fragmentClass);
            }

            if (fragment != null && !fragment.isVisible()) {
                fragment.setData(data);

                FragmentTransaction transaction = getFragmentManager().beginTransaction();

                transaction.replace(R.id.content, fragment, tag);
                if (addToBackStack) {
                    transaction.addToBackStack(tag);
                }
                transaction.commitAllowingStateLoss();
                getFragmentManager().executePendingTransactions();
            }
            closeDrawer();
            break;
        default:
            return;
        }
    }


    private MPinFragment getFragmentByClass(Class<? extends MPinFragment> fragmentClass) {
        MPinFragment fragment = null;
        try {
            fragment = fragmentClass.getConstructor().newInstance();
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

        return fragment;
    }


    private void goBack() {
        super.onBackPressed();
    }


    // Needed for Hockey App
    private void checkForUpdates() {
        // Remove this for store / production builds!
        UpdateManager.register(this, APP_ID);
    }


    // Needed for Hockey App
    private void checkForCrashes() {
        CrashManager.register(this, APP_ID);
    }


    // Needed for Hockey App
    public void showFeedbackActivity() {
        FeedbackManager.register(this, APP_ID, null);
        FeedbackManager.showFeedbackActivity(this);
    }


    // TODO: This is not done right, should be refactored
    public static String show() {
        Log.i(TAG, "SHOW PINPAD CALLED");
        mActivity.mController.handleMessage(MPinController.MESSAGE_ON_SHOW_PINPAD);
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
            String pin = mActivity.getPinPadFragment().getPin();
            mActivity.mController.handleMessage(MPinController.MESSAGE_AUTHENTICATION_STARTED);
            return pin;
        }
        return "";
    }


    // TODO: This is not done right, should be refactored
    public void addPinPadFragment() {
        if (getPinPadFragment() == null) {
            PinPadFragment pinPadFragment = new PinPadFragment();
            pinPadFragment.setUser(mController.getCurrentUser());

            FragmentTransaction transaction = getFragmentManager().beginTransaction();
            transaction.replace(R.id.content, pinPadFragment, FragmentTags.FRAGMENT_PINPAD);
            transaction.commitAllowingStateLoss();
            getFragmentManager().executePendingTransactions();
            mController.setCurrentFragmentTag(FragmentTags.FRAGMENT_PINPAD);
        }

        synchronized (MPinActivity.class) {
            MPinActivity.class.notifyAll();
        }
    }


    // TODO: This is not done right, should be refactored
    private PinPadFragment getPinPadFragment() {
        return (PinPadFragment) getFragmentManager().findFragmentByTag(FragmentTags.FRAGMENT_PINPAD);
    }


    private void showAuthSuccessDialog() {
        new AlertDialog.Builder(this).setTitle(getString(R.string.successful_login_title))
                .setMessage(getString(R.string.successful_login_text))
                .setPositiveButton(getString(R.string.button_ok), null).show();
    }


    private void showWrongPinDialog() {
        new AlertDialog.Builder(this).setTitle(getString(R.string.incorrect_pin_title))
                .setPositiveButton(getString(R.string.button_ok), null).show();
    }


    private void showOtpNotSupportedDialog() {
        new AlertDialog.Builder(this).setTitle(getString(R.string.otp_not_supported_title))
                .setMessage(getString(R.string.otp_not_supported_text))
                .setPositiveButton(getString(R.string.button_ok), null).show();
    }


    private void showIncorrectANDialog() {
        new AlertDialog.Builder(this).setTitle(getString(R.string.incorrect_access_number_title))
                .setPositiveButton(getString(R.string.button_ok), null).show();
    }


    private void showNetworkErrorDialog() {
        new AlertDialog.Builder(this).setTitle(getString(R.string.network_error_title))
                .setMessage(getString(R.string.try_again)).setPositiveButton(getString(R.string.button_ok), null)
                .show();
    }


    private void showInvalidUserDialog() {
        new AlertDialog.Builder(this).setTitle(getString(R.string.error_dialog_title))
                .setMessage(getString(R.string.user_not_authorized))
                .setPositiveButton(getString(R.string.button_ok), null).show();
    }


    public void hideKeyboard() {
        // Check if no view has focus:
        View view = this.getCurrentFocus();
        if (view != null) {
            InputMethodManager inputManager = (InputMethodManager) this.getSystemService(Context.INPUT_METHOD_SERVICE);
            inputManager.hideSoftInputFromWindow(view.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
            view.clearFocus();
        }
    }


    private void showNoInternetAccessToast() {
        if (mNoInternetToast == null) {
            mNoInternetToast = Toast.makeText(this, getString(R.string.no_internet_toast), Toast.LENGTH_LONG);
        }
        mNoInternetToast.show();
    }
}
