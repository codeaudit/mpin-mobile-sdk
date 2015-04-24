//
//  SettingsManager.m
//  MPinApp
//
//  Created by Tihomir Ganev on 10.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import "SettingsManager.h"

@implementation SettingsManager

+ (SettingsManager*)sharedManager
{
    static SettingsManager* sharedSettingsManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedSettingsManager = [[self alloc] init];
    });
    return sharedSettingsManager;
}

- (instancetype)init
{
    if (self = [super init]) {
        NSString* filePath = [[NSBundle mainBundle] pathForResource:@"Colors" ofType:@"plist"];
        NSDictionary* dict = [[NSDictionary alloc] initWithContentsOfFile:filePath];

        //TODO: Add nsassert and check for valid colors
        _color0 = [UIColor colorWithHexString:dict[@"color0"]];
        _color1 = [UIColor colorWithHexString:dict[@"color1"]];
        _color2 = [UIColor colorWithHexString:dict[@"color2"]];
        _color3 = [UIColor colorWithHexString:dict[@"color3"]];
        _color4 = [UIColor colorWithHexString:dict[@"color4"]];
        _color5 = [UIColor colorWithHexString:dict[@"color5"]];
        _color6 = [UIColor colorWithHexString:dict[@"color6"]];
        _color7 = [UIColor colorWithHexString:dict[@"color7"]];
        _color8 = [UIColor colorWithHexString:dict[@"color8"]];
        _color9 = [UIColor colorWithHexString:dict[@"color9"]];
        _color10 = [UIColor colorWithHexString:dict[@"color10"]];
    }
    return self;
}

- (void)dealloc
{
    // Should never be called, but just here for clarity really.
}

@end
