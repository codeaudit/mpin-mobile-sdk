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
