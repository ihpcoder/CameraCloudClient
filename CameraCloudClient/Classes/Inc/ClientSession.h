#pragma once

#import <objc/NSObject.h>
#import "CloudSdkCommon.h"
#import "CloudClientSDK.h"

typedef void (^fResponseCallBack)(void* lpData, unsigned int nDataLen) ;

typedef int (^fStreamDataCallBack)(char *lpInBuffer1, int nBufSize1, char *lpInBuffer2, int nBufSize2,HANDLE handle) ;

typedef void (^fSearchDevCallBack)(char *lpDevUid, unsigned char*lpBuf, int nBufSize) ;
typedef void (^fExceptionHandleCallBack)(void *hHandle) ;

@interface ClientSession : NSObject
@property(nonatomic,assign)void* hUserId;
@property (copy, nonatomic) fStreamDataCallBack lpStreamCbFun;
@property (copy, nonatomic) fExceptionHandleCallBack lpExceptionCbFun;

//20190603 - add by Jason
@property (nonatomic,copy) NSString * seriesNum;

-(bool) SetParam:(void*)hUserId  lpStreamCbFun:(fStreamDataCallBack) lpStreamCbFun lpExceptionCbFun:(fExceptionHandleCallBack) lpExceptionCbFun;
-(int) CallStream:(char*)lpInBuffer1 nBufSize1:(int)nBufSize1 lpInBuffer2:(char*)lpInBuffer2  nBufSize2:(int)nBufSize2 handle:(HANDLE)handlehUserId;

-(bool) SetParam:(void*)hUserId lpExceptionCbFun:(fExceptionHandleCallBack) lpExceptionCbFun;
//-(bool)setlpStreamCbFun:(fStreamDataCallBack)lpStreamCbFun userId:(void * )hUserId;
-(bool)setlpStreamCbFun:(fStreamDataCallBack)lpStreamCbFun userId:(void * )hUserId ;
//              seriesNum:(NSString *)seriesNum;


-(void) CallException:(void*)hHandle ;
@end

