package com.certivox.db;

import android.provider.BaseColumns;

public final class ConfigsContract {

	public static final class ConfigEntry implements BaseColumns {
        public static final String TABLE_NAME = "configs";
        public static final String COLUMN_NAME_TITLE = "title";
        public static final String COLUMN_NAME_BACKEND_URL = "backend_url";
    	public static final String COLUMN_NAME_REQUEST_OTP = "request_otp";
    	public static final String COLUMN_NAME_REQUEST_ACCESS_NUMBER = "request_access_number";
    	
    	public static final String[] getFullProjection() {
    		return new String[] {
	    		_ID,
	    		COLUMN_NAME_TITLE,
	    		COLUMN_NAME_BACKEND_URL,
	    		COLUMN_NAME_REQUEST_OTP,
	    		COLUMN_NAME_REQUEST_ACCESS_NUMBER
    		};
    	};
    	
    	private ConfigEntry() {}
    }
	
	private ConfigsContract() {}
	
}
