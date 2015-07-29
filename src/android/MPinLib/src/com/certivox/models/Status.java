package com.certivox.models;


public class Status {

    public static enum Code {
        OK, PIN_INPUT_CANCELED, // Local error, returned when user cancels pin entering
        CRYPTO_ERROR, // Local error in crypto functions
        STORAGE_ERROR, // Local storage related error
        NETWORK_ERROR, // Local error - cannot connect to remote server (no internet, or invalid server/port)
        RESPONSE_PARSE_ERROR, // Local error - cannot parse json response from remote server (invalid json or unexpected json structure)
        FLOW_ERROR, // Local error - unproper MPinSDK class usage
        IDENTITY_NOT_AUTHORIZED, // Remote error - the remote server refuses user registration
        IDENTITY_NOT_VERIFIED, // Remote error - the remote server refuses user registration because identity is not verified
        REQUEST_EXPIRED, // Remote error - the register/authentication request expired
        REVOKED, // Remote error - cannot get time permit (propably the user is temporary suspended)
        INCORRECT_PIN, // Remote error - user entered wrong pin
        INCORRECT_ACCESS_NUMBER, // Remote/local error - wrong access number (checksum failed or RPS returned 412)
        HTTP_SERVER_ERROR, // Remote error, that was not reduced to one of the above - the remote server returned internal server error status (5xx)
        HTTP_REQUEST_ERROR // Remote error, that was not reduced to one of the above - invalid data sent to server, the remote server returned 4xx error status
    }


    public Status(int statusCode, String error) {
        mStatusCode = Code.values()[statusCode];
        mErrorMessage = error;
    }


    public Code getStatusCode() {
        return mStatusCode;
    }


    public String getErrorMessage() {
        return mErrorMessage;
    }


    @Override
    public String toString() {
        return "Status [StatusCode=" + mStatusCode + ", ErrorMessage='" + mErrorMessage + "']";
    }

    private final Code   mStatusCode;
    private final String mErrorMessage;

}
