package com.certivox.adapters;

import java.lang.ref.WeakReference;

import android.content.Context;
import android.database.Cursor;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CursorAdapter;
import android.widget.RadioButton;
import android.widget.TextView;

import com.certivox.activities.PinpadConfigActivity;
import com.certivox.mpinsdk.Config;
import com.example.mpinsdk.R;

public class ConfigAdapter extends CursorAdapter {

	private final WeakReference<Context> m_context;
	private long m_activeId;

	public ConfigAdapter(Context context, Cursor c) {
		super(context, c, 0);
		m_context = new WeakReference<Context>(context);
		Config active = PinpadConfigActivity.getActiveConfiguration(context);
		m_activeId = active == null ? -1 : active.getId();
	}

	public ConfigAdapter(Context context, Cursor c, int flags) {
		super(context, c, flags);
		m_context = new WeakReference<Context>(context);
		Config active = PinpadConfigActivity.getActiveConfiguration(context);
		m_activeId = active == null ? -1 : active.getId();
	}

	@Override
	public void notifyDataSetChanged() {
		Config active = PinpadConfigActivity.getActiveConfiguration(m_context.get());
		m_activeId = active == null ? -1 : active.getId();
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
		holder.url.setText(config.getBackendUrl());
		if (config.getId() == m_activeId) {
			holder.button.setChecked(true);
		} else {
			holder.button.setChecked(false);
		}

	}

	private static class ViewHolder {
		TextView title;
		TextView url;
		RadioButton button;
	}

}
