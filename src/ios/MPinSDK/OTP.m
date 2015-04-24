//
//  OTP.m
//  MPinSDK
//
//  Created by Georgi Georgiev on 12/19/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import "OTP.h"

@implementation OTP

-(id) initWith:(MpinStatus *)status otp:(NSString*) otp expireTime:(long) expTime ttlSeconds:(int) ttlSeconds nowTime:(long) nowTime {
    self = [super init];
    if (self) {
        _status = status;
        _otp = otp;
        _expireTime = expTime;
        _ttlSeconds = ttlSeconds;
        _nowTime = nowTime;
    }
    return self;
}

@end
