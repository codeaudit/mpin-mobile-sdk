//
//  NetworkMonitor.h
//  MPinApp
//
//  Created by Tihomir Ganev on 4.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AFNetworkReachabilityManager.h"

@interface NetworkMonitor : NSObject

+ ( NetworkMonitor * )sharedManager;

@property (nonatomic) BOOL networkStatusUp;

@end
