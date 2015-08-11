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
