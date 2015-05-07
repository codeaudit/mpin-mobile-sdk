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
#import "ConfigurationManager.h"
#import "UIViewController+Helper.h"

#define RESETPIN_TAG 208
#define RESETPIN_BUTTON_INDEX 1

@interface IdentityBlockedViewController () {
     MPin* sdk;
    ATMHud* hud;
    UIStoryboard * storyboard;
}

- (void)startLoading;
- (void)stopLoading;
- (void)showPinPad;
- (void) deleteUser;

- (IBAction)showLeftMenuPressed:(id)sender;
- (IBAction)btnGoToIdListPressed:(id)sender;
- (IBAction)btnDeleteIdPressed:(id)sender;

@end

@implementation IdentityBlockedViewController

- (void)startLoading
{
    [hud showInView:self.view];
}

- (void)stopLoading
{
    [hud hide];
}

- (void) deleteUser {
    [MPin DeleteUser:_iuser];
    [[ConfigurationManager sharedManager] setSelectedUserForCurrentConfiguration:NOT_SELECTED];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];

    
    [[ThemeManager sharedManager] beautifyViewController:self];
    
    hud = [[ATMHud alloc] initWithDelegate:self];
    [hud setCaption:@"Please wait ... "];
    [hud setActivity:YES];
    
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
    if((alertView.tag == RESETPIN_TAG) && (buttonIndex == RESETPIN_BUTTON_INDEX)) {
        [self startLoading];
        NSString * userID = [self.iuser getIdentity];
        [self deleteUser];
        ConfigurationManager *cfm = [ConfigurationManager sharedManager];
        [sdk RegisterNewUser:userID devName:[cfm getDeviceName]];
    } else {
        if (buttonIndex == 1)
        {
            [self deleteUser];
            [self.navigationController popToRootViewControllerAnimated:YES];
        }
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
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"RESET_PIN"
                                                    message:[NSString stringWithFormat:@"Are you sure that you would like to reset pin of \"%@\" ?", [self.iuser getIdentity]]
                                                   delegate:self
                                          cancelButtonTitle:@"CANCEL"
                                          otherButtonTitles:@"RESET",
                          nil];
    alert.tag = RESETPIN_TAG;
    [alert show];
}

- (void)OnRegisterNewUserCompleted:(id)sender user:(const id<IUser>)user {
    [self stopLoading];
    switch ([user getState]) {
        case STARTED_REGISTRATION: {
            ConfirmEmailViewController* cevc = (ConfirmEmailViewController*)
            [storyboard instantiateViewControllerWithIdentifier:
             @"ConfirmEmailViewController"];
            cevc.iuser = user;
            [self.navigationController pushViewController:cevc animated:YES];
        } break;
        case ACTIVATED:
            [self startLoading];
            self.iuser = user;
            [sdk FinishRegistration:user];
            break;
        default:
            [self
             showError:[user getIdentity]
             desc:[NSString stringWithFormat:@"User state is unexpected %ld",
                   [user getState]]];
            break;
    }
    
    NSArray * users = [MPin listUsers];
    int index = 0;
    for (;index<[users count];index++) {
        id<IUser> cUser = [users objectAtIndex:index];
        if ([[cUser getIdentity] isEqualToString:[user getIdentity]])   break;
    }
    
    ConfigurationManager* cf = [ConfigurationManager sharedManager];
    [cf setSelectedUserForCurrentConfiguration:(index)];
}

- (void)OnRegisterNewUserError:(id)sender error:(NSError*)error {
    [self stopLoading];
    MpinStatus* mpinStatus = [error.userInfo objectForKey:kMPinSatus];
    [self showError:[mpinStatus getStatusCodeAsString]
               desc:mpinStatus.errorMessage];
}

- (void)showPinPad
{
    PinPadViewController* pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.userId = [self.iuser getIdentity];
    pinpadViewController.boolShouldShowBackButton = YES;
    pinpadViewController.title = kEnterPin;
    [self.navigationController pushViewController:pinpadViewController animated:YES];
}



@end
