/*******************************************************************************
 * Copyright (c) 2012-2015, Certivox All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * For full details regarding our CertiVox terms of service please refer to the following links:
 * 
 * * Our Terms and Conditions - http://www.certivox.com/about-certivox/terms-and-conditions/
 * 
 * * Our Security and Privacy - http://www.certivox.com/about-certivox/security-privacy/
 * 
 * * Our Statement of Position and Our Promise on Software Patents - http://www.certivox.com/about-certivox/patents/
 ******************************************************************************/
package com.certivox.activities;


import java.util.ArrayList;

import com.certivox.adapters.GuidePagerAdapter;
import com.certivox.constants.IntentConstants;
import com.certivox.enums.GuideFragmentsEnum;
import com.certivox.mpinsdk.R;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.view.ViewPager;
import android.support.v4.view.ViewPager.OnPageChangeListener;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageButton;


public class GuideActivity extends AppCompatActivity {

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
        mViewPager.addOnPageChangeListener(new OnPageChangeListener() {

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
