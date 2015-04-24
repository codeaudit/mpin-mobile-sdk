//
//  NSString+Helper.m
//  MPinApp
//
//  Created by Georgi Georgiev on 4/3/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "NSString+Helper.h"

static NSString *const kEmpty = @"";

@implementation NSString(Helper)

+(Boolean) isBlank:(NSString *) str {
    if (str == nil) return YES;
    return [kEmpty isEqualToString:str];
}

+(Boolean) isNotBlank:(NSString *) str {
    return ![NSString isBlank:str];
}

@end
