//
//  SettingsManager.m
//  MPinApp
//
//  Created by Tihomir Ganev on 10.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import "SettingsManager.h"

@implementation SettingsManager

+ ( SettingsManager * )sharedManager
{
    static SettingsManager *sharedSettingsManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^ {
        sharedSettingsManager = [[self alloc] init];
    });

    return sharedSettingsManager;
}

- ( instancetype )init
{
    if ( self = [super init] )
    {
        NSString *filePath = [[NSBundle mainBundle] pathForResource:@"Settings" ofType:@"plist"];
        NSDictionary *dict = [[NSDictionary alloc] initWithContentsOfFile:filePath];

        //TODO: Add nsassert and check for valid colors
        NSAssert(dict [@"COLORS"] != nil, @"Missing colors in Settings file");
        NSAssert(dict [@"BACKENDS"] != nil, @"Missing backends in Settings file");
        NSAssert(dict [@"HOCKEYAPP_ID"] != nil, @"Missing HOCKEYAPP_ID in Settings file");

        _color0 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color0"]];
        _color1 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color1"]];
        _color2 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color2"]];
        _color3 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color3"]];
        _color4 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color4"]];
        _color5 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color5"]];
        _color6 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color6"]];
        _color7 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color7"]];
        _color8 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color8"]];
        _color9 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color9"]];
        _color10 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color10"]];

        _strHockeyAppID = dict [@"HOCKEYAPP_ID"];
    }

    return self;
}

- ( void )dealloc
{
    // Should never be called, but just here for clarity really.
}

@end
