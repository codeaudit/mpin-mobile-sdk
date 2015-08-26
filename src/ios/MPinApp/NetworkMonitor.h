//
//  NetworkMonitor.h
//  MPinApp
//
//  Created by Tihomir Ganev on 4.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface NetworkMonitor : NSObject

+ ( NetworkMonitor * )sharedManager;
+ ( bool )isNetworkAvailable;

@property ( nonatomic ) BOOL networkStatusUp;

@end
