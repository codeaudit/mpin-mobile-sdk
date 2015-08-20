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
    self.title = NSLocalizedString(@"ID_CREATED_TITLE", @"");
    [self.btnSignIn setTitle:NSLocalizedString(@"ID_CREATED_BTN_SIGN_IN", @"") forState:UIControlStateNormal];
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    [self registerObservers];
    sdk = [[MPin alloc] init];
    sdk.delegate = self;
    _lblEmail.text = _strEmail;
    _lblMessage.text = [NSString stringWithFormat:NSLocalizedString(@"ID_CREATED_MESSAGE", @""), _strEmail];
    [[ThemeManager sharedManager] beautifyViewController:self];
}

- ( void )viewDidAppear:( BOOL )animated
{
    [super viewDidAppear:animated];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( showPinPad ) name:kShowPinPadNotification object:nil];
}

- ( void )viewWillDisappear:( BOOL )animated
{
    [super viewWillDisappear:animated];
    [self unRegisterObservers];
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

#pragma mark - NSNotification handlers -

-( void ) networkUp
{
    [[ThemeManager sharedManager] hideNetworkDown:self];
}

-( void ) networkDown
{
    NSLog(@"Network DOWN Notification");
    [self.view layoutIfNeeded];
    [UIView animateWithDuration:kFltNoNetworkMessageAnimationDuration animations:^{
        self.constraintNoNetworkViewHeight.constant = 36.0f;
        [self.view layoutIfNeeded];
    }];
}

-( void ) unRegisterObservers
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"NETWORK_DOWN_NOTIFICATION" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"NETWORK_UP_NOTIFICATION" object:nil];
}

- ( void ) registerObservers
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( networkUp ) name:@"NETWORK_UP_NOTIFICATION" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( networkDown ) name:@"NETWORK_DOWN_NOTIFICATION" object:nil];
}

@end
