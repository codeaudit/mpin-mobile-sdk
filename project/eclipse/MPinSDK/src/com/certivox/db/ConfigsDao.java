package com.certivox.db;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

import com.certivox.db.ConfigsContract.ConfigEntry;
import com.certivox.mpinsdk.Config;

public class ConfigsDao {

	public static Cursor getConfigs(Context context) {
		SQLiteDatabase db = new ConfigsDbHelper(context).getReadableDatabase();
		Cursor cursor = null;

		cursor = db.query(ConfigEntry.TABLE_NAME,
				ConfigEntry.getFullProjection(), null, null, null, null, null);

		return cursor;
	}

	public static Config getConfigurationById(Context context, long id) {
		Log.i("DEBUG", "getConfigurationById id = " + id);
		if (id == -1 || context == null) {
			return null;
		}

		SQLiteDatabase db = new ConfigsDbHelper(context).getReadableDatabase();
		Cursor cursor = null;
		try {
			cursor = db.query(ConfigEntry.TABLE_NAME,
					ConfigEntry.getFullProjection(), ConfigEntry._ID
							+ " LIKE ?", new String[] { String.valueOf(id) },
					null, null, null);
			if (cursor.moveToFirst()) {
				Config config = new Config();
				config.formCursor(cursor);
				return config;
			}
		} finally {
			if (cursor != null)
				cursor.close();
		}
		return null;
	}

	public static Cursor deleteConfigurationById(Context context, long configId) {
		SQLiteDatabase db = new ConfigsDbHelper(context).getWritableDatabase();

		db.delete(ConfigEntry.TABLE_NAME, ConfigEntry._ID + " LIKE ?",
				new String[] { String.valueOf(configId) });

		Cursor cursor = db.query(ConfigEntry.TABLE_NAME,
				ConfigEntry.getFullProjection(), null, null, null, null, null);

		return cursor;
	}

}
