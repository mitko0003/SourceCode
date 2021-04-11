package com.unisofia.fmi.musicplayer.service;

import android.app.Service;
import android.content.Intent;
import android.database.Cursor;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;

import com.unisofia.fmi.musicplayer.activity.PlaybackActivity;
import com.unisofia.fmi.musicplayer.media.Song;
import com.unisofia.fmi.musicplayer.players.MediaController;
import com.unisofia.fmi.musicplayer.players.Playlist;

import com.unisofia.fmi.musicplayer.constant.PlaybackMessage;

import java.io.IOException;
import java.util.ArrayDeque;
import java.util.Deque;

public class PlaybackService extends Service {

    private Deque<Song> mQueue = new ArrayDeque<>();
    private MediaController mMediaController;
    private Playlist mCurrentPlaylist;
    private Song mCurrentSong;
    private Song mNextSong;
    private Song mPlayNext;

    private PlaybackActivity mActivity;

    private final IBinder mBinder = new PlaybackBinder();

    public PlaybackService() {}

    @Override
    public void onCreate() {
        super.onCreate();
        mMediaController = new MediaController(this);
        mMediaController.setHandler(mPlaybackHandler);
        mCurrentPlaylist = Playlist.loadAllMusic(this);
        mCurrentSong = mCurrentPlaylist.getCurrent();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    public class PlaybackBinder extends Binder {

        public PlaybackService getService() {
            return PlaybackService.this;
        }

    }

    private Handler mPlaybackHandler = new Handler() {
        @Override
        public void handleMessage(Message message) {
            switch (message.what)
            {
                case PlaybackMessage.WENT_TO_NEXT:
                    if (mNextSong != null) {
                        mNextSong = null;
                    }
                    else if (!mQueue.isEmpty()) {
                        mQueue.pollFirst();
                    }
                    else {
                        mCurrentPlaylist.gotoNext();
                    }
                    break;

                case PlaybackMessage.WENT_TO_NEW:
                    mMediaController.dropNext();
                    Song nextSong = null;

                    if (mPlayNext != null)
                        nextSong = mPlayNext;
                    else if (!mQueue.isEmpty())
                        nextSong = mQueue.getFirst();
                    else if (mCurrentPlaylist.hasNext())
                        nextSong = mCurrentPlaylist.getNextSong();

                    if (nextSong != null)
                        mMediaController.setNextMedia(nextSong);

                    break;

                case PlaybackMessage.UPDATE_UI:
                    if (mActivity != null) {
                        mActivity.setPlayButtons();
                        mActivity.setSong(mCurrentSong);
                        sendEmptyMessageDelayed(PlaybackMessage.UPDATE_UI, 500);
                    }
                    break;

                default: break;
            }
        }
    };

    public void setActivity(PlaybackActivity activity) {
        mActivity = activity;
        mPlaybackHandler.sendEmptyMessage(PlaybackMessage.UPDATE_UI);
    }

    public Song getNextSong() {
        Song next = null;
        if (mNextSong != null) {
            next = mNextSong;
        }
        else if (!mQueue.isEmpty()) {
            next = mQueue.pollFirst();
        }
        else {
            next = mCurrentPlaylist.getNextSong();
        }
        return next;
    }

    public boolean isPlaying() {
        return mMediaController.isPlaying();
    }

    public Song getCurrentSong() {
        return mCurrentSong;
    }

    public MediaController getMediaController() {
        return mMediaController;
    }

    public void startPlayback() {
        mMediaController.start();
    }

    public void play() {
        mMediaController.play();
    }

    public void play(int position, Playlist playlist) {
        mCurrentPlaylist = playlist;
        mMediaController.play(mCurrentPlaylist.gotoSong(position));
        mCurrentSong = mCurrentPlaylist.getCurrent();
        mPlaybackHandler.sendEmptyMessage(PlaybackMessage.WENT_TO_NEW);
    }

//    private setNextSong() {
//
//    }

    public void next() {
        Song next = getNextSong();
        if (next != null){
            mCurrentSong = next;
            mMediaController.setNextMedia(mCurrentSong);
            mMediaController.next();
        }
    }

    public void previous() {
        if (mCurrentPlaylist.hasPrevious()) {
            mCurrentSong = mCurrentPlaylist.gotoPrevious();
            mMediaController.play(mCurrentSong);
            mMediaController.setNextMedia(getNextSong());
        }
    }

    public void playPause() {
        if (mMediaController.isPlaying()) {
            mMediaController.pause();
            return;
        }
        mMediaController.play();
    }

    public void playNext(Song song) {
        mPlayNext = song;
        mQueue.addFirst(song);
    }

    public void addToQueue(Song song) {
        mQueue.addLast(song);
    }
}
