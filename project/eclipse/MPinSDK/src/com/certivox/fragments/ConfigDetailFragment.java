package com.certivox.fragments;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Fragment;
import android.content.DialogInterface;
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

import com.certivox.activities.MPinActivityOld;
import com.certivox.dal.ConfigsDao;
import com.certivox.interfaces.ConfigController;
import com.certivox.models.Config;
import com.certivox.models.Status;
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

	private ConfigController controller;

	private Config mConfig;
	private ConfigsDao mConfigsDao;
	private long mConfigId;
	private String mConfigURL;

	private static final int INVALID_URL = 0;
	private static final int INVALID_BACKEND = 1;
	private static final int VALID_BACKEND = 2;

	private Status mChechBackendStatus;

	public void setController(ConfigController controller) {
		this.controller = controller;
	}

	public void setConfigId(long configId) {
		this.mConfigId = configId;
	}

	@Override
	public void onAttach(Activity activity) {
		super.onAttach(activity);
		mConfigsDao = new ConfigsDao(activity.getApplicationContext());
		mConfig = new Config();
		if (mConfigId != -1) {
			initConfig();
		}
	}

	private void initConfig() {
		mConfig = mConfigsDao.getConfigurationById(mConfigId);
		mConfigURL = mConfig.getBackendUrl();
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
		if (mConfig == null) {
			return;
		}

		mConfigsDao.saveOrUpdate(mConfig);
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
				switch (checkBackend()) {
				case INVALID_URL:
					showInvalidURLDialog();
					break;
				case INVALID_BACKEND:
					showInvalidBackednURL();
					break;
				case VALID_BACKEND:
					showValidBackendDialog();
					break;
				default:
					break;
				}
			}
		});

		mSaveServiceButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				if (isEmptyTitle()) {
					showEmptyTitleDialog();
				} else {
					switch (checkBackend()) {
					case INVALID_URL:
						showInvalidURLDialog();
						break;
					case INVALID_BACKEND:
						showInvalidBackednURL();
						break;
					case VALID_BACKEND:
						preSaveConfiguration();
						break;
					default:
						break;
					}
				}
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

	private boolean isEmptyTitle() {
		if (mServiceNameEditText.getText().toString().trim().length() == 0) {
			return true;
		}
		return false;
	}

	private void showInvalidURLDialog() {
		new AlertDialog.Builder(getActivity()).setTitle("Invalid URL address")
				.setMessage("Try Again").setPositiveButton("OK", null).show();
	}

	private void showInvalidBackednURL() {
		new AlertDialog.Builder(getActivity()).setTitle("Invalid backend URL")
				.setMessage("Try Again").setPositiveButton("OK", null).show();
	}

	private void showValidBackendDialog() {
		new AlertDialog.Builder(getActivity()).setTitle("Success")
				.setMessage("The backend URL is correct!")
				.setPositiveButton("OK", null).show();
	}

	private void showEmptyTitleDialog() {
		new AlertDialog.Builder(getActivity()).setTitle("Error")
				.setMessage("Service name field is empty")
				.setPositiveButton("OK", null).show();
	}

	private void saveConfiguration(String backendUrl) {
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

	private int checkBackend() {
		controller.showLoader();
		final String backendUrl = mServiceUrlEditText.getText().toString();
		if (!URLUtil.isValidUrl(backendUrl)) {
			controller.hideLoader();
			return INVALID_URL;
		} else {
			mChechBackendStatus = null;
			Thread checkBackendThread = new Thread(new Runnable() {
				@Override
				public void run() {
					mChechBackendStatus = MPinActivityOld.sdk().TestBackend(
							backendUrl);
				}
			});

			checkBackendThread.start();
			try {
				checkBackendThread.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			controller.hideLoader();
			if (mChechBackendStatus.getStatusCode() != Status.Code.OK) {
				return INVALID_BACKEND;
			} else {
				return VALID_BACKEND;
			}

		}
	}

	private void preSaveConfiguration() {
		final String backendUrl = mServiceUrlEditText.getText().toString();
		if (mConfigId != -1 && !mConfigURL.equals(backendUrl)) {
			new AlertDialog.Builder(getActivity())
					.setTitle("Updating configuration")
					.setMessage(
							"This action will also delete all identities, associated with this configuration.")
					.setPositiveButton("OK",
							new DialogInterface.OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog,
										int which) {
									saveConfiguration(backendUrl);
								}
							}).setNegativeButton("Cancel", null).show();
		} else {
			saveConfiguration(backendUrl);
		}
	}
}
