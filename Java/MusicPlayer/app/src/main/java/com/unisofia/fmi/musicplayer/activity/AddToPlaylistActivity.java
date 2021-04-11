package com.unisofia.fmi.musicplayer.activity;

import android.app.Activity;
import android.app.Fragment;
import android.content.Intent;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;

import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.adapter.PlaylistsAdapter;
import com.unisofia.fmi.musicplayer.players.Playlist;

import java.util.List;

public class AddToPlaylistActivity extends Activity implements AdapterView.OnItemClickListener {

    public static final String EXTRA_SONGS = "songs";
    private static final int REQUEST_CODE_NEW_PLAYLIST = 205;

    private PlaylistsAdapter mAdapter;
    private ListView mListView;
    private long[] mSongs;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_to_playlist);

        mAdapter = new PlaylistsAdapter(this);

        mListView = (ListView) findViewById(R.id.list_view_playlists);
        mListView.setAdapter(mAdapter);
        mListView.setOnItemClickListener(this);

        Intent intent = getIntent();
        mSongs = intent.getLongArrayExtra(EXTRA_SONGS);

        findViewById(R.id.button_playlist_cancel).setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                finish();
            }
        });
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        if (position == 0) {
            Intent createPlaylist = new Intent(this, CreatePlaylistActivity.class);
            createPlaylist.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            startActivityForResult(createPlaylist, REQUEST_CODE_NEW_PLAYLIST);
            return;
        }

        System.out.println(id);
        addToPlaylist(id, mAdapter.getItem(position));
    }

    @Override
    public void onResume() {
        super.onResume();
        setVisible(true);
    }

    @Override
    public void onPause() {
        setVisible(false);
        super.onPause();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode != RESULT_OK) {
            return;
        }

        switch (requestCode) {
            case REQUEST_CODE_NEW_PLAYLIST:
                addToPlaylist(data.getLongExtra(CreatePlaylistActivity.EXTRA_NEW_PLAYLIST_ID, -1),
                        data.getStringExtra(CreatePlaylistActivity.EXTRA_NEW_PLAYLIST_NAME));
                break;
        }
    }

    private void addToPlaylist(long id, String name) {
        int addedSongs = Playlist.addSongsToPlaylist(this, id, mSongs);
        Toast.makeText(this, addedSongs + getString(
                addedSongs == 1 ? R.string.was_added_to_playlist : R.string.were_added_to_playlist)
                + name, Toast.LENGTH_SHORT).show();
        finish();
    }
}
