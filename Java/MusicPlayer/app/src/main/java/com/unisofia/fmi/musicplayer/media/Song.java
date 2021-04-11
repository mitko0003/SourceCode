package com.unisofia.fmi.musicplayer.media;

import android.content.ContentResolver;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.widget.Toast;

import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.activity.AddToPlaylistActivity;

/**
 * Created by dimit_000 on 2/7/2015.
 */
public class Song implements Toolbar.OnMenuItemClickListener {

    private final long mAudioID;
    private final long mID;
    private final String mPath;
    private final String mTitle;
    private final String mArtist;
    private final String mAlbum;
    private final long mAlbumID;

    public Song(Cursor cursor) {
        mID = cursor.getLong(
                cursor.getColumnIndex(MediaStore.Audio.Media._ID));
        mPath = cursor.getString(
                cursor.getColumnIndex(MediaStore.Audio.Media.DATA));
        mTitle = cursor.getString(
                cursor.getColumnIndex(MediaStore.Audio.Media.TITLE));
        mArtist = cursor.getString(
                cursor.getColumnIndex(MediaStore.Audio.Media.ARTIST));
        mAlbumID = cursor.getLong(
                cursor.getColumnIndex(MediaStore.Audio.Media.ALBUM_ID));
        mAlbum = cursor.getString(
                cursor.getColumnIndex(MediaStore.Audio.Media.ALBUM));

        int audioIDColumn = cursor.getColumnIndex(
                MediaStore.Audio.Playlists.Members.AUDIO_ID);
        mAudioID = audioIDColumn != -1 ? cursor.getLong(audioIDColumn) : mID;
    }

    @Override
    public boolean onMenuItemClick(MenuItem item) {
//        switch(item.getItemId()) {
//            case R.id.action_play_next:
//                Toast.makeText(mContext, "You selected Phone", Toast.LENGTH_SHORT).show();
//                break;
//
//            case R.id.action_add_to_queue:
//                Toast.makeText(mContext, "You selected Computer", Toast.LENGTH_SHORT).show();
//                break;
//
//            case R.id.action_add_to_playlist:
//                Intent intent = new Intent(mContext, AddToPlaylistActivity.class);
//
//                intent.putExtra(AddToPlaylistActivity.EXTRA_SONGS, new long[] {mAudioID});
//                mContext.startActivity(intent);
//                break;
//        }
        return true;
    }

    public long getID() {
        return mID;
    }
    //
//        public String getAlbumName() {
////            if (mCursor == null){
////                return null;
////            }
//            return mCursor.getString(
//                    mCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.ALBUM));
//        }
//
    public Uri getThumbnail(ContentResolver resolver) {
        String data[] = new String[] {MediaStore.Audio.AlbumColumns.ALBUM_ART};
        String filter = MediaStore.Audio.Media._ID + "=" + mAlbumID;
        Cursor artCursor = resolver.query(
                MediaStore.Audio.Albums.EXTERNAL_CONTENT_URI, data, filter, null, null);
        artCursor.moveToFirst();

        try {
            String result = artCursor.getString(
                    artCursor.getColumnIndex(MediaStore.Audio.AlbumColumns.ALBUM_ART));
            if(result == null) {
                return null;
            }
            return Uri.parse("file://" + result);
        } finally {
            artCursor.close();
        }
    }

    public String getArtist() {
        return mArtist;
    }

    public String getTitle() {
        return mTitle;
    }

    public String getAlbum() {
        return mAlbum;
    }
//
//        public String getTitle() {
//            return mCursor.getString(
//                    mCursor.getColumnIndexOrThrow(MediaStore.Audio.Media.TITLE));
//        }

    public long getAudioID() {
        return mAudioID;
    }

    public String getPath() {
        return mPath;
    }
}
