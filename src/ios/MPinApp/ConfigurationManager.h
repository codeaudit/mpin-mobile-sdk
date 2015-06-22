//
//  ConfigurationManager.h
//  MPinApp
//
//  Created by Tihomir Ganev on 17.Mar.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>

#define NOT_SELECTED -1

@interface ConfigurationManager : NSObject

+ (ConfigurationManager*)sharedManager;

@property (nonatomic) NSInteger intSelectedConfiguration;

- (void)addConfigurationWithURL:(NSString*)url serviceType:(int)serviceType name:(NSString*)configurationName;
- (void)addConfigurationWithURL:(NSString*)url serviceType:(int)serviceType name:(NSString*)configurationName prefixName:(NSString *) prefixName;
- (BOOL)saveConfigurationAtIndex:(NSInteger)index url:(NSString*)url serviceType:(int)serviceType name:(NSString*)configurationName;
- (BOOL)saveConfigurationAtIndex:(NSInteger)index url:(NSString*)url serviceType:(int)serviceType name:(NSString*)configurationName prefixName:(NSString *) prefixName;
- (BOOL)deleteConfigurationAtIndex:(NSInteger)index;
- (BOOL)setSelectedConfiguration:(NSInteger)index;
- (BOOL)setSelectedUserForCurrentConfiguration:(NSInteger)userIndex;
- (BOOL)isEmpty;
- (NSInteger) defaultConfigCount;

@property (NS_NONATOMIC_IOSONLY, getter=getSelectedConfigurationIndex, readonly) NSInteger selectedConfigurationIndex;
@property (NS_NONATOMIC_IOSONLY, getter=getConfigurationsCount, readonly) NSInteger configurationsCount;
- (NSString*)getURLAtIndex:(NSInteger)index;
- (NSString*)getNameAtIndex:(NSInteger)index;
- (NSString*)getPrefixAtIndex:(NSInteger)index;
- (BOOL)getIsDeviceName:(NSInteger)index;
- (NSInteger)getConfigurationTypeAtIndex:(NSInteger)index;
- (NSDictionary*)getSelectedConfiguration;
- (NSDictionary*)getConfigurationAtIndex:(NSInteger) index;
@property (NS_NONATOMIC_IOSONLY, getter=getSelectedUserIndexforSelectedConfiguration, readonly) NSInteger selectedUserIndexforSelectedConfiguration;

-(NSString *) getDeviceName;
-(void) setDeviceName:(NSString *) devName;

@end
