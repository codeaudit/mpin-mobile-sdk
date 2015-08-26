package com.certivox.activities;


import java.util.ArrayList;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
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

    private static final String TAG = GuideActivity.class.getCanonicalName();

    private ViewPager mViewPager;

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
        mViewPager.setOnPageChangeListener(new OnPageChangeListener() {

            @Override
            public void onPageSelected(int pageNumber) {
                GuideActivity.this.showProperButtons(mViewPager.getAdapter().getCount() - 1 == pageNumber);
            }


            @Override
            public void onPageScrolled(int arg0, float arg1, int arg2) {
                // Do nothing
            }


            @Override
            public void onPageScrollStateChanged(int arg0) {
                // Do nothing
            }
        });

        showProperButtons(fragmentList.size() <= 1);
    }


    private void showProperButtons(boolean isEndOfPage) {
        if (isEndOfPage) {
            hideNextButton();
            hideSkipButton();
            showDoneButton();
        } else {
            hideDoneButton();
            showNextButton();
            showSkipButton();
        }
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


    private void showDoneButton() {
        mDoneButton.setVisibility(View.VISIBLE);
    }


    private void hideDoneButton() {
        mDoneButton.setVisibility(View.INVISIBLE);
    }


    private void showNextButton() {
        mNextButton.setVisibility(View.VISIBLE);
    }


    private void hideNextButton() {
        mNextButton.setVisibility(View.INVISIBLE);
    }


    private void showSkipButton() {
        mSkipButton.setVisibility(View.VISIBLE);
    }


    private void hideSkipButton() {
        mSkipButton.setVisibility(View.INVISIBLE);
    }
}
