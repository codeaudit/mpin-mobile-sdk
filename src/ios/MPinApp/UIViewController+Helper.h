//
//  UIViewController+Helper.h
//  MPinApp
//
//  Created by Georgi Georgiev on 2/11/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

@interface UIViewController (Helper)
- (void) invalidateNavBar;
- (void) showError:(NSString *) title desc:(NSString *) desc;
- (void)startLoading;
- (void)stopLoading;


@end
