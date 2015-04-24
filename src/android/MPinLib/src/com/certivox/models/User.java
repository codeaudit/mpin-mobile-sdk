package com.certivox.models;

import java.io.Closeable;

public class User implements Closeable {

	private boolean isUserSelected;

	private long mPtr;

	private native void nDestruct(long ptr);

	private native String nGetId(long ptr);

	private native int nGetState(long ptr);

	public enum State {
		INVALID, STARTED_REGISTRATION, ACTIVATED, REGISTERED, BLOCKED
	};

	private User(long ptr) {
		mPtr = ptr;
	}

	public String getId() {
		return nGetId(mPtr);
	}

	public State getState() {
		switch (nGetState(mPtr)) {
		case 1:
			return State.STARTED_REGISTRATION;
		case 2:
			return State.ACTIVATED;
		case 3:
			return State.REGISTERED;
		case 4:
			return State.BLOCKED;
		default:
			return State.INVALID;
		}
	}

	@Override
	public void close() {
		synchronized (this) {
			nDestruct(mPtr);
			mPtr = 0;
		}
	}

	@Override
	protected void finalize() throws Throwable {
		close();
		super.finalize();
	}

	@Override
	public String toString() {
		return getId();
	}

	public boolean isUserSelected() {
		return isUserSelected;
	}

	public void setUserSelected(boolean isUserSelected) {
		this.isUserSelected = isUserSelected;
	}

}
