//
//  User.m
//  MPinSDK
//
//  Created by Georgi Georgiev on 11/21/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import "User.h"


@interface User() {
    UserPtr userPtr;
}

@end

@implementation User

- (id) initWith:(UserPtr) usrPtr {
    self = [super init];
    if (self) {
        userPtr = usrPtr;
    }
    return self;
}

-(NSString *) getIdentity {
    return [NSString stringWithUTF8String:userPtr->GetId().c_str()];
}

-(UserState) getState {
    return (UserState)userPtr->GetState();
}

-(UserPtr) getUserPtr {
    return userPtr;
}

@end
