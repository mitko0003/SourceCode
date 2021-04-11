package com.unisofia.fmi.musicplayer.fragment;

import android.app.Fragment;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;

import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.adapter.AllSongsAdapter;
import com.unisofia.fmi.musicplayer.adapter.PlaylistAdapter;
import com.unisofia.fmi.musicplayer.media.Song;
import com.unisofia.fmi.musicplayer.players.Playlist;

/**
 * Created by dimit_000 on 2/7/2015.
 */
public class AllSongsFragment extends Fragment implements AdapterView.OnItemClickListener {

    private AllSongsAdapter mAdapter;
    private Context mContext;

    public AllSongsFragment() {}

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mContext = getActivity();
        mAdapter = new AllSongsAdapter(getActivity());
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_all_songs, container, false);

        // Set the adapter
        ListView listView = (ListView) view.findViewById(R.id.list_view_all_songs);
        listView.setAdapter(mAdapter);

        listView.setOnItemClickListener(this);

        return view;
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        if (mContext instanceof OnSongSelectedListener) {
            ((OnSongSelectedListener) mContext)
                    .onSongSelected(position, mAdapter.getPlaylist());
        } else {
            throw new IllegalStateException(
                    "Parent activity must implement OnSongSelectedListener");
        }
    }

    public static interface OnSongSelectedListener {
        public void onSongSelected(int position, Playlist playlist);
        public void onSongPlayNext(Song song);
        public void onSongAddToQueue(Song song);
    }
}
