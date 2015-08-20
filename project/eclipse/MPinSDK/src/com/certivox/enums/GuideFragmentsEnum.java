package com.certivox.enums;


import com.certivox.mpinsdk.R;


public enum GuideFragmentsEnum {
    FRAGMENT_GD_CREATE_IDENTITY(R.layout.fragment_gd_create_identity), FRAGMENT_GD_CONFIRM_EMAIL(
            R.layout.fragment_gd_confirm_email), FRAGMENT_GD_CREATE_PIN(
                    R.layout.fragment_gd_create_pin), FRAGMENT_GD_READY_TO_GO(
                            R.layout.fragment_gd_ready_to_go), FRAGMENT_GD_GET_ACCESS_NUMBER(
                                    R.layout.fragment_gd_get_access_number), FRAGMENT_GD_DOWNLOAD_SERVER(
                                            R.layout.fragment_gd_download_server), FRAGMENT_GD_ADD_SERVER_TO_APP(
                                                    R.layout.fragment_gd_add_server_to_app);

    private int mResourceId;


    private GuideFragmentsEnum(int resourceId) {
        mResourceId = resourceId;
    }


    public int getResourceId() {
        return mResourceId;
    }
}
