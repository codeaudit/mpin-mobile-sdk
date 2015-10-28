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


public class Mpin implements Closeable {

    public Mpin(Context context, Map<String, String> config) {
        mPtr = nConstruct(context, config);
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


    public Status FinishRegistration(User user) {
        return nFinishRegistration(mPtr, user);
    }


    public Status Authenticate(User user) {
        return nAuthenticate(mPtr, user);
    }


    public Status AuthenticateOTP(User user, OTP otp) {
        return nAuthenticateOtp(mPtr, user, otp);
    }


    public Status Authenticate(User user, StringBuilder authResultData) {
        return nAuthenticateResultData(mPtr, user, authResultData);
    }


    public Status AuthenticateAN(User user, String accessNumber) {
        return nAuthenticateAccessNumber(mPtr, user, accessNumber);
    }


    public void DeleteUser(User user) {
        nDeleteUser(mPtr, user);
    }


    public void ListUsers(List<User> users) {
        nListUsers(mPtr, users);
    }


    public boolean CanLogout(User user) {
        return nCanLogout(mPtr, user);
    }


    public boolean Logout(User user) {
        return nLogout(mPtr, user);
    }


    public Status TestBackend(String backend) {
        return nTestBackend(mPtr, backend);
    }


    public Status TestBackend(String backend, String rpsPrefix) {
        return nTestBackendRPS(mPtr, backend, rpsPrefix);
    }


    public Status SetBackend(String backend) {
        return nSetBackend(mPtr, backend, "rps");
    }


    public Status SetBackend(String backend, String rpsPrefix) {
        return nSetBackend(mPtr, backend, rpsPrefix);
    }


    public String GetClientParam(String key) {
        return nGetClientParam(mPtr, key);
    }

    private long mPtr;


    private native long nConstruct(Context context, Map<String, String> config);


    private native void nDestruct(long ptr);


    private native User nMakeNewUser(long ptr, String id, String deviceName);


    private native Status nStartRegistration(long ptr, User user, String userData);


    private native Status nRestartRegistration(long ptr, User user, String userData);


    private native Status nFinishRegistration(long ptr, User user);


    private native Status nAuthenticate(long ptr, User user);


    private native Status nAuthenticateOtp(long ptr, User user, OTP otp);


    private native Status nAuthenticateResultData(long ptr, User user, StringBuilder authResultData);


    private native Status nAuthenticateAccessNumber(long ptr, User user, String accessNumber);


    private native void nDeleteUser(long ptr, User user);


    private native void nListUsers(long ptr, List<User> users);


    private native boolean nCanLogout(long ptr, User user);


    private native boolean nLogout(long ptr, User user);


    private native Status nTestBackend(long ptr, String backend);


    private native Status nTestBackendRPS(long ptr, String backend, String rpsPrefix);


    private native Status nSetBackend(long ptr, String backend, String rpsPrefix);


    private native String nGetClientParam(long ptr, String key);
}