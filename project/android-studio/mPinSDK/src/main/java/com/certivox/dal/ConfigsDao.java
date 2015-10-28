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


import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

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


    public Config getDefaultConfiguration() {
        Config config = null;

        SQLiteDatabase db = new ConfigsDbHelper(mContext).getReadableDatabase();
        Cursor cursor = db.query(ConfigEntry.TABLE_NAME, ConfigEntry.getFullProjection(), ConfigEntry.COLUMN_IS_DEFAULT
                + " LIKE ?", new String[] {
            "1"
        }, null, null, null);

        if (cursor.moveToFirst()) {
            config = getByCursor(cursor);
        }
        cursor.close();
        db.close();

        return config;
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
        config.setIsDefault(cursor.getInt(cursor.getColumnIndexOrThrow(ConfigEntry.COLUMN_IS_DEFAULT)) == 1);

        return config;
    }


    public ContentValues toContentValues(Config config) {
        ContentValues values = new ContentValues();
        values.put(ConfigEntry.COLUMN_TITLE, config.getTitle());
        values.put(ConfigEntry.COLUMN_BACKEND_URL, config.getBackendUrl());
        values.put(ConfigEntry.COLUMN_RTS, config.getRTS());
        values.put(ConfigEntry.COLUMN_REQUEST_OTP, config.getRequestOtp());
        values.put(ConfigEntry.COLUMN_REQUEST_ACCESS_NUMBER, config.getRequestAccessNumber());
        values.put(ConfigEntry.COLUMN_IS_DEFAULT, config.isDefault());

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


    public ArrayList<Config> getConfigsByJsonArray(JSONArray jsonArray) {
        ArrayList<Config> configurations = new ArrayList<Config>();
        JSONObject currentJsonObject;
        Config currentConfig;
        String currentType;
        for (int i = 0; i < jsonArray.length(); i++) {
            try {
                currentJsonObject = jsonArray.getJSONObject(i);
                currentConfig = new Config();
                currentConfig.setBackendUrl(currentJsonObject.getString("url"));
                currentConfig.setTitle(currentJsonObject.getString("name"));
                currentType = currentJsonObject.getString("type");
                if (currentType.equals("otp")) {
                    currentConfig.setRequestOtp(true);
                } else
                    if (currentType.equals("online")) {
                        currentConfig.setRequestAccessNumber(true);
                    }
                configurations.add(currentConfig);
            } catch (JSONException e) {
                //Nothing to do
            }
        }

        return configurations;
    }
}
