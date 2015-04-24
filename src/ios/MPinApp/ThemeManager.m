//
//  ThemeManager.m
//  MPinApp
//
//  Created by Tihomir Ganev on 19.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import "ThemeManager.h"
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
#import "ATMHud.h"
#import "IdentityBlockedViewController.h"

@interface ThemeManager ()

@end

@implementation ThemeManager

+ (ThemeManager*)sharedManager
{
    static ThemeManager* sharedManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedManager = [[self alloc] init];
    });
    return sharedManager;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.hud = [[ATMHud alloc] initWithDelegate:self];
    }
    return self;
}
- (void)beautifyViewController:(id)vc
{

    UIViewController* v = (UIViewController*)vc;

    v.navigationController.navigationBar.barTintColor = [[SettingsManager sharedManager] color0];
    v.navigationController.navigationBar.tintColor = [[SettingsManager sharedManager] color6];
    [v.navigationController.navigationBar setTitleTextAttributes:@{ NSForegroundColorAttributeName : [[SettingsManager sharedManager] color6],
        NSFontAttributeName : [UIFont fontWithName:@"OpenSans" size:18.0f] }];
    v.navigationController.navigationBar.translucent = NO;

    if ([vc isMemberOfClass:[MenuViewController class]]) {
        MenuViewController* myVc = (MenuViewController*)vc;

        myVc.tblMenu.backgroundColor = [[SettingsManager sharedManager] color6];
        myVc.lblAppVersion.font = [UIFont fontWithName:@"OpenSans-Bold" size:12.f];
        myVc.view.backgroundColor = [[SettingsManager sharedManager] color6];

        myVc.lblConfigurationName.font = [UIFont fontWithName:@"OpenSans-Bold" size:14.f];
        myVc.lblConfigurationName.textColor = [[SettingsManager sharedManager] color0];
        myVc.lblConfigurationURL.font = [UIFont fontWithName:@"OpenSans" size:12.f];
        myVc.lblConfigurationURL.textColor = [[SettingsManager sharedManager] color0];
    }
    else if ([vc isMemberOfClass:[AccessNumberViewController class]]) {
        AccessNumberViewController* myVc = (AccessNumberViewController*)vc;
        myVc.lblEmail.textColor = [[SettingsManager sharedManager] color6];
        myVc.lblNote.textColor = [[SettingsManager sharedManager] color9];
        myVc.lblNote.font = [UIFont fontWithName:@"OpenSans-Semibold" size:12.];
        [self setupLoginButton:myVc.btnLogin];
    }

    else if ([vc isMemberOfClass:[ConfirmEmailViewController class]]) {
        ConfirmEmailViewController* myVc = (ConfirmEmailViewController*)vc;

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

    else if ([vc isMemberOfClass:[IdentityCreatedViewController class]]) {
        IdentityCreatedViewController* myVc = (IdentityCreatedViewController*)vc;
        [self setupLoginButton:myVc.btnSignIn];
        myVc.lblMessage.textColor = [[SettingsManager sharedManager] color2];
        myVc.lblEmail.textColor = [[SettingsManager sharedManager] color4];
    }

    else if ([vc isMemberOfClass:[PinPadViewController class]]) {
        PinPadViewController* myVc = (PinPadViewController*)vc;

        [self setupLoginButton:myVc.btnLogin];
    }

    else if ([vc isMemberOfClass:[AboutViewController class]]) {
    }

    else if ([vc isMemberOfClass:[AddIdentityViewController class]]) {
        AddIdentityViewController* myVc = (AddIdentityViewController*)vc;
        [myVc.txtIdentity setBottomBorder:[[SettingsManager sharedManager] color5] width:2.f alpha:.5f];
        [myVc.txtDevName setBottomBorder:[[SettingsManager sharedManager] color5] width:2.f alpha:.5f];
        [myVc.btnBack setup];
        myVc.lblIdentity.textColor = [[SettingsManager sharedManager] color5];
        myVc.lblIdentity.font = [UIFont fontWithName:@"OpenSans-Semibold" size:12.0];
        myVc.lblDevName.textColor = [[SettingsManager sharedManager] color5];
        myVc.lblDevName.font = [UIFont fontWithName:@"OpenSans-Semibold" size:12.0];
    }

    else if ([vc isMemberOfClass:[AddSettingViewController class]]) {
        AddSettingViewController* myVc = (AddSettingViewController*)vc;
        for (int i = 0; i < [myVc.view.subviews count]; i++) {
            if ([(myVc.view.subviews)[i] isMemberOfClass:[UILabel class]]) {
                UILabel* l = (UILabel*)(myVc.view.subviews)[i];
                l.textColor = [[SettingsManager sharedManager] color7];
            }
        }
        [self setupLoginButton:myVc.btnTestConfig];

    }

    else if ([vc isMemberOfClass:[OTPViewController class]]) {
        OTPViewController* myVc = (OTPViewController*)vc;

        myVc.lblEmail.textColor = [[SettingsManager sharedManager] color5];
        myVc.lblEmail.font = [UIFont fontWithName:@"OpenSans-Semibold" size:16.0];

        myVc.lblMessage.textColor = [[SettingsManager sharedManager] color2];
        myVc.lblMessage.font = [UIFont fontWithName:@"OpenSans" size:14.0];

        myVc.lblYourPassword.textColor = [[SettingsManager sharedManager] color4];
        myVc.lblYourPassword.font = [UIFont fontWithName:@"OpenSans-Semibold" size:12.0];

        myVc.lblOTP.textColor = [[SettingsManager sharedManager] color5];
        myVc.lblOTP.font = [UIFont fontWithName:@"OpenSans" size:38.0];

        myVc.lblMessage.text = @"You can now use this password to log in to your VPN service";
    }

    else if ([vc isMemberOfClass:[IdentityBlockedViewController class]]) {
        IdentityBlockedViewController* myVc = (IdentityBlockedViewController*)vc;
        
        myVc.navigationController.title = @"Identity blocked";
        myVc.lblUserEmail.textColor = [[SettingsManager sharedManager] color5];
        myVc.lblUserEmail.font = [UIFont fontWithName:@"OpenSans-Semibold" size:16.0];
        myVc.lblUserEmail.backgroundColor = [UIColor clearColor];
        
        myVc.lblMessage.textColor = [[SettingsManager sharedManager] color2];
        myVc.lblMessage.font = [UIFont fontWithName:@"OpenSans" size:14.0];
        myVc.lblMessage.numberOfLines = 0;
        myVc.lblMessage.text = @"Wrong PIN entered too many times. Your identity is blocked. You can sign up a new identity or choose existing one.";
        myVc.lblMessage.backgroundColor = [UIColor clearColor];

        [myVc.imgViewBlockedId setImage:[UIImage imageNamed:@"identity-blocked"]];
        
//        [myVc.btnResetPIN setTitleColor:[[SettingsManager sharedManager] color1] forState:UIControlStateNormal];
//        myVc.btnResetPIN.backgroundColor = [[SettingsManager sharedManager] color10];
//        [myVc.btnResetPIN setTitle:@"RESET PIN" forState:UIControlStateNormal];
        
        [myVc.btnBackToIdList setTitleColor:[[SettingsManager sharedManager] color10] forState:UIControlStateNormal];
        myVc.btnBackToIdList.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnBackToIdList setTitle:@"BACK TO IDENTITY LIST"  forState:UIControlStateNormal];

        [myVc.btnDeleteId setTitleColor:[[SettingsManager sharedManager] color1] forState:UIControlStateNormal];
        myVc.btnDeleteId.backgroundColor = [[SettingsManager sharedManager] color10];
        [myVc.btnDeleteId setTitle:@"REMOVE THIS IDENTITY"  forState:UIControlStateNormal];
        
        myVc.viewButtonsBG.backgroundColor = [[SettingsManager sharedManager] color3];
        
    }
    
    else if ([vc isMemberOfClass:[SettingsViewController class]]) {
        SettingsViewController* myVc = (SettingsViewController*)vc;

        myVc.btnDeleteConfiguration.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnDeleteConfiguration setTitleColor:[[SettingsManager sharedManager] color8] forState:UIControlStateNormal];

        myVc.btnEditConfiguration.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnEditConfiguration setTitleColor:[[SettingsManager sharedManager] color7] forState:UIControlStateNormal];

        myVc.viewButtons.backgroundColor = [[SettingsManager sharedManager] color3];
        [self setupLoginButton:myVc.btnSignIn];
    }

    else if ([vc isMemberOfClass:[UserListViewController class]]) {
        UserListViewController* myVc = (UserListViewController*)vc;

        myVc.btnDelete.backgroundColor = [[SettingsManager sharedManager] color1];
        [myVc.btnDelete setTitleColor:[[SettingsManager sharedManager] color8] forState:UIControlStateNormal];
        [myVc.btnDelete setTitle:@"DELETE" forState:UIControlStateNormal];

        //TODO: Add method to set button font
        //        [myVc.btnDelete.titleLabel setFont:];

        [self setupLoginButton:myVc.btnAuthenticate];
        [self setupLoginButton:myVc.btnAdd];

        [myVc.btnAdd setTitle:@"ADD NEW IDENTITY +" forState:UIControlStateNormal];
        myVc.viewButtonsContainer.backgroundColor = [[SettingsManager sharedManager] color3];
        myVc.view.backgroundColor = [[SettingsManager sharedManager] color3];
        myVc.title = @"Identity List";
    }

    else {
        for (int i = 0; i < [((UIViewController*)vc).view.subviews count]; i++) {
            if ([(((UIViewController*)vc).view.subviews)[i] isMemberOfClass:[UILabel class]]) {
                UILabel* l = (((UIViewController*)vc).view.subviews)[i];
                l.font = [UIFont fontWithName:@"OpenSans" size:22.0];
            }
        }
    }
}

- (void)setupLoginButton:(UIButton*)button
{
    [button setTitleColor:[[SettingsManager sharedManager] color1] forState:UIControlStateNormal];
    button.backgroundColor = [[SettingsManager sharedManager] color10];
}

- (void)setupRegularButton:(UIButton*)button
{
    [button setTitleColor:[[SettingsManager sharedManager] color1] forState:UIControlStateNormal];
    button.backgroundColor = [[SettingsManager sharedManager] color10];
}

- (void)setupDeleteButton:(UIButton*)button
{
    [button setTitleColor:[[SettingsManager sharedManager] color1] forState:UIControlStateNormal];
    button.backgroundColor = [[SettingsManager sharedManager] color10];

    button.titleLabel.font = [UIFont fontWithName:@"OpenSans" size:18.0];
}

- (void)customiseMenuCell:(MenuTableViewCell*)cell
{
    cell.accessoryType = UITableViewCellAccessoryNone;
    cell.backgroundColor = [[SettingsManager sharedManager] color6];
    cell.lblMenuID.font = [UIFont fontWithName:@"OpenSans" size:14.f];
    cell.lblMenuID.textColor = [[SettingsManager sharedManager] color0];
    cell.viewSeparator.backgroundColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"border-line"]];
    cell.selectedBackgroundView.backgroundColor = [[SettingsManager sharedManager] color5];
}

- (void)customiseConfigurationListCell:(ConfigListTableViewCell*)cell
{
    cell.accessoryType = UITableViewCellAccessoryNone;
    cell.backgroundColor = [[SettingsManager sharedManager] color0];

    cell.lblConfigurationName.font = [UIFont fontWithName:@"OpenSans" size:16.f];
    cell.lblConfigurationName.textColor = [[SettingsManager sharedManager] color6];

    cell.lblConfigurationType.font = [UIFont fontWithName:@"OpenSans" size:14.f];
    cell.lblConfigurationType.textColor = [[SettingsManager sharedManager] color4];
}

//TODO: Add method for title labels
@end