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


#import "AddIdentityViewController.h"
#import "Constants.h"
#import "IUser.h"
#import "UIViewController+Helper.h"
#import "IdentityCreatedViewController.h"
#import "ConfirmEmailViewController.h"
#import "PinPadViewController.h"
#import "ConfigurationManager.h"
#import "MPin.h"
#import "ThemeManager.h"
#import "AFNetworkReachabilityManager.h"

static NSString *const kEmpty = @"";
static NSString *const kMpinStatus = @"MpinStatus";
static NSString *const kUser = @"User";

@interface AddIdentityViewController ( ) {
    MPin *sdk;
    id<IUser> currentUser;
    ThemeManager *themeManager;
}

- ( void )showPinPad;
- ( void )showDeviceName;
- ( void )hideDeviceName;

- ( IBAction )textFieldReturn:( id )sender;
- ( IBAction )addAction:( id )sender;
- ( IBAction )back:( id )sender;
@end

@implementation AddIdentityViewController

- ( void )viewDidLoad
{
    [super viewDidLoad];
    self.automaticallyAdjustsScrollViewInsets = NO;

    [[ThemeManager sharedManager] beautifyViewController:self];

    ConfigurationManager *cfm = [ConfigurationManager sharedManager];
    self.txtDevName.text = [cfm getDeviceName];

    if ( [MPin isDeviceName] )
    {
        [self showDeviceName];
    }
    else
    {
        [self hideDeviceName];
    }
    _txtIdentity.placeholder    = NSLocalizedString(@"ADDIDVC_LBL_IDENTITY", @"");
    _txtDevName.placeholder     = NSLocalizedString(@"ADDIDVC_TXT_DEVNAME", @"");
    _lblIdentity.text           = NSLocalizedString(@"ADDIDVC_LBL_IDENTITY", @"");
    _lblDevName.text            = NSLocalizedString(@"ADDIDVC_LBL_DEVNAME", @"");
    self.title                  = NSLocalizedString(@"ADDIDVC_TITLE", @"");
}

-( void ) viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    sdk = [[MPin alloc] init];
    sdk.delegate = self;
    if ( ![AFNetworkReachabilityManager sharedManager].reachable )
    {}
}

- ( void )viewDidAppear:( BOOL )animated
{
    [super viewDidAppear:animated];
    [[NSNotificationCenter defaultCenter] addObserver:self
     selector:@selector( showPinPad )
     name:kShowPinPadNotification
     object:nil];
}

- ( void )viewDidDisappear:( BOOL )animated
{
    [super viewDidDisappear:animated];
    [[ErrorHandler sharedManager] hideMessage];
    [[NSNotificationCenter defaultCenter] removeObserver:self
     name:kShowPinPadNotification
     object:nil];
}

- ( void )showPinPad
{
    UIStoryboard *storyboard =
        [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    PinPadViewController *pinpadViewController =
        [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.currentUser = currentUser;
    pinpadViewController.boolShouldShowBackButton = NO;
    pinpadViewController.title = kSetupPin;
    [self.navigationController pushViewController:pinpadViewController
     animated:NO];
}

- ( void )showDeviceName
{
    self.txtDevName.hidden = NO;
    self.lblDevName.hidden = NO;
}

- ( void )hideDeviceName
{
    self.txtDevName.hidden = YES;
    self.lblDevName.hidden = YES;
}

- ( IBAction )textFieldReturn:( id )sender
{
    [sender resignFirstResponder];
}

- ( IBAction )addAction:( id )sender
{
    if ( [kEmpty isEqualToString:self.txtIdentity.text] )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"ERROR_PLEASE_ENTER_VALID_USER_ID", @"")
         addActivityIndicator:NO
         minShowTime:3];

        return;
    }

    self.txtIdentity.text = [self.txtIdentity.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];

    if ( ![self isValidEmail:self.txtIdentity.text] )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"ERROR_PLEASE_ENTER_VALID_EMAIL", @"")
         addActivityIndicator:NO
         minShowTime:3];

        return;
    }

    if ( [MPin isDeviceName] )
    {
        ConfigurationManager *cfm = [ConfigurationManager sharedManager];
        [cfm setDeviceName:self.txtDevName.text];
    }

    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:@""
     addActivityIndicator:YES
     minShowTime:0];

    [sdk RegisterNewUser:self.txtIdentity.text devName:self.txtDevName.text];
}

- ( void )OnRegisterNewUserCompleted:( id )sender user:( const id<IUser>)user
{
    switch ( [user getState] )
    {
    case STARTED_REGISTRATION:
    {
        UIStoryboard *storyboard =
            [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
        ConfirmEmailViewController *cevc = (ConfirmEmailViewController *)
                                           [storyboard instantiateViewControllerWithIdentifier:
                                            @"ConfirmEmailViewController"];
        cevc.iuser = user;
        [self.navigationController pushViewController:cevc animated:YES];
    } break;

    case ACTIVATED:
        currentUser = user;
        [sdk FinishRegistration:user];
        break;

    default:
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"ERROR_UNEXPECTED_USER_STATE", @"")
         addActivityIndicator:NO
         minShowTime:0];
        break;
    }

    NSArray *users = [MPin listUsers];
    int index = 0;
    for (; index < [users count]; index++ )
    {
        id<IUser> cUser = [users objectAtIndex:index];
        if ( [[cUser getIdentity] isEqualToString:[user getIdentity]] )
            break;
    }

    ConfigurationManager *cf = [ConfigurationManager sharedManager];
    [cf setSelectedUserForCurrentConfiguration:( index )];
}

- ( void )OnRegisterNewUserError:( id )sender error:( NSError * )error
{
    MpinStatus *mpinStatus = [error.userInfo objectForKey:kMPinSatus];
    [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(mpinStatus.statusCodeAsString, @"SERVER ERROR") addActivityIndicator:NO hideAfter:3];
}

- ( void )OnFinishRegistrationCompleted:( id )sender user:( const id<IUser>)user
{
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    IdentityCreatedViewController *vcIDCreated = (IdentityCreatedViewController *)
                                                 [storyboard instantiateViewControllerWithIdentifier:
                                                  @"IdentityCreatedViewController"];
    vcIDCreated.strEmail = [user getIdentity];
    vcIDCreated.user = user;
    [self.navigationController pushViewController:vcIDCreated animated:YES];
}

- ( void )OnFinishRegistrationError:( id )sender error:( NSError * )error
{
    if ( error.code == IDENTITY_NOT_VERIFIED )
    {
        UIStoryboard *storyboard =
            [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
        ConfirmEmailViewController *cevc = (ConfirmEmailViewController *)[storyboard
                                                                          instantiateViewControllerWithIdentifier:@"ConfirmEmailViewController"];
        cevc.iuser = [error.userInfo objectForKey:kUSER];
        [self.navigationController pushViewController:cevc animated:YES];
    }
    else
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:error.description addActivityIndicator:NO minShowTime:3];
    }
}

- ( BOOL )textFieldShouldReturn:( UITextField * )textField
{
    [textField resignFirstResponder];

    return YES;
}

- ( BOOL )textFieldShouldBeginEditing:( UITextField * )textField
{
    return YES;
}

- ( BOOL )isValidEmail:( NSString * )emailString
{
    if ( [emailString length] == 0 ||
         [emailString rangeOfString:@" "].location != NSNotFound )
    {
        return NO;
    }

    NSString *regExPattern = @"[A-Z0-9a-z._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}";

    NSRegularExpression *regEx = [[NSRegularExpression alloc]
                                  initWithPattern:regExPattern
                                  options:NSRegularExpressionCaseInsensitive
                                  error:nil];
    NSUInteger regExMatches =
        [regEx numberOfMatchesInString:emailString
         options:0
         range:NSMakeRange(0, [emailString length])];

    if ( regExMatches == 0 )
    {
        return NO;
    }
    else
    {
        return YES;
    }
}

- ( IBAction )back:( id )sender
{
    [self.navigationController popViewControllerAnimated:YES];
}

@end
