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
        setContentView(R.layout.activity_splash);
        mInstructionsDao = new AppInstanceInfoDao(getApplicationContext());
    }


    @Override
    protected void onResume() {
        super.onResume();
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
