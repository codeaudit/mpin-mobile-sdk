//
//  User.h
//  MPinSDK
//
//  Created by Georgi Georgiev on 11/21/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "mpin_sdk.h"
#import "IUser.h"

typedef MPinSDK::UserPtr UserPtr;

@interface User : NSObject <IUser>

@property(nonatomic, retain, readwrite) NSString * identity;
@property(nonatomic,readwrite) UserState userState;

-(id) initWith:(UserPtr)usrPtr;
-(UserPtr) getUserPtr;

@end
