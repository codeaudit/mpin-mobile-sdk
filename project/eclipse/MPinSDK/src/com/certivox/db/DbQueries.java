package com.certivox.db;

import com.certivox.db.ConfigsContract.ConfigEntry;

public class DbQueries {

	private static final String TYPE_ID = " INTEGER PRIMARY KEY";
	private static final String TYPE_TEXT = " TEXT";
	private static final String TYPE_BOOLEAN = " BOOLEAN";

	public static String createConfgisTableQuery() {
		return String.format("CREATE TABLE IF NOT EXISTS "
				+ ConfigEntry.TABLE_NAME + " (" + ConfigEntry._ID + TYPE_ID
				+ ", " + ConfigEntry.COLUMN_TITLE + TYPE_TEXT + ", "
				+ ConfigEntry.COLUMN_BACKEND_URL + TYPE_TEXT + ", "
				+ ConfigEntry.COLUMN_RTS + TYPE_TEXT + ", "
				+ ConfigEntry.COLUMN_REQUEST_OTP + TYPE_BOOLEAN + ", "
				+ ConfigEntry.COLUMN_REQUEST_ACCESS_NUMBER + TYPE_BOOLEAN
				+ ")");
	}

	public static String deleteConfigsTableQuery() {
		return "DROP TABLE IF EXISTS " + ConfigEntry.TABLE_NAME;
	}

	private DbQueries() {

	}
}
