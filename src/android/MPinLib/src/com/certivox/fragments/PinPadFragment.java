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


import com.certivox.models.User;
import com.certivox.mpin.R;
import com.certivox.view.SelectionCircles;

import android.app.AlertDialog;
import android.app.Fragment;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TextView;


public class PinPadFragment extends Fragment {

    private User                mUser;
    private View                mView;
    private TextView            mUserEmail;
    private TextView            mDigit0;
    private TextView            mDigit1;
    private TextView            mDigit2;
    private TextView            mDigit3;
    private TextView            mDigit4;
    private TextView            mDigit5;
    private TextView            mDigit6;
    private TextView            mDigit7;
    private TextView            mDigit8;
    private TextView            mDigit9;
    private ImageButton         mButtonLogin;
    private ImageButton         mButtonClear;
    private SelectionCircles    mSelectionCircles;
    private TextView            mWrongPinTextView;
    private OnClickListener     mOnDigitClickListener;
    private int                 mPinLength = 4;
    private final StringBuilder mInput     = new StringBuilder();
    private volatile boolean    mIsPinSet;


    public void setUser(User user) {
        mUser = user;
    }


    public void setPinLength(int length) {
        mPinLength = length;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        mView = inflater.inflate(R.layout.pinpad_layout, container, false);
        if (mUser == null) {
            showErrorDialog();
        } else {
            initViews();
            initPinPad();
        }
        return mView;
    }


    @Override
    public void onResume() {
        super.onResume();
        if (mUser != null) {
            if (mUser.getState().equals(User.State.REGISTERED)) {
                ((ActionBarActivity) getActivity()).getSupportActionBar().setTitle(R.string.enter_pin_title);
            } else {
                ((ActionBarActivity) getActivity()).getSupportActionBar().setTitle(R.string.setup_pin_title);
            }
        } else {
            showErrorDialog();
        }
    }


    private void initViews() {
        mUserEmail = (TextView) mView.findViewById(R.id.user_email);
        mUserEmail.setText(mUser.getId());

        mSelectionCircles = (SelectionCircles) mView.findViewById(R.id.pin_pad_circles);
        mSelectionCircles.setCount(mPinLength);

        mWrongPinTextView = (TextView) mView.findViewById(R.id.wrong_pin);
        mDigit0 = (TextView) mView.findViewById(R.id.pinpad_key_0);
        mDigit1 = (TextView) mView.findViewById(R.id.pinpad_key_1);
        mDigit2 = (TextView) mView.findViewById(R.id.pinpad_key_2);
        mDigit3 = (TextView) mView.findViewById(R.id.pinpad_key_3);
        mDigit4 = (TextView) mView.findViewById(R.id.pinpad_key_4);
        mDigit5 = (TextView) mView.findViewById(R.id.pinpad_key_5);
        mDigit6 = (TextView) mView.findViewById(R.id.pinpad_key_6);
        mDigit7 = (TextView) mView.findViewById(R.id.pinpad_key_7);
        mDigit8 = (TextView) mView.findViewById(R.id.pinpad_key_8);
        mDigit9 = (TextView) mView.findViewById(R.id.pinpad_key_9);

        mButtonLogin = (ImageButton) mView.findViewById(R.id.pinpad_key_login);
        mButtonClear = (ImageButton) mView.findViewById(R.id.pinpad_key_clear);
    }


    public void showWrongPin() {
        setEmptyPin();
        mWrongPinTextView.setVisibility(View.VISIBLE);
        mSelectionCircles.setDefaultColor(getResources().getColor(R.color.orange));
    }


    public void hideWrongPin() {
        mWrongPinTextView.setVisibility(View.INVISIBLE);
        mSelectionCircles.setDefaultColor(getResources().getColor(R.color.primaryColor));
    }


    private void setEmptyPin() {
        mIsPinSet = false;
        mInput.setLength(0);
        mSelectionCircles.deselectAll();
        updateButtons();
    }


    private void initPinPad() {
        setEmptyPin();
        mButtonLogin.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                synchronized (PinPadFragment.this) {
                    if (!mIsPinSet) {
                        mIsPinSet = true;
                        PinPadFragment.this.notifyAll();
                    }
                }
            }
        });

        mButtonClear.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                int inputLength = mInput.length();
                if (inputLength > 0) {
                    int newLenght = inputLength - 1;
                    mInput.setLength(newLenght);
                    mSelectionCircles.deselectPosition(newLenght);
                    updateButtons();
                }
            }
        });

        mDigit0.setOnClickListener(getOnDigitClickListener());
        mDigit1.setOnClickListener(getOnDigitClickListener());
        mDigit2.setOnClickListener(getOnDigitClickListener());
        mDigit3.setOnClickListener(getOnDigitClickListener());
        mDigit4.setOnClickListener(getOnDigitClickListener());
        mDigit5.setOnClickListener(getOnDigitClickListener());
        mDigit6.setOnClickListener(getOnDigitClickListener());
        mDigit7.setOnClickListener(getOnDigitClickListener());
        mDigit8.setOnClickListener(getOnDigitClickListener());
        mDigit9.setOnClickListener(getOnDigitClickListener());

    }


    private OnClickListener getOnDigitClickListener() {
        if (mOnDigitClickListener == null) {
            initOnDigitClickListener();
        }

        return mOnDigitClickListener;
    }


    private void initOnDigitClickListener() {
        mOnDigitClickListener = new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                if (mInput.length() >= mPinLength) {
                    return;
                }
                final int id = view.getId();
                if (id == R.id.pinpad_key_0) {
                    mInput.append('0');
                } else
                    if (id == R.id.pinpad_key_1) {
                        mInput.append('1');
                    } else
                        if (id == R.id.pinpad_key_2) {
                            mInput.append('2');
                        } else
                            if (id == R.id.pinpad_key_3) {
                                mInput.append('3');
                            } else
                                if (id == R.id.pinpad_key_4) {
                                    mInput.append('4');
                                } else
                                    if (id == R.id.pinpad_key_5) {
                                        mInput.append('5');
                                    } else
                                        if (id == R.id.pinpad_key_6) {
                                            mInput.append('6');
                                        } else
                                            if (id == R.id.pinpad_key_7) {
                                                mInput.append('7');
                                            } else
                                                if (id == R.id.pinpad_key_8) {
                                                    mInput.append('8');
                                                } else
                                                    if (id == R.id.pinpad_key_9) {
                                                        mInput.append('9');
                                                    }
                hideWrongPin();
                mSelectionCircles.selectPosition(mInput.length() - 1);
                updateButtons();
            }
        };

    }


    private String parsePin() {
        return mInput.toString();
    }


    @Override
    public void onDetach() {
        synchronized (this) {
            mInput.setLength(0);
            mIsPinSet = true;
            notifyAll();
        }

        super.onDetach();
    }


    public String getPin() {
        synchronized (this) {
            try {
                while (!mIsPinSet) {
                    wait();
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
                return "";
            }
        }
        String pin = parsePin();
        return pin;
    }


    private void updateButtons() {
        mButtonLogin.setEnabled(mInput.length() == mPinLength);
        mButtonClear.setEnabled(mInput.length() > 0);
    }


    private void showErrorDialog() {
        new AlertDialog.Builder(getActivity()).setTitle("Error").setMessage("Unexpected error occurred!")
                .setPositiveButton("OK", null).show();
    }
}
