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


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import com.certivox.adapters.ConfigurationListAdapter;
import com.certivox.constants.IntentConstants;
import com.certivox.dal.ConfigsDao;
import com.certivox.models.Config;
import com.certivox.mpinsdk.R;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.support.v7.widget.Toolbar;
import android.util.SparseBooleanArray;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;


public class SelectConfigsActivity extends ActionBarActivity {

    private ListView mListView;
    private Toolbar  mToolbar;

    private ConfigurationListAdapter mConfigsAdapter;
    private ConfigsDao               mConfigsDao;
    private SparseBooleanArray       mDuplicatesSelection;
    private int                      mActiveConfigPosition;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_select_configs);
        initAdapter();
        initViews();
        initConfigs();

    }


    @Override
    protected void onResume() {
        super.onResume();
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.select_configs, menu);
        return true;
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.select_configs_action_select) {
            warnForOverridesAndReturnResult();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }


    public void initConfigs() {
        mConfigsDao = new ConfigsDao(this);
        mDuplicatesSelection = new SparseBooleanArray();

        String activeConfigTitle = mConfigsDao.getActiveConfiguration().getTitle();
        List<Config> existingConfigs = mConfigsDao.getListConfigs();
        for (int i = 0; i < mConfigsAdapter.getCount(); i++) {
            String newConfigTitle = ((Config) mConfigsAdapter.getItem(i)).getTitle();
            for (int j = 0; j < existingConfigs.size(); j++) {
                String existingConfigTitle = existingConfigs.get(j).getTitle();

                if (existingConfigTitle.equals(newConfigTitle)) {
                    mDuplicatesSelection.put(i, true);

                    if (activeConfigTitle.equals(newConfigTitle)) {
                        mActiveConfigPosition = i;
                    }
                }
            }
        }
    }


    private void initAdapter() {
        Intent startIntent = getIntent();

        if (startIntent.getAction().equals(Intent.ACTION_PICK)) {
            if (startIntent.hasExtra(IntentConstants.EXTRA_CONFIGS_LIST)) {
                mConfigsAdapter = new ConfigurationListAdapter(getBaseContext(),
                        (List<Config>) startIntent.getSerializableExtra(IntentConstants.EXTRA_CONFIGS_LIST),
                        ConfigurationListAdapter.SELECT_ALL);
            } else {
                Toast.makeText(this, getString(R.string.no_configurations_loaded_message), Toast.LENGTH_LONG).show();
                finish();
            }
        } else {
            finish();
        }
    }


    private void initViews() {
        mToolbar = (Toolbar) findViewById(R.id.select_configs_toolbar);
        mToolbar.setTitle(R.string.import_configurations_title);
        setSupportActionBar(mToolbar);

        mListView = (ListView) findViewById(R.id.select_configs_list_view);
        // mConfigsAdapter = new ConfigurationListAdapter(this, getMockConfigs(), ConfigurationListAdapter.SELECT_ALL);
        mConfigsAdapter.setAdditionalContentAdapter(new DuplicatesAdapter());
        mListView.setAdapter(mConfigsAdapter);
        mListView.setOnItemClickListener(new SelectionListener());
    }


    private void returnResult() {
        Intent resultIntent = new Intent();
        resultIntent.putExtra(IntentConstants.EXTRA_SELECTED_CONFIGS_LIST,
                new ArrayList<Config>(mConfigsAdapter.getSelected()));
        setResult(RESULT_OK, resultIntent);
        finish();
    }


    private void warnForOverridesAndReturnResult() {
        if (mDuplicatesSelection.get(mActiveConfigPosition)) {
            showActiveConfigOverrideWarningDialog();
        } else
            if (mDuplicatesSelection.indexOfValue(true) >= 0) {
                showOverridesWarningDialog();
            } else {
                returnResult();
            }
    }


    private void showOverridesWarningDialog() {
        new AlertDialog.Builder(this).setTitle("There are duplicates")
                .setMessage("This action will also delete all identities, associated with this configuration.")
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {

                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        returnResult();
                    }
                }).setNegativeButton("Cancel", null).show();
    }


    private void showActiveConfigOverrideWarningDialog() {
        new AlertDialog.Builder(this).setTitle("Active config")
                .setMessage("This action will also delete all identities, associated with this configuration.")
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {

                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        showOverridesWarningDialog();
                    }
                }).setNegativeButton("Cancel", null).show();
    }


    private List<Config> getMockConfigs() {
        List<Config> configs = new ArrayList<Config>();
        Config config = new Config("Awesome config", "https://awesome.config.com", true, false, false);
        config.setId(123);
        configs.add(config);
        config = new Config("Best config", "https://best.config.com", true, false, false);
        config.setId(124);
        configs.add(config);
        config = new Config("Radical config", "https://rad.config.com", true, true, false);
        config.setId(125);
        configs.add(config);
        config = new Config("M-Pin Connect", "https://fake.mpin.com", true, true, false);
        config.setId(125);
        configs.add(config);
        return configs;
    }

    private class DuplicatesAdapter implements ConfigurationListAdapter.AdditionalContentAdapter {

        @Override
        public void fillView(Config item, int position, RelativeLayout parentView) {

            if (mDuplicatesSelection.indexOfKey(position) >= 0) {
                TextView duplicateTextView = new TextView(SelectConfigsActivity.this);
                duplicateTextView.setText("Duplicated");
                duplicateTextView.setTextColor(Color.RED);
                parentView.addView(duplicateTextView);
            } else {
                parentView.removeAllViews();
            }
        }

    }

    private class SelectionListener implements AdapterView.OnItemClickListener {

        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            // Switch the duplicated selection
            if (mDuplicatesSelection.indexOfKey(position) >= 0) {
                mDuplicatesSelection.put(position, !mDuplicatesSelection.get(position));
            }

            if (mConfigsAdapter.isSelected(position)) {
                mConfigsAdapter.deselect(position);
            } else {
                mConfigsAdapter.select(position);
            }
        }

    }
}
