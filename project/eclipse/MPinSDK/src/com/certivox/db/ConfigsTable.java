package com.certivox.db;

import android.content.ContentValues;
import android.content.Context;
import android.database.sqlite.SQLiteDatabase;

import com.certivox.db.ConfigsContract.ConfigEntry;
import com.certivox.mpinsdk.Config;

public class ConfigsTable {

	private SQLiteDatabase mDb;
	private Context mContext;

	public ConfigsTable(Context context, SQLiteDatabase db) {
		mContext = context;
		mDb = db;
	}

	public void createAndInitTable() {
		// Create the Table
		mDb.execSQL(DbQueries.createConfgisTableQuery());
		// Populate the table with initial data
		populateTable();
	}

	private void populateTable() {
		Config configurationMobileBanking = new Config("Mobile banking login",
				"http://tcb.certivox.org", false, false);
		Config configurationOnlineBanking = new Config("Online banking login",
				"http://tcb.certivox.org", false, true);
		Config configurationVPNLogin = new Config("VPN login",
				"http://otp.m-pin.id", true, false);

		insertConfig(configurationMobileBanking);
		insertConfig(configurationOnlineBanking);
		insertConfig(configurationVPNLogin);
	}

	private void insertConfig(Config config) {
		ContentValues sampleVals = new ContentValues();
		config.toContentValues(sampleVals);
		mDb.insert(ConfigEntry.TABLE_NAME, null, sampleVals);
	}

}
