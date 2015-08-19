package com.certivox.activities;


import java.util.ArrayList;

import com.certivox.constants.IntentConstants;
import com.certivox.dal.AppInstanceInfoDao;
import com.certivox.enums.GuideFragmentsEnum;
import com.certivox.mpinsdk.R;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;


public class SplashActivity extends ActionBarActivity {

    private AppInstanceInfoDao mInstructionsDao;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_guide);
        mInstructionsDao = new AppInstanceInfoDao(getApplicationContext());
        setInitialActivity();
    }


    private void setInitialActivity() {
        Intent guideIntent = null;
        Intent mpinIntent = new Intent(this, MPinActivity.class);

        if (mInstructionsDao.isFirstStart()) {
            mInstructionsDao.setIsFirstStart(false);
            guideIntent = new Intent(this, GuideActivity.class);
            guideIntent.putExtra(IntentConstants.FRAGMENT_LIST, getFirstStartGuideFragments());
        }

        if (guideIntent != null) {
            startActivities(new Intent[] {
                    mpinIntent, guideIntent
            });
        } else {
            startActivity(mpinIntent);
        }

        finish();
    }


    private ArrayList<GuideFragmentsEnum> getFirstStartGuideFragments() {
        ArrayList<GuideFragmentsEnum> fragmentList = new ArrayList<GuideFragmentsEnum>();
        fragmentList.add(GuideFragmentsEnum.FRAGMENT_GD_CREATE_IDENTITY);
        fragmentList.add(GuideFragmentsEnum.FRAGMENT_GD_CONFIRM_EMAIL);
        fragmentList.add(GuideFragmentsEnum.FRAGMENT_GD_CREATE_PIN);
        fragmentList.add(GuideFragmentsEnum.FRAGMENT_GD_READY_TO_GO);

        return fragmentList;
    }
}
