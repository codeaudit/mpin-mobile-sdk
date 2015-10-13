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




#import "PinPadViewController.h"
#import "Constants.h"
#import "MPin.h"
#import "BackButton.h"
#import "UIView+Helper.m"
#import "MFSideMenu.h"
#import "ThemeManager.h"
#import "AccountSummaryViewController.h"
#import "OTPViewController.h"
#import "ANAuthenticationSuccessful.h"
#import "IdentityBlockedViewController.h"
#import "IdentityCreatedViewController.h"
#import "ConfirmEmailViewController.h"
#define PIN_LENGTH 4

static NSMutableArray *kCircles;

@interface PinPadViewController ( )
{}

@property ( nonatomic, weak ) IBOutlet UIImageView *imgViewDigit0;
@property ( nonatomic, weak ) IBOutlet UIImageView *imgViewDigit1;
@property ( nonatomic, weak ) IBOutlet UIImageView *imgViewDigit2;
@property ( nonatomic, weak ) IBOutlet UIImageView *imgViewDigit3;


- ( void )renderNumberTextField:( NSInteger )numberLenght;
- ( IBAction )back:( UIBarButtonItem * )sender;

@property ( nonatomic, weak ) IBOutlet BackButton *backButton;

@end

@implementation PinPadViewController

- ( void )renderNumberTextField:( NSInteger )numberLenght
{
    kCircles = [NSMutableArray arrayWithCapacity:PIN_LENGTH + 1];
    NSString *pinField = @"";
    for ( int i = 0; i < PIN_LENGTH; i++ )
        pinField = [pinField stringByAppendingString:@"⚪"];
    [kCircles addObject:pinField];
    for ( int i = 0; i < PIN_LENGTH; i++ )
    {
        pinField = [pinField stringByReplacingCharactersInRange:NSMakeRange(i, 1) withString:@"⚫"];
        [kCircles addObject:pinField];
    }
}

- ( void )viewDidLoad
{
    [super viewDidLoad];
    max = PIN_LENGTH;
    [self renderNumberTextField:PIN_LENGTH];
    self.label.text = kCircles [0];
    [self.pinView setBottomBorder:[[SettingsManager sharedManager] color7] width:2.f alpha:.5f];
    self.sdk = [[MPin alloc] init];
    self.sdk.delegate = self;
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    [self registerObservers];
    [[ThemeManager sharedManager] beautifyViewController:self];
    [self hideWrongPIN];
    if ( self.sdk == nil )
    {
        self.sdk = [[MPin alloc] init];
    }
    self.sdk.delegate = self;
    self.strNumber = @"";
    _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];

    [self.menuContainerViewController setPanMode:MFSideMenuPanModeNone];

    BackButton *btnBack = [[BackButton alloc] initWithTitle:@"" style:UIBarButtonItemStylePlain target:self action:@selector( back: )];
    [btnBack setup];

    if ( !_boolShouldShowBackButton )
    {
        self.navigationItem.hidesBackButton = YES;
        self.navigationItem.leftBarButtonItem = nil;
    }
    else
    {
        self.navigationItem.hidesBackButton = NO;
        self.navigationItem.leftBarButtonItem = btnBack;
    }

    self.lblEmail.text = [_currentUser getIdentity];
    if ( _boolSetupPin )
    {
        self.title = NSLocalizedString(@"KEY_SETUP_PIN", @"");
    }
    else
    {
        self.title = NSLocalizedString(@"KEY_ENTER_PIN", @"");
    }
}

- ( IBAction )back:( UIBarButtonItem * )sender
{
    [MPin sendPin:kEmptyStr];
    self.sdk.delegate = nil;
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- ( void )viewWillDisappear:( BOOL )animated
{
    [super viewWillDisappear:animated];
    [self unRegisterObservers];
}

- ( void ) viewDidDisappear:( BOOL )animated
{
    [super viewDidDisappear:animated];
    self.sdk.delegate = nil;
}

- ( IBAction )logInAction:( id )sender
{
    [MPin sendPin:self.strNumber];
    [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"" addActivityIndicator:YES minShowTime:0];
    NSLog(@"sendPIN: %@", self.strNumber);
    if (self.boolIsSMS || self.boolSetupPin) {
        [self popToRoot];
    }
}

- ( IBAction )clearAction:( id )sender
{
    [super clearAction:sender];
    _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-empty"];
    _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];

    self.label.text = kCircles [numberIndex];
}

- ( void ) hideWrongPIN
{
    [self.pinView setBottomBorder:[[SettingsManager sharedManager] color7] width:2.f alpha:.5f];
    _lblWrongPIN.hidden = YES;
}

- ( void ) showWrongPIN
{
    [self.pinView setBottomBorder:[UIColor redColor] width:2.f alpha:.5f];
    _lblWrongPIN.text = NSLocalizedString(@"INCORRECT_PIN", @"Incorrect PIN.  Please try again.");
    [self clearAction:self];
    _lblWrongPIN.hidden = NO;
    [[ErrorHandler sharedManager] hideMessage];
}

-( void ) popToRoot
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- ( IBAction )numberSelectedAction:( id )sender
{
    if ( [self.strNumber length] >= 4 )
    {
        return;
    }
    NSLog(@"Number: %@", self.strNumber);
    [super numberSelectedAction:sender];
    [self hideWrongPIN];
    switch ( [self.strNumber length] )
    {
    case 0:
        _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];
        break;

    case 1:
        _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];
        break;

    case 2:
        _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-empty"];
        _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];
        break;

    case 3:
        _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-empty"];
        break;

    case 4:
        _imgViewDigit0.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit1.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit2.image = [UIImage imageNamed:@"pin-dot-full"];
        _imgViewDigit3.image = [UIImage imageNamed:@"pin-dot-full"];
        break;

    default:
        break;
    }
}

#pragma mark - SDK Handlers -

- ( void )OnFinishRegistrationCompleted:( id )sender user:( const id<IUser>)user
{
    IdentityCreatedViewController *vcIDCreated = (IdentityCreatedViewController *)[[UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil] instantiateViewControllerWithIdentifier:@"IdentityCreatedViewController"];
    vcIDCreated.user = user;
    vcIDCreated.strEmail = [user getIdentity];
    [self.navigationController pushViewController:vcIDCreated animated:YES];
}

- ( void )OnFinishRegistrationError:( id )sender error:( NSError * )error
{
    switch ( error.code )
    {
        case IDENTITY_NOT_VERIFIED:
        {
            [[ErrorHandler sharedManager] hideMessage];
            ConfirmEmailViewController *cevc = (ConfirmEmailViewController *)[[UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil] instantiateViewControllerWithIdentifier:@"ConfirmEmailViewController"];
            cevc.iuser = ( error.userInfo ) [kUSER];
            [self.navigationController pushViewController:cevc animated:YES];
        }
            break;
            
        case HTTP_SERVER_ERROR:
            [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:NSLocalizedString(@"HTTP_SERVER_ERROR", @"SERVER ERROR.  PLEASE CONTACT YOUR SYSTEM ADMINISTRATOR.") addActivityIndicator:NO minShowTime:3];
            
        default:
            break;
    }
}

- ( void )OnAuthenticateOTPCompleted:( id )sender user:( id<IUser>)user otp:( OTP * )otp
{
    NSLog(@"OnAuthenticateOTPCompleted");
    [self clearAction:self];
    if ( otp.status.status != OK )
    {
        [[ErrorHandler sharedManager] updateMessage:@"OTP is not supported!" addActivityIndicator:NO hideAfter:3];
        dispatch_after(dispatch_time( DISPATCH_TIME_NOW, (int64_t)( 2.0 * NSEC_PER_SEC ) ), dispatch_get_main_queue(), ^ {
            [self.navigationController popToRootViewControllerAnimated:YES];
        });
    }
    else
    {
        [[ErrorHandler sharedManager] hideMessage];
        OTPViewController *otpViewController = [[UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil] instantiateViewControllerWithIdentifier:@"OTP"];
        otpViewController.otpData = otp;
        otpViewController.strEmail = [user getIdentity];
        [self.navigationController pushViewController:otpViewController animated:YES];
    }
}

- ( void )OnAuthenticateOTPError:( id )sender error:( NSError * )error
{
    NSLog(@"OnAuthenticateOTPError");
    if ( [_currentUser getState] == BLOCKED )
    {
        [self showBlockedScreen];
    }
    else
    {
        switch ( error.code )
        {
        case INCORRECT_PIN:
            [[ErrorHandler sharedManager] hideMessage];
            [_sdk AuthenticateOTP:_currentUser askForFingerprint:NO];
            [self showWrongPIN];
            [self clearAction:self];

            break;

        case CRYPTO_ERROR:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"CRYPTO_ERROR", @"Request error") addActivityIndicator:NO hideAfter:5];
            break;

        case STORAGE_ERROR:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"STORAGE_ERROR", @"Request error") addActivityIndicator:NO hideAfter:5];
            break;

        case NETWORK_ERROR:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"NETWORK_ERROR", @"Request error") addActivityIndicator:NO hideAfter:5];
            break;

        case RESPONSE_PARSE_ERROR:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"RESPONSE_PARSE_ERROR", @"Request error") addActivityIndicator:NO hideAfter:5];
            break;

        case FLOW_ERROR:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"FLOW_ERROR", @"Request error") addActivityIndicator:NO hideAfter:5];
            break;

        case IDENTITY_NOT_AUTHORIZED:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"IDENTITY_NOT_AUTHORIZED", @"Request error") addActivityIndicator:NO hideAfter:5];
            break;

        case IDENTITY_NOT_VERIFIED:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"IDENTITY_NOT_VERIFIED", @"Request error") addActivityIndicator:NO hideAfter:5];
            break;

        case REQUEST_EXPIRED:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"REQUEST_EXPIRED", @"Request error") addActivityIndicator:NO hideAfter:5];
            break;

        case REVOKED:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"REVOKED", @"Request error") addActivityIndicator:NO hideAfter:5];
            break;

        case HTTP_SERVER_ERROR:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"HTTP_SERVER_ERROR", @"Request error") addActivityIndicator:NO hideAfter:5];
            break;

        case HTTP_REQUEST_ERROR:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"HTTP_REQUEST_ERROR", @"Request error") addActivityIndicator:NO hideAfter:5];
            break;

        case       PIN_INPUT_CANCELED:
            NSLog(@"PIN_INPUT_CANCELED");
            [self clearAction:self];
            [[ErrorHandler sharedManager] hideMessage];
            break;

        default:
            [self clearAction:self];
            [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"UNKNOWN ERROR"
             addActivityIndicator:NO
             minShowTime:3];
            break;
        }
    }
}

-( void ) onAccessNumber:( NSString * ) an
{
    NSLog(@"onAccessNumber");
}

- ( void )OnAuthenticateAccessNumberCompleted:( id )sender user:( id<IUser>)user
{
    NSLog(@"OnAuthenticateAccessNumberCompleted");
    ANAuthenticationSuccessful *vcANsuccess = [[UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil] instantiateViewControllerWithIdentifier:@"ANAuthenticationSuccessful"];
    vcANsuccess.currentUser = _currentUser;
    [self.navigationController pushViewController:vcANsuccess animated:YES];
}

- ( void )OnAuthenticateAccessNumberError:( id )sender error:( NSError * )error
{
    NSLog(@"OnAuthenticateAccessNumberError");
    if ( [_currentUser getState] == BLOCKED )
    {
        [self showBlockedScreen];
    }
    else
    {
        MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
        switch ( error.code )
        {
        case INCORRECT_ACCESS_NUMBER:
        {
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(mpinStatus.statusCodeAsString, mpinStatus.errorMessage)
             addActivityIndicator:NO
             hideAfter:5];

            dispatch_after(dispatch_time( DISPATCH_TIME_NOW, (int64_t)( 2.0 * NSEC_PER_SEC ) ), dispatch_get_main_queue(), ^ {
                    [self.navigationController popViewControllerAnimated:YES];
                });
            [self clearAction:self];
            break;
        }


        case INCORRECT_PIN:
            [[ErrorHandler sharedManager] hideMessage];
            [self showWrongPIN];
            [_sdk AuthenticateAN:_currentUser accessNumber:_strAccessNumber askForFingerprint:NO];
            break;

        case HTTP_REQUEST_ERROR:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(mpinStatus.statusCodeAsString, @"UNKNOWN ERROR") addActivityIndicator:NO hideAfter:5.0];

            break;

        default:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(mpinStatus.statusCodeAsString, @"UNKNOWN ERROR") addActivityIndicator:NO hideAfter:5.0];
            break;
        }
    }
}

- ( void )OnAuthenticateCompleted:( id )sender user:( const id<IUser>)user
{
    NSLog(@"OnAuthenticateCompleted");
    AccountSummaryViewController *vcAccountSummary = [[UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil] instantiateViewControllerWithIdentifier:@"AccountSummary"];
    vcAccountSummary.strEmail = [user getIdentity];
    [self.navigationController pushViewController:vcAccountSummary animated:YES];
}

- ( void ) OnAuthenticateCanceled
{
    NSLog(@"OnAuthenticateCanceled");
    [self popToRoot];
}

- ( void )OnAuthenticateError:( id )sender error:( NSError * )error
{
    if ( [_currentUser getState] == BLOCKED )
    {
        [self showBlockedScreen];
    }
    else
    {
        [[ErrorHandler sharedManager] hideMessage];
        [self showWrongPIN];
        [_sdk Authenticate:_currentUser askForFingerprint:NO];
    }
}

- ( void ) showBlockedScreen
{
    IdentityBlockedViewController *identityBlockedViewController = [[UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil] instantiateViewControllerWithIdentifier:@"IdentityBlockedViewController"];
    identityBlockedViewController.strUserEmail = [_currentUser getIdentity];
    identityBlockedViewController.iuser = _currentUser;
    [self.navigationController pushViewController:identityBlockedViewController animated:YES];
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

- (void) applicationWillResignActive
{
    [MPin sendPin:kEmptyStr];
    self.sdk.delegate = nil;
}

-( void ) unRegisterObservers
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"NETWORK_DOWN_NOTIFICATION" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"NETWORK_UP_NOTIFICATION" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationWillResignActiveNotification object:nil];
}

- ( void ) registerObservers
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( networkUp ) name:@"NETWORK_UP_NOTIFICATION" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( networkDown ) name:@"NETWORK_DOWN_NOTIFICATION" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillResignActive) name:UIApplicationWillResignActiveNotification object:nil];
    
}

@end