//
//  SettingsManager.h
//  MPinApp
//
//  Created by Tihomir Ganev on 10.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SettingsManager : NSObject

+ (SettingsManager*)sharedManager;

@property (nonatomic, strong) UIColor* color0;
@property (nonatomic, strong) UIColor* color1;
@property (nonatomic, strong) UIColor* color2;
@property (nonatomic, strong) UIColor* color3;
@property (nonatomic, strong) UIColor* color4;
@property (nonatomic, strong) UIColor* color5;
@property (nonatomic, strong) UIColor* color6;
@property (nonatomic, strong) UIColor* color7;
@property (nonatomic, strong) UIColor* color8;
@property (nonatomic, strong) UIColor* color9;
@property (nonatomic, strong) UIColor* color10;

@end
