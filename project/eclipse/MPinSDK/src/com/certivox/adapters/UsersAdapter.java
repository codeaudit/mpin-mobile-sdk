package com.certivox.adapters;

import java.util.ArrayList;
import java.util.Collection;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageButton;
import android.widget.TextView;

import com.certivox.models.User;
import com.certivox.mpinsdk.Mpin;
import com.example.mpinsdk.R;

public class UsersAdapter extends BaseAdapter {

	private final Context mContext;

	private ArrayList<User> mUsersList;

	public UsersAdapter(Context context) {
		mContext = context;
		mUsersList = new ArrayList<User>();
	}

	public void setData(Collection<User> data) {
		synchronized (mUsersList) {
			mUsersList.clear();
			mUsersList.addAll(data);
			notifyDataSetChanged();
		}
	}

	public void refresh(Mpin sdk) {
		synchronized (mUsersList) {
			mUsersList.clear();
			sdk.ListUsers(mUsersList);
		}
		notifyDataSetChanged();
	}

	@Override
	public int getCount() {
		synchronized (mUsersList) {
			return mUsersList.size();
		}
	}

	@Override
	public User getItem(int position) {
		synchronized (mUsersList) {
			return mUsersList.get(position);
		}
	}

	@Override
	public long getItemId(int position) {
		return getItem(position).getId().hashCode();
	}

	@Override
	public boolean hasStableIds() {
		return true;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		if (convertView == null) {
			if (mContext == null) {
				return null;
			}
			convertView = LayoutInflater.from(mContext).inflate(
					R.layout.users_list_item, parent, false);
			final Holder holder = new Holder();
			holder.setContainer(convertView
					.findViewById(R.id.fragment_users_list_item_container));
			holder.setTextView((TextView) convertView
					.findViewById(R.id.fragment_users_list_item_name));
			convertView.setTag(holder);
		}
		Holder holder = (Holder) convertView.getTag();
		User user = getItem(position);
		holder.setPosition(position);
		holder.setUser(user);
		holder.getTextView().setText(user.getId());
		if (user.isUserSelected()) {
			holder.getTextView().setBackgroundColor(
					mContext.getResources().getColor(
							R.color.selected_item_background));
			holder.getTextView().setCompoundDrawablesWithIntrinsicBounds(
					mContext.getResources().getDrawable(
							R.drawable.ic_avatar_selected), null, null,
					null);
		} else {
			holder.getTextView().setBackgroundColor(
					mContext.getResources().getColor(R.color.white));
			holder.getTextView().setCompoundDrawablesWithIntrinsicBounds(
					mContext.getResources().getDrawable(
							R.drawable.ic_avatar), null, null, null);
		}
		return convertView;
	}

	public void deselectAllUsers() {
		for (User user : mUsersList) {
			user.setUserSelected(false);
		}
		notifyDataSetChanged();
	}

	private class Holder {
		private View container;
		private ImageButton delete;
		private TextView textView;
		private User user;
		private int position;

		public Holder() {
			position = -1;
		}

		public View getContainer() {
			return container;
		}

		public void setContainer(View container) {
			this.container = container;
		}

		public ImageButton getDelete() {
			return delete;
		}

		public void setDelete(ImageButton delete) {
			this.delete = delete;
		}

		public TextView getTextView() {
			return textView;
		}

		public void setTextView(TextView textView) {
			this.textView = textView;
		}

		public User getUser() {
			return user;
		}

		public void setUser(User user) {
			this.user = user;
		}

		public int getPosition() {
			return position;
		}

		public void setPosition(int position) {
			this.position = position;
		}
	}

}
