package com.certivox.activities;


import java.util.ArrayList;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.support.v7.app.ActionBarActivity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageButton;

import com.certivox.adapters.GuidePagerAdapter;
import com.certivox.constants.IntentConstants;
import com.certivox.enums.GuideFragmentsEnum;
import com.certivox.mpinsdk.R;


public class GuideActivity extends ActionBarActivity {

    private ViewPager   mViewPager;

    private Button      mDoneButton;
    private ImageButton mNextButton;
    private Button      mSkipButton;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_guide);
        initViews();
        initPager();

        handleClickEvents();
    }


    private void initViews() {
        mDoneButton = (Button) findViewById(R.id.guide_done_button);
        mNextButton = (ImageButton) findViewById(R.id.guide_next_button);
        mSkipButton = (Button) findViewById(R.id.guide_skip_button);

        mViewPager = (ViewPager) findViewById(R.id.pager);
    }


    @SuppressWarnings("unchecked")
    private void initPager() {
        Intent intent = getIntent();
        ArrayList<GuideFragmentsEnum> fragmentList = (ArrayList<GuideFragmentsEnum>) intent
                .getSerializableExtra(IntentConstants.FRAGMENT_LIST);
        GuidePagerAdapter guidePagerAdapter = new GuidePagerAdapter(fragmentList, getSupportFragmentManager());
        mViewPager.setAdapter(guidePagerAdapter);
    }


    private void handleClickEvents() {
        mDoneButton.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                finish();
            }
        });

        mNextButton.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                mViewPager.setCurrentItem(mViewPager.getCurrentItem() + 1);
            }
        });

        mSkipButton.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                finish();
            }
        });
    }

}
