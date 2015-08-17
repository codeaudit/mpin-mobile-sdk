package com.certivox.adapters;


import java.util.List;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

import com.certivox.enums.GuideFragmentsEnum;
import com.certivox.fragments.GuideFragment;


public class GuidePagerAdapter extends FragmentPagerAdapter {

    private List<GuideFragmentsEnum> mFragmentList;


    public GuidePagerAdapter(List<GuideFragmentsEnum> fragmentList, FragmentManager fm) {
        super(fm);
        mFragmentList = fragmentList;
    }


    @Override
    public Fragment getItem(int position) {
        return GuideFragment.newInstance(mFragmentList.get(position));
    }


    @Override
    public int getCount() {
        return mFragmentList.size();
    }

}
