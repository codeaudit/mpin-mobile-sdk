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

static NSString* const kSettings = @"settings";
static NSString* const kCurrentSelectionIndex = @"currentSelectionIndex";

static NSString* const kOTP = @"OTP";
static NSString* const kAN = @"AN";

@implementation UserListTableViewCell
@end

@interface UserListViewController () {
    NSIndexPath* selectedIndexPath;
    BOOL boolIsInitialised;
    id<IUser> currentUser;
    ThemeManager* themeManager;
    MPin* sdk;
    BOOL boolFirstTime;
    UIStoryboard* storyboard;
}
@property (nonatomic, weak) IBOutlet NSLayoutConstraint* constraintLeadingSpace;
@property (nonatomic, weak) IBOutlet NSLayoutConstraint* constraintTrailingSpace;
@property (nonatomic, weak) IBOutlet NSLayoutConstraint* constraintBottomSpace;

- (void)showBottomBar:(BOOL)animated;
- (void)hideBottomBar:(BOOL)animated;
- (void)starAuthenticationFlow;

- (void)showPinPad;

- (IBAction)btnAddIDTap:(id)sender;
- (IBAction)btnEditTap:(id)sender;
- (IBAction)btnDeleteTap:(id)sender;
- (IBAction)btnAuthTap:(id)sender;
- (IBAction)onResetPinButtonClicked:(id)sender;

- (void)deleteSelectedUser;

@end

@implementation UserListViewController

- (instancetype)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        selectedIndexPath = [NSIndexPath indexPathForRow:NOT_SELECTED inSection:NOT_SELECTED_SEC];
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    boolFirstTime = YES;

    self.automaticallyAdjustsScrollViewInsets = NO;
    storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    boolIsInitialised = NO;

    [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                   errorString:NSLocalizedString(@"HUD_CHANGE_CONFIGURATION", @"")
                                          addActivityIndicator:NO
                                             autoHideInSeconds:0];
    
    [[ThemeManager sharedManager] beautifyViewController:self];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        MpinStatus* status = [MPin initWithConfig:[[ConfigurationManager sharedManager] getSelectedConfiguration]];
        dispatch_async(dispatch_get_main_queue(), ^(void) {
            if (status.status != OK) {
                [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                               errorString:status.errorMessage
                                                      addActivityIndicator:NO
                                                         autoHideInSeconds:0];
            }
            self.users = [MPin listUsers];
            
            if ([self.users count] == 0)
            {
                [self hideBottomBar:NO];
            }

            ConfigurationManager *cf = [ConfigurationManager sharedManager];
            NSInteger nSelectedUserIndex = [cf getSelectedUserIndexforSelectedConfiguration];
            if (nSelectedUserIndex >=0) {
                selectedIndexPath = [NSIndexPath indexPathForRow:nSelectedUserIndex inSection:NOT_SELECTED_SEC];
                if (self.users.count > selectedIndexPath.row)
                {
                    currentUser = (self.users)[selectedIndexPath.row];
                }
                else
                {
                    currentUser = (self.users)[0];
                }
                
                [self showBottomBar:NO];
                [self starAuthenticationFlow];
            } else  [self hideBottomBar:NO];
            
            [self.table reloadData];
        });
    });


    sdk = [[MPin alloc] init];
    sdk.delegate = self;
}

- (void)showBottomBar:(BOOL)animated
{
    _constraintLeadingSpace.constant = 0;
    _constraintTrailingSpace.constant = 0;
    _constraintBottomSpace.constant = 0;
    if (animated) {
        [UIView animateWithDuration:.25 animations:^{
            [self.view layoutIfNeeded];
        }];
    }
    else {
        [self.view layoutIfNeeded];
    }
}

- (void)hideBottomBar:(BOOL)animated
{
    _constraintLeadingSpace.constant = self.navigationController.navigationBar.frame.size.width / 2;
    _constraintTrailingSpace.constant = self.navigationController.navigationBar.frame.size.width / 2;
    _constraintBottomSpace.constant = -54;
    if (animated) {
        [UIView animateWithDuration:.25 animations:^{
            [self.view layoutIfNeeded];
        }];
    }
    else {
        [self.view layoutIfNeeded];
    }
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];

    [self.menuContainerViewController setPanMode:MFSideMenuPanModeDefault];
    [[ThemeManager sharedManager] beautifyViewController:self];
    [[ErrorHandler sharedManager] stopLoading];
    self.users = [MPin listUsers];
}

- (void)viewDidAppear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showPinPad) name:kShowPinPadNotification object:nil];
    
    if ([self.users count] == 0) {
        [self hideBottomBar:NO];
        [self.table reloadData];
        return;
    }
    [self.table reloadData];
    selectedIndexPath = [NSIndexPath indexPathForRow:NOT_SELECTED inSection:NOT_SELECTED_SEC];
    ConfigurationManager* cf = [ConfigurationManager sharedManager];
    NSInteger nSelectedUserIndex = [cf getSelectedUserIndexforSelectedConfiguration];
    if (nSelectedUserIndex >= 0) {
        selectedIndexPath = [NSIndexPath indexPathForRow:nSelectedUserIndex inSection:NOT_SELECTED_SEC];
        [self showBottomBar:NO];
    }
    else
        [self hideBottomBar:NO];
    
}
- (void)viewDidDisappear:(BOOL)animated
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}

- (void) requestAuth
{
    if (boolFirstTime)
    {
        LAContext *context = [[LAContext alloc] init];
        [context evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics localizedReason:@"Please, authorize yourself" reply:
         ^(BOOL success, NSError *authenticationError) {
             boolFirstTime = !success;
             if (!success)
             {
                 [self requestAuth];
             }
         }];
    }
}
#pragma mark - Table view delegate -

- (NSInteger)tableView:(UITableView*)tableView numberOfRowsInSection:(NSInteger)section
{
    return self.users.count;
}

- (UITableViewCell*)tableView:(UITableView*)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
    static NSString* userListTableIdentifier = @"UserListTableViewCell";
    UserListTableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:userListTableIdentifier];
    if (cell == nil)
        cell = [[UserListTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:userListTableIdentifier];
    cell.lblUserID.font = [UIFont fontWithName:@"OpenSans" size:14.f];
    cell.lblUserID.textColor = [[SettingsManager sharedManager] color6];
    return cell;
}

- (void)tableView:(UITableView*)tableView willDisplayCell:(UITableViewCell*)cell forRowAtIndexPath:(NSIndexPath*)indexPath
{
    UserListTableViewCell* c = (UserListTableViewCell*)cell;
    id<IUser> iuser = (self.users)[indexPath.row];
    c.lblUserID.text = [iuser getIdentity];

    if ((selectedIndexPath != nil) && (indexPath.row == selectedIndexPath.row)) {
        c.imgViewSelected.image = [UIImage imageNamed:@"checked"];
    }
    else {
        c.imgViewSelected.image = [UIImage imageNamed:@"pin-dot-empty"];
    }

    switch ([iuser getState]) {
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

- (void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
    if (selectedIndexPath.row == indexPath.row) {

        UserListTableViewCell* prevCell = (UserListTableViewCell*)[tableView cellForRowAtIndexPath:selectedIndexPath];
        prevCell.imgViewSelected.image = [UIImage imageNamed:@"pin-dot-empty"];
        selectedIndexPath = [NSIndexPath indexPathForRow:NOT_SELECTED inSection:NOT_SELECTED_SEC];
        [self hideBottomBar:YES];
        return;
    }

    if (selectedIndexPath.row == NOT_SELECTED) {
        [self showBottomBar:YES];
    }
    else {
        UserListTableViewCell* prevCell = (UserListTableViewCell*)[tableView cellForRowAtIndexPath:selectedIndexPath];
        prevCell.imgViewSelected.image = [UIImage imageNamed:@"pin-dot-empty"];
    }
    currentUser = (self.users)[indexPath.row];
    selectedIndexPath = indexPath;
    UserListTableViewCell* cell = (UserListTableViewCell*)[tableView cellForRowAtIndexPath:indexPath];
    cell.imgViewSelected.image = [UIImage imageNamed:@"checked"];
}

#pragma mark - SDK Handlers -
- (void)OnFinishRegistrationCompleted:(id)sender user:(const id<IUser>)user
{
    [[ErrorHandler sharedManager] stopLoading];
    IdentityCreatedViewController* vcIDCreated = (IdentityCreatedViewController*)[storyboard instantiateViewControllerWithIdentifier:@"IdentityCreatedViewController"];
    vcIDCreated.user = user;
    vcIDCreated.strEmail = [user getIdentity];
    [self.navigationController pushViewController:vcIDCreated animated:YES];
}

- (void)OnFinishRegistrationError:(id)sender error:(NSError*)error
{
    [[ErrorHandler sharedManager] stopLoading];
    if (error.code == IDENTITY_NOT_VERIFIED) {
        ConfirmEmailViewController* cevc = (ConfirmEmailViewController*)[storyboard instantiateViewControllerWithIdentifier:@"ConfirmEmailViewController"];
        cevc.iuser = (error.userInfo)[kUSER];
        [self.navigationController pushViewController:cevc animated:YES];
    }
    else {
        // TODO:
    }
}

- (void)OnAuthenticateOTPCompleted:(id)sender user:(id<IUser>)user otp:(OTP*)otp
{
    [[ErrorHandler sharedManager] stopLoading];

    if (otp.status.status != OK)
    {
        [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                       errorString:@"OTP is not supported!"
                                              addActivityIndicator:NO
                                                 autoHideInSeconds:0] ;
        return;
    }
    OTPViewController* otpViewController = [storyboard instantiateViewControllerWithIdentifier:@"OTP"];
    otpViewController.otpData = otp;
    otpViewController.strEmail = [user getIdentity];
    [[ErrorHandler sharedManager] stopLoading];
    [self.navigationController pushViewController:otpViewController animated:YES];
}

- (void)OnAuthenticateOTPError:(id)sender error:(NSError*)error
{
    [[ErrorHandler sharedManager] stopLoading];
    MpinStatus* mpinStatus = (error.userInfo)[kMPinSatus];
    [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                   errorString:mpinStatus.errorMessage
                                          addActivityIndicator:NO
                                             autoHideInSeconds:0];
}

-(void) onAccessNumber:(NSString *) an {
    [sdk AuthenticateAN:[self.users objectAtIndex:selectedIndexPath.row] accessNumber:an];
}

- (void)OnAuthenticateAccessNumberCompleted:(id)sender user:(id<IUser>)user
{
    [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                   errorString:NSLocalizedString(@"HUD_AUTH_SUCCESS", @"")
                                          addActivityIndicator:NO
                                             autoHideInSeconds:0];
}

- (void)OnAuthenticateAccessNumberError:(id)sender error:(NSError*)error
{
    [[ErrorHandler sharedManager] stopLoading];
    switch (error.code) {
    case INCORRECT_PIN:
        [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                       errorString:@"Wrong MPIN or Access Number!"
                                              addActivityIndicator:NO
                                                 autoHideInSeconds:0];
        break;
    case HTTP_REQUEST_ERROR:
        [[ErrorHandler sharedManager] presentErrorInViewController:self errorString:@"Wrong MPIN or Access Number!"
                                              addActivityIndicator:NO
                                                 autoHideInSeconds:0];
        break;
    default: {
        MpinStatus* mpinStatus = (error.userInfo)[kMPinSatus];
        [[ErrorHandler sharedManager] presentErrorInViewController:self errorString:mpinStatus.errorMessage
                                              addActivityIndicator:NO
                                                 autoHideInSeconds:0];
    } break;
    }
}

- (void)OnAuthenticateCompleted:(id)sender user:(const id<IUser>)user
{
    [[ErrorHandler sharedManager] stopLoading];

    AccountSummaryViewController* vcAccountSummary = [storyboard instantiateViewControllerWithIdentifier:@"AccountSummary"];
    vcAccountSummary.strEmail = [currentUser getIdentity];
    [self.navigationController pushViewController:vcAccountSummary animated:YES];
}

- (void) OnAuthenticateCanceled
{
    [[ErrorHandler sharedManager] stopLoading];
    [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                   errorString:@"TouchID failed"
                                          addActivityIndicator:NO
                                             autoHideInSeconds:0];
}

- (void)OnAuthenticateError:(id)sender error:(NSError*)error
{
    [[ErrorHandler sharedManager] stopLoading];
    MpinStatus* mpinStatus = (error.userInfo)[kMPinSatus];
    id<IUser> iuser = (self.users)[selectedIndexPath.row];
    if ([iuser getState] == BLOCKED)
    {
        IdentityBlockedViewController *identityBlockedViewController = [storyboard  instantiateViewControllerWithIdentifier:@"IdentityBlockedViewController"];
        identityBlockedViewController.strUserEmail = [iuser getIdentity];
        identityBlockedViewController.iuser = iuser;
        [self.navigationController pushViewController:identityBlockedViewController animated:YES];
    }
    else
    {
        switch (error.code)
        {
            case INCORRECT_PIN:
                [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                               errorString:@"Wrong MPIN"
                                                      addActivityIndicator:NO
                                                         autoHideInSeconds:0];
                break;
            default:
                [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                               errorString:mpinStatus.errorMessage
                                                      addActivityIndicator:NO
                                                         autoHideInSeconds:0];
                break;
        }
    }
}

- (void)alertView:(UIAlertView*)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if ((alertView.tag == ON_LOGOUT) && (buttonIndex == LOGOUT_BUTTON_INDEX)) {

        [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                       errorString:NSLocalizedString(@"HUD_LOGOUT", @"")
                                              addActivityIndicator:NO
                                                 autoHideInSeconds:0];
        

        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            BOOL isSuccessful = [MPin Logout:(self.users)[selectedIndexPath.row]];
            dispatch_async(dispatch_get_main_queue(), ^(void) {
                [[ErrorHandler sharedManager] hideError];
                NSString * descritpion = (isSuccessful)?NSLocalizedString(@"HUD_LOGOUT_OK", @""):NSLocalizedString(@"HUD_LOGOUT_NOT_OK", @"");
                [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                               errorString:descritpion
                                                      addActivityIndicator:NO
                                                         autoHideInSeconds:0];
                
            });
        });
        return;
    } else if ((alertView.tag == DELETE_TAG) && (buttonIndex == DELETE_BUTTON_INDEX)) {
        [self deleteSelectedUser];
    } else if((alertView.tag == RESETPIN_TAG) && (buttonIndex == RESETPIN_BUTTON_INDEX)) {
        [[ErrorHandler sharedManager] startLoadingInController:self  message:@""];
        id<IUser> iuser = (self.users)[selectedIndexPath.row];
        NSString * userID = [iuser getIdentity];
        [self deleteSelectedUser];
        ConfigurationManager *cfm = [ConfigurationManager sharedManager];
        [sdk RegisterNewUser:userID devName:[cfm getDeviceName]];
    }
}

- (void)deleteSelectedUser
{
    [self hideBottomBar:YES];
    id<IUser> iuser = (self.users)[selectedIndexPath.row];
    [MPin DeleteUser:iuser];
    [self.users removeObjectAtIndex:selectedIndexPath.row];
    selectedIndexPath = [NSIndexPath indexPathForRow:NOT_SELECTED inSection:NOT_SELECTED_SEC];
    [[ConfigurationManager sharedManager] setSelectedUserForCurrentConfiguration:NOT_SELECTED];
    [self.table reloadData];
}

#pragma mark - My actions -

- (IBAction)btnAddIDTap:(id)sender
{
    UIViewController* addViewController = [storyboard instantiateViewControllerWithIdentifier:@"Add"];
    [self.navigationController pushViewController:addViewController animated:YES];
}

- (IBAction)btnEditTap:(id)sender
{
}

- (IBAction)btnDeleteTap:(id)sender
{
    id<IUser> iuser = (self.users)[selectedIndexPath.row];
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"KEY_DELETE", @"")
                                                    message:[NSString stringWithFormat:NSLocalizedString(@"WARNING_USER_WILL_BE_DELETED", @""), [iuser getIdentity]]
                                                   delegate:self
                                          cancelButtonTitle:NSLocalizedString(@"KEY_CANCEL", @"")
                                          otherButtonTitles:NSLocalizedString(@"KEY_DELETE", @""),
                                          nil];
    alert.tag = DELETE_TAG;
    [alert show];
}





-(IBAction)onResetPinButtonClicked:(id)sender {
    
    id<IUser> iuser = (self.users)[selectedIndexPath.row];
    UIAlertView* alert = [[UIAlertView alloc] initWithTitle:@"RESET_PIN"
                                                    message:[NSString stringWithFormat:@"Are you sure that you would like to reset pin of \"%@\" ?", [iuser getIdentity]]
                                                   delegate:self
                                          cancelButtonTitle:@"CANCEL"
                                          otherButtonTitles:@"RESET",
                          nil];
    alert.tag = RESETPIN_TAG;
    [alert show];
}

- (IBAction)btnAuthTap:(id)sender
{
    ConfigurationManager* cf = [ConfigurationManager sharedManager];
    [cf setSelectedUserForCurrentConfiguration:selectedIndexPath.row];
    [self starAuthenticationFlow];
}

- (void)starAuthenticationFlow
{
    id<IUser> iuser = (self.users)[selectedIndexPath.row];
    NSDictionary* config;
    enum SERVICES s;
    IdentityBlockedViewController   *identityBlockedViewController;
    AccessNumberViewController      *accessViewController;
    
    switch ([iuser getState])
    {
        case INVALID:
            [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                           errorString:NSLocalizedString(@"HUD_REACTIVATE_USER", @"")
                                                  addActivityIndicator:NO
                                                     autoHideInSeconds:0];
            break;
        case STARTED_REGISTRATION:
            [[ErrorHandler sharedManager] startLoadingInController:self  message:@""];
            [sdk FinishRegistration:iuser];
            break;
        case ACTIVATED:
            [[ErrorHandler sharedManager] startLoadingInController:self  message:@""];
            [sdk FinishRegistration:iuser];
            break;
        case REGISTERED:
            [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                           errorString:NSLocalizedString(@"HUD_REACTIVATE_USER", @"")
                                                  addActivityIndicator:NO
                                                     autoHideInSeconds:0];
            
            config = [[ConfigurationManager sharedManager] getSelectedConfiguration];
            s = [config[kSERVICE_TYPE] intValue];
            switch (s)
            {
                case LOGIN_ON_MOBILE:
                    [sdk Authenticate:iuser];
                    break;
                case LOGIN_ONLINE:
                    accessViewController = [storyboard instantiateViewControllerWithIdentifier:@"accessnumber"];
                    accessViewController.delegate = self;
                    accessViewController.strEmail = [currentUser getIdentity];
                    [self.navigationController pushViewController:accessViewController animated:YES];
                    break;
                    
                case LOGIN_WITH_OTP:
                    [sdk AuthenticateOTP:iuser];
                    break;
            }
            break;
        case BLOCKED:
            identityBlockedViewController = [storyboard instantiateViewControllerWithIdentifier:@"IdentityBlockedViewController"];
            identityBlockedViewController.strUserEmail = [iuser getIdentity];
            identityBlockedViewController.iuser = iuser;
            [self.navigationController pushViewController:identityBlockedViewController animated:YES];
            break;
        default:
            [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                           errorString:NSLocalizedString(@"HUD_UNSUPPORTED_ACTION", @"")
                                                  addActivityIndicator:NO
                                                     autoHideInSeconds:0];
        break;
    }
}

- (void)showPinPad
{
    PinPadViewController* pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.userId = [currentUser getIdentity];
    pinpadViewController.boolShouldShowBackButton = YES;
    pinpadViewController.title = kEnterPin;
    [self.navigationController pushViewController:pinpadViewController animated:YES];
}

- (IBAction)showLeftMenuPressed:(id)sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

- (void)OnRegisterNewUserCompleted:(id)sender user:(const id<IUser>)user {
    [[ErrorHandler sharedManager] stopLoading];
    switch ([user getState]) {
        case STARTED_REGISTRATION: {
            ConfirmEmailViewController* cevc = (ConfirmEmailViewController*)
            [storyboard instantiateViewControllerWithIdentifier:
             @"ConfirmEmailViewController"];
            cevc.iuser = user;
            [self.navigationController pushViewController:cevc animated:YES];
        } break;
        case ACTIVATED:
            [[ErrorHandler sharedManager] startLoadingInController:self  message:@""];
            currentUser = user;
            [sdk FinishRegistration:user];
            break;
        default:
            [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                           errorString:[NSString stringWithFormat:@"User state is unexpected %ld",                                                                                       [user getState]]
                                                  addActivityIndicator:NO
                                                     autoHideInSeconds:0];
            break;
    }
    
    NSArray * users = [MPin listUsers];
    int index = 0;
    for (;index<[users count];index++) {
        id<IUser> cUser = [users objectAtIndex:index];
        if ([[cUser getIdentity] isEqualToString:[user getIdentity]])   break;
    }
    
    ConfigurationManager* cf = [ConfigurationManager sharedManager];
    [cf setSelectedUserForCurrentConfiguration:(index)];
}

- (void)OnRegisterNewUserError:(id)sender error:(NSError*)error {
    [[ErrorHandler sharedManager] stopLoading];
    MpinStatus* mpinStatus = [error.userInfo objectForKey:kMPinSatus];
    [[ErrorHandler sharedManager] presentErrorInViewController:self
                                                   errorString:mpinStatus.errorMessage
                                          addActivityIndicator:NO
                                             autoHideInSeconds:0];
}


@end
