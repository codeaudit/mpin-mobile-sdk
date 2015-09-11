//
//  SMSNotificationService.h
//  MPinApp
//
//  Created by Georgi Georgiev on 9/10/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "NotificationMessage.h"


@interface SMSRegistrationMessage : NotificationMessage

@property ( nonatomic , retain ) NSString * mpinId;
@property ( nonatomic , retain ) NSString * activateKey;

- ( id ) initWith:( NSURL * ) url;

@end
