package com.stormdzh.ffmpeg.sdk.util;


import com.stormdzh.ffmpeg.sdk.util.entity.MixAudioEntity;

import java.util.List;

public class MediaUtils {
    /**
     * 链接多个wav
     *
     * @param wavList    多个wav的列表
     * @param outputPath 输出的wavpath
     * @param listener   设置监听
     */
    public static void concatWav(List<String> wavList, String outputPath, FFmpegRun.FFmpegRunListener listener) {
        FFmpegRun.execute(FFmpegCommands.concatWav(wavList, outputPath), outputPath, listener);
    }

    /**
     * 混合wav 到 背景wav的指定位置
     *
     * @param bgWav      bgWav
     * @param wavList    wavList
     * @param outputPath outputPath
     * @param listener   listener
     */
    public static void mixMultiWav(String bgWav, List<MixAudioEntity> wavList, String outputPath, FFmpegRun.FFmpegRunListener listener) {
        FFmpegRun.execute(FFmpegCommands.mixMultiWav(bgWav, wavList, outputPath), outputPath, listener);
    }

    /**
     * mp3转wav
     *
     * @param mp3url   mp3
     * @param wavurl   wav
     * @param listener 监听
     */
    public static void mp3ToWav(String mp3url, String wavurl, FFmpegRun.FFmpegRunListener listener) {
        String[] common = FFmpegCommands.mp3ToWav(mp3url, wavurl);
        FFmpegRun.execute(common, wavurl, listener);
    }

    public static void compressWav(String inputWavPath, String outputWavPath, FFmpegRun.FFmpegRunListener listener) {
        String[] common = FFmpegCommands.compressWav(inputWavPath, outputWavPath);
        FFmpegRun.execute(common, outputWavPath, listener);
    }

    /**
     * 抽取出一个静音视频
     *
     * @param mp4      mp4
     * @param outmp4   outmp4
     * @param listener listener
     */
    public static void extractVideo(String mp4, String outmp4, FFmpegRun.FFmpegRunListener listener) {
        String[] common = FFmpegCommands.extractVideo(mp4, outmp4);
        FFmpegRun.execute(common, outmp4, listener);
    }

    public static void wavToMp3(String wav, String mp3, FFmpegRun.FFmpegRunListener listener) {
        //ffmpeg -i input.wav -f mp3 -acodec libmp3lame -y output.mp3
        String[] common = FFmpegCommands.wavToMp3(wav, mp3);
        FFmpegRun.execute(common, mp3, listener);

    }

}
