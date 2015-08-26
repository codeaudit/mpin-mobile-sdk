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


#import "ThemeManager.h"
#import "AFNetworking.h"
#import "AboutViewController.h"
#import "AddIdentityViewController.h"
#import "AddSettingViewController.h"
#import "AccessNumberViewController.h"
#import "OTPViewController.h"
#import "SettingsViewController.h"
#import "UserListViewController.h"
#import "ConfirmEmailViewController.h"
#import "PinPadViewController.h"
#import "IdentityCreatedViewController.h"
#import "MenuViewController.h"
#import "BackButton.h"
#import "ConfigListTableViewCell.h"
#import "MenuTableViewCell.h"
#import "IdentityBlockedViewController.h"
#import "ANAuthenticationSuccessful.h"
#import "NetworkDownViewController.h"
#import "NetworkMonitor.h"

@interface ThemeManager ( )
{
    BOOL boolReachabilityManagerReady;
}
@end

@implementation ThemeManager

+ ( ThemeManager * )sharedManager
{
    static ThemeManager *sharedManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^ {
        sharedManager = [[self alloc] init];
    });

    return sharedManager;
}

- ( instancetype )init
{
    self = [super init];
    if ( self )
    {
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( reachabilityManagerReady ) name:AFNetworkingReachabilityDidChangeNotification object:nil];
        boolReachabilityManagerReady = NO;
    }

    return self;
}

- ( void ) reachabilityManagerReady
{
    boolReachabilityManagerReady = YES;
}

- ( void )beautifyViewController:( id )vc
{
    SuperViewController *v = (SuperViewController *)vc;

    [self setupErrorViewInViewController:v];

    v.navigationController.navigationBar.barTintColor = [[SettingsManager sharedManager] color0];
    v.navigationController.navigationBar.tintColor = [[SettingsManager sharedManager] color6];
    [v.navigationController.navigationBar setTitleTextAttributes:@{ NSForegroundColorAttributeName : [[SettingsManager sharedManager] color6],
                                                                    NSFontAttributeName : [UIFont fontWithName:@"OpenSans" size:18.0f] }];
    v.navigationController.navigationBar.translucent = NO;

    if ( [vc isMemberOfClass:[ANAuthenticationSuccessful class]] )
    {
        ANAuthenticationSuccessful *myVc = (ANAuthenticationSuccessful *)vc;
        myVc.lblMessage.textColor = [[SettingsManager sharedManager] color7];
        myVc.lblMessage.font = [UIFont fontWithName:@"OpenSans-Semibold" size:22.f];
    }
    else
    if ( [vc isMemberOfClass:[NetworkDownViewController class]] )
    {
        NetworkDownViewController *myVc = (NetworkDownViewController *)vc;
        myVc.lblMessage.font = [UIFont fontWithName:@"OpenSans-Bold" size:20.f];
        myVc.lblMessage.textColor = [[SettingsManager sharedManager] color10];
    }
    else
    if ( [vc isMemberOfClass:[MenuViewController class]] )
    {
        MenuViewController *myVc = (MenuViewController *)vc;

        myVc.tblMenu.backgroundColor = [[SettingsManager sharedManager] color6];
        myVc.view.backgroundColor = [[SettingsManager sharedManager] color6];

        myVc.lblConfigurationName.font = [UIFont fontWithName:@"OpenSans-Bold" size:14.f];
        myVc.lblConfigurationName.textColor = [[SettingsManager sharedManager] color0];
        myVc.lblConfigurationURL.font = [UIFont fontWithName:@"OpenSans" size:12.f];
        myVc.lblConfigurationURL.textColor = [[SettingsManager sharedManager] color0];
    }
    else
    if ( [vc isMemberOfClass:[AccessNumberViewController class]] )
    {
        AccessNumberViewController *myVc = (AccessNumberViewController *)vc;
        myVc.lblEmail.textColor = [[SettingsManager sharedManager] color6];
        myVc.lblNote.textColor = [[SettingsManager sharedManager] color9];
        myVc.lblNote.font = [UIFont fontWithName:@"OpenSans-Semibold" size:12.];
        [self setupLoginButton:myVc.btnLogin];
    }

    else
    if ( [vc isMemberOfClass:[ConfirmEmailViewController class]] )
    {
        ConfirmEmailViewController *myVc = (ConfirmEmailViewController *)vc;

        myVc.btnGoToIdList.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnGoToIdList setTitleColor:[[SettingsManager sharedManager] color7] forState:UIControlStateNormal];

        myVc.btnResendEmail.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnResendEmail setTitleColor:[[SettingsManager sharedManager] color7] forState:UIControlStateNormal];

        [self setupLoginButton:myVc.btnEmailConfirmed];

        myVc.lblUserID.textColor = [[SettingsManager sharedManager] color2];
        myVc.lblUserID.font = [UIFont fontWithName:@"OpenSans-Semibold" size:22.0];
        myVc.lblMessage.textColor = [[SettingsManager sharedManager] color4];
        myVc.lblMessage.font = [UIFont fontWithName:@"OpenSans" size:18.0];

        myVc.viewButtons.backgroundColor = [[SettingsManager sharedManager] color3];
    }

    else
    if ( [vc isMemberOfClass:[IdentityCreatedViewController class]] )
    {
        IdentityCreatedViewController *myVc = (IdentityCreatedViewController *)vc;
        [self setupLoginButton:myVc.btnSignIn];
        myVc.lblMessage.textColor = [[SettingsManager sharedManager] color2];
        myVc.lblEmail.textColor = [[SettingsManager sharedManager] color4];
    }

    else
    if ( [vc isMemberOfClass:[PinPadViewController class]] )
    {
        PinPadViewController *myVc = (PinPadViewController *)vc;
        myVc.title = NSLocalizedString(@"PINPAD_VC_TITLE", @"");
        [self setupLoginButton:myVc.btnLogin];
    }

    else
    if ( [vc isMemberOfClass:[AboutViewController class]] )
    {}

    else
    if ( [vc isMemberOfClass:[AddIdentityViewController class]] )
    {
        AddIdentityViewController *myVc = (AddIdentityViewController *)vc;
        [myVc.txtIdentity setBottomBorder:[[SettingsManager sharedManager] color5] width:2.f alpha:.5f];
        [myVc.txtDevName setBottomBorder:[[SettingsManager sharedManager] color5] width:2.f alpha:.5f];
        [myVc.btnBack setup];
        myVc.lblIdentity.textColor = [[SettingsManager sharedManager] color5];
        myVc.lblIdentity.font = [UIFont fontWithName:@"OpenSans-Semibold" size:12.0];
        myVc.lblDevName.textColor = [[SettingsManager sharedManager] color5];
        myVc.lblDevName.font = [UIFont fontWithName:@"OpenSans-Semibold" size:12.0];
    }

    else
    if ( [vc isMemberOfClass:[AddSettingViewController class]] )
    {
        AddSettingViewController *myVc = (AddSettingViewController *)vc;
        for ( int i = 0; i < [myVc.view.subviews count]; i++ )
        {
            if ( [( myVc.view.subviews ) [i] isMemberOfClass:[UILabel class]] )
            {
                UILabel *l = (UILabel *)( myVc.view.subviews ) [i];
                l.textColor = [[SettingsManager sharedManager] color7];
            }
        }
        [self setupLoginButton:myVc.btnTestConfig];
    }

    else
    if ( [vc isMemberOfClass:[OTPViewController class]] )
    {
        OTPViewController *myVc = (OTPViewController *)vc;

        myVc.lblEmail.textColor = [[SettingsManager sharedManager] color5];
        myVc.lblEmail.font = [UIFont fontWithName:@"OpenSans-Semibold" size:16.0];

        myVc.lblMessage.textColor = [[SettingsManager sharedManager] color2];
        myVc.lblMessage.font = [UIFont fontWithName:@"OpenSans" size:14.0];

        myVc.lblYourPassword.textColor = [[SettingsManager sharedManager] color4];
        myVc.lblYourPassword.font = [UIFont fontWithName:@"OpenSans-Semibold" size:12.0];

        myVc.lblOTP.textColor = [[SettingsManager sharedManager] color5];
        myVc.lblOTP.font = [UIFont fontWithName:@"OpenSans" size:38.0];

        myVc.lblMessage.text = NSLocalizedString(@"OTPVC_LBL_MESSAGE", @"You can now use this password to log in to your RADIUS service");
    }

    else
    if ( [vc isMemberOfClass:[IdentityBlockedViewController class]] )
    {
        IdentityBlockedViewController *myVc = (IdentityBlockedViewController *)vc;

        myVc.title = NSLocalizedString(@"BLOCKED_ID_TITLE", @"Identity Blocked");
        myVc.lblUserEmail.textColor = [[SettingsManager sharedManager] color5];
        myVc.lblUserEmail.font = [UIFont fontWithName:@"OpenSans-Semibold" size:16.0];
        myVc.lblUserEmail.backgroundColor = [UIColor clearColor];

        myVc.lblMessage.textColor = [[SettingsManager sharedManager] color2];
        myVc.lblMessage.font = [UIFont fontWithName:@"OpenSans" size:14.0];
        myVc.lblMessage.numberOfLines = 0;
        myVc.lblMessage.text = NSLocalizedString(@"BLOCKED_ID_MESSAGE",@"Wrong PIN entered too many times. Your identity has been blocked. You can either select a different identity, reset your PIN or remove your blocked identity.");
        myVc.lblMessage.backgroundColor = [UIColor clearColor];

        [myVc.imgViewBlockedId setImage:[UIImage imageNamed:@"identity-blocked"]];

        [myVc.btnResetPIN setTitleColor:[[SettingsManager sharedManager] color10] forState:UIControlStateNormal];
        myVc.btnResetPIN.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnResetPIN setTitle:NSLocalizedString(@"BLOCKED_ID_RESET_PIN", @"RESET PIN") forState:UIControlStateNormal];

        [myVc.btnBackToIdList setTitleColor:[[SettingsManager sharedManager] color10] forState:UIControlStateNormal];
        myVc.btnBackToIdList.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnBackToIdList setTitle:NSLocalizedString(@"BLOCKED_ID_BACK_TO_ID_LIST", @"BACK TO IDENTITY LIST")  forState:UIControlStateNormal];

        [myVc.btnDeleteId setTitleColor:[[SettingsManager sharedManager] color1] forState:UIControlStateNormal];
        myVc.btnDeleteId.backgroundColor = [[SettingsManager sharedManager] color10];
        [myVc.btnDeleteId setTitle:NSLocalizedString(@"BLOCKED_ID_REMOVE_ID", @"REMOVE THIS IDENTITY")  forState:UIControlStateNormal];

        myVc.viewButtonsBG.backgroundColor = [[SettingsManager sharedManager] color3];
    }

    else
    if ( [vc isMemberOfClass:[SettingsViewController class]] )
    {
        SettingsViewController *myVc = (SettingsViewController *)vc;

        //[myVc.btnAddConfiguration setTitle:NSLocalizedString(@"KEY_ADD", @"")];

        [myVc.btnDeleteConfiguration setTitle:NSLocalizedString(@"KEY_DELETE", @"") forState:UIControlStateNormal];
        myVc.btnDeleteConfiguration.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnDeleteConfiguration setTitleColor:[[SettingsManager sharedManager] color8] forState:UIControlStateNormal];


        [myVc.btnEditConfiguration setTitle:NSLocalizedString(@"KEY_EDIT", @"") forState:UIControlStateNormal];
        myVc.btnEditConfiguration.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnEditConfiguration setTitleColor:[[SettingsManager sharedManager] color7] forState:UIControlStateNormal];

        myVc.viewButtons.backgroundColor = [[SettingsManager sharedManager] color3];
        [self setupLoginButton:myVc.btnSignIn];
        myVc.title = NSLocalizedString(@"CONFIGLISTVC_TITLE", @"");
    }

    else
    if ( [vc isMemberOfClass:[UserListViewController class]] )
    {
        UserListViewController *myVc = (UserListViewController *)vc;

        myVc.navigationController.navigationBar.barTintColor = [[SettingsManager sharedManager] color0];
        myVc.navigationController.navigationBar.tintColor = [[SettingsManager sharedManager] color6];
        [myVc.navigationController.navigationBar setTitleTextAttributes:@{ NSForegroundColorAttributeName : [[SettingsManager sharedManager] color6],
                                                                           NSFontAttributeName : [UIFont fontWithName:@"OpenSans" size:18.0f] }];
        v.navigationController.navigationBar.translucent = NO;


        myVc.btnDelete.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnDelete setTitleColor:[[SettingsManager sharedManager] color8] forState:UIControlStateNormal];

        [myVc.btnReset setTitle:NSLocalizedString(@"KEY_RESET", @"") forState:UIControlStateNormal];
        myVc.btnReset.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnReset setTitleColor:[[SettingsManager sharedManager] color7] forState:UIControlStateNormal];

        [self setupLoginButton:myVc.btnAuthenticate];
        myVc.viewButtonsContainer.backgroundColor = [[SettingsManager sharedManager] color3];

        myVc.view.backgroundColor = [[SettingsManager sharedManager] color3];
        myVc.title = NSLocalizedString(@"USERLISTVC_TITLE", @"");
        [myVc.btnDelete setTitle:NSLocalizedString(@"KEY_DELETE", @"") forState:UIControlStateNormal];
    }

    else
    {
        for ( int i = 0; i < [( (UIViewController *)vc ).view.subviews count]; i++ )
        {
            if ( [( ( (UIViewController *)vc ).view.subviews ) [i] isMemberOfClass:[UILabel class]] )
            {
                UILabel *l = ( ( (UIViewController *)vc ).view.subviews ) [i];
                l.font = [UIFont fontWithName:@"OpenSans" size:22.0];
            }
        }
    }
}

- ( void )setupLoginButton:( UIButton * )button
{
    [button setTitleColor:[[SettingsManager sharedManager] color1] forState:UIControlStateNormal];
    button.backgroundColor = [[SettingsManager sharedManager] color10];
}

- ( void )setupRegularButton:( UIButton * )button
{
    [button setTitleColor:[[SettingsManager sharedManager] color1] forState:UIControlStateNormal];
    button.backgroundColor = [[SettingsManager sharedManager] color10];
}

- ( void )setupDeleteButton:( UIButton * )button
{
    [button setTitleColor:[[SettingsManager sharedManager] color1] forState:UIControlStateNormal];
    button.backgroundColor = [[SettingsManager sharedManager] color10];

    button.titleLabel.font = [UIFont fontWithName:@"OpenSans" size:18.0];
}

- ( void )customiseMenuCell:( MenuTableViewCell * )cell
{
    cell.accessoryType = UITableViewCellAccessoryNone;
    cell.backgroundColor = [[SettingsManager sharedManager] color6];
    cell.lblMenuID.font = [UIFont fontWithName:@"OpenSans" size:14.f];
    cell.lblMenuID.textColor = [[SettingsManager sharedManager] color0];
    cell.viewSeparator.backgroundColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"border-line"]];
    cell.selectedBackgroundView.backgroundColor = [[SettingsManager sharedManager] color5];
}

- ( void )customiseConfigurationListCell:( ConfigListTableViewCell * )cell
{
    cell.accessoryType = UITableViewCellAccessoryNone;
    cell.backgroundColor = [[SettingsManager sharedManager] color0];

    cell.lblConfigurationName.font = [UIFont fontWithName:@"OpenSans" size:16.f];
    cell.lblConfigurationName.textColor = [[SettingsManager sharedManager] color6];

    cell.lblConfigurationType.font = [UIFont fontWithName:@"OpenSans" size:14.f];
    cell.lblConfigurationType.textColor = [[SettingsManager sharedManager] color4];
}

- ( void ) setupErrorViewInViewController: ( SuperViewController * ) superVc
{
    if ( [superVc isMemberOfClass:[MenuViewController class]]
         || [superVc isMemberOfClass:[ANAuthenticationSuccessful class]]
         || [superVc isMemberOfClass:[NetworkDownViewController class]] )
    {
        return;
    }

    if ( superVc.viewNoNetwork )
    {
        superVc.viewNoNetwork.backgroundColor = [[SettingsManager sharedManager] color11];
        superVc.lblNetworkDownMessage.textColor = [[SettingsManager sharedManager] color7];
        superVc.lblNetworkDownMessage.text = NSLocalizedString(@"CONNECTION_WAS_LOST", @"Connection was lost");
        superVc.lblNetworkDownMessage.font = [UIFont fontWithName:@"OpenSans" size:14.f];
        
        if ( boolReachabilityManagerReady == NO )
        {
            superVc.constraintNoNetworkViewHeight.constant = 0;
        }
        else
        if ( [AFNetworkReachabilityManager sharedManager].reachable )
        {
            superVc.constraintNoNetworkViewHeight.constant = 0;
        }
        else
        {
            superVc.constraintNoNetworkViewHeight.constant = 36.f;
        }
    }
}

- ( void ) showNetworkDown:( SuperViewController * )vc
{
    if ( vc.constraintNoNetworkViewHeight.constant > 0 )
    {
        return;
    }

    [vc.view layoutIfNeeded];
    [UIView animateWithDuration:kFltNoNetworkMessageAnimationDuration animations: ^ {
        vc.constraintNoNetworkViewHeight.constant = 36.0f;
        [vc.view layoutIfNeeded];
    }];
}

- ( void ) hideNetworkDown:( SuperViewController * )vc
{
    if ( vc.constraintNoNetworkViewHeight.constant == 0 )
    {
        return;
    }

    [vc.view layoutIfNeeded];
    [UIView animateWithDuration:kFltNoNetworkMessageAnimationDuration animations: ^ {
        vc.constraintNoNetworkViewHeight.constant = 0.0f;
        [vc.view layoutIfNeeded];
    }];
}

@end