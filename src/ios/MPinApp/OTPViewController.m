//
//  OTPViewController.m
//  MPinApp
//
//  Created by Georgi Georgiev on 12/19/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//
#import "OTP.h"
#import "OTPViewController.h"
#import "CycleProgressBar.h"
#import "ThemeManager.h"
#import "BackButton.h"

@interface OTPViewController ()
{
    ThemeManager *themeManager;
    BackButton   *btnBack;
}

- (IBAction) back:(UIBarButtonItem *)sender;
- (void) onProgressBarFinish:(id)sender;

@property (nonatomic, weak) UIView *viewPreloaderContainer;
@property (nonatomic, strong)	CycleProgressBar * cpb;

@end

@implementation OTPViewController
- (void)viewDidLoad
{
    [super viewDidLoad];
    [[ThemeManager sharedManager] beautifyViewController:self];

    
    self.title = @"One time password";
    NSMutableString *strOtp = [NSMutableString stringWithString:@""];
    for (int i = 0; i < [self.otpData.otp length] ; i++)
    {
        [strOtp appendString:[self.otpData.otp substringWithRange:NSMakeRange(i, 1)]];
        [strOtp appendString:@" "];
    }
    self.lblOTP.text = strOtp;
    CGRect screenRect = [[UIScreen mainScreen] bounds];
     self.cpb = [[CycleProgressBar alloc] initWithFrame:CGRectMake(screenRect.size.width/2 - 90.0, (2*screenRect.size.height)/3 - 90.0, 180.0, 180.0)];
    [self.cpb addTarget:self action:@selector(onProgressBarFinish:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:self.cpb];
    btnBack = [[BackButton alloc] initWithTitle:@"" style:UIBarButtonItemStylePlain target:self action:@selector(back:)];
}
- (void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.cpb startAnimation:self.otpData.ttlSeconds];

    _lblEmail.text = _strEmail;
    
    [btnBack setup];
    
    self.navigationItem.leftBarButtonItem = btnBack;
    self.title = NSLocalizedString(@"OTPVC_TITLE", @"");
    self.lblMessage.text = NSLocalizedString(@"OTPVC_LBL_MESSAGE", @"");
    self.lblYourPassword.text = NSLocalizedString(@"OTPVC_LBL_YOUR_PASSWORD", @"");
        
}

- (void) viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    [self.cpb stopAnimation];
}
- (void) onProgressBarFinish:(id)sender {   [self.navigationController popViewControllerAnimated:NO];   }

-(IBAction)OnClickNavButton:(id)sender {      [self.navigationController popToRootViewControllerAnimated:YES];  }

- (IBAction) back:(UIBarButtonItem *)sender
{
    
    [self.navigationController popViewControllerAnimated:YES];
    
}
@end
