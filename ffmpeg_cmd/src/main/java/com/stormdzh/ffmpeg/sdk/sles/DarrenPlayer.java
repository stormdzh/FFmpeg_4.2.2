package com.stormdzh.ffmpeg.sdk.sles;

import android.text.TextUtils;

import com.stormdzh.ffmpeg.sdk.sles.listener.MediaErrorListener;
import com.stormdzh.ffmpeg.sdk.sles.listener.MediaPreparedListener;

/**
 * Created by hcDarren on 2019/6/15.
 * 音频播放器的逻辑处理类
 */
public class DarrenPlayer {
    static {
        System.loadLibrary("audioplayer");
    }

    /**
     * url 可以是本地文件路径，也可以是 http 链接
     */
    private String url;

    private MediaErrorListener mErrorListener;

    private MediaPreparedListener mPreparedListener;

    public void setOnErrorListener(MediaErrorListener errorListener) {
        this.mErrorListener = errorListener;
    }

    public void setOnPreparedListener(MediaPreparedListener preparedListener) {
        this.mPreparedListener = preparedListener;
    }

    // called from jni
    private void onError(int code, String msg) {
        if (mErrorListener != null) {
            mErrorListener.onError(code, msg);
        }
    }

    // called from jni
    private void onPrepared() {
        if (mPreparedListener != null) {
            mPreparedListener.onPrepared();
        }
    }

    public void setDataSource(String url) {
        this.url = url;
    }

    public void play() {
        if (TextUtils.isEmpty(url)) {
            throw new NullPointerException("url is null, please call method setDataSource");
        }
        nPlay();
    }

    public void pause(){
        nPause();
    }

    private native void nPlay();

    public void prepare() {
        if (TextUtils.isEmpty(url)) {
            throw new NullPointerException("url is null, please call method setDataSource");
        }
        nPrepare(url);
    }

    /**
     * 异步准备
     */
    public void prepareAsync() {
        if (TextUtils.isEmpty(url)) {
            throw new NullPointerException("url is null, please call method setDataSource");
        }
        nPrepareAsync(url);
    }

    private native void nPrepareAsync(String url);

    private native void nPrepare(String url);

    private native void nPause();
}
