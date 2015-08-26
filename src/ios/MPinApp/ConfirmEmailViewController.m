/*
 Copyright (c) 2012-2015, Certivox
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 For full details regarding our CertiVox terms of service please refer to
 the following links:
 * Our Terms and Conditions -
 http://www.certivox.com/about-certivox/terms-and-conditions/
 * Our Security and Privacy -
 http://www.certivox.com/about-certivox/security-privacy/
 * Our Statement of Position and Our Promise on Software Patents -
 http://www.certivox.com/about-certivox/patents/
 */

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
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];

    sdk = [[MPin alloc] init];
    sdk.delegate = self;

    self.lblUserID.text = [self.iuser getIdentity];
    self.title = NSLocalizedString(@"CONFIRM_EMAIL_VC_TITLE", @"");

    self.lblMessage.text = [NSString stringWithFormat:NSLocalizedString(@"CONFIRM_EMAIL_VC_LBL_MESSAGE", @""), [self.iuser getIdentity]];
    [self.btnEmailConfirmed setTitle:NSLocalizedString(@"CONFIRM_EMAIL_VC_BTN_EMAIL_CONFIRMED", @"") forState:UIControlStateNormal];
    [self.btnGoToIdList setTitle:NSLocalizedString(@"CONFIRM_EMAIL_VC_BTN_GOTO_ID_LIST", @"") forState:UIControlStateNormal];
    [self.btnResendEmail setTitle:NSLocalizedString(@"CONFIRM_EMAIL_VC_BTN_RESEND_EMAIL", @"") forState:UIControlStateNormal];
}

- ( void )viewDidAppear:( BOOL )animated
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( showPinPad: ) name:kShowPinPadNotification object:nil];
}

- ( void )viewDidDisappear:( BOOL )animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}

- ( void )showPinPad:(NSNotification *)notification
{
    [[ErrorHandler sharedManager] hideMessage];
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    PinPadViewController *pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.currentUser = self.iuser;
    pinpadViewController.boolShouldShowBackButton = NO;
    pinpadViewController.title = kSetupPin;
    pinpadViewController.boolSetupPin = YES;
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
    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:@""
     addActivityIndicator:YES
     minShowTime:0];
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
    [[ErrorHandler sharedManager] updateMessage:[NSString stringWithFormat:NSLocalizedString(@"CONFIRM_EMAIL_ACTIVATE", @"%@ Please check your e-mail and follow the activation link!" ), mpinStatus.errorMessage]
     addActivityIndicator:NO hideAfter:3];
}

- ( IBAction )OnResendEmail:( id )sender
{
    //TODO: localize this   
    [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:NSLocalizedString(@"CONFIRM_EMAIL_RESEND", @"Resending email") addActivityIndicator:YES minShowTime:0];
    [sdk RestartRegistration:self.iuser];
}

- ( void )OnRestartRegistrationCompleted:( id )sender user:( const id<IUser>)user
{
    [[ErrorHandler sharedManager] updateMessage:[NSString stringWithFormat:NSLocalizedString(@"CONFIRM_EMAIL_ACTIVATE", @"%@ Please check your e-mail and follow the activation link!" ), [user getIdentity]]
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
