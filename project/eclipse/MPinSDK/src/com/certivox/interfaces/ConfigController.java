package com.certivox.interfaces;

import com.certivox.mpinsdk.Config;

public interface ConfigController {

	void configurationSelected(long mSelectedId);

	void createNewConfiguration();

	void editConfiguration(Config activeConfig);

	void onDeleteConfiguration(Config activeConfig);

	void configurationSaved();

}
