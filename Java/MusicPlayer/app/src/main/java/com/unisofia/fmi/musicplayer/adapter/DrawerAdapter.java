package com.unisofia.fmi.musicplayer.adapter;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.unisofia.fmi.musicplayer.R;
import com.unisofia.fmi.musicplayer.constant.DrawerOptionsEnum;
import com.unisofia.fmi.musicplayer.players.Playlist;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by dimit_000 on 2/7/2015.
 */
public class DrawerAdapter extends BaseAdapter {

    private DrawerOptionsEnum mOptions[];
    private LayoutInflater mInflater;
    private Context mContext;

    public DrawerAdapter(Context context) {
        mContext = context;
        mOptions = DrawerOptionsEnum.values();
        mInflater = LayoutInflater.from(context);
    }

    @Override
    public int getCount() {
        return mOptions.length;
    }

    @Override
    public DrawerOptionsEnum getItem(int position) {
        return mOptions[position];
    }

    @Override
    public long getItemId(int position) {
        return mOptions[position].ID;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder viewHolder;
        if (convertView == null) {
            convertView = mInflater.inflate(R.layout.item_drawer_option,
                    parent, false);

            viewHolder = new ViewHolder();

            viewHolder.mTextViewTitle = (TextView) convertView.findViewById(R.id.text_view_option_title);

            convertView.setTag(viewHolder);
        } else {
            viewHolder = (ViewHolder) convertView.getTag();
        }

        viewHolder.mTextViewTitle.setText(mOptions[position].getTitle(mContext));

        return convertView;
    }

    private class ViewHolder {
        public TextView mTextViewTitle;
    }
}
