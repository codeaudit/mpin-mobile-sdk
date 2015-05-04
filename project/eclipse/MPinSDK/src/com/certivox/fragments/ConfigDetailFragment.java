package com.certivox.fragments;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Fragment;
import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.webkit.URLUtil;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;

import com.certivox.activities.MPinActivity;
import com.certivox.db.ConfigsContract.ConfigEntry;
import com.certivox.db.ConfigsDbHelper;
import com.certivox.interfaces.ConfigController;
import com.certivox.models.Status;
import com.certivox.mpinsdk.Config;
import com.example.mpinsdk.R;

public class ConfigDetailFragment extends Fragment {

	private View mView;
	private EditText mServiceNameEditText;
	private EditText mServiceUrlEditText;
	private EditText mServiceRTSEditText;
	private CheckBox mServiceMobileCheckBox;
	private CheckBox mServiceOTPCheckBox;
	private CheckBox mServiceANCheckBox;
	private Button mCheckServiceButton;
	private Button mSaveServiceButton;

	private long configId;
	private ConfigController controller;

	private Config mConfig;

	public void setController(ConfigController controller) {
		this.controller = controller;
	}

	public void setConfigId(long configId) {
		this.configId = configId;
	}

	@Override
	public void onAttach(Activity activity) {
		super.onAttach(activity);

		mConfig = new Config();

		if (configId != -1) {
			SQLiteDatabase db = new ConfigsDbHelper(activity)
					.getReadableDatabase();
			Cursor cursor = null;
			try {
				cursor = db.query(ConfigEntry.TABLE_NAME,
						ConfigEntry.getFullProjection(), ConfigEntry._ID
								+ " LIKE ?",
						new String[] { String.valueOf(configId) }, null, null,
						null);
				if (cursor.moveToFirst()) {
					mConfig.formCursor(cursor);
				}
			} finally {
				if (cursor != null)
					cursor.close();
			}
		}
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		mView = inflater.inflate(R.layout.config_details_layout, container,
				false);
		initViews();
		return mView;
	}

	private void updateDb() {
		if (mConfig == null)
			return;
		SQLiteDatabase db = new ConfigsDbHelper(this.getActivity())
				.getReadableDatabase();
		ContentValues values = new ContentValues();
		mConfig.toContentValues(values);
		if (mConfig.getId() == -1) {
			mConfig.setId(db.insert(ConfigEntry.TABLE_NAME, null, values));
		} else {
			db.update(ConfigEntry.TABLE_NAME, values, ConfigEntry._ID
					+ " LIKE ?", new String[] { String.valueOf(configId) });
		}
	}

	private void initViews() {

		mServiceNameEditText = (EditText) mView
				.findViewById(R.id.service_name_input);
		mServiceUrlEditText = (EditText) mView
				.findViewById(R.id.service_url_input);
		mServiceRTSEditText = (EditText) mView
				.findViewById(R.id.service_rts_input);

		mServiceMobileCheckBox = (CheckBox) mView
				.findViewById(R.id.service_mobile);
		mServiceOTPCheckBox = (CheckBox) mView.findViewById(R.id.service_otp);
		mServiceANCheckBox = (CheckBox) mView.findViewById(R.id.service_an);

		mCheckServiceButton = (Button) mView
				.findViewById(R.id.check_service_button);
		mSaveServiceButton = (Button) mView
				.findViewById(R.id.save_service_button);

		mCheckServiceButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				checkBackend(false);
			}
		});

		mSaveServiceButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				checkBackend(true);
			}
		});

		if (mConfig != null) {
			mServiceNameEditText.setText(mConfig.getTitle());
			mServiceUrlEditText.setText(mConfig.getBackendUrl());
			mServiceRTSEditText.setText(mConfig.getRTS());
			mServiceMobileCheckBox.setChecked(!mConfig.getRequestOtp()
					&& !mConfig.getRequestAccessNumber());
			mServiceOTPCheckBox.setChecked(mConfig.getRequestOtp());
			mServiceANCheckBox.setChecked(mConfig.getRequestAccessNumber());

			mServiceMobileCheckBox
					.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
						@Override
						public void onCheckedChanged(CompoundButton buttonView,
								boolean isChecked) {
							if (isChecked) {
								mServiceOTPCheckBox.setChecked(false);
								mServiceANCheckBox.setChecked(false);
							} else if (!mServiceOTPCheckBox.isChecked()
									&& !mServiceANCheckBox.isChecked()) {
								buttonView.setChecked(true);
							}

						}
					});

			mServiceOTPCheckBox
					.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
						@Override
						public void onCheckedChanged(CompoundButton buttonView,
								boolean isChecked) {
							if (isChecked) {
								mServiceMobileCheckBox.setChecked(false);
								mServiceANCheckBox.setChecked(false);
							} else if (!mServiceMobileCheckBox.isChecked()
									&& !mServiceANCheckBox.isChecked()) {
								buttonView.setChecked(true);
							}
						}
					});

			mServiceANCheckBox
					.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
						@Override
						public void onCheckedChanged(CompoundButton buttonView,
								boolean isChecked) {
							if (isChecked) {
								mServiceOTPCheckBox.setChecked(false);
								mServiceMobileCheckBox.setChecked(false);
							} else if (!mServiceOTPCheckBox.isChecked()
									&& !mServiceMobileCheckBox.isChecked()) {
								buttonView.setChecked(true);
							}
						}
					});
		}
	}

	private void onValidBackend(String backendUrl) {
		// Setting service name
		String serviceName = mServiceNameEditText.getText().toString();
		mConfig.setTitle(serviceName);
		// Setting service url
		mConfig.setBackendUrl(backendUrl);
		String rts = mServiceRTSEditText.getText().toString();
		// Setting rts
		mConfig.setRTS(rts);
		// Set OTP
		boolean otpChecked = mServiceOTPCheckBox.isChecked();
		mConfig.setRequestOtp(otpChecked);

		// set AccessNumber
		boolean anChecked = mServiceANCheckBox.isChecked();
		mConfig.setRequestAccessNumber(anChecked);
		updateDb();
		controller.configurationSaved();
	}

	private void checkBackend(final boolean saveIfCorrect) {
		final Activity activity = this.getActivity();
		final String backendUrl = mServiceUrlEditText.getText().toString();

		if (!URLUtil.isValidUrl(backendUrl)) {
			new AlertDialog.Builder(activity).setTitle("Invalid URL address")
					.setMessage("Try Again").setPositiveButton("OK", null)
					.show();

			mSaveServiceButton.setClickable(true);
			mSaveServiceButton.setEnabled(true);
		} else {
			new Thread(new Runnable() {
				@Override
				public void run() {
					Status status = MPinActivity.sdk().TestBackend(backendUrl);
					if (status.getStatusCode() != Status.Code.OK) {
						activity.runOnUiThread(new Runnable() {
							@Override
							public void run() {
								new AlertDialog.Builder(activity)
										.setTitle("Error")
										.setMessage("Invalid backend URL")
										.setPositiveButton("OK", null).show();

								mSaveServiceButton.setEnabled(true);
							}
						});
						return;
					} else {
						activity.runOnUiThread(new Runnable() {
							@Override
							public void run() {
								if (saveIfCorrect) {
									onValidBackend(backendUrl);
								} else {
									new AlertDialog.Builder(activity)
											.setTitle("Success")
											.setMessage("The backend URL is correct!")
											.setPositiveButton("OK", null)
											.show();
								}
							}
						});
					}
				}
			}).start();
		}
	}
}
