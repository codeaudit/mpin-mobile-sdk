package com.certivox.db;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.preference.PreferenceManager;
import android.util.Log;

import com.certivox.db.ConfigsContract.ConfigEntry;
import com.certivox.models.Config;

public class ConfigsDao {

	private SQLiteDatabase mDb;
	private Context mContext;
	public static final String KEY_ACTIVE_CONFIG = "active_config";

	public ConfigsDao(Context context) {
		mContext = context;
		mDb = new ConfigsDbHelper(context).getReadableDatabase();
	}

	public Cursor getConfigs() {
		Cursor cursor = null;

		cursor = mDb.query(ConfigEntry.TABLE_NAME,
				ConfigEntry.getFullProjection(), null, null, null, null, null);

		return cursor;
	}

	public Config getConfigurationById(long id) {
		Log.i("DEBUG", "getConfigurationById id = " + id);
		if (id == -1) {
			return null;
		}

		Cursor cursor = null;
		try {
			cursor = mDb.query(ConfigEntry.TABLE_NAME,
					ConfigEntry.getFullProjection(), ConfigEntry._ID
							+ " LIKE ?", new String[] { String.valueOf(id) },
					null, null, null);
			if (cursor.moveToFirst()) {
				Config config = new Config();
				config = getByCursor(cursor);
				return config;
			}
		} finally {
			if (cursor != null)
				cursor.close();
		}
		return null;
	}

	public Cursor deleteConfigurationById(long configId) {

		mDb.delete(ConfigEntry.TABLE_NAME, ConfigEntry._ID + " LIKE ?",
				new String[] { String.valueOf(configId) });

		Cursor cursor = mDb.query(ConfigEntry.TABLE_NAME,
				ConfigEntry.getFullProjection(), null, null, null, null, null);

		return cursor;
	}

	public Config saveOrUpdate(Config config) {
		ContentValues values = toContentValues(config);

		if (config.getId() == -1) {
			config.setId(mDb.insert(ConfigEntry.TABLE_NAME, null, values));
		} else {
			mDb.update(ConfigEntry.TABLE_NAME, values, ConfigEntry._ID
					+ " LIKE ?",
					new String[] { String.valueOf(config.getId()) });
		}
		return config;
	}

	public Config getByCursor(Cursor cursor) {
		Config config = new Config();
		config.setId(cursor.getLong(cursor
				.getColumnIndexOrThrow(ConfigEntry._ID)));
		config.setTitle(cursor.getString(cursor
				.getColumnIndexOrThrow(ConfigEntry.COLUMN_TITLE)));
		config.setBackendUrl(cursor.getString(cursor
				.getColumnIndexOrThrow(ConfigEntry.COLUMN_BACKEND_URL)));
		config.setRTS(cursor.getString(cursor
				.getColumnIndexOrThrow(ConfigEntry.COLUMN_RTS)));
		config.setRequestOtp(cursor.getInt(cursor
				.getColumnIndexOrThrow(ConfigEntry.COLUMN_REQUEST_OTP)) == 1);
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
		values.put(ConfigEntry.COLUMN_REQUEST_ACCESS_NUMBER,
				config.getRequestAccessNumber());

		return values;
	}

	public Config getActiveConfiguration() {
		long id = PreferenceManager.getDefaultSharedPreferences(mContext)
				.getLong(KEY_ACTIVE_CONFIG, -1);

		return getConfigurationById(id);
	}

}
