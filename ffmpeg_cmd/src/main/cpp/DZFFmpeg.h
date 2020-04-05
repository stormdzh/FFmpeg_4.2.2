//
// Created by tal on 2020-04-04.
//

#ifndef FFMPEG_4_2_2_DZFFMPEG_H
#define FFMPEG_4_2_2_DZFFMPEG_H

#include "DZJNICall.h"
#include "DZAudio.h"
#include <pthread.h>

extern "C" {
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
};

class DZFFmpeg {
public:
    AVFormatContext *pFormatContext = NULL;
    char *url = NULL;
    DZJNICall *pJniCall = NULL;
    DZAudio *pAudio = NULL;

public:
    DZFFmpeg(DZJNICall *pJniCall, const char *url);

    ~DZFFmpeg();

public:
    void play();

    void prepare();

    void prepareAsync();

    void prepare(ThreadMode threadMode);

    void callPlayerJniError(ThreadMode threadMode, int code, char *msg);

    void release();
};

#endif //FFMPEG_4_2_2_DZFFMPEG_H
