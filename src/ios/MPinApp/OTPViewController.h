//
//  OTPViewController.h
//  MPinApp
//
//  Created by Georgi Georgiev on 12/19/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

@class OTP;

@interface OTPViewController : UIViewController
@property (nonatomic, weak) IBOutlet UILabel* lblOTP;
@property (nonatomic, weak) IBOutlet UILabel* lblEmail;
@property (nonatomic, weak) IBOutlet UILabel* lblMessage;
@property (nonatomic, weak) IBOutlet UILabel* lblYourPassword;

@property (nonatomic, strong) NSString* strEmail;
@property (nonatomic, retain, readwrite) OTP* otpData;

- (IBAction)OnClickNavButton:(id)sender;
@end
