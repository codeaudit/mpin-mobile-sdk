//
//  ThemeManager.h
//  MPinApp
//
//  Created by Tihomir Ganev on 19.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>

@class ConfigListTableViewCell;
@class MenuTableViewCell;
@class ATMHud;

@interface ThemeManager : NSObject

+ (ThemeManager*)sharedManager;
- (void)beautifyViewController:(id)vc;
- (void)customiseMenuCell:(MenuTableViewCell*)cell;
- (void)customiseConfigurationListCell:(ConfigListTableViewCell*)cell;

@end
