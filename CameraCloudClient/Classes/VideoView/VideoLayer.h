//
//  VideoLayer.h
//  ANTSCMS2_0Demo
//
//  Created by ants-mac on 16/5/4.
//  Copyright © 2016年 www.yurfly.com. All rights reserved.
//
#pragma once

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "CloudClientSDKCommon.h"

typedef struct
{
    unsigned int nBuffLen ;
    unsigned char lpBuff[] ;
} BuffNode ;

@interface VideoLayer : AVSampleBufferDisplayLayer //遵循协议
@property(nonatomic,assign)long *handleRef;
@property (assign, nonatomic) unsigned short nVideoWidth;
@property (assign, nonatomic) unsigned short nVideoHeight;
@property (assign, nonatomic) unsigned short nCodecId;
@property (nonatomic,assign) int layerIndex;

@property (nonatomic,assign) HANDLE lpSaveHandle;
@property (nonatomic,assign) int videoRecordStartTime;
@property (nonatomic,assign) char codecId;
@property (nonatomic,assign) BOOL isVideoRecording;//是否在录像
@property (nonatomic,assign) BOOL bDevodeFlag; // 0 硬解 1 软解
@property (nonatomic,assign) int channel ;
@property (nonatomic,assign) int frameNo ;
@property (nonatomic,assign) int mFrameNo ;
@property (nonatomic,assign) double mFrameTime ;

@property (nonatomic,assign) BOOL bReplayAudioFlag; // 1 回放音频播放
@property (assign, nonatomic) BOOL bFirstFrame;

@property (nonatomic,copy) int (^stream2FFMPEG)(unsigned char * buff,int nBuffSize);
@property (nonatomic,copy) void (^firstIframeCome)(BOOL bFirstIframe);

@property (nonatomic,copy) int (^smartFrameCallBack)(unsigned char * buff,int nBuffSize,eAntsFrameType frameType);

//播放实时流信息
-(void)InitDecoder:(int) nType ; //0: RealPlay  1;Replay

-(void)SetPlaySpeed:(int) nSpeed ;

-(void)SetPlayPause:(BOOL) bPause ;

-(BOOL)AddOneFrame:(int) nType lpBuff:(unsigned char *)lpBuff nBuffSize:(unsigned int)nBuffSize frameRate:(int)frameRate;

-(BOOL)ClearAllFrames;

-(void)UnInitDecoder;

@end

