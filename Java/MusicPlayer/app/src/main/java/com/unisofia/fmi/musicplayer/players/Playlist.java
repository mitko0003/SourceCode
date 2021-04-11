package com.unisofia.fmi.musicplayer.players;

import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.widget.Toast;

import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.activity.AddToPlaylistActivity;
import com.unisofia.fmi.musicplayer.media.Song;

import java.net.URI;
import java.util.ArrayList;
import java.util.List;
import java.util.jar.Attributes;

public class Playlist {

    private static final Uri STORAGE = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
    private static final String EXTERNAL = "external";
    private static final String COUNT = "count(*)";

    private List<Song> mSongs = new ArrayList<>();
    private Context mContext;
    private ContentResolver mResolver;
    private int mCurrentSong = 0;
    private long mID;
    private String mName;

    private final static String[] SONG_INFO = new String[] {
            MediaStore.Audio.Playlists.Members.AUDIO_ID,
            MediaStore.Audio.Media._ID,
            MediaStore.Audio.Media.DATA,
            MediaStore.Audio.Media.TITLE,
            MediaStore.Audio.Media.ARTIST,
            MediaStore.Audio.Media.ALBUM,
            MediaStore.Audio.Media.ALBUM_ID
    };

    private Playlist(Context context) {
        mContext = context;
        mResolver = mContext.getContentResolver();
    }

    public Playlist(Context context, String name, long id) {
        mContext = context;
        mResolver = mContext.getContentResolver();
        mName = name;
        mID = id;

        Uri uri = MediaStore.Audio.Playlists.Members.getContentUri(EXTERNAL, id);
        loadSongs(uri);
    }

    public Playlist(Context context, long id) {
        mContext = context;
        mResolver = mContext.getContentResolver();
        mCurrentSong = 0;
        mID = id;

        Uri uri = MediaStore.Audio.Playlists.Members.getContentUri(EXTERNAL, id);
        loadSongs(uri);
    }

    private void loadName() {
        String data[] = {MediaStore.Audio.Playlists.NAME};
        final String filter = MediaStore.Audio.Playlists._ID + "=" + mID;
        Cursor cursor = mContext.getContentResolver().query(
                MediaStore.Audio.Playlists.EXTERNAL_CONTENT_URI,
                data, filter, null, null);
        cursor.moveToFirst();
        int column = cursor.getColumnIndex(data[0]);

        try {
            mName = cursor.getString(column);
        } finally {
            cursor.close();
        }
    }

    public boolean hasNext() {
        return mCurrentSong < mSongs.size();
    }

    public boolean hasPrevious() {
        return mCurrentSong > 0;
    }

    public Song gotoPrevious() {
        if (mCurrentSong <= 0)
            return null;
        return mSongs.get(mCurrentSong);
    }

    public Song getCurrent() {
        return mSongs.get(mCurrentSong);
    }

    public Song gotoSong(int position) {
        mCurrentSong = position;
        return mSongs.get(position);
    }

    public String getName() {
        return mName;
    }

    public Song getNextSong() {
        if (mSongs.size() <= mCurrentSong + 1) {
            return null;
        }
        return mSongs.get(mCurrentSong + 1);
    }

    public Song gotoNext() {
        if (mSongs.size() <= mCurrentSong + 1)
            return null;
        return mSongs.get(++mCurrentSong);
    }

    public int size() {
        return mSongs.size();
    }

    private void loadSongs(Uri uri) {
        Cursor cursor = mResolver.query(uri, SONG_INFO, null, null, null);
        mSongs = new ArrayList<>(cursor.getCount());

        try {
            while (cursor.moveToNext()) {
                mSongs.add(new Song(cursor));
            }
        } finally {
            cursor.close();
        }
    }

    public static Playlist loadAllMusic(Context context) {
        String[] data = new String[] {
                MediaStore.Audio.Media._ID,
                MediaStore.Audio.Media.DATA,
                MediaStore.Audio.Media.TITLE,
                MediaStore.Audio.Media.ARTIST,
                MediaStore.Audio.Media.ALBUM,
                MediaStore.Audio.Media.ALBUM_ID
        };

        Playlist result = new Playlist(context);
        result.mName = context.getString(R.string.all_songs);
        result.mID = -1;

        final String filter = MediaStore.Audio.Media.IS_MUSIC + "=1"; // gets only music
        final Cursor cursor = result.mResolver.query(STORAGE, data, filter, null, null);

        try {
            while (cursor.moveToNext()) {
                result.mSongs.add(new Song(cursor));
            }
            return result;
        } finally {
            cursor.close();
        }
    }

    public static List<String> getPlaylistsNames(Context context) {
        String data[] = {MediaStore.Audio.Playlists.NAME};
        Cursor cursor = context.getContentResolver().query(
                MediaStore.Audio.Playlists.EXTERNAL_CONTENT_URI,
                data , null, null, null);
        int column = cursor.getColumnIndex(data[0]);

        List<String> result = new ArrayList<>(cursor.getCount());
        try {
            while(cursor.moveToNext()) {
                result.add(cursor.getString(column));
            }
            return result;
        } finally {
            cursor.close();
        }
    }

    /**
     * Letting the classes closer to UI validate creating playlist is possible to make
     * sure that the user will be worn if not.
     */
    public static Uri createPlaylist(Context context, String name) {
        ContentValues valuesToInsert = new ContentValues();
        valuesToInsert.put(MediaStore.Audio.Playlists.NAME, name);
        valuesToInsert.put(MediaStore.Audio.Playlists.DATE_ADDED, System.currentTimeMillis());
        valuesToInsert.put(MediaStore.Audio.Playlists.DATE_MODIFIED, System.currentTimeMillis());

         return context.getContentResolver().insert(
                MediaStore.Audio.Playlists.EXTERNAL_CONTENT_URI, valuesToInsert);
    }

    public Song getSong(int index) {
        return mSongs.get(index);
    }

    public static List<Playlist> getPlaylists(Context context) {
        String data[] = {
                MediaStore.Audio.Playlists.NAME,
                MediaStore.Audio.Playlists._ID
        };

        Cursor cursor = context.getContentResolver().query(
                MediaStore.Audio.Playlists.EXTERNAL_CONTENT_URI,
                data , null, null, null);
        int nameColumn = cursor.getColumnIndex(data[0]);
        int idColumn = cursor.getColumnIndex(data[1]);

        List<Playlist> result = new ArrayList<>(cursor.getCount());
        try {
            while(cursor.moveToNext()) {
                result.add(new Playlist(context,
                        cursor.getString(nameColumn), cursor.getLong(idColumn)));
            }
            return result;
        } finally {
            cursor.close();
        }
    }

    public long getID() {
        return mID;
    }

    public static long getID(Context context, String name) {
        String data[] = { MediaStore.Audio.Playlists._ID };
        String filter = MediaStore.Audio.Playlists.NAME + "='" + name + "'";
        Cursor cursor = context.getContentResolver().query(
                MediaStore.Audio.Playlists.EXTERNAL_CONTENT_URI,
                data, filter, null, null);
        cursor.moveToFirst();
        int column = cursor.getColumnIndex(data[0]);

        try {
            return cursor.getLong(column);
        } finally {
            cursor.close();
        }
    }

    public static long getID(Uri uri) {
        return Long.parseLong(uri.getLastPathSegment());
    }

    public static int addSongsToPlaylist(Context context, long id, long[] songs) {
        Uri uri = MediaStore.Audio.Playlists.Members.getContentUri(EXTERNAL, id);

        int count = getPlaylistSize(context, uri);
        ContentValues[] values = new ContentValues[songs.length];

        for (int i = 0; i < songs.length; i++) {
            values[i] = new ContentValues();
            values[i].put(MediaStore.Audio.Playlists.Members.PLAY_ORDER, i + count);
            values[i].put(MediaStore.Audio.Playlists.Members.AUDIO_ID, songs[i]);
        }

        ContentResolver resolver = context.getContentResolver();
        int added = resolver.bulkInsert(uri, values);
        return added;
    }


    public static int getPlaylistSize(Context context, Uri uri) {
        String data[] = { COUNT };
        Cursor cursor = context.getContentResolver().query(uri, data, null, null, null);
        int column = cursor.getColumnIndex( COUNT );
        cursor.moveToFirst();

        try {
            return cursor.getInt(column);
        } finally {
            cursor.close();
        }
    }

//    public void removeSongsFromPlaylist(Context context, long playlistId,
//                                    long audioId[]) {
//        ContentResolver resolver = context.getContentResolver();
//        int countDel = 0;
//        try {
//            Uri uri = MediaStore.Audio.Playlists.Members.getContentUri(
//                    "external", playlistId);
//            String filter = MediaStore.Audio.Playlists.Members._ID + "=?";
//
//            String[] whereVal = { Long.toString(audioId) };
//            countDel=resolver.delete(uri, filter, whereVal);
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
//
//        return countDel;
//
//    }

    public void deletePlaylist() {
        ContentResolver resolver = mContext.getContentResolver();
        String filter;
        filter = MediaStore.Audio.Playlists._ID + "=" + mID;
        resolver.delete(MediaStore.Audio.Playlists.EXTERNAL_CONTENT_URI,
                filter, null);
        Toast.makeText(mContext, mName + mContext.getString(R.string.playlist_deleted),
                Toast.LENGTH_SHORT).show();
    }
}
