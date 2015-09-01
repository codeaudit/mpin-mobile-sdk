package com.certivox.adapters;


import java.util.Comparator;
import java.util.List;

import com.certivox.models.Config;

import android.content.Context;
import android.view.View;


public class DuplicatesConfigListAdapter extends ConfigurationListAdapter {

    private List<Integer> mDuplicatedPositions;


    public DuplicatesConfigListAdapter(Context context, List<Config> newConfigurations,
            List<Config> existingConfigurations, Comparator<Config> compareRule, Integer... selectedConfigurations) {
        super(context, newConfigurations, selectedConfigurations);

        for (int i = 0; i < newConfigurations.size(); i++) {
            Config newConfig = newConfigurations.get(i);
            for (int j = 0; j < existingConfigurations.size(); j++) {
                if (compareRule.compare(newConfig, existingConfigurations.get(j)) == 0) {
                    mDuplicatedPositions.add(i);
                }
            }
        }
    }

}
