package com.unisofia.fmi.musicplayer.fragment;

import android.app.Fragment;
import android.content.Context;
import android.os.Bundle;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.adapter.PlaylistsGridAdapter;

public class PlaylistsGridFragment extends Fragment {

    private RecyclerView mRecyclerView;
    private GridLayoutManager mLayoutManager;

    PlaylistsGridAdapter myRecyclerAdapter;


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_playlists_grid, container, false);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        Context context = this.getActivity();

        mRecyclerView = (RecyclerView) view.findViewById(R.id.recycler_view_playlist_grid);
        mLayoutManager = new GridLayoutManager(context, 2, GridLayoutManager.VERTICAL, false);
        mRecyclerView.setLayoutManager(mLayoutManager);
        mRecyclerView.setHasFixedSize(false);
        myRecyclerAdapter = new PlaylistsGridAdapter(context);
        mRecyclerView.setAdapter(myRecyclerAdapter);
    }
}
