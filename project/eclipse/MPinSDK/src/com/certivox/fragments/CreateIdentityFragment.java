package com.certivox.fragments;


import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Message;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;

import com.certivox.constants.FragmentTags;
import com.certivox.controllers.MPinController;
import com.example.mpinsdk.R;


public class CreateIdentityFragment extends MPinFragment implements OnClickListener {

    private View     mView;
    private EditText mEmailEditText;
    private Button   mCreateIdentitiyButton;


    @Override
    public void setData(Object data) {

    }


    @Override
    protected OnClickListener getDrawerBackClickListener() {
        return null;
    }


    @Override
    protected String getFragmentTag() {
        return FragmentTags.FRAGMENT_CREATE_IDENTITY;
    }


    @Override
    public boolean handleMessage(Message msg) {
        switch (msg.what) {
        case MPinController.MESSAGE_IDENTITY_EXISTS:
            showIdentityExistsDialog();
            break;

        default:
            break;
        }
        return false;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        mView = inflater.inflate(R.layout.fragment_create_identity, container, false);
        initViews();
        return mView;
    }


    @Override
    protected void initViews() {
        setTooblarTitle(R.string.add_identity_title);

        mEmailEditText = (EditText) mView.findViewById(R.id.email_input);
        mCreateIdentitiyButton = (Button) mView.findViewById(R.id.create_identity_button);

        mCreateIdentitiyButton.setOnClickListener(this);
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()) {
        case R.id.create_identity_button:
            onCreateIdentity();
            break;
        default:
            break;
        }
    }


    // TODO: Maybe this check should be in the controller
    private boolean validateEmail(String email) {
        if (TextUtils.isEmpty(email)) {
            return false;
        } else {
            return android.util.Patterns.EMAIL_ADDRESS.matcher(email).matches();
        }
    }


    private void showInvalidEmailDialog() {
        new AlertDialog.Builder(getActivity()).setTitle("Invalid email")
                .setMessage("Please, enter valid email address!")
                .setPositiveButton("Ok", new DialogInterface.OnClickListener() {

                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.cancel();
                    }
                }).show();
    }


    private void closeKeyBoard() {
        View view = getActivity().getCurrentFocus();
        if (view != null) {
            InputMethodManager inputManager = (InputMethodManager) getActivity().getSystemService(
                    Context.INPUT_METHOD_SERVICE);
            inputManager.hideSoftInputFromWindow(view.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
        }
    }


    private void onCreateIdentity() {
        closeKeyBoard();
        String email = mEmailEditText.getText().toString().trim();
        mEmailEditText.setText(email);
        mEmailEditText.setSelection(email.length());
        if (validateEmail(email)) {
            getMPinController().handleMessage(MPinController.MESSAGE_CREATE_IDENTITY, email);
        } else {
            showInvalidEmailDialog();
        }
    }


    private void showIdentityExistsDialog() {
        new AlertDialog.Builder(getActivity()).setTitle("User already registered")
                .setMessage("Do you want to re-register the user?")
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {

                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        getMPinController().handleMessage(MPinController.MESSAGE_RESET_PIN);
                    }
                }).setNegativeButton("Cancel", null).show();
    }
}
