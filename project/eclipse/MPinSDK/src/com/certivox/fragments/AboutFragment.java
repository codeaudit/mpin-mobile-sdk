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


import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.os.Message;
import android.text.method.LinkMovementMethod;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.TextView;

import com.certivox.constants.FragmentTags;
import com.certivox.controllers.MPinController;
import com.example.mpinsdk.R;


public class AboutFragment extends MPinFragment {

    private View     mView;
    private TextView mLinkTextView;
    private TextView mVersionTextView;
    private TextView mBuildTextView;


    @Override
    public void setData(Object data) {
    };


    @Override
    protected String getFragmentTag() {
        return FragmentTags.FRAGMENT_ABOUT;
    }


    @Override
    public boolean handleMessage(Message msg) {
        return false;
    }


    @Override
    protected OnClickListener getDrawerBackClickListener() {
        OnClickListener drawerBackClickListener = new OnClickListener() {

            @Override
            public void onClick(View v) {
                getMPinController().handleMessage(MPinController.MESSAGE_ON_DRAWER_BACK);
            }
        };
        return drawerBackClickListener;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        disableDrawer();
        setTooblarTitle(R.string.about_title);
        mView = inflater.inflate(R.layout.fragment_about, container, false);
        initViews();
        setVersion();
        return mView;
    }


    @Override
    protected void initViews() {
        mLinkTextView = (TextView) mView.findViewById(R.id.terms_and_conditions_link);
        mLinkTextView.setMovementMethod(LinkMovementMethod.getInstance());
        mVersionTextView = (TextView) mView.findViewById(R.id.about_version);
        mBuildTextView = (TextView) mView.findViewById(R.id.about_build);
    }


    private void setVersion() {
        PackageInfo pInfo;
        try {
            pInfo = getActivity().getPackageManager().getPackageInfo(getActivity().getPackageName(), 0);

            String versionName = pInfo.versionName;
            int versionCode = pInfo.versionCode;

            mVersionTextView.setText(versionName);
            mBuildTextView.setText(versionCode + "");

        } catch (NameNotFoundException e) {
            e.printStackTrace();
        }

    }
}
