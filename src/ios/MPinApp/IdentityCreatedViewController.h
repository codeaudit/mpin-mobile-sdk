//
//  IdentityCreatedViewController.h
//  MPinApp
//
//  Created by Tihomir Ganev on 27.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import "MPin+AsyncOperations.h"
#import "AccessNumberViewController.h"


@interface IdentityCreatedViewController : UIViewController <MPinSDKDelegate, AccessNumberDelegate, UIAlertViewDelegate>

@property(nonatomic,strong) NSString *strEmail;
@property(nonatomic,strong) id<IUser> user;
@property(nonatomic,weak) IBOutlet UILabel *lblEmail;
@property(nonatomic,weak) IBOutlet UILabel *lblMessage;
@property(nonatomic,weak) IBOutlet UIButton *btnSignIn;

- (IBAction)showLeftMenuPressed:(id)sender;

@end
