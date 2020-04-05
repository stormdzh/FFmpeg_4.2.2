package com.stormdzh.ffmpeg.demo;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;

import com.stormdzh.ffmpeg.sdk.audio.AudioPlayer;
import com.stormdzh.ffmpeg.sdk.sles.DarrenPlayer;
import com.stormdzh.ffmpeg.sdk.sles.listener.MediaErrorListener;
import com.stormdzh.ffmpeg.sdk.sles.listener.MediaPreparedListener;
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
//                audioPlay();
                audioSles();
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

//        File mp3File = new File(Environment.getExternalStorageDirectory(), "3_test.wav");
//        File mp3File = new File(Environment.getExternalStorageDirectory(), "mp3test.mp3");
        File mp3File = new File(Environment.getExternalStorageDirectory(), "bb.mp3");
//        File mp3File = new File(Environment.getExternalStorageDirectory(), "aa.mp3");

        AudioPlayer mAudioPlayer = new AudioPlayer();
        mAudioPlayer.setDataSource(mp3File.getAbsolutePath());
        mAudioPlayer.play();

//        //音频流类型
//        private static final int mStreamType = AudioManager.STREAM_MUSIC;
//        //指定采样率 （MediaRecoder 的采样率通常是8000Hz AAC的通常是44100Hz。 设置采样率为44100，目前为常用的采样率，官方文档表示这个值可以兼容所有的设置）
//        private static final int mSampleRateInHz=44100 ;
//        //指定捕获音频的声道数目。在AudioFormat类中指定用于此的常量
//        private static final int mChannelConfig= AudioFormat.CHANNEL_CONFIGURATION_MONO; //单声道
//        //指定音频量化位数 ,在AudioFormaat类中指定了以下各种可能的常量。通常我们选择ENCODING_PCM_16BIT和ENCODING_PCM_8BIT PCM代表的是脉冲编码调制，它实际上是原始音频样本。
//        //因此可以设置每个样本的分辨率为16位或者8位，16位将占用更多的空间和处理能力,表示的音频也更加接近真实。
//        private static final int mAudioFormat=AudioFormat.ENCODING_PCM_16BIT;
//        //指定缓冲区大小。调用AudioRecord类的getMinBufferSize方法可以获得。
//        private int mMinBufferSize;
//        //STREAM的意思是由用户在应用程序通过write方式把数据一次一次得写到audiotrack中。这个和我们在socket中发送数据一样，
//        // 应用层从某个地方获取数据，例如通过编解码得到PCM数据，然后write到audiotrack。
//        private static int mMode = AudioTrack.MODE_STREAM;
    }


    DarrenPlayer mPlayer;

    private void audioSles() {
        File mp3File = new File(Environment.getExternalStorageDirectory(), "bb.mp3");
        mPlayer = new DarrenPlayer();

        mPlayer.setDataSource(mp3File.getAbsolutePath().trim());

        mPlayer.setOnErrorListener(new MediaErrorListener() {
            @Override
            public void onError(int code, String msg) {
                Log.e("TAG", "error code: " + code);
                Log.e("TAG", "error msg: " + msg);
                // Java 的逻辑代码
            }
        });

//        mPlayer.setOnPreparedListener(new MediaPreparedListener() {
//            @Override
//            public void onPrepared() {
//                Log.e("TAG", "准备完毕");
//                mPlayer.play();
//            }
//        });
//        mPlayer.prepareAsync();

        mPlayer.prepare();
        mPlayer.play();

    }

}

