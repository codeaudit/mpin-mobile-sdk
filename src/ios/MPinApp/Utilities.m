//
//  Utilities.m
//  MPinApp
//
//  Created by Georgi Georgiev on 7/13/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "Utilities.h"



@implementation Utilities

+( enum SERVICES ) ServerJSONConfigTypeToService_type:( NSString * ) jsonTypeName
{
    if ( [kJSON_TYPE_MOBILE isEqualToString:jsonTypeName] )
    {
        return LOGIN_ON_MOBILE;
    }
    else
    if ( [kJSON_TYPE_ONLINE isEqualToString:jsonTypeName] )
    {
        return LOGIN_ONLINE;
    }
    else
    {
        return LOGIN_WITH_OTP;
    }
}

@end
