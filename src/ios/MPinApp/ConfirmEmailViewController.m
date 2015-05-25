//
//  ConfirmEmailViewController.m
//  MPinApp
//
//  Created by Tihomir Ganev on 12.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import "ConfirmEmailViewController.h"
#import "PinPadViewController.h"
#import "UIViewController+Helper.h"
#import "IdentityCreatedViewController.h"
#import "ATMHud.h"
#import "MFSideMenu.h"
#import "ThemeManager.h"

@interface ConfirmEmailViewController () {
    ATMHud* hud;
    MPin* sdk;
}

- (void)startLoading;
- (void)stopLoading;
- (void)showPinPad;

@end

@implementation ConfirmEmailViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    [[ThemeManager sharedManager] beautifyViewController:self];
    self.automaticallyAdjustsScrollViewInsets = NO;
    [self.menuContainerViewController setPanMode:MFSideMenuPanModeDefault];

    hud = [[ATMHud alloc] initWithDelegate:self];
    [hud setActivity:YES];

    sdk = [[MPin alloc] init];
    sdk.delegate = self;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    self.lblUserID.text = [self.iuser getIdentity];
    self.lblMessage.text = [NSString stringWithFormat:@"We have sent you an email to: \r\n %@ \r\n Click the link in the email to confirm your identity and proceed.", [self.iuser getIdentity]];
}

- (void)viewDidAppear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showPinPad) name:kShowPinPadNotification object:nil];
}
- (void)viewDidDisappear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}

- (void)startLoading
{
    [hud showInView:self.view];
}
- (void)stopLoading
{
    [hud hide];
}

- (void)showPinPad
{
    UIStoryboard* storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    PinPadViewController* pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.userId = [self.iuser getIdentity];
    pinpadViewController.boolShouldShowBackButton = NO;
    pinpadViewController.title = kSetupPin;
    [self.navigationController pushViewController:pinpadViewController animated:NO];
}

- (IBAction)backToIDList:(id)sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- (IBAction)showLeftMenuPressed:(id)sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

- (IBAction)OnConfirmEmail:(id)sender
{
    [self startLoading];
    [sdk FinishRegistration:self.iuser];
}

- (void)OnFinishRegistrationCompleted:(id)sender user:(const id<IUser>)user
{
    [self stopLoading];

    UIStoryboard* storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    IdentityCreatedViewController* vcIDCreated = (IdentityCreatedViewController*)[storyboard instantiateViewControllerWithIdentifier:@"IdentityCreatedViewController"];
    vcIDCreated.strEmail = [user getIdentity];
    vcIDCreated.user = user;
    [self stopLoading];
    [self.navigationController pushViewController:vcIDCreated animated:YES];
}

- (void)OnFinishRegistrationError:(id)sender error:(NSError*)error
{
    [self stopLoading];

    MpinStatus* mpinStatus = (error.userInfo)[kMPinSatus];
    [self showError:[mpinStatus getStatusCodeAsString] desc:[NSString stringWithFormat:@"%@ Please check your e-mail and follow the activation link!", mpinStatus.errorMessage]];
}

- (IBAction)OnResendEmail:(id)sender
{
    [hud setTitle:@"Sending Email ..."];
    [self startLoading];

    [sdk RestartRegistration:self.iuser];
}

- (void)OnRestartRegistrationCompleted:(id)sender user:(const id<IUser>)user
{
    [self stopLoading];
    [hud setTitle:@""];
}

- (void)OnRestartRegistrationError:(id)sender error:(NSError*)error
{
    [self stopLoading];

    MpinStatus* mpinStatus = (error.userInfo)[kMPinSatus];
    [self showError:[mpinStatus getStatusCodeAsString] desc:mpinStatus.errorMessage];
}

@end
