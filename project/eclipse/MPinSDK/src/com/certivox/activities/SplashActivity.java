package com.certivox.activities;


import java.util.ArrayList;
import java.util.List;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;

import com.certivox.constants.IntentConstants;
import com.certivox.dal.InstructionsDao;
import com.certivox.enums.GuideFragmentsEnum;


public class SplashActivity extends ActionBarActivity {

    private InstructionsDao mInstructionsDao;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mInstructionsDao = new InstructionsDao(getApplicationContext());
        setInitialActivity();
    }


    private void setInitialActivity() {
        Intent intent;
        if (mInstructionsDao.isFirstStart()) {
            //            mInstructionsDao.setIsFirstStart(false);
            intent = new Intent(this, GuideActivity.class);
            intent.putExtra(IntentConstants.FRAGMENT_LIST, getFirstStartGuideFragments());
            startActivity(intent);
        } else {
            intent = new Intent(this, MPinActivity.class);
            startActivity(intent);
        }
        finish();
    }


    private ArrayList<GuideFragmentsEnum> getFirstStartGuideFragments() {
        ArrayList<GuideFragmentsEnum> fragmentList = new ArrayList<GuideFragmentsEnum>();
        fragmentList.add(GuideFragmentsEnum.FRAGMENT_1);
        fragmentList.add(GuideFragmentsEnum.FRAGMENT_3);
        fragmentList.add(GuideFragmentsEnum.FRAGMENT_4);

        return fragmentList;
    }
}
