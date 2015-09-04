/*
   Copyright (c) 2012-2015, Certivox
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

   3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   For full details regarding our CertiVox terms of service please refer to
   the following links:
 * Our Terms and Conditions -
   http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
   http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
   http://www.certivox.com/about-certivox/patents/
 */


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
        _color11 = [UIColor colorWithHexString:dict [@"COLORS"] [@"color11"]];

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
