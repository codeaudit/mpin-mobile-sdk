/*******************************************************************************
 * Copyright (c) 2012-2015, Certivox All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * For full details regarding our CertiVox terms of service please refer to the following links:
 * 
 * * Our Terms and Conditions - http://www.certivox.com/about-certivox/terms-and-conditions/
 * 
 * * Our Security and Privacy - http://www.certivox.com/about-certivox/security-privacy/
 * 
 * * Our Statement of Position and Our Promise on Software Patents - http://www.certivox.com/about-certivox/patents/
 ******************************************************************************/
package com.certivox.fragments;


import com.certivox.activities.MPinActivity;
import com.certivox.controllers.MPinController;

import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.view.View.OnClickListener;


public abstract class MPinFragment extends Fragment implements Handler.Callback {

    private static final String TAG = MPinFragment.class.getCanonicalName();

    private Handler mHandler;


    abstract protected void initViews();


    abstract public void setData(Object data);


    abstract protected OnClickListener getDrawerBackClickListener();


    abstract protected String getFragmentTag();


    // TODO: replace this deprecated method with onAttach(Context context) after converting to use the
    // android.support.v4.Fragment class
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        mHandler = new Handler(this);

        hideKeyboard();
    }


    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        MPinController controller = getMPinController();
        if (controller != null) {
            controller.addOutboxHandler(mHandler);
            controller.setCurrentFragmentTag(getFragmentTag());
        }
    }


    @Override
    public void onDetach() {
        super.onDetach();
        getMPinController().removeOutboxHandler(mHandler);
    }


    public MPinController getMPinController() {
        return ((MPinActivity) getActivity()).getController();
    }


    protected void setToolbarTitle(int resId) {
        ((AppCompatActivity) getActivity()).getSupportActionBar().setTitle(resId);
    }


    protected void enableDrawer() {
        ((MPinActivity) getActivity()).enableDrawer();
    }


    protected void disableDrawer() {
        ((MPinActivity) getActivity()).disableDrawer(getDrawerBackClickListener());
    }


    protected void hideKeyboard() {
        ((MPinActivity) getActivity()).hideKeyboard();
    }
}
