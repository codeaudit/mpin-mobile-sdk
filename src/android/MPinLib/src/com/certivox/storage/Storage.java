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

    private final Context      context;
    private final String       path;
    private String             errorMessage = null;


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
