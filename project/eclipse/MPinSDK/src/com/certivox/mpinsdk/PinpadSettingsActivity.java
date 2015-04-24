package com.certivox.mpinsdk;

import android.app.Activity;
import android.os.Bundle;

import com.certivox.fragments.ConfigListFragment;

public class PinpadSettingsActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		getFragmentManager().beginTransaction().add(android.R.id.content, new ConfigListFragment()).commit();
	}

}
