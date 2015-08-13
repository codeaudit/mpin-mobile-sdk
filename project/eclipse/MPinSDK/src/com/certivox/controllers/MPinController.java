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
package com.certivox.controllers;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.Context;
import android.net.ConnectivityManager;
import android.os.Handler;
import android.os.HandlerThread;
import android.preference.PreferenceManager;
import android.text.TextUtils;
import android.util.Log;

import com.certivox.constants.FragmentTags;
import com.certivox.dal.ConfigsDao;
import com.certivox.models.Config;
import com.certivox.models.OTP;
import com.certivox.models.Status;
import com.certivox.models.User;
import com.certivox.models.User.State;
import com.certivox.mpinsdk.Mpin;


public class MPinController extends Controller {

    private static final String  TAG                                      = MPinController.class.getSimpleName();

    static {
        System.loadLibrary("AndroidMpinSDK");
    }

    private Object               mSDKLockObject                           = new Object();

    private HandlerThread        mWorkerThread;
    private Handler              mWorkerHandler;

    private static final String  PREFERENCE_USER                          = "PREFERENCE_USER";
    private Context              mContext;
    private static volatile Mpin sSDK;
    private ConfigsDao           mConfigsDao;
    private List<User>           mUsersList;
    private User                 mCurrentUser;
    private Config               mCurrentConfiguration;
    private String               mCurrentFragmentTag;

    private String               mAccessNumberLength;

    // Receive Messages
    public static final int      MESSAGE_ON_CREATE                        = 0;
    public static final int      MESSAGE_ON_DESTROY                       = 1;
    public static final int      MESSAGE_ON_START                         = 2;
    public static final int      MESSAGE_ON_STOP                          = 3;
    public static final int      MESSAGE_ON_BACK                          = 4;
    public static final int      MESSAGE_ON_DRAWER_BACK                   = 5;
    public static final int      MESSAGE_ON_SHOW_IDENTITY_LIST            = 6;
    public static final int      MESSAGE_ON_CHANGE_SERVICE                = 7;
    public static final int      MESSAGE_ON_ABOUT                         = 8;
    public static final int      MESSAGE_RESET_PIN                        = 9;
    public static final int      MESSAGE_ON_SHOW_PINPAD                   = 10;
    public static final int      MESSAGE_NETWORK_CONNECTION_CHANGE        = 11;

    // Receive Messages from Fragment Configurations List
    public static final int      MESSAGE_ON_NEW_CONFIGURATION             = 12;
    public static final int      MESSAGE_ON_SELECT_CONFIGURATION          = 13;
    public static final int      MESSAGE_ON_EDIT_CONFIGURATION            = 14;
    public static final int      MESSAGE_DELETE_CONFIGURATION             = 15;

    // Receive Messages from Fragment Configuration Edit
    public static final int      MESSAGE_CHECK_BACKEND_URL                = 16;
    public static final int      MESSAGE_SAVE_CONFIG                      = 17;

    // Receive Messages from Fragment Users List
    public static final int      MESSAGE_ON_CREATE_IDENTITY               = 18;

    // Receive Messages from Fragment Create identity
    public static final int      MESSAGE_CREATE_IDENTITY                  = 19;

    // Receive Messages from Fragment CONFIRM EMAIL
    public static final int      MESSAGE_EMAIL_CONFIRMED                  = 20;
    public static final int      MESSAGE_RESEND_EMAIL                     = 21;

    // Receive Messages from Fragment Identity created
    public static final int      MESSAGE_ON_SIGN_IN                       = 22;

    // Receive Messages from Fragment Identity blocked
    public static final int      MESSAGE_ON_DELETE_IDENTITY               = 23;

    // Receive Messages from Fragment OTP
    public static final int      MESSAGE_OTP_EXPIRED                      = 24;

    // Receive Messages from MPinActivity
    public static final int      MESSAGE_AUTHENTICATION_STARTED           = 25;

    // Sent Messages
    public static final int      MESSAGE_GO_BACK                          = 1;
    public static final int      MESSAGE_START_WORK_IN_PROGRESS           = 2;
    public static final int      MESSAGE_STOP_WORK_IN_PROGRESS            = 3;
    public static final int      MESSAGE_CONFIGURATION_DELETED            = 4;
    public static final int      MESSAGE_CONFIGURATION_CHANGED            = 5;
    public static final int      MESSAGE_NO_ACTIVE_CONFIGURATION          = 6;
    public static final int      MESSAGE_CONFIGURATION_CHANGE_ERROR       = 7;
    public static final int      MESSAGE_VALID_BACKEND                    = 8;
    public static final int      MESSAGE_INVALID_BACKEND                  = 9;
    public static final int      MESSAGE_CONFIGURATION_SAVED              = 10;
    public static final int      MESSAGE_IDENTITY_EXISTS                  = 11;
    public static final int      MESSAGE_SHOW_CONFIGURATIONS_LIST         = 12;
    public static final int      MESSAGE_SHOW_CONFIGURATION_EDIT          = 13;
    public static final int      MESSAGE_SHOW_ABOUT                       = 14;
    public static final int      MESSAGE_SHOW_IDENTITIES_LIST             = 15;
    public static final int      MESSAGE_SHOW_CREATE_IDENTITY             = 16;
    public static final int      MESSAGE_SHOW_CONFIRM_EMAIL               = 17;
    public static final int      MESSAGE_SHOW_IDENTITY_CREATED            = 18;
    public static final int      MESSAGE_SHOW_SIGN_IN                     = 19;
    public static final int      MESSAGE_SHOW_ACCESS_NUMBER               = 20;
    public static final int      MESSAGE_SHOW_USER_BLOCKED                = 21;
    public static final int      MESSAGE_SHOW_LOGGED_IN                   = 22;
    public static final int      MESSAGE_SHOW_OTP                         = 23;
    public static final int      MESSAGE_EMAIL_NOT_CONFIRMED              = 24;
    public static final int      MESSAGE_EMAIL_SENT                       = 25;
    public static final int      MESSAGE_INCORRECT_ACCESS_NUMBER          = 26;
    public static final int      MESSAGE_INCORRECT_PIN                    = 27;
    public static final int      MESSAGE_INCORRECT_PIN_AN                 = 28;
    public static final int      MESSAGE_NETWORK_ERROR                    = 29;
    public static final int      MESSAGE_IDENTITY_DELETED                 = 30;
    public static final int      MESSAGE_AUTH_SUCCESS                     = 31;
    public static final int      MESSAGE_SDK_INITIALIZED                  = 32;
    public static final int      MESSAGE_OTP_NOT_SUPPORTED                = 33;
    public static final int      MESSAGE_IDENTITY_NOT_AUTHORIZED          = 34;
    public static final int      MESSAGE_NO_INTERNET_ACCESS               = 35;
    public static final int      MESSAGE_NO_INTERNET_CONNECTION_AVAILABLE = 36;
    public static final int      MESSAGE_INTERNET_CONNECTION_AVAILABLE    = 37;


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
            onDestroy();
            return true;
        case MESSAGE_ON_START:
            onStart();
            return true;
        case MESSAGE_ON_STOP:
            onStop();
            return true;
        case MESSAGE_ON_BACK:
            onBack();
            return true;
        case MESSAGE_ON_DRAWER_BACK:
            onBack();
            return true;
        case MESSAGE_NETWORK_CONNECTION_CHANGE:
            onNetworkConnectionChange();
            return true;
        case MESSAGE_ON_NEW_CONFIGURATION:
            onEditConfiguration(-1);
            return true;
        case MESSAGE_ON_SHOW_IDENTITY_LIST:
            onChangeIdentity();
            return true;
        case MESSAGE_ON_CHANGE_SERVICE:
            notifyOutboxHandlers(MESSAGE_SHOW_CONFIGURATIONS_LIST, 0, 0, null);
            return true;
        case MESSAGE_ON_ABOUT:
            notifyOutboxHandlers(MESSAGE_SHOW_ABOUT, 0, 0, null);
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
        case MESSAGE_ON_DELETE_IDENTITY:
            deleteCurrentIdentity();
            return true;
        case MESSAGE_OTP_EXPIRED:
            onSignIn();
            return true;
        case MESSAGE_ON_SHOW_PINPAD:
            notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
            return true;
        case MESSAGE_AUTHENTICATION_STARTED:
            Log.i(TAG, "MESSAGE_AUTHENTICATION_STARTED");
            notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
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
                if (isNetworkAvailable()) {
                    Status status = getSdk().TestBackend(backendUrl);
                    Log.i(TAG, "TEST BACKEND STATUS = " + status);

                    if (status.getStatusCode() == Status.Code.OK) {
                        notifyOutboxHandlers(MESSAGE_VALID_BACKEND, 0, 0, null);
                    } else {
                        notifyOutboxHandlers(MESSAGE_INVALID_BACKEND, 0, 0, null);
                    }
                } else {
                    notifyOutboxHandlers(MESSAGE_NO_INTERNET_ACCESS, 0, 0, null);
                }
                notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
            }
        });
    }


    private void saveConfig(final Config config) {
        notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                if (isNetworkAvailable()) {
                    Status status = getSdk().TestBackend(config.getBackendUrl());

                    if (status.getStatusCode() == Status.Code.OK) {
                        mConfigsDao.saveOrUpdate(config);
                        notifyOutboxHandlers(MESSAGE_CONFIGURATION_SAVED, 0, 0, null);
                        notifyOutboxHandlers(MESSAGE_SHOW_CONFIGURATIONS_LIST, 0, 0, null);
                    } else {
                        notifyOutboxHandlers(MESSAGE_INVALID_BACKEND, 0, 0, null);
                    }
                } else {
                    notifyOutboxHandlers(MESSAGE_NO_INTERNET_ACCESS, 0, 0, null);
                }
                notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
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
                    if (isNetworkAvailable()) {
                        final Status status = getSdk().SetBackend(config.getBackendUrl());
                        if (status.getStatusCode() == Status.Code.OK) {
                            // TODO: check if could just sent the id
                            mConfigsDao.setActiveConfig(config);
                            // TODO: The model should listen for this to update
                            initUsersList();
                            notifyOutboxHandlers(MESSAGE_CONFIGURATION_CHANGED, 0, 0, null);
                            notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
                        } else {
                            notifyOutboxHandlers(MESSAGE_CONFIGURATION_CHANGE_ERROR, 0, 0, null);
                        }
                    } else {
                        notifyOutboxHandlers(MESSAGE_NO_INTERNET_ACCESS, 0, 0, null);
                    }
                    notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
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
                    notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
                    notifyOutboxHandlers(MESSAGE_CONFIGURATION_DELETED, 0, 0, null);
                }
            }
        });
    }


    private void startSDKInitializationThread() {
        notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                synchronized (mSDKLockObject) {
                    initializeMPin();
                    notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
                }
            }
        });
    }


    private void startSetupInitialScreenThread() {
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                if (mCurrentConfiguration == null) {
                    notifyOutboxHandlers(MESSAGE_SHOW_CONFIGURATIONS_LIST, 0, 0, null);
                } else
                    if (getCurrentUser() != null) {
                        onSignIn();
                    } else {
                        notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
                    }
            }
        });
    }


    private void startRegistration(final String userId) {
        notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                for (User user : getUsersList()) {
                    if (user.getId().equals(userId)) {
                        mCurrentUser = user;
                        notifyOutboxHandlers(MESSAGE_IDENTITY_EXISTS, 0, 0, null);
                        notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
                        return;
                    }
                }
                if (isNetworkAvailable()) {
                    mCurrentUser = getSdk().MakeNewUser(userId);
                    Status status = getSdk().StartRegistration(getCurrentUser());
                    // TODO: This is not the right place for initing the list
                    initUsersList();
                    Log.i(TAG, "startRegistration status code = " + status.getStatusCode());
                    switch (status.getStatusCode()) {
                    case OK:
                        if (mCurrentUser.getState().equals(State.ACTIVATED)) {
                            finishRegistration();
                        } else {
                            notifyOutboxHandlers(MESSAGE_SHOW_CONFIRM_EMAIL, 0, 0, null);
                        }
                        break;
                    case IDENTITY_NOT_AUTHORIZED:
                        notifyOutboxHandlers(MESSAGE_IDENTITY_NOT_AUTHORIZED, 0, 0, null);
                        break;
                    default:
                        break;
                    }
                } else {
                    notifyOutboxHandlers(MESSAGE_NO_INTERNET_ACCESS, 0, 0, null);
                }
                notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
            }
        });
    }


    private void restarRegistration() {
        notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                if (isNetworkAvailable()) {
                    
                    Status status = getSdk().RestartRegistration(getCurrentUser());
                    Log.i(TAG, "restarRegistration status code = " + status.getStatusCode());

                    notifyOutboxHandlers(MESSAGE_EMAIL_SENT, 0, 0, null);
                } else {
                    notifyOutboxHandlers(MESSAGE_NO_INTERNET_ACCESS, 0, 0, null);
                }
                notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
            }
        });
    }


    private void finishRegistration() {
        notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                if (isNetworkAvailable()) {
                    Status status = getSdk().FinishRegistration(getCurrentUser());
                    Log.i(TAG, "finishRegistration status code = " + status.getStatusCode());
                    if (status.getStatusCode() != Status.Code.OK) {
                        notifyOutboxHandlers(MESSAGE_EMAIL_NOT_CONFIRMED, 0, 0, null);
                    } else {
                        notifyOutboxHandlers(MESSAGE_SHOW_IDENTITY_CREATED, 0, 0, null);
                    }
                } else {
                    notifyOutboxHandlers(MESSAGE_NO_INTERNET_ACCESS, 0, 0, null);
                }
                notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
            }
        });
    }


    private void resetPin() {
        notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                if (isNetworkAvailable()) {
                    Log.i(TAG, "RESETING PIN");
                    // TODO: This should be called from model
                    String userId = getCurrentUser().getId();
                    // TODO: This should be separate method
                    getSdk().DeleteUser(getCurrentUser());
                    // TODO: NOT GOOD!
                    saveCurrentUser(null);
                    initUsersList();
                    startRegistration(userId);
                } else {
                    notifyOutboxHandlers(MESSAGE_NO_INTERNET_ACCESS, 0, 0, null);
                }
            }
        });
    }
    
    // Do not call on UI Thread
    private Mpin getSdk() {
        try {
            synchronized (mSDKLockObject) {
                while (sSDK == null) {
                    mSDKLockObject.wait();
                }
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
        notifyOutboxHandlers(MESSAGE_SDK_INITIALIZED, 0, 0, null);
        notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
        Log.i(TAG, "MPin initialization finished");
    }


    // TODO this should be in model
    private void initUsersList() {
        mUsersList.clear();
        getSdk().ListUsers(mUsersList);
    }


    // TODO this should be in model
    public User getCurrentUser() {
        if (mCurrentUser != null) {
            return mCurrentUser;
        }

        String id = PreferenceManager.getDefaultSharedPreferences(mContext).getString(PREFERENCE_USER, "");

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
        notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                Log.i(TAG, getCurrentUser().getState().toString());
                switch (getCurrentUser().getState()) {
                case INVALID:
                    break;
                case ACTIVATED:
                    notifyOutboxHandlers(MESSAGE_SHOW_SIGN_IN, 0, 0, null);
                    break;
                case REGISTERED:
                    if (mConfigsDao.getActiveConfiguration().getRequestAccessNumber()) {
                        notifyOutboxHandlers(MESSAGE_SHOW_ACCESS_NUMBER, 0, 0, null);
                    } else {
                        preAuthenticate("");
                    }
                    break;
                case STARTED_REGISTRATION:
                    notifyOutboxHandlers(MESSAGE_SHOW_CONFIRM_EMAIL, 0, 0, null);
                    break;
                case BLOCKED:
                    notifyOutboxHandlers(MESSAGE_SHOW_USER_BLOCKED, 0, 0, null);
                    notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
                    break;
                default:
                    break;
                }
                notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
            }
        });
    }


    private void preAuthenticate(final String accessNumber) {
        if (isNetworkAvailable()) {
            OTP otp = mConfigsDao.getActiveConfiguration().getRequestOtp() ? new OTP() : null;
            if (!accessNumber.equals("")) {
                authenticateAN(accessNumber);
            } else
                if (otp != null) {
                    authenticateOTP(otp);
                } else {
                    authenticate();
                }
        } else {
            notifyOutboxHandlers(MESSAGE_NO_INTERNET_ACCESS, 0, 0, null);
        }
    }


    private void authenticateAN(final String accessNumber) {
        Status status = getSdk().AuthenticateAN(getCurrentUser(), accessNumber);
        Log.i(TAG, "authenticateAN Status code = " + status.getStatusCode());
        switch (status.getStatusCode()) {
        case OK:
            saveCurrentUser(mCurrentUser);
            notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
            notifyOutboxHandlers(MESSAGE_AUTH_SUCCESS, 0, 0, null);
            break;
        case INCORRECT_ACCESS_NUMBER:
            notifyOutboxHandlers(MESSAGE_INCORRECT_ACCESS_NUMBER, 0, 0, null);
            onSignIn();
            break;
        case INCORRECT_PIN:
            notifyOutboxHandlers(MESSAGE_INCORRECT_PIN_AN, 0, 0, null);
            onSignIn();
            break;
        case PIN_INPUT_CANCELED:
            notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
            break;
        case NETWORK_ERROR:
            onNetworkError();
            break;
        case IDENTITY_NOT_AUTHORIZED:
            notifyOutboxHandlers(MESSAGE_IDENTITY_NOT_AUTHORIZED, 0, 0, null);
            notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
            break;
        default:
            return;
        }
    }


    private void authenticateOTP(final OTP otp) {
        Status status = getSdk().AuthenticateOTP(getCurrentUser(), otp);
        Log.i(TAG, "STATUS " + status);
        switch (status.getStatusCode()) {
        case OK:
            if (otp.status != null && otp.ttlSeconds > 0) {
                saveCurrentUser(mCurrentUser);
                notifyOutboxHandlers(MESSAGE_SHOW_OTP, 0, 0, otp);
            } else {
                notifyOutboxHandlers(MESSAGE_OTP_NOT_SUPPORTED, 0, 0, null);
                notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
            }
            break;
        case INCORRECT_PIN:
            notifyOutboxHandlers(MESSAGE_INCORRECT_PIN, 0, 0, null);
            onSignIn();
            break;
        case PIN_INPUT_CANCELED:
            notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
            break;
        case NETWORK_ERROR:
            onNetworkError();
            break;
        case IDENTITY_NOT_AUTHORIZED:
            notifyOutboxHandlers(MESSAGE_IDENTITY_NOT_AUTHORIZED, 0, 0, null);
            notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
            break;
        default:
            return;
        }
    }


    private void authenticate() {
        final StringBuilder resultData = new StringBuilder();
        Status status = getSdk().Authenticate(getCurrentUser(), resultData);
        switch (status.getStatusCode()) {
        case OK:
            saveCurrentUser(mCurrentUser);
            notifyOutboxHandlers(MESSAGE_SHOW_LOGGED_IN, 0, 0, null);
            break;
        case INCORRECT_PIN:
            notifyOutboxHandlers(MESSAGE_INCORRECT_PIN, 0, 0, null);
            onSignIn();
            break;
        case PIN_INPUT_CANCELED:
            notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
            break;
        case NETWORK_ERROR:
            onNetworkError();
            break;
        case IDENTITY_NOT_AUTHORIZED:
            notifyOutboxHandlers(MESSAGE_IDENTITY_NOT_AUTHORIZED, 0, 0, null);
            notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
            break;
        default:
            return;
        }
    }


    private void onNetworkError() {
        notifyOutboxHandlers(MESSAGE_NETWORK_ERROR, 0, 0, null);
        notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
    }


    private void onChangeIdentity() {
        if (mConfigsDao.getActiveConfiguration() != null) {
            notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
        } else {
            notifyOutboxHandlers(MESSAGE_NO_ACTIVE_CONFIGURATION, 0, 0, null);
            notifyOutboxHandlers(MESSAGE_SHOW_CONFIGURATIONS_LIST, 0, 0, null);
        }
    }


    private void deleteCurrentIdentity() {
        notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                getSdk().DeleteUser(getCurrentUser());
                saveCurrentUser(null);
                initUsersList();
                notifyOutboxHandlers(MESSAGE_IDENTITY_DELETED, 0, 0, null);
                notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
                notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
            }
        });

    }


    // TODO: this should be message
    public void onAccessNumberEntered(final String accessNumber) {
        notifyOutboxHandlers(MESSAGE_START_WORK_IN_PROGRESS, 0, 0, null);
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                preAuthenticate(accessNumber);
                notifyOutboxHandlers(MESSAGE_STOP_WORK_IN_PROGRESS, 0, 0, null);
            }
        });
    }


    // TODO this should be in model
    private void saveCurrentUser(final User user) {
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                PreferenceManager.getDefaultSharedPreferences(mContext).edit()
                        .putString(PREFERENCE_USER, user != null ? user.getId() : "").commit();
            }
        });
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


    public void onIdentitySelected(User user) {
        mCurrentUser = user;
        onSignIn();
    }


    public void onResetPin(User user) {
        mCurrentUser = user;
        resetPin();
    }


    public void onDeleteIdentity(User user) {
        mCurrentUser = user;
        deleteCurrentIdentity();
    }


    public void setCurrentFragmentTag(String tag) {
        Log.i(TAG, "Current fragment is " + tag);
        mCurrentFragmentTag = tag;
    }


    private void onStart() {
        if (!mWorkerThread.isAlive()) {
            initWorkerThread();
        }
    }


    private void onStop() {
        mWorkerThread.interrupt();
    }


    private void onDestroy() {
        if (mWorkerThread != null && mWorkerThread.getLooper() != null) {
            mWorkerThread.interrupt();
            mWorkerThread.getLooper().quit();
            mWorkerThread = null;
            mWorkerHandler = null;
        }
    }


    private void onBack() {
        if (mCurrentFragmentTag != null) {
            if (mCurrentFragmentTag.equals(FragmentTags.FRAGMENT_ABOUT)) {
                notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
            } else
                if (mCurrentFragmentTag.equals(FragmentTags.FRAGMENT_CONFIGURATIONS_LIST)) {
                    if (mCurrentConfiguration == null) {
                        notifyOutboxHandlers(MESSAGE_GO_BACK, 0, 0, null);
                    } else {
                        notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
                    }

                } else
                    if (mCurrentFragmentTag.equals(FragmentTags.FRAGMENT_CONFIGURATION_EDIT)) {
                        notifyOutboxHandlers(MESSAGE_SHOW_CONFIGURATIONS_LIST, 0, 0, null);
                    } else
                        if (mCurrentFragmentTag.equals(FragmentTags.FRAGMENT_USERS_LIST)) {
                            notifyOutboxHandlers(MESSAGE_GO_BACK, 0, 0, null);
                        } else {
                            notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
                        }
        } else {
            notifyOutboxHandlers(MESSAGE_SHOW_IDENTITIES_LIST, 0, 0, null);
        }
    }


    private void onNetworkConnectionChange() {
        if (isNetworkAvailable()) {
            notifyOutboxHandlers(MESSAGE_INTERNET_CONNECTION_AVAILABLE, 0, 0, null);
        } else {
            notifyOutboxHandlers(MESSAGE_NO_INTERNET_CONNECTION_AVAILABLE, 0, 0, null);
        }
    }


    public int getAccessNumberLength() {
        mAccessNumberLength = null;
        mWorkerHandler.post(new Runnable() {

            @Override
            public void run() {
                synchronized (mSDKLockObject) {
                    mAccessNumberLength = getSdk().GetClientParam("accessNumberDigits");
                    mSDKLockObject.notifyAll();
                }
            }
        });

        synchronized (mSDKLockObject) {
            try {
                while (mAccessNumberLength == null) {
                    mSDKLockObject.wait();
                }
                return Integer.parseInt(mAccessNumberLength);
            } catch (InterruptedException e) {
                return -1;
            }
        }
    }


    private boolean isNetworkAvailable() {
        ConnectivityManager connectivityManager = ((ConnectivityManager) mContext
                .getSystemService(Context.CONNECTIVITY_SERVICE));
        return connectivityManager.getActiveNetworkInfo() != null
                && connectivityManager.getActiveNetworkInfo().isConnected();
    }
}
