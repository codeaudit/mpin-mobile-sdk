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
                + ", " + ConfigEntry.COLUMN_REQUEST_ACCESS_NUMBER + TYPE_BOOLEAN + ")");
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
//        Config configurationMobileBanking = new Config("Mobile banking login", "http://tcb.certivox.org", false, false);
//        Config configurationOnlineBanking = new Config("Online banking login", "http://tcb.certivox.org", false, true);
//        Config configurationVPNLogin = new Config("VPN login", "http://otp.m-pin.id", true, false);

        Config configurationMPinConnect = new Config("M-Pin Connect", "https://m-pin.my.id", false, true);

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
