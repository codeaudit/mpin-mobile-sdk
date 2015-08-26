//
//  TestUtils.m
//  MPinApp
//
//  Created by Tihomir Ganev on 13.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "TestUtils.h"
#include <stdlib.h>

@implementation TestUtils

- ( NSString * ) randomText
{
    NSString *letters = @"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    int len = rand() % 10000;

    NSMutableString *randomString = [NSMutableString stringWithCapacity: len];

    for ( int i = 0; i < len; i++ )
    {
        [randomString appendFormat: @"%C", [letters characterAtIndex: arc4random_uniform((int)[letters length])]];
    }
    return randomString;
}

- (NSInteger) randomInt
{
    return rand() % 100000000;
}

@end
