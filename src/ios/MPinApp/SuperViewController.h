//
//  SuperViewController.h
//  MPinApp
//
//  Created by Tihomir Ganev on 17.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol NoNetworkNotificationProtocol <NSObject>

@required
- ( void ) networkUp;
- ( void ) networkDown;
- ( void ) unRegisterObservers;
- ( void ) registerObservers;


@end

@interface SuperViewController : UIViewController
{}

@property( nonatomic,weak ) IBOutlet UIView *viewNoNetwork;
@property( nonatomic,weak ) IBOutlet NSLayoutConstraint *constraintNoNetworkViewHeight;
@property( nonatomic,weak ) IBOutlet UILabel *lblNetworkDownMessage;
@property( nonatomic,weak ) IBOutlet UIImageView *imgViewNetworkDown;

@end
