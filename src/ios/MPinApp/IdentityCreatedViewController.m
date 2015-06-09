//
//  IdentityCreatedViewController.m
//  MPinApp
//
//  Created by Tihomir Ganev on 27.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

#import "IdentityCreatedViewController.h"
#import "ConfigurationManager.h"
#import "OTPViewController.h"
#import "AccountSummaryViewController.h"
#import "ThemeManager.h"
#import "MFSideMenu.h"

@interface IdentityCreatedViewController () {
    MPin* sdk;
}

- (void)showPinPad;
- (void)showError:(NSString*)title desc:(NSString*)desc;

- (void)startAuthenticationFlow:(id<IUser>)forUser forService:(enum SERVICES)service;

- (IBAction)gotoIDList:(id)sender;

@end

@implementation IdentityCreatedViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    sdk = [[MPin alloc] init];
    sdk.delegate = self;
    [[ThemeManager sharedManager] beautifyViewController:self];
    
    self.title = NSLocalizedString(@"ID_CREATED_TITLE", @"");
    [self.btnSignIn setTitle:NSLocalizedString(@"ID_CREATED_BTN_SIGN_IN", @"") forState:UIControlStateNormal];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    _lblEmail.text = _strEmail;
    _lblMessage.text = [NSString stringWithFormat:NSLocalizedString(@"ID_CREATED_MESSAGE", @""), _strEmail];
}

- (void)viewDidAppear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showPinPad) name:kShowPinPadNotification object:nil];
}
- (void)viewDidDisappear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}

- (void)showPinPad
{
    UIStoryboard* storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    PinPadViewController* pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.sdk = sdk;
    pinpadViewController.sdk.delegate = pinpadViewController;
    pinpadViewController.currentUser = self.user;
    pinpadViewController.boolShouldShowBackButton = YES;
    pinpadViewController.title = kEnterPin;
    [self.navigationController pushViewController:pinpadViewController animated:YES];
}

- (void)startAuthenticationFlow:(id<IUser>)forUser forService:(enum SERVICES)service;
{
    self.user = forUser;
    switch (service) {
    case LOGIN_ON_MOBILE:
        [sdk Authenticate:self.user  askForFingerprint:YES];
        break;
    case LOGIN_ONLINE: {
        UIStoryboard* storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
        AccessNumberViewController* accessViewController = [storyboard instantiateViewControllerWithIdentifier:@"accessnumber"];
        accessViewController.currentUser = self.user;
        accessViewController.delegate = self;
        accessViewController.strEmail = [self.user getIdentity];
        [self.navigationController pushViewController:accessViewController animated:YES];
    } break;
    case LOGIN_WITH_OTP:
        [sdk AuthenticateOTP:self.user askForFingerprint:YES];
        break;
    }
}

- (IBAction)gotoIDList:(id)sender
{
    NSDictionary* config = [[ConfigurationManager sharedManager] getSelectedConfiguration];
    [self startAuthenticationFlow:self.user forService:[config[kSERVICE_TYPE] intValue]];
}

- (void)OnAuthenticateCanceled
{
    [self showError:@"Authentication Failed!" desc:@"TouchID failed"];
}

- (void)OnAuthenticateOTPCompleted:(id)sender user:(id<IUser>)user otp:(OTP*)otp
{
    if (otp.status.status != OK) {
        [self showError:[otp.status getStatusCodeAsString] desc:@"OTP is not supported!"];
        return;
    }
    UIStoryboard* storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    OTPViewController* otpViewController = [storyboard instantiateViewControllerWithIdentifier:@"OTP"];
    otpViewController.otpData = otp;
    otpViewController.strEmail = [user getIdentity];
    [self.navigationController pushViewController:otpViewController animated:YES];
}

- (void)OnAuthenticateOTPError:(id)sender error:(NSError*)error
{
    MpinStatus* mpinStatus = (error.userInfo)[kMPinSatus];
    [self showError:[mpinStatus getStatusCodeAsString] desc:mpinStatus.errorMessage];
}

-(void) onAccessNumber:(NSString *) an {
    [sdk AuthenticateAN:self.user accessNumber:an  askForFingerprint:YES];
}

- (void)OnAuthenticateAccessNumberCompleted:(id)sender user:(id<IUser>)user
{    
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"Authentication Successful!" message:nil delegate:nil cancelButtonTitle:NSLocalizedString(@"KEY_CLOSE", @"") otherButtonTitles:nil, nil];
    [alert show];
}

- (void)OnAuthenticateAccessNumberError:(id)sender error:(NSError*)error
{
    switch (error.code) {
    case INCORRECT_PIN:
        [self showError:@"Authentication Failed!" desc:@"Wrong MPIN or Access Number!"];
        break;
    case HTTP_REQUEST_ERROR:
        [self showError:@"Authentication Failed!" desc:@"Wrong MPIN or Access Number!"];
        break;
    default: {
        MpinStatus* mpinStatus = (error.userInfo)[kMPinSatus];
        [self showError:[mpinStatus getStatusCodeAsString] desc:mpinStatus.errorMessage];
    } break;
    }
}

- (void)OnAuthenticateCompleted:(id)sender user:(const id<IUser>)user
{
    UIStoryboard* storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    AccountSummaryViewController* vcAccountSummary = [storyboard instantiateViewControllerWithIdentifier:@"AccountSummary"];
    vcAccountSummary.strEmail = [self.user getIdentity];
    [self.navigationController pushViewController:vcAccountSummary animated:YES];
}

- (void)OnAuthenticateError:(id)sender error:(NSError*)error
{
    switch (error.code) {
    case INCORRECT_PIN:
        [self showError:@"Authentication Failed!" desc:@"Wrong MPIN"];
        break;
    default: {
        MpinStatus* mpinStatus = (error.userInfo)[kMPinSatus];
        [self showError:[mpinStatus getStatusCodeAsString] desc:mpinStatus.errorMessage];
    } break;
    }
}

- (IBAction)showLeftMenuPressed:(id)sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

- (void)alertView:(UIAlertView*)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

@end
