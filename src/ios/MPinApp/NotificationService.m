//
//  NotificationService.m
//  MPinApp
//
//  Created by Georgi Georgiev on 9/11/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "NotificationService.h"

@implementation NotificationService

- ( void ) postNotification:(NotificationMessage *) message {
    if (message == nil) return;
    if (self.delegate == nil) return;
    
    if (message.error != nil) {
        if ( [(NSObject *)self.delegate respondsToSelector:@selector( OnNotificationError:error: )] )
            [self.delegate OnNotificationError:self error:message.error];
        return;
    }
    
    if ( [(NSObject *)self.delegate respondsToSelector:@selector( OnReceiveNotification:message: )] )
        [self.delegate OnReceiveNotification:self message:message];
}

@end
