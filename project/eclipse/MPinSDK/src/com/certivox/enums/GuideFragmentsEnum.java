package com.certivox.enums;


import com.certivox.mpinsdk.R;


public enum GuideFragmentsEnum {
    FRAGMENT_1(R.layout.fragment_gd_create_identity), FRAGMENT_2(R.layout.fragment_guide), FRAGMENT_3(R.layout.fragment_guide), FRAGMENT_4(
            R.layout.fragment_guide);

    private int mResourceId;


    private GuideFragmentsEnum(int resourceId) {
        mResourceId = resourceId;
    }


    public int getResourceId() {
        return mResourceId;
    }
}
