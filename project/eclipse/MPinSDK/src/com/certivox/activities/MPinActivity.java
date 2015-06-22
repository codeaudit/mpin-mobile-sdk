package com.certivox.activities;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.AlertDialog;
import android.app.FragmentTransaction;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;

import com.certivox.adapters.UsersAdapter;
import com.certivox.db.ConfigsDao;
import com.certivox.fragments.AboutFragment;
import com.certivox.fragments.AccessNumberFragment;
import com.certivox.fragments.AddUsersFragment;
import com.certivox.fragments.ConfirmEmailFragment;
import com.certivox.fragments.IdentityBlockedFragment;
import com.certivox.fragments.IdentityCreatedFragment;
import com.certivox.fragments.NewUserFragment;
import com.certivox.fragments.OTPFragment;
import com.certivox.fragments.PinPadFragment;
import com.certivox.fragments.SuccessfulLoginFragment;
import com.certivox.fragments.UsersListFragment;
import com.certivox.interfaces.PinPadController;
import com.certivox.listeners.OnAddNewUserListener;
import com.certivox.listeners.OnUserSelectedListener;
import com.certivox.models.Config;
import com.certivox.models.OTP;
import com.certivox.models.Status;
import com.certivox.models.User;
import com.certivox.models.User.State;
import com.certivox.mpinsdk.Mpin;
import com.example.mpinsdk.R;

public class MPinActivity extends BaseMPinActivity implements PinPadController {

	static {
		System.loadLibrary("AndroidMpinSDK");
	}

	public static final String KEY_ACCESS_NUMBER = "AccessNumberActivity.KEY_ACCESS_NUMBER";
	private static final String PREF_LAST_USER = "MPinActivity.PREF_LAST_USER";

	// Fragments
	private static final String FRAG_PINPAD = "FRAG_PINPAD";
	private static final String FRAG_ACCESSNUMBER = "FRAG_ACCESSNUMBER";
	private static final String FRAG_ADDUSERS = "FRAG_ADDUSERS";
	private static final String FRAG_USERSLIST = "FRAG_USERSLIST";
	private static final String FRAG_NEWUSER = "FRAG_NEWUSER";
	private static final String FRAG_CONFIRMEMAIL = "FRAG_CONFIRMEMAIL";
	private static final String FRAG_IDENTITY_CREATED = "FRAG_IDENTITY_CREATED";
	private static final String FRAG_OTP = "FRAG_OTP";
	private static final String FRAG_SUCCESSFUL_LOGIN = "SUCCESSFUL_LOGIN";
	private static final String FRAG_IDENTITY_BLOCKED = "IDENTITY_BLOCKED";
	private static final String FRAG_ABOUT = "ABOUT";

	private static volatile Mpin s_sdk;
	private static volatile MPinActivity mActivity;

	private List<User> mUsersList = new ArrayList<User>();
	private User mCurrentUser;

	private Config mConfiguration;
	private ConfigsDao mConfigsDao;

	// Threads
	private Thread mSDKInitializationThread;
	private LogoutAsyncTask mLogoutAsyncTask;
	private StartRegistrationNewUserAsyncTask mCreateNewUserTask;
	private AuthenticateAsyncTask mShowAuthenticateAsyncTask;
	private FinishRegistrationAsyncTask mFinishRegistrationAsyncTask;
	private RestartRegistrationAsyncTask mRestartRegistrationAsyncTask;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.i("DEBUG", "MPin Activity onCreate()");
		mActivity = this;
		mConfigsDao = new ConfigsDao(getApplicationContext());
		if (!isConfigurationInited()) {
			setInitialConfiguration();
		} else {
			initSDK(mConfiguration);
			setChosenConfiguration(mConfiguration.getTitle());
			initUsersList();
			setInitialScreen();
		}
	}

	@Override
	protected void onStart() {
		super.onStart();

	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		Log.i("DEBUG", "MPin Activity onDestroy()");
		mActivity = null;
		mConfiguration = null;
		mCurrentUser = null;
		mUsersList = null;

		stopRunningThreads();
	}

	private void stopRunningThreads() {
		Log.i("DEBUG", "Stop running Threads");
		if (mSDKInitializationThread != null
				&& mSDKInitializationThread.isAlive()) {
			mSDKInitializationThread.interrupt();
		}
		if (mLogoutAsyncTask != null) {
			mLogoutAsyncTask.cancel(true);
		}
		if (mCreateNewUserTask != null) {
			mCreateNewUserTask.cancel(true);
		}
		if (mShowAuthenticateAsyncTask != null) {
			mShowAuthenticateAsyncTask.cancel(true);
		}
		if (mFinishRegistrationAsyncTask != null) {
			mFinishRegistrationAsyncTask.cancel(true);
		}
		if (mRestartRegistrationAsyncTask != null) {
			mRestartRegistrationAsyncTask.cancel(true);
		}
	}

	private void setInitialConfiguration() {
		initEmptySDK();
		startActivity(new Intent(this, PinpadConfigActivity.class));
		finish();
	}

	private boolean isConfigurationInited() {
		mConfiguration = mConfigsDao.getActiveConfiguration();
		if (mConfiguration == null) {
			Toast.makeText(this, "No active configuration", Toast.LENGTH_SHORT)
					.show();
			return false;
		}
		return true;
	}

	private void initSDK(Config config) {
		Mpin sdk = MPinActivity.peekSdk();
		if (sdk == null) {
			HashMap<String, String> serverConfig = new HashMap<String, String>();
			serverConfig.put("RPA_server", config.getBackendUrl());
			startSDKInitialization(this, serverConfig);
		}
	}

	private void initEmptySDK() {
		startSDKInitialization(this, null);
	}

	@Override
	public User getCurrentUser() {
		if (mCurrentUser != null) {
			return mCurrentUser;
		}

		String id = PreferenceManager.getDefaultSharedPreferences(this)
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

	public void setCurrentUser(User user) {
		mCurrentUser = user;
		PreferenceManager.getDefaultSharedPreferences(this).edit()
				.putString(PREF_LAST_USER, user != null ? user.getId() : "")
				.commit();
		if (getUsersListFragment() != null) {
			enableContextToolbar();
			getUsersListFragment().setSelectedUser(user);
		}
	}

	@Override
	public void createNewUser(String email) {
		mCreateNewUserTask = new StartRegistrationNewUserAsyncTask();
		mCreateNewUserTask.execute(email);
	}

	private OnUserSelectedListener getOnUserSelectedListener() {
		OnUserSelectedListener onUserSelectedListener = new OnUserSelectedListener() {
			@Override
			public void onUserSelected(final User user) {
				enableContextToolbar();
				setCurrentUser(user);
			}
		};
		return onUserSelectedListener;
	}

	public OnAddNewUserListener getOnAddNewUserListener() {
		OnAddNewUserListener onAddNewUserListener = new OnAddNewUserListener() {
			@Override
			public void onAddNewUser() {
				addNewUserFragment();
			}
		};

		return onAddNewUserListener;
	}

	private void showAuthenticate() {
		if (mConfiguration.getRequestAccessNumber()) {
			addAccessNumberFragment();
		} else {
			showAuthenticate("");
		}
	}

	private void showAuthenticate(final String accessNumber) {
		mShowAuthenticateAsyncTask = new AuthenticateAsyncTask();
		mShowAuthenticateAsyncTask.execute(accessNumber);
	}

	private void onFailedToAuthenticate(Status status, boolean isAccessNumber) {
		Log.i("DEBUG", "Failed to auth  status = " + status);

		if (getCurrentUser().getState().equals(User.State.BLOCKED)) {
			userBlocked();
			return;
		}

		switch (status.getStatusCode()) {
		case INCORRECT_ACCESS_NUMBER:
			new AlertDialog.Builder(mActivity)
					.setTitle("Incorrect Access Number!").setMessage("")
					.setPositiveButton("OK", null).show();
			setChooseUserScreen();
			break;
		case INCORRECT_PIN:
			if (isAccessNumber) {
				new AlertDialog.Builder(MPinActivity.this)
						.setTitle("INCORRECT PIN")
						.setMessage("You entered wrong pin!")
						.setPositiveButton("OK", null).show();

			} else {
				getPinPadFragment().showWrongPin();
			}
			showAuthenticate();
			break;
		case RESPONSE_PARSE_ERROR:
			new AlertDialog.Builder(MPinActivity.this)
					.setTitle("OTP not supported")
					.setMessage("The configuration does not support OTP")
					.setPositiveButton("OK", null).show();
			setChooseUserScreen();
			break;
		default:

			break;
		}
	}

	private void showCreatingNewIdentity(final User user, Status reason) {
		setCurrentUser(user);
		if (user.getState() == State.ACTIVATED) {
			emailConfirmed();
		} else {
			addConfirmEmailFragment();
		}
	}

	@Override
	public void emailConfirmed() {
		mFinishRegistrationAsyncTask = new FinishRegistrationAsyncTask();
		mFinishRegistrationAsyncTask.execute();
	}

	@Override
	public void resendEmail() {
		mRestartRegistrationAsyncTask = new RestartRegistrationAsyncTask();
		mRestartRegistrationAsyncTask.execute();
	}

	@Override
	public void signIn() {
		showAuthenticate();
	}

	@Override
	public void logout() {
		mLogoutAsyncTask = new LogoutAsyncTask();
		mLogoutAsyncTask.execute();
	}

	private void initUsersList() {
		mUsersList.clear();
		mCurrentUser = null;
		sdk().ListUsers(mUsersList);
	}

	private void setInitialScreen() {
		if (getCurrentUser() != null) {
			userChosen();
		} else {
			setChooseUserScreen();
		}
	}

	private void setChooseUserScreen() {
		initUsersList();
		if (mUsersList.isEmpty()) {
			addUsersFragment();
		} else {
			addUsersListFragment();
		}
	}

	@Override
	public void addUsersFragment() {
		Log.d("CV", " + users");
		if (getAddUserFragment() == null) {
			AddUsersFragment addUserFragment = new AddUsersFragment();
			addUserFragment.setController(mActivity);
			addUserFragment.setOnAddNewListener(getOnAddNewUserListener());

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, addUserFragment, FRAG_ADDUSERS);
			transaction.commit();
			getFragmentManager().executePendingTransactions();
			enableDrawer();
		}
	}

	@Override
	public void removeAddUsersFragment() {
		Log.d("CV", " - users");
		if (getAddUserFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getAddUserFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	@Override
	public void addUsersListFragment() {
		Log.d("CV", " + users list");
		UsersAdapter usersAdapter = new UsersAdapter(mActivity);
		usersAdapter.setData(mUsersList);

		if (getUsersListFragment() == null) {

			UsersListFragment usersListFramgent = new UsersListFragment();
			usersListFramgent.setController(mActivity);
			usersListFramgent.setListAdapter(usersAdapter);

			usersListFramgent
					.setOnUserSelectedListener(getOnUserSelectedListener());

			usersListFramgent.setOnAddNewListener(getOnAddNewUserListener());

			getFragmentManager().beginTransaction()
					.replace(R.id.content, usersListFramgent, FRAG_USERSLIST)
					.commit();
			getFragmentManager().executePendingTransactions();
			enableDrawer();
		}

	}

	@Override
	public void removeUsersListFragment() {
		Log.d("CV", " - users list");
		if (getUsersListFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getUsersListFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	@Override
	public void addNewUserFragment() {
		Log.d("CV", " + new user");
		if (getNewUserFragment() == null) {
			NewUserFragment newUserFragment = new NewUserFragment();
			newUserFragment.setController(mActivity);

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, newUserFragment, FRAG_NEWUSER);
			transaction.commit();
			getFragmentManager().executePendingTransactions();
			enableDrawer();
		}
	}

	@Override
	public void removeNewUserFragment() {
		Log.d("CV", " - new user");
		if (getNewUserFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getNewUserFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	@Override
	public void addConfirmEmailFragment() {
		Log.d("CV", " + confirm");
		if (getConfirmEmailFragment() == null) {
			ConfirmEmailFragment confirmEmailFragment = new ConfirmEmailFragment();
			confirmEmailFragment.setController(mActivity);
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, confirmEmailFragment,
					FRAG_CONFIRMEMAIL);
			transaction.commit();
			getFragmentManager().executePendingTransactions();
			enableDrawer();
		}
	}

	@Override
	public void removeConfirmEmailFragment() {
		Log.d("CV", " - confirm");
		if (getConfirmEmailFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getConfirmEmailFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	@Override
	public void addIdentityCreatedFragment() {
		Log.d("CV", " + id created");
		if (getIdentityCreatedFragment() == null) {
			IdentityCreatedFragment identityCreatedFragment = new IdentityCreatedFragment();
			identityCreatedFragment.setController(mActivity);

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, identityCreatedFragment,
					FRAG_IDENTITY_CREATED);
			transaction.commit();
			getFragmentManager().executePendingTransactions();
			enableDrawer();
		}
	}

	@Override
	public void removeIdentityCreatedFragment() {
		Log.d("CV", " - id created");
		if (getIdentityCreatedFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getIdentityCreatedFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	@Override
	public void addPinPadFragment() {
		Log.d("CV", " + pinpad");
		if (getPinPadFragment() == null) {
			PinPadFragment pinPadFragment = new PinPadFragment();
			pinPadFragment.setController(mActivity);

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, pinPadFragment, FRAG_PINPAD);
			transaction.commit();
			getFragmentManager().executePendingTransactions();
			enableDrawer();
		}

		synchronized (MPinActivity.class) {
			MPinActivity.class.notifyAll();
		}
	}

	@Override
	public void removePinPadFragment() {
		Log.d("CV", " - pinpad");
		if (getPinPadFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getPinPadFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	@Override
	public void addAccessNumberFragment() {
		Log.d("CV", " + an");
		if (getAccessNumberFragment() == null) {
			AccessNumberFragment accessNumberFragment = new AccessNumberFragment();
			accessNumberFragment.setController(mActivity);

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, accessNumberFragment,
					FRAG_ACCESSNUMBER);
			transaction.commit();
			getFragmentManager().executePendingTransactions();
			enableDrawer();
		}

	}

	@Override
	public void removeAccessNumberFragment() {
		Log.d("CV", " - an");
		if (getAccessNumberFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getAccessNumberFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	@Override
	public void addOTPFragment(OTP otp) {
		Log.d("CV", " + otp");
		if (getOTPFragment() == null) {
			OTPFragment otpFragment = new OTPFragment();
			otpFragment.setController(mActivity);
			otpFragment.setOTP(otp);

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, otpFragment, FRAG_OTP);
			transaction.commit();
			getFragmentManager().executePendingTransactions();
			enableDrawer();
		}

	}

	@Override
	public void removeOTPFragment() {
		Log.d("CV", " - otp");
		if (getOTPFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getOTPFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	@Override
	public void addSuccessfulLoginFragment() {
		Log.d("CV", " + SuccessfulLoginFragment");
		if (getSuccessfulLoginFragment() == null) {
			SuccessfulLoginFragment successfulLoginFragment = new SuccessfulLoginFragment();
			successfulLoginFragment.setController(mActivity);

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, successfulLoginFragment,
					FRAG_SUCCESSFUL_LOGIN);
			transaction.commit();
			getFragmentManager().executePendingTransactions();
			enableDrawer();
		}
	}

	@Override
	public void removeSuccessfulLoginFragment() {
		Log.d("CV", " - SuccessfulLoginFragment");
		if (getSuccessfulLoginFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getSuccessfulLoginFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	@Override
	public void addIdentityBlockedFragment() {
		Log.d("CV", " + IdentityBlockedFragment");
		if (getIdentityBlockedFragment() == null) {
			IdentityBlockedFragment identityBlockedFragment = new IdentityBlockedFragment();
			identityBlockedFragment.setController(mActivity);

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, identityBlockedFragment,
					FRAG_IDENTITY_BLOCKED);
			transaction.commit();
			getFragmentManager().executePendingTransactions();
			enableDrawer();
		}
	}

	@Override
	public void removeIdentityBlockedFragment() {
		Log.d("CV", " - IdentityBlockedFragment");
		if (getIdentityBlockedFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getIdentityBlockedFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	@Override
	public void addAboutFragment() {
		Log.d("CV", " + AboutFragment");
		if (getAboutFragment() == null) {
			AboutFragment aboutFragment = new AboutFragment();
			aboutFragment.setController(mActivity);

			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.replace(R.id.content, aboutFragment, FRAG_ABOUT);
			transaction.commit();
			getFragmentManager().executePendingTransactions();
			disableDrawer();
			setNavigationBack();
		}
	}

	@Override
	public void removeAboutFragment() {
		Log.d("CV", " - AboutFragment");
		if (getAboutFragment() != null) {
			FragmentTransaction transaction = getFragmentManager()
					.beginTransaction();
			transaction.remove(getAboutFragment());
			transaction.commit();
			getFragmentManager().executePendingTransactions();
		}
	}

	private void updateUsersList() {
		if (getUsersListFragment() != null) {
			if (mUsersList == null || mUsersList.isEmpty()) {
				addUsersFragment();
			} else {
				getUsersListFragment().getListAdapter().setData(mUsersList);
			}
		}
	}

	@Override
	public void userChosen() {
		Log.i("DEBUG", "user Selected state = " + getCurrentUser().getState());
		if (getCurrentUser() != null) {
			switch (getCurrentUser().getState()) {
			case REGISTERED:
				disableSelectUser();
				showAuthenticate();
				break;
			case ACTIVATED:
				disableSelectUser();
				emailConfirmed();
				break;
			case STARTED_REGISTRATION:
				disableSelectUser();
				showCreatingNewIdentity(getCurrentUser(), null);
				break;
			case INVALID:
				enableSelectUser();
				break;
			case BLOCKED:
				userBlocked();
				enableSelectUser();
				break;
			default:
				break;
			}
		}

	}

	@Override
	public void userBlocked() {
		addIdentityBlockedFragment();
	}

	@Override
	public void deleteCurrentUser() {
		new AlertDialog.Builder(MPinActivity.this)
				.setTitle("Delete user")
				.setMessage(
						"Do you want to delete user "
								+ getCurrentUser().getId() + "?")
				.setPositiveButton("Delete",
						new DialogInterface.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog,
									int which) {
								sdk().DeleteUser(getCurrentUser());
								disableContextToolbar();
								mCurrentUser = null;
								initUsersList();
								setInitialScreen();
							}
						}).setNegativeButton("Cancel", null).show();
	}

	@Override
	public void onAccessNumberEntered(String accessNumber) {
		showAuthenticate(accessNumber);
	}

	@Override
	public void onPinEntered(String pin) {
	}

	@Override
	public void resetPin() {
		reRegisterUser(getCurrentUser());
	}

	@Override
	public void reRegisterUser(User user) {
		String userId = user.getId();
		sdk().DeleteUser(user);
		createNewUser(userId);

	}

	@Override
	public void deselectAllUsers() {
		if (getUsersListFragment() != null) {
			getUsersListFragment().deselectAllUsers();
		}
	}

	@Override
	protected void onChangeIdentityClicked() {
		mDrawerLayout.closeDrawers();
		setChooseUserScreen();
	}

	@Override
	protected void onAboutClicked() {
		addAboutFragment();
		closeDrawer();
	};

	@Override
	public void onOTPExpired() {
		setInitialScreen();
	}

	@Override
	public void onBackPressed() {
		if ((mUsersList.isEmpty() && getAddUserFragment() != null)
				|| getUsersListFragment() != null) {
			super.onBackPressed();
			return;
		}
		setChooseUserScreen();
	}

	// Fragments
	private PinPadFragment getPinPadFragment() {
		return (PinPadFragment) getFragmentManager().findFragmentByTag(
				FRAG_PINPAD);
	}

	private AccessNumberFragment getAccessNumberFragment() {
		return (AccessNumberFragment) getFragmentManager().findFragmentByTag(
				FRAG_ACCESSNUMBER);
	}

	private AddUsersFragment getAddUserFragment() {
		return (AddUsersFragment) getFragmentManager().findFragmentByTag(
				FRAG_ADDUSERS);
	}

	private UsersListFragment getUsersListFragment() {
		return (UsersListFragment) getFragmentManager().findFragmentByTag(
				FRAG_USERSLIST);
	}

	private NewUserFragment getNewUserFragment() {
		return (NewUserFragment) getFragmentManager().findFragmentByTag(
				FRAG_NEWUSER);
	}

	private ConfirmEmailFragment getConfirmEmailFragment() {
		return (ConfirmEmailFragment) getFragmentManager().findFragmentByTag(
				FRAG_CONFIRMEMAIL);
	}

	private IdentityCreatedFragment getIdentityCreatedFragment() {
		return (IdentityCreatedFragment) getFragmentManager()
				.findFragmentByTag(FRAG_IDENTITY_CREATED);
	}

	private OTPFragment getOTPFragment() {
		return (OTPFragment) getFragmentManager().findFragmentByTag(FRAG_OTP);
	}

	private IdentityBlockedFragment getIdentityBlockedFragment() {
		return (IdentityBlockedFragment) getFragmentManager()
				.findFragmentByTag(FRAG_IDENTITY_BLOCKED);
	}

	private SuccessfulLoginFragment getSuccessfulLoginFragment() {
		return (SuccessfulLoginFragment) getFragmentManager()
				.findFragmentByTag(FRAG_SUCCESSFUL_LOGIN);
	}

	private AboutFragment getAboutFragment() {
		return (AboutFragment) getFragmentManager().findFragmentByTag(
				FRAG_ABOUT);
	}

	public void startSDKInitialization(final Context context,
			final Map<String, String> config) {
		mSDKInitializationThread = new Thread(new Runnable() {
			@Override
			public void run() {
				synchronized (MPinActivity.class) {
					s_sdk = new Mpin(context, config);
					MPinActivity.class.notifyAll();
					if (mActivity != null) {
						mActivity.runOnUiThread(new Runnable() {
							@Override
							public void run() {
								if (mActivity != null) {
									mActivity.updateUsersList();
								}
							}
						});
					}
				}
			}
		});

		mSDKInitializationThread.start();
	}

	// Static methods
	public static Mpin peekSdk() {
		synchronized (MPinActivity.class) {
			return s_sdk;
		}
	}

	public static Mpin sdk() {
		try {
			synchronized (MPinActivity.class) {
				while (s_sdk == null)
					MPinActivity.class.wait();
				return s_sdk;
			}
		} catch (InterruptedException e) {
			return null;
		}
	}

	public static String show() {
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
		Log.d("CV", "get empty pin");
		return "";
	}

	public static void hide() {
		if (mActivity != null) {
			mActivity.removePinPadFragment();
		}
	}

	public static void finishInstance() {
		if (mActivity != null) {
			mActivity.finish();
			mActivity = null;
		}
	}

	private class StartRegistrationNewUserAsyncTask extends
			AsyncTask<String, Void, Integer> {
		User user;
		private final int USER_EXISTS = 0;
		private final int REGISTRATION_STARTED = 1;

		@Override
		protected void onPreExecute() {
			super.onPreExecute();
			Log.i("DEBUG", "StartRegistrationNewUserAsyncTask");
			showLoader();
		}

		@Override
		protected Integer doInBackground(String... emails) {
			String newUserId = emails[0];
			ArrayList<User> users = new ArrayList<User>();
			sdk().ListUsers(users);
			for (User user : users) {
				if (user.getId().equals(newUserId)) {
					mCurrentUser = user;
					return USER_EXISTS;
				}
			}
			user = sdk().MakeNewUser(emails[0]);
			mCurrentUser = user;
			sdk().StartRegistration(user);

			return REGISTRATION_STARTED;
		}

		@Override
		protected void onPostExecute(Integer result) {
			hideLoader();
			switch (result) {
			case REGISTRATION_STARTED:
				showCreatingNewIdentity(user, null);
				break;
			case USER_EXISTS: {
				new AlertDialog.Builder(mActivity)
						.setTitle("User already registered")
						.setMessage("Do you want to re-register the user?")
						.setPositiveButton("OK", new OnClickListener() {

							@Override
							public void onClick(DialogInterface dialog,
									int which) {
								resetPin();
							}
						}).setNegativeButton("Cancel", null).show();
				break;
			}
			default:
				break;
			}
		}
	}

	private class AuthenticateAsyncTask extends AsyncTask<String, Void, Void> {
		String accessNumber;
		OTP otp;
		com.certivox.models.Status status;

		@Override
		protected Void doInBackground(String... accessNumbers) {
			accessNumber = accessNumbers[0];
			otp = mConfiguration.getRequestOtp() ? new OTP() : null;
			com.certivox.models.Status tempStatus = null;
			final StringBuilder resultData = new StringBuilder();
			if (!accessNumber.equals("")) {
				tempStatus = sdk().AuthenticateAN(getCurrentUser(),
						accessNumber);
			} else if (otp != null) {
				tempStatus = sdk().AuthenticateOTP(getCurrentUser(), otp);
			} else {
				tempStatus = sdk().Authenticate(getCurrentUser(), resultData);
			}

			status = tempStatus;

			return null;
		}

		@Override
		protected void onPostExecute(Void result) {
			if (status.getStatusCode() != com.certivox.models.Status.Code.PIN_INPUT_CANCELED) {

				if ((status.getStatusCode() != com.certivox.models.Status.Code.OK)) {
					onFailedToAuthenticate(status, !accessNumber.equals(""));

				} else if (otp != null
						&& otp.status != null
						&& otp.status.getStatusCode() != com.certivox.models.Status.Code.OK) {
					onFailedToAuthenticate(otp.status, false);
				} else {
					if (otp != null
							&& otp.status != null
							&& otp.status.getStatusCode() == com.certivox.models.Status.Code.OK
							&& otp.ttlSeconds > 0) {
						addOTPFragment(otp);
					} else {
						if (accessNumber.equals("")) {
							addSuccessfulLoginFragment();
						} else {
							new AlertDialog.Builder(mActivity)
									.setTitle("Successful Login")
									.setMessage("You are now logged in!")
									.setPositiveButton(
											"OK",
											new DialogInterface.OnClickListener() {
												@Override
												public void onClick(
														DialogInterface dialog,
														int which) {
													onBackPressed();
												}
											}).show();
						}
					}
				}
			}
		}
	}

	private class RestartRegistrationAsyncTask extends
			AsyncTask<Void, Void, Void> {

		@Override
		protected void onPreExecute() {
			super.onPreExecute();
			Log.i("DEBUG", "RestartRegistrationAsyncTask");
			showLoader();
		}

		@Override
		protected Void doInBackground(Void... params) {
			com.certivox.models.Status status = sdk().RestartRegistration(
					getCurrentUser());
			return null;
		}

		@Override
		protected void onPostExecute(Void result) {
			hideLoader();
			Toast.makeText(mActivity, "Email sent", Toast.LENGTH_LONG).show();
		}

	}

	private class FinishRegistrationAsyncTask extends
			AsyncTask<Void, Void, Void> {

		com.certivox.models.Status status;

		@Override
		protected Void doInBackground(Void... params) {
			status = sdk().FinishRegistration(getCurrentUser());
			return null;
		}

		@Override
		protected void onPostExecute(Void result) {
			if (status.getStatusCode() != com.certivox.models.Status.Code.OK) {
				new AlertDialog.Builder(mActivity)
						.setTitle("Email not confirmed")
						.setMessage(
								"Please, click the link in the email, to confirm your identity and proceed.")
						.setPositiveButton("OK", null).show();
			} else {
				addIdentityCreatedFragment();
			}
		}
	}

	private class LogoutAsyncTask extends AsyncTask<Void, Void, Void> {

		boolean isLoggedOut;

		@Override
		protected Void doInBackground(Void... params) {
			isLoggedOut = sdk().Logout(getCurrentUser());
			return null;
		}

		@Override
		protected void onPostExecute(Void result) {
			hideLoader();
			if (isLoggedOut) {
				new AlertDialog.Builder(mActivity)
						.setTitle("Successful Logout")
						.setMessage("You are now logged out!")
						.setPositiveButton("OK", null).show();
			} else {
				new AlertDialog.Builder(mActivity).setTitle("Failed Logout")
						.setMessage("Failed to perform logout")
						.setPositiveButton("OK", null).show();
			}
		}
	}
}
