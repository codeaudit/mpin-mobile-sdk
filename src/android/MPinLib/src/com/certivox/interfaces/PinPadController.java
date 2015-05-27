package com.certivox.interfaces;

import com.certivox.models.User;

public interface PinPadController {

	User getCurrentUser();

	void onPinEntered(String pin);

	void disableContextToolbar();

	void setTooblarTitle(int enter_pin_title);

}
