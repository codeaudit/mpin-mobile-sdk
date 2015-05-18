//
//  ErrorHandler.h
//  MPinApp
//
//  Created by Tihomir Ganev on 15.May.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "ATMHud.h"

@interface CVXATMHud : ATMHud

@end

@interface ErrorHandler : NSObject

+ (ErrorHandler*)sharedManager;

-(void) presentErrorInViewController:(UIViewController *)viewController
                         errorString:(NSString *)strError
                addActivityIndicator:(BOOL)addActivityIndicator
                   autoHideInSeconds:(NSInteger) seconds;

-(void) startLoadingInController:(UIViewController *)viewController message:(NSString *)message;
-(void) stopLoading;

-(void) hideError;




@end
