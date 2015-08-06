//
//  Constants.h
//  MPinSDK
//
//  Created by Georgi Georgiev on 11/27/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#ifndef MPinSDK_Constants_h
#define MPinSDK_Constants_h

#pragma mark - Navigation Controller Titles -

static NSString *const kSettingsFile = @"Settings";
static NSString *const kBackendsKey = @"BACKENDS";


static NSString *kSetupPin = @"Setup PIN";
static NSString *kEnterPin = @"Enter Your PIN";

/// UI constants
static NSString *const kBackBarButtonItem = @"image.png";
static NSString *const kDevName = @"Device Name";

static NSString *const kRPSURL = @"backend";
static NSString *const kRPSPrefix = @"rps_prefix";
static NSString *const kSERVICE_TYPE = @"SERVICE_TYPE";
static NSString *const kIS_DN = @"dn";
static NSString *const kCONFIG_NAME = @"CONFIG_NAME";
static NSString *const kSelectedUser = @"SELECTED_USER";
static NSString *const kConfigHashValue = @"hashValue";
static NSString *const kDefConfigThreshold = @"DefConfigThreshold";
static NSString *const kSelectedConfiguration = @"SelectedConfiguration";

/// BEGIN JSON CONFIG FROM SERVER
static NSString *const kJSON_URL = @"url";
static NSString *const kJSON_NAME = @"name";
static NSString *const kJSON_TYPE = @"type";
static NSString *const kJSON_PREFIX = @"prefix";

static NSString *const kJSON_TYPE_OTP = @"otp";
static NSString *const kJSON_TYPE_MOBILE = @"mobile";
static NSString *const kJSON_TYPE_ONLINE = @"online";
//// END


static NSString *const kDeviceName = @"setDeviceName";
static NSString *const kDefaultDeviceName = @"Sample IOS App";
static NSString *const kShowPinPadNotification = @"ShowPinPadNotification";

static NSString *const kEmptyStr = @"";

static NSString *constStrNetworkDown = @"NetworkDown";
static NSString *constStrNetworkUp = @"NetworkUp";

enum SERVICES
{
    LOGIN_ON_MOBILE = 0,
    LOGIN_ONLINE    = 1,
    LOGIN_WITH_OTP  = 2
};

#endif
