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



#import "MFSideMenu.h"
#import "IdentityBlockedViewController.h"
#import "IdentityCreatedViewController.h"
#import "ThemeManager.h"
#import "PinPadViewController.h"
#import "ConfirmEmailViewController.h"
#import "ConfigurationManager.h"
#import "UIViewController+Helper.h"

#define RESETPIN_TAG 208
#define RESETPIN_BUTTON_INDEX 1

@interface IdentityBlockedViewController ( ) {
    MPin *sdk;
    UIStoryboard *storyboard;
}

- ( void )showPinPad;
- ( void ) deleteUser;

- ( IBAction )showLeftMenuPressed:( id )sender;
- ( IBAction )btnGoToIdListPressed:( id )sender;
- ( IBAction )btnDeleteIdPressed:( id )sender;

@end

@implementation IdentityBlockedViewController


- ( void ) deleteUser
{
    [MPin DeleteUser:_iuser];
    [[ConfigurationManager sharedManager] setSelectedUserForCurrentConfiguration:NOT_SELECTED];
}

- ( void )viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];


    [[ThemeManager sharedManager] beautifyViewController:self];
}

-( void ) viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];

    sdk = [[MPin alloc] init];
    sdk.delegate = self;

    if ( _strUserEmail != nil )
    {
        _lblUserEmail.text = _strUserEmail;
    }
    else
    {
        _lblUserEmail.text = @"";
    }

    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( showPinPad ) name:kShowPinPadNotification object:nil];
}

- ( void )viewDidDisappear:( BOOL )animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}

- ( void )didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- ( IBAction )showLeftMenuPressed:( id )sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

- ( IBAction )btnGoToIdListPressed:( id )sender
{
    [self.navigationController popToRootViewControllerAnimated:YES];
}

- ( IBAction )btnDeleteIdPressed:( id )sender
{
    [[[UIAlertView alloc] initWithTitle:@"REMOVE IDENTITY" message:@"This action will remove the identity permanently.  Are you sure?" delegate:self cancelButtonTitle:@"NO" otherButtonTitles:@"YES", nil] show];
}

#pragma mark - Alert view delegate -

- ( void )alertView:( UIAlertView * )alertView didDismissWithButtonIndex:( NSInteger )buttonIndex
{
    if ( ( alertView.tag == RESETPIN_TAG ) && ( buttonIndex == RESETPIN_BUTTON_INDEX ) )
    {
        NSString *userID = [self.iuser getIdentity];
        [self deleteUser];
        ConfigurationManager *cfm = [ConfigurationManager sharedManager];
        [sdk RegisterNewUser:userID devName:[cfm getDeviceName]];
    }
    else
    {
        if ( buttonIndex == 1 )
        {
            [self deleteUser];
            [self.navigationController popToRootViewControllerAnimated:YES];
        }
    }
}

- ( void ) OnFinishRegistrationCompleted:( id ) sender user:( const id<IUser>) user
{
    IdentityCreatedViewController *vcIDCreated = (IdentityCreatedViewController *)[storyboard instantiateViewControllerWithIdentifier:@"IdentityCreatedViewController"];
    vcIDCreated.user = self.iuser;
    vcIDCreated.strEmail = [user getIdentity];
    [self.navigationController pushViewController:vcIDCreated animated:YES];
}

- ( void ) OnFinishRegistrationError:( id ) sender error:( NSError * ) error
{
    if ( error.code == IDENTITY_NOT_VERIFIED )
    {
        ConfirmEmailViewController *cevc = (ConfirmEmailViewController *)[storyboard instantiateViewControllerWithIdentifier:@"ConfirmEmailViewController"];
        cevc.iuser = ( error.userInfo ) [kUSER];
        [self.navigationController pushViewController:cevc animated:YES];
    }
    else
    {
        // TODO:
    }
}

-( IBAction )onResetPinButtonClicked:( id )sender
{
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"KEY_RESET", @"RESET PIN")
                          message:[NSString stringWithFormat:NSLocalizedString(@"BLOCKED_ID_RESET_PIN_CONFIRM", @"Are you sure that you would like to reset pin of \"%@\" ?"), [self.iuser getIdentity]]
                          delegate:self
                          cancelButtonTitle:NSLocalizedString(@"KEY_CANCEL", @"CANCEL")
                          otherButtonTitles:NSLocalizedString(@"KEY_RESET", @"RESET PIN"),
                          nil];
    alert.tag = RESETPIN_TAG;
    [alert show];
}

- ( void )OnRegisterNewUserCompleted:( id )sender user:( const id<IUser>)user
{
    switch ( [user getState] )
    {
    case STARTED_REGISTRATION:
    {
        ConfirmEmailViewController *cevc = (ConfirmEmailViewController *)
                                           [storyboard instantiateViewControllerWithIdentifier:
                                            @"ConfirmEmailViewController"];
        cevc.iuser = user;
        [self.navigationController pushViewController:cevc animated:YES];
    } break;

    case ACTIVATED:
        self.iuser = user;
        [sdk FinishRegistration:user];
        break;

    default:
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:[NSString stringWithFormat:@"User state is unexpected %ld",[user getState]]
         addActivityIndicator:NO
         minShowTime:0
        ];
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
    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:mpinStatus.errorMessage
     addActivityIndicator:NO
     minShowTime:0];
}

- ( void )showPinPad
{
    PinPadViewController *pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.currentUser = self.iuser;
    pinpadViewController.boolShouldShowBackButton = YES;
    pinpadViewController.title = kEnterPin;
    [self.navigationController pushViewController:pinpadViewController animated:YES];
}

@end
