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
package com.certivox.adapters;


import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import com.certivox.models.Config;
import com.certivox.mpinsdk.R;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.RadioButton;
import android.widget.RelativeLayout;
import android.widget.TextView;


public class ConfigurationListAdapter extends BaseAdapter {

    public interface AdditionalContentAdapter {

        void fillView(Config item, int position, RelativeLayout parentView);
    }

    private Context                  mContext;
    private List<Config>             mConfigsList;
    private Set<Integer>             mSelectedPositions;
    private AdditionalContentAdapter mAdditionalContentProvider;


    public ConfigurationListAdapter(Context context, List<Config> configurations, int... selectedConfigurations) {
        mContext = context;
        mConfigsList = configurations;
        mSelectedPositions = new HashSet<Integer>();
        setSelectedPositions(selectedConfigurations);
    }


    public void updateConfigsList(List<Config> configList) {
        mConfigsList.clear();
        mConfigsList.addAll(configList);
        notifyDataSetChanged();
    }


    public void setAdditionalContentAdapter(AdditionalContentAdapter adapter) {
        mAdditionalContentProvider = adapter;
    }


    public void setSelected(int... selectedConfigurations) {
        setSelectedPositions(selectedConfigurations);
        notifyDataSetChanged();
    }


    public boolean isSelected(int position) {
        return mSelectedPositions.contains(position);
    }


    public void select(int position) {
        if (!mSelectedPositions.contains(position)) {
            mSelectedPositions.add(position);
            notifyDataSetChanged();
        }
    }


    public void selectAll() {
        int[] allPositions = new int[mConfigsList.size()];
        for (int i = 0; i < mConfigsList.size(); i++) {
            allPositions[i] = i;
        }
        setSelectedPositions(allPositions);
    }


    public void deselect(int position) {
        if (mSelectedPositions.contains(position)) {
            mSelectedPositions.remove(Integer.valueOf(position));
            notifyDataSetChanged();
        }
    }


    public void deselectAll() {
        setSelectedPositions();
    }


    public List<Config> getSelected() {
        List<Config> selectedConfigs = new ArrayList<Config>();
        for (int pos : mSelectedPositions) {
            selectedConfigs.add(mConfigsList.get(pos));
        }
        return selectedConfigs;
    }


    @Override
    public void notifyDataSetChanged() {
        super.notifyDataSetChanged();
    }


    @Override
    public int getCount() {
        return mConfigsList.size();
    }


    @Override
    public Object getItem(int position) {
        return mConfigsList.get(position);
    }


    @Override
    public long getItemId(int position) {
        return mConfigsList.get(position).getId();
    }


    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder holder;
        if (convertView == null) {
            convertView = LayoutInflater.from(mContext).inflate(R.layout.item_config, parent, false);
            holder = new ViewHolder();
            holder.title = (TextView) convertView.findViewById(R.id.item_config_title);
            holder.url = (TextView) convertView.findViewById(R.id.item_config_url);
            holder.button = (RadioButton) convertView.findViewById(R.id.toggle_button);
            holder.additionalContent = (RelativeLayout) convertView.findViewById(R.id.item_config_additional_content);
            convertView.setTag(holder);
        }

        holder = (ViewHolder) convertView.getTag();
        Config config = mConfigsList.get(position);
        holder.title.setText(config.getTitle());
        holder.url.setText(getConfigurationType(config));

        convertView.setId((int) config.getId());

        if (mSelectedPositions.contains(position)) {
            holder.button.setChecked(true);
        } else {
            holder.button.setChecked(false);
        }

        if (mAdditionalContentProvider != null) {
            mAdditionalContentProvider.fillView(config, position, holder.additionalContent);
        }
        return convertView;
    }


    private String getConfigurationType(Config config) {
        if (config.getRequestAccessNumber()) {
            return mContext.getResources().getString(R.string.request_an_title);
        } else
            if (config.getRequestOtp()) {
                return mContext.getResources().getString(R.string.request_otp_title);
            }

        return mContext.getResources().getString(R.string.request_mobile_title);
    }


    private void setSelectedPositions(int... selectedPos) {
        if (selectedPos != null) {
            mSelectedPositions.clear();

            for (int pos : selectedPos) {
                mSelectedPositions.add(pos);
            }
        }
    }

    private static class ViewHolder {

        TextView       title;
        TextView       url;
        RadioButton    button;
        RelativeLayout additionalContent;
    }
}
