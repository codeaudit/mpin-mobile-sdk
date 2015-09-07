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


import java.io.Serializable;


public final class Config implements Serializable {

    private long    mId;
    private String  mTitle;
    private String  mBackendUrl;
    private String  mRTS;
    private boolean mRequestOtp;
    private boolean mRequestAccessNumber;
    private boolean mIsDefault;


    public Config() {
        mId = -1;
        mRequestOtp = false;
        mRequestAccessNumber = false;
    }


    public Config(String title, String backendUrl, boolean requestOtp, boolean requestAccessNumber, boolean isDefault) {
        mId = -1;
        mTitle = title;
        mBackendUrl = backendUrl;
        mRequestOtp = requestOtp;
        mRequestAccessNumber = requestAccessNumber;
        mRTS = "";
        mIsDefault = isDefault;
    }


    public Config(String title, String backendUrl, String rts, boolean requestOtp, boolean requestAccessNumber,
            boolean isDefault) {
        this(title, backendUrl, requestOtp, requestAccessNumber, isDefault);
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


    public boolean isDefault() {
        return mIsDefault;
    }


    public void setIsDefault(boolean isDefault) {
        mIsDefault = isDefault;
    }

}
