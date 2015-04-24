//
//  ConfigurationManager.h
//  MPinApp
//
//  Created by Tihomir Ganev on 17.Mar.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ConfigurationManager : NSObject

+ (ConfigurationManager*)sharedManager;

@property (nonatomic) NSInteger intSelectedConfiguration;
- (void)addConfigurationWithURL:(NSString*)url serviceType:(int)serviceType name:(NSString*)configurationName;
- (BOOL)saveConfigurationAtIndex:(NSInteger)index url:(NSString*)url serviceType:(int)serviceType name:(NSString*)configurationName;
- (BOOL)deleteConfigurationAtIndex:(NSInteger)index;
- (BOOL)setSelectedConfiguration:(NSInteger)index;
- (BOOL)setSelectedUserForCurrentConfiguration:(NSInteger)userIndex;

@property (NS_NONATOMIC_IOSONLY, getter=getSelectedConfigurationIndex, readonly) NSInteger selectedConfigurationIndex;
@property (NS_NONATOMIC_IOSONLY, getter=getConfigurationsCount, readonly) NSInteger configurationsCount;
- (NSString*)getURLAtIndex:(NSInteger)index;
- (NSString*)getNameAtIndex:(NSInteger)index;
- (NSString*)getPrefixAtIndex:(NSInteger)index;
- (BOOL)getIsDeviceName:(NSInteger)index;
- (NSInteger)getConfigurationTypeAtIndex:(NSInteger)index;
- (NSDictionary*)getSelectedConfiguration;
@property (NS_NONATOMIC_IOSONLY, getter=getSelectedUserIndexforSelectedConfiguration, readonly) NSInteger selectedUserIndexforSelectedConfiguration;

-(NSString *) getDeviceName;
-(void) setDeviceName:(NSString *) devName;

@end