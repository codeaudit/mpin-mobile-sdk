//
//  SuperViewController.h
//  MPinApp
//
//  Created by Tihomir Ganev on 17.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface SuperViewController : UIViewController
{
    BOOL boolNetworkWasDown;
}

@property( nonatomic,weak ) IBOutlet UIView *viewNoNetwork;
@property( nonatomic,weak ) IBOutlet NSLayoutConstraint *constraintNoNetworkViewHeight;

@end
