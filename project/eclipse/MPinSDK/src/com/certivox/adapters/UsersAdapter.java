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


import java.util.List;

import com.certivox.models.User;
import com.certivox.mpinsdk.R;

import android.content.Context;
import android.support.v4.content.ContextCompat;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;


public class UsersAdapter extends BaseAdapter {

    private Context    mContext;
    private List<User> mUsersList;


    public UsersAdapter(Context context, List<User> usersList) {
        mContext = context;
        mUsersList = usersList;
    }


    public void updateUsersList(List<User> usersList) {
        mUsersList.clear();
        mUsersList.addAll(usersList);
        notifyDataSetChanged();
    }


    @Override
    public int getCount() {
        return mUsersList.size();
    }


    @Override
    public User getItem(int position) {
        return mUsersList.get(position);
    }


    @Override
    public long getItemId(int position) {
        return getItem(position).getId().hashCode();
    }


    @Override
    public boolean hasStableIds() {
        return true;
    }


    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder holder;
        if (convertView == null) {
            convertView = LayoutInflater.from(mContext).inflate(R.layout.users_list_item, parent, false);
            holder = new ViewHolder();
            holder.textView = (TextView) convertView.findViewById(R.id.fragment_users_list_item_name);
            convertView.setTag(holder);
        }

        holder = (ViewHolder) convertView.getTag();
        User user = getItem(position);
        holder.textView.setText(user.getId());
        if (user.isUserSelected()) {
            holder.textView.setBackgroundColor(ContextCompat.getColor(mContext,R.color.selected_item_background));
            holder.textView.setCompoundDrawablesWithIntrinsicBounds(
                    ContextCompat.getDrawable(mContext,R.drawable.ic_avatar_selected), null, null, null);
        } else {
            holder.textView.setBackgroundColor(ContextCompat.getColor(mContext,R.color.white));
            holder.textView.setCompoundDrawablesWithIntrinsicBounds(
                    ContextCompat.getDrawable(mContext,R.drawable.ic_avatar), null, null, null);
        }

        return convertView;
    }


    public void deselectAllUsers() {
        for (User user : mUsersList) {
            user.setUserSelected(false);
        }
        notifyDataSetChanged();
    }


    public void setActiveUser(int position) {
        deselectAllUsers();
        User user = mUsersList.get(position);
        user.setUserSelected(true);
        notifyDataSetChanged();
    }

    private class ViewHolder {

        TextView textView;
    }
}
