package com.certivox.dal;


import java.util.ArrayList;
import java.util.List;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.preference.PreferenceManager;
import android.util.Log;

import com.certivox.dal.ConfigsContract.ConfigEntry;
import com.certivox.models.Config;


public class ConfigsDao {

    private Context            mContext;
    public static final String KEY_ACTIVE_CONFIG = "active_config";


    public ConfigsDao(Context context) {
        mContext = context;
    }


    public Config getConfigurationById(long id) {
        Log.i("DEBUG", "getConfigurationById id = " + id);
        if (id == -1) {
            return null;
        }

        SQLiteDatabase db = new ConfigsDbHelper(mContext).getReadableDatabase();
        Cursor cursor = db.query(ConfigEntry.TABLE_NAME, ConfigEntry.getFullProjection(), ConfigEntry._ID + " LIKE ?",
                new String[] {
                    String.valueOf(id)
                }, null, null, null);

        Config config = null;
        if (cursor.moveToFirst()) {
            config = getByCursor(cursor);
        }

        cursor.close();
        db.close();
        return config;
    }


    public void deleteConfigurationById(long configId) {

        SQLiteDatabase db = new ConfigsDbHelper(mContext).getReadableDatabase();
        db.delete(ConfigEntry.TABLE_NAME, ConfigEntry._ID + " LIKE ?", new String[] {
            String.valueOf(configId)
        });
        db.close();
    }


    public Config saveOrUpdate(Config config) {
        ContentValues values = toContentValues(config);
        SQLiteDatabase db = new ConfigsDbHelper(mContext).getReadableDatabase();
        if (config.getId() == -1) {
            config.setId(db.insert(ConfigEntry.TABLE_NAME, null, values));
        } else {
            db.update(ConfigEntry.TABLE_NAME, values, ConfigEntry._ID + " LIKE ?", new String[] {
                String.valueOf(config.getId())
            });
        }

        db.close();
        return config;
    }


    public Config getByCursor(Cursor cursor) {
        Config config = new Config();
        config.setId(cursor.getLong(cursor.getColumnIndexOrThrow(ConfigEntry._ID)));
        config.setTitle(cursor.getString(cursor.getColumnIndexOrThrow(ConfigEntry.COLUMN_TITLE)));
        config.setBackendUrl(cursor.getString(cursor.getColumnIndexOrThrow(ConfigEntry.COLUMN_BACKEND_URL)));
        config.setRTS(cursor.getString(cursor.getColumnIndexOrThrow(ConfigEntry.COLUMN_RTS)));
        config.setRequestOtp(cursor.getInt(cursor.getColumnIndexOrThrow(ConfigEntry.COLUMN_REQUEST_OTP)) == 1);
        config.setRequestAccessNumber(cursor.getInt(cursor
                .getColumnIndexOrThrow(ConfigEntry.COLUMN_REQUEST_ACCESS_NUMBER)) == 1);

        return config;
    }


    public ContentValues toContentValues(Config config) {
        ContentValues values = new ContentValues();
        values.put(ConfigEntry.COLUMN_TITLE, config.getTitle());
        values.put(ConfigEntry.COLUMN_BACKEND_URL, config.getBackendUrl());
        values.put(ConfigEntry.COLUMN_RTS, config.getRTS());
        values.put(ConfigEntry.COLUMN_REQUEST_OTP, config.getRequestOtp());
        values.put(ConfigEntry.COLUMN_REQUEST_ACCESS_NUMBER, config.getRequestAccessNumber());

        return values;
    }


    public Config getActiveConfiguration() {
        return getConfigurationById(getActiveConfigurationId());
    }


    public long getActiveConfigurationId() {
        return PreferenceManager.getDefaultSharedPreferences(mContext).getLong(KEY_ACTIVE_CONFIG, -1);
    }


    public void setActiveConfig(Config config) {

        long id = config != null ? config.getId() : -1;

        PreferenceManager.getDefaultSharedPreferences(mContext).edit().putLong(KEY_ACTIVE_CONFIG, id).commit();
    }


    public List<Config> getListConfigs() {
        Cursor cursor = new ConfigsDbHelper(mContext).getReadableDatabase().query(ConfigEntry.TABLE_NAME,
                ConfigEntry.getFullProjection(), null, null, null, null, null);

        ArrayList<Config> configurations = new ArrayList<Config>();
        while (cursor.moveToNext()) {
            configurations.add(getByCursor(cursor));
        }

        cursor.close();

        return configurations;
    }
}