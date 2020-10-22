//
//  ViewController.m
//  cameraExample
//
//  Created by yuhao yuhao on 2020/10/22.
//

#import "ViewController.h"
#import <CameraCloudClient/CloudClientSDK4iOS.h>
#import <CameraCloudClient/VideoView.h>
#import <AntsUtil.h>
#import <MJExtension.h>

UIKIT_EXTERN NSString * const testDeviceUid = @"02f40002000bcaa98f7c";
UIKIT_EXTERN NSString * const testDeviceUserName = @"admin";
UIKIT_EXTERN NSString * const testDevicePwd = @"";

@interface ViewController ()
@property (nonatomic,strong) NSMutableDictionary * mLoginMap;

@property (nonatomic,assign) HANDLE deviceHandle;

@property (nonatomic,strong) VideoView * videoView;

@property (nonatomic,strong) VideoView * replayVideoView;
@property (nonatomic,assign) int currentReplayId;

@property (weak, nonatomic) IBOutlet UILabel *speedLabel;
@property (nonatomic,assign) int nSpeed;//倍速播放速率

@property (nonatomic,copy) NSString * p2pServer;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    NSString * str = [AntsUtil ants_3des_decFun:@"gbiedkpmohpfjfeh"];
    
    NSLog(@"decode str [%@] \r\n",str);
    
    self.view.backgroundColor = [UIColor whiteColor];
    
    [self.view addSubview:self.videoView];
    [self.view addSubview:self.replayVideoView];
    
    self.nSpeed = 0;
    self.speedLabel.text = [self getSpeedFromeLevel:self.nSpeed];
    
    [self queryDeviceP2PServer];

}


/*
 报警订阅接口
 如果不想订阅全部消息，或者只想订阅某种消息 请看下面注释
 */
-(void)subScibeAlarmV2{
    /*
    //这里是订阅了所有的报警消息 -2147483647 = 11111111 11111111 11111111 11111111
      如果只想订阅某种报警消息 需要按位设置报警类型
    //所有报警类型的枚举 cloudsdk_alarm_type
     例如: CLOUDSDK_ALARM_VIDEOMOTION_TYPE             = 0x0001,           //!移动侦测报警类型
     只想订阅移动侦测报警 那么就是 数组里面的数字 第(0x0001 -1) 位 1代表订阅 0代表不订阅  计算结果:00000000 00000000 00000000 00000001
     
     传到接口的subScibeFlagArr[8] = {1,0,0,0,0,0,0,0};
     
     CLOUDSDK_ALARM_VIDEOMOTION_TYPE             = 0x0001,           //!移动侦测报警类型  设置int数组里面 第一个 int数据 第0位是1
     CLOUDSDK_ALARM_ALARMIN_TYPE                     = 0x0002,           //!输入报警类型 设置int 数组里面 第一个 int数据 第1位是1
     
     
     订阅移动侦测和输入报警 则 计算的结果是 00000000 00000000 00000000 00000011 = 3
     传到接口的subScibeFlagArr[8] = {3,0,0,0,0,0,0,0};

     报警类型大于int长度 32bit  例如
        CLOUDSDK_ALARM_IVS_MOTIONDETECT = 0x0021,       33  //!智能移动侦测
        CLOUDSDK_ALARM_IVS_TEMPHIGH = 0x0022,        34     //!温度高

     顺延到数组下个int数值上 然后减去当前数组个数 * 32 来计算bit下标  下标:33 - 32 - 1 = 0 ;34 - 32 - 1 = 1;
     计算的数据就是 00000000 00000000 00000000 0000011
     
     传到接口的subScibeFlagArr[8] = {0,3,0,0,0,0,0,0};
     */
    
    int subScibeFlagArr[8] = {-2147483647,15,0,0,0,0,0,0};//订阅全部消息

    CLOUDSDK_ALARM_COND_V2 alarmCmd;
    memset(&alarmCmd, 0, sizeof(CLOUDSDK_ALARM_COND_V2));

    memcpy(&alarmCmd.u32AlarmRules, subScibeFlagArr, sizeof(subScibeFlagArr));

    [[CloudClientSDK4iOS getInstance] subScribeAlarmV2:self.deviceHandle playParamter:alarmCmd resCallBack:^(void *lpData, unsigned int nDataLen) {
      CLOUDSDK_RESULT * lpResult = (CLOUDSDK_RESULT *)lpData ;
      
      NSLog(@"streamHeader->mStreamType --%d response ret:%d len:%d \n", lpResult->wRes,lpResult->wResult, nDataLen) ;
    }];
}


//视频解码Layer
-(VideoView *)videoView{
    if(!_videoView){
        _videoView = [[VideoView alloc] initWithFrame:CGRectMake(0, 20, [UIScreen mainScreen].bounds.size.width,150)];

        [_videoView.videoLayer InitDecoder:0];
    }
    return _videoView;
}


-(VideoView *)replayVideoView{
    if(!_replayVideoView){
        _replayVideoView = [[VideoView alloc] initWithFrame:CGRectMake(0, 180, [UIScreen mainScreen].bounds.size.width,  150)];

        [_replayVideoView.videoLayer InitDecoder:1];

    }
    return _replayVideoView;
}

/*
 查询设备所在的服务器地址
 */
#pragma mark ------------查询设备所在P2P服务器是否在线
-(void)queryDeviceP2PServer{
    CLOUDSDK_SERVER_INFO serverInfo;
    memset(&serverInfo, 0, sizeof(CLOUDSDK_SERVER_INFO));
    
    //查询接口  同步接口 调用完成才能做其他操作  建议放入设备登录线程中 以免影响UI执行
    if([[CloudClientSDK4iOS getInstance] CloudClient_QueryServer:testDeviceUid serverInfo:&serverInfo]){
        //请求成功
        NSString * superServerAddr = [NSString stringWithCString:serverInfo.szSuperAddr encoding:NSUTF8StringEncoding];
        NSString * p2pServerAddr = [NSString stringWithCString:serverInfo.szP2PServerAddr encoding:NSUTF8StringEncoding];
        
        int supServerPort = serverInfo.u16SuperPort;
        int p2pServerPort = serverInfo.u16P2PServerPort;

        NSLog(@"中心服务器地址：[%@:%d] \r\n注册服务器地址[%@:%d] \r\n",superServerAddr,supServerPort,p2pServerAddr,p2pServerPort);

        self.p2pServer = p2pServerAddr;
        /*
         查询设备状态(是否上线)2在线;1不在线
         * 设备状态不在线的设备不需要调用下面的登录接口
         */
        int deviceState = [self queryDeviceIsOnline:testDeviceUid p2pServer:p2pServerAddr];
           
        //如果在线
        if(DeviceServerStateOnline == deviceState){
            /*
             * 设备登录需要打通App与设备之间的网络，两端之间的网络情况复杂程度都不太相同， 调用1次不能保证100%连接成功 登录不成功可能需要调用多次。
             * 建议通过开辟子线程，遍历队列的方式 进行登录设备，登录不成功则在子线程中再次登录。字典数据:NSMutableDictionary <NSString * uid,P2PDeviceInfoModel * p2pDeviceInfo>*
             * 报错: 错误码29的设备 可能是密码错误 不需要再次登录
             */
            HANDLE handle = [self deviceLogin:p2pServerAddr device:testDeviceUid];
            
            [self setDeviceStreamCallBack:handle];
            
            self.deviceHandle = handle;
        }
    }
}
#pragma mark -----------------


#pragma mark ------------查询设备所在P2P服务器是否在线
-(int)queryDeviceIsOnline:(NSString *)deviceUid p2pServer:(NSString *)p2pServer{
    int state = 0;
    
    if([[CloudClientSDK4iOS getInstance] CloudClient_QueryDeviceState:deviceUid P2PServerAddr:p2pServer state:&state]){
        //查询成功
        NSLog(@"查询成功 设备[%@] 状态[%d]\r\n",deviceUid,state);
    }
    
    return state;
}
#pragma mark -----------------


/*
 设备登录
 */
#pragma mark ----------设备登录
-(HANDLE)deviceLogin:(NSString *)p2pServer device:(NSString *)deviceUid{
    CLOUDSDK_DEVICE_INFO device_info;
    memset(&device_info, 0, sizeof(CLOUDSDK_DEVICE_INFO));
    
    int errorNo = 0;
    
    HANDLE handle = [[CloudClientSDK4iOS getInstance] ClientLogin:p2pServer lpUid:deviceUid lpName:testDeviceUserName lpPassWord:testDevicePwd nConnectTime:10000 lpDeviceInfo:&device_info errorNo:&errorNo];
        
    if(handle){
        //登录成功
        NSString * devName = [NSString stringWithCString:(char *)device_info.byDevName encoding:NSUTF8StringEncoding];
        NSString * seriesNo = [NSString stringWithCString:(char *)device_info.bySerialNo encoding:NSUTF8StringEncoding];

        NSLog(@"设备登录 名称[%@]\r\n序列号[%@]\r\n通道总数[%d]\r\n",devName,seriesNo,device_info.wChanNum);
    }else{
        //登录失败 errorNo
        NSLog(@"登录失败 错误码[%d] \r\n",errorNo);
    }
    
    return handle;
}
#pragma mark -----------------


#pragma mark ----------设置流回调
-(void)setDeviceStreamCallBack:(HANDLE)handle{
    BOOL state = [[CloudClientSDK4iOS getInstance] setStreamCallBack:handle callBack:^int(char *lpInBuffer1, int nBufSize1, char *lpInBuffer2, int nBufSize2, HANDLE handle) {
        CLOUDSDK_HELPER_HEADER1 * streamHeader = (CLOUDSDK_HELPER_HEADER1 * )lpInBuffer1;
        int channel = streamHeader->channel;//通道号

        switch (streamHeader->stream_data_type) {
            case REALTIME_VIDEO_DATA:
                {
                    AntsFrameHeader * frameHeader = (AntsFrameHeader *)lpInBuffer2;
                    unsigned int uiFrameType = frameHeader->uiFrameType;
                    unsigned int uiFrameNo = frameHeader->uiFrameNo;
                    unsigned int uiFrameLen = frameHeader->uiFrameLen;
                    unsigned short videoWidth =  frameHeader->uMedia.struVideoHeader.usWidth;
                    unsigned short videoHeight = frameHeader->uMedia.struVideoHeader.usHeight;

                    DWORD frameRate =  *(DWORD *)(lpInBuffer1 + 27);

                    BOOL res = [self.videoView.videoLayer AddOneFrame:0 lpBuff:(unsigned char *)lpInBuffer2 nBuffSize:nBufSize2 frameRate:frameRate];
                    
                    NSLog(@"帧类型[%d] 通道号[%d] 帧号[%d] 帧长度[%d] 视频宽度[%d] 视频高度[%d]  塞流结果[%d] buffSize[%d]\r\n",uiFrameType,channel,uiFrameNo,uiFrameLen,videoWidth,videoHeight,res,nBufSize2);

                    return res?CloudSdk_ResultCode_Succ:CloudSdk_ResultCode_Failed;
                }
                break;
            case RECORD_VIDEO_DATA:{
                DWORD frameRate = *(DWORD *)(lpInBuffer1 + 31);

                AntsFrameHeader * frameHeader = (AntsFrameHeader *)lpInBuffer2;
                
                if(frameHeader->uiFrameType != AntsPktIFrames \
                    && frameHeader->uiFrameType != AntsPktSubIFrames \
                    && frameHeader->uiFrameType != AntsPktPFrames \
                    && frameHeader->uiFrameType != AntsPktSubPFrames \
                    && frameHeader->uiFrameType != AntsPktThirdIFrames \
                    && frameHeader->uiFrameType != AntsPktThirdPFrames \
                    && frameHeader->uiFrameType != AntsPkt_VIFreame \
                    && frameHeader->uiFrameType != AntsPkt_SubVIFreame \
                    && frameHeader->uiFrameType != AntsPkt_ThirdVIFreame){

                    return CloudSdk_ResultCode_Succ ;
                }

                //回放ID
                DWORD replayId = *(DWORD *)(lpInBuffer1 + 4);

                double uiFrameTime = frameHeader->uiFrameTime;
                double uiFrameTickCount = frameHeader->uiFrameTickCount;
                double timeInterval = uiFrameTime * 1000* 1000 + uiFrameTickCount;
                
                NSLog(@"当前回放ID[%d],回放时间戳[%.2f]\r\n",replayId,timeInterval/1000000);
                
#warning 回放时间戳带上了 摄像机所在的时区 需要减掉当前手机所在的时区时间 例如：北京时间+8  UTC时间是 时间戳 - 8*60*60
                
                BOOL res= [self.replayVideoView.videoLayer AddOneFrame:0 lpBuff:(unsigned char *)lpInBuffer2 nBuffSize:nBufSize2 frameRate:frameRate];

                return res?CloudSdk_ResultCode_Succ:CloudSdk_ResultCode_Failed;

            }break;
            case ALARMV2_DATA:
            {
                NSString * alarmStr = [NSString stringWithCString:lpInBuffer2 encoding:NSUTF8StringEncoding];
                               
                NSMutableDictionary * alarmMsgDic = [alarmStr mj_JSONObject];

//                {"uid":"036c0123287a8489aba5","alarm_data":[{"type":1,"param":3,"state":1,"time":"2020-08-31 18:22:29"}]}
                //人脸或者部分智能报警可能还有context节点 里面包含人脸图片等信息
                NSLog(@"ALARMV2_DATA msg[%@] \r\n",alarmStr);
            }
                break;
            default:
                break;
        }

        return CloudSdk_ResultCode_Succ;
    }];
    
    
    if(state){
        NSLog(@"流回调设置成功");
    }
}
#pragma mark -----------------


#pragma mark ----------开启预览
//开启设备预览  (单个)
-(void)openRealPlay:(NSMutableArray *)channelsArray finishHandle:(void(^)(void))finishHandle errorHandle:(void(^)(void))errorHandle{
    if(!self.deviceHandle){
        NSLog(@"设备未登录!!! \r\n");
        return;
    }
    
    int numOfChannels = (int)channelsArray.count;

    if(numOfChannels == 0){
        return;
    }

    HANDLE handle  = self.deviceHandle;

    CLOUDSDK_REALPLAY_COND  command[numOfChannels];
    memset(&command, 0, sizeof(CLOUDSDK_REALPLAY_COND)*numOfChannels);

    //计算到底有几个通道需要开流 只需要开1通道 则传数据[@(1)]
    int cmdIndex = 0;

    for (NSInteger i = 0 ; i < numOfChannels; i++) {
        int channel1 = [channelsArray[i] intValue] ;

        if(channel1 == 0){
           cmdIndex -- ;
           continue;
        }

        command[i].wChannel = channel1;//当前的渠道
        command[i].wStreamType = SUB_STREAM; //1 高清 2 标清

        cmdIndex ++;
    }
    
    BOOL res = [[CloudClientSDK4iOS getInstance] ClientRealPlay:handle lpRealPlayConds:command nCount:cmdIndex resCallBack:^(void *lpData, unsigned int nDataLen) {
        CLOUDSDK_RESULT * lpResult = (CLOUDSDK_RESULT *)lpData ;

        if(lpResult->wResult == 0){
            //开预览指令发送到设备成功
            if(finishHandle){
                finishHandle();
            }
        }else {
            //开预览指令发送到设备失败
            if(errorHandle){
                errorHandle();
            }
        }
    }];

    if(res){
        NSLog(@"调用开预览接口成功");
    }
}


/*
 关预览接口
 */
-(void)stopRealPlay:(HANDLE)handle
            channel:(NSMutableArray *)channels
          animation:(BOOL)animation{
    if(!channels){
        return ;
    }
    
    if(!handle){
        return ;
    }
    
    if(channels.count == 0){
        return;
    }
        
    CLOUDSDK_REALPLAY_COND  command[(int)channels.count] ;
    
    memset(&command, 0, sizeof(CLOUDSDK_REALPLAY_COND));
    
    int chIndex = 0;
    
    for (int i = 0;  i < channels.count; i++) {
        int channel = [channels[i] intValue];
        
        if(channel == 0){
            continue;
        }
        
        command[chIndex].wChannel = channel;//当前的渠道
        command[chIndex].wStreamType = SUB_STREAM; //1 高清 2 标清
    
        chIndex ++;
    }
  
    NSLog(@"ClientStopRealPlay [%@] \r\n",[channels componentsJoinedByString:@"_"]);
    BOOL result = [[CloudClientSDK4iOS getInstance] ClientStopRealPlay:handle lpRealPlayConds:command nCount:chIndex resCallBack:^(void *lpData, unsigned int nDataLen) {
        CLOUDSDK_RESULT * lpResult = (CLOUDSDK_RESULT *)lpData ;
        
        NSLog(@"openOrStop stop response-wres--%d response ret:%d len:%d  channel----%@\r\n", lpResult->wRes,lpResult->wResult, nDataLen,[channels componentsJoinedByString:@"_"]) ;
      
        if(lpResult->wResult == 0){
            //关流成功
        }
    }];
}


/*
 设置异常回调
 */
-(void)setExpetionCallBack{
    [[CloudClientSDK4iOS getInstance] setExceptionCallBack:^(void *hHandle) {
        NSLog(@"这里是异常回调，设备已掉线\r\n");
    }];
}
#pragma mark -----------------


/*
 查询文件信息
 */
-(void)queryReplayFile{
    CLOUDSDK_FIND_FILE_COND cond;
    memset(&cond, 0, sizeof(CLOUDSDK_FIND_FILE_COND));

    cond.wChannel = 1;//通道号
     
    //文件起始时间  2020-8-11 00:00:00
    cond.struBeginTime.dwYear = 2020;
    cond.struBeginTime.dwMonth = 8;
    cond.struBeginTime.dwDay = 11;
    cond.struBeginTime.dwHour = 0;
    cond.struBeginTime.dwMinute = 0;
    cond.struBeginTime.dwSecond = 0;

    //文件结束时间 2020-8-11 23:59:59 可以自行封装model对于结构体的转换或者赋值
    cond.struEndTime.dwYear = 2020;
    cond.struEndTime.dwMonth = 8;
    cond.struEndTime.dwDay = 11;
    cond.struEndTime.dwHour = 23;
    cond.struEndTime.dwMinute = 59;
    cond.struEndTime.dwSecond = 59;

    cond.dwFileType = 0xffffffff; //文件类型表示全部

    [[CloudClientSDK4iOS getInstance] ClientFindFile:self.deviceHandle lpFindFileConds:&cond nCount:1 resCallBack:^(void *lpData, unsigned int nDataLen) {
       if(nDataLen > 0){
           int count = (nDataLen) / sizeof(CLOUDSDK_FIND_FILE_DATA);
           
           if(count == 0){
               NSLog(@"没有文件片段!!!不能开启回放\r\n");
               return;
           }
           
           CLOUDSDK_FIND_FILE_DATA * fileData = (CLOUDSDK_FIND_FILE_DATA*)(lpData);
           
           for (int i = 0; i < count; ++i) {
               CLOUDSDK_FIND_FILE_DATA  tempFileData = fileData[i];
               
               NSLog(@"文件片段:[%d] 起始时间[%d]年 [%d]月 [%d]日 %d:%d:%d 时区[%d] 文件类型[%d] \r\n",i,tempFileData.struStartTime.dwYear,tempFileData.struStartTime.dwMonth,tempFileData.struStartTime.dwDay,tempFileData.struStartTime.dwHour,tempFileData.struStartTime.dwMinute,tempFileData.struStartTime.dwSecond,tempFileData.struStartTime.dwZoneTime,tempFileData.dwFileType);
               
               if(i==0){
                   [self openRecordByMode:0 startTime:tempFileData.struStartTime];
               }
           }
           
       }else{
           NSLog(@"没有文件片段!!!不能开启回放\r\n");
       }
    }];
}


//开回放  byModel传0  起始时间进回放默认传查询到的第一个时间片段
//
-(void)openRecordByMode:(BYTE)byModel startTime:(CLOUDSDK_TIME)startTime{ //0回放 1录像
    CLOUDSDK_REPLAY_COND cond ;
    memset(&cond, 0, sizeof(CLOUDSDK_REPLAY_COND));

    cond.byMode = byModel;
    
    cond.struReplayParam[0].wChannel = 1;
    cond.struReplayParam[0].wStreamType = SUB_STREAM;
    cond.wCount = 1;
    
    cond.struStartTime.dwYear = startTime.dwYear;
    cond.struStartTime.dwMonth = startTime.dwMonth;
    cond.struStartTime.dwDay = startTime.dwDay;
    cond.struStartTime.dwHour = startTime.dwHour;
    cond.struStartTime.dwMinute = startTime.dwMinute;
    cond.struStartTime.dwSecond = startTime.dwSecond;
    
    //通过开始时间计算获得当天的最大结束时间 传入
    cond.struStopTime.dwYear = 2020;
    cond.struStopTime.dwMonth = 8;
    cond.struStopTime.dwDay = 11;
    cond.struStopTime.dwHour = 23;
    cond.struStopTime.dwMinute = 59;
    cond.struStopTime.dwSecond = 59;
    
    bool res = [[CloudClientSDK4iOS getInstance] ClientReplay:self.deviceHandle lpReplayCond:&cond resCallBack:^(void *lpData, unsigned int nDataLen) {
        CLOUDSDK_REPLAY_RESULT * result = (CLOUDSDK_REPLAY_RESULT *)lpData;

        NSLog(@"openOrStop replay --- open result->wRes[%d] result->wResult[%d] result->id[%d]\r\n",result->wRes,result->wResult,result->dwReplayId);
        
        if(result->wResult==0){
            self.currentReplayId = result->dwReplayId;
        }else{
            self.currentReplayId = 0;
            NSLog(@"开回放失败\r\n");
        }
    }];

    if(!res){
        self.currentReplayId = 0;
    }
}


/*
 关回放接口
 */
-(void)stopRePlay:(DWORD)rePlayId completeHandle:(void(^)(void))completeHandle{
    if(!self.deviceHandle){
        NSLog(@"设备未登录");
        return;
    }
    
    if(!self.currentReplayId){
        return;
    }
    
    CLOUDSDK_STOP_REPLAY_COND cond;
    memset(&cond, 0, sizeof(CLOUDSDK_STOP_REPLAY_COND));
    
    cond.dwReplayId = rePlayId;
    
    self.currentReplayId = 0;
    
    [self.replayVideoView.videoLayer ClearAllFrames];
    
    [[CloudClientSDK4iOS getInstance] ClientStopReplay:self.deviceHandle lpStopRePlayCond:&cond resCallBack:^(void *lpData, unsigned int nDataLen) {
        CLOUDSDK_RESULT * lpResult = (CLOUDSDK_RESULT *)lpData ;

        NSLog(@"openOrStop replay --- stop result->wRes[%d] result->wResult[%d]\r\n",lpResult->wRes,lpResult->wResult);

        if(lpResult->wResult !=0){
           
        }else{
           
        }

    }];
}


//回放控制 开关音频 暂停 播放  快进 快退
//pauseOrPlay 暂停的时候 表示是否播放  快放慢放的时候表示倍速
-(void)rePlayControl:(DWORD)replayId cmd:(DWORD)replaycmd pauseOrPlay:(DWORD)pauseOrPlay{
    if(!self.deviceHandle){
          NSLog(@"设备未登录");
          return;
    }
    
    if(replayId == 0){
        NSLog(@"没有开回放");
        return;
    }
    
    CLOUDSDK_CONTROL_REPLAY_COND cond;
    memset(&cond, 0, sizeof(CLOUDSDK_CONTROL_REPLAY_COND));
    
    cond.dwReplayId = replayId;
    cond.dwReplayCmd = replaycmd;
 
    cond.dwParams[0] = SUB_STREAM;
    cond.dwParams[1] = 1;
    cond.dwParams[2] = pauseOrPlay;
    
    [[CloudClientSDK4iOS getInstance] ClientCtrlReplay:self.deviceHandle lpCtrlReplayCond:&cond resCallBack:^(void *lpData, unsigned int nDataLen) {
        CLOUDSDK_RESULT * lpResult = (CLOUDSDK_RESULT *)lpData ;

        NSLog(@"rePlayControl- response-wres--%d response ret:%d len:%d cmd-- [%d] \n ", lpResult->wRes,lpResult->wResult, nDataLen,replaycmd) ;

    }];
}

#pragma mark  ---- 点击事件
//开预览按钮点击
- (IBAction)clickOpenRealBtn:(UIButton *)sender {
    NSMutableArray * channelsArr = [NSMutableArray array];
    
    [channelsArr addObject:@(1)];
    
    self.videoView.channel = 1;
    [self.videoView.videoLayer ClearAllFrames];
    
    //默认开通道 1的码流
    [self openRealPlay:channelsArr finishHandle:^{
        
    } errorHandle:^{
        
    }];
}


- (IBAction)deviceLogin:(UIButton *)sender {
    [self queryDeviceP2PServer];
}


- (IBAction)clickReplayBtn:(UIButton *)sender {
    [self queryReplayFile];
}

/*
 停止预览
 */
- (IBAction)clickStopRealPlay:(UIButton *)sender {
    NSMutableArray * channelsArr = [NSMutableArray array];
    
    [channelsArr addObject:@(1)];
    
    self.videoView.channel = 1;
    [self.videoView.videoLayer ClearAllFrames];
    
    [self stopRealPlay:self.deviceHandle channel:channelsArr animation:NO];
}


/*
 停回放
 */
- (IBAction)clickStopRePlay:(UIButton *)sender {
    NSMutableArray * channelsArr = [NSMutableArray array];
    
    [channelsArr addObject:@(1)];
        
    [self stopRePlay:self.currentReplayId completeHandle:nil];
}


/*
 回放减速
 */
- (IBAction)clickSlowReplay:(UIButton *)sender {
    if(self.nSpeed > -4){
        self.nSpeed --;
    }
    
    NSString * speedStr = [self getSpeedFromeLevel:self.nSpeed];
    self.speedLabel.text = speedStr;

    NSLog(@"self.nSpeed [%d]\r\n",self.nSpeed);
    if(self.nSpeed >=0){
        //调用倍速播放接口
        [self rePlayControl:self.currentReplayId cmd:CloudSdk_ControlReplay_SetSpeed pauseOrPlay:pow(2, self.nSpeed)];
    }
    
    [self.replayVideoView.videoLayer SetPlaySpeed:self.nSpeed];
}


-(NSString *)getSpeedFromeLevel:(int)speedLevel{
    if(speedLevel >= 0){
       return [NSString stringWithFormat:@"倍速%.0f",pow(2, speedLevel)];
    }else{
        return [NSString stringWithFormat:@"倍速- %.0f",pow(2, -speedLevel)];;
    }
}
/*
 回放加速
 */
- (IBAction)clickSpeedUpReplay:(UIButton *)sender {
    if(self.nSpeed < 4){
        self.nSpeed ++;
    }
      
    self.speedLabel.text = [self getSpeedFromeLevel:self.nSpeed];
    
    if(self.nSpeed >=0){
        //调用倍速播放接口
        [self rePlayControl:self.currentReplayId cmd:CloudSdk_ControlReplay_SetSpeed pauseOrPlay:pow(2, self.nSpeed)];
    }
      
    [self.replayVideoView.videoLayer SetPlaySpeed:self.nSpeed];
}


- (IBAction)clickAlarmBtn:(UIButton *)sender {
    [self subScibeAlarmV2];
}


@end
