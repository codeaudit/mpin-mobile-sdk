//
//  NotificationService.h
//  MPinApp
//
//  Created by Georgi Georgiev on 9/10/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>

static NSString *kHashUserId = @"hash_user_id";

@interface NotificationMessage : NSObject

@property (nonatomic, retain ) NSString * userID;
@property ( nonatomic, retain ) NSError * error;


- ( BOOL ) setUserID:(NSString *) userID forHashValue:(NSString *) hash_user_id;

- ( NSString * ) getUserID:(NSString *) hash_user_id;

@end
