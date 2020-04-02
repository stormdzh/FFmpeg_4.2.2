//#include <jni.h>
//#include <malloc.h>
//
//#include "libavcodec/avcodec.h"
//#include "libavformat/avformat.h"
//#include "ffmpeg.h"
//#include <android/log.h>
//#include <pthread.h>
//
//
//void *readPacked(void *arg);
//void iter();
//
//#define TAG "jbl"
//#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
//
//
////全局变量
//JavaVM *g_jvm = NULL;
//jobject g_obj = NULL;
//char *pp;
//int ppsize;
//jclass MainActivity;
//
//JNIEXPORT jint
//Java_com_blplayer_FFmpegCmd_ffmpegRunCmd(
//        JNIEnv *env,
//        jclass job/* this */,  jobjectArray commands) {
//        //保存全局JVM以便在子线程中使用
////        LOGE("error is %s",av_err2str(-22));
////    iter();
//    AVCodec *avCodec=avcodec_find_encoder_by_name("libmp3lame");
//    if(avCodec){
//        LOGE("mp3编码器存在");
//    }else{
//        LOGE("mp3编码器不存在");
//    }
//  int argc = (*env)->GetArrayLength(env,commands);
//    char *argv[argc];
//
//    int i;
//    for (i = 0; i < argc; i++) {
//        jstring js = (jstring) (*env)->GetObjectArrayElement(env,commands, i);
//        argv[i] = (char*) (*env)->GetStringUTFChars(env,js, 0);
//    }
//    int ret =run(argc, argv);
//    if(ret){
//        LOGE("转换失败");
//    }else{
//        LOGE("转换成功");
//    }
//
//    return ret;
////    return 0;
//
//}
//void iter(){
//    char *info = (char *)malloc(40000);
//    memset(info, 0, 40000);
//
//    av_register_all();
//
//    AVCodec *c_temp = av_codec_next(NULL);
//
//    while (c_temp != NULL)
//    {
//        if (c_temp->decode != NULL)
//        {
//            strcat(info, "[Decode]");
//        }
//        else
//        {
//            strcat(info, "[Encode]");
//            switch (c_temp->type)
//            {
//                case AVMEDIA_TYPE_VIDEO:
//                    strcat(info, "[Video]");
//                    break;
//                case AVMEDIA_TYPE_AUDIO:
//                    strcat(info, "[Audeo]");
//                    LOGE("%s", c_temp->name);
//                    break;
//                default:
//                    strcat(info, "[Other]");
//                    break;
//            }
//        }
//
//
//      //  LOGE(info, "%s %10s\n", info, c_temp->name);
//        c_temp = c_temp->next;
//    }
//    puts(info);
//    free(info);
//}
//
