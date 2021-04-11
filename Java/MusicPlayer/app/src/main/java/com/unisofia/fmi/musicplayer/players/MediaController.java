package com.unisofia.fmi.musicplayer.players;

import android.content.Context;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Handler;
import android.os.PowerManager;
import android.util.Log;

import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.constant.PlaybackMessage;
import com.unisofia.fmi.musicplayer.media.Song;
import com.unisofia.fmi.musicplayer.service.PlaybackService;

import java.io.IOException;

import javax.xml.transform.Source;

public class MediaController implements MediaPlayer.OnCompletionListener {

    private PlaybackService mPlaybackService;
    private MediaPlayer mCurrentMediaPlayer;
    private MediaPlayer mNextMediaPlayer;
    private Handler mHandler;

    public MediaController(PlaybackService playbackService) {
        mPlaybackService = playbackService;
    }

    @Override
    public void onCompletion(MediaPlayer mp) {
        if (mp != mCurrentMediaPlayer) {
            return;
        }
        if (mNextMediaPlayer == null) {
            mHandler.sendEmptyMessage(PlaybackMessage.SONGS_FINISHED);
            return;
        }
        mCurrentMediaPlayer.release();
        mCurrentMediaPlayer = mNextMediaPlayer;
        mCurrentMediaPlayer.start();
        mNextMediaPlayer = null;
        mHandler.sendEmptyMessage(PlaybackMessage.WENT_TO_NEXT);
    }

    /**
     * Todo:
     * Let the IOException go all the way to the UI and there decide
     * if the user should know about it and maybe skip the song on the way.
     **/
    public void setSource(MediaPlayer mp, Song source) {
        try {
            mp.reset();
            mp.setDataSource(source.getPath());
            mp.setAudioStreamType(AudioManager.STREAM_MUSIC);
            mp.prepare();
            mp.setOnCompletionListener(this);
        } catch (IOException e) {

        }
    }

    public void setNextMedia(Song song) {
        mCurrentMediaPlayer.setNextMediaPlayer(null);
        mNextMediaPlayer = new MediaPlayer();
        mNextMediaPlayer.setWakeMode(mPlaybackService, PowerManager.PARTIAL_WAKE_LOCK);
        mNextMediaPlayer.setAudioSessionId(getAudioSessionID());
        setSource(mNextMediaPlayer, song);
        mCurrentMediaPlayer.setNextMediaPlayer(mNextMediaPlayer);
    }

    public void dropNext() {
        if (mCurrentMediaPlayer != null)
            mCurrentMediaPlayer.setNextMediaPlayer(null);
        mNextMediaPlayer = null;
    }

    public int getAudioSessionID() {
        return mCurrentMediaPlayer.getAudioSessionId();
    }

    public void setHandler(Handler handler) {
        mHandler = handler;
    }

    public void start() {
        mCurrentMediaPlayer = new MediaPlayer();
        setSource(mCurrentMediaPlayer, mPlaybackService.getCurrentSong());
        setNextMedia(mPlaybackService.getNextSong());
        mCurrentMediaPlayer.start();
    }

    public boolean isPlaying() {
        return mCurrentMediaPlayer != null && mCurrentMediaPlayer.isPlaying();
    }

    public void play() {
        if (mCurrentMediaPlayer == null) {
            start();
            return;
        }
        mCurrentMediaPlayer.start();
    }

    public void play(Song song) {
        if (mCurrentMediaPlayer != null)
            mCurrentMediaPlayer.release();
        mCurrentMediaPlayer = new MediaPlayer();
        setSource(mCurrentMediaPlayer, song);
        mCurrentMediaPlayer.setNextMediaPlayer(mNextMediaPlayer);
        mCurrentMediaPlayer.start();
    }

    public void stop() {
        mCurrentMediaPlayer.stop();
    }

    public void pause() {
        mCurrentMediaPlayer.pause();
    }

    public void next() {
        onCompletion(mCurrentMediaPlayer);
    }

    public long position() {
        return mCurrentMediaPlayer.getCurrentPosition();
    }
}
