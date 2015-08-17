package com.certivox.activities;


import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;

import com.certivox.dal.InstructionsDao;


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
            mInstructionsDao.setIsFirstStart(false);
        } else {
            intent = new Intent(this, MPinActivity.class);
            startActivity(intent);
        }
        finish();
    }
}
