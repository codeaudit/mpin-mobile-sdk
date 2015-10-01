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


import java.util.List;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.Toast;

import com.certivox.activities.QRReaderActivity;
import com.certivox.adapters.ConfigurationListAdapter;
import com.certivox.constants.FragmentTags;
import com.certivox.constants.IntentConstants;
import com.certivox.controllers.MPinController;
import com.certivox.models.Config;
import com.certivox.mpinsdk.R;


public class ConfigsListFragment extends MPinFragment implements OnClickListener, AdapterView.OnItemClickListener {

    private String                   TAG = ConfigsListFragment.class.getCanonicalName();

    private View                     mView;
    private ListView                 mListView;
    private ConfigurationListAdapter mAdapter;
    private ImageButton              mAddServiceButton;
    private long                     mSelectedConfigId;
    private Config                   mSelectedConfig;


    @Override
    public void setData(Object data) {
    }


    @Override
    protected String getFragmentTag() {
        return FragmentTags.FRAGMENT_CONFIGURATIONS_LIST;
    }


    @Override
    protected OnClickListener getDrawerBackClickListener() {
        OnClickListener drawerBackClickListener = new OnClickListener() {

            @Override
            public void onClick(View v) {
                if (mSelectedConfigId == -1) {
                    showNoSelectedConfigurationDialog();
                } else
                    if (getMPinController().getActiveConfiguration() == null) {
                        showNoActivatedConfigurationDialog();
                    } else {
                        getMPinController().handleMessage(MPinController.MESSAGE_ON_DRAWER_BACK);
                    }
            }
        };
        return drawerBackClickListener;
    }


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        setToolbarTitle(R.string.select_service_toolbar_title);

        mView = inflater.inflate(R.layout.fragment_configs_list, container, false);
        mSelectedConfig = getMPinController().getActiveConfiguration();
        if (mSelectedConfig != null) {
            mSelectedConfigId = mSelectedConfig.getId();
        } else {
            mSelectedConfigId = -1;
        }
        disableDrawer();
        initViews();

        return mView;
    }


    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
    }


    @Override
    public void onResume() {
        super.onResume();
        initAdapter();
    }


    @Override
    public boolean handleMessage(Message msg) {
        switch (msg.what) {
        case MPinController.MESSAGE_CONFIGURATION_DELETED:
            mSelectedConfigId = -1;
            mAdapter.updateConfigsList(getMPinController().getConfigurationsList());
            return true;
        case MPinController.MESSAGE_CONFIGURATION_CHANGED:
            Toast.makeText(getActivity(), "Configuration activated!", Toast.LENGTH_SHORT).show();
            return true;
        case MPinController.MESSAGE_CONFIGURATION_CHANGE_ERROR:
            Toast.makeText(getActivity(), "Failed to activate configuration", Toast.LENGTH_SHORT).show();
            return true;
        case MPinController.MESSAGE_NO_ACTIVE_CONFIGURATION:
            showNoSelectedConfigurationDialog();
        default:
            return false;
        }
    }


    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        inflater.inflate(R.menu.configs_list, menu);
        super.onCreateOptionsMenu(menu, inflater);
    }


    @Override
    public void onPrepareOptionsMenu(Menu menu) {
        super.onPrepareOptionsMenu(menu);
        if (mSelectedConfig != null && mSelectedConfig.isDefault()) {
            menu.findItem(R.id.configs_list_edit).setEnabled(false);
            menu.findItem(R.id.configs_list_delete).setEnabled(false);
        } else {
            menu.findItem(R.id.configs_list_edit).setEnabled(true);
            menu.findItem(R.id.configs_list_delete).setEnabled(true);
        }
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.select_config:
            onSelectConfig();
            return true;
        case R.id.configs_list_new:
            onNewConfig();
            return true;
        case R.id.configs_list_edit:
            onEditConfig();
            return true;
        case R.id.configs_list_delete:
            onDeleteConfig();
            return true;
        case R.id.configs_scan_qr:
            startQRCodeScanning();
            return true;
        default:
            return false;
        }
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
        case R.id.add_service_button:
            getMPinController().handleMessage(MPinController.MESSAGE_ON_NEW_CONFIGURATION);
            break;
        default:
        }
    }


    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        mSelectedConfigId = view.getId();
        mAdapter.setSelected(position);
        mSelectedConfig = getMPinController().getConfiguration((int) mSelectedConfigId);
        getActivity().invalidateOptionsMenu();
    }


    @Override
    protected void initViews() {
        mListView = (ListView) mView.findViewById(android.R.id.list);
        mAddServiceButton = (ImageButton) mView.findViewById(R.id.add_service_button);
        mAddServiceButton.setOnClickListener(this);
    }


    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == IntentConstants.QR_CODE_RESULT) {
            switch (resultCode) {
            case Activity.RESULT_OK:
                String url = data.getStringExtra(IntentConstants.QR_CODE_URL);
                getMPinController().handleQRCodeUrl(url);
                break;
            case Activity.RESULT_CANCELED:
                break;
            default:
                break;
            }
        }
    }


    private void initAdapter() {
        List<Config> listConfigurations = getMPinController().getConfigurationsList();
        mSelectedConfigId = getMPinController().getActiveConfigurationId();
        int selectedPos = -1;
        for (int i = 0; i < listConfigurations.size(); i++) {
            Config config = listConfigurations.get(i);
            if (config.getId() == mSelectedConfigId) {
                selectedPos = i;
            }
        }
        if (selectedPos == -1) {
            mAdapter = new ConfigurationListAdapter(getActivity().getApplicationContext(), listConfigurations);
        } else {
            mAdapter = new ConfigurationListAdapter(getActivity().getApplicationContext(), listConfigurations,
                    selectedPos);
        }
        mListView.setAdapter(mAdapter);
        mListView.setOnItemClickListener(this);
    }


    private void onSelectConfig() {
        if (mSelectedConfigId == -1) {
            showNoSelectedConfigurationDialog();
        } else {
            getMPinController().handleMessage(MPinController.MESSAGE_ON_SELECT_CONFIGURATION, mSelectedConfigId);
        }
    }


    private void onNewConfig() {
        getMPinController().handleMessage(MPinController.MESSAGE_ON_NEW_CONFIGURATION);
    }


    private void onEditConfig() {
        if (mSelectedConfigId == -1) {
            showNoSelectedConfigurationDialog();
        } else {
            getMPinController().handleMessage(MPinController.MESSAGE_ON_EDIT_CONFIGURATION, mSelectedConfigId);
        }
    }


    private void onDeleteConfig() {
        if (mSelectedConfig == null || mSelectedConfig.getId() == -1) {
            showNoSelectedConfigurationDialog();
        } else
            new AlertDialog.Builder(getActivity()).setTitle("Delete configuration")
                    .setMessage("This action will also delete all identities, associated with this configuration.")
                    .setPositiveButton("OK", new DialogInterface.OnClickListener() {

                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            getMPinController().handleMessage(MPinController.MESSAGE_DELETE_CONFIGURATION,
                                    mSelectedConfigId);
                        }
                    }).setNegativeButton("Cancel", null).show();
    }


    private void showNoSelectedConfigurationDialog() {
        new AlertDialog.Builder(getActivity()).setTitle("No selected configuration")
                .setMessage("Please, choose a configuration").setPositiveButton("OK", null).show();
    }


    private void showNoActivatedConfigurationDialog() {
        new AlertDialog.Builder(getActivity()).setTitle("No activated configuration")
                .setMessage("Please, activate a configuration").setPositiveButton("OK", null).show();
    }


    private void startQRCodeScanning() {
        Intent startQRCodeActivityIntent = new Intent(getActivity(), QRReaderActivity.class);
        startActivityForResult(startQRCodeActivityIntent, IntentConstants.QR_CODE_RESULT);
    }
}