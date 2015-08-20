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
#import "AFNetworkReachabilityManager.h"
#import <SystemConfiguration/SCNetworkReachability.h>

@interface NetworkMonitor ( )
{
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
        appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
        [self runNetowrkMonitoring];
    }

    return self;
}

+( bool )isNetworkAvailable
{
    SCNetworkReachabilityFlags flags;
    SCNetworkReachabilityRef address;
    //TODO: Fix the address
    address = SCNetworkReachabilityCreateWithName(NULL, "www.apple.com" );
    Boolean success = SCNetworkReachabilityGetFlags(address, &flags);
    CFRelease(address);

    bool canReach = success
                    && !( flags & kSCNetworkReachabilityFlagsConnectionRequired )
                    && ( flags & kSCNetworkReachabilityFlagsReachable );

    return canReach;
}

- ( void ) runNetowrkMonitoring
{
    [[AFNetworkReachabilityManager sharedManager] setReachabilityStatusChangeBlock: ^ (AFNetworkReachabilityStatus status) {
        switch ( status )
        {
        case AFNetworkReachabilityStatusReachableViaWiFi:
        case AFNetworkReachabilityStatusReachableViaWWAN:
            dispatch_async(dispatch_get_main_queue(),^ {
                [[ApplicationManager sharedManager] setBackend];
                [[NSNotificationCenter defaultCenter] postNotificationName: @"NETWORK_UP_NOTIFICATION" object:nil userInfo:nil];
            });
            self.networkStatusUp = YES;
            break;

        default:
            {
                dispatch_async(dispatch_get_main_queue(),^ {
                    [[NSNotificationCenter defaultCenter] postNotificationName: @"NETWORK_DOWN_NOTIFICATION" object:nil userInfo:nil];
                });

                self.networkStatusUp = NO;
            }
            break;
        }
    }];
}

@end
