package com.certivox.mpinsdk;

import android.content.ContentValues;
import android.database.Cursor;

import com.certivox.db.ConfigsContract.ConfigEntry;

public final class Config {

	private long mId;
	private String mTitle;
	private String mBackendUrl;
	private String mRTS;
	private boolean mRequestOtp;
	private boolean mRequestAccessNumber;

	public Config() {
		mId = -1;
	}

	public Config(String title, String backendUrl, boolean requestOtp,
			boolean requestAccessNumber) {
		mId = -1;
		mTitle = title;
		mBackendUrl = backendUrl;
		mRequestOtp = requestOtp;
		mRequestAccessNumber = requestAccessNumber;
		mRTS = "";
	}

	public Config(String title, String backendUrl, String rts,
			boolean requestOtp, boolean requestAccessNumber) {
		this(title, backendUrl, requestOtp, requestAccessNumber);
		mRTS = rts;
	}

	public long getId() {
		return mId;
	}

	public void setId(long id) {
		mId = id;
	}

	public String getTitle() {
		return mTitle;
	}

	public void setTitle(String title) {
		mTitle = title;
	}

	public String getBackendUrl() {
		return mBackendUrl;
	}

	public void setBackendUrl(String backendUrl) {
		mBackendUrl = backendUrl;
	}

	public String getRTS() {
		return mRTS;
	}

	public void setRTS(String rts) {
		mRTS = rts;
	}

	public boolean getRequestOtp() {
		return mRequestOtp;
	}

	public void setRequestOtp(boolean requestOtp) {
		mRequestOtp = requestOtp;
	}

	public boolean getRequestAccessNumber() {
		return mRequestAccessNumber;
	}

	public void setRequestAccessNumber(boolean requestAccessNumber) {
		mRequestAccessNumber = requestAccessNumber;
	}

	public void toContentValues(ContentValues values) {
		values.put(ConfigEntry.COLUMN_TITLE, getTitle());
		values.put(ConfigEntry.COLUMN_BACKEND_URL, getBackendUrl());
		values.put(ConfigEntry.COLUMN_RTS, getRTS());
		values.put(ConfigEntry.COLUMN_REQUEST_OTP, getRequestOtp());
		values.put(ConfigEntry.COLUMN_REQUEST_ACCESS_NUMBER,
				getRequestAccessNumber());
	}

	public void formCursor(Cursor cursor) {
		setId(cursor.getLong(cursor.getColumnIndexOrThrow(ConfigEntry._ID)));
		setTitle(cursor.getString(cursor
				.getColumnIndexOrThrow(ConfigEntry.COLUMN_TITLE)));
		setBackendUrl(cursor.getString(cursor
				.getColumnIndexOrThrow(ConfigEntry.COLUMN_BACKEND_URL)));
		setRTS(cursor.getString(cursor
				.getColumnIndexOrThrow(ConfigEntry.COLUMN_RTS)));
		setRequestOtp(cursor.getInt(cursor
				.getColumnIndexOrThrow(ConfigEntry.COLUMN_REQUEST_OTP)) == 1);
		setRequestAccessNumber(cursor
				.getInt(cursor
						.getColumnIndexOrThrow(ConfigEntry.COLUMN_REQUEST_ACCESS_NUMBER)) == 1);
	}
}
