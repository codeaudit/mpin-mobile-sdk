//
//  NotificationService.h
//  MPinApp
//
//  Created by Georgi Georgiev on 9/11/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NotificationMessage.h"

@protocol NotificationDelegate
@optional
- ( void ) OnReceiveNotification:( id ) sender message:(NotificationMessage *) message;
- ( void ) OnNotificationError:( id ) sender error:( NSError * ) error;
@end

@interface NotificationService : NSObject

@property(nonatomic, assign) id<NotificationDelegate> delegate;

- ( void ) postNotification:(NotificationMessage *) message;

@end
