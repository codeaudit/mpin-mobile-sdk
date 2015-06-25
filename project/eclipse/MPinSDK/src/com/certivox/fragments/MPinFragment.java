package com.certivox.fragments;

import android.app.Fragment;

import com.certivox.controllers.MPinController;

public abstract class MPinFragment extends Fragment {

	// MPinController
	private MPinController mMPinController;

	abstract protected void initViews();

	public void setMPinController(MPinController mController2) {
		mMPinController = mController2;
	}

	public MPinController getMPinController() {
		return mMPinController;
	}

}
