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


import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.webkit.URLUtil;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;

import com.certivox.constants.FragmentTags;
import com.certivox.controllers.MPinController;
import com.certivox.models.Config;
import com.certivox.mpinsdk.R;


public class ConfigDetailFragment extends MPinFragment implements OnClickListener {

    private static final String TAG = ConfigDetailFragment.class.getCanonicalName();

    private View     mView;
    private EditText mServiceNameEditText;
    private EditText mServiceUrlEditText;
    private EditText mServiceRTSEditText;
    private CheckBox mServiceOTPCheckBox;
    private CheckBox mServiceANCheckBox;
    private Button   mCheckServiceButton;
    private Button   mSaveServiceButton;

    private Config mConfig;
    private int    mConfigId;
    private String mConfigURL;


    @Override
    public void setData(Object data) {
        mConfigId = ((Integer) data).intValue();
    }


    @Override
    protected String getFragmentTag() {
        return FragmentTags.FRAGMENT_CONFIGURATION_EDIT;
    }


    @Override
    public boolean handleMessage(Message msg) {
        switch (msg.what) {
        case MPinController.MESSAGE_VALID_BACKEND:
            showValidBackendDialog();
            return true;
        case MPinController.MESSAGE_INVALID_BACKEND:
            showInvalidBackednURL();
            return true;
        default:
            break;
        }
        return false;
    }


    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        mConfig = new Config();
        if (mConfigId != -1) {
            initConfig();
        }
    }


    private void initConfig() {
        // TODO: maybe the configuration should be directly sent
        mConfig = getMPinController().getConfiguration(mConfigId);
        mConfigURL = mConfig.getBackendUrl();
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        mView = inflater.inflate(R.layout.fragment_config_details, container, false);
        initViews();
        initScreen();

        return mView;
    }


    @Override
    protected void initViews() {
        mServiceNameEditText = (EditText) mView.findViewById(R.id.service_name_input);
        mServiceUrlEditText = (EditText) mView.findViewById(R.id.service_url_input);
        mServiceRTSEditText = (EditText) mView.findViewById(R.id.service_rts_input);

        mServiceOTPCheckBox = (CheckBox) mView.findViewById(R.id.service_otp);
        mServiceANCheckBox = (CheckBox) mView.findViewById(R.id.service_an);
        mServiceANCheckBox.setChecked(true);

        mCheckServiceButton = (Button) mView.findViewById(R.id.check_service_button);
        mSaveServiceButton = (Button) mView.findViewById(R.id.save_service_button);

        mCheckServiceButton.setOnClickListener(this);
        mSaveServiceButton.setOnClickListener(this);

        // TODO: This should be natural radio group buttons
        mServiceOTPCheckBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    mServiceANCheckBox.setChecked(false);
                } else
                    if (!mServiceANCheckBox.isChecked()) {
                        buttonView.setChecked(true);
                    }
            }
        });

        mServiceANCheckBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {

            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    mServiceOTPCheckBox.setChecked(false);
                } else
                    if (!mServiceOTPCheckBox.isChecked()) {
                        buttonView.setChecked(true);
                    }
            }
        });
    }


    private void initScreen() {
        disableDrawer();
        if (mConfig.getId() != -1) {
            setToolbarTitle(R.string.config_detail_toolbar_title);
            mServiceNameEditText.setText(mConfig.getTitle());
            mServiceUrlEditText.setText(mConfig.getBackendUrl());
            mServiceRTSEditText.setText(mConfig.getRTS());
            mServiceOTPCheckBox.setChecked(mConfig.getRequestOtp());
            mServiceANCheckBox.setChecked(mConfig.getRequestAccessNumber());
        } else {
            setToolbarTitle(R.string.add_service_toolbar_title);
        }
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


    private boolean isEmptyTitle() {
        if (mServiceNameEditText.getText().toString().trim().length() == 0) {
            return true;
        }
        return false;
    }


    private void showInvalidURLDialog() {
        new AlertDialog.Builder(getActivity()).setTitle(getResources().getString(R.string.invalid_url_address_title))
                .setMessage(getResources().getString(R.string.try_again))
                .setPositiveButton(getResources().getString(R.string.button_ok), null).show();
    }


    private void showInvalidBackednURL() {
        new AlertDialog.Builder(getActivity()).setTitle(getResources().getString(R.string.invalid_backend_title))
                .setMessage(R.string.invalid_backend_content)
                .setPositiveButton(getResources().getString(R.string.button_ok), null).show();
    }


    private void showValidBackendDialog() {
        new AlertDialog.Builder(getActivity()).setTitle(getResources().getString(R.string.correct_url_title))
                .setMessage(getResources().getString(R.string.correct_url_content))
                .setPositiveButton(getResources().getString(R.string.button_ok), null).show();
    }


    private void showEmptyTitleDialog() {
        new AlertDialog.Builder(getActivity()).setTitle(getResources().getString(R.string.error_dialog_title))
                .setMessage(getResources().getString(R.string.empty_title_content))
                .setPositiveButton(getResources().getString(R.string.button_ok), null).show();
    }


    private Config getConfig() {
        // Setting service name
        String serviceName = mServiceNameEditText.getText().toString().trim();
        mConfig.setTitle(serviceName);
        // Setting service url
        String backendUrl = mServiceUrlEditText.getText().toString().trim();
        mConfig.setBackendUrl(backendUrl);
        String rts = mServiceRTSEditText.getText().toString().trim();
        // Setting rts
        mConfig.setRTS(rts);
        // Set OTP
        boolean otpChecked = mServiceOTPCheckBox.isChecked();
        mConfig.setRequestOtp(otpChecked);

        // set AccessNumber
        boolean anChecked = mServiceANCheckBox.isChecked();
        mConfig.setRequestAccessNumber(anChecked);

        return mConfig;
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
        case R.id.check_service_button:
            onCheckConfigClicked();
            return;
        case R.id.save_service_button:
            onSaveConfigClicked();
            return;
        default:
            return;
        }

    }


    private void onCheckConfigClicked() {
        String backendUrl = mServiceUrlEditText.getText().toString().trim();
        mServiceUrlEditText.setText(backendUrl);
        mServiceUrlEditText.setSelection(backendUrl.length());
        if (!URLUtil.isValidUrl(backendUrl)) {
            showInvalidURLDialog();
        } else {
            getMPinController().handleMessage(MPinController.MESSAGE_CHECK_BACKEND_URL, backendUrl);
        }
    }


    private void onSaveConfigClicked() {
        String backendUrl = mServiceUrlEditText.getText().toString().trim();
        mServiceUrlEditText.setText(backendUrl);
        mServiceUrlEditText.setSelection(backendUrl.length());
        if (isEmptyTitle()) {
            showEmptyTitleDialog();
        } else
            if (mConfigId != -1 && !mConfigURL.equals(backendUrl)) {
                new AlertDialog.Builder(getActivity()).setTitle("Updating configuration")
                        .setMessage("This action will also delete all identities, associated with this configuration.")
                        .setPositiveButton("OK", new DialogInterface.OnClickListener() {

                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                getMPinController().handleMessage(MPinController.MESSAGE_SAVE_CONFIG, getConfig());
                            }
                        }).setNegativeButton("Cancel", null).show();
            } else {
                getMPinController().handleMessage(MPinController.MESSAGE_SAVE_CONFIG, getConfig());
            }
    }
}
