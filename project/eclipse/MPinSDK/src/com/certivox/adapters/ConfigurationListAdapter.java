package com.certivox.adapters;

import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.RadioButton;
import android.widget.TextView;

import com.certivox.models.Config;
import com.example.mpinsdk.R;

public class ConfigurationListAdapter extends BaseAdapter {

	private Context mContext;
	private List<Config> mConfigsList;
	private long mSelectedConfigurationId;

	public ConfigurationListAdapter(Context context,
			List<Config> configurations, long selectedConfigurationId) {
		mContext = context;
		mConfigsList = configurations;
		mSelectedConfigurationId = selectedConfigurationId;
	}

	public void setSelectedfigurationId(long id) {
		mSelectedConfigurationId = id;
		notifyDataSetChanged();
	}

	@Override
	public void notifyDataSetChanged() {
		super.notifyDataSetChanged();
	}

	@Override
	public int getCount() {
		return mConfigsList.size();
	}

	@Override
	public Object getItem(int position) {
		return mConfigsList.get(position);
	}

	@Override
	public long getItemId(int position) {
		return mConfigsList.get(position).getId();
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		View view = convertView;
		ViewHolder holder;
		if (view == null) {
			view = LayoutInflater.from(mContext).inflate(R.layout.item_config,
					parent, false);
			holder = new ViewHolder();
			holder.title = (TextView) view.findViewById(R.id.item_config_title);
			holder.url = (TextView) view.findViewById(R.id.item_config_url);
			holder.button = (RadioButton) view.findViewById(R.id.toggle_button);
			view.setTag(holder);
		}

		holder = (ViewHolder) view.getTag();
		Config config = mConfigsList.get(position);
		holder.title.setText(config.getTitle());
		holder.url.setText(getConfigurationType(config));

		view.setId((int) config.getId());

		if (config.getId() == mSelectedConfigurationId) {
			holder.button.setChecked(true);
		} else {
			holder.button.setChecked(false);
		}

		return view;
	}

	private String getConfigurationType(Config config) {
		if (config.getRequestAccessNumber()) {
			return mContext.getResources().getString(R.string.request_an_title);
		} else if (config.getRequestOtp()) {
			return mContext.getResources()
					.getString(R.string.request_otp_title);
		}

		return mContext.getResources().getString(R.string.request_mobile_title);
	}

	private static class ViewHolder {
		TextView title;
		TextView url;
		RadioButton button;
	}
}
