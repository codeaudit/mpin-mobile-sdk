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


import android.content.ContentValues;
import android.content.Context;
import android.database.sqlite.SQLiteDatabase;

import com.certivox.dal.ConfigsContract.ConfigEntry;
import com.certivox.models.Config;


public class ConfigsTable {

    private SQLiteDatabase      mDb;
    private ConfigsDao          mConfigsDao;
    private Context             mContext;

    private static final String TYPE_ID      = " INTEGER PRIMARY KEY";
    private static final String TYPE_TEXT    = " TEXT";
    private static final String TYPE_BOOLEAN = " BOOLEAN";


    private static String createConfgisTableQuery() {
        return String.format("CREATE TABLE IF NOT EXISTS " + ConfigEntry.TABLE_NAME + " (" + ConfigEntry._ID + TYPE_ID
                + ", " + ConfigEntry.COLUMN_TITLE + TYPE_TEXT + ", " + ConfigEntry.COLUMN_BACKEND_URL + TYPE_TEXT
                + ", " + ConfigEntry.COLUMN_RTS + TYPE_TEXT + ", " + ConfigEntry.COLUMN_REQUEST_OTP + TYPE_BOOLEAN
                + ", " + ConfigEntry.COLUMN_REQUEST_ACCESS_NUMBER + TYPE_BOOLEAN + ", " + ConfigEntry.COLUMN_IS_DEFAULT
                + TYPE_BOOLEAN + ")");
    }


    public static String deleteConfigsTableQuery() {
        return "DROP TABLE IF EXISTS " + ConfigEntry.TABLE_NAME;
    }


    public ConfigsTable(Context context, SQLiteDatabase db) {
        mDb = db;
        mContext = context;
        mConfigsDao = new ConfigsDao(mContext);
    }


    public void createAndInitTable() {
        // Create the Table
        mDb.execSQL(createConfgisTableQuery());
        // Populate the table with initial data
        populateTable();
    }


    private void populateTable() {
        // Config configurationMobileBanking = new Config("Mobile banking login", "http://tcb.certivox.org", false,
        // false);
        // Config configurationOnlineBanking = new Config("Online banking login", "http://tcb.certivox.org", false,
        // true);
        // Config configurationVPNLogin = new Config("VPN login", "http://otp.m-pin.id", true, false);

        Config configurationMPinConnect = new Config("M-Pin Connect", "https://m-pin.my.id", false, true, true);

        // insertConfig(configurationMobileBanking);
        // insertConfig(configurationOnlineBanking);
        // insertConfig(configurationVPNLogin);

        insertConfig(configurationMPinConnect);
    }


    // TODO check if this could be done in the DAO
    private void insertConfig(Config config) {
        ContentValues sampleVals = mConfigsDao.toContentValues(config);
        mDb.insert(ConfigEntry.TABLE_NAME, null, sampleVals);
    }

}
