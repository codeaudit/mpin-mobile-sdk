//
//  IUser.h
//  MPinSDK
//
//  Created by Georgi Georgiev on 11/25/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, UserState) {
    INVALID = 0,
    STARTED_REGISTRATION,
    ACTIVATED,
    REGISTERED,
    BLOCKED
};

@protocol IUser <NSObject>
- (NSString*)getIdentity;
- (UserState)getState;
@end
