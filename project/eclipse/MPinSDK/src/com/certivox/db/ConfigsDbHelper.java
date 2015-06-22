package com.certivox.db;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

public final class ConfigsDbHelper extends SQLiteOpenHelper {

	/* Constants for db */
	public static final int DATABASE_VERSION = 1;
	public static final String DATABASE_NAME = "configs.db";

	public ConfigsDbHelper(Context context) {
		super(context, DATABASE_NAME, null, DATABASE_VERSION);
	}

	public void onCreate(SQLiteDatabase db) {
		// Create the DB schema and populate it
		ConfigsTable configsTable = new ConfigsTable(db);
		configsTable.createAndInitTable();
	}

	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		db.execSQL(ConfigsTable.deleteConfigsTableQuery());
		onCreate(db);
	}

	public void onDowngrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		onUpgrade(db, oldVersion, newVersion);
	}
}
