//
//  NetworkMonitor.m
//  MPinApp
//
//  Created by Tihomir Ganev on 4.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "NetworkMonitor.h"
#import "ApplicationManager.h"

@interface NetworkMonitor()
{
    BOOL boolWasDown;
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
                if (boolWasDown)
                {
                    NSLog(@"Setting up backend");
                    [[ApplicationManager sharedManager] setBackend];
                }
                boolWasDown = NO;
                self.networkStatusUp = YES;
            break;

        default:
            {
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
