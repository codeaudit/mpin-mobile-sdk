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

@interface IdentityCreatedViewController ( ) {
    MPin *sdk;
}

- ( void )showPinPad;

- ( void )startAuthenticationFlow:( id<IUser>)forUser forService:( enum SERVICES )service;

- ( IBAction )gotoIDList:( id )sender;

@end

@implementation IdentityCreatedViewController

- ( void )viewDidLoad
{
    [super viewDidLoad];


    [[ThemeManager sharedManager] beautifyViewController:self];

    self.title = NSLocalizedString(@"ID_CREATED_TITLE", @"");
    [self.btnSignIn setTitle:NSLocalizedString(@"ID_CREATED_BTN_SIGN_IN", @"") forState:UIControlStateNormal];
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];

    sdk = [[MPin alloc] init];
    sdk.delegate = self;

    _lblEmail.text = _strEmail;
    _lblMessage.text = [NSString stringWithFormat:NSLocalizedString(@"ID_CREATED_MESSAGE", @""), _strEmail];
}

- ( void )viewDidAppear:( BOOL )animated
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( showPinPad ) name:kShowPinPadNotification object:nil];
}

- ( void )viewDidDisappear:( BOOL )animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}

#pragma mark - mpin async delegates -

- ( void )OnAuthenticateCanceled
{
    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:@"Authentication canceled" addActivityIndicator:NO minShowTime:3];
}

- ( void )OnAuthenticateOTPCompleted:( id )sender user:( id<IUser>)user otp:( OTP * )otp
{
    if ( otp.status.status != OK )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:[otp.status getStatusCodeAsString] addActivityIndicator:NO minShowTime:3];

        return;
    }
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    OTPViewController *otpViewController = [storyboard instantiateViewControllerWithIdentifier:@"OTP"];
    otpViewController.otpData = otp;
    otpViewController.strEmail = [user getIdentity];
    [self.navigationController pushViewController:otpViewController animated:YES];
}

- ( void )OnAuthenticateOTPError:( id )sender error:( NSError * )error
{
    MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
    [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:NSLocalizedString([mpinStatus getStatusCodeAsString], @"") addActivityIndicator:NO minShowTime:3];
}

-( void ) onAccessNumber:( NSString * ) an
{
    [sdk AuthenticateAN:self.user accessNumber:an askForFingerprint:NO];
}

- ( void )OnAuthenticateAccessNumberCompleted:( id )sender user:( id<IUser>)user
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Authentication Successful!" message:nil delegate:nil cancelButtonTitle:NSLocalizedString(@"KEY_CLOSE", @"") otherButtonTitles:nil, nil];
    [alert show];
}

- ( void )OnAuthenticateAccessNumberError:( id )sender error:( NSError * )error
{
    switch ( error.code )
    {
    case INCORRECT_PIN:
        [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"Wrong MPIN or Access Number" addActivityIndicator:NO minShowTime:3];
        break;

    case HTTP_REQUEST_ERROR:
        [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"HTTP REQUEST ERROR" addActivityIndicator:NO minShowTime:3];
        break;

    default:
        [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:[( error.userInfo ) [kMPinSatus] getStatusCodeAsString] addActivityIndicator:NO minShowTime:3];
    }
}

- ( void )OnAuthenticateCompleted:( id )sender user:( const id<IUser>)user
{
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    AccountSummaryViewController *vcAccountSummary = [storyboard instantiateViewControllerWithIdentifier:@"AccountSummary"];
    vcAccountSummary.strEmail = [self.user getIdentity];
    [self.navigationController pushViewController:vcAccountSummary animated:YES];
}

- ( void )OnAuthenticateError:( id )sender error:( NSError * )error
{
    switch ( error.code )
    {
    case INCORRECT_PIN:
        [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"Authentication Failed" addActivityIndicator:NO minShowTime:3];

        break;

    default:
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:[( error.userInfo ) [kMPinSatus] getStatusCodeAsString] addActivityIndicator:NO minShowTime:3];
    } break;
    }
}

#pragma mark - My methods -

- ( void )showPinPad
{
    [[ErrorHandler sharedManager] hideMessage];
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    PinPadViewController *pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.sdk = sdk;
    pinpadViewController.sdk.delegate = pinpadViewController;
    pinpadViewController.currentUser = self.user;
    pinpadViewController.boolShouldShowBackButton = YES;
    pinpadViewController.title = kEnterPin;
    [self.navigationController pushViewController:pinpadViewController animated:YES];
}

- ( void )startAuthenticationFlow:( id<IUser>)forUser forService:( enum SERVICES )service;
{
    self.user = forUser;
    switch ( service )
    {
    case LOGIN_ON_MOBILE:
        [sdk Authenticate:self.user askForFingerprint:NO];
        break;

    case LOGIN_ONLINE:
    {
        UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
        AccessNumberViewController *accessViewController = [storyboard instantiateViewControllerWithIdentifier:@"accessnumber"];
        accessViewController.currentUser = self.user;
        accessViewController.delegate = self;
        accessViewController.strEmail = [self.user getIdentity];
        [self.navigationController pushViewController:accessViewController animated:YES];
    } break;

    case LOGIN_WITH_OTP:
        [sdk AuthenticateOTP:self.user askForFingerprint:NO];
        break;
    }
}


#pragma mark - My actions -
- ( IBAction )showLeftMenuPressed:( id )sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

- ( IBAction )gotoIDList:( id )sender
{
    [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"" addActivityIndicator:YES minShowTime:0];
    NSDictionary *config = [[ConfigurationManager sharedManager] getSelectedConfiguration];
    [self startAuthenticationFlow:self.user forService:[config [kSERVICE_TYPE] intValue]];
}

#pragma mark - Alert view delegate -
- ( void )alertView:( UIAlertView * )alertView clickedButtonAtIndex:( NSInteger )buttonIndex
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

@end
