package com.certivox.interfaces;

import com.certivox.models.OTP;
import com.certivox.models.User;

public interface MPinController {

	void addUsersFragment();

	void removeAddUsersFragment();

	void addUsersListFragment();

	void removeUsersListFragment();

	void addNewUserFragment();

	void removeNewUserFragment();

	void addConfirmEmailFragment();

	void removeConfirmEmailFragment();

	void addIdentityCreatedFragment();

	void removeIdentityCreatedFragment();

	void addPinPadFragment();

	void removePinPadFragment();

	void addAccessNumberFragment();

	void removeAccessNumberFragment();

	void addOTPFragment(OTP otp);

	void removeOTPFragment();

	void addSuccessfulLoginFragment();

	void removeSuccessfulLoginFragment();

	void addIdentityBlockedFragment();

	void removeIdentityBlockedFragment();

	void addAboutFragment();

	void removeAboutFragment();

	void setTooblarTitle(int resId);

	void userChosen();

	void userBlocked();

	void deleteCurrentUser();

	void resetPin();

	void deselectAllUsers();

	void createNewUser(final String email);

	void emailConfirmed();

	void resendEmail();

	void signIn();

	void logout();

	void enableContextToolbar();

	void disableContextToolbar();

	User getCurrentUser();

	void onPinEntered(String pin);

	void onAccessNumberEntered(String accessNumber);

	void setChosenConfiguration(String configTitle);

	void reRegisterUser(User user);

	void onOTPExpired();
}
