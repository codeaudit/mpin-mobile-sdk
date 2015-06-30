package com.certivox.controllers;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.Context;
import android.os.Handler;
import android.os.HandlerThread;
import android.preference.PreferenceManager;
import android.text.TextUtils;
import android.util.Log;

import com.certivox.dal.ConfigsDao;
import com.certivox.interfaces.Controller;
import com.certivox.models.Config;
import com.certivox.models.OTP;
import com.certivox.models.Status;
import com.certivox.models.User;
import com.certivox.models.User.State;
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
	public static final int MESSAGE_ON_BACK = 4;
	public static final int MESSAGE_ON_DRAWER_BACK = 5;
	public static final int MESSAGE_GO_BACK_REQUEST = 6;
	public static final int MESSAGE_ON_CHANGE_IDENTITY = 7;
	public static final int MESSAGE_ON_CHANGE_SERVICE = 8;
	public static final int MESSAGE_ON_ABOUT = 9;
	public static final int MESSAGE_RESET_PIN = 10;

	// Receive Messages from Fragment Configurations List
	public static final int MESSAGE_ON_NEW_CONFIGURATION = 11;
	public static final int MESSAGE_ON_SELECT_CONFIGURATION = 12;
	public static final int MESSAGE_ON_EDIT_CONFIGURATION = 13;
	public static final int MESSAGE_DELETE_CONFIGURATION = 14;

	// Receive Messages from Fragment Configuration Edit
	public static final int MESSAGE_CHECK_BACKEND_URL = 15;
	public static final int MESSAGE_SAVE_CONFIG = 16;

	// Receive Messages from Fragment Users List
	public static final int MESSAGE_ON_CREATE_IDENTITY = 17;

	// Receive Messages from Fragment Create identity
	public static final int MESSAGE_CREATE_IDENTITY = 18;

	// Receive Messages from Fragment CONFIRM EMAIL
	public static final int MESSAGE_EMAIL_CONFIRMED = 19;
	public static final int MESSAGE_RESEND_EMAIL = 20;

	// Receive Messages from Fragment Identity created
	public static final int MESSAGE_ON_SIGN_IN = 21;

	// Sent Messages
	public static final int MESSAGE_GO_BACK = 1;
	public static final int MESSAGE_START_WORK_IN_PROGRESS = 2;
	public static final int MESSAGE_STOP_WORK_IN_PROGRESS = 3;
	public static final int MESSAGE_CONFIGURATION_DELETED = 4;
	public static final int MESSAGE_CONFIGURATION_CHANGED = 5;
	public static final int MESSAGE_CONFIGURATION_CHANGE_ERROR = 6;
	public static final int MESSAGE_VALID_BACKEND = 7;
	public static final int MESSAGE_INVALID_BACKEND = 8;
	public static final int MESSAGE_CONFIGURATION_SAVED = 9;
	public static final int MESSAGE_IDENTITY_EXISTS = 10;
	public static final int MESSAGE_SHOW_CONFIGURATIONS_LIST = 11;
	public static final int MESSAGE_SHOW_CONFIGURATION_EDIT = 12;
	public static final int MESSAGE_SHOW_ABOUT = 13;
	public static final int MESSAGE_SHOW_USERS_LIST = 14;
	public static final int MESSAGE_SHOW_CREATE_IDENTITY = 15;
	public static final int MESSAGE_SHOW_CONFIRM_EMAIL = 16;
	public static final int MESSAGE_SHOW_IDENTITY_CREATED = 17;
	public static final int MESSAGE_SHOW_SIGN_IN = 18;
	public static final int MESSAGE_SHOW_ACCESS_NUMBER = 19;
	public static final int MESSAGE_SHOW_USER_BLOCKED = 20;
	public static final int MESSAGE_SHOW_LOGGED_IN = 21;
	public static final int MESSAGE_SHOW_OTP = 22;
	public static final int MESSAGE_EMAIL_NOT_CONFIRMED = 23;
	public static final int MESSAGE_EMAIL_SENT = 24;
	public static final int MESSAGE_INCORRECT_ACCESS_NUMBER = 25;
	public static final int MESSAGE_INCORRECT_PIN = 26;

	public MPinController(Context context) {
		mContext = context;
		mConfigsDao = new ConfigsDao(mContext);
		mUsersList = new ArrayList<User>();
		mCurrentConfiguration = mConfigsDao.getActiveConfiguration();

		initWorkerThread();
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
		case MESSAGE_ON_BACK:
			return true;
		case MESSAGE_ON_DRAWER_BACK:
			// TODO: change the logic according the fragment
			notifyOutboxHandlers(MESSAGE_GO_BACK, 0, 0, null);
			return true;
		case MESSAGE_ON_NEW_CONFIGURATION:
			onEditConfiguration(-1);
			return true;
		case MESSAGE_ON_CHANGE_IDENTITY:
			notifyOutboxHandlers(MESSAGE_SHOW_USERS_LIST, 0, 0, null);
			return true;
		case MESSAGE_ON_CHANGE_SERVICE:
			notifyOutboxHandlers(MESSAGE_SHOW_CONFIGURATIONS_LIST, 0, 0, null);
			return true;
		case MESSAGE_ON_ABOUT:
			notifyOutboxHandlers(MESSAGE_SHOW_ABOUT, 0, 0, null);
			return true;
		case MESSAGE_GO_BACK_REQUEST:
			notifyOutboxHandlers(MESSAGE_GO_BACK, 0, 0, null);
			return true;
		case MESSAGE_ON_CREATE_IDENTITY:
			notifyOutboxHandlers(MESSAGE_SHOW_CREATE_IDENTITY, 0, 0, null);
			return true;
		case MESSAGE_EMAIL_CONFIRMED:
			finishRegistration();
			return true;
		case MESSAGE_RESEND_EMAIL:
			restarRegistration();
			return true;
		case MESSAGE_RESET_PIN:
			resetPin();
			return true;
		case MESSAGE_ON_SIGN_IN:
			onSignIn();
			return true;
		default:
			return false;
		}
	}

	@Override
	public boolean handleMessage(int what, Object data) {
		switch (what) {
		case MESSAGE_ON_SELECT_CONFIGURATION:
			activateConfiguration(((Long) data).longValue());
			return true;
		case MESSAGE_ON_EDIT_CONFIGURATION:
			onEditConfiguration((int) ((Long) data).longValue());
			return true;
		case MESSAGE_DELETE_CONFIGURATION:
			deleteConfiguration(((Long) data).longValue());
			return true;
		case MESSAGE_CHECK_BACKEND_URL:
			checkBackendUrl((String) data);
			return true;
		case MESSAGE_SAVE_CONFIG:
			saveConfig((Config) data);
			return true;
		case MESSAGE_CREATE_IDENTITY:
			startRegistration((String) data);
			return true;
		default:
			return false;
		}
	}

	private void initWorkerThread() {
		mWorkerThread = new HandlerThread("Controller Worker Thread");
		mWorkerThread.start();
		mWorkerHandler = new Handler(mWorkerThread.getLooper());
	}

	private void checkBackendUrl(final String backendUrl) {
		notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
		mWorkerHandler.post(new Runnable() {
			@Override
			public void run() {
				Status status = getSdk().TestBackend(backendUrl);

				notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);

				if (status.getStatusCode() == Status.Code.OK) {
					notifyOutboxHandlers(MESSAGE_VALID_BACKEND, 0, 0, null);
				} else {
					notifyOutboxHandlers(MESSAGE_INVALID_BACKEND, 0, 0, null);
				}
			}
		});
	}

	private void saveConfig(final Config config) {
		notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
		mWorkerHandler.post(new Runnable() {
			@Override
			public void run() {
				Status status = getSdk().TestBackend(config.getBackendUrl());

				notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);

				if (status.getStatusCode() == Status.Code.OK) {
					mConfigsDao.saveOrUpdate(config);
					mConfigsDao.setActiveConfig(config);
					notifyOutboxHandlers(MESSAGE_CONFIGURATION_SAVED, 0, 0,
							null);
				} else {
					notifyOutboxHandlers(MESSAGE_INVALID_BACKEND, 0, 0, null);
				}
			}
		});
	}

	private void activateConfiguration(long id) {
		final Config config = mConfigsDao.getConfigurationById(id);
		if (config != null) {
			notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
			mWorkerHandler.post(new Runnable() {

				@Override
				public void run() {
					final Status status = getSdk().SetBackend(
							config.getBackendUrl());
					if (status.getStatusCode() == Status.Code.OK) {
						// TODO: check if could just sent the id
						mConfigsDao.setActiveConfig(config);
						notifyOutboxHandlers(MESSAGE_CONFIGURATION_CHANGED, 0,
								0, null);
						// TODO: The model should listen for this to update
						initUsersList();
					} else {
						notifyOutboxHandlers(
								MESSAGE_CONFIGURATION_CHANGE_ERROR, 0, 0, null);
					}
					notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0,
							null);
				}
			});
		}
	}

	private void onEditConfiguration(int id) {
		notifyOutboxHandlers(MESSAGE_SHOW_CONFIGURATION_EDIT, id, 0, null);
	}

	private void deleteConfiguration(final long id) {
		notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
		mWorkerHandler.post(new Runnable() {

			@Override
			public void run() {
				if (id != -1) {
					mConfigsDao.deleteConfigurationById(id);
					notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0,
							null);
					notifyOutboxHandlers(MESSAGE_CONFIGURATION_DELETED, 0, 0,
							null);
				}
			}
		});
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
					notifyOutboxHandlers(MESSAGE_SHOW_CONFIGURATIONS_LIST, 0,
							0, null);
				} else {
					notifyOutboxHandlers(MESSAGE_SHOW_USERS_LIST, 0, 0, null);
				}
			}
		});
	}

	private void startRegistration(final String userId) {
		Log.i(TAG, "createIdentity " + userId);
		mWorkerHandler.post(new Runnable() {
			@Override
			public void run() {
				for (User user : getUsersList()) {
					if (user.getId().equals(userId)) {
						mCurrentUser = user;
						notifyOutboxHandlers(MESSAGE_IDENTITY_EXISTS, 0, 0,
								null);
						return;
					}
				}
				mCurrentUser = getSdk().MakeNewUser(userId);
				getSdk().StartRegistration(mCurrentUser);
				if (mCurrentUser.getState() == State.ACTIVATED) {
					finishRegistration();
				} else {
					notifyOutboxHandlers(MESSAGE_SHOW_CONFIRM_EMAIL, 0, 0, null);
				}
			}
		});
	}

	private void restarRegistration() {
		Log.i(TAG, "Restarting registration " + getCurrentUser().getId());
		mWorkerHandler.post(new Runnable() {
			@Override
			public void run() {
				Status status = getSdk().RestartRegistration(getCurrentUser());
				notifyOutboxHandlers(MESSAGE_EMAIL_SENT, 0, 0, null);
			}
		});
	}

	private void finishRegistration() {
		Log.i(TAG, "finishRegistration ");
		mWorkerHandler.post(new Runnable() {

			@Override
			public void run() {
				Status status = getSdk().FinishRegistration(getCurrentUser());
				if (status.getStatusCode() != Status.Code.OK) {
					notifyOutboxHandlers(MESSAGE_EMAIL_NOT_CONFIRMED, 0, 0,
							null);
				} else {
					notifyOutboxHandlers(MESSAGE_SHOW_IDENTITY_CREATED, 0, 0,
							null);
				}
			}
		});
	}

	private void resetPin() {
		mWorkerHandler.post(new Runnable() {

			@Override
			public void run() {
				Log.i(TAG, "RESETING PIN");
				// TODO: This should be called from model
				String userId = getCurrentUser().getId();
				// TODO: This should be separate method
				getSdk().DeleteUser(getCurrentUser());
				// TODO: NOT GOOD!
				setCurrentUser(null);
				initUsersList();
				startRegistration(userId);
			}
		});
	}

	// Do not call on UI Thread
	private Mpin getSdk() {
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
		// TODO: This should be event catched by model
		initUsersList();
		mSDKLockObject.notifyAll();
		notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
		Log.i(TAG, "MPin initialization finished");
	}

	// TODO this should be in model
	private void initUsersList() {
		Log.i(TAG, "INIT USERS LIST");
		mUsersList.clear();
		mCurrentUser = null;
		getSdk().ListUsers(mUsersList);
	}

	// TODO this should be in model
	public User getCurrentUser() {
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

	private void onSignIn() {
		mWorkerHandler.post(new Runnable() {

			@Override
			public void run() {
				if (mConfigsDao.getActiveConfiguration()
						.getRequestAccessNumber()) {
					notifyOutboxHandlers(MESSAGE_SHOW_ACCESS_NUMBER, 0, 0, null);
				} else {
					preAuthenticate("");
				}
			}
		});
		notifyOutboxHandlers(MESSAGE_SHOW_SIGN_IN, 0, 0, null);

	}

	private void preAuthenticate(final String accessNumber) {
		mWorkerHandler.post(new Runnable() {

			@Override
			public void run() {
				OTP otp = mConfigsDao.getActiveConfiguration().getRequestOtp() ? new OTP()
						: null;
				if (!accessNumber.equals("")) {
					authenticateAN(accessNumber);
				} else if (otp != null) {
					authenticateOTP(otp);
				} else {
					authenticate();
				}
			}
		});
	}

	private void authenticateAN(final String accessNumber) {
		mWorkerHandler.post(new Runnable() {

			@Override
			public void run() {
				Status status = getSdk().AuthenticateAN(getCurrentUser(),
						accessNumber);

				switch (status.getStatusCode()) {
				case PIN_INPUT_CANCELED:
					break;
				case INCORRECT_ACCESS_NUMBER:
					notifyOutboxHandlers(MESSAGE_INCORRECT_ACCESS_NUMBER, 0, 0,
							null);
					break;
				case INCORRECT_PIN:
					notifyOutboxHandlers(MESSAGE_INCORRECT_PIN, 0, 0, null);
					onSignIn();
					break;
				case OK:
					break;
				default:
					return;
				}
			}
		});
	}

	private void authenticateOTP(final OTP otp) {
		mWorkerHandler.post(new Runnable() {

			@Override
			public void run() {
				Status status = getSdk().AuthenticateOTP(getCurrentUser(), otp);

				switch (status.getStatusCode()) {
				case PIN_INPUT_CANCELED:
					break;
				case OK: {
					if (otp.status != null && otp.ttlSeconds > 0) {
						// TODO: pass the otp
						notifyOutboxHandlers(MESSAGE_SHOW_OTP, 0, 0, null);
					}
				}
					break;
				default:
					return;
				}
			}
		});
	}

	private void authenticate() {
		mWorkerHandler.post(new Runnable() {

			@Override
			public void run() {
				final StringBuilder resultData = new StringBuilder();
				Status status = getSdk().Authenticate(getCurrentUser(),
						resultData);

				if (getCurrentUser().getState().equals(User.State.BLOCKED)) {
					notifyOutboxHandlers(MESSAGE_SHOW_USER_BLOCKED, 0, 0, null);
					return;
				}

				switch (status.getStatusCode()) {
				case PIN_INPUT_CANCELED:
					break;
				case INCORRECT_PIN:
					notifyOutboxHandlers(MESSAGE_INCORRECT_PIN, 0, 0, null);
					onSignIn();
					break;
				case OK:
					notifyOutboxHandlers(MESSAGE_SHOW_LOGGED_IN, 0, 0, null);
					break;
				default:
					return;
				}
			}
		});
	}

	// TODO this should be in model
	private void setCurrentUser(User user) {
		mCurrentUser = user;
		PreferenceManager.getDefaultSharedPreferences(mContext).edit()
				.putString(PREF_LAST_USER, user != null ? user.getId() : "")
				.commit();
	}

	// TODO this should be in model
	public List<User> getUsersList() {
		return mUsersList;
	}

	public List<Config> getConfigurationsList() {
		return mConfigsDao.getListConfigs();
	}

	public long getActiveConfigurationId() {
		return mConfigsDao.getActiveConfigurationId();
	}

	public Config getActiveConfiguration() {
		return mConfigsDao.getActiveConfiguration();
	}

	public Config getConfiguration(int configId) {
		return mConfigsDao.getConfigurationById(configId);
	}
}
