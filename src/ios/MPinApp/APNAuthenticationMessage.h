//
//  APNService.h
//  MPinApp
//
//  Created by Georgi Georgiev on 9/10/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "NotificationMessage.h"

@interface APNAuthenticationMessage : NotificationMessage
@property(nonatomic, retain) NSString * accessNumber;

- ( id ) initWith:( NSDictionary * ) userInfo;

@end
