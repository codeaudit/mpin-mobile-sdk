package com.certivox.interfaces;


public interface ConfigController {

	void configurationSelected(long selectedId);

	void createNewConfiguration();

	void editConfiguration(long selectedId);

	void onDeleteConfiguration(long selectedId);

	void configurationSaved();

	public void showLoader();

	public void hideLoader();
}
