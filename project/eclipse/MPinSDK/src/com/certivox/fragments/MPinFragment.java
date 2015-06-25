package com.certivox.fragments;

import android.app.Activity;
import android.app.Fragment;
import android.os.Handler;
import android.support.v7.app.ActionBarActivity;

import com.certivox.controllers.MPinController;

public abstract class MPinFragment extends Fragment implements Handler.Callback {

	// MPinController
	private MPinController mMPinController;

	abstract protected void initViews();

	@Override
	public void onAttach(Activity activity) {
		super.onAttach(activity);
		if (mMPinController != null) {
			getMPinController().addOutboxHandler(new Handler(this));
		}
	}

	public void setMPinController(MPinController controller) {
		mMPinController = controller;
	}

	public MPinController getMPinController() {
		return mMPinController;
	}

	abstract public void setData(Object data);

	protected void setTooblarTitle(int resId) {
		((ActionBarActivity) getActivity()).getSupportActionBar().setTitle(
				resId);
	}

}
