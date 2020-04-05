//
// Created by tal on 2020-04-04.
//

#ifndef FFMPEG_4_2_2_DZPACKETQUEUE_H
#define FFMPEG_4_2_2_DZPACKETQUEUE_H


#include <queue>
#include <pthread.h>

extern "C" {
#include <libavcodec/avcodec.h>
};

class DZPacketQueue {
public:
    std::queue<AVPacket *> *pPacketQueue;
    pthread_mutex_t packetMutex;
    pthread_cond_t packetCond;
public:
    DZPacketQueue();

    ~DZPacketQueue();

public:
    void push(AVPacket *pPacket);

    AVPacket *pop();

    /**
     * 请求整个队列
     */
    void clear();
};

#endif //FFMPEG_4_2_2_DZPACKETQUEUE_H
