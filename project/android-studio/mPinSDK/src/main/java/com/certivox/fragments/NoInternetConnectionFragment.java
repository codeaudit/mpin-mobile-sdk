package com.certivox.fragments;


import com.certivox.constants.FragmentTags;
import com.certivox.mpinsdk.R;

import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;


public class NoInternetConnectionFragment extends MPinFragment {

    private View mContentView;


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        disableDrawer();
        setToolbarTitle(R.string.about_title);
        mContentView = inflater.inflate(R.layout.fragment_no_internet_connection, container, false);
        initViews();
        return mContentView;
    }


    @Override
    public boolean handleMessage(Message msg) {
        return false;
    }


    @Override
    protected void initViews() {
        setToolbarTitle(R.string.no_internet_title);
    }


    @Override
    public void setData(Object data) {
    }


    @Override
    protected OnClickListener getDrawerBackClickListener() {
        return null;
    }


    @Override
    protected String getFragmentTag() {
        return FragmentTags.FRAGMENT_NO_INTERNET_CONNECTION;
    }

}
