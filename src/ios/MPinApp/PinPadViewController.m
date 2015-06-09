//
//  ViewController.m
//  MPinSDK
//
//  Created by Georgi Georgiev on 11/14/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

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

#define PIN_LENGTH 4

static NSMutableArray *kCircles;

@interface PinPadViewController ( )

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
    [[ThemeManager sharedManager] beautifyViewController:self];
    self.sdk = [[MPin alloc] init];
    self.sdk.delegate = self;
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
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
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- ( void )viewWillDisappear:( BOOL )animated
{
    [super viewWillDisappear:animated];
}

- ( IBAction )logInAction:( id )sender
{
    [MPin sendPin:self.strNumber];
    [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"" addActivityIndicator:YES minShowTime:0];
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

- ( void )OnAuthenticateOTPCompleted:( id )sender user:( id<IUser>)user otp:( OTP * )otp
{
    if ( otp.status.status != OK )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:@"OTP is not supported!"
         addActivityIndicator:NO
         minShowTime:0];

        return;
    }
    OTPViewController *otpViewController = [[UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil] instantiateViewControllerWithIdentifier:@"OTP"];
    otpViewController.otpData = otp;
    otpViewController.strEmail = [user getIdentity];
    [self.navigationController pushViewController:otpViewController animated:YES];
}

- ( void )OnAuthenticateOTPError:( id )sender error:( NSError * )error
{
    MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
    _lblWrongPIN.text = NSLocalizedString(mpinStatus.statusCodeAsString, @"UNKNOWN ERROR");
    [_sdk AuthenticateOTP:_currentUser askForFingerprint:NO];
    [self showWrongPIN];
    [self clearAction:self];
}

-( void ) onAccessNumber:( NSString * ) an
{
    NSLog(@"onAccessNumber");
}

- ( void )OnAuthenticateAccessNumberCompleted:( id )sender user:( id<IUser>)user
{
    ANAuthenticationSuccessful *vcANsuccess = [[UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil] instantiateViewControllerWithIdentifier:@"ANAuthenticationSuccessful"];
    vcANsuccess.currentUser = _currentUser;
    [self.navigationController pushViewController:vcANsuccess animated:YES];
}

- ( void )OnAuthenticateAccessNumberError:( id )sender error:( NSError * )error
{
    [[ErrorHandler sharedManager] hideMessage];
    [self showWrongPIN];
    [_sdk AuthenticateAN:_currentUser accessNumber:_strAccessNumber askForFingerprint:NO];
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
    [self popToRoot];
}

- ( void )OnAuthenticateError:( id )sender error:( NSError * )error
{
    if ( [_currentUser getState] == BLOCKED )
    {
        IdentityBlockedViewController *identityBlockedViewController = [[UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil] instantiateViewControllerWithIdentifier:@"IdentityBlockedViewController"];
        identityBlockedViewController.strUserEmail = [_currentUser getIdentity];
        identityBlockedViewController.iuser = _currentUser;
        [self.navigationController pushViewController:identityBlockedViewController animated:YES];

    }
    else
    {
        [[ErrorHandler sharedManager] hideMessage];
        [self showWrongPIN];
        [_sdk Authenticate:_currentUser askForFingerprint:NO];
    }
}

@end
