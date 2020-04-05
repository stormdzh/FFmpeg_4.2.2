//
// Created by tal on 2020-04-04.
//

#ifndef FFMPEG_4_2_2_DZJNICALL_H
#define FFMPEG_4_2_2_DZJNICALL_H

#include <jni.h>

enum ThreadMode {
    THREAD_CHILD, THREAD_MAIN
};

class DZJNICall {
public:
    JavaVM *javaVM;
    JNIEnv *jniEnv;
    jmethodID jPlayerErrorMid;
    jmethodID jPlayerPreparedMid;
    jobject jPlayerObj;
public:
    DZJNICall(JavaVM *javaVM, JNIEnv *jniEnv, jobject jPlayerObj);

    ~DZJNICall();

public:
    void callPlayerError(ThreadMode threadMode, int code, char *msg);

    void callPlayerPrepared(ThreadMode mode);
};

#endif //FFMPEG_4_2_2_DZJNICALL_H
