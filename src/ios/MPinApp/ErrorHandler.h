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

-(void) presentMessageInViewController:(UIViewController *)viewController
                         errorString:(NSString *)strMessage
                addActivityIndicator:(BOOL)addActivityIndicator
                   minShowTime:(NSInteger) seconds;


- (void) updateMessage:(NSString *) strMessage   addActivityIndicator:(BOOL)addActivityIndicator hideAfter:(NSInteger) hideAfter;

-(void) hideMessage;




@end
