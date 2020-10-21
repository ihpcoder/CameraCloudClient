#pragma once

#import <objc/NSObject.h>
#import <Foundation/NSString.h>
#import <Foundation/NSLock.h>
#import <Foundation/NSThread.h>
#import <Foundation/Foundation.h>
#import <dispatch/once.h>

#include <string>
#import <map>

#import "CloudSdkCommon.h"
#import "CloudClientSDK.h"
#import "ClientSession.h"

#define MAX_CLIENT_NUM  16

#ifdef DEBUG
#define NSLog(...) NSLog(__VA_ARGS__)
//#define NSLog(...)

#else
#define NSLog(...)

#endif

typedef enum : NSUInteger {
    DeviceServerStateOffline = 1,//设备离线
    DeviceServerStateOnline  = 2//设备在线
} DeviceServerStatus;

@interface CloudClientSDK4iOS : NSObject<NSCopying,NSMutableCopying>
@property (copy, nonatomic) fSearchDevCallBack lpSearchCbFun;
@property (nonatomic,assign) BOOL canNetWork;//是否有网络

@property (nonatomic,copy) void (^lastErrorCodeHandle)(void);
@property (nonatomic,copy) fExceptionHandleCallBack fExceptionCallBack;

@property (nonatomic,assign) int nTranControlCount;//透传计数 只能1次透传命令结束后再进行下一次透传命令

+(instancetype)getInstance;

/*
 客户端登录
 */
- (void*)ClientLogin: (NSString*) lpServerIP lpUid: (NSString*)lpUid lpName: (NSString*)lpName lpPassWord: (NSString*)lpPassWord  nConnectTime: (int) nConnectTime lpDeviceInfo: (CLOUDSDK_DEVICE_INFO*)lpDeviceInfo errorNo:(int *)errorNo;

/*
 客户端登出
 */
- (bool)ClientLogout: (void*) hLogin ;

- (bool)ClientControlPTZ: (void*)hLogin lpCtrlPTZConds:(CLOUDSDK_CONTROL_PTZ_COND*)lpCtrlPTZConds resCallBack: (fResponseCallBack) resCallBack;

/*
 开预览接口
 hLogin 句柄
 lpRealPlayConds 结构体指针
 nCount 表示结构体个数
 resCallBack 结果回调
 */
- (bool)ClientRealPlay: (void*)hLogin lpRealPlayConds:(CLOUDSDK_REALPLAY_COND*)lpRealPlayConds nCount:(int)nCount resCallBack: (fResponseCallBack) resCallBack;

- (bool)ClientCtrlRealPlay: (void*)hLogin lpCtrlRealPlayConds:(CLOUDSDK_CONTROL_REALPLAY_COND*)lpCtrlRealPlayConds nCount:(int)nCount resCallBack: (fResponseCallBack) resCallBack;

/*
关预览接口
hLogin 句柄
lpRealPlayConds 结构体指针
nCount 表示结构体个数
resCallBack 结果回调
*/
- (bool)ClientStopRealPlay:(void*)hLogin lpRealPlayConds:(CLOUDSDK_REALPLAY_COND*)lpRealPlayConds nCount:(int)nCount resCallBack: (fResponseCallBack) resCallBack;

- (void*)ClientRealPlayv2: (NSString*) lpServerIP lpUid: (NSString*)lpUid lpName: (NSString*)lpName lpPassWord: (NSString*)lpPassWord  nConnectTime: (int) nConnectTime lpRealPlayConds: (CLOUDSDK_REALPLAY_COND*)lpRealPlayConds nCount:(int)nCount streamCallBack: (fStreamDataCallBack) streamCallBack exceptionCallBack: (fExceptionHandleCallBack) exceptionCallBack resCallBack: (fResponseCallBack) resCallBack;

- (bool)ClientStopRealPlayv2: (void*) hLogin ;

/*
 开对讲
 */
- (bool)ClientTalk: (void*)hLogin lpTalkConds:(CLOUDSDK_TALK_COND*)lpTalkConds nCount:(int)nCount resCallBack: (fResponseCallBack) resCallBack;


/*
 发送iPhone采集的音频数据给设备
 */
- (bool)ClientSendTalkData: (void*)hLogin lpHeader:(BYTE*)lpHeader nHeadSize:(int)nHeadSize lpBuffer:(BYTE*)lpBuffer nBufferSize:(int)nBufferSize;


/*
 关对讲
 */
- (bool)ClientStopTalk: (void*)hLogin lpStopTalks:(CLOUDSDK_TALK_COND*)lpStopTalks nCount:(int)nCount resCallBack: (fResponseCallBack) resCallBack;


/*
 文件查询
 */
- (bool)ClientFindFile: (void*)hLogin lpFindFileConds:(CLOUDSDK_FIND_FILE_COND*)lpFindFileConds nCount:(int)nCount resCallBack: (fResponseCallBack) resCallBack;


/*
 开回放
 */
- (bool)ClientReplay: (void*)hLogin lpReplayCond:(LPCLOUDSDK_REPLAY_COND)lpReplayCond  resCallBack: (fResponseCallBack) resCallBack;


/*
 回放控制 倍速播放和暂停、播放等操作
 */
- (bool)ClientCtrlReplay: (void*)hLogin lpCtrlReplayCond:(LPCLOUDSDK_CONTROL_REPLAY_COND)lpCtrlReplayCond  resCallBack: (fResponseCallBack) resCallBack;


/*
 关回放
 */
- (bool)ClientStopReplay: (void*)hLogin lpStopRePlayCond:(LPCLOUDSDK_STOP_REPLAY_COND)lpStopRePlayCond  resCallBack: (fResponseCallBack) resCallBack;


- (HANDLE)ClientReplayv2: (NSString*) lpServerIP lpUid: (NSString*)lpUid lpName: (NSString*)lpName lpPassWord: (NSString*)lpPassWord  nConnectTime: (int) nConnectTime lpReplayCond: (LPCLOUDSDK_REPLAY_COND)lpReplayCond  streamCallBack: (fStreamDataCallBack) streamCallBack exceptionCallBack: (fExceptionHandleCallBack) exceptionCallBack resCallBack: (fResponseCallBack) resCallBack;

- (bool)ClientStopReplayv2: (void*) hLogin ;


/*
 透传命令
 */
- (bool)ClientTranControl: (void*)hLogin lpTranControlCond:(LPCLOUDSDK_TRAN_CONTROL_COND)lpTranControlCond  resCallBack: (fResponseCallBack) resCallBack;


/*
 局域网搜索指令
 */
- (bool)ClientStartSearch: (int)nSearchPort  searchCallBack: (fSearchDevCallBack) searchCallBack ;


/*
 局域网搜索停止
 */
- (bool)ClientStopSearch ;



-(BOOL)setExceptionCallBack:(fExceptionHandleCallBack)callback;//设置异常回调
-(BOOL)setStreamCallBack:(HANDLE)handle callBack:(fStreamDataCallBack)streamCallBack;//设置流回调


/*
 透传方法V2
 */
- (bool)ClientTranControlV2: (void*)hLogin lpTranControlCond:(LPCLOUDSDK_TRAN_CONTROL_COND)lpTranControlCond  resCallBack: (fResponseCallBack) resCallBack;//透传V2接口

-(void)ClientStopTranControlV2;


/*
 局域网直连登录
 通过局域网IP地址和端口进行登录
 也可以通过端口映射到路由器后 通过广域网进行登录
 */
- (void*)ClientLogin4Direct: (NSString*) lpDeviceIP nPort: (short)u16Port lpName: (NSString*)lpName lpPassWord: (NSString*)lpPassWord  nConnectTime: (int) nConnectTime lpDeviceInfo: (CLOUDSDK_DEVICE_INFO*)lpDeviceInfo errorNo:(int *)errorNo;

-(void)releaseTranV2Count;

/*
 查询设备的中心服务器和P2P服务器地址
 */
-(BOOL)CloudClient_QueryServer:(NSString *)uid serverInfo:(CLOUDSDK_SERVER_INFO *)lpServerInfo;
- (bool)ClientTranControlNoInfo: (void*)hLogin lpTranControlCond:(LPCLOUDSDK_TRAN_CONTROL_COND)lpTranControlCond  resCallBack: (fResponseCallBack) resCallBack;

/*
 手动查询设备的注册服务器地址 需要填入中心服务器地址
 */
-(BOOL)CloudClient_QueryP2PServer:(NSString *)uid superServerAddress:(NSString *)superServerAddress p2pServerAddress:(char *)lpP2PServerAddress p2pServerPort:(int *)p2pServerPort;

/*
 查询设备的状态
 */
-(BOOL)CloudClient_QueryDeviceState:(NSString *)uid P2PServerAddr:(NSString *)P2PServerAddr state:(int *)state;

- (bool)ClientTranControlV4: (void*)hLogin lpTranControlCond:(CLOUDSDK_TRAN_CONTROL_COND_V2)lpTranControlCond  resCallBack: (fResponseCallBack) resCallBack;

//不需要判断当前的计数 直接请求数据
- (bool)ClientTranControlV4NoWait: (void*)hLogin lpTranControlCond:(CLOUDSDK_TRAN_CONTROL_COND_V2)lpTranControlCond  resCallBack: (fResponseCallBack) resCallBack;

-(BOOL)CloudClient_ConfigureDevice:(NSString *)ipAddr wPort:(DWORD)dwPort cmd:(DWORD)dwCmd lpInBufferDic:(NSMutableDictionary *)lpInParam;

/*
 报警消息订阅
 */
-(bool)subScribeAlarmV2:(HANDLE)handle playParamter:(CLOUDSDK_ALARM_COND_V2)alarmCmd resCallBack:(fResponseCallBack)resCallBack;

@end
