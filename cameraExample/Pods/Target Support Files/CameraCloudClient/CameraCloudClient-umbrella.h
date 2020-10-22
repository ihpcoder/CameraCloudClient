#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "AntsUtil.h"
#import "ClientSession.h"
#import "CloudClientSDK.h"
#import "CloudClientSDK4iOS.h"
#import "CloudClientSDKCommon.h"
#import "CloudSDKCommon.h"
#import "VideoLayer.h"
#import "VideoView.h"

FOUNDATION_EXPORT double CameraCloudClientVersionNumber;
FOUNDATION_EXPORT const unsigned char CameraCloudClientVersionString[];

