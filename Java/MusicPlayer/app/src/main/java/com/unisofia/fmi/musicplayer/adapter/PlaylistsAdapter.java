package com.unisofia.fmi.musicplayer.adapter;

import android.content.Context;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.support.v7.widget.PopupMenu;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.MenuInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.squareup.picasso.Picasso;
import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.players.Playlist;

import java.util.ArrayList;
import java.util.List;

public class PlaylistsAdapter extends BaseAdapter {

    private List<String> mPlaylists = new ArrayList<>();
    private LayoutInflater mInflater;
    private Context mContext;

    public PlaylistsAdapter(Context context) {
        mContext = context;
        mPlaylists.add(context.getString(R.string.new_playlist));
        mPlaylists.addAll(Playlist.getPlaylistsNames(context));
        mInflater = LayoutInflater.from(context);
    }

    @Override
    public int getCount() {
        return mPlaylists.size();
    }

    @Override
    public String getItem(int position) {
        return mPlaylists.get(position);
    }

    @Override
    public long getItemId(int position) {
        if (position == 0)
            return -1;
        return Playlist.getID(mContext, mPlaylists.get(position));
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder viewHolder;
        if (convertView == null) {
            convertView = mInflater.inflate(R.layout.item_playlist_name,
                    parent, false);

            viewHolder = new ViewHolder();

            viewHolder.mName = (TextView) convertView.findViewById(R.id.text_view_playlist_name);

            convertView.setTag(viewHolder);
        } else {
            viewHolder = (ViewHolder) convertView.getTag();
        }

        viewHolder.mName.setText(mPlaylists.get(position));

        return convertView;
    }

    private class ViewHolder {
        public TextView mName;
    }
}
