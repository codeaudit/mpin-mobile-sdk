package com.certivox.fragments;


import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.os.Message;
import android.text.method.LinkMovementMethod;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.TextView;

import com.certivox.constants.FragmentTags;
import com.certivox.controllers.MPinController;
import com.example.mpinsdk.R;


public class AboutFragment extends MPinFragment {

    private View     mView;
    private TextView mLinkTextView;
    private TextView mVersionTextView;
    private TextView mBuildTextView;


    @Override
    public void setData(Object data) {
    };


    @Override
    protected String getFragmentTag() {
        return FragmentTags.FRAGMENT_ABOUT;
    }


    @Override
    public boolean handleMessage(Message msg) {
        return false;
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


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        disableDrawer();
        setTooblarTitle(R.string.about_title);
        mView = inflater.inflate(R.layout.fragment_about, container, false);
        initViews();
        setVersion();
        return mView;
    }


    @Override
    protected void initViews() {
        mLinkTextView = (TextView) mView.findViewById(R.id.terms_and_conditions_link);
        mLinkTextView.setMovementMethod(LinkMovementMethod.getInstance());
        mVersionTextView = (TextView) mView.findViewById(R.id.about_version);
        mBuildTextView = (TextView) mView.findViewById(R.id.about_build);
    }


    private void setVersion() {
        PackageInfo pInfo;
        try {
            pInfo = getActivity().getPackageManager().getPackageInfo(getActivity().getPackageName(), 0);

            String versionName = pInfo.versionName;
            int versionCode = pInfo.versionCode;

            mVersionTextView.setText(versionName);
            mBuildTextView.setText(versionCode + "");

        } catch (NameNotFoundException e) {
            e.printStackTrace();
        }

    }
}
