//
//  ConnectionManager.h
//  MPinApp
//
//  Created by Georgi Georgiev on 5/14/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MPin+AsyncOperations.h"

@interface ApplicationManager : NSObject <MPinSDKDelegate>

+ (ApplicationManager*)sharedManager;
- (void) runNetowrkMonitoring;


@end
