package com.stormdzh.ffmpeg.sdk.util;

import android.text.TextUtils;

import com.stormdzh.ffmpeg.sdk.util.entity.MixAudioEntity;

import java.util.ArrayList;
import java.util.List;

public class FFmpegCommands {


    /**
     * mp3 文件 转 wav
     *
     * @param mp3url mp3地址
     * @param wavurl wav地址
     * @return 转换命令
     */
    public static String[] mp3ToWav(String mp3url, String wavurl) {
//        ffmpeg -i /Users/a111/Desktop/ffmpeg/bgm.mp3 -ar 8000 -ab 12.2k -ac 1 -f wav /Users/a111/Desktop/ffmpeg/bgm.wav
        ArrayList<String> _commands = new ArrayList<>();
        _commands.add("ffmpeg");
        _commands.add("-i");
        _commands.add(mp3url);
        _commands.add("-ar");
        _commands.add("8000");
        _commands.add("-ab");
        _commands.add("12.2k");
        _commands.add("-ac");
        _commands.add("1");
        _commands.add("-f");
        _commands.add("wav");
        _commands.add("-y");
        _commands.add(wavurl);
        String[] commands = new String[_commands.size()];
        for (int i = 0; i < _commands.size(); i++) {
            commands[i] = _commands.get(i);
        }
        return commands;
    }

    /**
     * 提取单独的视频，没有声音
     *
     * @param mp4    mp4
     * @param outmp4 outmp4
     * @return _commands
     */
    public static String[] extractVideo(String mp4, String outmp4) {
        String[] commands = new String[8];
        commands[0] = "ffmpeg";
        commands[1] = "-i";
        commands[2] = mp4;
        commands[3] = "-vcodec";
        commands[4] = "copy";
        commands[5] = "-an";
        commands[6] = "-y";
        commands[7] = outmp4;
        return commands;
    }


    //-----------------有用的ffmpeg命令------------------------------------------

    /**
     * wav拼接
     *
     * @param wavList    wavList
     * @param outputPath outputPath
     * @return _commands
     */
    public static String[] concatWav(List<String> wavList, String outputPath) {

//        ffmpeg  -i 1.wav -i 2.wav -i 3.wav -i 4.wav -i 5.wav -filter_complex '[0:0] [1:0]  concat=n=5:v=0:a=1[out]' -map '[out]' concat_1.wav

        if (wavList == null || wavList.size() < 2) return null;
        ArrayList<String> _commands = new ArrayList<>();
        _commands.add("ffmpeg");

        for (int i = 0; i < wavList.size(); i++) {
            _commands.add("-i");
            _commands.add(wavList.get(i));
        }
        _commands.add("-filter_complex");
        _commands.add("[0:0] [1:0]  concat=n=" + wavList.size() + ":v=0:a=1[out]");
        _commands.add("-map");
        _commands.add("[out]");
        _commands.add("-ar");
        _commands.add("8000");
        _commands.add("-ab");
        _commands.add("2.4k");
        _commands.add("-ac");
        _commands.add("1");
        _commands.add(outputPath);

        String[] commands = new String[_commands.size()];
        for (int i = 0; i < _commands.size(); i++) {
            commands[i] = _commands.get(i);
        }
        return commands;
    }

    public static  String[] compressWav(String inputPath, String outputPath){

        ArrayList<String> _commands = new ArrayList<>();
        _commands.add("ffmpeg");
        _commands.add("-i");
        _commands.add(inputPath);
        _commands.add("-ar");
        _commands.add("8000");
        _commands.add("-ac");
        _commands.add("1");
        _commands.add("-ab");
        _commands.add("12.2k");
        _commands.add("-f");
        _commands.add("wav");
        _commands.add(outputPath);

        String[] commands = new String[_commands.size()];
        for (int i = 0; i < _commands.size(); i++) {
            commands[i] = _commands.get(i);
        }
        return commands;
    }


    /**
     * 混合wav到指定位置
     *
     * @param bgWav      bgWav
     * @param wavList    wavList
     * @param outputPath outputPath
     * @return commands
     */
    public static String[] mixMultiWav(String bgWav, List<MixAudioEntity> wavList, String outputPath) {
        //    ffmpeg  -y  -i  bgm.mp3    -itsoffset  5  -i  0_p_record.wav  -itsoffset  15  -i  1_p_record.wav  -itsoffset  25  -i  2_p_record.wav  -itsoffset  35  -i  3_p_record.wav  -itsoffset  45  -i  4_p_record.wav  -filter_complex  "amix=inputs=6:duration=first:dropout_transition=1,volume=10"  -async  1  -b:a  512k  output_5_vol_10.mp3

        if (TextUtils.isEmpty(bgWav) || wavList == null || wavList.size() == 0) return null;

        ArrayList<String> _commands = new ArrayList<>();
        _commands.add("ffmpeg");
        _commands.add("-y");
        _commands.add("-i");
        _commands.add(bgWav);

        for (MixAudioEntity entity : wavList) {
            if (entity == null || TextUtils.isEmpty(entity.wavPath)) continue;
            _commands.add("-itsoffset");
            _commands.add(String.valueOf(entity.start));
            _commands.add("-i");
            _commands.add(entity.wavPath);
        }

        _commands.add("-filter_complex");
        _commands.add("amix=inputs=" + (wavList.size() + 1) + ":duration=longest:dropout_transition=1,volume=10");
        _commands.add("-async");
        _commands.add("1");
        _commands.add("-b:a");
//        _commands.add("512k");
        _commands.add("32k");
        _commands.add(outputPath);

        String[] commands = new String[_commands.size()];
        for (int i = 0; i < _commands.size(); i++) {
            commands[i] = _commands.get(i);
        }

        return commands;
    }


    public static String[] wavToMp3(String wavurl, String mp3url) {
//        ffmpeg -i input.wav -f mp3 -acodec libmp3lame -y output.mp3
        ArrayList<String> _commands = new ArrayList<>();
        _commands.add("ffmpeg");
        _commands.add("-i");
        _commands.add(wavurl);
        _commands.add("-f");
        _commands.add("mp3");
        _commands.add("-acodec");
        _commands.add("libmp3lame");
        _commands.add("-y");
        _commands.add(mp3url);
        String[] commands = new String[_commands.size()];
        for (int i = 0; i < _commands.size(); i++) {
            commands[i] = _commands.get(i);
        }
        return commands;
    }


}
