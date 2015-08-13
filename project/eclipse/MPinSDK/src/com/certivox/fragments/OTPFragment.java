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
package com.certivox.fragments;


import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.certivox.constants.FragmentTags;
import com.certivox.controllers.MPinController;
import com.certivox.models.OTP;
import com.certivox.mpinsdk.R;


public class OTPFragment extends MPinFragment {

    private View        mView;
    private TextView    mUserEmailTextView;
    private TextView    mOTPTextView;
    private ProgressBar mOTPProgressBar;
    private TextView    mTimeLeftTextView;
    private OTP         mOTP;
    private boolean     mIsFragmentDestroyed;


    @Override
    public void setData(Object otp) {
        mOTP = (OTP) otp;
    }


    @Override
    public void onDestroy() {
        super.onDestroy();
        mIsFragmentDestroyed = true;
    }


    @Override
    protected String getFragmentTag() {
        return FragmentTags.FRAGMENT_OTP;
    }


    @Override
    protected OnClickListener getDrawerBackClickListener() {
        return null;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        mView = inflater.inflate(R.layout.fragment_otp, container, false);
        mIsFragmentDestroyed = false;
        initViews();

        return mView;
    }


    private void initProgressBar() {
        final long start = System.currentTimeMillis();
        getActivity().runOnUiThread(new Runnable() {

            @Override
            public void run() {
                if (!mIsFragmentDestroyed) {
                    int remaining = mOTP.ttlSeconds - (int) ((System.currentTimeMillis() - start) / 1000L);
                    if (remaining > 0) {
                        mTimeLeftTextView.setText(remaining + " sec");
                        double prog = 1 - (System.currentTimeMillis() - start) / (mOTP.ttlSeconds * 1000.0);
                        mOTPProgressBar.setProgress((int) (mOTPProgressBar.getMax() * prog));
                        mOTPProgressBar.postDelayed(this, 100);
                    } else {
                        getMPinController().handleMessage(MPinController.MESSAGE_OTP_EXPIRED);
                    }
                }
            }
        });
    }


    @Override
    public boolean handleMessage(Message msg) {
        // TODO Auto-generated method stub
        return false;
    }


    @Override
    protected void initViews() {
        setTooblarTitle(R.string.otp_title);
        mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
        if (getMPinController().getCurrentUser() != null) {
            mUserEmailTextView.setText(getMPinController().getCurrentUser().getId());
        }

        mOTPTextView = (TextView) mView.findViewById(R.id.otp);
        String otp = "";
        for (int i = 0; i < mOTP.otp.length(); i++) {
            otp += mOTP.otp.charAt(i) + " ";
        }
        mOTPTextView.setText(otp);
        mOTPProgressBar = (ProgressBar) mView.findViewById(R.id.otp_progress);
        mTimeLeftTextView = (TextView) mView.findViewById(R.id.otp_time_left);

        initProgressBar();

    }
}
