//
//  CloudClientSDKHelper.h
//  CloudClientSDKDemo
//
//  Created by Jason on 2020/8/10.
//  Copyright © 2020 Jason. All rights reserved.
//

#ifndef CloudClientSDKCommon_h
#define CloudClientSDKCommon_h

#import "CloudSDKCommon.h"

typedef enum : short {
    REALTIME_VIDEO_DATA = 1,//预览流
    REALTIME_AUDIO_DATA,//预览音频流
    INTERCOM_DATA ,//对讲流 (设备发来的音频流)
     RECORD_VIDEO_DATA,//回放视频流
    RECORD_AUDIO_DATA,//回放音频流
    ALARMV2_DATA//报警消息流
} STREAM_DATA_TYPE;

typedef enum : short {
    MAIN_STREAM = 1,
    SUB_STREAM
} STREAM_TYPE;


//!
/*
 CloudSdk 实时流回调Buffer1 头部结构体 按照结构强转数据
 音频、回放、预览、对讲 所对应的数据结构类型不同 有需要的数据 进行按位取
 例如:S16 channel = *(S16 *)(lpBuffer1+0);//S16 表示2个Byte = 16bit
      STREAM_TYPE streamType = *(STREAM_TYPE *)(lpBuffer1+2);
 */
typedef struct
{
    S16 channel; //当前数据流的通道号
    STREAM_DATA_TYPE stream_data_type; //数据类型
} CLOUDSDK_HELPER_HEADER1,*LPCLOUDSDK_HELPER_HEADER1;


/*
    视频帧信息
 */
typedef  struct _tagVideoHeader{
    unsigned short usWidth;                //!视频宽度
    unsigned short usHeight;                //!视频高度
    char cCodecId;                //!视频编码类型
    char cColorSpace;         // 0-yuv420,1-yuv422,2-444
    char cResv[2];                    //!保留位
}AntsVideoHeader,*pAntsVideoHeader;


typedef  struct _tagAudioHeader{
    char cCodecId;            //!音频编码类型
    char cSampleRate;            //!采样率
    char cBitRate;                //!比特率
    char cChannels;            //!通道数
    char cResolution;            //!分辨力
    char cResv[3];            //!保留位
}AntsAudioHeader,*pAntsAudioHeader;
/*
   视频帧信息
*/
typedef  struct _tagANTSAPPHeader{
    unsigned char byAppPayloadType;                //!APP 负载类型
    unsigned char byAppNameLen;                 //! APP 名称长度,接在帧头之后，数据之前,最大255个字节 nul结束
    unsigned char byRes[6];
}AntsAppHeader,*pAntsAppHeader;


typedef struct _tagSmartHeader{
    DWORD ivs_node_num;
    char recv[4];
}ANTS_SMART_HEADER,*LPANTS_SMART_HEADER;


typedef  struct _tagAntsFrameHeader{
    unsigned int uiStartId;                    //!帧同步头
    unsigned int uiFrameType;                //!帧类型
    unsigned int uiFrameNo;                //!帧号
    unsigned int uiFrameTime;                //!UTC时间
    unsigned int uiFrameTickCount;            //!微秒为单位的毫秒时间
    unsigned int uiFrameLen;                //!帧载长度
    //!联合体,用于存储音频帧或是视频帧信息
    union {
        AntsAudioHeader struAudioHeader;    //!音频帧信息
        AntsVideoHeader struVideoHeader;    //!视频帧信息
        AntsAppHeader   struAppHeader;
        ANTS_SMART_HEADER smart_header;     //!智能帧信息
    }uMedia; // 8bytes
    //unsigned char ucReserve[4];                //!保留位
    unsigned int uiTimeStamp;
}AntsFrameHeader,*pAntsFrameHeader;


typedef enum {
    //!主码流帧类型
    AntsPktError=0x0,
    AntsPktIFrames=0x01,
    AntsPktAudioFrames=0x08,
    AntsPktPFrames=0x09,
    AntsPktBBPFrames=0x0a,
    AntsPktMotionDetection=0x0b,
    AntsPktDspStatus=0x0c,
    AntsPktOrigImage=0x0d,
    AntsPktSysHeader=0x0e,
    AntsPktBPFrames=0x0f,
    AntsPktSFrames=0x10,
    //!子码流帧类型
    AntsPktSubSysHeader=0x11,
    AntsPktSubIFrames=0x12,
    AntsPktSubPFrames=0x13,
    AntsPktSubBBPFrames=0x14,
    //!智能分析信息帧类型
    AntsPktVacEventZones=0x15,
    AntsPktVacObjects=0x16,
    //zhangw:这是在接入智能帧数据时加入的
    //!第三码流帧类型
    AntsPktThirdSysHeader=0x17,
    AntsPktThirdIFrames=0x18,
    AntsPktThirdPFrames=0x19,
    AntsPktThirdBBPFrames=0x1a,
    
    //!智能检测帧类型
    AntsPktSmartIFrames=0x1b,
    AntsPktSmartPFrames=0x1c,
    // APP metedata帧.
    AntsPktAppFrames=0x20,
    // AntsComb 帧
    AntsPktAntsCombFrames=0x21,
    
    AntsPkt_VIFreame = 0xa1,
    AntsPkt_SubVIFreame = 0xa2,
    AntsPkt_ThirdVIFreame = 0xa8,

}eAntsFrameType;


typedef enum {
    AntsH264_hisi=1,//base/main
    AntsMJPEG_hisi=2,
    AntsH264_advance=3,
    AntsH264_hisi_high=4,//high
    AntsH264_hisi_RTP=8,//high
    AntsH264_RTP_PACK=9, //
    AntsH265=16, //
    AntsH265_RTP_PACK=17, //
    AntsSVAC=20, //
}eAntsVideoCodecId;


#endif /* CloudClientSDKCommon_h */
