package com.certivox.enums;


import com.certivox.mpinsdk.R;


public enum GuideFragmentsEnum {
    FRAGMENT_1(R.layout.fragment_gd_create_identity), FRAGMENT_2(R.layout.fragment_gd_confirm_email), FRAGMENT_3(R.layout.fragment_gd_create_pin), FRAGMENT_4(
            R.layout.fragment_gd_ready_to_go);

    private int mResourceId;


    private GuideFragmentsEnum(int resourceId) {
        mResourceId = resourceId;
    }


    public int getResourceId() {
        return mResourceId;
    }
}
