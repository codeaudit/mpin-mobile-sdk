//
//  IdentityBlockedViewController.m
//  MPinApp
//
//  Created by Tihomir Ganev on 21.Apr.15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//
#import "MFSideMenu.h"
#import "IdentityBlockedViewController.h"
#import "IdentityCreatedViewController.h"
#import "ThemeManager.h"
#import "PinPadViewController.h"
#import "ConfirmEmailViewController.h"
#import "ATMHud.h"

@interface IdentityBlockedViewController () {
     MPin* sdk;
    ATMHud* hud;
    UIStoryboard * storyboard;
}

- (void)startLoading;
- (void)stopLoading;
- (void)showPinPad;

- (IBAction)showLeftMenuPressed:(id)sender;
- (IBAction)btnGoToIdListPressed:(id)sender;
- (IBAction)btnDeleteIdPressed:(id)sender;

@end

@implementation IdentityBlockedViewController

- (void)showPinPad
{
    PinPadViewController* pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.userId = [self.iuser getIdentity];
    pinpadViewController.boolShouldShowBackButton = YES;
    pinpadViewController.title = kEnterPin;
    [self.navigationController pushViewController:pinpadViewController animated:YES];
}

- (void)startLoading
{
    [hud showInView:self.view];
}

- (void)stopLoading
{
    [hud hide];
}


- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];

    
    [[ThemeManager sharedManager] beautifyViewController:self];
    
    hud = [[ATMHud alloc] initWithDelegate:self];
    [hud setCaption:@"Changing configuration. Please wait."];
    [hud setActivity:YES];
    [hud showInView:self.view];
    
    sdk = [[MPin alloc] init];
    sdk.delegate = self;
}

-(void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    if (_strUserEmail != nil)
    {
        _lblUserEmail.text = _strUserEmail;
    }
    else
    {
        _lblUserEmail.text = @"";
    }
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showPinPad) name:kShowPinPadNotification object:nil];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)showLeftMenuPressed:(id)sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

- (IBAction)btnGoToIdListPressed:(id)sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- (IBAction)btnDeleteIdPressed:(id)sender
{
   [[[UIAlertView alloc] initWithTitle:@"REMOVE IDENTITY" message:@"This action will remove the identity permanently.  Are you shure?" delegate:self cancelButtonTitle:@"NO" otherButtonTitles:@"YES", nil] show];
    
}

#pragma mark - Alert view delegate -

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1)
    {
        [MPin DeleteUser:_iuser];
        [self.navigationController popToRootViewControllerAnimated:YES];
    }
}

- (void) OnFinishRegistrationCompleted:(id) sender user:(const id<IUser>) user {
    [self stopLoading];
    IdentityCreatedViewController* vcIDCreated = (IdentityCreatedViewController*)[storyboard instantiateViewControllerWithIdentifier:@"IdentityCreatedViewController"];
    vcIDCreated.user = self.iuser;
    vcIDCreated.strEmail = [user getIdentity];
    [self.navigationController pushViewController:vcIDCreated animated:YES];
}

- (void) OnFinishRegistrationError:(id) sender  error:(NSError *) error {
    [self stopLoading];
    if (error.code == IDENTITY_NOT_VERIFIED) {
        ConfirmEmailViewController* cevc = (ConfirmEmailViewController*)[storyboard instantiateViewControllerWithIdentifier:@"ConfirmEmailViewController"];
        cevc.iuser = (error.userInfo)[kUSER];
        [self.navigationController pushViewController:cevc animated:YES];
    }
    else {
        // TODO:
    }
}

-(IBAction)onResetPinButtonClicked:(id)sender {
    
}

@end
