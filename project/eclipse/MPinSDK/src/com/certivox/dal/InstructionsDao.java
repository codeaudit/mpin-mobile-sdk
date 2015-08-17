package com.certivox.dal;


import android.content.Context;
import android.preference.PreferenceManager;


public class InstructionsDao {

    private Context            mContext;
    public static final String KEY_FIRST_START = "first_start";


    public InstructionsDao(Context context) {
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

}
