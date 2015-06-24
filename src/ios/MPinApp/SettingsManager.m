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


        NSAssert(dict [@"COLORS"] != nil, @"Missing colors in Settings file");
        NSAssert(dict [@"BACKENDS"] != nil, @"Missing backends in Settings file");


        //TODO: Add nsassert and check for valid hex colors
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

        NSAssert(dict [@"HOCKEYAPP_ID"] != nil, @"Missing HOCKEYAPP_ID in Settings file");
        _strHockeyAppID = dict [@"HOCKEYAPP_ID"];

        NSAssert(dict [@"URL_MOBILE_GUIDE"] != nil, @"Missing URL_MOBILE_GUIDE in Settings file");
        _strUrlMobGuide = dict [@"URL_MOBILE_GUIDE"];

        NSAssert(dict [@"URL_SDK"] != nil, @"Missing URL_SDK in Settings file");
        _strUrlSDK      = dict [@"URL_SDK"];

        NSAssert(dict [@"URL_HOMEPAGE"] != nil, @"Missing URL_HOMEPAGE in Settings file");
        _strUrlHomepage = dict [@"URL_HOMEPAGE"];

        NSAssert(dict [@"URL_SUPPORT"] != nil, @"Missing URL_SUPPORT in Settings file");
        _strUrlSupport  = dict [@"URL_SUPPORT"];

        NSAssert(dict [@"URL_TERMS"] != nil, @"Missing URL_TERMS in Settings file");
        _strUrlTerms    = dict [@"URL_TERMS"];

        NSAssert(dict [@"URL_VALUES"] != nil, @"Missing URL_VALUES in Settings file");
        _strUrlValues   = dict [@"URL_VALUES"];
    }

    return self;
}

@end
