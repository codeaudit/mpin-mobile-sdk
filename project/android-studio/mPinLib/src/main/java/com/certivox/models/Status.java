/*******************************************************************************
 * Copyright (c) 2012-2015, Certivox All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * For full details regarding our CertiVox terms of service please refer to the following links:
 * 
 * * Our Terms and Conditions - http://www.certivox.com/about-certivox/terms-and-conditions/
 * 
 * * Our Security and Privacy - http://www.certivox.com/about-certivox/security-privacy/
 * 
 * * Our Statement of Position and Our Promise on Software Patents - http://www.certivox.com/about-certivox/patents/
 ******************************************************************************/
package com.certivox.models;


public class Status {

    public static enum Code {
        OK, PIN_INPUT_CANCELED, // Local error, returned when user cancels pin entering
        CRYPTO_ERROR, // Local error in crypto functions
        STORAGE_ERROR, // Local storage related error
        NETWORK_ERROR, // Local error - cannot connect to remote server (no internet, or invalid server/port)
        RESPONSE_PARSE_ERROR, // Local error - cannot parse json response from remote server (invalid json or unexpected
                              // json structure)
        FLOW_ERROR, // Local error - unproper MPinSDK class usage
        IDENTITY_NOT_AUTHORIZED, // Remote error - the remote server refuses user registration
        IDENTITY_NOT_VERIFIED, // Remote error - the remote server refuses user registration because identity is not
                               // verified
        REQUEST_EXPIRED, // Remote error - the register/authentication request expired
        REVOKED, // Remote error - cannot get time permit (propably the user is temporary suspended)
        INCORRECT_PIN, // Remote error - user entered wrong pin
        INCORRECT_ACCESS_NUMBER, // Remote/local error - wrong access number (checksum failed or RPS returned 412)
        HTTP_SERVER_ERROR, // Remote error, that was not reduced to one of the above - the remote server returned
                           // internal server error status (5xx)
        HTTP_REQUEST_ERROR // Remote error, that was not reduced to one of the above - invalid data sent to server, the
                           // remote server returned 4xx error status
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
