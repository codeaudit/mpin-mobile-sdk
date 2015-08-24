package com.certivox.dal;


import android.content.Context;
import android.preference.PreferenceManager;


public class AppInstanceInfoDao {

    private Context             mContext;
    public static final String  KEY_FIRST_START              = "first_start";
    private static final String KEY_HAS_AUTHENTICATED        = "has_authenticated";
    private static final String KEY_CONFIGURATION_LIST_SHOWN = "configuration_list_been_shown";


    public AppInstanceInfoDao(Context context) {
        mContext = context;
    }


    /**
     * Checks whether the application is started for a first time after installation.
     * 
     * @return <code>true</code> if the application is started for a first time after installation and
     *         <code>false</code> otherwise.
     */
    public boolean isFirstStart() {
        return PreferenceManager.getDefaultSharedPreferences(mContext).getBoolean(KEY_FIRST_START, true);
    }


    /**
     * Set if the application was already started.
     */
    public void setIsFirstStart(boolean isFirstStart) {
        PreferenceManager.getDefaultSharedPreferences(mContext).edit().putBoolean(KEY_FIRST_START, isFirstStart)
                .commit();
    }


    /**
     * Checks whether the user has authenticated before.
     * 
     * @return whether the user has authenticated before
     */
    public boolean hasAuthenticatedToMpinConnect() {
        return PreferenceManager.getDefaultSharedPreferences(mContext).getBoolean(KEY_HAS_AUTHENTICATED, false);
    }


    /**
     * Set whether the user has authenticated before or not.
     * 
     * @param hasAuthenticated
     *            - whether the user has authenticated before
     */
    public void setHasAuthenticatedToMpinConnect(boolean hasAuthenticated) {
        PreferenceManager.getDefaultSharedPreferences(mContext).edit()
                .putBoolean(KEY_HAS_AUTHENTICATED, hasAuthenticated).commit();
    }


    /**
     * Checks whether the configuration list has been shown before.
     * 
     * @return whether the configuration list has been shown before
     */
    public boolean hasConfigurationListBeenShown() {
        return PreferenceManager.getDefaultSharedPreferences(mContext).getBoolean(KEY_CONFIGURATION_LIST_SHOWN, false);
    }


    /**
     * Set whether the configuration list has been shown before.
     * 
     * @param hasAuthenticated
     *            - whether the the configuration list has been shown before
     */
    public void setConfigurationListBeenShown(boolean configListShown) {
        PreferenceManager.getDefaultSharedPreferences(mContext).edit()
                .putBoolean(KEY_CONFIGURATION_LIST_SHOWN, configListShown).commit();
    }

}
