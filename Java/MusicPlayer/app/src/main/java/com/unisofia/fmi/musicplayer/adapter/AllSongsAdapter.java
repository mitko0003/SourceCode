package com.unisofia.fmi.musicplayer.adapter;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.support.v7.widget.PopupMenu;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.squareup.picasso.Picasso;
import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.activity.AddToPlaylistActivity;
import com.unisofia.fmi.musicplayer.fragment.AllSongsFragment;
import com.unisofia.fmi.musicplayer.media.Song;
import com.unisofia.fmi.musicplayer.players.Playlist;

/**
 * Created by dimit_000 on 2/7/2015.
 */
public class AllSongsAdapter extends BaseAdapter {

    private Playlist mAllSongs;
    private LayoutInflater mInflater;
    private Context mContext;

    public AllSongsAdapter(Context context) {
        mContext = context;
        mAllSongs = Playlist.loadAllMusic(context);
        mInflater = LayoutInflater.from(context);
    }

    public Playlist getPlaylist() {
        return mAllSongs;
    }

    @Override
    public int getCount() {
        return mAllSongs.size();
    }

    @Override
    public Song getItem(int position) {
        return mAllSongs.getSong(position);
    }

    @Override
    public long getItemId(int position) {
        return mAllSongs.getSong(position).getID();
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder viewHolder;
        if (convertView == null) {
            convertView = mInflater.inflate(R.layout.item_song,
                    parent, false);

            viewHolder = new ViewHolder();

            viewHolder.mSettings = (ImageButton) convertView
                    .findViewById(R.id.image_button_song_settings);
            viewHolder.mSongTitle = (TextView) convertView
                    .findViewById(R.id.text_view_song_title);
            viewHolder.mSongArtist = (TextView) convertView
                    .findViewById(R.id.text_view_song_artist);
            viewHolder.mSongThumbnail = (ImageView) convertView
                    .findViewById(R.id.image_view_song_thumbnail);

            convertView.setTag(viewHolder);
        } else {
            viewHolder = (ViewHolder) convertView.getTag();
        }

        final Song item = getItem(position);
        viewHolder.mSongTitle.setText(item.getTitle());
        viewHolder.mSongArtist.setText(item.getArtist());

        viewHolder.mSettings.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showPopup(v).setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                        switch(menuItem.getItemId()) {
                            case R.id.action_play_next:
                                ((AllSongsFragment.OnSongSelectedListener) mContext)
                                        .onSongPlayNext(item);
                                break;

                            case R.id.action_add_to_queue:
                                ((AllSongsFragment.OnSongSelectedListener) mContext)
                                        .onSongAddToQueue(item);
                                break;

                            case R.id.action_add_to_playlist:
                                Intent intent = new Intent(mContext, AddToPlaylistActivity.class);

                                intent.putExtra(AddToPlaylistActivity.EXTRA_SONGS, new long[] {item.getAudioID()});
                                mContext.startActivity(intent);
                                break;
                        }
                        return true;
                    }
                });
            }

        });

        Uri image = item.getThumbnail(mContext.getContentResolver());
        if (image == null) {
            Picasso.with(mContext).load(R.drawable.default_song_thumbnail)
                    .into(viewHolder.mSongThumbnail);
        } else {
            Picasso.with(mContext).load(image).fit().into(viewHolder.mSongThumbnail);
        }

        return convertView;
    }

    public PopupMenu showPopup(View v) {
        PopupMenu popup = new PopupMenu(mContext, v);
        MenuInflater inflater = popup.getMenuInflater();
        inflater.inflate(R.menu.menu_song, popup.getMenu());
        popup.show();
        return popup;
    }

    private class ViewHolder {
        public ImageButton mSettings;
        public ImageView mSongThumbnail;
        public TextView mSongTitle;
        public TextView mSongArtist;
    }
}
