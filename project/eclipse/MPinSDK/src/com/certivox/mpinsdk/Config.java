package com.certivox.mpinsdk;

public final class Config {

	private long mId;
	private String mTitle;
	private String mBackendUrl;
	private String mRTS;
	private boolean mRequestOtp;
	private boolean mRequestAccessNumber;

	public Config() {
		mId = -1;
	}

	public Config(String title, String backendUrl, boolean requestOtp,
			boolean requestAccessNumber) {
		mId = -1;
		mTitle = title;
		mBackendUrl = backendUrl;
		mRequestOtp = requestOtp;
		mRequestAccessNumber = requestAccessNumber;
		mRTS = "";
	}

	public Config(String title, String backendUrl, String rts,
			boolean requestOtp, boolean requestAccessNumber) {
		this(title, backendUrl, requestOtp, requestAccessNumber);
		mRTS = rts;
	}

	public long getId() {
		return mId;
	}

	public void setId(long id) {
		mId = id;
	}

	public String getTitle() {
		return mTitle;
	}

	public void setTitle(String title) {
		mTitle = title;
	}

	public String getBackendUrl() {
		return mBackendUrl;
	}

	public void setBackendUrl(String backendUrl) {
		mBackendUrl = backendUrl;
	}

	public String getRTS() {
		return mRTS;
	}

	public void setRTS(String rts) {
		mRTS = rts;
	}

	public boolean getRequestOtp() {
		return mRequestOtp;
	}

	public void setRequestOtp(boolean requestOtp) {
		mRequestOtp = requestOtp;
	}

	public boolean getRequestAccessNumber() {
		return mRequestAccessNumber;
	}

	public void setRequestAccessNumber(boolean requestAccessNumber) {
		mRequestAccessNumber = requestAccessNumber;
	}
}
