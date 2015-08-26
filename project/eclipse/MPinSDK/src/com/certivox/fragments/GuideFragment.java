package com.certivox.fragments;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.certivox.enums.GuideFragmentsEnum;


public class GuideFragment extends Fragment {

    private View                mView;
    private static final String KEY_LAYOUT_ID = "KEY_LAYOUT_ID";


    public static GuideFragment newInstance(GuideFragmentsEnum fragment) {
        GuideFragment guideFragment = new GuideFragment();
        Bundle args = new Bundle();
        args.putInt(KEY_LAYOUT_ID, fragment.getResourceId());
        guideFragment.setArguments(args);

        return guideFragment;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        int layoutId = getArguments().getInt(KEY_LAYOUT_ID, -1);
        mView = inflater.inflate(layoutId, container, false);

        return mView;
    }

}
