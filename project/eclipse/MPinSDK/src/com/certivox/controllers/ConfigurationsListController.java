package com.certivox.controllers;

import com.certivox.interfaces.Controller;

public class ConfigurationsListController extends Controller {

	// MPinController
	private MPinController mMPinController;

	// Receive Messages
	public static final int GET_CONFIGURATIONS_LIST = 1;

	public ConfigurationsListController(MPinController mPinController) {
		mMPinController = mPinController;
	}

	@Override
	public boolean handleMessage(int what, Object data) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean handleMessage(int what) {
		// TODO Auto-generated method stub
		return false;
	}

}
