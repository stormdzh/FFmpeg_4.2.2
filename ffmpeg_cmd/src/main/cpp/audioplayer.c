#include <jni.h>
#include "ffmpeg.h"
#include "libavformat/avformat.h"
#include <string.h>
#include <android/log.h>
#include <unistd.h>
/*
 * Class:     我自己写的音频播放类
 * Method:    run
 * Signature: (Ljava/lang/String;)I
 */

#define TAG "dzh"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

jobject jAudioTrack;
jmethodID jWriteMedId;

/**
 * 将 String 类型转成 char*
 * @param env  env
 * @param jstr  java字符串
 * @return  c字符串
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
 * 将char* 转成 其他类型
 * @param env  env
 * @param pat  pat
 * @return
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


/**
 * 在jni中创建AudioTrack
 * @param env
 * @return
 */
jobject createAudioTrack(JNIEnv *env) {

    //获取AudioTrack类字节码
    jclass jAudioTrackClass = (*env)->FindClass(env, "android/media/AudioTrack");
    //获取到new方法
    jmethodID jAudioTrackMedId = (*env)->GetMethodID(env, jAudioTrackClass, "<init>", "(IIIIII)V");

    //设置参数
    int streamType = 3;
    int sampleRateInHz = 44100;
    int channelConfig = (0x4 | 0x8);
    int audioFormat = 2;
    int mode = 1;

    //获取getMinBufferSize方法的id 参数：int getMinBufferSize(int sampleRateInHz, int channelConfig, int audioFormat)
    jmethodID getMinBufferSizeId = (*env)->GetStaticMethodID(env, jAudioTrackClass,
                                                             "getMinBufferSize",
                                                             "(III)I");
    //通过调用getMinBufferSize方法获取minBufferSize的值
    int minBufferSize = (*env)->CallStaticIntMethod(env, jAudioTrackClass, getMinBufferSizeId,
                                                    sampleRateInHz,
                                                    channelConfig,
                                                    audioFormat);

    if (minBufferSize <= 0) {
        LOGE("调用getMinBufferSize没有获取到音频正常长度！");
        return NULL;
    }

    //获取AudioTrack对象 参数：AudioTrack(int streamType, int sampleRateInHz, int channelConfig, int audioFormat, int bufferSizeInBytes, int mode)
    jobject jAudioTrackObj = (*env)->NewObject(env, jAudioTrackClass, jAudioTrackMedId,
                                               streamType, sampleRateInHz,
                                               channelConfig, audioFormat, minBufferSize, mode);

    //获取播放方法的id
    jmethodID playMedid = (*env)->GetMethodID(env, jAudioTrackClass, "play", "()V");
    //调用播放方法
    (*env)->CallVoidMethod(env, jAudioTrackObj, playMedid);
    //获取write方法的id
    jWriteMedId = (*env)->GetMethodID(env, jAudioTrackClass, "write", "([BII)I");

    return jAudioTrackObj;
}


JNIEXPORT void JNICALL
Java_com_stormdzh_ffmpeg_sdk_audio_AudioPlayer_start(JNIEnv *env, jobject thiz, jstring jurl) {
    // TODO: implement start()
    const char *url = Jstring2CStr(env, jurl);
    LOGE("已经进入ini方法了！播放地址是：%s", url);
    //注册
    av_register_all();
    //初始化网络
    avformat_network_init();

    AVFormatContext *pFormatContext = NULL;

    //打开输入流文件
    int formatOpenInputRes = avformat_open_input(&pFormatContext, url, NULL, NULL);
    if (formatOpenInputRes != 0) { //打开不成功
        LOGE("打开文件 失败 %s", av_err2str(formatOpenInputRes));
        goto _av_resource_destry;
    } else {
        LOGE("打开文件 成功！");
    }

    //查找音频流信息
    int avformatFindStreamInfo = avformat_find_stream_info(pFormatContext, NULL);
    if (avformatFindStreamInfo < 0) {
        LOGE("查找音频流信息 失败 %s", av_err2str(avformatFindStreamInfo));
        goto _av_resource_destry;
    } else {
        LOGE("查找音频流信息 成功！");
    }
    //获取正确的音频流
    int audioStreamIndex = av_find_best_stream(pFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1,
                                               NULL, 0);
    if (audioStreamIndex < 0) {
        LOGE("获取正确的音频流 失败 %s", av_err2str(audioStreamIndex));
        goto _av_resource_destry;
    } else {
        LOGE("获取正确的音频流 成功！");
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

    //音频参数拷贝到AVCodecContext中
    AVCodecContext *pContext = avcodec_alloc_context3(pCodec);
    int avcodec_parameters = avcodec_parameters_to_context(pContext, pParameters);
    if (avcodec_parameters < 0) {
        LOGE("拷贝参数 失败 %s", av_err2str(avcodec_parameters));
        goto _av_resource_destry;
    } else {
        LOGE("拷贝参数 成功!~");
    }

    //打开解码器
    int avcodecOpen = avcodec_open2(pContext, pCodec, NULL);
    if (avcodecOpen < 0) {
        LOGE("打开解码器 失败 %s", av_err2str(avcodecOpen));
        goto _av_resource_destry;
    } else {
        LOGE("打开解码器  成功!~");
    }
    //数据目前的几个音频参数
    LOGE("采样率：%d, 通道数: %d   format:%d", pParameters->sample_rate, pParameters->channels,
         pParameters->format);


    // ---------- 重采样 start ----------

    int out_ch_layout = AV_CH_LAYOUT_STEREO;
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    int out_sample_rate = 44100;

    int in_ch_layout = pContext->channel_layout;
    enum AVSampleFormat in_sample_fmt = pContext->sample_fmt;
    int in_sample_rate = pContext->sample_rate;

    LOGE("重采样数据  in==> in_ch_layout：%d, in_sample_fmt: %d   in_sample_rate:%d", in_ch_layout,
         in_sample_fmt, in_sample_rate);
    LOGE("重采样数据 out==> out_ch_layout：%d, out_sample_fmt: %d   out_sample_rate:%d", out_ch_layout,
         out_sample_fmt, out_sample_rate);
    //重采样的参数
    SwrContext *swrContext = swr_alloc_set_opts(NULL, out_ch_layout, out_sample_fmt,
                                                out_sample_rate, in_ch_layout, in_sample_fmt,
                                                in_sample_rate, 0, NULL);
    if (swrContext == NULL) {
        LOGE("设置重采样参数 失败!~");
        goto _av_resource_destry;
    }

    int swrInitRes = swr_init(swrContext);
    if (swrInitRes < 0) {
        LOGE("重采样初始化 失败!~");
        goto _av_resource_destry;
    }
    // size 是播放指定的大小，是最终输出的大小
    int outChannels = av_get_channel_layout_nb_channels(out_ch_layout);
    LOGE("重采样outChannels==> out_ch_layout:%d  outChannels:%d", out_ch_layout, outChannels);
    int dataSize = av_samples_get_buffer_size(NULL, outChannels, pContext->frame_size,
                                              out_sample_fmt, 0);
    LOGE("重采样==> dataSize:%d ", dataSize);
    uint8_t *resampleOutBuffer = (uint8_t *) malloc(dataSize);

    // ---------- 重采样 end ----------

    jbyteArray jPcmByteArray = (*env)->NewByteArray(env, dataSize);
    // native 创建 c 数组
    jbyte *jPcmData = (*env)->GetByteArrayElements(env, jPcmByteArray, NULL);

    //初始化AudioTrack
    jAudioTrack = createAudioTrack(env);
    if (jAudioTrack == NULL) {
        goto _av_resource_destry;
    }

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
                    //打印参数 pFrame->data   这个已近是pcm数据了
                    //LOGE("解码音频：channels:%d  nb_samples:%d   sample_fmt:%d", frameInde,
                    //    pFrame->channels, pFrame->nb_samples, pContext->sample_fmt);

                    // 调用重采样的方法
                    swr_convert(swrContext, &resampleOutBuffer, pFrame->nb_samples,
                                (const uint8_t **) pFrame->data, pFrame->nb_samples);

                    //拷贝数据
                    // write 写到缓冲区 pFrame.data -> javabyte
                    // size 是多大，装 pcm 的数据
                    // 1s 44100 点  2通道 ，2字节    44100*2*2
                    // 1帧不是一秒，pFrame->nb_samples点
                    memcpy(jPcmData, resampleOutBuffer, dataSize);
                    // 0 把 c 的数组的数据同步到 jbyteArray , 然后释放native数组
                    (*env)->ReleaseByteArrayElements(env, jPcmByteArray, jPcmData, JNI_COMMIT);
                    //调用AudioTrack方法
                    (*env)->CallIntMethod(env, jAudioTrack, jWriteMedId,
                                          jPcmByteArray, 0, dataSize);
                }
            }
        }
        //解引用
        av_packet_unref(pPacket);
        av_frame_unref(pFrame);
    }

    //销毁AudioTrack实例
    if (jAudioTrack != NULL) {
        (*env)->DeleteLocalRef(env, jAudioTrack);
        jAudioTrack = NULL;
    }

    //jPcmByteArray
    (*env)->DeleteLocalRef(env, jPcmByteArray);

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

}