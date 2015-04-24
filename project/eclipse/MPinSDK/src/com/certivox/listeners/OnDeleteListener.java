package com.certivox.listeners;

import com.certivox.models.User;

public interface OnDeleteListener {
	void onDelete(User user, int position);
}
