//
//  MPinSDK.m
//  MPinSDK
//
//  Created by Georgi Georgiev on 11/17/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import "MPin.h"
#import "mpin_sdk.h"
#import "def.h"
#import "Context.h"
#import <vector>
#import "User.h"
#import "Constants.h"

static MPinSDK mpin;

/// TEMPORARY FIX
static NSString * rpsURL;

static NSLock * lock = [[NSLock alloc] init];

typedef MPinSDK::UserPtr UserPtr;
typedef MPinSDK::Status Status;
typedef sdk::Context Context;

@implementation MPin

/// TEMPORARY FIX
+(NSString * ) getRPSUrl {
    return rpsURL;
}

+(MpinStatus *) initWithConfig:(NSDictionary*) config {
    /// TEMPORARY FIX
    rpsURL = [config objectForKey:kRPSURL];
    ///
    NSString * rpsPrefix = [config objectForKey:kRPSPrefix];
    
    StringMap sm;
    sm[([kRPSURL UTF8String])] = [rpsURL UTF8String];
    if (rpsPrefix != nil && rpsPrefix.length != 0) {
        sm[([kRPSPrefix UTF8String])] = [rpsPrefix UTF8String];
    }
    
    [lock lock];
    Status s =  mpin.Init(sm, Context::Instance());
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+(MpinStatus *) TestBackend:(const NSString * ) url {
    [lock lock];
    Status s = mpin.TestBackend([url UTF8String]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+(MpinStatus *) SetBackend:(const NSString * ) url {
    [lock lock];
    Status s = mpin.SetBackend([url UTF8String]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+(MpinStatus *) TestBackend:(const NSString * ) url rpsPrefix:(NSString *) rpsPrefix {
    if (rpsPrefix == nil || rpsPrefix.length == 0) {
        return [MPin TestBackend:url];
    }
    [lock lock];
    Status s = mpin.TestBackend([url UTF8String], [rpsPrefix UTF8String]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}
+(MpinStatus *) SetBackend:(const NSString * ) url rpsPrefix:(NSString *) rpsPrefix {
    if (rpsPrefix == nil || rpsPrefix.length == 0) {
        return [MPin SetBackend:url];
    }
    [lock lock];
    Status s = mpin.SetBackend([url UTF8String],[rpsPrefix UTF8String]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+ (id<IUser>) MakeNewUser:(const NSString *) identity {
    [lock lock];
    UserPtr userPtr = mpin.MakeNewUser([identity UTF8String]);
    [lock unlock];
    return [[User alloc] initWith:userPtr];
}

+ (id<IUser>) MakeNewUser: (const NSString *) identity deviceName:(const NSString *) devName {
    [lock lock];
    UserPtr userPtr = mpin.MakeNewUser([identity UTF8String], [devName UTF8String]);
    [lock unlock];
    return [[User alloc] initWith:userPtr];
}

+(void) DeleteUser:(const id<IUser>) user {
    [lock lock];
    mpin.DeleteUser([((User *) user) getUserPtr]);
    [lock unlock];
}

+ (MpinStatus*) StartRegistration:(const  id<IUser>) user {
    return [MPin StartRegistration:user userData:@""];
}

+ (MpinStatus*) RestartRegistration:(const id<IUser>) user {
    return [MPin RestartRegistration:user userData:@""];
}

+ (MpinStatus*)StartRegistration:(const id<IUser>)user userData:(NSString *) userData {
    [lock lock];
    Status s = mpin.StartRegistration([((User *) user) getUserPtr], [userData UTF8String]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
    
}
+ (MpinStatus*)RestartRegistration:(const id<IUser>)user userData:(NSString *) userData {
    [lock lock];
    Status s = mpin.RestartRegistration([((User *) user) getUserPtr], [userData UTF8String]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+ (MpinStatus*) FinishRegistration:(const id<IUser>) user {
    [lock lock];
    Status s = mpin.FinishRegistration([((User *) user) getUserPtr]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+ (MpinStatus*)ResetPin:(const id<IUser>)user {
    [lock lock];
    Status s = mpin.ResetPin([((User *) user) getUserPtr]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+ (MpinStatus*) Authenticate:(const id<IUser>) user {
    [lock lock];
    Status s = mpin.Authenticate([((User *) user) getUserPtr]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];;
}

+ (MpinStatus*)Authenticate:(const id<IUser>)user authResultData:(NSString **)authResultData {
    MPinSDK::String c_authResultData;
    [lock lock];
    Status s = mpin.Authenticate([((User *) user) getUserPtr], c_authResultData);
    [lock unlock];
    *authResultData = [NSString stringWithUTF8String:c_authResultData.c_str()];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+ (MpinStatus*) AuthenticateOTP:(id<IUser>) user otp:(OTP **) otp {
    MPinSDK::OTP c_otp;
    [lock lock];
    Status s = mpin.AuthenticateOTP([((User *) user) getUserPtr], c_otp);
    [lock unlock];
    *otp = [[OTP alloc] initWith:[[MpinStatus alloc] initWith:(MPinStatus)c_otp.status.GetStatusCode() errorMessage:[NSString stringWithUTF8String:c_otp.status.GetErrorMessage().c_str()]]
                             otp:[NSString stringWithUTF8String:c_otp.otp.c_str()]
                      expireTime:c_otp.expireTime
                      ttlSeconds:c_otp.ttlSeconds
                         nowTime:c_otp.nowTime];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+ (MpinStatus *) AuthenticateAN:(id<IUser>) user  accessNumber:(NSString *) an {
    [lock lock];
     Status s = mpin.AuthenticateAN([((User *) user) getUserPtr], [an UTF8String]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+ (Boolean) Logout:(const  id<IUser>) user {
    [lock lock];
    Boolean b = mpin.Logout([((User *) user) getUserPtr]);
    [lock unlock];
    return b;
}

+ (Boolean) CanLogout:(const  id<IUser>) user {
    [lock lock];
    Boolean b = mpin.CanLogout([((User *) user) getUserPtr]);
    [lock unlock];
    return b;
}

+(NSString *) GetClientParam:(const NSString *) key {
    [lock lock];
    String value = mpin.GetClientParam([key UTF8String]);
    [lock unlock];
    return [NSString stringWithUTF8String:value.c_str()];
}

+(NSMutableArray *) listUsers {
    NSMutableArray * users = [NSMutableArray array];
    std::vector<UserPtr> vUsers;
    mpin.ListUsers(vUsers);
    for (int i = 0; i<vUsers.size(); i++) {
        [users addObject:[[User alloc] initWith:vUsers[i]]];
    }
    return users;
}

+(void) sendPin:(const NSString *) pin {
    Context *ctx = Context::Instance();
    ctx->setPin([pin UTF8String]);
}

@end
