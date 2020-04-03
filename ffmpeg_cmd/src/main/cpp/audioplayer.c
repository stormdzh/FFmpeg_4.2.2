#include <jni.h>
#include "ffmpeg.h"
#include "libavformat/avformat.h"
#include <string.h>
#include <android/log.h>
/*
 * Class:     我自己写的音频播放类
 * Method:    run
 * Signature: (Ljava/lang/String;)I
 */

#define TAG "dzh"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

jclass jAudioTrackClass;
jobject jAudioTrack;
jmethodID jWriteMedId;

/*
    将 String 类型转成 char*
*/
char *Jstring2CStr(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = (*env)->FindClass(env, "java/lang/String");
    jstring strencode = (*env)->NewStringUTF(env, "GB2312");
    jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) (*env)->CallObjectMethod(env, jstr, mid, strencode);
    jsize alen = (*env)->GetArrayLength(env, barr);
    jbyte *ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);         //new   char[alen+1];
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    (*env)->ReleaseByteArrayElements(env, barr, ba, 0);

    return rtn;
}

/**
将char* 转成 其他类型
*/
jstring CStr2Jstring(JNIEnv *env, const char *pat) {
    //定义java String类 strClass
    jclass strClass = (*env)->FindClass(env, "java/lang/String");
    //获取java String类方法String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = (*env)->GetMethodID(env, strClass, "<init>", "([BLjava/lang/String;)V");
    //建立byte数组
    jbyteArray bytes = (*env)->NewByteArray(env, (jsize) strlen(pat));
    //将char* 转换为byte数组
    (*env)->SetByteArrayRegion(env, bytes, 0, (jsize) strlen(pat), (jbyte *) pat);
    //设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (*env)->NewStringUTF(env, "GB2312");
    //将byte数组转换为java String,并输出
    return (jstring) (*env)->NewObject(env, strClass, ctorID, bytes, encoding);

}


jobject createAudioTrack(JNIEnv *env) {
//    public AudioTrack(int streamType, int sampleRateInHz, int channelConfig, int audioFormat, int bufferSizeInBytes, int mode) throws IllegalArgumentException {
//            throw new RuntimeException("Stub!");
//    }

    LOGE("-----------------1-------------------");
    jAudioTrackClass = (*env)->FindClass(env, "android/media/AudioTrack");
    LOGE("-----------------1.1-------------------");
    jmethodID jAudioTrackMedId = (*env)->GetMethodID(env, jAudioTrackClass, "<init>", "(IIIIII)V");
    LOGE("-----------------2-------------------");

    int streamType = 3;
//    int sampleRateInHz = 44100;
    int sampleRateInHz = 16000;
    int channelConfig = (0x4 | 0x8);
    int audioFormat = 2;
    int mode = 1;

//    public static int getMinBufferSize(int sampleRateInHz, int channelConfig, int audioFormat) {
//        throw new RuntimeException("Stub!");
//    }LOGE("-----------------1-------------------");
    jmethodID getMinBufferSizeId = (*env)->GetStaticMethodID(env, jAudioTrackClass,
                                                             "getMinBufferSize",
                                                             "(III)I");
    LOGE("-----------------3-------------------");
    int bufferSizeBytes = (*env)->CallStaticIntMethod(env, jAudioTrackClass, getMinBufferSizeId,
                                                      sampleRateInHz,
                                                      channelConfig,
                                                      audioFormat);
    LOGE("-----------------4-------------------bufferSizeBytes:%d", bufferSizeBytes);

    jobject pVoid = (*env)->NewObject(env, jAudioTrackClass, jAudioTrackMedId,
                                      streamType, sampleRateInHz,
                                      channelConfig, audioFormat, bufferSizeBytes, mode);

    LOGE("-----------------5-------------------");
    //播放
    jmethodID playMedid = (*env)->GetMethodID(env, jAudioTrackClass, "play", "()V");
    LOGE("-----------------5.1-------------------");
    (*env)->CallVoidMethod(env, pVoid, playMedid);
    LOGE("-----------------6-------------------");


    return pVoid;
}


JNIEXPORT void JNICALL
Java_com_stormdzh_ffmpeg_sdk_audio_AudioPlayer_start(JNIEnv *env, jobject thiz, jstring jurl) {
    // TODO: implement start()
    const char *url = Jstring2CStr(env, jurl);
    LOGE("已经进入ini方法了！播放地址是：%s", url);

    av_register_all();
//    av_register_input_format()
//    av_register_output_format()

    avformat_network_init();

    AVFormatContext *pFormatContext = NULL;

    int formatOpenInputRes = 0;
    formatOpenInputRes = avformat_open_input(&pFormatContext, url, NULL, NULL);
    if (formatOpenInputRes != 0) { //打开不成功
        LOGE("avformat_open_input 失败 %s", av_err2str(formatOpenInputRes));

        goto _av_resource_destry;
    } else {
        LOGE("avformat_open_input 成功！");
    }

    //查找音频流
    int avformatFindStreamInfo = avformat_find_stream_info(pFormatContext, NULL);
    if (avformatFindStreamInfo < 0) {
        LOGE("avformatFindStreamInfo 失败 %s", av_err2str(avformatFindStreamInfo));
        goto _av_resource_destry;
    } else {
        LOGE("avformatFindStreamInfo 成功！");
    }

    int audioStreamIndex = -1;
    audioStreamIndex = av_find_best_stream(pFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1,
                                           NULL, 0);
    if (audioStreamIndex < 0) {
        LOGE("av_find_best_stream 失败 %s", av_err2str(audioStreamIndex));

        goto _av_resource_destry;
    } else {
        LOGE("av_find_best_stream 成功！");
    }

    //查找解码
    AVCodecParameters *pParameters = pFormatContext->streams[audioStreamIndex]->codecpar;
    AVCodec *pCodec = avcodec_find_decoder(pParameters->codec_id);
    if (pCodec == NULL) {
        LOGE("查找解码 失败!~");

        goto _av_resource_destry;
    } else {
        LOGE("查找解码 成功!~");
    }

    //打开解码器
    AVCodecContext *pContext = avcodec_alloc_context3(pCodec);
    int avcodec_parameters;
    avcodec_parameters = avcodec_parameters_to_context(pContext, pParameters);
    if (avcodec_parameters < 0) {
        LOGE("打开解码器avcodec_parameters_to_context 失败 %s", av_err2str(avcodec_parameters));

        goto _av_resource_destry;
    } else {
        LOGE("打开解码器avcodec_parameters_to_context 成功!~");
    }

    int avcodecOpen = avcodec_open2(pContext, pCodec, NULL);
    if (avcodecOpen < 0) {
        LOGE("打开解码器avcodec_open2  失败 %s", av_err2str(avcodecOpen));

        goto _av_resource_destry;
    } else {
        LOGE("打开解码器avcodec_open2  成功!~");
    }

    LOGE("音频参数：%d %d", pParameters->sample_rate, pParameters->channels);
    LOGE("采样率：%d, 通道数: %d", pParameters->sample_rate,pParameters->channels);

    LOGE("-----------------99------------------");
    jAudioTrack = createAudioTrack(env);
    LOGE("-----------------100-------------------");
    jWriteMedId = (*env)->GetMethodID(env, jAudioTrackClass, "write", "([BII)I");
    LOGE("-----------------101-------------------");

    int frameInde = 0;
    AVPacket *pPacket = av_packet_alloc();
    AVFrame *pFrame = av_frame_alloc();
    while (av_read_frame(pFormatContext, pPacket) >= 0) {
        if (pPacket->stream_index == audioStreamIndex) { //只处理音频数据

            int avcodecSendPacket = avcodec_send_packet(pContext, pPacket);
            if (avcodecSendPacket >= 0) {
                int avcodecReceiveFrame = avcodec_receive_frame(pContext, pFrame);
                if (avcodecReceiveFrame >= 0) {
                    frameInde++;
//                    pFrame->data   这个已近是pcm数据了

                    LOGE("解码音频：%d", frameInde);

                    //写到缓冲区  size 是pcm的大小
                    int dataSize = av_samples_get_buffer_size(NULL, pFrame->channels,
                                                              pFrame->nb_samples,
                                                              pContext->sample_fmt, 0);
                    LOGE("-----------------201------------------%d", dataSize);
                    jbyteArray jPcmDataArray = (*env)->NewByteArray(env, dataSize);
                    LOGE("-----------------202------------------");
                    jbyte *jpcmData = (*env)->GetByteArrayElements(env, jPcmDataArray, NULL);
                    LOGE("-----------------203------------------");
                    memcpy(jpcmData, pFrame->data, dataSize);
                    LOGE("-----------------204------------------");
                    (*env)->ReleaseByteArrayElements(env, jPcmDataArray, jpcmData, 0);
                    LOGE("-----------------205------------------");
                    (*env)->CallIntMethod(env, jAudioTrack, jWriteMedId, jPcmDataArray, 0,
                                          dataSize);
                    LOGE("-----------------206------------------");
                    if (jPcmDataArray == NULL || jpcmData == NULL) {
                        LOGE("--------------jPcmDataArray == NULL---------------------");
                        return;
                    }

                    (*env)->ReleaseByteArrayElements(env, jPcmDataArray, jpcmData, 0);

                    LOGE("-----------------207------------------");
                    (*env)->CallIntMethod(env, jAudioTrack, jWriteMedId,
                                          jPcmDataArray, 0, dataSize);
                    LOGE("-----------------208------------------");

                    (*env)->DeleteLocalRef(env,jPcmDataArray);
                }
            }
        }
        //解引用
        av_packet_unref(pPacket);
        av_frame_unref(pFrame);
    }

    if (jAudioTrack != NULL) {
        (*env)->DeleteLocalRef(env, jAudioTrack);
    }

    //解引用数据data  销毁pPacket对象  pPacket==null
    av_packet_unref(pPacket);
    av_frame_unref(pFrame);


//----------------------------------------------------------------
    _av_resource_destry:
    LOGE(" _av_resource_destry  释放资源");
    if (pFormatContext != NULL) {
        avformat_close_input(&pFormatContext);
        avformat_free_context(pFormatContext);
        pFormatContext = NULL;
    }

    avformat_network_deinit();

//    env->ReleaseStringUTFChar();

}