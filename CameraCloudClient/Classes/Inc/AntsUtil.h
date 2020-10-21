//
//  AntsUtil.h
//  VideoRemoteMonitor
//
//  Created by Jason on 2018/7/31.
//  Copyright © 2018年 duzhi. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AntsUtil : NSObject

//3DES解密方法
+(NSString *)ants_3des_decFun:(NSString *)encodeStr;

//3DES加密方法
+(NSString *)encode_3desStr:(NSString *)srcStr;

@end
