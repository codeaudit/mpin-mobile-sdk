package com.certivox.mpinsdk;

import android.content.ContentValues;
import android.database.Cursor;

import com.certivox.db.ConfigsContract.ConfigEntry;

public final class Config {
	
	public Config() {
		m_id = -1;
	}
	
	public Config(String title, String backendUrl, boolean requestOtp, boolean requestAccessNumber) {
		m_id = -1;
		m_title = title;
		m_backendUrl = backendUrl;
		m_requestOtp = requestOtp;
		m_requestAccessNumber = requestAccessNumber;
	}

	public long getId() {
		return m_id;
	}

	public void setId(long id) {
		m_id = id;
	}

	public String getTitle() {
		return m_title;
	}
	
	public void setTitle(String title) {
		m_title = title;
	}
	
	public String getBackendUrl() {
		return m_backendUrl;
	}
	
	public void setBackendUrl(String backendUrl) {
		m_backendUrl = backendUrl;
	}
	
	public boolean getRequestOtp() {
		return m_requestOtp;
	}
	
	public void setRequestOtp(boolean requestOtp) {
		m_requestOtp = requestOtp;
	}
	
	public boolean getRequestAccessNumber() {
		return m_requestAccessNumber;
	}
	
	public void setRequestAccessNumber(boolean requestAccessNumber) {
		m_requestAccessNumber = requestAccessNumber;
	}
	
	public void toContentValues(ContentValues values) {
		values.put(ConfigEntry.COLUMN_NAME_TITLE, getTitle());
		values.put(ConfigEntry.COLUMN_NAME_BACKEND_URL, getBackendUrl());
		values.put(ConfigEntry.COLUMN_NAME_REQUEST_OTP, getRequestOtp());
		values.put(ConfigEntry.COLUMN_NAME_REQUEST_ACCESS_NUMBER, getRequestAccessNumber());
	}
	
	public void formCursor(Cursor cursor) {
		setId(cursor.getLong(cursor.getColumnIndexOrThrow(ConfigEntry._ID)));
		setTitle(cursor.getString(cursor.getColumnIndexOrThrow(ConfigEntry.COLUMN_NAME_TITLE)));
		setBackendUrl(cursor.getString(cursor.getColumnIndexOrThrow(ConfigEntry.COLUMN_NAME_BACKEND_URL)));
		setRequestOtp(cursor.getInt(cursor.getColumnIndexOrThrow(ConfigEntry.COLUMN_NAME_REQUEST_OTP)) == 1);
		setRequestAccessNumber(cursor.getInt(cursor.getColumnIndexOrThrow(ConfigEntry.COLUMN_NAME_REQUEST_ACCESS_NUMBER)) == 1);
	}
	
	private long m_id;
	private String m_title;
	private String m_backendUrl;
	private boolean m_requestOtp;
	private boolean m_requestAccessNumber;
	
}
