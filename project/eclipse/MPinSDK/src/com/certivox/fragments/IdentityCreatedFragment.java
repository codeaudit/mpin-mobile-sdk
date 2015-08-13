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
import android.widget.Button;
import android.widget.TextView;

import com.certivox.constants.FragmentTags;
import com.certivox.controllers.MPinController;
import com.certivox.mpinsdk.R;


public class IdentityCreatedFragment extends MPinFragment implements OnClickListener {

    private TextView mInfoTextView;
    private View     mView;
    private TextView mUserEmailTextView;
    private Button   mSignInButton;
    private Button   mBackButton;


    @Override
    protected String getFragmentTag() {
        return FragmentTags.FRAGMENT_IDENTITY_CREATED;
    }


    @Override
    public boolean handleMessage(Message msg) {
        return false;
    }


    @Override
    protected OnClickListener getDrawerBackClickListener() {
        return null;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        mView = inflater.inflate(R.layout.fragment_identity_created, container, false);

        initViews();
        return mView;
    }


    @Override
    protected void initViews() {
        setToolbarTitle(R.string.identity_created_title);
        mInfoTextView = (TextView) mView.findViewById(R.id.info_text);
        mInfoTextView.setText(String.format(getResources().getString(R.string.identity_created),
                getMPinController().getCurrentUser().getId()));
        mUserEmailTextView = (TextView) mView.findViewById(R.id.user_email);
        mUserEmailTextView.setText(getMPinController().getCurrentUser().getId());

        mSignInButton = (Button) mView.findViewById(R.id.sign_in_button);
        mBackButton = (Button) mView.findViewById(R.id.back_button);

        mSignInButton.setOnClickListener(this);
        mBackButton.setOnClickListener(this);

    }


    @Override
    public void setData(Object data) {

    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
        case R.id.sign_in_button:
            getMPinController().handleMessage(MPinController.MESSAGE_ON_SIGN_IN);
            break;
        case R.id.back_button:
            getMPinController().handleMessage(MPinController.MESSAGE_ON_SHOW_IDENTITY_LIST);
            break;
        default:
            break;
        }
    }
}
