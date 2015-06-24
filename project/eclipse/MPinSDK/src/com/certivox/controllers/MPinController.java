package com.certivox.controllers;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.Context;
import android.database.Cursor;
import android.os.Handler;
import android.os.HandlerThread;
import android.preference.PreferenceManager;
import android.text.TextUtils;
import android.util.Log;

import com.certivox.dal.ConfigsDao;
import com.certivox.interfaces.Controller;
import com.certivox.models.Config;
import com.certivox.models.User;
import com.certivox.mpinsdk.Mpin;

public class MPinController extends Controller {

	private static final String TAG = MPinController.class.getSimpleName();

	static {
		System.loadLibrary("AndroidMpinSDK");
	}

	private Object mSDKLockObject = new Object();

	private HandlerThread mWorkerThread;
	private Handler mWorkerHandler;

	private static final String PREF_LAST_USER = "MPinActivity.PREF_LAST_USER";
	private Context mContext;
	private static volatile Mpin sSDK;
	private ConfigsDao mConfigsDao;
	private List<User> mUsersList;
	private User mCurrentUser;
	private Config mCurrentConfiguration;

	// Receive Messages
	public static final int MESSAGE_ON_CREATE = 0;
	public static final int MESSAGE_ON_DESTROY = 1;
	public static final int MESSAGE_ON_START = 2;
	public static final int MESSAGE_ON_STOP = 3;
	public static final int MESSAGE_ON_BACK_PRESSED = 4;

	public static final int GET_CONFIGURATIONS_LIST = 5;

	// Sent Messages
	public static final int MESSAGE_START_WORK_IN_PROGRESS = 1;
	public static final int MESSAGE_STOP_WORK_IN_PROGRESS = 2;
	public static final int MESSAGE_SHOW_CONFIGURATIONS_LIST_FRAGMENT = 3;

	// Threads
	private Thread mSDKInitializationThread;

	public MPinController(Context context) {
		mContext = context;
		mConfigsDao = new ConfigsDao(mContext);
		mUsersList = new ArrayList<User>();
		mCurrentConfiguration = mConfigsDao.getActiveConfiguration();

		mWorkerThread = new HandlerThread("Controller Worker Thread");
		mWorkerThread.start();
		mWorkerHandler = new Handler(mWorkerThread.getLooper());
		startSDKInitializationThread();
		startSetupInitialScreenThread();
	}

	@Override
	public boolean handleMessage(int what) {
		switch (what) {
		case MESSAGE_ON_CREATE:
			return true;
		case MESSAGE_ON_DESTROY:
			mWorkerThread.getLooper().quit();
			return true;
		case MESSAGE_ON_START:
			return true;
		case MESSAGE_ON_STOP:
			return true;
		case MESSAGE_ON_BACK_PRESSED:
			return true;
		default:
			return false;
		}
	}

	@Override
	public boolean handleMessage(int what, Object data) {
		return false;
	}

	private void startSDKInitializationThread() {
		mWorkerHandler.post(new Runnable() {

			@Override
			public void run() {
				notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
				synchronized (mSDKLockObject) {
					initializeMPin();
				}
			}
		});
	}

	private void startSetupInitialScreenThread() {
		mWorkerHandler.post(new Runnable() {

			@Override
			public void run() {
				if (mCurrentConfiguration == null) {
					notifyOutboxHandlers(
							MESSAGE_SHOW_CONFIGURATIONS_LIST_FRAGMENT, 0, 0,
							null);
				} else {

				}
			}
		});
	}

	// Do not call on UI Thread
	private Mpin getSDK() {
		try {
			synchronized (mSDKLockObject) {
				while (sSDK == null)
					mSDKLockObject.wait();
				return sSDK;
			}
		} catch (InterruptedException e) {
			return null;
		}
	}

	private void initializeMPin() {
		Log.i(TAG, "MPin initialization started");
		setupSDK(mCurrentConfiguration);
	}

	private void setupSDK(Config config) {
		HashMap<String, String> serverConfig = new HashMap<String, String>();
		if (config != null) {
			serverConfig.put("RPA_server", config.getBackendUrl());
		}
		startSDKInitialization(serverConfig);
	}

	private void startSDKInitialization(final Map<String, String> config) {
		sSDK = new Mpin(mContext, config);
		mSDKLockObject.notifyAll();
		notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
		Log.i(TAG, "MPin initialization finished");
	}

	private void initUsersList() {
		mUsersList.clear();
		mCurrentUser = null;
		getSDK().ListUsers(mUsersList);
	}

	private void stopRunningThreads() {
		if (mSDKInitializationThread != null
				&& mSDKInitializationThread.isAlive()) {
			mSDKInitializationThread.interrupt();
		}
	}

	// TODO this should be in model
	private User getCurrentUser() {
		if (mCurrentUser != null) {
			return mCurrentUser;
		}

		String id = PreferenceManager.getDefaultSharedPreferences(mContext)
				.getString(PREF_LAST_USER, "");

		if (TextUtils.isEmpty(id)) {
			return null;
		}

		for (User user : mUsersList) {
			if (TextUtils.equals(user.getId(), id)) {
				mCurrentUser = user;
				return mCurrentUser;
			}
		}

		return null;
	}

	// TODO this should be in model
	private void setCurrentUser(User user) {
		mCurrentUser = user;
		PreferenceManager.getDefaultSharedPreferences(mContext).edit()
				.putString(PREF_LAST_USER, user != null ? user.getId() : "")
				.commit();
	}

	public List<Config> getConfigurationsList() {
		return mConfigsDao.getListConfigs();
	}

	public long getActiveConfigurationId() {
		return mConfigsDao.getActiveConfigurationId();
	}
}
