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
package com.certivox.storage;


import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.content.Context;


public class Storage implements IStorage {

    public static final String MPIN_STORAGE = "MpinStorage";
    public static final String USER_STORAGE = "UserStorage";
    public static int          chunkSize    = 255;

    private final Context context;
    private final String  path;
    private String        errorMessage = null;


    public Storage(Context context, boolean isMpinType) {
        super();
        this.context = context.getApplicationContext();
        path = isMpinType ? MPIN_STORAGE : USER_STORAGE;
    }


    @Override
    public boolean SetData(String data) {
        errorMessage = null;
        FileOutputStream fos = null;
        try {
            fos = context.openFileOutput(path, Context.MODE_PRIVATE);
            fos.write(data.getBytes());
        } catch (FileNotFoundException e) {
            errorMessage = e.getLocalizedMessage();
        } catch (IOException e) {
            errorMessage = e.getLocalizedMessage();
        } finally {
            if (fos == null)
                return false;
            try {
                fos.close();
            } catch (IOException e) {
                errorMessage = e.getLocalizedMessage();
            }
        }

        return (errorMessage == null);
    }


    @Override
    public String GetData() {
        String data = "";
        errorMessage = null;
        FileInputStream fis = null;
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        try {
            fis = context.openFileInput(path);
            byte[] buffer = new byte[chunkSize];
            int nbread;
            while ((nbread = fis.read(buffer, 0, chunkSize)) > 0) {
                bos.write(buffer, 0, nbread);
            }
            data = new String(bos.toByteArray());
        } catch (FileNotFoundException e) {
            errorMessage = e.getLocalizedMessage();
        } catch (IOException e) {
            errorMessage = e.getLocalizedMessage();
        } finally {
            if (fis != null) {
                try {
                    fis.close();
                    bos.close();
                } catch (IOException e) {
                    errorMessage = e.getLocalizedMessage();
                }
            }
        }
        return data;
    }


    @Override
    public String GetErrorMessage() {
        return errorMessage;
    }

}
