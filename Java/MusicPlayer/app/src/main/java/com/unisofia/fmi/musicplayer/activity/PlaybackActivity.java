package com.unisofia.fmi.musicplayer.activity;

import android.app.Fragment;
import android.app.FragmentManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.res.Configuration;
import android.media.AudioManager;
import android.net.Uri;
import android.os.IBinder;
import android.os.Bundle;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarActivity;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.widget.Toolbar;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.SearchView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.sothree.slidinguppanel.SlidingUpPanelLayout;
import com.sothree.slidinguppanel.SlidingUpPanelLayout.PanelState;
import com.squareup.picasso.Picasso;
import com.unisofia.fmi.musicplayer.adapter.DrawerAdapter;
import com.unisofia.fmi.musicplayer.adapter.PlaylistsGridAdapter;
import com.unisofia.fmi.musicplayer.fragment.AllSongsFragment;
import com.unisofia.fmi.musicplayer.fragment.PlaylistFragment;
import com.unisofia.fmi.musicplayer.fragment.PlaylistsGridFragment;
import com.unisofia.fmi.musicplayer.media.Song;
import com.unisofia.fmi.musicplayer.players.MediaController;
import com.unisofia.fmi.musicplayer.players.Playlist;
import com.unisofia.fmi.musicplayer.service.PlaybackService;
import com.unisofia.fmi.musicplayer.service.PlaybackService.PlaybackBinder;
import com.unisofia.fmi.musicplayer.R;


public class PlaybackActivity extends ActionBarActivity implements
        View.OnClickListener, PlaylistsGridAdapter.OnPlaylistSelectedListener,
        AdapterView.OnItemClickListener, AllSongsFragment.OnSongSelectedListener {

    private static final int REQUEST_CODE_NEW_PLAYLIST = 8532;

    private PlaybackService mPlaybackService;
    private boolean mIsBound = false;
    private AudioManager mAudioManager;
    private SlidingUpPanelLayout mSlidingLayout;
    private DrawerLayout mDrawerLayout;
    private ActionBarDrawerToggle mDrawerToggle;
    private DrawerAdapter mDrawerAdapter;
    private FragmentManager mFragmentManager;

    private ImageButton mButtonPlay;
    private ImageButton mButtonPanelPlay;
    private ImageButton mButtonNext;
    private ImageButton mButtonPrevious;
    private ImageButton mButtonRepeat;
    private ImageButton mButtonShuffle;

    private TextView mTextViewSongTitle;
    private TextView mTextViewSongArtist;

    private ImageView mImageViewThumbnail;
    private ImageView mImageViewLargeThumbnail;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_drawer_layout);

        setVolumeControlStream(AudioManager.STREAM_MUSIC);
        mAudioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);

        mButtonPlay = (ImageButton) findViewById(R.id.image_button_play);
        mButtonPanelPlay = (ImageButton) findViewById(R.id.image_button_panel_control);
        mButtonNext = (ImageButton) findViewById(R.id.image_button_next);
        mButtonPrevious = (ImageButton) findViewById(R.id.image_button_prev);
        mButtonRepeat = (ImageButton) findViewById(R.id.image_button_repeat);
        mButtonShuffle = (ImageButton) findViewById(R.id.image_button_shuffle);

        mButtonPlay.setOnClickListener(this);
        mButtonPanelPlay.setOnClickListener(this);
        mButtonNext.setOnClickListener(this);
        mButtonPrevious.setOnClickListener(this);
        mButtonRepeat.setOnClickListener(this);
        mButtonShuffle.setOnClickListener(this);

        mTextViewSongTitle = (TextView) findViewById(R.id.text_view_song_title);
        mTextViewSongArtist = (TextView) findViewById(R.id.text_view_song_artist);

        mImageViewThumbnail = (ImageView) findViewById(R.id.image_view_song_thumbnail);
        mImageViewLargeThumbnail = (ImageView) findViewById(R.id.image_view);

        mSlidingLayout = (SlidingUpPanelLayout) findViewById(R.id.sliding_layout);
        setUpDrawer();

        mFragmentManager = getFragmentManager();
        AllSongsFragment fragmentSongs = new AllSongsFragment();
        mFragmentManager
                .beginTransaction()
                .replace(R.id.frame_layout_content, fragmentSongs).commit();

    }

    public void setPlayButtons() {
        if (!mIsBound) {
            Picasso.with(this).load(android.R.drawable.ic_media_play).fit().into(mButtonPanelPlay);
            Picasso.with(this).load(android.R.drawable.ic_media_play).fit().into(mButtonPlay);
        } else if (mPlaybackService.isPlaying()) {
            Picasso.with(this).load(android.R.drawable.ic_media_pause).fit().into(mButtonPanelPlay);
            Picasso.with(this).load(android.R.drawable.ic_media_pause).fit().into(mButtonPlay);
        } else {
            Picasso.with(this).load(android.R.drawable.ic_media_play).fit().into(mButtonPanelPlay);
            Picasso.with(this).load(android.R.drawable.ic_media_play).fit().into(mButtonPlay);
        }
    }

    public void setSong(Song song) {
        Picasso.with(this).load(R.drawable.default_song_thumbnail).fit().into(mImageViewLargeThumbnail);
        Picasso.with(this).load(R.drawable.default_song_thumbnail).fit().into(mImageViewThumbnail);
        if (song != null) {
            mTextViewSongArtist.setText(song.getArtist());
            mTextViewSongTitle.setText(song.getTitle());
            Uri uri = song.getThumbnail(this.getContentResolver());
            if (uri != null) {
                Picasso.with(this).load(uri).fit().into(mImageViewLargeThumbnail);
                Picasso.with(this).load(uri).fit().into(mImageViewThumbnail);
            }
        } else {
            mTextViewSongArtist.setText(getString(R.string.not_available));
            mTextViewSongTitle.setText(getString(R.string.not_available));
        }
    }

    private void setUpDrawer() {
        mDrawerLayout = (DrawerLayout) findViewById(R.id.drawer_layout);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        mDrawerToggle = new ActionBarDrawerToggle(this, mDrawerLayout,
                toolbar, R.string.app_name, R.string.app_name);
        mDrawerLayout.setDrawerListener(mDrawerToggle);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setHomeButtonEnabled(true);

        mDrawerAdapter = new DrawerAdapter(this);
        ListView listView = (ListView) findViewById(R.id.list_view_drawer);
        listView.setAdapter(mDrawerAdapter);
        listView.setOnItemClickListener(this);
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        mDrawerLayout.closeDrawers();
        mFragmentManager.popBackStack(null, FragmentManager.POP_BACK_STACK_INCLUSIVE);
        Fragment fragment = null;
        switch (mDrawerAdapter.getItem(position)) {
            case ALL_SONGS:
                fragment = new AllSongsFragment();
                break;
            case PLAYLISTS:
                fragment = new PlaylistsGridFragment();
                break;
        }
        mFragmentManager
                .beginTransaction()
                .setCustomAnimations(
                        R.anim.slide_in_right,
                        R.anim.slide_out_left
                )
                .replace(R.id.frame_layout_content, fragment).commit();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.image_button_play:
                if (mIsBound) {
                    mPlaybackService.playPause();
                }
                break;
            case R.id.image_button_panel_control:
                if (mIsBound) {
                    mPlaybackService.playPause();
                }
                break;
            case R.id.image_button_next:
                if (mIsBound)
                    mPlaybackService.next();
                break;
            case R.id.image_button_prev:
                if (mIsBound)
                    mPlaybackService.previous();
                break;
        }
    }

    @Override
    public void onPlaylistSelected(long id) {
        PlaylistFragment playlist = PlaylistFragment.newInstance(id);

        mFragmentManager
                .beginTransaction()
                .setCustomAnimations(
                        R.anim.slide_in_right,
                        R.anim.slide_out_left,
                        R.anim.slide_in_left,
                        R.anim.slide_out_right
                )
                .replace(R.id.frame_layout_content, playlist).addToBackStack("PlaylistGrid").commit();
    }

    @Override
    public void onBackPressed() {
        if (mSlidingLayout != null &&
                (mSlidingLayout.getPanelState() == PanelState.EXPANDED ||
                        mSlidingLayout.getPanelState() == PanelState.ANCHORED)) {
            mSlidingLayout.setPanelState(PanelState.COLLAPSED);
        }
        else if (mFragmentManager.getBackStackEntryCount() > 0) {
            mFragmentManager.popBackStack();
        } else {
            super.onBackPressed();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode != RESULT_OK) {
            return;
        }

        switch (requestCode) {
            case REQUEST_CODE_NEW_PLAYLIST:
                Toast.makeText(getApplicationContext(),
                        data.getStringExtra(CreatePlaylistActivity.EXTRA_NEW_PLAYLIST_NAME),
                        Toast.LENGTH_SHORT).show();
                break;
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        System.out.println("start");

        Intent intent = new Intent(this, PlaybackService.class);
        startService(intent);
        bindService(intent, mServiceConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onStop() {
        System.out.println("stop");
        super.onStart();
    }

    @Override
    public void onDestroy() {
        System.out.println("destroy");
        if (mIsBound) {
            unbindService(mServiceConnection);
        }
        super.onDestroy();
    }

    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        mDrawerToggle.syncState();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        mDrawerToggle.onConfigurationChanged(newConfig);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);

        SearchView searchView =
                (SearchView) menu.findItem(R.id.search_view).getActionView();
        searchView.setQueryHint(getResources().getString(R.string.action_search));

        return true;
    }

    public void recreateFragment(Fragment fragment) {
        mFragmentManager
                .beginTransaction()
                .replace(R.id.frame_layout_content, fragment).commit();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        return mDrawerToggle.onOptionsItemSelected(item) || super.onOptionsItemSelected(item);
    }

    @Override
    public void onSongSelected(int position, Playlist playlist) {
        if (mIsBound) {
            mPlaybackService.play(position, playlist);
        }
    }

    @Override
    public void onSongPlayNext(Song song) {
        mPlaybackService.addToQueue(song);
    }

    @Override
    public void onSongAddToQueue(Song song) {
        mPlaybackService.addToQueue(song);
    }

    private ServiceConnection mServiceConnection = new ServiceConnection(){

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            PlaybackBinder binder = (PlaybackBinder) service;
            mPlaybackService = binder.getService();
            mIsBound = true;
            mPlaybackService.setActivity(PlaybackActivity.this);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mPlaybackService = null;
            mIsBound = false;
        }

    };

    @Override
    public boolean onKeyLongPress(int keyCode, KeyEvent event) {
        switch(keyCode){
            case KeyEvent.KEYCODE_VOLUME_UP:
                if (mIsBound && mPlaybackService.isPlaying())
                    mPlaybackService.next();
                return true;
            case KeyEvent.KEYCODE_VOLUME_DOWN:
                if (mIsBound && mPlaybackService.isPlaying())
                    mPlaybackService.previous();
                return true;
        }
        return super.onKeyLongPress(keyCode, event);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        switch(keyCode){
            case KeyEvent.KEYCODE_VOLUME_UP:
                event.startTracking();
                return true;
            case KeyEvent.KEYCODE_VOLUME_DOWN:
                event.startTracking();
                return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if(!event.isCanceled()){
            switch(keyCode){
                case KeyEvent.KEYCODE_VOLUME_UP:
                    mAudioManager.adjustStreamVolume(AudioManager.STREAM_MUSIC,
                            AudioManager.ADJUST_RAISE, AudioManager.FLAG_SHOW_UI);
                    return true;
                case KeyEvent.KEYCODE_VOLUME_DOWN:
                    mAudioManager.adjustStreamVolume(AudioManager.STREAM_MUSIC,
                            AudioManager.ADJUST_LOWER, AudioManager.FLAG_SHOW_UI);
                    return true;
            }
        }
        return super.onKeyUp(keyCode, event);
    }
}
