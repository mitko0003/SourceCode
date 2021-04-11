package com.unisofia.fmi.musicplayer.adapter;

import android.app.Activity;
import android.content.Context;
import android.net.Uri;
import android.support.v7.widget.PopupMenu;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.LayoutInflater;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.squareup.picasso.Picasso;
import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.activity.PlaybackActivity;
import com.unisofia.fmi.musicplayer.fragment.PlaylistsGridFragment;
import com.unisofia.fmi.musicplayer.players.Playlist;

import java.util.List;
import java.util.Random;

public class PlaylistsGridAdapter extends RecyclerView.Adapter<PlaylistsGridAdapter.ViewHolder> {

    private Context mContext;
    private List<Playlist> mPlaylists;
    private static final int BG_COLORS[] = {
            R.color.blue_light,
            R.color.purple_light,
            R.color.red_light,
            R.color.cyan_light,
            R.color.teal_light,
            R.color.lime_light,
            R.color.green_light,
            R.color.primary_light
    };
    private final Random mGenerator = new Random();

    public PlaylistsGridAdapter(Context context) {
        mContext = context;
        mPlaylists = Playlist.getPlaylists(context);
    }

    class ViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        public ImageView mImageViews[];
        public TextView mTextViewTitle;
        public ImageButton mImageButton;
        public RelativeLayout mLayout;

        public ViewHolder (View itemView) {
            super(itemView);

            mImageButton = (ImageButton) itemView.findViewById(R.id.image_button_playlist_settings);

            mImageViews = new ImageView[4];
            mImageViews[0] =(ImageView) itemView.findViewById(R.id.image_view_top_left);
            mImageViews[1] =(ImageView) itemView.findViewById(R.id.image_view_top_right);
            mImageViews[2] =(ImageView) itemView.findViewById(R.id.image_view_bottom_left);
            mImageViews[3] =(ImageView) itemView.findViewById(R.id.image_view_bottom_right);

            mLayout = (RelativeLayout)
                    itemView.findViewById(R.id.relative_layout_playlist);
            mTextViewTitle = (TextView) itemView.findViewById(R.id.text_view_playlist_title);

            mLayout.setOnClickListener(this);
        }


        @Override
        public void onClick(View v) {
            if (mContext instanceof OnPlaylistSelectedListener) {
                ((OnPlaylistSelectedListener) mContext)
                        .onPlaylistSelected(mPlaylists.get(getPosition()).getID());
            } else {
                throw new IllegalStateException(
                        "Parent activity must implement OnUserItemSelected");
            }
        }
    }

    @Override
    public int getItemCount() {
        return mPlaylists.size();
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        final Playlist playlist = mPlaylists.get(position);

        Uri image;
        for (int i = 0; i < 4; ++i) {
            if (i >= playlist.size()) {
                holder.mImageViews[i].setImageDrawable(null);
                continue;
            }
            image = playlist.getSong(i).getThumbnail(mContext.getContentResolver());
            if (image == null)
                Picasso.with(mContext).load(R.drawable.default_song_thumbnail)
                        .into(holder.mImageViews[i]);
            else
                Picasso.with(mContext).load(image).fit().into(holder.mImageViews[i]);
        }

        holder.mTextViewTitle.setText(playlist.getName());

        holder.mImageButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showPopup(v).setOnMenuItemClickListener(new PopupMenu.OnMenuItemClickListener() {
                    @Override
                    public boolean onMenuItemClick(MenuItem menuItem) {
                        switch (menuItem.getItemId()) {
                            case R.id.action_remove_playlist:
                                playlist.deletePlaylist();
                                ((PlaybackActivity) mContext).recreateFragment(
                                        new PlaylistsGridFragment());
                                break;
                        }
                        return true;
                    }
                });
            }
        });

        holder.mLayout.setBackgroundResource(BG_COLORS[mGenerator.nextInt(BG_COLORS.length - 1)]);
//        holder.mTextViewName.setText(playlist.getName());
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int arg1) {
        LayoutInflater inflater =
                (LayoutInflater) mContext.getSystemService(Activity.LAYOUT_INFLATER_SERVICE);
        View convertView = inflater.inflate(R.layout.item_playlist, parent, false);
        return new ViewHolder(convertView);
    }

    public PopupMenu showPopup(View v) {
        PopupMenu popup = new PopupMenu(mContext, v);
        MenuInflater inflater = popup.getMenuInflater();
        inflater.inflate(R.menu.menu_playlist, popup.getMenu());
        popup.show();
        return popup;
    }

    public static interface OnPlaylistSelectedListener {
        public void onPlaylistSelected(long id);
    }
}