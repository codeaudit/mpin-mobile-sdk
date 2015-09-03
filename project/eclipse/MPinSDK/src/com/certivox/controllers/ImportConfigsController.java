package com.certivox.controllers;


import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.certivox.activities.ImportConfigsActivity;
import com.certivox.adapters.ConfigurationListAdapter;
import com.certivox.constants.IntentConstants;
import com.certivox.dal.ConfigsDao;
import com.certivox.models.Config;
import com.certivox.mpinsdk.R;

import android.content.Intent;
import android.graphics.Color;
import android.util.SparseBooleanArray;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;
import android.widget.TextView;


public class ImportConfigsController {

    private ConfigurationListAdapter mConfigsAdapter;
    private ConfigsDao               mConfigsDao;
    private Map<Config, Config>      mDuplicates;
    private SparseBooleanArray       mDuplicatesSelection;
    private int                      mActiveConfigPosition;
    private ImportConfigsActivity    mActivity;


    public ImportConfigsController(ImportConfigsActivity activity) {
        mActivity = activity;
    }


    public void handleOnCreate() {
        initAdapter();
        initConfigs();
        mActivity.setConfigsAdapter(mConfigsAdapter);
    }


    public void handleOnOverridesConfirmed() {
        saveSelection();
        mActivity.finish();
    }


    public void handleActiveConfigOverrideConfirmed() {
        mActivity.showOverridesWarningDialog();
    }


    public void handleOnActionSelect() {
        checkForOverridesAndSave();
    }


    public void handleOnItemSelected(int position) {
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


    private void initAdapter() {
        Intent startIntent = mActivity.getIntent();

        if (startIntent.getAction().equals(Intent.ACTION_PICK)) {
            if (startIntent.hasExtra(IntentConstants.EXTRA_CONFIGS_LIST)) {
                mConfigsAdapter = new ConfigurationListAdapter(mActivity.getBaseContext(),
                        (List<Config>) startIntent.getSerializableExtra(IntentConstants.EXTRA_CONFIGS_LIST),
                        ConfigurationListAdapter.SELECT_ALL);
                mConfigsAdapter.setAdditionalContentAdapter(new DuplicatesAdapter());
            } else {
                mActivity.finish();
            }
        } else {
            mActivity.finish();
        }
    }


    private void checkForOverridesAndSave() {
        if (mDuplicatesSelection.get(mActiveConfigPosition)) {
            mActivity.showActiveConfigOverrideWarningDialog();
        } else
            if (mDuplicatesSelection.indexOfValue(true) >= 0) {
                mActivity.showOverridesWarningDialog();
            } else {
                saveSelection();
                mActivity.finish();
            }
    }


    private void saveSelection() {
        List<Config> selectedList = mConfigsAdapter.getSelected();

        for (Config selected : selectedList) {
            if (mDuplicates.containsKey(selected)) {
                // Override existing configurations manually, because the newly imported configuration may not have
                // proper id
                Config existing = mDuplicates.get(selected);
                overrideConfig(existing, selected);

                mConfigsDao.saveOrUpdate(existing);
            } else {
                mConfigsDao.saveOrUpdate(selected);
            }

        }

    }


    private void overrideConfig(Config existingConfig, Config newConfig) {
        // Do not change the id of the existingConfig even if the new configuration has other valid id in order to
        // actually override
        existingConfig.setRequestAccessNumber(newConfig.getRequestAccessNumber());
        existingConfig.setRequestOtp(newConfig.getRequestOtp());
        if (newConfig.getBackendUrl() != null) {
            existingConfig.setBackendUrl(newConfig.getBackendUrl());
        }
        if (newConfig.getRTS() != null) {
            existingConfig.setRTS(newConfig.getRTS());
        }
        if (newConfig.getTitle() != null) {
            existingConfig.setTitle(newConfig.getTitle());
        }
    }


    private void initConfigs() {
        mConfigsDao = new ConfigsDao(mActivity.getBaseContext());
        mDuplicatesSelection = new SparseBooleanArray();
        mDuplicates = new HashMap<Config, Config>();
        mActiveConfigPosition = -1;

        Config activeConfig = mConfigsDao.getActiveConfiguration();
        String activeConfigTitle = activeConfig == null ? null : activeConfig.getTitle();

        List<Config> existingConfigs = mConfigsDao.getListConfigs();
        for (int i = 0; i < mConfigsAdapter.getCount(); i++) {
            Config newConfig = ((Config) mConfigsAdapter.getItem(i));
            String newConfigTitle = newConfig.getTitle();

            for (int j = 0; j < existingConfigs.size(); j++) {
                Config existingConfig = existingConfigs.get(j);
                String existingConfigTitle = existingConfig.getTitle();

                if (existingConfigTitle.equals(newConfigTitle)) {
                    mDuplicatesSelection.put(i, true);

                    if (activeConfig != null && activeConfigTitle.equals(newConfigTitle)) {
                        mActiveConfigPosition = i;
                        mDuplicates.put(newConfig, activeConfig);
                    } else {
                        mDuplicates.put(newConfig, existingConfig);
                    }
                }
            }
        }
    }

    private class DuplicatesAdapter implements ConfigurationListAdapter.AdditionalContentAdapter {

        @Override
        public void fillView(Config item, int position, RelativeLayout parentView) {

            if (mDuplicates.containsKey(item)) {
                TextView duplicateTextView = new TextView(mActivity);
                duplicateTextView.setText(R.string.import_configs_duplicated_config_info);
                duplicateTextView.setTextColor(Color.RED);
                RelativeLayout.LayoutParams lp = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
                lp.addRule(RelativeLayout.CENTER_VERTICAL);
                parentView.addView(duplicateTextView, lp);
            } else {
                parentView.removeAllViews();
            }
        }

    }

}
