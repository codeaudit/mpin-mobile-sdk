//
//  NSString+Helper.m
//  MPinApp
//
//  Created by Georgi Georgiev on 4/3/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "NSString+Helper.h"

static NSString *const kEmpty = @"";
static NSString *const regExPattern = @"(http|https)://((\\w)*|([0-9]*)|([-|_])*)+([\\.|/"@"]((\\w)*|([0-9]*)|([-|_])*))+";

@implementation NSString(Helper)

+(Boolean) isBlank:(NSString *) str {
    if (str == nil) return YES;
    return [kEmpty isEqualToString:str];
}

+(Boolean) isNotBlank:(NSString *) str {
    return ![NSString isBlank:str];
}

+ (BOOL)isValidURL:(NSString*)url {
    if ([NSString isBlank:url]) return NO;
    
    NSRegularExpression* regEx = [[NSRegularExpression alloc] initWithPattern:regExPattern
                                                                      options:NSRegularExpressionCaseInsensitive
                                                                        error:nil];
    NSUInteger regExMatches = [regEx numberOfMatchesInString:url
                                                     options:0
                                                       range:NSMakeRange(0, [url length])];
    
    return (regExMatches != 0);
}



@end
