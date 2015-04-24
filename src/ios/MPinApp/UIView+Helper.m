//
//  UIView+Helper.m
//  MPinApp
//
//  Created by Georgi Georgiev on 2/10/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UIView+Helper.h"

@implementation UIView (Helper)

- (void) setBottomBorder:(UIColor *) color width: (CGFloat) width alpha: (CGFloat) alpha {
    CALayer *bottomBorder = [CALayer layer];
    bottomBorder.frame = CGRectMake(0.0f, self.frame.size.height - width, self.frame.size.width, width);
    bottomBorder.backgroundColor = [color colorWithAlphaComponent:alpha].CGColor;
    [self.layer addSublayer:bottomBorder];
}

@end
