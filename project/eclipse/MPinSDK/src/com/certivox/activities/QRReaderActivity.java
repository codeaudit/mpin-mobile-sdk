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

package com.certivox.activities;


import net.sourceforge.zbar.Config;
import net.sourceforge.zbar.Image;
import net.sourceforge.zbar.ImageScanner;
import net.sourceforge.zbar.Symbol;
import net.sourceforge.zbar.SymbolSet;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.widget.FrameLayout;
import android.widget.TextView;

import com.certivox.constants.IntentConstants;
import com.certivox.mpinsdk.R;
import com.certivox.view.CameraPreview;


public class QRReaderActivity extends Activity {

    static {
        System.loadLibrary("iconv");
    }

    private CameraPreview mPreview;
    private Handler       mAutoFocusHandler;
    private ImageScanner  mScanner;
    private boolean       mIsPreviewing;
    private Runnable      mDoAutoFocusRunnable;
    private FrameLayout   mPreviewFrameLayout;
    private Camera        mCamera;
    PreviewCallback       mPreviewCallBack;
    // Mimic continuous auto-focusing
    AutoFocusCallback     mAutoFocusCallBack;


    /** A safe way to get an instance of the Camera object. */
    private static Camera getCameraInstance() {
        Camera camera = null;
        try {
            camera = Camera.open();
        } catch (Exception e) {
        }

        return camera;
    }


    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_qr_reader);

    }


    @Override
    protected void onStart() {
        super.onResume();
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        init();

        mPreviewFrameLayout = (FrameLayout) findViewById(R.id.cameraPreview);
        mPreviewFrameLayout.addView(mPreview);
    }


    public void onPause() {
        super.onPause();
        releaseCamera();
    }


    private void releaseCamera() {
        if (mCamera != null) {
            mIsPreviewing = false;
            mPreviewFrameLayout.removeView(mPreview);
            mPreviewFrameLayout = null;
            mCamera.setPreviewCallback(null);
            mCamera.release();
            mCamera = null;
        }
    }


    private void init() {
        mIsPreviewing = true;
        initDoAutoFocusRunnable();
        initPreviewCallBack();
        initAutoFocusCallBack();

        mAutoFocusHandler = new Handler();
        mCamera = getCameraInstance();

        /* Instance barcode scanner */
        mScanner = new ImageScanner();
        mScanner.setConfig(0, Config.ENABLE, 0); //Disable all the Symbols
        mScanner.setConfig(Symbol.QRCODE, Config.ENABLE, 1);

        mPreview = new CameraPreview(this, mCamera, mPreviewCallBack, mAutoFocusCallBack);
    }


    private void initDoAutoFocusRunnable() {
        mDoAutoFocusRunnable = new Runnable() {

            public void run() {
                if (mIsPreviewing)
                    mCamera.autoFocus(mAutoFocusCallBack);
            }
        };
    }


    private void initPreviewCallBack() {
        mPreviewCallBack = new PreviewCallback() {

            public void onPreviewFrame(byte[] data, Camera camera) {
                Camera.Parameters parameters = camera.getParameters();
                Size size = parameters.getPreviewSize();

                Image barcode = new Image(size.width, size.height, "Y800");
                barcode.setData(data);

                int result = mScanner.scanImage(barcode);

                if (result != 0) {
                    mIsPreviewing = false;
                    mCamera.setPreviewCallback(null);
                    mCamera.stopPreview();

                    SymbolSet syms = mScanner.getResults();
                    String url = null;
                    for (Symbol sym : syms) {
                        url = sym.getData();
                    }

                    Intent resultIntent = new Intent();
                    resultIntent.putExtra(IntentConstants.QR_CODE_URL, url);
                    setResult(Activity.RESULT_OK, resultIntent);
                    finish();
                }
            }
        };
    }


    private void initAutoFocusCallBack() {
        mAutoFocusCallBack = new AutoFocusCallback() {

            public void onAutoFocus(boolean success, Camera camera) {
                mAutoFocusHandler.postDelayed(mDoAutoFocusRunnable, 1000);
            }
        };
    }
}
