package com.certivox.dal;


import android.provider.BaseColumns;


public final class ConfigsContract {

    public static final class ConfigEntry implements BaseColumns {

        /* Constants for table "configs" */
        public static final String TABLE_NAME                   = "configs";
        public static final String COLUMN_TITLE                 = "title";
        public static final String COLUMN_BACKEND_URL           = "backend_url";
        public static final String COLUMN_RTS                   = "rts";                   ;
        public static final String COLUMN_REQUEST_OTP           = "request_otp";
        public static final String COLUMN_REQUEST_ACCESS_NUMBER = "request_access_number";


        public static final String[] getFullProjection() {
            return new String[] {
                    _ID, COLUMN_TITLE, COLUMN_BACKEND_URL, COLUMN_RTS, COLUMN_REQUEST_OTP, COLUMN_REQUEST_ACCESS_NUMBER
            };
        };


        private ConfigEntry() {
        }
    }


    private ConfigsContract() {
    }

}
