//
//  VideoLayer.m
//  ANTSCMS2_0Demo
//
//  Created by ants-mac on 16/5/4.
//  Copyright © 2016年 www.yurfly.com. All rights reserved.
//  修改记录:20190718 -- Jason
//  新增了 回放音频播放的时候 协调音视频的发送帧率 防止音频卡顿和 卡死问题解决

#import "VideoLayer.h"
#import <sys/time.h>
#import <string.h>
#import <list>
#import <vector>
#import <VideoToolbox/VideoToolbox.h>
#import <VideoToolbox/VTErrors.h>
#include <sys/param.h>
#include <sys/mount.h>

#define MAX_VIDEO_BUF_NUM  25
#define MAX_VIDEO_WARN_NUM 20
#define MAX_TCOUNT 1000

#define CONTAINER_OF(ptr, type, field) ((type *) ((char *) (ptr) - ((char *) &((type *) 0)->field)))
#define SIZE_ROUNDUP4(size) (((size) % 4) ? (size) + (4 - ((size) % 4)) : (size))

UIKIT_EXTERN NSString *const ANTSCloudSDKSnapshotFailedNotification = @"ANTSCloudSDKSnapshotFailedNotification";

@interface VideoLayer()
{
    long lHandle;
}
enum
{
    NAL_UNKNOWN   = 0,
    NAL_SLICE     = 1,
    NAL_SLICE_DPA = 2,
    NAL_SLICE_DPB = 3,
    NAL_SLICE_DPC = 4,
    NAL_SLICE_IDR = 5,
    NAL_SEI       = 6,
    NAL_SPS       = 7,
    NAL_PPS       = 8 ,
    NAL_AUD       = 9,
    NAL_FILLER    = 12,
    
    HEVC_NAL_VPS            = 32,
    HEVC_NAL_SPS            = 33,
    HEVC_NAL_PPS            = 34 ,
    HEVC_PICTURE_DELIMITER  = 35,
    HEVC_NAL_EOS            = 36,
    HEVC_NAL_EOB            = 37,
    HEVC_NAL_FILTER_DATA    = 38,
    HEVC_NAL_SEI            = 39,
    HEVC_NAL_SEI_SUFFIX     = 40,
};

enum{
    NAL_PRIORITY_DISPOSABLE = 0,
    NAL_PRIORITY_LOW        = 1,
    NAL_PRIORITY_HIGH       = 2,
    NAL_PRIORITY_HIGHEST    = 3,
};


+(const uint8_t *)avc_find_startcode:(const uint8_t *)p  end:(const uint8_t *)end;
+(const uint8_t *)ff_avc_find_startcode_internal:(const uint8_t *)p  end:(const uint8_t *)end;
-(const uint8_t *)change_startcode_to_size:(const uint8_t *)data  size:(size_t)size out_size:(size_t*)out_size;
+(void)get_sps_pps:(const uint8_t *)data  size:(size_t)size sps:(const uint8_t **)sps sps_size:(size_t*)sps_size  pps:(const uint8_t **)pps pps_size:(size_t*)pps_size ;
+(void)get_sps_pps_vps4hevc:(const uint8_t *)data  size:(size_t)size sps:(const uint8_t **)sps sps_size:(size_t*)sps_size  pps:(const uint8_t **)pps pps_size:(size_t*)pps_size vps:(const uint8_t **)vps vps_size:(size_t*)vps_size;

-(BOOL)ReleaseFrame:(unsigned char *)lpBuffer ;
-(BOOL)ReadOneFrame:(int) nType lpOutBuff:(unsigned char **)lpOutBuff lpOutSize :(unsigned int*)lpOutSize ;
-(BOOL)DecodeOneFrame: (unsigned char *)lpBuff nBuffSize :(unsigned int)nBuffSize  n64PreAbsTime:(int64_t)n64PreAbsTime n64CurAbsTime:(int64_t)n64CurAbsTime;
-(void)DecodeVideoProc ;
-(void)StartPlayVideoThread ;
-(void)StopPlayVideoThread ;
-(void)ResetTimebase:(int) nType nSpeed :(int) nSpeed;
-(void)HandleCapture:(uint8_t *)lpFrame nSize:(size_t)nSize ;
-(void)DoCapture:(uint8_t *)lpIFrame nSize:(size_t)nSize lpSPS:(uint8_t*)lpSPS  nSPSSize:(size_t)nSPSSize lpPPS:(uint8_t*)lpPPS nPPSSize:(size_t)nPPSSize lpVPS:(uint8_t*)lpVPS nVPSSize:(size_t)nVPSSize;

@property (assign, nonatomic) BOOL bVideoThreadExitFlag;
@property (assign,nonatomic)  BOOL bResetTimebase;
@property (assign, nonatomic) BOOL bPause;
@property (assign, nonatomic) int nCaptureJPEG;
@property (assign, nonatomic) int nPlayType;
@property (assign, nonatomic) int nSpeed;
@property (assign, nonatomic) int nLastSpeed;
@property (assign, nonatomic) unsigned int nLastFrameNo;
@property (assign, nonatomic) unsigned int nPreFrameNo;
@property (assign, nonatomic) int64_t nPreRelativeTime;
@property (assign, nonatomic) int64_t n64StartPTS;
@property (assign, nonatomic) int64_t n64AbsPTS;
@property (assign, nonatomic) int64_t n64SyncPTS;
@property (assign, nonatomic) unsigned char *lpCaptureIframe;
@property (assign, nonatomic) unsigned char *lpDecodeFrame;
@property (assign, nonatomic) CMVideoFormatDescriptionRef lpVideoFormatDescr;  //视频格式描述
@property (nonatomic, nonatomic) std::list<BuffNode*> lsVideoBuff;
@property (nonatomic, nonatomic) std::vector<uint8_t*> vecNalBuff;
@property (nonatomic, nonatomic) std::vector<uint32_t> vecNalBuffSize;
@property (readwrite, nonatomic, strong) NSLock *videoBuffLock;
@property (readwrite, nonatomic, strong) NSLock *syncTimeLock;
@property (nonatomic,strong) NSThread * decodeThread;

@property (nonatomic,strong) NSLock * speedLock;

@property (nonatomic,assign) int mFrameRate;

@property (nonatomic,strong) NSTimer * cycleRecordTimer;

@property (nonatomic,assign) NSTimeInterval devodeErrorTimeInt;

@property (nonatomic,assign ) int decodeErrorTimes;

@property (nonatomic,assign) int nFFMPEGCaptureJPEG;

@property (nonatomic,copy) NSString * fileNameByFFMPEG;

@property (assign, nonatomic) BOOL bVideoDecodeFlag;

@property (nonatomic,assign) BuffNode * iFrameBuffNode;

@property (nonatomic,strong) NSLock * iFrameLock;

@end
//类实现

@implementation VideoLayer

+(const uint8_t *)avc_find_startcode:(const uint8_t *)p  end:(const uint8_t *)end
{
    const uint8_t * outP= [VideoLayer ff_avc_find_startcode_internal:p end:end];
    if (p < outP && outP < end && !outP[-1]) outP--;
    return outP;
}

+(const uint8_t *)ff_avc_find_startcode_internal:(const uint8_t *)p  end:(const uint8_t *)end
{
    const uint8_t *a = p + 4 - ((intptr_t)p & 3);
    
    for (end -= 3; p < a && p < end; p++) {
        if (p[0] == 0 && p[1] == 0 && p[2] == 1)
            return p;
    }
    
    for (end -= 3; p < end; p += 4) {
        uint32_t x = *(const uint32_t*)p;
        
        if ((x - 0x01010101) & (~x) & 0x80808080) {//判断四字节整数中有一个为0
            if (p[1] == 0) {
                if (p[0] == 0 && p[2] == 1)
                    return p;
                if (p[2] == 0 && p[3] == 1)
                    return p+1;
            }
            
            if (p[3] == 0) {
                if (p[2] == 0 && p[4] == 1)
                    return p+2;
                if (p[4] == 0 && p[5] == 1)
                    return p+3;
            }
        }
    }
    
    for (end += 3; p < end; p++) {
        if (p[0] == 0 && p[1] == 0 && p[2] == 1)
            return p;
    }
    
    return end + 3;
}


+(void)get_sps_pps:(const uint8_t *)data  size:(size_t)size sps:(const uint8_t **)sps sps_size:(size_t*)sps_size  pps:(const uint8_t **)pps pps_size:(size_t*)pps_size
{
    const uint8_t *nal_start, *nal_end;
    const uint8_t *end = data+size;
    int type;
    
    nal_start = [VideoLayer avc_find_startcode:data end:end];
    
    while (true) {
        while (nal_start < end && !*(nal_start++));
        
        if (nal_start == end)
            break;
        
        nal_end = [VideoLayer avc_find_startcode:nal_start end:end];
        
        type = nal_start[0] & 0x1F;
        if (type == NAL_SPS) {
            *sps = nal_start;
            *sps_size = nal_end - nal_start;
        } else if (type == NAL_PPS) {
            *pps = nal_start;
            *pps_size = nal_end - nal_start;
        }

        nal_start = nal_end;
    }
}


+(void)get_sps_pps_vps4hevc:(const uint8_t *)data  size:(size_t)size sps:(const uint8_t **)sps sps_size:(size_t*)sps_size  pps:(const uint8_t **)pps pps_size:(size_t*)pps_size vps:(const uint8_t **)vps vps_size:(size_t*)vps_size
{
    const uint8_t *nal_start, *nal_end;
    const uint8_t *end = data+size;
    int type;
    
    nal_start = [VideoLayer avc_find_startcode:data end:end];
    
    while (true) {
        while (nal_start < end && !*(nal_start++));
        
        if (nal_start == end)
            break;
        
        nal_end = [VideoLayer avc_find_startcode:nal_start end:end];
        
        type = (nal_start[0] >> 1) & 63;
        if (type == HEVC_NAL_SPS) {
            *sps = nal_start;
            *sps_size = nal_end - nal_start;
        } else if (type == HEVC_NAL_PPS) {
            *pps = nal_start;
            *pps_size = nal_end - nal_start;
        }else if (type == HEVC_NAL_VPS) {
            *vps = nal_start;
            *vps_size = nal_end - nal_start;
        }
        
        nal_start = nal_end;
    }
}


-(const uint8_t *)change_startcode_to_size:(const uint8_t *)data  size:(size_t)size out_size:(size_t*)out_size
{
    const uint8_t *nal_start, *nal_end;
    const uint8_t *end = data+size;
    int nStartCodeLen = 0 ;
    uint32_t nNalDataSize =  0;
    uint32_t nWriteSize =  0;
    BOOL b3ByteStartCode = false;
    
    _vecNalBuff.clear() ;
    _vecNalBuffSize.clear() ;
    
    nal_start = [VideoLayer avc_find_startcode:data end:end];
    
    while (true) {
        nStartCodeLen = 0 ;
        while (nal_start < end )
        {
            if(!*(nal_start++))
            {
                nStartCodeLen++ ;
                continue ;
            }
            else
            {
                nStartCodeLen++ ;
                break ;
            }
        }
    
        if (nal_start == end)
            break;
    
        if(nStartCodeLen == 3)
        {
            b3ByteStartCode = true ;
        }
        
        nal_end = [VideoLayer avc_find_startcode:nal_start end:end];
        
        _vecNalBuff.push_back((uint8_t *)nal_start);
        
        _vecNalBuffSize.push_back((uint32_t)(nal_end - nal_start)) ;
        
        nNalDataSize += (nal_end - nal_start) ;
        
        nal_start = nal_end;
    }
    
    if(!b3ByteStartCode)
    {
        for(unsigned int nIndex = 0 ; nIndex < _vecNalBuff.size(); nIndex++)
        {
          uint32_t nDataLen = htonl(_vecNalBuffSize[nIndex]);
            
          memcpy((void*)(_vecNalBuff[nIndex] - 4), (void*)&nDataLen, sizeof(uint32_t));
        }
        
        *out_size = size ;
        
        return data ;
    }
    else
    {
        if(nNalDataSize + 4 * _vecNalBuff.size() > 1024 *1024)
        {
            if(_lpDecodeFrame != NULL)
            {
                free(_lpDecodeFrame) ;
                
                _lpDecodeFrame = (unsigned char*)malloc(nNalDataSize + 4 * _vecNalBuff.size()) ;
            }
        }
        
        if(_lpDecodeFrame != NULL)
        {
            for(unsigned int nIndex = 0 ; nIndex < _vecNalBuff.size(); nIndex++)
            {
                uint32_t nDataLen = htonl(_vecNalBuffSize[nIndex]);
                
                memcpy(_lpDecodeFrame + nWriteSize, (void*)&nDataLen, sizeof(uint32_t));
                
                nWriteSize += sizeof(uint32_t) ;
                
                memcpy(_lpDecodeFrame + nWriteSize, _vecNalBuff[nIndex], _vecNalBuffSize[nIndex]);
                
                nWriteSize += _vecNalBuffSize[nIndex] ;
            }
            
            *out_size = (size_t)(nNalDataSize + 4 * _vecNalBuff.size());
            
            return _lpDecodeFrame ;
        }
        
    }
    
    *out_size = 0 ;
    
    return NULL ;
}


- (instancetype)init
{
    NSLog(@"VideoLayer--init---%s",__FUNCTION__);

    self = [super init];
    
    if(self)
    {
        self.handleRef = &lHandle;
        
        self.videoGravity = AVLayerVideoGravityResize;
        
        self.bResetTimebase = YES;
        
        self.bFirstFrame = true ;
        
        self.nCodecId = 0 ;
        
        self.nVideoWidth = 0 ;
        
        self.nVideoHeight = 0 ;
        
        self.nPreFrameNo = 0 ;
        
        self.nPreRelativeTime = 0 ;
        
        self.nPlayType = 0 ;
        
        self.nSpeed = 0 ;
        
        self.nLastSpeed = 0 ;
        
        self.n64AbsPTS = 0 ;
        
        self.n64SyncPTS = 0 ;
        
        self.bPause = false ;
        
        self.nCaptureJPEG = 0 ;
        
        self.lpCaptureIframe = (unsigned char *)malloc(1024*1024);
        
        self.lpDecodeFrame = (unsigned char *)malloc(1024*1024);
        
        self.videoBuffLock = [[NSLock alloc] init];
        
        self.syncTimeLock = [[NSLock alloc] init];
                
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(failedToDecodeNotify:) name:AVSampleBufferDisplayLayerFailedToDecodeNotification object:nil];
        
        self.speedLock = [NSLock new];
        
        self.isVideoRecording = NO;
        
        self.bDevodeFlag = FALSE;
        
        self.decodeErrorTimes = 0;
        
        self.iFrameLock = [NSLock new];
        
        _iFrameBuffNode = NULL;
        
        //NSLog(@"VideoLayer init ============================= %p",self);
    }
    
    return self;
}


- (void)dealloc
{
    if(_iFrameBuffNode != NULL){
        free(_iFrameBuffNode);
        _iFrameBuffNode = NULL;
    }
    
    [[NSNotificationCenter defaultCenter] removeObserver:self name:AVSampleBufferDisplayLayerFailedToDecodeNotification object:nil];

    NSLog(@"VideoLayer dealloc =========================== %p",self);
}


-(void)UnInitDecoder
{
    if(!self.bVideoDecodeFlag)
    {
        return ;
    }
    
    [self StopPlayVideoThread] ;
    
    if (self.lpVideoFormatDescr)
    {
        CFRelease(_lpVideoFormatDescr);
        
        self.lpVideoFormatDescr = NULL;
    }
    
    if(self.lpCaptureIframe)
    {
        free(_lpCaptureIframe);
        
        self.lpCaptureIframe = NULL ;
    }
    
    if(self.lpDecodeFrame)
    {
        free(_lpDecodeFrame);
        
        self.lpDecodeFrame = NULL ;
    }
}


- (void)failedToDecodeNotify:(NSNotification *)notification
{
    if(self.devodeErrorTimeInt == 0)
    {
        self.devodeErrorTimeInt = [[NSDate date] timeIntervalSince1970];
    }else
    {
        if(([NSDate date].timeIntervalSince1970 - self.devodeErrorTimeInt < 2) &&  self.decodeErrorTimes > 15)
        {
            self.decodeErrorTimes = 0;
            
            self.devodeErrorTimeInt = 0;
            
            return;
        }
    }
    
    self.decodeErrorTimes ++;
    
    NSLog(@"failedToDecodeNotify:%@",notification);
}

- (void)ResetTimebase:(int) nType nSpeed :(int) nSpeed
{
    //CMTimebase：时间控制器，可以设置rate和time；
    CMTimebaseRef controlTimebase ;
    
    CMTimebaseCreateWithMasterClock(CFAllocatorGetDefault(), CMClockGetHostTimeClock(), &controlTimebase);
    
//    self.controlTimebase = controlTimebase;
    
    CMTimebaseSetTime(controlTimebase, kCMTimeZero);  //时间的映射
    
    CMTimebaseSetRate(controlTimebase, 1.0 * pow((float)2, nSpeed) );           //速率控制
    
    CFRelease(controlTimebase);
    
//    self.controlTimebase = nil;
    
    if(nType == 0)
    {
        NSLog(@"ResetTimebase \r\n ") ;
        [self flush];
    }
    else
    {
        if(nSpeed >= 3)
        {
             [self flush];
        }
//        NSLog(@"speed:%d rate:%f\r\n", nSpeed, 1.0 * pow((float)2, nSpeed)) ;
    }
    
    self.bResetTimebase = false;
}


-(void)InitDecoder:(int) nType
{
    if(self.bVideoDecodeFlag)
    {
        return ;
    }
    
    if(nType == 0 || nType == 1)
    {
        self.nPlayType = nType ;
    }
    
    [self StartPlayVideoThread] ;
}


-(void)StartPlayVideoThread
{
    self.bVideoDecodeFlag = true ;

    self.bVideoThreadExitFlag = false ;

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        [self DecodeVideoProc];
        
    });
    
//    self.decodeThread  = [[NSThread alloc] initWithTarget:self selector:@selector(DecodeVideoProc) object:nil];
//
//    [self.decodeThread setQualityOfService:NSQualityOfServiceUserInteractive];
//
//    [self.decodeThread start];
}


-(void)StopPlayVideoThread
{
    self.bVideoDecodeFlag = false ;

    if(!self.bVideoThreadExitFlag)
    {
        NSLog(@"StopPlayVideoThread--%@",[NSThread currentThread]);

        [self.decodeThread cancel];
    }

    self.bVideoThreadExitFlag = true ;

    [self ClearAllFrames] ;
}


-(void)SetPlaySpeed:(int) nSpeed
{
    [self flush];
    //改变速度 重新刷新解码器？
    if(self.nPlayType == 1)
    {
        [self.speedLock lock];
        
        self.nSpeed = nSpeed ;
        
        [self.speedLock unlock];
    }
}


-(void)SetPlayPause:(BOOL) bPause
{
    self.bPause = bPause ;
}


-(BOOL)AddOneFrame:(int) nType lpBuff:(unsigned char *)lpBuff nBuffSize:(unsigned int)nBuffSize frameRate:(int)frameRate
{
    BOOL bRet = false ;
    BuffNode *lpNode = NULL ;
    AntsFrameHeader *lpFrameHeader = NULL ;
    
    self.mFrameRate = frameRate ==0?25:frameRate;
    
    if(lpBuff == NULL || nBuffSize <= 0)
    {
        return bRet ;
    }
    
    if(!self.bVideoDecodeFlag)
    {
        return true;
    }

    lpFrameHeader = (AntsFrameHeader*)lpBuff ;
    
    //video buffer
    if(nType == 0) {
        
        if(lpFrameHeader->uiFrameType == AntsPktIFrames \
           || lpFrameHeader->uiFrameType == AntsPktSubIFrames){
                //这里需要写一帧i帧
            [self.iFrameLock lock];

            if(_iFrameBuffNode != NULL){
                free(_iFrameBuffNode);
                _iFrameBuffNode = NULL;
            }

            _iFrameBuffNode = (BuffNode*)malloc(sizeof(BuffNode) + nBuffSize * sizeof(unsigned char)) ;

            if(_iFrameBuffNode){
                    //拷贝
                memcpy(_iFrameBuffNode->lpBuff, lpBuff, nBuffSize);

                _iFrameBuffNode->nBuffLen = nBuffSize;
            }

            [self.iFrameLock unlock];
        }
        
        [self.speedLock lock];
        
        if(self.nSpeed >= 3) //>=8倍速只接受I帧数据
        {
            [self.speedLock unlock];
            
            if(lpFrameHeader->uiFrameType != AntsPktIFrames && lpFrameHeader->uiFrameType != AntsPktSubIFrames && lpFrameHeader->uiFrameType != AntsPkt_VIFreame && lpFrameHeader->uiFrameType != AntsPkt_SubVIFreame && lpFrameHeader->uiFrameType != AntsPkt_ThirdVIFreame)
            {
                //过滤非视频帧数据
                return true ;
            }
        }
        
        [self.speedLock unlock];
        
        [self.videoBuffLock lock] ;
       
        if(_lsVideoBuff.size() >= MAX_VIDEO_BUF_NUM){
            [self.videoBuffLock unlock] ;
            
            return bRet ;
        }
        else
        {
             [self.videoBuffLock unlock] ;
        }
        
        lpNode = (BuffNode*)malloc(sizeof(BuffNode) + nBuffSize * sizeof(unsigned char)) ;
        
        if(lpNode != NULL)
        {
            lpNode->nBuffLen = nBuffSize ;
            
            memcpy(lpNode->lpBuff, lpBuff, nBuffSize) ;
            
            bRet = true ;
            
            [self.videoBuffLock lock] ;
            
            _lsVideoBuff.push_back(lpNode) ;
            
            [self.videoBuffLock unlock] ;
        }
        
        return bRet ;
    }
    else if(nType == 1) //audio buffer
    {
        return bRet ;
    }
    
    return bRet ;
}


-(BOOL)ReadOneFrame:(int) nType lpOutBuff:(unsigned char **)lpOutBuff lpOutSize :(unsigned int*)lpOutSize
{
    BOOL bRet = false ;
    BuffNode *lpNode = NULL ;
    
    if(lpOutBuff == NULL || lpOutSize == NULL || !self.bVideoDecodeFlag)
    {
        return bRet ;
    }
    
    if(nType == 0) //Video
    {
        [self.videoBuffLock lock] ;
        
        if(_lsVideoBuff.size() <= 0)
        {
            [self.videoBuffLock unlock] ;
        }
        else
        {
            lpNode = _lsVideoBuff.front() ;
            
            _lsVideoBuff.pop_front() ;
            
            [self.videoBuffLock unlock] ;
            
            if(lpNode != NULL)
            {
                *lpOutBuff = (unsigned char*)((unsigned char*)lpNode + sizeof(unsigned int)) ;
                
                *lpOutSize = lpNode->nBuffLen ;
                
                bRet = true ;
            }
        }
        
        return bRet ;
    }
    else if(nType == 1) //Audio
    {
        return bRet ;
    }
    
    return bRet ;
}


-(BOOL)ReleaseFrame:(unsigned char *)lpBuffer
{
    BuffNode *lpNode = NULL ;
    
    if(lpBuffer != NULL)
    {
        lpNode = CONTAINER_OF(lpBuffer, BuffNode, lpBuff) ;
        
        if(lpNode != NULL)
        {
            free(lpNode) ;
    
            lpNode = NULL ;
            
            lpBuffer = NULL ;
        }
    }
    
    return true ;
}


-(BOOL)ClearAllFrames
{
    BOOL bRet = true ;
    
    BuffNode *lpNode = NULL ;
    
    [self flushAndRemoveImage];    
    [self.videoBuffLock lock] ;
    
    while(!_lsVideoBuff.empty())
    {
        lpNode = _lsVideoBuff.front() ;
        
        _lsVideoBuff.pop_front() ;
        
        if(lpNode != NULL)
        {
            free(lpNode) ;
            
            lpNode = NULL ;
        }
    }
    
    [self.videoBuffLock unlock] ;

    [self.iFrameLock lock];
    
    if(_iFrameBuffNode != NULL){
        free(_iFrameBuffNode);
        _iFrameBuffNode = NULL;
    }
    
    [self.iFrameLock unlock];
    
    return bRet ;
}


-(BOOL)DecodeOneFrame: (unsigned char *)lpBuff nBuffSize :(unsigned int)nBuffSize n64PreAbsTime:(int64_t)n64PreAbsTime n64CurAbsTime:(int64_t)n64CurAbsTime
{
    BOOL bRet = false ;
    BOOL bReopen = false ;
    BOOL bReadyForData = false ;
    uint8_t *lpPPS = NULL ;
    uint8_t *lpSPS = NULL ;
    uint8_t *lpVPS = NULL ;
    const uint8_t *lpOutBuff = NULL ;
    int nCount = 0 ;
    size_t nSPSSize = 0 ;
    size_t nPPSSize = 0 ;
    size_t nVPSSize = 0 ;
    size_t nOutSize = 0 ;
    int64_t n64TimePTS = 0 ;
    OSStatus osStatus ;
    AntsFrameHeader *lpFramaHeader = NULL ;
    CMBlockBufferRef videoBlockBuffer = NULL;  //CMBlockBuffer：编码后结果图像的数据结构
    CMSampleBufferRef sampleBufferRef;
    CMSampleTimingInfo timingInfo ;
    
    if(lpBuff == NULL || nBuffSize <= 0)
    {
        return bRet ;
    }
    
    lpFramaHeader = (AntsFrameHeader*)lpBuff ;
    
    if(lpFramaHeader->uiFrameType == AntsPktAudioFrames)
    {
        return bRet ;
    }
    
    [self.speedLock lock];
    
    if(self.nSpeed >= 3)
    {
        if(lpFramaHeader->uiFrameType != AntsPktIFrames && lpFramaHeader->uiFrameType != AntsPktSubIFrames && lpFramaHeader->uiFrameType != AntsPkt_VIFreame && lpFramaHeader->uiFrameType != AntsPkt_SubVIFreame && lpFramaHeader->uiFrameType != AntsPkt_ThirdVIFreame)
        {
            NSLog(@"Not I Frame and speed[%d]\r\n", self.nSpeed);
        }
    }

    [self.speedLock unlock];
    
    /*
     AntsPkt_VIFreame = 0xa1,
     AntsPkt_SubVIFreame = 0xa2,
     AntsPkt_ThirdVIFreame = 0xa8,
     */
    
    if(self.bFirstFrame) //丢弃第一次非I帧数据
    {
      if(lpFramaHeader->uiFrameType != AntsPktIFrames && lpFramaHeader->uiFrameType != AntsPktSubIFrames && lpFramaHeader->uiFrameType != AntsPkt_VIFreame && lpFramaHeader->uiFrameType != AntsPkt_SubVIFreame && lpFramaHeader->uiFrameType != AntsPkt_ThirdVIFreame)
      {
          return bRet ;
      }
        
      self.nLastFrameNo = lpFramaHeader->uiFrameNo ;
    }
    
    if((self.nPreFrameNo + 1) != lpFramaHeader->uiFrameNo)
    {
        if(lpFramaHeader->uiFrameType != AntsPktIFrames && lpFramaHeader->uiFrameType != AntsPktSubIFrames && lpFramaHeader->uiFrameType != AntsPkt_VIFreame && lpFramaHeader->uiFrameType != AntsPkt_SubVIFreame && lpFramaHeader->uiFrameType != AntsPkt_ThirdVIFreame)
        {
            NSLog(@"Decode Error! 帧不连续!!!!\r\n");

            self.bFirstFrame = true ;
            
            self.bResetTimebase = true;
            
            return bRet ;
        }
    }
    
    self.nPreFrameNo = lpFramaHeader->uiFrameNo ;
    
    if(lpFramaHeader->uiFrameType == AntsPktIFrames || lpFramaHeader->uiFrameType == AntsPktSubIFrames) //I帧数据读取PPS和SPS
    {
        if(self.bFirstFrame)
        {
            if(lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_advance|| lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi_high || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi_RTP || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_RTP_PACK )
            {
                [VideoLayer get_sps_pps:lpBuff + sizeof(AntsFrameHeader) size:lpFramaHeader->uiFrameLen sps:(const uint8_t**)&lpSPS sps_size: &nSPSSize pps:(const uint8_t**)&lpPPS pps_size: &nPPSSize];
                
                if(lpSPS == NULL || lpPPS == NULL)
                {
                   return bRet ;
                }
                NSLog(@"bFirstFrame reopen!\r\n") ;
                
                bReopen = true ;
            }
            else if(lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH265 || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH265_RTP_PACK)
            {
                [VideoLayer get_sps_pps_vps4hevc:lpBuff + sizeof(AntsFrameHeader) size:lpFramaHeader->uiFrameLen sps:(const uint8_t**)&lpSPS sps_size: &nSPSSize pps:(const uint8_t**)&lpPPS pps_size: &nPPSSize vps:(const uint8_t **)&lpVPS vps_size:&nVPSSize];
                
                if(lpSPS == NULL || lpPPS == NULL || lpVPS == NULL)
                {
                    return bRet ;
                }
                
                NSLog(@"bFirstFrame reopen!\r\n") ;
                
                bReopen = true ;
            }
            else
            {
                self.bFirstFrame = true ;
                
                self.bResetTimebase = true;
                
                return false ;
            }
        }
        else
        {
            if(lpFramaHeader->uMedia.struVideoHeader.usHeight != self.nVideoHeight || lpFramaHeader->uMedia.struVideoHeader.usWidth != self.nVideoWidth || self.nCodecId != lpFramaHeader->uMedia.struVideoHeader.cCodecId)
            {
                if(lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_advance|| lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi_high || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi_RTP || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_RTP_PACK )
                {
                    [VideoLayer get_sps_pps:lpBuff + sizeof(AntsFrameHeader) size:lpFramaHeader->uiFrameLen sps:(const uint8_t**)&lpSPS sps_size: &nSPSSize pps:(const uint8_t**)&lpPPS pps_size: &nPPSSize];
                    
                    if(lpSPS == NULL || lpPPS == NULL)
                    {
                        return bRet ;
                    }
                    
                    NSLog(@"bFirstFrame reopen!\r\n") ;
                    
                    bReopen = true ;
                }
                else if(lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH265 || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH265_RTP_PACK)
                {
                    [VideoLayer get_sps_pps_vps4hevc:lpBuff + sizeof(AntsFrameHeader) size:lpFramaHeader->uiFrameLen sps:(const uint8_t**)&lpSPS sps_size: &nSPSSize pps:(const uint8_t**)&lpPPS pps_size: &nPPSSize vps:(const uint8_t **)&lpVPS vps_size:&nVPSSize];
                    
                    if(lpSPS == NULL || lpPPS == NULL || lpVPS == NULL)
                    {
                        return bRet ;
                    }
                    
                    NSLog(@"bFirstFrame reopen!\r\n") ;
                    
                    bReopen = true ;
                }
                else
                {
                    self.bFirstFrame = true ;
                    
                    self.bResetTimebase = true;
                    
                    return false ;
                }
                
                self.bFirstFrame = true ;
                
                self.bResetTimebase = true ;
                
            }
        }
        
        if(bReopen)
        {
            //nalu单元的startcode 00 00 00 01不需要 此处需要注意
            const uint8_t * const parameterSetPointers[3] = {lpSPS, lpPPS, lpVPS};
            const size_t parameterSetSizes[3] = {nSPSSize, nPPSSize, nVPSSize};
            
            if (self.lpVideoFormatDescr)
            {
                CFRelease(_lpVideoFormatDescr);
                
                self.lpVideoFormatDescr = NULL ;
            }

            if(lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_advance|| lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi_high || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi_RTP || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_RTP_PACK )
            {
                //根据pps和sps数据生成H264的FormatDesc
                osStatus = CMVideoFormatDescriptionCreateFromH264ParameterSets(kCFAllocatorDefault, 2, parameterSetPointers, parameterSetSizes, 4, &(_lpVideoFormatDescr));
                
                NSLog(@"CMVideoFormatDescriptionCreateFromH264ParameterSets !\r\n") ;
            }
            else if(lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH265 || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH265_RTP_PACK)
            {
                osStatus = CMVideoFormatDescriptionCreateFromHEVCParameterSets(kCFAllocatorDefault, 3, parameterSetPointers, parameterSetSizes, 4, NULL, &_lpVideoFormatDescr);
                NSLog(@"CMVideoFormatDescriptionCreateFromHEVCParameterSets !\r\n") ;
            }
            else
            {
                self.bFirstFrame = true ;
                
                self.bResetTimebase = true;
                
                return false ;
            }
            
            if(osStatus != noErr)
            {
                NSLog(@"CMVideoFormatDescriptionCreateFromH264ParameterSets Error!\r\n") ;
                
                self.bFirstFrame = true ;
               
                self.bResetTimebase = true;
                
                return bRet ;
            }
        }
    }
    
    if(self.nCaptureJPEG == 1){
        if(_iFrameBuffNode != NULL && _iFrameBuffNode->nBuffLen > 0){
            [self HandleCapture:_iFrameBuffNode->lpBuff nSize:_iFrameBuffNode->nBuffLen] ;
        }else{
            [self HandleCapture:lpBuff nSize:nBuffSize] ;
        }
        //立马用缓存的帧
    }

    //将0x00 0x00 0x00 0x01 最后的0x01变成 实际的SPS或者PPS 的长度
    lpOutBuff = [self change_startcode_to_size:lpBuff + sizeof(AntsFrameHeader) size:lpFramaHeader->uiFrameLen out_size:(size_t *)&nOutSize] ;

    NSLog(@"change_startcode_to_size [%zu]  buffsize[%d]\r\n",nOutSize,nBuffSize);

    //2.提取视频图像数据生成CMBlockBuffer (先用CMBlockBufferCreateWithMemoryBlock 从H.264数据创建一个CMBlockBufferRef实例)
   
    osStatus = CMBlockBufferCreateWithMemoryBlock(NULL, (void*)lpOutBuff, nOutSize, kCFAllocatorNull, NULL, 0, nOutSize, 0, &videoBlockBuffer);
    
    if (osStatus == kCMBlockBufferNoErr)
    {
        n64TimePTS = lpFramaHeader->uiFrameTime * 1000 + lpFramaHeader->uiFrameTickCount / 1000 ;
        
        [self.speedLock lock];
    
        if(self.nSpeed != self.nLastSpeed)
        {
            self.bResetTimebase = true;
            
            self.nLastSpeed  = self.nSpeed ;
        }
        
        [self.speedLock unlock];
        
        if(n64CurAbsTime < self.n64StartPTS)//实际上就是第一次进行时间轴重置
        {
            self.bResetTimebase = true;
        }
        
        if (self.bResetTimebase) //出现丢帧后者倍速播放，进行时间轴重置
        {
            [self.speedLock lock];
            
            if(self.bFirstFrame)
            {
              [self ResetTimebase:0 nSpeed:self.nSpeed] ;
            }
            else
            {
              [self ResetTimebase:1 nSpeed:self.nSpeed] ;
            }
            
            [self.speedLock unlock];
            
            self.n64StartPTS = n64CurAbsTime ;
            
            self.n64AbsPTS = n64CurAbsTime ;
        }
        
        if(n64CurAbsTime - n64PreAbsTime > 40 || n64CurAbsTime - n64PreAbsTime < 0 )
        {
            timingInfo = {kCMTimeInvalid, CMTimeMake(self.n64AbsPTS + 40 - self.n64StartPTS, 1000), kCMTimeInvalid};
            
            self.n64AbsPTS += 40 ;
        }
        else{
            timingInfo = {kCMTimeInvalid, CMTimeMake(self.n64AbsPTS + n64CurAbsTime - n64PreAbsTime - self.n64StartPTS, 1000), kCMTimeInvalid};

            self.n64AbsPTS += (n64CurAbsTime - n64PreAbsTime) ;
        }
        
//        NSLog(@"pts n64CurAbsTime:%lld n64PreAbsTime:%lld self.n64AbsPTS:%lld timeStart:%lld\r\n", n64CurAbsTime, n64PreAbsTime, self.n64AbsPTS, self.n64AbsPTS - self.n64StartPTS);
        
        const size_t sampleSize = lpFramaHeader->uiFrameLen ;

        //3.根据需要，生成CMTime信息  (使用CMSampleBufferCreate接口得到CMSampleBuffer数据---这个待解码的原始的数据)

        osStatus = CMSampleBufferCreate(kCFAllocatorDefault,  //分配器用于分配cmsamplebuffer对象。kcfallocatordefault---使用默认的分配器
                                      videoBlockBuffer,
                                      YES,   //videoBlock是否已经包含媒体数据
                                      NULL,
                                      NULL,
                                      _lpVideoFormatDescr,
                                      1,
                                      1,
                                      &timingInfo,
                                      1,
                                      &sampleSize,
                                      &sampleBufferRef);
        if (osStatus == noErr)
        {
            while(1/*nCount > 0*/)
            {
                if(self.status == AVQueuedSampleBufferRenderingStatusFailed){
                    break ;
                }
                
                bReadyForData = [self isReadyForMoreMediaData]; //是否能接受媒体的数据
                
                if(bReadyForData){
                    break ;
                }else{
                    [NSThread sleepForTimeInterval:0.008];
                    nCount-- ;
                }
            }
            
            if(bReadyForData)
            {
                //在每次进行渲染前都会判断这个status的类型，一旦这个类型为failed，就调用这个方法 add by hq in 2017-8-24
                if(self.status == AVQueuedSampleBufferRenderingStatusFailed)
                {
                    NSLog(@"渲染失败了,flush\r\n");
                    
                    [self flush];
                    
                    self.bFirstFrame = true ;
                    
                    self.bResetTimebase = true;
                    
                    return bRet ;
                }
                
                if(sampleBufferRef)
                {
                    [self enqueueSampleBuffer:sampleBufferRef];
                }
                
                if(self.bFirstFrame)
                {
                    self.nVideoHeight = lpFramaHeader->uMedia.struVideoHeader.usHeight ;
                    
                    self.nVideoWidth = lpFramaHeader->uMedia.struVideoHeader.usWidth ;
                    
                    self.nCodecId = lpFramaHeader->uMedia.struVideoHeader.cCodecId ;
                    NSLog(@"first set width:%d height:%d id:%d\r\n",self.nVideoWidth, self.nVideoHeight, self.nCodecId) ;
                }
                
                self.bFirstFrame = false ;
                
                self.bResetTimebase = false;
                
                bRet = true ;
                
                if(nCount < 0)
                {
                    NSLog(@"Insert Succ FrameNO:%d time:%lld count:[%d]\r\n", lpFramaHeader->uiFrameNo, n64TimePTS, nCount) ;
                }
            }
            else
            {
                [self flush];

                self.bResetTimebase = true;
                
                self.bFirstFrame = true ;
                
                NSLog(@"Insert Failed FrameNO:%d time:%lld\r\n", lpFramaHeader->uiFrameNo, n64TimePTS) ;

                return bRet ;
            }
        }
        else
        {
            NSLog(@"CMSampleBufferCreate Error FrameNO:%d time:%lld\r\n", lpFramaHeader->uiFrameNo, n64TimePTS) ;
            
            if (videoBlockBuffer != NULL)
            {
                CFRelease(videoBlockBuffer);
                
                videoBlockBuffer = NULL ;
            }
            
            self.bResetTimebase = true;

            self.bFirstFrame = true ;
            
            return bRet ;
        }
    }
    else
    {
        NSLog(@"CMBlockBufferCreateWithMemoryBlock Error FrameNO:%d time:%lld\r\n", lpFramaHeader->uiFrameNo, n64TimePTS) ;
        
        self.bResetTimebase = true;
        
        self.bFirstFrame = true ;
        
        return bRet;
    }
    
    if (videoBlockBuffer != NULL)
    {
        CFRelease(videoBlockBuffer);
        
        videoBlockBuffer = NULL ;
    }
    
    if (sampleBufferRef != NULL)
    {
        CFRelease(sampleBufferRef);
        sampleBufferRef = NULL;
    }
    
    return bRet ;
}


- (void)DecodeVideoProc
{
    bool bRet = 0 ;
    unsigned int nOutSize = 0 ;
    unsigned int nCurFrameNo = 0 ;
    unsigned int nCodecId = 0 ;
    unsigned short nCurVideoHeight = 0;
    unsigned short nCurVideoWidth = 0;
    unsigned int nCurframeType = 0;
    
    int64_t nRelativeTime = 0 ;
    int64_t nAbsoulteTime = 0 ;
    double nIntervalTime = 0 ;
    int64_t n64StartTime = 0 ;
    int64_t n64StopTime = 0 ;
    int64_t nUseTime = 0 ;
    int64_t nPreAbsoulteTime = 0 ;
    int64_t nTempTime = 0 ;
    int64_t nTempTime2 = 0 ;
    double nSleepTime = 0 ;
    unsigned char *lpOutBuff = NULL ;
    AntsFrameHeader *lpFrameHeader = NULL ;
    struct timeval tv  ;

    while(self.bVideoDecodeFlag)
    {
        [NSThread currentThread].name = @"DecodeVideoProc";
   
        nRelativeTime = 0 ;
        nAbsoulteTime = 0 ;
        nIntervalTime = 0 ;
        n64StartTime = 0 ;
        n64StopTime = 0 ;
        nUseTime = 0 ;
        nTempTime = 0 ;
        nSleepTime = 0 ;
        
        if(self.decodeThread.isCancelled)
        {
            [NSThread exit];
         
            break;
        }
        
        if(self.bPause)
        {
            [NSThread sleepForTimeInterval:0.040];
          
            continue ;
        }
        
        memset(&tv, 0, sizeof(struct timeval)) ;

        gettimeofday(&tv, NULL) ;
        
        n64StartTime = tv.tv_sec * 1000 + tv.tv_usec / 1000 ;
        
        bRet = [self ReadOneFrame:0 lpOutBuff:(unsigned char **)(&lpOutBuff) lpOutSize:(unsigned int *)(&nOutSize)];
        
        if(!bRet)
        {
            [NSThread sleepForTimeInterval:0.002];
            
            continue ;
        }
        
        lpFrameHeader = (AntsFrameHeader*)lpOutBuff ;
        
        if(lpFrameHeader->uiFrameType == AntsPktSmartIFrames || lpFrameHeader->uiFrameType == AntsPktSmartPFrames){
            
            if(self.smartFrameCallBack){
                self.smartFrameCallBack(lpOutBuff, nOutSize,(eAntsFrameType)lpFrameHeader->uiFrameType);
            }
        
            [self ReleaseFrame:lpOutBuff] ;

            [NSThread sleepForTimeInterval:0.0010];

            continue;
        }
        
        nCodecId = lpFrameHeader->uMedia.struVideoHeader.cCodecId ;
        
        nCurFrameNo = lpFrameHeader->uiFrameNo ;
        nRelativeTime = lpFrameHeader->uiTimeStamp ;
        nCurVideoHeight = lpFrameHeader->uMedia.struVideoHeader.usHeight;
        nCurVideoWidth = lpFrameHeader->uMedia.struVideoHeader.usWidth;
        nCurframeType = lpFrameHeader->uiFrameType;
    
        nAbsoulteTime = lpFrameHeader->uiFrameTime * 1000 + lpFrameHeader->uiFrameTickCount / 1000 ;
        {
            [self.syncTimeLock lock] ;
            
             nPreAbsoulteTime = self.n64SyncPTS;
            
            [self.syncTimeLock unlock] ;
        }
    
        if(self.bDevodeFlag){
            //软解
            bRet = _stream2FFMPEG(lpOutBuff , nOutSize);
        }else{
            bRet = [self DecodeOneFrame:lpOutBuff nBuffSize:nOutSize n64PreAbsTime:nPreAbsoulteTime n64CurAbsTime:nAbsoulteTime];
        }
        
        NSLog(@"DecodeOneFrame [%d] \r\n",bRet);
        
        [self ReleaseFrame:lpOutBuff] ;
        {
            [self.syncTimeLock lock] ;
            
            nTempTime2 = self.n64SyncPTS;
            
            self.n64SyncPTS = nAbsoulteTime ;
            
            [self.syncTimeLock unlock] ;
        }
        
        if(!bRet)
        {
            NSLog(@"Decode Error! channel [%d] self.nLastFrameNo[%d]\r\n",self.channel,self.nLastFrameNo) ;
        
            self.nLastFrameNo = nCurFrameNo;
        
            //帧号不连续 在这里就被continue了
            continue ;
        }
    
        if(nCodecId == AntsH264_hisi_RTP || nCodecId == AntsH264_RTP_PACK || nCodecId == AntsH265){
              //用两帧之间的相对时间戳进行计算睡眠时间
            if(nRelativeTime > self.nPreRelativeTime){
                nIntervalTime = (nRelativeTime - self.nPreRelativeTime) / 90;
            }
            else{
                nIntervalTime = (self.nPreRelativeTime - nRelativeTime ) / 90;
            }
            
            self.nPreRelativeTime = nRelativeTime ;
          
        }else{
            
            //用两帧之间的绝对时间戳计算睡眠时间
            nIntervalTime = nAbsoulteTime - self.nPreRelativeTime ;
            self.nPreRelativeTime = nAbsoulteTime ;
        }
    
        double timePerFrame = self.mFrameRate==0?40:1000./self.mFrameRate;
    
        if(nIntervalTime - timePerFrame >= 0 || nIntervalTime <=0 || nCurFrameNo < self.nLastFrameNo){
            //睡眠时间 如果不合法 超过了40ms  或者当前帧号小于上一帧的帧号
            nIntervalTime = timePerFrame;
        }
    
        self.nLastFrameNo = nCurFrameNo;
    
        if(self.nPlayType == 0){
            [self.videoBuffLock lock] ;
            if(_lsVideoBuff.size() >= MAX_VIDEO_WARN_NUM){
                //每次判断缓冲区的大小是否已经 达到临界值20 目前缓冲区25帧 临界值20 超过20帧开始加速解码 睡眠时间减少
                nIntervalTime = nIntervalTime/2;
            }
    
            [self.videoBuffLock unlock] ;
        }else{
            nIntervalTime = 40;
        }
    
        memset(&tv, 0, sizeof(struct timeval)) ;
        
        gettimeofday(&tv, NULL) ;
        
        n64StopTime = tv.tv_sec * 1000 + tv.tv_usec / 1000 ;
        
        nUseTime = n64StopTime - n64StartTime;
    
        //倍速 调整
        if(self.nSpeed >= 3){
            //只收I帧
            nIntervalTime = 1000/(1<<self.nSpeed);
        }else{
            nIntervalTime = nIntervalTime * pow((float)2, -1 * self.nSpeed);
        }
    
        if(nIntervalTime > nUseTime)
        {
            nSleepTime = 1.0 * (nIntervalTime - nUseTime ) / 1000. ;
        }
        else
        {
            nSleepTime = 0 ;
        }
    
        NSLog(@"解码睡眠时间 [%.2f] 帧号[%d] 帧率[%d]\r\n",nSleepTime * 1000,nCurFrameNo,self.mFrameRate);
        if(nSleepTime > 0){
           [NSThread sleepForTimeInterval:nSleepTime];
        }
     }

    self.bVideoThreadExitFlag = true ;
}

-(BOOL)captureJpegByFFMPEG:(NSString *)fileName callback:(void (^)(BOOL bFlag))fpCaptureCallback
{
    if(!self.bVideoDecodeFlag){
        return false;
    }
    
    if(self.nFFMPEGCaptureJPEG != 0){
        return false;
    }
    
    self.nFFMPEGCaptureJPEG = 1;
    self.fileNameByFFMPEG = fileName;
    return true;
}


-(void)writeCaptureFile:(uint8_t *)lpFrame nSize:(size_t)nSize
{
    NSString * fileName = [NSString stringWithFormat:@"captureFile"];
    NSString * filePath = [NSString stringWithFormat:@"%@/%@",NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES).firstObject,fileName];
    
    char * lpFilePath = (char *)[filePath cStringUsingEncoding:NSUTF8StringEncoding];
    
    FILE *fp = fopen(lpFilePath, "ab+") ;
    
    if(fp != NULL)
    {
        fwrite(lpFrame, 1, nSize, fp) ;
 
        fclose(fp) ;
    }
}

-(void)HandleCapture:(uint8_t *)lpFrame nSize:(size_t)nSize
{
    int type = 0;
    size_t nSPSSize = 0 ;
    size_t nPPSSize = 0 ;
    size_t nVPSSize = 0 ;
    uint32_t nNalDataSize =  0;
    uint32_t nWriteSize =  0;
    const uint8_t *lpPPS = NULL ;
    const uint8_t *lpSPS = NULL ;
    const uint8_t *lpVPS = NULL ;
    const uint8_t *nal_start, *nal_end, *data, *end;
    
    AntsFrameHeader *lpFramaHeader = NULL ;
    
    if(lpFrame == NULL || nSize <= 0 || self.nCaptureJPEG != 1){
        return ;
    }
    
    lpFramaHeader = (AntsFrameHeader*)lpFrame ;
    
    if(self.nCaptureJPEG == 1)
    {
        [self writeCaptureFile:lpFrame nSize:nSize];
        
        if(lpFramaHeader->uiFrameType == AntsPktIFrames || lpFramaHeader->uiFrameType == AntsPktSubIFrames)
        {
            self.nCaptureJPEG = 2 ;
            
            data = lpFrame + sizeof(AntsFrameHeader) ;
            
            end = data + lpFramaHeader->uiFrameLen;
            
            _vecNalBuff.clear() ;
            
            _vecNalBuffSize.clear() ;
            
            nal_start = [VideoLayer avc_find_startcode:data end:end];
            
            while (true) {
                while (nal_start < end && !*(nal_start++)) ;
    
                if (nal_start == end)
                    break;
                
                nal_end = [VideoLayer avc_find_startcode:nal_start end:end];
                
                if(lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_advance|| lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi_high || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_hisi_RTP || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH264_RTP_PACK )
                {
                    type = nal_start[0] & 0x1F;
                    if (type == NAL_SPS) {
                        lpSPS = nal_start;
                        nSPSSize = nal_end - nal_start;
                    } else if (type == NAL_PPS) {
                        lpPPS = nal_start;
                        nPPSSize  = nal_end - nal_start;
                    }
                }
                else if(lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH265 || lpFramaHeader->uMedia.struVideoHeader.cCodecId == AntsH265_RTP_PACK)
                {
                    type = (nal_start[0] >> 1) & 63;
                    if (type == HEVC_NAL_SPS) {
                        lpSPS = nal_start;
                        nSPSSize = nal_end - nal_start;
                    } else if (type == HEVC_NAL_PPS) {
                        lpPPS = nal_start;
                        nPPSSize = nal_end - nal_start;
                    }else if (type == HEVC_NAL_VPS) {
                        lpVPS = nal_start;
                        nVPSSize = nal_end - nal_start;
                    }
                }
                
                _vecNalBuff.push_back((uint8_t *)nal_start);
                
                _vecNalBuffSize.push_back((uint32_t)(nal_end - nal_start)) ;
                
                nNalDataSize += (nal_end - nal_start) ;
                
                nal_start = nal_end;
            }
        }
        
        if(nSPSSize <= 0 && nPPSSize <= 0 && nVPSSize <= 0)
        {
            _vecNalBuff.clear() ;
            
            _vecNalBuffSize.clear() ;
            
            [self DoCapture:NULL nSize:0 lpSPS:NULL nSPSSize:0 lpPPS:NULL nPPSSize:0 lpVPS:NULL nVPSSize:0] ;
            
            return ;
        }
        
        if(nNalDataSize + 4 * _vecNalBuff.size() > 1024 *1024)
        {
            if(_lpCaptureIframe != NULL)
            {
                free(_lpCaptureIframe) ;
                
                _lpCaptureIframe = (unsigned char*)malloc(nNalDataSize + 4 * _vecNalBuff.size()) ;
            }
        }
        
        if(_lpCaptureIframe != NULL)
        {
            for(unsigned int nIndex = 0 ; nIndex < _vecNalBuff.size(); nIndex++)
            {
                uint32_t nDataLen = htonl(_vecNalBuffSize[nIndex]);
                
                memcpy(_lpCaptureIframe + nWriteSize, (void*)&nDataLen, sizeof(uint32_t));
                
                nWriteSize += sizeof(uint32_t) ;
                
                memcpy(_lpCaptureIframe + nWriteSize, _vecNalBuff[nIndex], _vecNalBuffSize[nIndex]);
                
                if(_vecNalBuff[nIndex] == lpSPS)
                {
                    lpSPS = _lpCaptureIframe + nWriteSize ;
                }
                else if(_vecNalBuff[nIndex] == lpPPS)
                {
                    lpPPS = _lpCaptureIframe + nWriteSize ;
                }
                else if(_vecNalBuff[nIndex] == lpVPS)
                {
                    lpVPS = _lpCaptureIframe + nWriteSize ;
                }
                
                nWriteSize += _vecNalBuffSize[nIndex] ;
            }
            
            _vecNalBuff.clear() ;
            
            _vecNalBuffSize.clear() ;
            
            [self DoCapture:_lpCaptureIframe nSize:nWriteSize lpSPS:(uint8_t *)lpSPS nSPSSize:nSPSSize lpPPS:(uint8_t *)lpPPS nPPSSize:nPPSSize lpVPS:(uint8_t *)lpVPS nVPSSize:nVPSSize] ;
        }
        else
        {
             [self DoCapture:NULL nSize:0 lpSPS:NULL nSPSSize:0 lpPPS:NULL nPPSSize:0 lpVPS:NULL nVPSSize:0] ;
        }
    }
}


-(BuffNode *)getCurrentIframeBuffNode
{
    BuffNode * buffNode = NULL;
    
    [self.iFrameLock lock];
    
    buffNode = _iFrameBuffNode;
    
    [self.iFrameLock unlock];

    return buffNode;
}


-(void)setFrame:(CGRect)frame{
    [super setFrame:frame];
    
    self.bFirstFrame = true;
}
@end


