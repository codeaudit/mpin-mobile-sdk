//
//  NetworkDownViewController.h
//  MPinApp
//
//  Created by Tihomir Ganev on 6.Aug.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SuperViewController.h"

@interface NetworkDownViewController : SuperViewController <NoNetworkNotificationProtocol>

@property ( nonatomic, weak ) IBOutlet UILabel *lblMessage;

@end
