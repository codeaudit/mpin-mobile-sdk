//
//  IdentityBlockedViewController.h
//  MPinApp
//
//  Created by Tihomir Ganev on 21.Apr.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "IUser.h"
#import "MPin+AsyncOperations.h"

@interface IdentityBlockedViewController : UIViewController <MPinSDKDelegate>


@property (nonatomic, strong) NSString *strUserEmail;
@property (nonatomic, strong) id<IUser> iuser;
@property (nonatomic, weak) IBOutlet UILabel            *lblUserEmail;
@property (nonatomic, weak) IBOutlet UILabel            *lblMessage;
@property (nonatomic, weak) IBOutlet UIImageView        *imgViewBlockedId;
@property (nonatomic, weak) IBOutlet UIButton           *btnBackToIdList;
@property (nonatomic, weak) IBOutlet UIButton           *btnResetPIN;
@property (nonatomic, weak) IBOutlet UIButton           *btnDeleteId;
@property (nonatomic, weak) IBOutlet UIBarButtonItem    *barBtnMenu;
@property (nonatomic, weak) IBOutlet UIView             *viewButtonsBG;

-(IBAction)onResetPinButtonClicked:(id)sender;

@end
