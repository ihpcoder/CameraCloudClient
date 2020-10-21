//
//  MyVideoView.h
//  ClearVision
//
//  Created by ants-mac on 17/3/17.
//  Copyright © 2017年 www.yurfly.com. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "VideoLayer.h"
@interface VideoView : UIView  //UIScrollView

@property(nonatomic,strong)VideoLayer *videoLayer;
@property(nonatomic,strong)UIButton *button; //添加按钮

//add by hq in 2017-7-13 h265视频解码器和显示器
//@property (strong, nonatomic) H265Dec *h265decoder;
//@property (strong, nonatomic) OpenGLView20 *playH265View;
//add by hq in 2017-6-10

@property(nonatomic,strong)NSString *uuid;  //指明当前画布播放的是哪一个设备的视频
@property(nonatomic,assign)int channel; //指明当前画布播放的是哪一个通道
@property(nonatomic,assign)int streamType; //指明当前画布播放预览视频的是主码流还是子码流
@property(nonatomic,assign)int backStreamType; //指明当前画布播放回放视频的是主码流还是子码流

//当开启预览时,realVideoFlag = yes, realAudioFlag = yes, realPTZFlag = no, realIntercomFlag = no
@property(nonatomic,assign)BOOL realVideoFlag; //当前画布是否开启预览视频
@property(nonatomic,assign)BOOL realAudioFlag; //当前画布是否开启预览音频
@property(nonatomic,assign)BOOL realPTZFlag; //当前画布是否开启预览云台 默认NO
@property(nonatomic,assign)BOOL realIntercomFlag; //当前画布是否开启语音对讲 默认NO

//当开启回放时,backVideoFlag = yes, backAudioFlag = yes backPauseFlag＝no
@property(nonatomic,assign)BOOL backVideoFlag; //当前画布是否开启回放视频
@property(nonatomic,assign)BOOL backPauseFlag; //当前画布回放是否暂停
@property(nonatomic,assign)BOOL backAudioFlag; //当前画布是否开启回放音频
@property(nonatomic,assign)BOOL isHaveRecord; //当前画布是否搜索到录像

@property (nonatomic,assign) BOOL isVideoRecording;//当前画布是否 正在录制视频
/*
* zoom操作指示图标
*/
- (void)zoomOut:(BOOL)isOut start:(BOOL)isStart;

-(void)showAllImages;
-(void)dismissAllImages;

-(void)showIndicatorView;
-(void)dismissIndicatorView;

@end
