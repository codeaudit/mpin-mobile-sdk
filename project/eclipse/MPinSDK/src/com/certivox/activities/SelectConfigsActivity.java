package com.certivox.activities;


import java.util.ArrayList;
import java.util.List;

import com.certivox.adapters.ConfigurationListAdapter;
import com.certivox.constants.IntentConstants;
import com.certivox.models.Config;
import com.certivox.mpinsdk.R;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;


public class SelectConfigsActivity extends ActionBarActivity {

    private ListView                 mListView;
    private ConfigurationListAdapter mConfigsAdapter;
    private Toolbar                  mToolbar;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_select_configs);
        resolveStartIntent();
        initViews();
    }


    @Override
    protected void onResume() {
        super.onResume();
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.select_configs, menu);
        return true;
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.select_configs_action_select) {
            returnResult();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }


    private void resolveStartIntent() {
        Intent startIntent = getIntent();

        if (startIntent.getAction().equals(Intent.ACTION_PICK)) {
            if (startIntent.hasExtra(IntentConstants.EXTRA_CONFIGS_LIST)) {
                mConfigsAdapter = new ConfigurationListAdapter(getBaseContext(),
                        (List<Config>) startIntent.getSerializableExtra(IntentConstants.EXTRA_CONFIGS_LIST),
                        ConfigurationListAdapter.SELECT_ALL);
            } else {
                Toast.makeText(this, getString(R.string.no_configurations_loaded_message), Toast.LENGTH_LONG).show();
                setResult(RESULT_CANCELED);
                finish();
            }
        } else {
            setResult(RESULT_CANCELED);
            finish();
        }
    }


    private void initViews() {
        mToolbar = (Toolbar) findViewById(R.id.select_configs_toolbar);
        mToolbar.setTitle(R.string.import_configurations_title);
        setSupportActionBar(mToolbar);

        mListView = (ListView) findViewById(R.id.select_configs_list_view);
        mConfigsAdapter = new ConfigurationListAdapter(this, getMockConfigs(), ConfigurationListAdapter.SELECT_ALL);
        mListView.setAdapter(mConfigsAdapter);
        mListView.setOnItemClickListener(new SelectionListener());
    }


    private void returnResult() {
        Intent resultIntent = new Intent();
        resultIntent.putExtra(IntentConstants.EXTRA_SELECTED_CONFIGS_LIST,
                new ArrayList<Config>(mConfigsAdapter.getSelected()));
        setResult(RESULT_OK, resultIntent);
        finish();
    }


    private List<Config> getMockConfigs() {
        List<Config> configs = new ArrayList<Config>();
        Config config = new Config("Awesome config", "https://awesome.config.com", true, false, false);
        config.setId(123);
        configs.add(config);
        config = new Config("Best config", "https://best.config.com", true, false, false);
        config.setId(124);
        configs.add(config);
        config = new Config("Radical config", "https://rad.config.com", true, true, false);
        config.setId(125);
        configs.add(config);
        return configs;
    }

    private class SelectionListener implements AdapterView.OnItemClickListener {

        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            if (mConfigsAdapter.isSelected(position)) {
                mConfigsAdapter.deselect(position);
            } else {
                mConfigsAdapter.select(position);
            }
        }

    }
}
