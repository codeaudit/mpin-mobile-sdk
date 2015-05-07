//
//  LocalizationManager.h
//  MPinApp
//
//  Created by Tihomir Ganev on 29.Apr.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LocalizationManager : NSObject

+ (LocalizationManager*)sharedManager;
- (NSString *)localizedString:(NSString *)key comment:(NSString *)comment;

@end
