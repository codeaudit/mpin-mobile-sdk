package com.certivox.adapters;

import android.content.Context;
import android.database.Cursor;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CursorAdapter;
import android.widget.RadioButton;
import android.widget.TextView;

import com.certivox.activities.PinpadConfigActivity;
import com.certivox.fragments.ConfigListFragment;
import com.certivox.mpinsdk.Config;
import com.example.mpinsdk.R;

public class ConfigAdapter extends CursorAdapter {

	private final Context mContext;
	private long mActiveId;

	public ConfigAdapter(Context context, Cursor c) {
		super(context, c, 0);
		mContext = context;
		Config active = PinpadConfigActivity.getActiveConfiguration(context);
		mActiveId = active == null ? -1 : active.getId();
	}

	public ConfigAdapter(Context context, Cursor c, int flags) {
		super(context, c, flags);
		mContext = context;
		Config active = PinpadConfigActivity.getActiveConfiguration(context);
		mActiveId = active == null ? -1 : active.getId();
	}

	@Override
	public void notifyDataSetChanged() {
		mActiveId = ConfigListFragment.sSelectedId;
		Log.i("DEBUG", "mActiveId = " + mActiveId);
		super.notifyDataSetChanged();
	}

	@Override
	public View newView(Context context, Cursor cursor, ViewGroup parent) {
		View view = LayoutInflater.from(context).inflate(R.layout.item_config,
				parent, false);
		ViewHolder holder = new ViewHolder();
		holder.title = (TextView) view.findViewById(R.id.item_config_title);
		holder.url = (TextView) view.findViewById(R.id.item_config_url);
		holder.button = (RadioButton) view.findViewById(R.id.toggle_button);
		view.setTag(holder);
		return view;
	}

	@Override
	public void bindView(View view, Context context, Cursor cursor) {
		ViewHolder holder = (ViewHolder) view.getTag();
		Config config = new Config();
		config.formCursor(cursor);
		holder.title.setText(config.getTitle());
		holder.url.setText(getConfigurationType(config));
		if (config.getId() == mActiveId) {
			holder.button.setChecked(true);
		} else {
			holder.button.setChecked(false);
		}

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
