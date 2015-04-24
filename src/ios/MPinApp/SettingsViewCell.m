//
//  SettingsViewCell.m
//  MPinApp
//
//  Created by Georgi Georgiev on 1/20/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "SettingsViewCell.h"
#import "Constants.h"

@interface SettingsViewCell ()

@property (atomic, assign) NSInteger cellIndex;
@property (nonatomic, assign) NSMutableArray* settings;
- (void)resetData;
@end

@implementation SettingsViewCell

- (void)resetData
{
    NSDictionary* data = @{ kRPSURL : self.url.text,
        kSERVICE_TYPE : @([self.otp isOn]),
        kIS_DN : @([self.devName isOn]) };
    (self.settings)[self.cellIndex] = data;
}

- (void)invalidate:(NSMutableArray*)settings atIndex:(NSInteger)index
{
    self.settings = settings;
    self.cellIndex = index;
    NSDictionary* data = settings[index];
    self.url.text = data[kRPSURL];
    [self.devName setOn:[data[kIS_DN] boolValue]];
}

- (IBAction)onOTPValueChanged:(id)sender
{
    if ([self.otp isOn] && [self.an isOn])
        [self.an setOn:NO];
    [self resetData];
}
- (IBAction)onAccessNumberValueChanged:(id)sender
{
    if ([self.an isOn] && [self.otp isOn])
        [self.otp setOn:NO];
    [self resetData];
}

- (IBAction)onDeviceNameValueChanged:(id)sender
{
    [self resetData];
}

@end
