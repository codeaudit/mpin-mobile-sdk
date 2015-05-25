package com.certivox.fragments;

import java.util.HashMap;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Fragment;
import android.content.ContentValues;
import android.content.Context;
import android.content.DialogInterface;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.os.Process;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Toast;

import com.certivox.activities.MPinActivity;
import com.certivox.db.ConfigsContract.ConfigEntry;
import com.certivox.db.ConfigsDbHelper;
import com.certivox.models.Status;
import com.certivox.mpinsdk.Config;
import com.example.mpinsdk.R;

public class ConfigDetailFragment extends Fragment {

	private static final String KEY_ID = "KEY_ID";
	
	private Config m_config;
	
	public static ConfigDetailFragment newInstance(long id) {
		ConfigDetailFragment frag = new ConfigDetailFragment();
        Bundle args = new Bundle();
        args.putLong(KEY_ID, id);
        frag.setArguments(args);
        return frag;
    }
	
	@Override
	public void onAttach(Activity activity) {
		super.onAttach(activity);
		
		m_config = new Config();
		
		long id = getArguments().getLong(KEY_ID, -1);
		
		if (id != -1) {
			SQLiteDatabase db = new ConfigsDbHelper(activity).getReadableDatabase();
			Cursor c = null;
			try {
				c = db.query(ConfigEntry.TABLE_NAME, ConfigEntry.getFullProjection(),
						ConfigEntry._ID + " LIKE ?",
						new String[] {String.valueOf(id)}, null, null, null);
				if (c.moveToFirst()) {
					m_config.formCursor(c);
				}
			} finally {
				if (c != null) c.close();
			}
		} else {
			m_config.setTitle("My new configuration");
			m_config.setBackendUrl("http://");
		}
	}

	private void updateDb(Context context) {
		if (m_config == null) return;
		SQLiteDatabase db = new ConfigsDbHelper(context).getReadableDatabase();
		ContentValues values = new ContentValues();
		m_config.toContentValues(values);
		if (m_config.getId() == -1) {
			m_config.setId(db.insert(ConfigEntry.TABLE_NAME, null, values));
		} else {
			db.update(ConfigEntry.TABLE_NAME, values, ConfigEntry._ID + " LIKE ?",
						new String[] {String.valueOf(getArguments().getLong(KEY_ID, -1))});
		}
		updateView(getView());
	}
	
	private void updateView(View view) {
		if (m_config != null && view != null) {
			Button title = (Button) view.findViewById(R.id.fragment_config_detail_title);
			Button url = (Button) view.findViewById(R.id.fragment_config_detail_url);
			final CheckBox otp = (CheckBox) view.findViewById(R.id.fragment_config_detail_otp);
			final CheckBox an = (CheckBox)view.findViewById(R.id.fragment_config_detail_an);
			
			title.setText(m_config.getTitle());
			url.setText(m_config.getBackendUrl());
			otp.setChecked(m_config.getRequestOtp());
			an.setChecked(m_config.getRequestAccessNumber());
			
			title.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					final Activity activity = getActivity();
					if (activity == null) return;
					final EditText input = new EditText(activity);
					input.setText(m_config.getTitle());
					input.setSelectAllOnFocus(true);
					new AlertDialog.Builder(activity)
							.setTitle("Set configuration name")
							.setView(input)
							.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog, int which) {
									if (m_config != null) {
										m_config.setTitle(input.getText().toString());
										updateDb(activity);
									}
								}
							}).setNegativeButton("Cancel", null).show();
					
				}
			});
			
			url.setOnClickListener(new View.OnClickListener() {
				@Override
				public void onClick(View v) {
					final Activity activity = getActivity();
					if (activity == null) return;
					final EditText input = new EditText(activity);
					input.setInputType(EditorInfo.TYPE_TEXT_VARIATION_URI);
					input.setText(m_config.getBackendUrl());
					input.setSelectAllOnFocus(true);
					new AlertDialog.Builder(activity)
							.setTitle("Set backend URL")
							.setView(input)
							.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog, int which) {
									setBackend(input.getText().toString(), activity);
								}
							}).setNegativeButton("Cancel", null).show();
					
				}
			});
			
			otp.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
				@Override
				public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
					Activity activity = getActivity();
					if (activity == null) return;
					if (isChecked && an.isChecked()) {
						an.setChecked(false);
					}
					m_config.setRequestOtp(isChecked);
					updateDb(activity);
				}
			});
			
			an.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
				@Override
				public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
					Activity activity = getActivity();
					if (activity == null) return;
					if (isChecked && otp.isChecked()) {
						otp.setChecked(false);
					}
					m_config.setRequestAccessNumber(isChecked);
					updateDb(activity);
				}
			});
		}
	}
	
	private void setBackend(final String backend, final Activity activity) {
		if (m_config == null) return;
		Toast.makeText(activity, "Testing backend...", Toast.LENGTH_SHORT).show();
		
		new Thread(new Runnable() {
			@Override
			public void run() {
				Process.setThreadPriority(Process.THREAD_PRIORITY_BACKGROUND);
				Status status = MPinActivity.sdk().TestBackend(backend);
				if (status.getStatusCode() != Status.Code.OK) {
					activity.runOnUiThread(new Runnable() {
						@Override
						public void run() {
							new AlertDialog.Builder(activity).setTitle("Error")
								.setMessage("Invalid backend URL")
								.setPositiveButton("OK", null).show();
						}
					});
					return;
				} else {
					activity.runOnUiThread(new Runnable() {
						@Override
						public void run() {
							HashMap<String, String> cfg = new HashMap<String, String>();
							cfg.put("RPA_server", backend);
							MPinActivity.init(activity.getApplicationContext(), cfg);
							
							m_config.setBackendUrl(backend);
							updateDb(activity);
							new AlertDialog.Builder(activity).setTitle("Success")
								.setMessage("Backend URL changed successfully")
								.setPositiveButton("OK", null).show();
						}
					});
				}
			}
		}).start();
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		View view = inflater.inflate(R.layout.config_details_layout, container, false);
		updateView(view);
		return view;
	}
	
}
