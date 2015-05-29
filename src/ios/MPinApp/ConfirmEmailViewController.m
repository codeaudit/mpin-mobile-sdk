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
#import "MFSideMenu.h"
#import "ThemeManager.h"

@interface ConfirmEmailViewController ( ) {
    MPin *sdk;
}

- ( void )showPinPad;

@end

@implementation ConfirmEmailViewController

- ( void )viewDidLoad
{
    [super viewDidLoad];
    [[ThemeManager sharedManager] beautifyViewController:self];
    self.automaticallyAdjustsScrollViewInsets = NO;
    [self.menuContainerViewController setPanMode:MFSideMenuPanModeDefault];

    sdk = [[MPin alloc] init];
    sdk.delegate = self;
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    self.lblUserID.text = [self.iuser getIdentity];
    self.title = NSLocalizedString(@"CONFIRM_EMAIL_VC_TITLE", @"");

    self.lblMessage.text = [NSString stringWithFormat:NSLocalizedString(@"CONFIRM_EMAIL_VC_LBL_MESSAGE", @""), [self.iuser getIdentity]];
    [self.btnEmailConfirmed setTitle:NSLocalizedString(@"CONFIRM_EMAIL_VC_BTN_EMAIL_CONFIRMED", @"") forState:UIControlStateNormal];
    [self.btnGoToIdList setTitle:NSLocalizedString(@"CONFIRM_EMAIL_VC_BTN_GOTO_ID_LIST", @"") forState:UIControlStateNormal];
    [self.btnResendEmail setTitle:NSLocalizedString(@"CONFIRM_EMAIL_VC_BTN_RESEND_EMAIL", @"") forState:UIControlStateNormal];
}

- ( void )viewDidAppear:( BOOL )animated
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( showPinPad ) name:kShowPinPadNotification object:nil];
}

- ( void )viewDidDisappear:( BOOL )animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}

- ( void )showPinPad
{
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    PinPadViewController *pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.currentUser = self.iuser;
    pinpadViewController.boolShouldShowBackButton = NO;
    pinpadViewController.title = kSetupPin;
    [self.navigationController pushViewController:pinpadViewController animated:YES];
}

- ( IBAction )backToIDList:( id )sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- ( IBAction )showLeftMenuPressed:( id )sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

- ( IBAction )OnConfirmEmail:( id )sender
{
    [sdk FinishRegistration:self.iuser];
}

- ( void )OnFinishRegistrationCompleted:( id )sender user:( const id<IUser>)user
{
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    IdentityCreatedViewController *vcIDCreated = (IdentityCreatedViewController *)[storyboard instantiateViewControllerWithIdentifier:@"IdentityCreatedViewController"];
    vcIDCreated.strEmail = [user getIdentity];
    vcIDCreated.user = user;
    [self.navigationController pushViewController:vcIDCreated animated:YES];
}

- ( void )OnFinishRegistrationError:( id )sender error:( NSError * )error
{
    MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:[NSString stringWithFormat:@"%@ Please check your e-mail and follow the activation link!", mpinStatus.errorMessage]
     addActivityIndicator:NO
     minShowTime:3];
}

- ( IBAction )OnResendEmail:( id )sender
{
    //TODO: localize this
    [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"Resending email" addActivityIndicator:YES minShowTime:0];
    [sdk RestartRegistration:self.iuser];
}

- ( void )OnRestartRegistrationCompleted:( id )sender user:( const id<IUser>)user
{
    [[ErrorHandler sharedManager] updateMessage:[NSString stringWithFormat:@"%@ Please check your e-mail and follow the activation link!", [user getIdentity]]
     addActivityIndicator:NO hideAfter:3];
}

- ( void )OnRestartRegistrationError:( id )sender error:( NSError * )error
{
    MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:mpinStatus.errorMessage
     addActivityIndicator:NO
     minShowTime:0];
}

@end
