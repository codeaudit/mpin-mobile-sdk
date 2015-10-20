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
package com.certivox.mpinsdk;


import java.io.Closeable;
import java.util.List;
import java.util.Map;

import android.content.Context;

import com.certivox.models.OTP;
import com.certivox.models.Status;
import com.certivox.models.User;


public class MPinSDKv2 implements Closeable {

    public static final String CONFIG_BACKEND = "backend";

	public MPinSDKv2() {
        mPtr = nConstruct();
    }

    @Override
    public void close() {
        synchronized (this) {
            nDestruct(mPtr);
            mPtr = 0;
        }
    }

    @Override
    protected void finalize() throws Throwable {
        close();
        super.finalize();
    }

    public Status Init(Map<String, String> config, Context context) {
    	return nInit(mPtr, config, context);
    }
    
    public Status TestBackend(String server) {
        return nTestBackend(mPtr, server);
    }

    public Status TestBackend(String server, String rpsPrefix) {
        return nTestBackendRPS(mPtr, server, rpsPrefix);
    }

    public Status SetBackend(String server) {
        return nSetBackend(mPtr, server);
    }

    public Status SetBackend(String server, String rpsPrefix) {
        return nSetBackendRPS(mPtr, server, rpsPrefix);
    }

    public User MakeNewUser(String id) {
        return nMakeNewUser(mPtr, id, "");
    }

    public User MakeNewUser(String id, String deviceName) {
        return nMakeNewUser(mPtr, id, deviceName);
    }

    public Status StartRegistration(User user) {
        return nStartRegistration(mPtr, user, "");
    }

    public Status StartRegistration(User user, String userData) {
        return nStartRegistration(mPtr, user, userData);
    }

    public Status RestartRegistration(User user) {
        return nRestartRegistration(mPtr, user, "");
    }

    public Status RestartRegistration(User user, String userData) {
        return nRestartRegistration(mPtr, user, userData);
    }

    public Status VerifyUser(User user, String mpinId, String activationKey) {
    	return nVerifyUser(mPtr, user, mpinId, activationKey);
    }
    
    public Status ConfirmRegistration(User user) {
    	return nConfirmRegistration(mPtr, user, "");
    }

    public Status ConfirmRegistration(User user, String pushMessageIdentifier) {
    	return nConfirmRegistration(mPtr, user, pushMessageIdentifier);
    }

    public Status FinishRegistration(User user, String pin) {
        return nFinishRegistration(mPtr, user, pin);
    }

    public Status StartAuthentication(User user) {
        return nStartAuthentication(mPtr, user);
    }

    public Status CheckAccessNumber(String accessNumber) {
    	return nCheckAccessNumber(mPtr, accessNumber);
    }
    
    public Status FinishAuthentication(User user, String pin) {
    	return nFinishAuthentication(mPtr, user, pin);
    }
    
    public Status FinishAuthentication(User user, String pin, StringBuilder authResultData) {
    	return nFinishAuthenticationResultData(mPtr, user, pin, authResultData);
    }

    public Status FinishAuthenticationOTP(User user, String pin, OTP otp) {
        return nFinishAuthenticationOTP(mPtr, user, pin, otp);
    }

    public Status FinishAuthenticationAN(User user, String pin, String accessNumber) {
        return nFinishAuthenticationAN(mPtr, user, pin, accessNumber);
    }

    public void DeleteUser(User user) {
        nDeleteUser(mPtr, user);
    }

    public void ListUsers(List<User> users) {
        nListUsers(mPtr, users);
    }

    public String GetVersion() {
    	return nGetVersion(mPtr);
    }

    public boolean CanLogout(User user) {
        return nCanLogout(mPtr, user);
    }

    public boolean Logout(User user) {
        return nLogout(mPtr, user);
    }

    public String GetClientParam(String key) {
        return nGetClientParam(mPtr, key);
    }

    private long mPtr;

    private native long nConstruct();
    private native void nDestruct(long ptr);
    private native Status nInit(long ptr, Map<String, String> config, Context context);
    private native Status nTestBackend(long ptr, String server);
    private native Status nTestBackendRPS(long ptr, String server, String rpsPrefix);
    private native Status nSetBackend(long ptr, String server);
    private native Status nSetBackendRPS(long ptr, String server, String rpsPrefix);
    private native User nMakeNewUser(long ptr, String id, String deviceName);
    private native Status nStartRegistration(long ptr, User user, String userData);
    private native Status nRestartRegistration(long ptr, User user, String userData);
    private native Status nVerifyUser(long ptr, User user, String mpinId, String activationKey);
    private native Status nConfirmRegistration(long ptr, User user, String pushMessageIdentifier);
    private native Status nFinishRegistration(long ptr, User user, String pin);
    private native Status nStartAuthentication(long ptr, User user);
    private native Status nCheckAccessNumber(long ptr, String accessNumber);
    private native Status nFinishAuthentication(long ptr, User user, String pin);
    private native Status nFinishAuthenticationResultData(long ptr, User user, String pin, StringBuilder authResultData);
    private native Status nFinishAuthenticationOTP(long ptr, User user, String pin, OTP otp);
    private native Status nFinishAuthenticationAN(long ptr, User user, String pin, String accessNumber);
    private native void nDeleteUser(long ptr, User user);
    private native void nListUsers(long ptr, List<User> users);
    private native String nGetVersion(long ptr);
    private native boolean nCanLogout(long ptr, User user);
    private native boolean nLogout(long ptr, User user);
    private native String nGetClientParam(long ptr, String key);
}
