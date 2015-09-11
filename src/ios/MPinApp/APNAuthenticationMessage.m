//
//  APNService.m
//  MPinApp
//
//  Created by Georgi Georgiev on 9/10/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

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
