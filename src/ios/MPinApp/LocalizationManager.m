//
//  LocalizationManager.m
//  MPinApp
//
//  Created by Tihomir Ganev on 29.Apr.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "LocalizationManager.h"

@implementation LocalizationManager

+ (LocalizationManager*)sharedManager
{
    static LocalizationManager* sharedManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedManager = [[self alloc] init];
    });
    return sharedManager;
}

- (NSString *)localizedString:(NSString *)key comment:(NSString *)comment
{
    NSString *localizedString = @"";
    NSLocale *locale = [NSLocale currentLocale];
    NSString *path = [[NSBundle mainBundle] pathForResource:locale.localeIdentifier ofType:@"strings"];
    if ( ![[NSFileManager defaultManager] fileExistsAtPath:path] )
    {
        localizedString = [[NSBundle mainBundle] localizedStringForKey:key value:comment table:nil];
    }
    else
    {
        
        NSDictionary *dict = [NSDictionary dictionaryWithContentsOfFile:path];
        localizedString = [dict objectForKey:key];
        
    }

    return (localizedString == nil ) ?  @"" : localizedString;
}

@end
