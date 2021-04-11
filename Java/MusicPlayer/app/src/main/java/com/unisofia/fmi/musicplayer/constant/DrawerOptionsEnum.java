package com.unisofia.fmi.musicplayer.constant;

import android.content.Context;

import com.unisofia.fmi.musicplayer.R;

/**
 * Created by dimit_000 on 2/7/2015.
 */
public enum DrawerOptionsEnum {
    ALL_SONGS(R.string.all_songs),
    PLAYLISTS(R.string.playlists);

    public final int ID;

    public String getTitle(Context context) {
        return context.getString(ID);
    }

    private DrawerOptionsEnum(int id) {
        ID = id;
    }
}
