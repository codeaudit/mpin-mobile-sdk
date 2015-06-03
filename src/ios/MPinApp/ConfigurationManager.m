//
//  ConfigurationManager.m
//  MPinApp
//
//  Created by Tihomir Ganev on 17.Mar.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "ConfigurationManager.h"
#import "Constants.h"
#import "NSString+Helper.h"


static NSString *const kCurrentSelectionIndex = @"currentSelectionIndex";
static NSString *const kSettings = @"settings";

@interface ConfigurationManager ( )

@property ( nonatomic, strong ) NSMutableArray *arrConfigrations;

- ( BOOL )saveConfigurationAtIndex:( NSInteger )index configData:( NSDictionary * )configData;

@end

@implementation ConfigurationManager

+ ( ConfigurationManager * )sharedManager
{
    static ConfigurationManager *sharedManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^ {
        sharedManager = [[self alloc] init];
    });

    return sharedManager;
}

- ( instancetype )init
{
    self = [super init];
    if ( self )
    {
        _intSelectedConfiguration = [[NSUserDefaults standardUserDefaults] integerForKey:kCurrentSelectionIndex];
        _arrConfigrations = [[[NSUserDefaults standardUserDefaults] objectForKey:kSettings] mutableCopy];
        if ( _arrConfigrations == nil )
        {
            NSDictionary *data = @{ kRPSURL : @"http://tcb.certivox.org",
                                    kSERVICE_TYPE : @( LOGIN_ON_MOBILE ),
                                    kCONFIG_NAME : @"Mobile banking login" };
            NSDictionary *dataOTP = @{ kRPSURL : @"http://otp.m-pin.id",
                                       kSERVICE_TYPE : @( LOGIN_WITH_OTP ),
                                       kCONFIG_NAME : @"VPN login" };
            NSDictionary *dataAN = @{ kRPSURL : @"http://tcb.certivox.org",
                                      kSERVICE_TYPE : @( LOGIN_ONLINE ),
                                      kCONFIG_NAME : @"Online banking login" };

            _arrConfigrations = [NSMutableArray array];
            [_arrConfigrations addObject:data];
            [_arrConfigrations addObject:dataOTP];
            [_arrConfigrations addObject:dataAN];

            [self saveConfigurations];
        }
    }

    return self;
}

- ( BOOL )saveConfigurationAtIndex:( NSInteger )index configData:( NSDictionary * )configData
{
    if ( index < [_arrConfigrations count] )
    {
        _arrConfigrations [index] = configData;
        [self saveConfigurations];

        return YES;
    }

    return false;
}

- ( void )addConfigurationWithURL:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName
{
    NSDictionary *data = @{ kRPSURL : url,
                            kSERVICE_TYPE : @( serviceType ),
                            kCONFIG_NAME : configurationName };

    [_arrConfigrations addObject:data];
    [self saveConfigurations];
}

- ( BOOL )saveConfigurationAtIndex:( NSInteger )index url:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName
{
    if ( index < [_arrConfigrations count] )
    {
        NSDictionary *data = @{ kRPSURL : url,
                                kSERVICE_TYPE : @( serviceType ),
                                kCONFIG_NAME : configurationName };

        _arrConfigrations [index] = data;
        [self saveConfigurations];

        return YES;
    }

    return false;
}

- ( BOOL )deleteConfigurationAtIndex:( NSInteger )index
{
    if ( _intSelectedConfiguration == index )
    {
        //What to do here?
        [_arrConfigrations removeObjectAtIndex:index];
        [self saveConfigurations];

        return YES;
    }

    return false;
}

- ( BOOL )setSelectedConfiguration:( NSInteger )index
{
    if ( index < [_arrConfigrations count] )
    {
        if ( index == _intSelectedConfiguration )
        {
            _intSelectedConfiguration = NOT_SELECTED;
        }
        else
        {
            _intSelectedConfiguration = index;
        }
        [[NSUserDefaults standardUserDefaults] setInteger:_intSelectedConfiguration forKey:kCurrentSelectionIndex];
        [self saveConfigurations];

        return true;
    }

    return false;
}

- ( BOOL )setSelectedUserForCurrentConfiguration:( NSInteger )userIndex
{
    NSMutableDictionary *dictConfig = (NSMutableDictionary *)[self getSelectedConfiguration];
    dictConfig [kSelectedUser] = @( userIndex );

    return [self saveConfigurationAtIndex:_intSelectedConfiguration configData:dictConfig];
}

- ( NSInteger )getSelectedUserIndexforSelectedConfiguration
{
    NSDictionary *configDict = [self getSelectedConfiguration];
    NSNumber *nSelectedUserIndex = configDict [kSelectedUser];

    if ( nSelectedUserIndex == nil )
        return NOT_SELECTED;

    return [nSelectedUserIndex integerValue];
}

- ( NSString * )getURLAtIndex:( NSInteger )index
{
    if ( index < [_arrConfigrations count] )
    {
        NSDictionary *dictConfiguration = _arrConfigrations [index];

        return dictConfiguration [kRPSURL];
    }

    return @"";
}

- ( NSString * )getNameAtIndex:( NSInteger )index
{
    if ( index < [_arrConfigrations count] )
    {
        NSDictionary *dictConfiguration = _arrConfigrations [index];

        return dictConfiguration [kCONFIG_NAME];
    }

    return @"";
}

- ( NSString * )getPrefixAtIndex:( NSInteger )index
{
    if ( index < [_arrConfigrations count] )
    {
        NSDictionary *dictConfiguration = _arrConfigrations [index];

        return dictConfiguration [kRPSPrefix];
    }

    return @"";
}

- ( NSInteger )getConfigurationTypeAtIndex:( NSInteger )index
{
    if ( index < [_arrConfigrations count] )
    {
        NSDictionary *dictConfiguration = _arrConfigrations [index];

        return [dictConfiguration [kSERVICE_TYPE] integerValue];
    }

    return -1;
}

- ( BOOL )getIsDeviceName:( NSInteger )index
{
    if ( index < [_arrConfigrations count] )
    {
        NSDictionary *dictConfiguration = _arrConfigrations [index];

        return [dictConfiguration [kIS_DN] boolValue];
    }

    return NO;
}

- ( NSInteger )getConfigurationsCount
{
    return [_arrConfigrations count];
}

- ( void )saveConfigurations
{
    [[NSUserDefaults standardUserDefaults] setInteger:_intSelectedConfiguration forKey:kCurrentSelectionIndex];
    [[NSUserDefaults standardUserDefaults] setObject:_arrConfigrations forKey:kSettings];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- ( NSDictionary * )getSelectedConfiguration
{
    return [NSMutableDictionary dictionaryWithDictionary:[_arrConfigrations objectAtIndexedSubscript:_intSelectedConfiguration]];
}

- ( NSInteger )getSelectedConfigurationIndex
{
    return _intSelectedConfiguration;
}

-( NSString * ) getDeviceName
{
    NSString *devName = [[NSUserDefaults standardUserDefaults] objectForKey:kDeviceName];
    if ( devName == nil )
        return kDefaultDeviceName;

    return devName;
}

-( void ) setDeviceName:( NSString * ) devName
{
    if ( [NSString isBlank:devName] )
        return;

    if ( [kDefaultDeviceName isEqualToString:devName] )
        return;

    [[NSUserDefaults standardUserDefaults] setObject:devName forKey:kDeviceName];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

@end
