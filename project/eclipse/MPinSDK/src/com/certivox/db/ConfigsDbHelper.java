package com.certivox.db;

import android.content.ContentValues;
import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import com.certivox.db.ConfigsContract.ConfigEntry;
import com.certivox.mpinsdk.Config;

public final class ConfigsDbHelper extends SQLiteOpenHelper {

	public static final int DATABASE_VERSION = 1;
    public static final String DATABASE_NAME = "configs.db";

    private final Context m_context;
    
    public ConfigsDbHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
        m_context = context;
    }
    
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(SQL_CREATE_ENTRIES);
        
        ContentValues sampleVals = new ContentValues();
        
        Config cfg = new Config("Mobile banking login", "http://tcb.certivox.org", false, false);
        cfg.toContentValues(sampleVals);
        db.insert(ConfigEntry.TABLE_NAME, null, sampleVals);
        
        cfg = new Config("Online banking login", "http://tcb.certivox.org", false, true);
        cfg.toContentValues(sampleVals);
        db.insert(ConfigEntry.TABLE_NAME, null, sampleVals);
        
        cfg = new Config("VPN login", "http://otp.m-pin.id", true, false);
        cfg.toContentValues(sampleVals);
        db.insert(ConfigEntry.TABLE_NAME, null, sampleVals);

    }
    
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL(SQL_DELETE_ENTRIES);
        onCreate(db);
    }
    
    public void onDowngrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        onUpgrade(db, oldVersion, newVersion);
    }
	
    private static final String TYPE_ID = " INTEGER PRIMARY KEY";
	private static final String TYPE_TEXT = " TEXT";
	private static final String TYPE_BOOLEAN = " BOOLEAN";
	
	private static final String SQL_CREATE_ENTRIES =
	    "CREATE TABLE IF NOT EXISTS " + ConfigEntry.TABLE_NAME + " (" +
	    		ConfigEntry._ID + TYPE_ID + ", " +
	    		ConfigEntry.COLUMN_NAME_TITLE + TYPE_TEXT + ", " +
	    		ConfigEntry.COLUMN_NAME_BACKEND_URL + TYPE_TEXT + ", " +
	    		ConfigEntry.COLUMN_NAME_REQUEST_OTP + TYPE_BOOLEAN + ", " +
	    		ConfigEntry.COLUMN_NAME_REQUEST_ACCESS_NUMBER + TYPE_BOOLEAN + ")";

	private static final String SQL_DELETE_ENTRIES = "DROP TABLE IF EXISTS " + ConfigEntry.TABLE_NAME;
	
}
