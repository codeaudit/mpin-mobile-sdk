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


import java.io.Closeable;


public class User implements Closeable {

    private boolean isUserSelected;

    private long mPtr;


    private native void nDestruct(long ptr);


    private native String nGetId(long ptr);


    private native int nGetState(long ptr);

    public enum State {
        INVALID, STARTED_REGISTRATION, ACTIVATED, REGISTERED, BLOCKED
    };


    private User(long ptr) {
        mPtr = ptr;
    }


    public String getId() {
        return nGetId(mPtr);
    }


    public State getState() {
        switch (nGetState(mPtr)) {
        case 1:
            return State.STARTED_REGISTRATION;
        case 2:
            return State.ACTIVATED;
        case 3:
            return State.REGISTERED;
        case 4:
            return State.BLOCKED;
        default:
            return State.INVALID;
        }
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


    @Override
    public String toString() {
        return getId();
    }


    public boolean isUserSelected() {
        return isUserSelected;
    }


    public void setUserSelected(boolean isUserSelected) {
        this.isUserSelected = isUserSelected;
    }

}
