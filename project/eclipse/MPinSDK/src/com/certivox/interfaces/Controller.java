package com.certivox.interfaces;

import java.util.ArrayList;
import java.util.List;

import android.os.Handler;
import android.os.Message;

abstract public class Controller {

	private static final String TAG = Controller.class.getCanonicalName();
	private final List<Handler> outboxHandlers = new ArrayList<Handler>();

	abstract public boolean handleMessage(int what, Object data);

	abstract public boolean handleMessage(int what);

	public final void addOutboxHandler(Handler handler) {
		outboxHandlers.add(handler);
	}

	public final void removeOutboxHandler(Handler handler) {
		outboxHandlers.remove(handler);
	}

	protected final void notifyOutboxHandlers(int what, int arg1, int arg2,
			Object obj) {
		if (!outboxHandlers.isEmpty()) {
			for (Handler handler : outboxHandlers) {
				Message msg = Message.obtain(handler, what, arg1, arg2, obj);
				msg.sendToTarget();
			}
		}
	}
}
