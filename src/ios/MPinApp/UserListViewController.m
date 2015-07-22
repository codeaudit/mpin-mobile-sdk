//
//  UserListViewController.m
//  MPinApp
//
//  Created by Georgi Georgiev on 11/19/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//


#import "IUser.h"
#import "Constants.h"
#import "MFSideMenu.h"
#import "UIViewController+Helper.h"
#import "MPin+AsyncOperations.h"

#pragma mark - import viewcontrollers -
#import "AccountSummaryViewController.h"
#import "IdentityCreatedViewController.h"
#import "ConfirmEmailViewController.h"
#import "UserListViewController.h"
#import "OTPViewController.h"
#import "IdentityBlockedViewController.h"
#import "MenuViewController.h"

#pragma mark - import managers -
#import "ThemeManager.h"
#import "SettingsManager.h"
#import "ConfigurationManager.h"
#import "SettingsManager.h"

@import LocalAuthentication;

#define ON_LOGOUT 403
#define LOGOUT_BUTTON_INDEX 1
#define DELETE_TAG 204
#define DELETE_BUTTON_INDEX 1
#define RESETPIN_TAG 208
#define RESETPIN_BUTTON_INDEX 1
#define NOT_SELECTED -1
#define NOT_SELECTED_SEC 0


static NSString *const constStrConnectionTimeoutNotification = @"ConnectionTimeoutNotification";

static NSString *const kSettings = @"settings";
static NSString *const kCurrentSelectionIndex = @"currentSelectionIndex";

static NSString *const kOTP = @"OTP";
static NSString *const kAN = @"AN";

@implementation UserListTableViewCell
@end

@interface UserListViewController ( )
{
    NSIndexPath *selectedIndexPath;
    id<IUser> currentUser;
    ThemeManager *themeManager;
    MPin *sdk;
    BOOL boolFirstTime;
    BOOL boolShouldAskForFingerprint;
    UIStoryboard *storyboard;
    NSString *storedBackendURL;
}
@property ( nonatomic, weak ) IBOutlet NSLayoutConstraint *constraintMenuHeight;

- ( void )showBottomBar:( BOOL )animated;
- ( void )hideBottomBar:( BOOL )animated;
- ( void )startAuthenticationFlow;

- ( void )showPinPad;

- ( IBAction )btnAddIDTap:( id )sender;
- ( IBAction )btnEditTap:( id )sender;
- ( IBAction )btnDeleteTap:( id )sender;
- ( IBAction )btnAuthTap:( id )sender;
- ( IBAction )onResetPinButtonClicked:( id )sender;

- ( void )deleteSelectedUser;

@end

@implementation UserListViewController

- ( instancetype )initWithCoder:( NSCoder * )coder
{
    self = [super initWithCoder:coder];
    if ( self )
    {
        selectedIndexPath = [NSIndexPath indexPathForRow:NOT_SELECTED inSection:NOT_SELECTED_SEC];
    }

    return self;
}

- ( void )viewDidLoad
{
    [super viewDidLoad];

    boolFirstTime = YES;
    boolShouldAskForFingerprint = NO;
    self.automaticallyAdjustsScrollViewInsets = NO;
    storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    [self hideBottomBar:NO];
    [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"Initializing" addActivityIndicator:YES minShowTime:0];
    storedBackendURL = [[ConfigurationManager sharedManager] getSelectedConfiguration][@"backend"];
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];

    sdk = [[MPin alloc] init];
    sdk.delegate = self;
    NSString *config = [[ConfigurationManager sharedManager] getSelectedConfiguration][@"backend"];
    
    // Executed if for some reason backend url is changed in other controllers
    // For example - QR Scanned backends can overwrite the selected backend and the url maybe will be different
    if (![storedBackendURL isEqualToString:config])
    {
        [sdk SetBackend:[[ConfigurationManager sharedManager] getSelectedConfiguration]];
    }
    
    [self.menuContainerViewController setPanMode:MFSideMenuPanModeDefault];
    [[ThemeManager sharedManager] beautifyViewController:self];
    self.users = [MPin listUsers];
    [(MenuViewController *)self.menuContainerViewController.leftMenuViewController setConfiguration];
    [[NSNotificationCenter defaultCenter] removeObserver:self
     name:kShowPinPadNotification
     object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( showPinPad ) name:kShowPinPadNotification object:nil];
}

- ( void )viewDidAppear:( BOOL )animated
{
    if ( [self.users count] == 0 )
    {
        [self hideBottomBar:NO];
        [self.table reloadData];

        return;
    }
    [self.table reloadData];

    selectedIndexPath = [NSIndexPath indexPathForRow:NOT_SELECTED inSection:NOT_SELECTED_SEC];
    ConfigurationManager *cf = [ConfigurationManager sharedManager];
    NSInteger nSelectedUserIndex = [cf getSelectedUserIndexforSelectedConfiguration];
    if ( nSelectedUserIndex >= 0 )
    {
        currentUser = self.users [nSelectedUserIndex];
        selectedIndexPath = [NSIndexPath indexPathForRow:nSelectedUserIndex inSection:NOT_SELECTED_SEC];
        [self showBottomBar:YES];
    }
    else
    {
        [self hideBottomBar:NO];
    }
}

-( void ) viewWillDisappear:( BOOL )animated
{
    [super viewWillDisappear:animated];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}

- ( void ) invalidate
{
    self.users = [MPin listUsers];

    if ( [self.users count] == 0 )
    {
        [self hideBottomBar:NO];
    }

    ConfigurationManager *cf = [ConfigurationManager sharedManager];
    NSInteger nSelectedUserIndex = [cf getSelectedUserIndexforSelectedConfiguration];
    if ( nSelectedUserIndex >= 0 && self.users && [self.users count] )
    {
        selectedIndexPath = [NSIndexPath indexPathForRow:nSelectedUserIndex inSection:NOT_SELECTED_SEC];
        if ( self.users.count > selectedIndexPath.row )
        {
            currentUser = ( self.users ) [selectedIndexPath.row];
        }
        else
        {
            currentUser = ( self.users ) [0];
        }

        [self showBottomBar:NO];
        [self startAuthenticationFlow];
    }
    else
    {
        [self hideBottomBar:NO];
    }
    [self.table reloadData];
    [[ErrorHandler sharedManager] hideMessage];
}

- ( void )showBottomBar:( BOOL )animated
{
    _constraintMenuHeight.constant = 54;
    if ( animated )
    {
        [UIView animateWithDuration:.25 animations: ^ {
            [self.view layoutIfNeeded];
        }];
    }
    else
    {
        [self.view layoutIfNeeded];
    }
}

- ( void )hideBottomBar:( BOOL )animated
{
    _constraintMenuHeight.constant = 0;
    if ( animated )
    {
        [UIView animateWithDuration:.25 animations: ^ {
            [self.view layoutIfNeeded];
        }
        ];
    }
    else
    {
        [self.view layoutIfNeeded];
    }
}

- ( void ) requestAuth
{
    if ( boolFirstTime )
    {
        LAContext *context = [[LAContext alloc] init];
        [context evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics localizedReason:@"Please, authorize yourself" reply:
         ^ (BOOL success, NSError *authenticationError) {
            boolFirstTime = !success;
            if ( !success )
            {
                [self requestAuth];
            }
        }
        ];
    }
}

#pragma mark - Table view delegate -

- ( NSInteger )tableView:( UITableView * )tableView numberOfRowsInSection:( NSInteger )section
{
    return self.users.count;
}

- ( UITableViewCell * )tableView:( UITableView * )tableView cellForRowAtIndexPath:( NSIndexPath * )indexPath
{
    static NSString *userListTableIdentifier = @"UserListTableViewCell";
    UserListTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:userListTableIdentifier];
    if ( cell == nil )
    {
        cell = [[UserListTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:userListTableIdentifier];
    }
    cell.lblUserID.font = [UIFont fontWithName:@"OpenSans" size:14.f];
    cell.lblUserID.textColor = [[SettingsManager sharedManager] color6];

    return cell;
}

- ( void )tableView:( UITableView * )tableView willDisplayCell:( UITableViewCell * )cell forRowAtIndexPath:( NSIndexPath * )indexPath
{
    UserListTableViewCell *c = (UserListTableViewCell *)cell;
    id<IUser> iuser = ( self.users ) [indexPath.row];
    c.lblUserID.text = [iuser getIdentity];

    if ( ( selectedIndexPath != nil ) && ( indexPath.row == selectedIndexPath.row ) )
    {
        c.imgViewSelected.image = [UIImage imageNamed:@"checked"];
    }
    else
    {
        c.imgViewSelected.image = [UIImage imageNamed:@"pin-dot-empty"];
    }

    switch ( [iuser getState] )
    {
    case INVALID:
        c.imgViewUser.image = [UIImage imageNamed:@"avatar-list-unregistered"];
        break;

    case STARTED_REGISTRATION:
        c.imgViewUser.image = [UIImage imageNamed:@"avatar-list-unregistered"];
        break;

    case ACTIVATED:
        c.imgViewUser.image = [UIImage imageNamed:@"avatar-list-unregistered"];
        break;

    case REGISTERED:
        c.imgViewUser.image = [UIImage imageNamed:@"avatar-list-registered"];
        break;

    default:
        c.imgViewUser.image = [UIImage imageNamed:@"avatar-list-unregistered"];
        break;
    }
}

- ( void )tableView:( UITableView * )tableView didSelectRowAtIndexPath:( NSIndexPath * )indexPath
{
    if ( [selectedIndexPath row] == [indexPath row] )
    {
        UserListTableViewCell *prevCell = (UserListTableViewCell *)[tableView cellForRowAtIndexPath:selectedIndexPath];
        prevCell.imgViewSelected.image = [UIImage imageNamed:@"pin-dot-empty"];
        selectedIndexPath = [NSIndexPath indexPathForRow:NOT_SELECTED inSection:NOT_SELECTED_SEC];
        [self hideBottomBar:YES];

        return;
    }

    if ( selectedIndexPath.row == NOT_SELECTED )
    {
        [self showBottomBar:YES];
    }
    else
    {
        UserListTableViewCell *prevCell = (UserListTableViewCell *)[tableView cellForRowAtIndexPath:selectedIndexPath];
        prevCell.imgViewSelected.image = [UIImage imageNamed:@"pin-dot-empty"];
    }
    currentUser = ( self.users ) [indexPath.row];
    selectedIndexPath = indexPath;
    UserListTableViewCell *cell = (UserListTableViewCell *)[tableView cellForRowAtIndexPath:indexPath];
    [self showBottomBar:YES];
    cell.imgViewSelected.image = [UIImage imageNamed:@"checked"];
}

#pragma mark - AN Delegate -

-( void ) onAccessNumber:( NSString * ) an
{}

#pragma mark - SDK Handlers -
- ( void )OnFinishRegistrationCompleted:( id )sender user:( const id<IUser>)user
{
    IdentityCreatedViewController *vcIDCreated = (IdentityCreatedViewController *)[storyboard instantiateViewControllerWithIdentifier:@"IdentityCreatedViewController"];
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
        ConfirmEmailViewController *cevc = (ConfirmEmailViewController *)[storyboard instantiateViewControllerWithIdentifier:@"ConfirmEmailViewController"];
        cevc.iuser = ( error.userInfo ) [kUSER];
        [self.navigationController pushViewController:cevc animated:YES];
    }
    break;

    case HTTP_SERVER_ERROR:
        [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"Server error" addActivityIndicator:NO minShowTime:3];

    default:
        break;
    }
}

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
    OTPViewController *otpViewController = [storyboard instantiateViewControllerWithIdentifier:@"OTP"];
    otpViewController.otpData = otp;
    otpViewController.strEmail = [user getIdentity];
    [self.navigationController pushViewController:otpViewController animated:YES];
}

- ( void )OnAuthenticateOTPError:( id )sender error:( NSError * )error
{
    MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:NSLocalizedString(mpinStatus.statusCodeAsString, @"UNKNOWN ERROR")
     addActivityIndicator:NO
     minShowTime:0];
}

- ( void )OnAuthenticateAccessNumberCompleted:( id )sender user:( id<IUser>)user
{
    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:NSLocalizedString(@"HUD_AUTH_SUCCESS", @"")
     addActivityIndicator:NO
     minShowTime:0];
}

- ( void )OnAuthenticateAccessNumberError:( id )sender error:( NSError * )error
{
    switch ( error.code )
    {
    case INCORRECT_PIN:
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:@"Wrong MPIN or Access Number!"
         addActivityIndicator:NO
         minShowTime:3];
        break;

    case HTTP_REQUEST_ERROR:
        [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"Wrong MPIN or Access Number!"
         addActivityIndicator:NO
         minShowTime:3];
        break;

    default:
    {
        MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
        [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:NSLocalizedString(mpinStatus.statusCodeAsString, @"UNKNOWN ERROR")
         addActivityIndicator:NO
         minShowTime:0];
    } break;
    }
}

- ( void )OnAuthenticateCompleted:( id )sender user:( const id<IUser>)user
{
    [[ErrorHandler sharedManager] hideMessage];
    AccountSummaryViewController *vcAccountSummary = [storyboard instantiateViewControllerWithIdentifier:@"AccountSummary"];
    vcAccountSummary.strEmail = [currentUser getIdentity];
    [self.navigationController pushViewController:vcAccountSummary animated:YES];
}

- ( void ) OnAuthenticateCanceled
{
    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:@"TouchID failed"
     addActivityIndicator:NO
     minShowTime:3];
}

- ( void )OnAuthenticateError:( id )sender error:( NSError * )error
{
    MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
    id<IUser> iuser = ( self.users ) [selectedIndexPath.row];
    if ( [iuser getState] == BLOCKED )
    {
        IdentityBlockedViewController *identityBlockedViewController = [storyboard  instantiateViewControllerWithIdentifier:@"IdentityBlockedViewController"];
        identityBlockedViewController.strUserEmail = [iuser getIdentity];
        identityBlockedViewController.iuser = iuser;
        [self.navigationController pushViewController:identityBlockedViewController animated:YES];
    }
    else
    {
        switch ( error.code )
        {
        case INCORRECT_PIN:
            [[ErrorHandler sharedManager] presentMessageInViewController:self
             errorString:@"Wrong MPIN"
             addActivityIndicator:NO
             minShowTime:0];
            break;

        default:
            [[ErrorHandler sharedManager] presentMessageInViewController:self
             errorString:mpinStatus.errorMessage
             addActivityIndicator:NO
             minShowTime:0];
            break;
        }
    }
}

- ( void )deleteSelectedUser
{
    [self hideBottomBar:YES];
    id<IUser> iuser = ( self.users ) [selectedIndexPath.row];
    [MPin DeleteUser:iuser];
    [self.users removeObjectAtIndex:selectedIndexPath.row];
    selectedIndexPath = [NSIndexPath indexPathForRow:NOT_SELECTED inSection:NOT_SELECTED_SEC];
    [[ConfigurationManager sharedManager] setSelectedUserForCurrentConfiguration:NOT_SELECTED];
    [self.table reloadData];
}

#pragma mark - My actions -

- ( IBAction )btnAddIDTap:( id )sender
{
    if ( [[ConfigurationManager sharedManager] isEmpty] )
        return;

    UIViewController *addViewController = [storyboard instantiateViewControllerWithIdentifier:@"Add"];
    [self.navigationController pushViewController:addViewController animated:YES];
}

- ( IBAction )btnEditTap:( id )sender
{}

- ( IBAction )btnDeleteTap:( id )sender
{
    id<IUser> iuser = ( self.users ) [selectedIndexPath.row];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"KEY_DELETE", @"")
                          message:[NSString stringWithFormat:NSLocalizedString(@"WARNING_USER_WILL_BE_DELETED", @""), [iuser getIdentity]]
                          delegate:self
                          cancelButtonTitle:NSLocalizedString(@"KEY_CANCEL", @"")
                          otherButtonTitles:NSLocalizedString(@"KEY_DELETE", @""),
                          nil];
    alert.tag = DELETE_TAG;
    [alert show];
}

-( IBAction )onResetPinButtonClicked:( id )sender
{
    id<IUser> iuser = ( self.users ) [selectedIndexPath.row];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"RESET_PIN"
                          message:[NSString stringWithFormat:@"Are you sure that you would like to reset pin of \"%@\" ?", [iuser getIdentity]]
                          delegate:self
                          cancelButtonTitle:@"CANCEL"
                          otherButtonTitles:@"RESET",
                          nil];
    alert.tag = RESETPIN_TAG;
    [alert show];
}

- ( IBAction )btnAuthTap:( id )sender
{
    [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"" addActivityIndicator:YES minShowTime:0];
    [[ConfigurationManager sharedManager] setSelectedUserForCurrentConfiguration:selectedIndexPath.row];
    [self startAuthenticationFlow];
}

- ( void )startAuthenticationFlow
{
    NSIndexPath *path = selectedIndexPath;
    if ( [path row] < [self.users count] )
    {
        id<IUser> iuser = ( self.users ) [[path row]];
        NSDictionary *config;
        enum SERVICES s;
        IdentityBlockedViewController   *identityBlockedViewController;
        AccessNumberViewController      *accessViewController;

        switch ( [iuser getState] )
        {
        case INVALID:
            [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"HUD_REACTIVATE_USER", @"") addActivityIndicator:NO hideAfter:3];
            break;

        case STARTED_REGISTRATION:
            [sdk FinishRegistration:iuser];
            break;

        case ACTIVATED:
            [sdk FinishRegistration:iuser];
            break;

        case REGISTERED:
            config = [[ConfigurationManager sharedManager] getSelectedConfiguration];
            s = [config [kSERVICE_TYPE] intValue];



            switch ( s )
            {
            case LOGIN_ON_MOBILE:
                [sdk Authenticate:iuser askForFingerprint:boolShouldAskForFingerprint];
                break;

            case LOGIN_ONLINE:
                [[ErrorHandler sharedManager] hideMessage];
                accessViewController = [storyboard instantiateViewControllerWithIdentifier:@"accessnumber"];
                accessViewController.delegate = self;
                sdk.delegate = nil;
                accessViewController.strEmail = [currentUser getIdentity];
                accessViewController.currentUser = currentUser;
                [self.navigationController pushViewController:accessViewController animated:YES];
                break;

            case LOGIN_WITH_OTP:
                [sdk AuthenticateOTP:iuser askForFingerprint:boolShouldAskForFingerprint];
                break;
            }
            break;

        case BLOCKED:

            [[ErrorHandler sharedManager] hideMessage];
            identityBlockedViewController = [storyboard instantiateViewControllerWithIdentifier:@"IdentityBlockedViewController"];
            identityBlockedViewController.strUserEmail = [iuser getIdentity];
            identityBlockedViewController.iuser = iuser;
            [self.navigationController pushViewController:identityBlockedViewController animated:YES];
            break;

        default:
            [[ErrorHandler sharedManager] presentMessageInViewController:self
             errorString:NSLocalizedString(@"HUD_UNSUPPORTED_ACTION", @"")
             addActivityIndicator:NO
             minShowTime:0];
            break;
        }
    }
    else
    {
        [self hideBottomBar:NO];
    }
}

- ( IBAction )showLeftMenuPressed:( id )sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

- ( void )OnRegisterNewUserCompleted:( id )sender user:( const id<IUser>)user
{
    [[ErrorHandler sharedManager] hideMessage];
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
        currentUser = user;
        [sdk FinishRegistration:user];
        break;

    default:
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:[NSString stringWithFormat:@"User state is unexpected %ld",                                                                                       (long)[user getState]]
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
    [[ErrorHandler sharedManager] hideMessage];
    MpinStatus *mpinStatus = [error.userInfo objectForKey:kMPinSatus];
    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:mpinStatus.errorMessage
     addActivityIndicator:NO
     minShowTime:0];
}

#pragma mark - Alert view delegate -


- ( void )alertView:( UIAlertView * )alertView clickedButtonAtIndex:( NSInteger )buttonIndex
{
    if ( ( alertView.tag == ON_LOGOUT ) && ( buttonIndex == LOGOUT_BUTTON_INDEX ) )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"HUD_LOGOUT", @"")
         addActivityIndicator:NO
         minShowTime:0];


        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
            BOOL isSuccessful = [MPin Logout:( self.users ) [selectedIndexPath.row]];
            dispatch_async(dispatch_get_main_queue(), ^ (void) {
                [[ErrorHandler sharedManager] hideMessage];
                NSString *descritpion = ( isSuccessful ) ? NSLocalizedString(@"HUD_LOGOUT_OK", @"") : NSLocalizedString(@"HUD_LOGOUT_NOT_OK", @"");
                [[ErrorHandler sharedManager] presentMessageInViewController:self
                 errorString:descritpion
                 addActivityIndicator:NO
                 minShowTime:0];
            }
                           );
        }
                       );

        return;
    }
    else
    if ( ( alertView.tag == DELETE_TAG ) && ( buttonIndex == DELETE_BUTTON_INDEX ) )
    {
        [self deleteSelectedUser];
    }
    else
    if ( ( alertView.tag == RESETPIN_TAG ) && ( buttonIndex == RESETPIN_BUTTON_INDEX ) )
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self errorString:@"" addActivityIndicator:YES minShowTime:0];
        id<IUser> iuser = ( self.users ) [selectedIndexPath.row];
        NSString *userID = [iuser getIdentity];
        [self deleteSelectedUser];
        ConfigurationManager *cfm = [ConfigurationManager sharedManager];
        [sdk RegisterNewUser:userID devName:[cfm getDeviceName]];
    }
}

#pragma mark - Notifications handlers -

- ( void )showPinPad
{
    [[ErrorHandler sharedManager] hideMessage];
    PinPadViewController *pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.sdk = sdk;
    pinpadViewController.sdk.delegate = pinpadViewController;
    pinpadViewController.currentUser = currentUser;
    pinpadViewController.boolShouldShowBackButton = YES;
    pinpadViewController.title = kEnterPin;
    switch ( [currentUser getState] )
    {
    case REGISTERED:
        pinpadViewController.boolSetupPin = NO;
        break;

    case STARTED_REGISTRATION:
        pinpadViewController.boolSetupPin = YES;
        break;

    default:
        break;
    }
    NSLog(@"Calling PinPad from UserList");
    [self.navigationController pushViewController:pinpadViewController animated:YES];
}

@end
