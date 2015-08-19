//
//  NetworkMonitor.m
//  MPinApp
//
//  Created by Tihomir Ganev on 4.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "NetworkMonitor.h"
#import "ApplicationManager.h"
#import "AppDelegate.h"
#import "ThemeManager.h"

@interface NetworkMonitor ( )
{
    BOOL boolWasDown;
    AppDelegate *appDelegate;
}
@end

@implementation NetworkMonitor

+ ( NetworkMonitor * )sharedManager
{
    static NetworkMonitor *sharedManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^ {
        sharedManager = [[self alloc] init];
    });

    return sharedManager;
}

- ( instancetype )init
{
    self = [super init];
    if ( self )
    {
        boolWasDown = YES;
        appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
        [self runNetowrkMonitoring];
    }

    return self;
}

- ( void ) runNetowrkMonitoring
{
    [[AFNetworkReachabilityManager sharedManager] setReachabilityStatusChangeBlock: ^ (AFNetworkReachabilityStatus status) {
        switch ( status )
        {
        case AFNetworkReachabilityStatusReachableViaWiFi:
        case AFNetworkReachabilityStatusReachableViaWWAN:
            NSLog(@"!!! NETWORK UP");
            if ( boolWasDown )
            {
                [[ApplicationManager sharedManager] setBackend];
                dispatch_async(dispatch_get_main_queue(),^ {
                    [[NSNotificationCenter defaultCenter] postNotificationName: @"NETWORK_UP_NOTIFICATION" object:nil userInfo:nil];
                });
            }
            boolWasDown = NO;
            self.networkStatusUp = YES;
            break;

        default:
            {
                dispatch_async(dispatch_get_main_queue(),^ {
                    [[NSNotificationCenter defaultCenter] postNotificationName: @"NETWORK_DOWN_NOTIFICATION" object:nil userInfo:nil];
                });

                NSLog(@"!!! NETWORK DOWN");
                boolWasDown = YES;
                self.networkStatusUp = NO;
            }
            break;
        }
    }];

    [[AFNetworkReachabilityManager sharedManager] startMonitoring];
}

@end
