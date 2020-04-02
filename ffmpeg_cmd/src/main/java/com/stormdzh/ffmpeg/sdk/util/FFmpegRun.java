package com.stormdzh.ffmpeg.sdk.util;

import android.os.AsyncTask;


/**
 * Created by dzh
 * Date 2018/11/19
 * email tangyx@live.com
 */

public class FFmpegRun {
    static {
        System.loadLibrary("ffmpeginvoke");
    }

    @SuppressWarnings("all")
    public native static int run(String[] commands);

    public static AsyncTask execute(String[] commands, final String tag, final FFmpegRunListener fFmpegRunListener) {
        AsyncTask<String[], Integer, Integer> task = new AsyncTask<String[], Integer, Integer>() {
            @Override
            protected void onPreExecute() {
                if (fFmpegRunListener != null) {
                    fFmpegRunListener.onStart();
                }
            }

            @Override
            protected Integer doInBackground(String[]... params) {
                return run(params[0]);
            }

            @Override
            protected void onPostExecute(Integer integer) {
                if (fFmpegRunListener != null) {
                    fFmpegRunListener.onEnd(integer);
                }
            }
        }.execute(commands);
        return task;
    }

    public interface FFmpegRunListener {
        void onStart();

        void onEnd(int result);
    }
}
