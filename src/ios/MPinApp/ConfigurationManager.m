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

@interface ConfigurationManager ( ) {
    NSInteger defaultConfigCount;
}

@property ( nonatomic, strong ) NSMutableArray *arrConfigrations;

- ( BOOL )saveConfigurationAtIndex:( NSInteger )index configData:( NSDictionary * )configData;
- ( BOOL ) validate:( NSDictionary * ) dict;

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

- ( BOOL ) validate:( NSDictionary * ) dict
{
    NSEnumerator *enumerator = [dict keyEnumerator];
    NSString *key;
    while ( ( key = [enumerator nextObject] ) )
    {
        if ( ( ![key isEqualToString:kRPSURL] ) &&
             ( ![key isEqualToString:kRPSPrefix] ) &&
             ( ![key isEqualToString:kSERVICE_TYPE] ) &&
             ( ![key isEqualToString:kCONFIG_NAME] ) )
            return false;

        if ( [key isEqualToString:kRPSURL]  && ![NSString isValidURL:[dict objectForKey:kRPSURL]] )
            return false;
    }

    return true;
}

- ( instancetype )init
{
    self = [super init];
    if ( self )
    {
        _intSelectedConfiguration = [[NSUserDefaults standardUserDefaults] integerForKey:kCurrentSelectionIndex];
        _arrConfigrations = [[[NSUserDefaults standardUserDefaults] objectForKey:kSettings] mutableCopy];
        if ( _arrConfigrations == nil )
            _arrConfigrations = [NSMutableArray array];

        NSString *filePath = [[NSBundle mainBundle] pathForResource:kSettingsFile ofType:@"plist"];
        NSDictionary *settingsDict = [[NSDictionary alloc] initWithContentsOfFile:filePath];
        NSArray *configs = [settingsDict objectForKey:kBackendsKey];
        if ( configs == nil )
            configs = [NSArray array];
        NSString *fileContent = [NSString stringWithFormat:@"%@", configs];

        NSInteger hashValue  = [[NSUserDefaults standardUserDefaults] integerForKey:kConfigHashValue];
        long configHash = [fileContent hash];

        defaultConfigCount = [configs count];


        if ( hashValue != configHash )
        {
            NSMutableArray *tmpArray = [NSMutableArray array];

            for ( int i = 0; i < [configs count]; i++ )
                if ( [self validate:configs [i]] )
                    [tmpArray addObject:configs [i]];

            if ( hashValue != 0 )
            {
                NSInteger threshold = [[NSUserDefaults standardUserDefaults] integerForKey:kDefConfigThreshold];
                for ( int i = (int)threshold; i < [_arrConfigrations count]; i++ )
                    [tmpArray addObject:_arrConfigrations [i]];

                if ( [configs count] != threshold )
                {
                    _intSelectedConfiguration = ( ( _intSelectedConfiguration >= threshold ) && ( [_arrConfigrations count] != 0 ) ) ? ( _intSelectedConfiguration + ( [configs count] - threshold ) ) : ( 0 );
                    [[NSUserDefaults standardUserDefaults] setInteger:_intSelectedConfiguration forKey:kCurrentSelectionIndex];
                }
            }

            _arrConfigrations = tmpArray;

            [[NSUserDefaults standardUserDefaults] setInteger:configHash forKey:kConfigHashValue];
            [[NSUserDefaults standardUserDefaults] setInteger:[configs count] forKey:kDefConfigThreshold];

            [self saveConfigurations];
        }
    }

    return self;
}

- ( BOOL )isEmpty
{
    return [_arrConfigrations count] == 0;
}

- ( NSInteger ) defaultConfigCount
{
    return defaultConfigCount;
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

- ( void )addConfiguration:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName prefixName:( NSString * ) prefixName
{
    NSDictionary *data = @{ kRPSURL : url,
                            kSERVICE_TYPE : @( serviceType ),
                            kCONFIG_NAME : configurationName,
                            kRPSPrefix : ( prefixName == nil ) ? ( @"" ) : ( prefixName ) };

    [_arrConfigrations addObject:data];
}

- ( void )addConfigurationWithURL:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName
{
    [self addConfigurationWithURL:url serviceType:serviceType name:configurationName prefixName:nil];
}

- ( void )addConfigurationWithURL:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName prefixName:( NSString * ) prefixName
{
    [self addConfiguration:url serviceType:serviceType name:configurationName prefixName:prefixName];
    [self saveConfigurations];
}

- ( BOOL )saveConfigurationAtIndex:( NSInteger )index url:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName
{
    return [self saveConfigurationAtIndex:index url:url serviceType:serviceType name:configurationName prefixName:nil];
}

- ( BOOL )saveConfigurationAtIndex:( NSInteger )index url:( NSString * )url serviceType:( int )serviceType name:( NSString * )configurationName prefixName:( NSString * ) prefixName
{
    if ( index < [_arrConfigrations count] )
    {
        NSDictionary *data = @{ kRPSURL : url,
                                kSERVICE_TYPE : @( serviceType ),
                                kCONFIG_NAME : configurationName,
                                kRPSPrefix : ( prefixName == nil ) ? ( @"" ) : ( prefixName ) };

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
    if ( [_arrConfigrations count] == 0 )
        return nil;
    else
        return [NSMutableDictionary dictionaryWithDictionary:[_arrConfigrations objectAtIndexedSubscript:_intSelectedConfiguration]];
}

- ( NSDictionary * )getConfigurationAtIndex:( NSInteger ) index
{
    if ( index >= [_arrConfigrations count] )
        return nil;
    else
        return [NSMutableDictionary dictionaryWithDictionary:[_arrConfigrations objectAtIndexedSubscript:index]];
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

-( NSInteger ) configurationExists: ( NSDictionary * ) configuration
{
    NSInteger intAtIndex = -1;
    for ( int j = 0; j < [ConfigurationManager sharedManager].configurationsCount; j++ )
    {
        if ( [[[ConfigurationManager sharedManager] getNameAtIndex:j] isEqualToString:[configuration valueForKey:kJSON_NAME]] )
        {
            intAtIndex = j;
            NSLog(@"Configuratoin exists at index: %d", j);
            break;
        }
    }
    return intAtIndex;
}

@end
