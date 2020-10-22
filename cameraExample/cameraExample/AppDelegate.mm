//
//  AppDelegate.m
//  cameraExample
//
//  Created by yuhao yuhao on 2020/10/22.
//

#import "AppDelegate.h"
#import "ViewController.h"
#import <CameraCloudClient/CloudClientSDK4iOS.h>
@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    [CloudClientSDK4iOS getInstance];
    
    
//    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
//    self.window.backgroundColor = [UIColor whiteColor];
//
//    self.window.rootViewController = [[ViewController alloc] init];;
//    
//    [self.window makeKeyAndVisible];
    
    return YES;
}


@end
