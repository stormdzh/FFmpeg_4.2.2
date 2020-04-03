package com.stormdzh.ffmpeg.demo;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.media.AudioTrack;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.TextView;

import com.stormdzh.ffmpeg.sdk.audio.AudioPlayer;
import com.stormdzh.ffmpeg.sdk.util.FFmpegRun;
import com.stormdzh.ffmpeg.sdk.util.MediaUtils;

import java.io.File;
import java.util.ArrayList;

public class MainActivity extends Activity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        requestPermissions();
        setContentView(R.layout.activity_main);
        findViewById(R.id.mp3ToWav).setOnClickListener(this);
        findViewById(R.id.wavToMp3).setOnClickListener(this);
        findViewById(R.id.audioPlay).setOnClickListener(this);
        String CPU_ABI = android.os.Build.CPU_ABI;
        Log.i("BAI=====", CPU_ABI);
    }

    private void requestPermissions() {
        ArrayList<String> ps = new ArrayList<>();
        int per = ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA);
        if (per != PackageManager.PERMISSION_GRANTED) {
            ps.add(Manifest.permission.CAMERA);
        }
        per = ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        if (per != PackageManager.PERMISSION_GRANTED) {
            ps.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
        }
        per = ContextCompat.checkSelfPermission(this, Manifest.permission.READ_PHONE_STATE);
        if (per != PackageManager.PERMISSION_GRANTED) {
            ps.add(Manifest.permission.READ_PHONE_STATE);
        }
        if (!ps.isEmpty()) {
            String[] ps3 = new String[ps.size()];
            ps.toArray(ps3);
            ActivityCompat.requestPermissions(this, ps3, 100);
        }
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.mp3ToWav:
                mp3ToWav();
                break;
            case R.id.wavToMp3:
                wavToMp3();
                break;
            case R.id.audioPlay:
                audioPlay();
                break;
        }
    }

    long time;

    private void wavToMp3() {

        File wavFile = new File(Environment.getExternalStorageDirectory(), "3_test.wav");
        Log.i("dzh", "输入文件存在吗？==》" + wavFile.exists());
        File mp3File = new File(Environment.getExternalStorageDirectory(), "mp3test.mp3");
        if (mp3File.exists()) {
            mp3File.delete();
        }
        time = System.currentTimeMillis();
        MediaUtils.wavToMp3(wavFile.getAbsolutePath(), mp3File.getAbsolutePath(), new FFmpegRun.FFmpegRunListener() {
            @Override
            public void onStart() {
                Log.i("dzh", "---------------onStart--------");
            }

            @Override
            public void onEnd(int result) {
                Log.i("dzh", "---------------end 耗时" + (System.currentTimeMillis() - time));
            }
        });
    }

    private void mp3ToWav() {

        File mp3File = new File(Environment.getExternalStorageDirectory(), "mp3test.mp3");
        Log.i("dzh", "输入文件存在吗？==》" + mp3File.exists());
        File wavFile = new File(Environment.getExternalStorageDirectory(), "wavtest.wav");
        if (wavFile.exists()) {
            wavFile.delete();
        }
        time = System.currentTimeMillis();
        MediaUtils.mp3ToWav(mp3File.getAbsolutePath(), wavFile.getAbsolutePath(), new FFmpegRun.FFmpegRunListener() {
            @Override
            public void onStart() {
                Log.i("dzh", "---------------onStart--------");
            }

            @Override
            public void onEnd(int result) {
                Log.i("dzh", "---------------end 耗时" + (System.currentTimeMillis() - time));
            }
        });
    }


    private void audioPlay() {

        File mp3File = new File(Environment.getExternalStorageDirectory(), "3_test.wav");
//        File mp3File = new File(Environment.getExternalStorageDirectory(), "mp3test.mp3");
//        File mp3File = new File(Environment.getExternalStorageDirectory(), "bb.mp3");

        AudioPlayer mAudioPlayer = new AudioPlayer();
        mAudioPlayer.setDataSource(mp3File.getAbsolutePath());
        mAudioPlayer.play();

//        AudioTrack
    }

}

