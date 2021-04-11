package com.unisofia.fmi.musicplayer.fragment;

import android.app.Activity;
import android.app.Fragment;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;

import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.adapter.PlaylistAdapter;
import com.unisofia.fmi.musicplayer.players.Playlist;

public class PlaylistFragment extends Fragment implements AdapterView.OnItemClickListener {

    private static final String PLAYLIST = "playlist";

    private PlaylistAdapter mAdapter;
    private ListView mListView;
    private Context mContext;

    public static PlaylistFragment newInstance(long playlist) {
        PlaylistFragment fragment = new PlaylistFragment();
        Bundle args = new Bundle();
        args.putLong(PLAYLIST, playlist);
        fragment.setArguments(args);
        return fragment;
    }

    public PlaylistFragment() {}

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        long playlist = getArguments().getLong(PLAYLIST);
        mAdapter = new PlaylistAdapter(getActivity(), playlist);
        mContext = getActivity();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_playlist, container, false);

        // Set the adapter
        mListView = (ListView) view.findViewById(R.id.list_view_playlist);
        mListView.setAdapter(mAdapter);

        mListView.setOnItemClickListener(this);

        return view;
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        if (mContext instanceof AllSongsFragment.OnSongSelectedListener) {
            ((AllSongsFragment.OnSongSelectedListener) mContext)
                    .onSongSelected(position, mAdapter.getPlaylist());
        } else {
            throw new IllegalStateException(
                    "Parent activity must implement OnSongSelectedListener");
        }
    }
}