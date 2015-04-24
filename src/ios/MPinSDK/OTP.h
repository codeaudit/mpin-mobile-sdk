//
//  OTP.h
//  MPinSDK
//
//  Created by Georgi Georgiev on 12/19/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "MpinStatus.h"

@interface OTP : NSObject
@property (nonatomic, retain, readonly) MpinStatus * status;
@property (nonatomic, retain , readonly) NSString * otp;
@property (atomic, readonly) long expireTime;
@property (atomic, readonly) int ttlSeconds;
@property (atomic, readonly) long nowTime;

-(id) initWith:(MpinStatus *)status otp:(NSString*) otp expireTime:(long) expTime ttlSeconds:(int) ttlSeconds nowTime:(long) nowTime;

@end
