package com.unisofia.fmi.musicplayer.activity;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.media.AudioManager;
import android.net.Uri;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;

import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.players.Playlist;

import java.util.List;

public class CreatePlaylistActivity extends Activity implements TextWatcher, View.OnClickListener {

    public static final String EXTRA_NEW_PLAYLIST_ID = "new_playlist_id";
    public static final String EXTRA_NEW_PLAYLIST_NAME = "new_playlist_name";

    private Button mButtonCreate;
    private EditText mEditTextName;
    private List<String> mPlaylistsNames;
    private String mPlaylistName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_playlist);

        setVolumeControlStream(AudioManager.STREAM_MUSIC);

        mEditTextName = (EditText) findViewById(R.id.edit_text_playlist_name);
        mEditTextName.addTextChangedListener(this);

        findViewById(R.id.button_cancel).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                finish();
            }
        });

        mButtonCreate = (Button) findViewById(R.id.button_create);
        mButtonCreate.setEnabled(false);
        mButtonCreate.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        long id = Playlist.getID(Playlist.createPlaylist(this, mPlaylistName));

        Intent result = new Intent();
        result.putExtra(EXTRA_NEW_PLAYLIST_ID, id);
        result.putExtra(EXTRA_NEW_PLAYLIST_NAME, mPlaylistName);

        setResult(RESULT_OK, result);
        finish();
    }

    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count) {
        mPlaylistName = mEditTextName.getText().toString().trim();
        if (mPlaylistsNames == null) {
            mPlaylistsNames = Playlist.getPlaylistsNames(this);
        }

        if (mPlaylistName.length() == 0 || mPlaylistsNames.contains(mPlaylistName)) {
            mButtonCreate.setEnabled(false);
        } else {
            mButtonCreate.setEnabled(true);
        }
    }

    @Override
    public void beforeTextChanged(CharSequence s, int start, int count, int after) {}

    @Override
    public void afterTextChanged(Editable s) {}

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        return true;
    }
}
