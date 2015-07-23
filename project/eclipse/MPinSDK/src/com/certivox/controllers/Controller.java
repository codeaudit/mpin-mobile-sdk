package com.certivox.controllers;

import java.util.ArrayList;
import java.util.List;

import android.os.Handler;
import android.os.Message;

abstract public class Controller {

	private static final String TAG = "Controllerr";
	private final List<Handler> outboxHandlers = new ArrayList<Handler>();
	private Object lock = new Object();

	abstract public boolean handleMessage(int what, Object data);

	abstract public boolean handleMessage(int what);

	public final void addOutboxHandler(Handler handler) {
		synchronized (lock) {
			outboxHandlers.add(handler);
		}
	}

	public final void removeOutboxHandler(Handler handler) {
		synchronized (lock) {
			outboxHandlers.remove(handler);
		}
	}

	protected final void notifyOutboxHandlers(int what, int arg1, int arg2,
			Object obj) {
		synchronized (lock) {
			if (!outboxHandlers.isEmpty()) {
				for (Handler handler : outboxHandlers) {
					Message msg = Message
							.obtain(handler, what, arg1, arg2, obj);
					msg.sendToTarget();
				}
			}
		}
	}
}