package com.certivox.activities;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.certivox.db.ConfigsDbHelper;

public class MainActivity extends Activity {

	public native String testStorage();

	public native String testHTTPRequest();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		Button btn = new Button(this);
		btn.setText("Go to MPIN");
		setContentView(btn);

		btn.setOnClickListener(new View.OnClickListener() {

			@Override
			public void onClick(View v) {
				startActivity(new Intent(MainActivity.this, MPinActivity.class));
				finish();
			}
		});

		new ConfigsDbHelper(this).getReadableDatabase();

		btn.performClick();
	}

}
