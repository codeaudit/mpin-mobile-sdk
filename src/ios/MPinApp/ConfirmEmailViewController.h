//
//  ConfirmEmailViewController.h
//  MPinApp
//
//  Created by Tihomir Ganev on 12.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import "MPin+AsyncOperations.h"


@interface ConfirmEmailViewController : UIViewController <MPinSDKDelegate>

@property(nonatomic, retain) IBOutlet UILabel *lblUserID;
@property(nonatomic, retain) IBOutlet UILabel *lblMessage;
@property(nonatomic, retain) IBOutlet UIButton *btnEmailConfirmed;
@property(nonatomic, retain) IBOutlet UIButton *btnResendEmail;
@property(nonatomic, retain) IBOutlet UIButton *btnGoToIdList;
@property(nonatomic, retain) IBOutlet UIView    *viewButtons;

@property(nonatomic, retain) id<IUser> iuser;

-(IBAction)OnConfirmEmail:(id)sender;
-(IBAction)OnResendEmail:(id)sender;
-(IBAction)backToIDList:(id)sender;
- (IBAction)showLeftMenuPressed:(id)sender;
@end
