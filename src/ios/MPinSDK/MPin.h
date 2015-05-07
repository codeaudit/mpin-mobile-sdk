//
//  MPinSDK.h
//  MPinSDK
//
//  Created by Georgi Georgiev on 11/17/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IUser.h"
#import "MpinStatus.h"
#import "OTP.h"

@interface MPin : NSObject

+ (MpinStatus*)initWithConfig:(const NSDictionary*)config;

+ (MpinStatus*)TestBackend:(const NSString*)url;
+ (MpinStatus*)SetBackend:(const NSString*)url;
+ (MpinStatus*)TestBackend:(const NSString*)url rpsPrefix:(NSString*)rpsPrefix;
+ (MpinStatus*)SetBackend:(const NSString*)url rpsPrefix:(NSString*)rpsPrefix;

+ (id<IUser>)MakeNewUser:(const NSString*)identity;
+ (id<IUser>)MakeNewUser:(const NSString*)identity
              deviceName:(const NSString*)devName;

+ (void)DeleteUser:(const id<IUser>)user;

+ (MpinStatus*)StartRegistration:(const id<IUser>)user;
+ (MpinStatus*)RestartRegistration:(const id<IUser>)user;
+ (MpinStatus*)StartRegistration:(const id<IUser>)user userData:(NSString *) userData;
+ (MpinStatus*)RestartRegistration:(const id<IUser>)user userData:(NSString *) userData;
+ (MpinStatus*)FinishRegistration:(const id<IUser>)user;
+ (MpinStatus*)ResetPin:(const id<IUser>)user;

+ (MpinStatus*)Authenticate:(const id<IUser>)user;
+ (MpinStatus*)Authenticate:(const id<IUser>)user authResultData:(NSString **)authResultData;
+ (MpinStatus*)AuthenticateOTP:(id<IUser>)user otp:(OTP**)otp;
+ (MpinStatus*)AuthenticateAN:(id<IUser>)user accessNumber:(NSString *)an;

+ (Boolean)Logout:(const id<IUser>)user;
+ (Boolean)CanLogout:(const id<IUser>)user;

+ (NSMutableArray*)listUsers;

+ (NSString *) GetClientParam:(const NSString *) key;

+ (void)sendPin:(const NSString*)pin;

/// TEMPORARY FIX
+ (NSString*)getRPSUrl;

@end
