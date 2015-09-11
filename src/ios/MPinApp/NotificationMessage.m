//
//  NotificationService.m
//  MPinApp
//
//  Created by Georgi Georgiev on 9/10/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "NotificationMessage.h"

@implementation NotificationMessage

- ( BOOL ) setUserID:(NSString *) userID forHashValue:(NSString *) hash_user_id {
    if ( hash_user_id ==  nil || userID == nil ) {
        self.error = [NSError errorWithDomain:@"hash_user_id or userID are nil" code:-1 userInfo:nil];
        return NO;
    }
    [[NSUserDefaults standardUserDefaults] setObject:userID forKey:hash_user_id];
    [[NSUserDefaults standardUserDefaults] synchronize];
    return YES;
}



- ( NSString * ) getUserID:(NSString *) hash_user_id {
    if (hash_user_id == nil) return nil;
    return  [[NSUserDefaults standardUserDefaults] objectForKey:hash_user_id];;
}



@end
