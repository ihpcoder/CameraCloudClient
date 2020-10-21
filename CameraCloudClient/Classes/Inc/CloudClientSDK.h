#ifndef Cloud_Client_SDK_H
#define Cloud_Client_SDK_H

#include "CloudSDKCommon.h"


#ifdef WIN32
#ifdef __cplusplus
#if defined(CLOUD_CLIENT_SDK_EXPORTS)
#define CLOUDCLIENT_API extern "C" __declspec(dllexport)
#else
#define CLOUDCLIENT_API extern "C"
#endif
#else
#define CLOUDCLIENT_API
#endif
#define CLOUDCLIENT_CALL 
#else/*Linux*/
#define CALLBACK 
#ifdef __cplusplus
#define CLOUDCLIENT_API extern "C"
#else
#define CLOUDCLIENT_API
#endif
#define CLOUDCLIENT_CALL
#endif


namespace CloudSDK
{
////////////////////////////////////////////////////////////////////////////////
// 回调函数定义
////////////////////////////////////////////////////////////////////////////////

/*!
* 用户流数据回调
* \param hUserId		注册用户ID句柄
* \param lpInBuffer1	数据1有效指针地址
* \param nBufSize1		数据1有效长度
* \param lpInBuffer2	数据2有效指针地址
* \param nBufSize2		数据2有效长度
* \param lpUser			回调函数上下文
* \return 返回cloudsdk_result_code_t错误码
*/
typedef int (CALLBACK *fStreamDataCallBack)(IN HANDLE hUserId, IN char *lpInBuffer1, IN int nBufSize1, IN char *lpInBuffer2, IN int nBufSize2, IN VOID *lpUser) ;


/*!
* 异常句柄回调
* \param hHandle		注册ID句柄/预览ID句柄/回放ID句柄
* \param lpUser			回调函数上下文
* \return 返回VOID类型
*/
typedef VOID (CALLBACK *fExceptionHandleCallBack)(IN HANDLE hHandle, IN VOID *lpUser) ;


/*!
* 设备搜索回调
* \param lpDevUid		设备序列号
* \param lpBuf			设备额外数据(为之后便于扩展,暂时未使用)
* \param nBufSize		设备额外数据长度(为之后便于扩展,暂时未使用)
* \param lpUser			回调函数上下文
* \return 返回VOID类型
*/
typedef VOID (CALLBACK *fSearchDevCallBack)(IN char *lpDevUid, IN BYTE *lpBuf, IN int nBufSize, IN VOID *lpUser) ;


/*!
* \brief
* \param nMaxClientNum 				注册用户连接总数(包括用户注册 预览 回放)
* \param lpDirPath                  系统可读写路径
* 客户端库初始化
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_Init(IN int nMaxClientNum, IN char *lpDirPath = NULL) ;


/*!
* \brief
* 客户端库反初始化
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_UnInit( ) ;


/*!
* \brief
* 注册异常句柄回调函数
* \param fExceptionHandleCallBack	异常句柄回调函数指针
* \param lpUser						回调函数上下文
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_SetExceptionHandleCallBack(IN fExceptionHandleCallBack fpExceptionHandleCallBack, IN VOID *lpUser) ;


/*!
* \brief
* 查询设备对应的注册服务器和中心服务器
* \param lpUid				设备Uid
* \param lpServerInfo	设备对应服务器相关信息
* \param nTimeOut		查询超时时间（单位:秒）
* \return 成功返回TRUR,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_QueryServer(IN char *lpUid, INOUT CLOUDSDK_SERVER_INFO *lpServerInfo, IN int nTimeOut = 3) ;


/*!
* \brief
* 查询设备对应的注册服务器
* \param lpUid						设备Uid
* \param lpSuperServerAddr	设备对应P2P服务器地址
* \param nAddrLen				设备对应P2P服务器地址长度
* \param nTimeout				查询超时时间（单位:秒）
* \return 成功返回TRUR,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_QueryP2PServer(IN char *lpUid, IN char *lpSuperServerAddr, INOUT char *lpP2PServerAddr, IN int nAddrLen, INOUT int *lpP2PServerPort, IN int nTimeout = 3) ;



/*!
* \brief
* 通过P2P服务器地址查询设备状态(0x02-在线或者0x01-离线)
* \param lpUid						设备Uid
* \param lpP2PServerAddr	设备对应P2P服务器地址
* \param nAddrLen				设备对应P2P服务器地址长度
* \param nTimeOut				查询超时时间（单位:秒）
* \return 成功返回TRUR,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_QueryDeviceState(IN char *lpUid, INOUT char *lpP2PServerAddr, INOUT int *lpState, IN int nTimeout = 3) ;



/*!
* \brief
* 注册函数
* \param lpSuperServer	中心服务器的IP地址
* \param lpUid			设备Uid
* \param lpUserName		登陆用户名
* \param lpPassword		登陆密码
* \param dwConnectTime	连接超时时间(豪秒单位,高16位表示连接时间,低16位表示等待响应时间)
* \param lpDeviceInfo	设备信息
* \return 成功返回hUserId,失败返回NULL,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API HANDLE CLOUDCLIENT_CALL CloudClient_Login(IN char *lpSuperServer, IN char *lpUid, IN char *lpUserName, IN char *lpPassword, IN DWORD dwConnectTime, INOUT CLOUDSDK_DEVICE_INFO *lpDeviceInfo) ;


/*!
* \brief
* 注册函数
* \param lpP2PServer	注册服务器的IP地址，已知设备所在注册服务器情况下使用
* \param lpUid			设备Uid
* \param lpUserName		登陆用户名
* \param lpPassword		登陆密码
* \param dwConnectTime	连接超时时间(豪秒单位,高16位表示连接时间,低16位表示等待响应时间)
* \param lpDeviceInfo	设备信息
* \return 成功返回hUserId,失败返回NULL,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API HANDLE CLOUDCLIENT_CALL CloudClient_Login4P2PServer(IN char *lpP2PServer, IN char *lpUid, IN char *lpUserName, IN char *lpPassword, IN DWORD dwConnectTime, INOUT CLOUDSDK_DEVICE_INFO *lpDeviceInfo) ;


/*!
* \brief
* 局域网直连注册函数
* \param lpDeviceIP	    设备IP地址
* \param u16Port		设备监听端口
* \param lpUserName		登陆用户名
* \param lpPassword		登陆密码
* \param dwConnectTime	连接超时时间(豪秒单位,高16位表示连接时间,低16位表示等待响应时间)
* \param lpDeviceInfo	设备信息
* \return 成功返回hUserId,失败返回NULL,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API HANDLE CLOUDCLIENT_CALL CloudClient_Login4Direct(IN char *lpDeviceIP, IN unsigned short u16Port, IN char *lpUserName, IN char *lpPassword, IN DWORD dwConnectTime, INOUT CLOUDSDK_DEVICE_INFO *lpDeviceInfo) ;


/*!
* \brief 
* 注销函数
* \param hUserId 注册句柄ID
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_Logout(IN HANDLE hUserId) ;


/*!
* \brief
* PTZ控制函数
* \param hHandle			注册ID句柄/预览ID句柄
* \param struCtrlPTZConds	PTZ数组首地址
* \param dwCount			PTZ数组大小
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_ControlPTZ(IN HANDLE hHandle, IN CLOUDSDK_CONTROL_PTZ_COND struCtrlPTZConds[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 设置流数据回调(预览/回放/对讲公用回调)
* \param hHandle					注册ID句柄/预览ID句柄/回放ID句柄
* \param fpStreamDataDataCallBack	流数据回调函数
* \param lpUser						流数据回调上下文
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_SetStreamDataCallBack(IN HANDLE hHandle, IN fStreamDataCallBack fpStreamDataCallBack, IN VOID *lpUser) ;


/*!
* \brief
* 实时流打开函数
* \param hUserId				注册ID句柄
* \param struRealPlayConds		实时流请求数组指针                                                                                                                                                                                                                                                                                                opp
* \param dwCount				实时流请求数组大小
* \param lpSeqNo				控制消息序号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_RealPlay(IN HANDLE hUserId, IN CLOUDSDK_REALPLAY_COND struRealPlayConds[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 实时流控制函数
* \param hHandle					注册ID句柄/预览ID句柄
* \param struControlRealPlayConds	实时流控制数组
* \param dwCount					实时流控制数组大小
* \param lpSeqNo					控制消息序号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_ControlRealPlay(IN HANDLE hHandle, IN CLOUDSDK_CONTROL_REALPLAY_COND struCtrlRealPlayConds[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 实时流关闭函数(关闭指定通道实时流)
* \param hUserId				注册ID句柄
* \param struRealPlayConds		实时流请求数组指针
* \param dwCount				实时流请求数组大小
* \param lpSeqNo				控制消息序号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_StopRealPlay(IN HANDLE hUserId, IN CLOUDSDK_REALPLAY_COND struRealPlayConds[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 实时流打开v2函数(新建链接处理预览,返回新连接的句柄,该句柄只能预览/PTZ控制/语音对讲模块,且需要重新设置流回调)
* \param lpSuperServer			中心服务器的IP地址
* \param lpUid					设备Uid
* \param lpUserName				登陆用户名
* \param lpPassword				登陆密码
* \param dwConnectTime			连接超时时间(豪秒单位 高16位表示连接时间,低16位表示等待响应时间)
* \param struRealPlayConds		实时流请求数组指针
* \param dwCount				实时流请求数组大小
* \param lpSeqNo				控制消息序号
* \return 成功返回UserId,失败返回NULL,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API HANDLE CLOUDCLIENT_CALL CloudClient_RealPlayv2(IN char *lpSuperServer, IN char *lpUid, IN char *lpUserName, IN char *lpPassword, IN DWORD dwConnectTime, IN CLOUDSDK_REALPLAY_COND struRealPlayConds[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 实时流打开v2函数(新建链接处理预览,返回新连接的句柄,该句柄只能预览/PTZ控制/语音对讲模块,且需要重新设置流回调)
* \param lpP2PServer			P2P服务器的IP地址
* \param lpUid						设备Uid
* \param lpUserName			登陆用户名
* \param lpPassword			登陆密码
* \param dwConnectTime		连接超时时间(豪秒单位 高16位表示连接时间,低16位表示等待响应时间)
* \param struRealPlayConds	实时流请求数组指针
* \param dwCount				实时流请求数组大小
* \param lpSeqNo				控制消息序号
* \return 成功返回UserId,失败返回NULL,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API HANDLE CLOUDCLIENT_CALL CloudClient_RealPlayv24P2PServer(IN char *lpP2PServer, IN char *lpUid, IN char *lpUserName, IN char *lpPassword, IN DWORD dwConnectTime, IN CLOUDSDK_REALPLAY_COND struRealPlayConds[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 直连方式实时流打开v2函数(新建链接处理预览,返回新连接的句柄,该句柄只能预览/PTZ控制/语音对讲模块,且需要重新设置流回调)
* \param lpDeviceIP				设备IP地址
* \param u16Port				设备监听端口号
* \param lpUserName				登陆用户名
* \param lpPassword				登陆密码
* \param dwConnectTime			连接超时时间(豪秒单位 高16位表示连接时间,低16位表示等待响应时间)
* \param struRealPlayConds		实时流请求数组指针
* \param dwCount				实时流请求数组大小
* \param lpSeqNo				控制消息序号
* \return 成功返回UserId,失败返回NULL,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API HANDLE CLOUDCLIENT_CALL CloudClient_RealPlayv24Direct(IN char *lpDeviceIP, IN unsigned short u16Port, IN char *lpUserName, IN char *lpPassword, IN DWORD dwConnectTime, IN CLOUDSDK_REALPLAY_COND struRealPlayConds[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;



/*!
* \brief
* 实时流关闭函数(关闭所有实时流)
* \param hRealPlay				预览ID句柄
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_StopRealPlayv2(IN HANDLE hRealPlay) ;


/*!
* \brief
* 语音对讲打开函数
* \param hHandle				注册ID句柄/预览ID句柄
* \param struTalkConds			语音对讲请求数组指针
* \param dwCount				语音对讲请求数组大小
* \param lpSeqNo				控制消息序号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_Talk(IN HANDLE hHandle, IN CLOUDSDK_TALK_COND struTalkConds[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 语音对讲数据发送函数
* \param hHandle				注册ID句柄/预览ID句柄
* \param lpHeader				语音对讲包头数据
* \param dwHeadSize				包头大小	
* \param lpBuffer				语音对讲包体数据
* \param dwBufferSize			包体大小
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_SendTalkData(IN HANDLE hHandle, IN BYTE *lpHeader, IN DWORD dwHeadSize, IN BYTE *lpBuffer, IN DWORD dwBufferSize) ;


/*!
* \brief
* 语音对讲关闭函数
* \param hHandle				注册ID句柄/预览ID句柄
* \param struStopTalks			语音对讲请求数组指针
* \param dwCount				语音对讲请求数组大小
* \param lpSeqNo				控制消息序号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_StopTalk(IN HANDLE hHandle, IN CLOUDSDK_TALK_COND struStopTalks[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 历史文件查询函数
* \param hUserId				注册ID句柄
* \param struFindFileConds		历史文件查询数组指针
* \param dwCount				历史文件查询数组大小
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_FindFile(IN HANDLE hUserId, IN CLOUDSDK_FIND_FILE_COND struFindFileConds[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 历史流打开函数(在已有连接上打开,响应消息为CLOUDSDK_REPLAY_RESULT,一次只能打开一组回放)
* \param hUserId				注册ID句柄
* \param lpReplayCond			历史流请求参数指针(同一通道同一码流类型不能请求多次)
* \param lpSeqNo				控制消息序号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_Replay(IN HANDLE hUserId, IN LPCLOUDSDK_REPLAY_COND lpReplayCond, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 历史流控制函数
* \param hUserId				注册ID句柄/回放v2接口返回ID句柄
* \param lpControlReplayCond	历史流控制数组
* \param lpSeqNo				控制消息序号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_ControlReplay(IN HANDLE hUserId, IN LPCLOUDSDK_CONTROL_REPLAY_COND lpCtrlReplayCond, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 历史流关闭函数(指定通道码流类型关闭回放)
* \param hUserId				注册ID句柄
* \param lpStopRePlayCond		历史流请求数组指针
* \param lpSeqNo				控制消息序号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_StopReplay(IN HANDLE hUserId, IN LPCLOUDSDK_STOP_REPLAY_COND lpStopRePlayCond, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 历史流打开v2函数(新建链接处理回放,返回新连接的句柄,该句柄只能操作回放模块,且需要重新设置流回调,其他与v1接口类似)
* \param lpSuperServer			中心服务器的IP地址
* \param lpUid					设备Uid
* \param lpUserName				登陆用户名
* \param lpPassword				登陆密码
* \param dwConnectTime			连接超时时间(豪秒单位,高16位表示连接时间,低16位表示等待响应时间)
* \param lpReplayCond			历史流请求结构指针
* \param lpSeqNo				控制消息序号
* \return 成功返回hUserId,失败返回NULL,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API HANDLE CLOUDCLIENT_CALL CloudClient_Replayv2(IN char *lpSuperServer, IN char *lpUid, IN char *lpUserName, IN char *lpPassword, IN DWORD dwConnectTime, IN LPCLOUDSDK_REPLAY_COND lpReplayCond, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 历史流打开v2函数(新建链接处理回放,返回新连接的句柄,该句柄只能操作回放模块,且需要重新设置流回调,其他与v1接口类似)
* \param lpP2PServer			P2P服务器的IP地址
* \param lpUid						设备Uid
* \param lpUserName			登陆用户名
* \param lpPassword			登陆密码
* \param dwConnectTime		连接超时时间(豪秒单位,高16位表示连接时间,低16位表示等待响应时间)
* \param lpReplayCond			历史流请求结构指针
* \param lpSeqNo				控制消息序号
* \return 成功返回hUserId,失败返回NULL,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API HANDLE CLOUDCLIENT_CALL CloudClient_Replayv24P2PServer(IN char *lpP2PServer, IN char *lpUid, IN char *lpUserName, IN char *lpPassword, IN DWORD dwConnectTime, IN LPCLOUDSDK_REPLAY_COND lpReplayCond, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 直连方式历史流打开v2函数(新建链接处理回放,返回新连接的句柄,该句柄只能操作回放模块,且需要重新设置流回调,其他与v1接口类似)
* \param lpDeviceIP				设备IP地址
* \param u16Port				设备监听端口号
* \param lpUserName				登陆用户名
* \param lpPassword				登陆密码
* \param dwConnectTime			连接超时时间(豪秒单位,高16位表示连接时间,低16位表示等待响应时间)
* \param lpReplayCond			历史流请求结构指针
* \param lpSeqNo				控制消息序号
* \return 成功返回hUserId,失败返回NULL,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API HANDLE CLOUDCLIENT_CALL CloudClient_Replayv24Direct(IN char *lpDeviceIP, IN unsigned short u16Port, IN char *lpUserName, IN char *lpPassword, IN DWORD dwConnectTime, IN LPCLOUDSDK_REPLAY_COND lpReplayCond, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 历史流关闭函数(关闭该会话内所有回放)
* \param hReplay				回放ID句柄
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_StopReplayv2(IN HANDLE hReplay) ;


/*!
* \brief
* 订阅报警
* \param hHandle			注册ID句柄&预览ID句柄
* \param struAlarms			推送关联报警结构数组
* \param dwCount			推送关联报警结构数组个数
* \param lpSeqNo			控制消息序号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_SubscribeAlarm(IN HANDLE hHandle, IN CLOUDSDK_ALARM_COND struAlarms[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 取消订阅报警
* \param hHandle			注册ID句柄&预览ID句柄
* \param struAlarms			推送关联报警结构数组
* \param dwCount			推送关联报警结构数组个数
* \param lpSeqNo			控制消息序号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_UnsubscribeAlarm(IN HANDLE hHandle, IN CLOUDSDK_ALARM_COND struAlarms[], IN DWORD dwCount, INOUT DWORD *lpSeqNo) ;

	/*!
	* \brief
	* 订阅报警
	* \param hHandle			注册ID句柄&预览ID句柄
	* \param struAlarmRules		报警订阅参数
	* \param lpSeqNo			控制消息序号
	* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
	*/
	CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_SubscribeAlarmv2(IN HANDLE hHandle, IN LPCLOUDSDK_ALARM_COND_V2 lpAlarmRules, INOUT DWORD *lpSeqNo);

	/*!
	* \brief
	* 透传控制
	* \param hUserId			注册ID句柄
	* \param lpTranControlCond	透传数据结构
	* \param lpSeqNo			设备序列号
	* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
	*/
	CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_TranControl(IN HANDLE hUserId, IN LPCLOUDSDK_TRAN_CONTROL_COND lpTranControlCond, INOUT DWORD *lpSeqNo);


/*!
* \brief
* 透传控制
* \param hUserId			注册ID句柄
* \param lpTranControlCond	透传数据结构
* \param lpSeqNo			设备序列号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_TranControlv2(IN HANDLE hUserId, IN LPCLOUDSDK_TRAN_CONTROL_COND lpTranControlCond, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 透传v3接口，支持请求/响应/通知消息
* \param hUserId			注册ID句柄
* \param dwOpt				bit0-bit1 描述消息类型  0-请求 1-响应 2-通知
* \param lpTranControlCond	透传数据结构
* \param lpSeqNo			设备序列号，若为响应/通知消息，该值无效
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_TranControlv3(IN HANDLE hUserId, IN DWORD dwOpt ,IN LPCLOUDSDK_TRAN_CONTROL_COND lpTranControlCond, INOUT DWORD *lpSeqNo) ;


/*!
* \brief
* 透传控制
* \param hUserId			注册ID句柄
* \param lpTranControlCond	透传数据结构
* \param lpSeqNo			设备序列号
* \return 成功返回TRUE,失败返回FALSE,获取错误码调用CloudClient_GetLastError
*/
CLOUDCLIENT_API BOOL CLOUDCLIENT_CALL CloudClient_TranControlv4(IN HANDLE hUserId, IN LPCLOUDSDK_TRAN_CONTROL_COND_V2 lpTranControlCond, INOUT DWORD *lpSeqNo);


/*!
* \brief
* 获取一条响应函数,与查询响应消息接口不能同时使用
* \param lpSeqNo				响应消息序号
* \param lpBuffer				响应消息缓冲,具体响应结构参照SeqNo对应的消息请求响应结构（如:预览控制对应公共的CLOUDSDK_RESULT）
* \param dwInSize				响应消息缓冲大小
* \param lpOutSize				实际传出响应消息大小
* \param dwTimeout				超时时间
* \return 成功返回0,失败返回错误码,若错误码为CloudSdk_ResultCode_BufTooSmall时，lpOutSize传出为实际需要的缓冲区长度，须再次获取
*/
CLOUDCLIENT_API int CLOUDCLIENT_CALL CloudClient_GetResponse(OUT DWORD *lpSeqNo, OUT DWORD *lpCmd, OUT BYTE *lpBuffer, IN DWORD dwInSize, OUT DWORD *lpOutSize, IN DWORD dwTimeout) ;


/*!
* \brief
* 查询响应消息,与获取一条响应消息接口不能同时使用
* \param dwSeqNo				查询响应消息序号
* \param lpBuffer				响应消息缓冲
* \param dwInSize				响应消息缓冲大小
* \param lpOutSize				实际传出响应消息大小
* \param dwTimeout				超时时间
* \return 成功返回0,失败返回错误码,若错误码为CloudSdk_ResultCode_BufTooSmall时，lpOutSize传出为实际需要的缓冲区长度，须再次获取
*/
CLOUDCLIENT_API int CLOUDCLIENT_CALL CloudClient_GetResponsev2(IN DWORD dwSeqNo, OUT DWORD *lpCmd, OUT BYTE *lpBuffer, IN DWORD dwInSize, OUT DWORD *lpOutSize, IN DWORD dwTimeout) ;


/*!
* \brief
* 获取错误码函数
* \return 返回错误码,参考cloudsdk_result_code_t
*/
CLOUDCLIENT_API int CLOUDCLIENT_CALL CloudClient_GetLastError( ) ;


/*!
* \brief
* 开启搜索设备函数
* \param fpSearchDevCallBack	搜索设备回调指针地址
* \param lpUser					搜索设备上下文地址
* \return 返回错误码,参考cloudsdk_result_code_t
*/
CLOUDCLIENT_API int CLOUDCLIENT_CALL CloudClient_StartSearch(IN WORD wSearchPort, IN fSearchDevCallBack fpSearchDevCallBack, IN VOID *lpUser) ;


/*!
* \brief
* 关闭搜索设备函数
* \return 返回错误码,参考cloudsdk_result_code_t
*/
CLOUDCLIENT_API int CLOUDCLIENT_CALL CloudClient_StopSearch( ) ;

	/*!
	* \brief
	* 未登陆条件下,配置设备信息函数
	* \param lpDeviceIP		设备IP
	* \param wPort			设备配置端口
	* \param dwCmd			配置命令
	* \param lpInParam		输入参数
	* \param dwInLength		输入参数长度
	* \param lpOutParam		输出参数
	* \param dwOutLength	输出参数长度
	* \param dwTimeOut		超时时间
	* \return 返回错误码,参考cloudsdk_result_code_t
	*/
	CLOUDCLIENT_API int CLOUDCLIENT_CALL CloudClient_ConfigureDevice(IN char *lpDeviceIP, IN WORD wPort, IN DWORD dwCmd, IN VOID *lpInParam, IN DWORD dwInLength, IN VOID *lpOutParam, IN DWORD dwOutLength, IN DWORD dwTimeOut);

/*!
* \brief 获取版本信息
* \param lpBuildDate 编译日期
* \param lpVersion	 版本号
*/
CLOUDCLIENT_API void CLOUDCLIENT_CALL CloudClient_GetVersion(OUT int *lpBuildDate, OUT int *lpVersion) ;


}

#endif

