package com.certivox.activities;

import android.app.Activity;
import android.os.Bundle;

import com.certivox.fragments.ConfigDetailFragment;

public class ConfigDetailActivity extends Activity {

	public static final String EXTRA_ID = "ConfigDetailActivity.EXTRA_ID";
	
	@Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        long id = -1;
        if (savedInstanceState != null) {
        	id = savedInstanceState.getLong(EXTRA_ID, -1);
        } else {
        	id = getIntent().getLongExtra(EXTRA_ID, -1);
        }
        
        getFragmentManager().beginTransaction().replace(android.R.id.content, ConfigDetailFragment.newInstance(id)).commit();
    }
	
}
