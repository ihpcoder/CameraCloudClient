#ifndef CloudSDK_Common_H
#define CloudSDK_Common_H

#ifndef VOID
typedef void VOID;
#endif

#ifndef U64
typedef unsigned long long U64 ;
#endif

#ifndef U32
typedef unsigned int U32;
#endif

#ifndef U16
typedef unsigned short U16 ;
#endif

#ifndef U8
typedef unsigned char U8 ;
#endif

#ifndef S32
typedef int S32;
#endif

#ifndef S16
typedef short S16 ;
#endif

#ifndef S8
typedef char S8 ;
#endif

#if (defined(WIN32) || defined(_WIN64))
#include <Windows.h>
typedef HANDLE pthread_t ;
#if (!defined(_WIN64))
typedef int intptr_t;
#endif
#else
#define CALLBACK
#include <pthread.h>

#ifndef DWORD
typedef unsigned int DWORD;
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef USHORT
typedef unsigned short USHORT;
#endif

#ifndef LONG
typedef int LONG;
#endif

//#ifndef BOOL
//typedef int BOOL;
//#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef HANDLE
typedef void* HANDLE;
#endif

#ifndef INFINITE
#ifdef WIN32
#define INFINITE 0xFFFFFFFFFFFFFFFF
#else
#define INFINITE 0xFFFFFFFF
#endif
#endif

#ifndef INVALID_HANDLE_VALUE
#ifdef WIN32
#define INVALID_HANDLE_VALUE    (VOID *)0xFFFFFFFFFFFFFFFF
#else
#define INVALID_HANDLE_VALUE    (VOID *)0xFFFFFFFF
#endif
#endif


#ifndef NULL
#define NULL    0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#endif



////////////////////////////////////////////////////////////////////////////////
// 宏定义
////////////////////////////////////////////////////////////////////////////////

//!输入参数
#ifndef IN
#define IN
#endif

//!输出参数
#ifndef OUT
#define OUT
#endif

//!输入输出参数
#ifndef INOUT
#define INOUT
#endif


#define CLOUDSDK_MAX_UUID_LEN               48
#define ZERO_CHANNEL                    0xFFFF

////////////////////////////////////////////////////////////////////////////////
// 枚举定义
////////////////////////////////////////////////////////////////////////////////

//!接口返回值宏定义
typedef enum e_cloudsdk_result_code
{
	CloudSdk_ResultCode_Succ                    				= 0,                    //!成功
	CloudSdk_ResultCode_Failed                  				= 1,                    //!失败
	CloudSdk_ResultCode_VersionMisMatch        			= 2,                    //!版本不匹配
	CloudSdk_ResultCode_NoInit                  				= 3,                    //!sdk未初始化
	CloudSdk_ResultCode_NoStart                	 			= 4,                    //!未启动
	CloudSdk_ResultCode_HasInit                				= 5,                    //!已经初始化
	CloudSdk_ResultCode_HasUninit               			= 6,                    //!sdk已反初始化
	CloudSdk_ResultCode_HasStarted              			= 7,                    //!已经启动
	CloudSdk_ResultCode_Unsupported            			= 8,                    //!不支持的操作

	CloudSdk_ResultCode_InvalidParam            			= 9,                    //!参数无效
	CloudSdk_ResultCode_InvalidHandle           			= 10,                  //!句柄值无效
	CloudSdk_ResultCode_InValidUser             			= 11,                  //!无效用户

	CloudSdk_ResultCode_NoMem                   			= 12,                   //!没有内存
	CloudSdk_ResultCode_NoHandle                			= 13,                   //!没有句柄
	CloudSdk_ResultCode_BufTooSmall             			= 14,                   //!缓冲区太小

	CloudSdk_ResultCode_ConnectFailed           			= 15,                   //!连接失败
	CloudSdk_ResultCode_SendFailed              			= 16,                   //!发送失败
	CloudSdk_ResultCode_RecvFailed              			= 17,                   //!接收失败

	CloudSdk_ResultCode_ConnectTimeout         			= 18,                   //!连接超时
	CloudSdk_ResultCode_RecvTimeout             			= 19,                   //!接收超时

	CloudSdk_ResultCode_AuthenticationError			= 20,                   //!用户认证错误
	CloudSdk_ResultCode_NoRight                 			= 21,                   //!当前操作无权限

	CloudSdk_ResultCode_CallP2PNetAPIError = 22,   //!调用P2P透传库API接口失败
	CloudSdk_ResultCode_DevException = 23,		   //!设备异常
	CloudSdk_ResultCode_SerializeDataError = 24,   //!序列化错误
	CloudSdk_ResultCode_DeserializeDataError = 25, //!反序列化错误
	CloudSdk_ResultCode_OverMaxNum = 26,		   //!超过总数
	CloudSdk_ResultCode_GetReponseTimeout = 27,
	CloudSdk_ResultCode_InvalidFunction = 28,
	CloudSdk_ResultCode_CallDevAPIError = 29,
	CloudSdk_ResultCode_HasCall = 30,
	CloudSdk_ResultCode_HandleExist = 31,
	CloudSdk_ResultCode_OptBusy = 32,
	CloudSdk_ResultCode_CallI8HTranCtrlError = 33,
	CloudSdk_ResultCode_CallI8HTranCtrlResultError = 34,
	CloudSdk_ResultCode_CallDirectNetError = 35, //!直连API错误
	CloudSdk_ResultCode_FaceBaseFull = 36,		 //!人脸数据库满
	CloudSdk_ResultCode_SerialNoMatchError = 37,
	CloudSdk_ResultCode_UpgradeCheckMD5Error = 38,
	CloudSdk_ResultCode_Upgrading = 39,
    
    CloudSdk_ResultCode_DirectLinkUidChangeError = 1000,
} cloudsdk_result_code_t;

typedef enum
{
	CloudSdk_User_Login_Right 		= 0x00,
	CloudSdk_User_RealPlay_Right,      		 //!用户实时流预览权限
	CloudSdk_User_RePlay_Right,     			//!用户回放权限
	CloudSdk_User_Talk_Right,          		 	//!用户语音对讲权限
	CloudSdk_User_PTZ_Right,
} cloudsdk_user_right_t ;


//!PTZ控制命令宏定义
typedef enum e_cloudsdk_ptz_ctrl
{
	CLOUDSDK_PTZ_CTRL_STOP = 100,
	CLOUDSDK_PTZ_CTRL_MOVE_UP,
	CLOUDSDK_PTZ_CTRL_MOVE_DOWN,
	CLOUDSDK_PTZ_CTRL_MOVE_LEFT,
	CLOUDSDK_PTZ_CTRL_MOVE_RIGHT,
	CLOUDSDK_PTZ_CTRL_MOVE_UPLEFT,
	CLOUDSDK_PTZ_CTRL_MOVE_DOWNLEFT,
	CLOUDSDK_PTZ_CTRL_MOVE_UPRIGHT,
	CLOUDSDK_PTZ_CTRL_MOVE_DOWNRIGHT,

	CLOUDSDK_PTZ_CTRL_IRIS_IN,
	CLOUDSDK_PTZ_CTRL_IRIS_OUT,
	CLOUDSDK_PTZ_CTRL_FOCUS_IN,
	CLOUDSDK_PTZ_CTRL_FOCUS_OUT,
	CLOUDSDK_PTZ_CTRL_ZOOM_IN,
	CLOUDSDK_PTZ_CTRL_ZOOM_OUT,

	CLOUDSDK_PTZ_CTRL_SET_PRESET,
	CLOUDSDK_PTZ_CTRL_CALL_PRESET,
	CLOUDSDK_PTZ_CTRL_DELETE_PRESET,

	CLOUDSDK_PTZ_CTRL_BEGIN_CRUISE_SET,
	CLOUDSDK_PTZ_CTRL_SET_CRUISE,
	CLOUDSDK_PTZ_CTRL_END_CRUISE_SET,
	CLOUDSDK_PTZ_CTRL_CALL_CRUISE,
	CLOUDSDK_PTZ_CTRL_DELETE_CRUISE,
	CLOUDSDK_PTZ_CTRL_STOP_CRUISE,

	CLOUDSDK_PTZ_CTRL_AUTO_SCAN,

	CLOUDSDK_PTZ_CTRL_RAINBRUSH_START,
	CLOUDSDK_PTZ_CTRL_RAINBRUSH_STOP,
	CLOUDSDK_PTZ_CTRL_LIGHT_ON,
	CLOUDSDK_PTZ_CTRL_LIGHT_OFF,

	CLOUDSDK_PTZ_CTRL_ADDTRACE,
	CLOUDSDK_PTZ_CTRL_DELTRACE,
	CLOUDSDK_PTZ_CTRL_CALLTRACE,

	CLOUDSDK_PTZ_CTRL_SET3D,

	CLOUDSDK_PTZ_CTRL_MAX,
} cloudsdk_ptz_ctrl_t ;


typedef enum e_cloudsdk_thread_flag
{
	CloudSdk_Disable4Thread     = 0,
	CloudSdk_Enable4Thread      = 1,
} cloudsdk_thread_flag_t ;


typedef enum e_cloudsdk_session_status
{
	CloudSdk_SessionStatus4Dead = 0,
	CloudSdk_SessionStatus4Live = 1,
} cloudsdk_session_status_t ;


typedef enum e_cloudsdk_referenced_status
{
	CloudSdk_Referenced4Idle = 0,
	CloudSdk_Referenced4Busy = 1,
} cloudsdk_referenced_status_t ;


//!码流类型宏定义
typedef enum e_cloudsdk_stream_type
{
	CloudSdk_MainStream 	= 1,
	CloudSdk_SubStream  	= 2,
	CloudSdk_MixStream 	= 3,
} cloudsdk_stream_type_t ;


//!传输数据类型宏定义
typedef enum e_cloudsdk_data_tran_type
{
	CloudSdk_TranRealVideoData = 0x01,
	CloudSdk_TranRealAudioData = 0x02,
	CloudSdk_TranTalkData = 0x03,
	CloudSdk_TranHistroyVideoData = 0x04,
	CloudSdk_TranHistroyAudioData = 0x05,
	CloudSdk_TranAlarmData = 0x06,
} cloudsdk_data_tran_type_t;

//!视频编码格式宏定义
typedef enum e_cloudsdk_video_format
{
	CloudSdk_Video_I8       		= 0x01,
	CloudSdk_Video_H264     	= 0x02,
	CloudSdk_Video_H265     	= 0x03,
	CloudSdk_Video_AVI      		= 0x04,
} cloudsdk_video_format_t ;


//!帧类型宏定义
typedef enum e_cloudsdk_frame_type
{
	CloudSdk_Main_IFrame            	= 0x01,     //!主码流I帧
	CloudSdk_Main_PFrame            	= 0x02,     //!主码流P帧
	CloudSdk_Sub_IFrame             	= 0x03,     //!子码流I帧
	CloudSdk_Sub_PFrame             	= 0x04,     //!子码流P帧
	CloudSdk_Main_AudioFrame        	= 0x05,     //!主码流音频帧
	CloudSdk_Sub_AudioFrame         	= 0x06,     //!子码流音频帧
	CloudSdk_Main_Smart_IFrame      	= 0x07,     //!主码流智能I帧
	CloudSdk_Main_Smart_PFrame      = 0x08,     //!主码流智能P帧
	CloudSdk_Main_Smart_LPRFrame	= 0x09,     //!主码流智能车牌识别帧
	CloudSdk_Sub_Smart_IFrame       	= 0x0a,     //!子码流智能I帧
	CloudSdk_Sub_Smart_PFrame       	= 0x0b,     //!子码流智能P帧
	CloudSdk_Sub_Smart_LPRFrame	= 0x0c,     //!子码流智能车牌识别帧
	CloudSdk_Zero_IFrame            	= 0x0d,     //!零通道视频I帧
	CloudSdk_Zero_PFrame            	= 0x0e,     //!零通道视频P帧
	CloudSdk_ZkTrans_Frame 			= 0x0f,		//!中控透传帧数据
	CloudSdk_Main_VIFrame				= 0x11,  
	CloudSdk_Sub_VIFrame				= 0x13,
	CloudSdk_Zero_VIFrame			= 0x1d,
} cloudsdk_frame_type_t ;


//!音频编码格式宏定义
typedef enum e_cloudsdk_audio_format
{
	CloudSdk_Audio_G711_U       	= 0x01,
	CloudSdk_Audio_G711_A       	= 0x02,
	CloudSdk_Audio_G726         	= 0x03,
} cloudsdk_audio_format_t ;


//!音频采样率宏定义
typedef enum e_cloudsdk_audio_samplerate
{
	CloudSdk_Audio_SampleRate_8000HZ    	= 0x01,
	CloudSdk_Audio_SampleRate_32000HZ   	= 0x02,
	CloudSdk_Audio_SampleRate_48000HZ   	= 0x03,
	CloudSdk_Audio_SampleRate_96000HZ   	= 0x04,
} cloudsdk_audio_samplerate_t ;


//!音频位率宏定义
typedef enum e_cloudsdk_audio_bitwidth
{
	CloudSdk_Audio_BitWidth_8   	= 0x01,
	CloudSdk_Audio_BitWidth_16  	= 0x02,
} cloudsdk_audio_bitwidth_t ;


//!音频声道宏定义
typedef enum e_cloudsdk_audio_track
{
	CloudSdk_Audio_mono     = 0x01,
	CloudSdk_Audio_Stereo   = 0x02,
} cloudsdk_audio_track_t ;


//!消息数据包版本宏定义
typedef enum e_cloudsdk_version
{
	CloudSdk_VersionV10 = 0x00010000,
} cloudsdk_version_t ;


//!消息数据包类型
typedef enum e_cloudsdk_message_type
{
	CloudSdk_Message4Request    	= 0x0000,
	CloudSdk_Message4Response   	= 0x0001,
	CloudSdk_Message4Notify     		= 0x0002,
} cloudsdk_message_type_t ;


//!查找录像文件类型宏定义
typedef enum e_cloudsdk_findfile_type
{
	CLOUDSDK_FINDFILE_TYPE_ALL                  			= 0xFF,
	CLOUDSDK_FINDFILE_TYPE_TIME                 			= 0,
	CLOUDSDK_FINDFILE_TYPE_MOTION               		= 1,
	CLOUDSDK_FINDFILE_TYPE_ALARM               	 		= 2,
	CLOUDSDK_FINDFILE_TYPE_MOTION_OR_ALARM      	= 3,
	CLOUDSDK_FINDFILE_TYPE_MOTION_AND_ALARM     	= 4,
	CLOUDSDK_FINDFILE_TYPE_COMMAND              		= 5,
	CLOUDSDK_FINDFILE_TYPE_MANUAL               		= 6,
} cloudsdk_findfile_type_t ;


typedef enum e_cloudsdk_findfile_type_v2
{
	CLOUDSDK_FINDFILE_TYPE_V2_NONE 				= 0,

	CLOUDSDK_FINDFILE_TYPE_V2_REGULAR       		= 0x01,     //!常规录像
	CLOUDSDK_FINDFILE_TYPE_V2_MD            			= 0x02,     //!移动侦测录像
	CLOUDSDK_FINDFILE_TYPE_V2_AI            			= 0x04,     //!报警录像
	CLOUDSDK_FINDFILE_TYPE_V2_COUNTER    			= 0x08,
	CLOUDSDK_FINDFILE_TYPE_V2_WIRE       			= 0x10,
	CLOUDSDK_FINDFILE_TYPE_V2_REGION     			= 0x20,
	CLOUDSDK_FINDFILE_TYPE_V2_OBJECT     			= 0x40,
	CLOUDSDK_FINDFILE_TYPE_V2_SCENCE        		= 0x80,     //!场景突变录像
	CLOUDSDK_FINDFILE_TYPE_V2_SOUND     			= 0x100,   //!声音告警录像
	CLOUDSDK_FINDFILE_TYPE_V2_FIRE      			= 0x200,   //!火灾检测录像
	CLOUDSDK_FINDFILE_TYPE_V2_FACE      			= 0x400,   //!人脸检测录像
	CLOUDSDK_FINDFILE_TYPE_V2_PLATE     			= 0x800,   //!车牌检测录像
	/*****如果新增加录像类型，记得同步增加到I8H_REC_TRIG_BY_ALL中去哦********/

	CLOUDSDK_FINDFILE_TYPE_V2_PRE 				= CLOUDSDK_FINDFILE_TYPE_V2_NONE, //!预录
	//!对于类型的全部，最好不要用0xFFFFFFFF来处理了，增加一种类型，就修改一下全部的定义，这样用户查询的时候，选择全部 + 同时满足 的时候才能查得到
	CLOUDSDK_FINDFILE_TYPE_V2_ALL = CLOUDSDK_FINDFILE_TYPE_V2_REGULAR | CLOUDSDK_FINDFILE_TYPE_V2_MD | CLOUDSDK_FINDFILE_TYPE_V2_AI | CLOUDSDK_FINDFILE_TYPE_V2_COUNTER | CLOUDSDK_FINDFILE_TYPE_V2_WIRE | CLOUDSDK_FINDFILE_TYPE_V2_REGION | CLOUDSDK_FINDFILE_TYPE_V2_OBJECT | CLOUDSDK_FINDFILE_TYPE_V2_SCENCE | CLOUDSDK_FINDFILE_TYPE_V2_SOUND | CLOUDSDK_FINDFILE_TYPE_V2_FIRE | CLOUDSDK_FINDFILE_TYPE_V2_FACE | CLOUDSDK_FINDFILE_TYPE_V2_PLATE,
} cloudsdk_findfile_type_v2_t;

//!报警类型宏定义
typedef enum e_cloudsdk_alarm_type
{
	CLOUDSDK_ALARM_VIDEOMOTION_TYPE         	= 0x0001,       	//!移动侦测报警类型
	CLOUDSDK_ALARM_ALARMIN_TYPE             		= 0x0002,      	 //!输入报警类型
	CLOUDSDK_ALARM_VIDEOLOST_TYPE           		= 0x0003,      	 //!视频丢失报警类型
	CLOUDSDK_ALARM_VIDEOMASK_TYPE           		= 0x0004,       	//!视频遮挡报警类型
	CLOUDSDK_ALARM_DISKFULL_TYPE            		= 0x0005,       	//!硬盘满
	CLOUDSDK_ALARM_DISKERROR_TYPE           		= 0x0006,      	 //!硬盘错误
	CLOUDSDK_ALARM_ILLEGEACCESS_TYPE        	= 0x0007,      	 //!非法访问
	CLOUDSDK_ALARM_NETDISCONNECT_TYPE       	= 0x0008,       	//!网线断开
	CLOUDSDK_ALARM_IPCONFLICT_TYPE          		= 0x0009,       	//!IP冲突
	CLOUDSDK_ALARM_BODYINDUCTION            		= 0x000a,		//!人体感应报警
	CLOUDSDK_ALARM_MEDIADISCONNECT          	= 0x000b,   	//!断流报警

	CLOUDSDK_ALARM_IVS_COUNTWIRE = 0x0014,			 //!计数报警
	CLOUDSDK_ALARM_IVS_DETECTWIRE = 0x0015,			 //!过线报警
	CLOUDSDK_ALARM_IVS_DETECTREGION = 0x0016,		 //!区域检测
	CLOUDSDK_ALARM_IVS_OBJECTREGION = 0x0017,		 //!对象区域
	CLOUDSDK_ALARM_IVS_SOUNDALARM = 0x0018,			 //!声音报警
	CLOUDSDK_ALARM_IVS_PLATEDETECTALARM = 0x0019,	 //!车牌识别
	CLOUDSDK_ALARM_IVS_FACEDETECTALARM = 0x001a,	 //!人脸检测报警
	CLOUDSDK_ALARM_IVS_FIREDETECTALARM = 0x001b,	 //!火灾检测报警
	CLOUDSDK_ALARM_IVS_VIDEODIAGONSECOLOR = 0x001c,	 //!视频诊断颜色
	CLOUDSDK_ALARM_IVS_VIDEODIAGONSEBRIGHT = 0x001d, //!视频诊断亮度
	CLOUDSDK_ALARM_IVS_VIDEODIAGONSEBLUR = 0x001e,	 //!视频诊断
	CLOUDSDK_ALARM_IVS_VIDEODIAGONSEDARK = 0x001f,	 //!视频诊断
	CLOUDSDK_ALARM_IVS_VIDEODIAGONSEVILOST = 0x0020, //!视频诊断
	CLOUDSDK_ALARM_IVS_MOTIONDETECT = 0x0021,		 //!智能移动侦测
	CLOUDSDK_ALARM_IVS_TEMPHIGH = 0x0022,			 //!温度高
	CLOUDSDK_ALARM_IVS_TEMPLOW = 0x0023,			 //!温度低
	CLOUDSDK_ALARM_IVS_RETROGRADE = 0x0024,			 //!逆行检测
	CLOUDSDK_ALARM_IVS_DETECTABSENT = 0x0025,		 //!离岗检测
	CLOUDSDK_ALARM_IVS_ENDTYPE,

} cloudsdk_alarm_type;

//!消息命令宏定义
typedef enum e_cloudsdk_message_cmd
{
	CloudSdk_Message_Login = 0xA000,        	//!用户注册命令(c->s)
	CloudSdk_Message_Logout,                		//!用户注销命令(c->s)

	CloudSdk_Message_RealPlay,              		//!实时流请求命令(c->s)
	CloudSdk_Message_StopRealPlay,          	//!停止实时流请求命令(c->s)
	CloudSdk_Message_ControlRealPlay,     	//!控制实时流控制请求命令(如:添加新通道实时流/删除已通道实时流)(c->s)

	CloudSdk_Message_ZeroRealPlay,          	//!零通道实时流请求命令(c->s)
	CloudSdk_Message_StopZeroRealPlay,     	//!停止零通道实时流请求命令(c->s)

	CloudSdk_Message_Replay,                		//!历史流请求命令(c->s)
	CloudSdk_Message_StopReplay,            		//!停止历史流请求命令(c->s)
	CloudSdk_Message_ControlReplay,         	//!控制历史流请求命令(如:Seek/Fast/Slow/SetSpeed等操作)(c->s)

	CloudSdk_Message_AddAlarm,              		//!报警推送请求命令(c->s)
	CloudSdk_Message_DelAlarm,              		//!停止报警推送请求命令(c->s)
	CloudSdk_Message_AlarmInfo,             		//!推送报警信息通知(s->c)

	CloudSdk_Message_ControlPTZ,            		//!PTZ请求命令(c->s)

	CloudSdk_Message_FindFile,              		//!历史文件查询请求(c->s)

	CloudSdk_Message_Talk,                  		//!语音对讲请求(c->s)
	CloudSdk_Message_StopTalk,              		//!停止语音对讲请求(c->s)

	CloudSdk_Message_GetConfig,             		//!获取配置参数请求(c->s)
	CloudSdk_Message_SetConfig,             		//!设置配置参数请求(c->s)

	CloudSdk_Message_TranControl,           		//!透传信息请求命令(仅限于65KB之内透传数据传输)
	CloudSdk_Message_StopRealPlayv2,
	CloudSdk_Message_StopReplayv2,

	CloudSdk_Message_SearchDev,
	CloudSdk_Message_TranControlv2, //!透传信息请求命令(支持超过65KB透传数据传输)
	CloudSdk_Message_TranControlv3, //!透传到底层model层的的数据
	CloudSdk_Message_TranControlv4, //!透传到NVR/IPC上autop2p层的数据

	CloudSdk_Message_ModifyDevIP,	//!修改设备IP地址
	CloudSdk_Message_ModifyDevPort, //!修改设备P2P直连端口

	CloudSdk_Message_SubscribeAlarm,//!报警订阅命令(c->s)
} cloudsdk_message_cmd_t;


//!配置参数获取与设置宏定义
typedef enum e_cloudsdk_config_cmd
{
	CloudSdk_Config_GetDevInfo = 0x0000,        //!获取设备基本信息
} cloudsdk_config_cmd_t ;


//!实时流播放控制命令
typedef enum e_cloudsdk_control_realplay_cmd
{
	CloudSdk_ControlRealplay_PlayAudio              	= 0x01, //!打开声音 使用CLOUD_CONTROL_REALPLAY_COND结构中 uParam.dwParams[0]描述主子码流类型 1-主码流 2-子码流 uParam.dwParams[1]描述通道号
	CloudSdk_ControlRealPlay_StopAudio              	= 0x02, //!关闭声音 使用CLOUD_CONTROL_REALPLAY_COND结构中 uParam.dwParams[0]描述主子码流类型 1-主码流 2-子码流 uParam.dwParams[1]描述通道号
	CloudSdk_ControlRealPlay_SwitchStreamType      = 0x03, //!切换主子码流类型 使用CLOUD_CONTROL_REALPLAY_COND结构中 uParam.dwParams[0]描述 1-主码流 2-子码流 uParam.dwParams[1]描述通道号 uParam.dwParams[2]描述切换目标码流类型 1-主码流 2-子码流
	CloudSdk_ControlRealPlay_AddChan                	= 0x04, //!添加通道 使用CLOUD_CONTROL_REALPLAY_COND结构中 uParam.dwParams[0]描述码流类型 1-主码流 2-子码流 uParam.dwParams[1]描述通道号
	CloudSdk_ControlRealPlay_DelChan                	= 0x05, //!删除通道 使用CLOUD_CONTROL_REALPLAY_COND结构中 uParam.dwParams[0]描述码流类型 1-主码流 2-子码流 uParam.dwParams[1]描述通道号
	CloudSdk_ControlRealPlay_AddZeroChan            	= 0x06, //!添加通道 使用CLOUD_CONTROL_REALPLAY_COND结构中 uParam.byChanMaps填写窗口所对应的通道号
	CloudSdk_ControlRealPlay_DelZeroChan            	= 0x07, //!删除通道 使用CLOUD_CONTROL_REALPLAY_COND结构中 uParam.byChanMaps填写窗口所对应的通道号
	CloudSdk_ControlRealPlay_SwitchZeroChan         	= 0x08, //!切换通道(即是基于已有零通道进行后一组合成画面推送相应码流) 使用CLOUD_CONTROL_REALPLAY_COND结构中 uParam.byChanMaps填写窗口所对应的通道号
	CloudSdk_ControlRealPlay_AddChanv2				= 0x09,	//!添加通道 使用CLOUD_CONTROL_REALPLAY_COND结构中 uParam.dwParams[0]描述码流类型 1-主码流 2-子码流 uParam.dwParams[1]描述通道号 uParam.dwParams[2] 1:描述开启视频流 2:开启报警流 3:同时开启
	CloudSdk_ControlRealPlay_DelChanv2				= 0x0a,	//!删除通道 使用CLOUD_CONTROL_REALPLAY_COND结构中 uParam.dwParams[0]描述码流类型 1-主码流 2-子码流 uParam.dwParams[1]描述通道号 uParam.dwParams[2] 1:描述关闭视频流 2:关闭报警流 4:同时关闭
} cloudsdk_control_realplay_cmd_t ;


//!历史流回放/下载播放控制命令
typedef enum e_cloudsdk_control_replay_cmd
{
    CloudSdk_ControlReplay_noCmd = 0x00,
	CloudSdk_ControlReplay_Play             			= 0x1,  //!开启播放 使用CLOUDSDK_CONTROL_REALPLAY_COND结构中 dwInParams[0]描述码流类型 1-主码流 2-子码流 dwInParams[1]描述通道号
	CloudSdk_ControlReplay_Pause            			= 0x2,  //!暂停播放 使用CLOUDSDK_CONTROL_REALPLAY_COND结构中 dwInParams[0]描述码流类型 1-主码流 2-子码流 dwInParams[1]描述通道号 dwInParams[2]描述暂停命令 0-恢复 1-暂停
	CloudSdk_ControlReplay_Repeat           			= 0x3,  //!重新播放 使用CLOUDSDK_CONTROL_REALPLAY_COND结构中 dwInParams[0]描述码流类型 1-主码流 2-子码流 dwInParams[1]描述通道号
	CloudSdk_ControlReplay_PlayAudio        		= 0x4,  //!打开声音 使用CLOUDSDK_CONTROL_REALPLAY_COND结构中 dwInParams[0]描述码流类型 1-主码流 2-子码流 dwInParams[1]描述通道号
	CloudSdk_ControlReplay_StopAudio        		= 0x5,  //!关闭声音 使用CLOUDSDK_CONTROL_REALPLAY_COND结构中 dwInParams[0]描述码流类型 1-主码流 2-子码流 dwInParams[1]描述通道号
	CloudSdk_ControlReplay_SetSpeed         		= 0x6,  //!设置播放速度 使用CLOUDSDK_CONTROL_REALPLAY_COND结构中 dwInParams[0]描述码流类型 1-主码流 2-子码流 dwInParams[1]描述通道号 dwInParams[2]描述速度值 -16~16
	CloudSdk_ControlReplay_SetTime          			= 0x7,  //!设置回放时间 使用CLOUDSDK_CONTROL_REALPLAY_COND结构中 dwInParams[0]描述码流类型 1-主码流 2-子码流 dwInParams[1]描述通道号 dwInParams[2]描述时间
	CloudSdk_ControlReplay_SwitchStreamType 	= 0x8,  //!切换主子码流类型 使用CLOUDSDK_CONTROL_REALPLAY_COND结构中 dwInParams[0]描述码流类型 1-主码流 2-子码流 dwInParams[1]描述通道号 dwInParams[2]描述切换目标码流类型
	CloudSdk_ControlReplay_PlayDirect       		= 0x9,  //!控制回放正放/倒放 使用CLOUDSDK_CONTROL_REPLAY_COND结构中 dwInParams[0]描述码流类型 1-主码流 2-子码流 dwInParams[1]描述通道号 dwInParams[2]描述是否正放倒放 0-正放 1-倒放
	CloudSdk_ControlReplay_AddChan          		= 0xa,  //!加入同步通道 使用CLOUDSDK_CONTROL_REPLAY_COND结构中 dwInParams[0]描述码流类型 1-主码流 2-子码流 dwInParams[1]描述通道号
	CloudSdk_ControlReplay_DelChan          			= 0xb,  //!移除同步通道 使用CLOUDSDK_CONTROL_REPLAY_COND结构中 dwInParams[0]描述码流类型 1-主码流 2-子码流 dwInParams[1]描述通道号
} cloudsdk_control_replay_cmd_t ;


//!与设备连接模式
typedef enum e_cloudsdk_link_model
{
	CloudSdk_Link_Unknown   	= 0x00,     //!未知模式
	CloudSdk_Link_Direct        	= 0x01,     //!P2P模式下TCP普通直连模式
	CloudSdk_Link_UDP2Peer  	= 0x02,     //!UDP穿透模式
	CloudSdk_Link_Agent     		= 0x03,     //!转发模式
	CloudSdk_Link_TCPDirect 	= 0x04,     //!局域网IP直连
} cloudsdk_link_model_t ;


typedef enum e_cloudsdk_nat_type
{
	CloudSdk_NatType_Unknown                		= 0x00,     //!未知NAT类型
	CloudSdk_NatType_RestrictedCone         	= 0x01,     //!受限锥形
	CloudSdk_NatType_PortRestrictedCone     	= 0x02,     //!端口受限锥形
	CloudSdk_NatType_Symm                   		= 0x03,     //!对称型
} cloudsdk_nat_type_t ;


////////////////////////////////////////////////////////////////////////////////
// 结构体定义
////////////////////////////////////////////////////////////////////////////////
//!CloudSdk 设备基本信息数据结构
typedef struct
{
	BYTE byDevName[32] ;            	//!设备名称
	BYTE bySerialNo[48] ;          	//!设备序列号
	WORD wChanNum ;                 	//!设备视频通道总数
	WORD wStartChan ;               	//!设备视频起始通道号
	WORD wAlarmInNum ;              //!设备报警输入总数
	WORD wAlarmOutNum ;           //!设备报警输出总数
	BYTE byType ;                   		//!设备类型
	BYTE byDiskNum ;                	//!硬盘总数
	BYTE byAudioChanNum ;         //!音频通道总数
	BYTE byZeroChanNum ;           //!零通道总数
	BYTE byZeroStartChan ;          //!零通道起始通道号
	BYTE bySubStreamRec ;          //!是否支持子码流录像功能
	BYTE byLinkModel ;              	//!与设备连接模式(直连/UDP穿透/转发模式,参考cloudsdk_link_model_t宏定义)
	BYTE byNatType ;                	//!NAT类型(参考cloudsdk_nat_type_t宏定义)
	DWORD dwAbility ;               	//!能力集合(Bit0用于描述是否具有零通道功能,Bit1用于描述是否具有人脸检测功能,其他暂时保留)
	DWORD dwFaceChannels[4];
	DWORD dwFirmwareVersion; //!(主版本<<24|子版本号<<16|保留位置0)
	DWORD dwBuildDate;		 //!(年<<16|月<<8|日)
	BYTE byUpgrade;			 //!有新版本
	BYTE byRes1[23];		 //!保留位,暂时不用
} CLOUDSDK_DEVICE_INFO, *LPCLOUDSDK_DEVICE_INFO;

//!CloudSdk 校时数据结构
typedef struct
{
	DWORD dwYear;       	//!年
	DWORD dwMonth;      	//!月
	DWORD dwDay;        	//!日
	DWORD dwHour;       	//!时
	DWORD dwMinute;     	//!分
	DWORD dwSecond;     	//!秒
	DWORD dwZoneTime ;  	//!时区
} CLOUDSDK_TIME, *LPCLOUDSDK_TIME;


//!公共响应结构
typedef struct
{
	WORD wResult ;
	WORD wRes ;
} CLOUDSDK_RESULT,*LPCLOUDSDK_RESULT;


//!CloudSdk 实时流开关请求结构,对应响应结构为CLOUDSDK_RESULT
//!零通道结构

typedef struct CLOUDSDK_NORMAL_PARAM
{
	BYTE bySmartModel ;			//!智能模式 0/1-仅音视频流 2-仅智能帧流 3-同时支持智能帧和音视频
	BYTE byRes[31] ;
}CLOUDSDK_NORMAL_PARAM ,*LPCLOUDSDK_NORMAL_PARAM ;


typedef struct
{
	WORD wCount ;				 //!画面总数,最大仅支持16画面合屏
	BYTE bySmartModel ;			//!智能模式 1-仅音视频流 2-仅智能帧流 3-同时音视频和智能帧流
	BYTE byRes[13] ;			 //!保留位
	BYTE byChanMaps[16] ;   	//!因要保持数据结构兼容性,故采用BYTE数据类型 0-255分别用表示第1通道至第256通道,也就是实际通道减
} CLOUDSDK_ZERO_CHANNEL ;


typedef struct
{
	WORD wChannel;	  //!实时流请求通道(从1开始,如果填写0xFFFF则示为请求零通道,则需要填写uParam.struZeroChannel结构,否则可不用填写联合体相关结构信息)
	WORD wStreamType; //!实时流请求码流类型(通道号填写1~0xFFFE,则需要填写码流类型,通道号填写0xFFFF则不必填写)
	union
	{
		CLOUDSDK_ZERO_CHANNEL struZeroChannel;
		CLOUDSDK_NORMAL_PARAM struNormalParam;
	} uParam;
} CLOUDSDK_REALPLAY_COND, *LPCLOUDSDK_REALPLAY_COND;


//!CloudSdk 实时流控制结构,对应响应结构为CLOUDSDK_RESULT
typedef struct
{
	DWORD dwRealPlayCmd; //!实时流控制命令(若零通道添加/删除/切换,则高16位用于描述零通道画面总数,否则高16位均置0)
	union
	{
		DWORD dwParams[4];	 //!实时流控制参数,Param[0]表示码流类型，Param[1]表示通道号 Param[2]表示通道开启方式（仅音视频，仅智能，同时）
		BYTE byChanMaps[16]; //!零通道画面通道映射
	} uParam;
} CLOUDSDK_CONTROL_REALPLAY_COND, *LPCLOUDSDK_CONTROL_REALPLAY_COND;


//!CloudSdk PTZ控制参数结构,对应响应结构为CLOUD_RESULT
typedef struct
{
	WORD wChannel ;                         	//!通道号(从1开始)
	WORD wPTZCmd ;                          	//!PTZ控制ID
	DWORD dwInParams[4] ;                  //!PTZ控制输入参数
} CLOUDSDK_CONTROL_PTZ_COND, *LPCLOUDSDK_CONTROL_PTZ_COND ;


//!CloudSdk 对讲控制参数结构,对应响应结构为CLOUD_RESULT
typedef struct
{
	WORD wChannel ;				//!语音对讲通道号
	BYTE byEncodeType ;                        //!语音对讲编码类型 0-G711-U 1-G711A
	BYTE byTrack ;                              	//!语音对讲单双声道 0/1-mono 2-stero
	BYTE bySampleBitWidth ;            	//!语音对讲采样位率 8bit 16bit
	BYTE byRes1[3] ;                            	//!保留位
	DWORD dwSampleRate ;                  	//!语音对讲采样率 8000kbs
	BYTE byRes2[32] ;                           	//!保留位
} CLOUDSDK_TALK_COND, *LPCLOUDSDK_TALK_COND ;


//!CloudSdk 历史文件查询结构,对应响应结构为CLOUDSDK_FIND_FILE_DATA
typedef struct
{
	WORD wChannel ;                 		//!查询通道号
	BYTE byFileType ;               		//!录像文件类型0xff-全部，0-定时录像,1-移动侦测,2-报警触发,3-报警|移动侦测 4-报警&移动侦测 5-命令触发 6-手动录像
	BYTE byRes ;                        		//!保留位
	CLOUDSDK_TIME struBeginTime ;   //!开始时间
	CLOUDSDK_TIME struEndTime ;     	//!结束时间
	DWORD dwFileType ;              		//!录像文件类型
	BYTE byRes1[12] ;               		//!保留位
} CLOUDSDK_FIND_FILE_COND, *LPCLOUDSDK_FIND_FILE_COND ;


//!文件查询响应结构
typedef struct
{
	WORD wChannel ;                         		//!视频通道号(从1开始)
	BYTE byFileType;                        		//!文件类型
	BYTE byRes ;                            			//!保留位
	CLOUDSDK_TIME struStartTime;            	//!文件的开始时间
	CLOUDSDK_TIME struStopTime;             	//!文件的结束时间
	DWORD dwFileSize;                       		//!文件的大小
	DWORD dwFileType ;
	BYTE byRes1[12];                        		//!保留位
} CLOUDSDK_FIND_FILE_DATA, *LPCLOUDSDK_FIND_FILE_DATA ;


typedef struct
{
	WORD wChannel ;
	WORD wStreamType ;
	BYTE byRes1[4] ;                //!保留位,暂时不用
} CLOUDSDK_REPLAY_PARAM, *LPCLOUDSDK_REPLAY_PARAM ;


//!CloudSdk 开启回放请求结构,对应响应CLOUDSDK_REPLAY_RESULT结构
typedef struct
{
	BYTE byMode ;                               					//!回放模式,(0:正常/1:下载)
	BYTE byRes ;                                					//!保留位
	WORD wCount ;                               					//!回放数组有效个数
	CLOUDSDK_TIME struStartTime ;             				//!历史流开始时间
	CLOUDSDK_TIME struStopTime ;             				//!历史流结束时间
	CLOUDSDK_REPLAY_PARAM struReplayParam[32] ; 	//!参数数组结构
	BYTE byRes1[32] ;                           					//!保留位,暂时不用
} CLOUDSDK_REPLAY_COND, *LPCLOUDSDK_REPLAY_COND ;


//!CloudSdk 开启回放响应
typedef struct
{
	WORD wResult ;
	WORD wRes ;
	DWORD dwReplayId ;              //!回放ID
} CLOUDSDK_REPLAY_RESULT, *LPCLOUDSDK_REPLAY_RESULT;


//!CloudSdk 历史流控制结构,对应响应结构为CLOUD_RESULT
typedef struct
{
	DWORD dwReplayId ;             	 	//!回放ID
	DWORD dwReplayCmd ;             	//!历史流控制命令
	DWORD dwParams[4] ;             		//!历史流控制参数
	BYTE byRes[16] ;                		//!保留位
} CLOUDSDK_CONTROL_REPLAY_COND, *LPCLOUDSDK_CONTROL_REPLAY_COND ;


//!CloudSdk 历史流关闭结构,对应响应结构为CLOUD_RESULT
typedef struct
{
	DWORD dwReplayId ;              	//!回放ID
	BYTE byRes[16] ;                	//!保留位
} CLOUDSDK_STOP_REPLAY_COND, *LPCLOUDSDK_STOP_REPLAY_COND ;


//!CloudSdk报警推送条件
typedef struct
{
	DWORD dwAlarmType ;
	WORD wChannel ;
	WORD wRes ;
	CLOUDSDK_TIME struBeginTime ;
	CLOUDSDK_TIME struEndTime ;
	BYTE byRes[32] ;
} CLOUDSDK_ALARM_COND, *LPCLOUDSDK_ALARM_COND ;

typedef struct
{
	U32 u32AlarmRules[8];		//每一位对应着一种报警订阅与否,第n位对应着值为n的报警类型
	BYTE byRes[224];
}CLOUDSDK_ALARM_COND_V2, *LPCLOUDSDK_ALARM_COND_V2;

//!CloudSdk 报警信息结构
typedef struct
{
	DWORD dwAlarmType ;                         	//!报警类型
	WORD wParam ;                               		//!报警关联参数
	WORD wRes ;                                 		//!保留位
	DWORD dwTimeStamp ;                         	//!报警时间戳
	DWORD dwBufferSize ;
	BYTE *lpDataBuffer ;
} CLOUDSDK_ALARM_INFO, *LPCLOUDSDK_ALARM_INFO ;


//!透传控制请求结构
typedef struct
{
	DWORD dwDataSize ;                  //!透传请求内容大小
	BYTE byRes[3];
	BYTE byBinaryData[1] ;              //!透传请求内容第一个字节地址
} CLOUDSDK_TRAN_CONTROL_COND, *LPCLOUDSDK_TRAN_CONTROL_COND ;


//!透传数据给NVR/IPC上的autop2p请求结构
typedef struct
{
	DWORD dwOpt;						//!0--put,1--get,2--post,3--delete
	DWORD dwUrlSize ;					//!透传url内容大小
	BYTE *lpUrlData ;					//!透传url内容
	DWORD dwBinarySize ;                //!透传数据内容大小
	BYTE *lpBinaryData ;              //!透传数据内容
} CLOUDSDK_TRAN_CONTROL_COND_V2, *LPCLOUDSDK_TRAN_CONTROL_COND_V2 ;


//!透传控制响应结构
typedef struct
{
	WORD wResult ;                      		//!响应结果 cloudsdk_result_code_t
	WORD wRes ;                         		//!保留位,暂时不用
	DWORD dwDataSize ;                  	//!透传响应内容大小
	BYTE byRes[3];
	BYTE byBinaryData[1] ;              	//!透传请求内容第一个字节地址
} CLOUDSDK_TRAN_CONTROL_RESULT, *LPCLOUDSDK_TRAN_CONTROL_RESULT ;


//!报警事件订阅与取消报警事件订阅结构
typedef struct
{
	DWORD dwAlarmType ;
	WORD wChannel ;
	WORD wRes ;
	DWORD dwBeginTime ;
	DWORD dwEndTime ;
} CLOUDSDK_EVENT_COND, *LPCLOUDSDK_EVENT_COND ;


typedef struct
{
	S8 szSuperAddr[128] ;
	S8 szP2PServerAddr[64] ;
	U16 u16SuperPort ;
	U16 u16P2PServerPort ;
	U8 u8Res[4] ;
} CLOUDSDK_SERVER_INFO, *LPCLOUDSDK_SERVER_INFO ;

typedef struct
{
	U16 u16DevType;	   //!设备类型1:DVR 2:NVR 3:DVS/IPC,4-DEC ......
	U16 u16DevChanNum; //!设备通道数
	S8 s8UserName[64];
	S8 s8Password[64]; //!密码,加密过后的，采用3des加密
	U8 u8Res[1024];
} CLOUDSDK_START_DEVICE_INFO, *LPCLOUDSDK_START_DEVICE_INFO;

typedef struct
{
	S8 szSerialNo[48];
	WORD wSrcPort;
	WORD wDestPort;
	BYTE byRes[64];
} CLOUDSDK_MODIFY_PORT, *LPCLOUDSDK_MODIFY_PORT;

typedef struct
{
	char szSerialNo[48];
	char szSrcIPAddr[40];
	char szDestIPAddr[40];
	BYTE byRes[64];
} CLOUDSDK_MODIFY_IPADDR, *LPCLOUDSDK_MODIFY_IPADDR;

#endif

