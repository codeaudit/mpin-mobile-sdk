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
package com.certivox.controllers;


import java.util.ArrayList;
import java.util.List;

import android.os.Handler;
import android.os.Message;


abstract public class Controller {

    private static final String TAG            = "Controller";
    private final List<Handler> outboxHandlers = new ArrayList<Handler>();
    private Object              lock           = new Object();


    abstract public boolean handleMessage(int what, Object data);


    abstract public boolean handleMessage(int what);


    protected Controller(Handler handler) {
        addOutboxHandler(handler);
    }


    public final void addOutboxHandler(Handler handler) {
        synchronized (lock) {
            outboxHandlers.add(handler);
        }
    }


    public final void removeOutboxHandler(Handler handler) {
        synchronized (lock) {
            outboxHandlers.remove(handler);
        }
    }


    protected final void notifyOutboxHandlers(int what, int arg1, int arg2, Object obj) {
        synchronized (lock) {
            if (!outboxHandlers.isEmpty()) {
                for (Handler handler : outboxHandlers) {
                    Message msg = Message.obtain(handler, what, arg1, arg2, obj);
                    msg.sendToTarget();
                }
            }
        }
    }
}
