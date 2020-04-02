package com.stormdzh.ffmpeg.sdk.util.entity;

/**
 * 混合到指定位置的实体类
 */
public class MixAudioEntity {

    public String wavPath; //wav的路径
    public int start;     //开始混合的位置 单位秒

    public MixAudioEntity(String wavPath, int start) {
        this.wavPath = wavPath;
        this.start = start;
    }
}
