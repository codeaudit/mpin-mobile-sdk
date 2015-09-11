/*
 Copyright (c) 2012-2015, Certivox
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 For full details regarding our CertiVox terms of service please refer to
 the following links:
 * Our Terms and Conditions -
 http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
 http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
 http://www.certivox.com/about-certivox/patents/
 */


#import "MPin.h"
#import "mpin_sdk.h"
#import "def.h"
#import "Context.h"
#import <vector>
#import "User.h"
#import "Constants.h"

static MPinSDK mpin;
static BOOL isInitialized = false;

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

+(void) initSDK {
    if (isInitialized) return;
    StringMap sm;
    [lock lock];
    mpin.Init(sm, Context::Instance());
    [lock unlock];
    isInitialized = true;
}

+(MpinStatus *) TestBackend:(const NSString * ) url {
    [lock lock];
    Status s = mpin.TestBackend((url == nil)?(""):([url UTF8String]));
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+(MpinStatus *) SetBackend:(const NSString * ) url {
    [lock lock];
    Status s = mpin.SetBackend((url == nil)?(""):([url UTF8String]));
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

+ (MpinStatus*) VerifyUser:(const id<IUser>)user mpinId:(NSString* ) mpinId activationKey:(NSString *) activationKey {
    [lock lock];
    Status s = mpin.VerifyUser([((User *) user) getUserPtr], [mpinId UTF8String], [activationKey UTF8String]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+ (MpinStatus*) FinishRegistration:(const id<IUser>) user {
    [lock lock];
    Status s = mpin.FinishRegistration([((User *) user) getUserPtr]);
    [lock unlock];
    return [[MpinStatus alloc] initWith:(MPinStatus)s.GetStatusCode() errorMessage:[NSString stringWithUTF8String:s.GetErrorMessage().c_str()]];
}

+ (MpinStatus*)FinishRegistration:(const id<IUser>)user pushNotificationIdentifier:(NSString *) pushNotificationIdentifier {
    [lock lock];
    Status s = mpin.FinishRegistration([((User *) user) getUserPtr], [pushNotificationIdentifier UTF8String]);
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

+ ( id<IUser> ) getIUserById:(NSString *) userId {
    if( userId == nil ) return nil;
    if ([@"" isEqualToString:userId]) return nil;
    
    NSArray * users = [MPin listUsers];
    
    for (User * user in users)
        if ( [userId isEqualToString:[user getIdentity]] )
            return user;
    
    return nil;
}


+(void) sendPin:(const NSString *) pin {
    Context *ctx = Context::Instance();
    ctx->setPin([pin UTF8String]);
}

@end
