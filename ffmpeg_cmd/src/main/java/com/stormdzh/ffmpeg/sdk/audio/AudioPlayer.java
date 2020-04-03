package com.stormdzh.ffmpeg.sdk.audio;

import android.text.TextUtils;
import android.util.Log;

/**
 * @Description: 描述
 * @Author: dzh
 * @CreateDate: 2020-04-03 12:39
 */
public class AudioPlayer {
    static {
        System.loadLibrary("audioplayer");
    }


    private String url;

    public void setDataSource(String url) {
        this.url = url;
    }

    public void play() {
        if (TextUtils.isEmpty(url)) {
            throw new NullPointerException("音频播放地址为空！");
        } else {
            Log.i("dzh", " start play==>" + url);
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                start(url);
            }
        }).start();


    }

    private native void start(String url);
}
