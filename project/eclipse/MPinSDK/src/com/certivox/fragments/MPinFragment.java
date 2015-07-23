package com.certivox.fragments;

import android.app.Activity;
import android.app.Fragment;
import android.os.Handler;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.View.OnClickListener;

import com.certivox.activities.MPinActivity;
import com.certivox.controllers.MPinController;

public abstract class MPinFragment extends Fragment implements Handler.Callback {

	private static final String TAG = MPinFragment.class.getCanonicalName();

	// MPinController
	private MPinController mMPinController;
	private Handler mHandler;

	abstract protected void initViews();

	abstract public void setData(Object data);

	abstract protected OnClickListener getDrawerBackClickListener();

	abstract protected String getFragmentTag();

	@Override
	public void onAttach(Activity activity) {
		Log.i(TAG, "Fragment on Attach");
		super.onAttach(activity);
		mHandler = new Handler(this);
		if (mMPinController != null) {
			getMPinController().addOutboxHandler(mHandler);
		}

		mMPinController.setCurrentFragmentTag(getFragmentTag());
		hideKeyboard();
	}

	@Override
	public void onDetach() {
		Log.i(TAG, "Fragment on Detach");
		super.onDetach();
		getMPinController().removeOutboxHandler(mHandler);
	}

	public void setMPinController(MPinController controller) {
		mMPinController = controller;
	}

	public MPinController getMPinController() {
		return mMPinController;
	}

	protected void setTooblarTitle(int resId) {
		((ActionBarActivity) getActivity()).getSupportActionBar().setTitle(
				resId);
	}

	protected void enableDrawer() {
		((MPinActivity) getActivity()).enableDrawer();
	}

	protected void disableDrawer() {
		((MPinActivity) getActivity())
				.disableDrawer(getDrawerBackClickListener());
	}

	protected void hideKeyboard() {
		((MPinActivity) getActivity()).hideKeyboard();
	}
}
