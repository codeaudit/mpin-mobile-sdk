//
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

#import "APNAuthenticationMessage.h"

static NSString *kAPS = @"aps";
static NSString *kAlert = @"alert";
static NSString *kAccessNumber = @"mobileToken";

@implementation APNAuthenticationMessage

- ( id ) initWith:( NSDictionary * ) userInfo {
    if ( self = [super init] )
    {
        if( userInfo == nil )     {
            self.error = [NSError errorWithDomain:@"APNAuthMessage user info is nil" code:-1 userInfo:nil];
            return self;
        }
        if( userInfo[kAPS] == nil ) {
            self.error = [NSError errorWithDomain:@"UserInfo missing aps: field" code:-1 userInfo:nil];
            return self;
        }
        if( userInfo[kAPS][kAlert] == nil ) {
            self.error = [NSError errorWithDomain:@"UserInfo missing alert: field" code:-1 userInfo:nil];
            return self;
        }
        
        NSString * JsonNotificationData = userInfo[kAPS][kAlert];
        NSError *error = nil;
        NSDictionary *notificationDictionary = [NSJSONSerialization JSONObjectWithData:[JsonNotificationData dataUsingEncoding:NSUTF8StringEncoding] options:kNilOptions error:&error];
        if(error != nil) {
            self.error = error;
            return self;
        }
        
        NSString * hash_user_id =  notificationDictionary[kHashUserId];
        self.accessNumber = notificationDictionary[kAccessNumber];
        
        if (hash_user_id == nil || self.accessNumber == nil) {
            self.error = [NSError errorWithDomain:@"Invalid AccessNumber or hash_user_id" code:-1 userInfo:nil];
            return self;
        }
        
        self.userID = [self getUserID:hash_user_id];
        
        if (self.userID == nil) {
            self.error = [NSError errorWithDomain:@"UserID is nil. It has not been stored proporly in the persistent storage!" code:-1 userInfo:nil];
            return self;
        }

    }
    return self;
}

@end
