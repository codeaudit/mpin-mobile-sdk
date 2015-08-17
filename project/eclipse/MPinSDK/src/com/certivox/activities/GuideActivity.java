package com.certivox.activities;


import java.util.ArrayList;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.support.v7.app.ActionBarActivity;

import com.certivox.adapters.GuidePagerAdapter;
import com.certivox.constants.IntentConstants;
import com.certivox.enums.GuideFragmentsEnum;
import com.certivox.mpinsdk.R;


public class GuideActivity extends ActionBarActivity {

    private ViewPager mViewPager;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_guide);

        mViewPager = (ViewPager) findViewById(R.id.pager);
        initPager();
    }


    @SuppressWarnings("unchecked")
    private void initPager() {
        Intent intent = getIntent();
        ArrayList<GuideFragmentsEnum> fragmentList = (ArrayList<GuideFragmentsEnum>) intent
                .getSerializableExtra(IntentConstants.FRAGMENT_LIST);
        GuidePagerAdapter guidePagerAdapter = new GuidePagerAdapter(fragmentList, getSupportFragmentManager());
        mViewPager.setAdapter(guidePagerAdapter);
    }

}
