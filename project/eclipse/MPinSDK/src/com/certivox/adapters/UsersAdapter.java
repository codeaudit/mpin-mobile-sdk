package com.certivox.adapters;

import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.certivox.models.User;
import com.example.mpinsdk.R;

public class UsersAdapter extends BaseAdapter {

	private Context mContext;
	private List<User> mUsersList;

	public UsersAdapter(Context context, List<User> usersList) {
		mContext = context;
		mUsersList = usersList;
	}

	public void updateUsersList(List<User> usersList) {
		mUsersList.clear();
		mUsersList.addAll(usersList);
		notifyDataSetChanged();
	}

	@Override
	public int getCount() {
		return mUsersList.size();
	}

	@Override
	public User getItem(int position) {
		return mUsersList.get(position);
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
		ViewHolder holder;
		if (convertView == null) {
			convertView = LayoutInflater.from(mContext).inflate(
					R.layout.users_list_item, parent, false);
			holder = new ViewHolder();
			holder.textView = (TextView) convertView
					.findViewById(R.id.fragment_users_list_item_name);
			convertView.setTag(holder);
		}

		holder = (ViewHolder) convertView.getTag();
		User user = getItem(position);
		holder.textView.setText(user.getId());
		if (user.isUserSelected()) {
			holder.textView.setBackgroundColor(mContext.getResources()
					.getColor(R.color.selected_item_background));
			holder.textView.setCompoundDrawablesWithIntrinsicBounds(mContext
					.getResources().getDrawable(R.drawable.ic_avatar_selected),
					null, null, null);
		} else {
			holder.textView.setBackgroundColor(mContext.getResources()
					.getColor(R.color.white));
			holder.textView.setCompoundDrawablesWithIntrinsicBounds(mContext
					.getResources().getDrawable(R.drawable.ic_avatar), null,
					null, null);
		}

		return convertView;
	}

	public void deselectAllUsers() {
		for (User user : mUsersList) {
			user.setUserSelected(false);
		}
		notifyDataSetChanged();
	}

	private class ViewHolder {
		TextView textView;
	}
}
