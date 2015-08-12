package com.certivox.fragments;


import com.certivox.constants.FragmentTags;
import com.certivox.controllers.MPinController;
import com.example.mpinsdk.R;

import android.os.Bundle;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;


public class NoInternetConnectionFragment extends MPinFragment {

    private View   mContentView;
    private Button mRetryButton;


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
        mRetryButton = (Button) mContentView.findViewById(R.id.retry_button);

        mRetryButton.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                getMPinController().handleMessage(MPinController.MESSAGE_RETRY_INITIALIZATION);
            }
        });
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
