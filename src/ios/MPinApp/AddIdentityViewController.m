//
//  AddIdentityViewController.m
//  MPinApp
//
//  Created by Georgi Georgiev on 11/20/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import "AddIdentityViewController.h"
#import "Constants.h"
#import "IUser.h"
#import "UIViewController+Helper.h"
#import "IdentityCreatedViewController.h"
#import "ConfirmEmailViewController.h"
#import "PinPadViewController.h"
#import "ConfigurationManager.h"
#import "MPin.h"
#import "ATMHud.h"
#import "ThemeManager.h"
#import "iToast.h"

static NSString* const kEmpty = @"";
static NSString* const kMpinStatus = @"MpinStatus";
static NSString* const kUser = @"User";

@interface AddIdentityViewController () {
    MPin* sdk;
    id<IUser> currentUser;
    ATMHud* hud;
    ThemeManager* themeManager;
}

- (void)startLoading;
- (void)stopLoading;

- (void)showPinPad;
- (void)showDeviceName;
- (void)hideDeviceName;

- (IBAction)textFieldReturn:(id)sender;
- (IBAction)addAction:(id)sender;
- (IBAction)back:(id)sender;
@end

@implementation AddIdentityViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.automaticallyAdjustsScrollViewInsets = NO;
    
    hud = [[ATMHud alloc] initWithDelegate:self];
    [hud setActivity:YES];
    [[ThemeManager sharedManager] beautifyViewController:self];
    
    sdk = [[MPin alloc] init];
    sdk.delegate = self;
    
    ConfigurationManager *cfm = [ConfigurationManager sharedManager];
    self.txtDevName.text = [cfm getDeviceName];
    
    if([MPin isDeviceName]) {
        [self showDeviceName];
    } else {
        [self hideDeviceName];
    }
}

- (void)viewDidAppear:(BOOL)animated {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(showPinPad)
                                                 name:kShowPinPadNotification
                                               object:nil];
}
- (void)viewDidDisappear:(BOOL)animated {
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:kShowPinPadNotification
                                                  object:nil];
}

- (void)startLoading {
    [hud showInView:self.view];
}
- (void)stopLoading {
    [hud hide];
}

- (void)showPinPad {
    UIStoryboard* storyboard =
    [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    PinPadViewController* pinpadViewController =
    [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.userId = [currentUser getIdentity];
    pinpadViewController.boolShouldShowBackButton = NO;
    pinpadViewController.title = kSetupPin;
    [self.navigationController pushViewController:pinpadViewController
                                         animated:NO];
}

- (void)showDeviceName {
    self.txtDevName.hidden = NO;
    self.lblDevName.hidden = NO;
}

- (void)hideDeviceName {
    self.txtDevName.hidden = YES;
    self.lblDevName.hidden = YES;
}

- (IBAction)textFieldReturn:(id)sender {
    [sender resignFirstResponder];
}

- (IBAction)addAction:(id)sender {
    if ([kEmpty isEqualToString:self.txtIdentity.text]) {
        [[[[iToast makeText:NSLocalizedString(@"Enter text in user id text field!", @"")]
           setGravity:iToastGravityBottom] setDuration:iToastDurationLong] show];

        UIAlertView* alert =
        [[UIAlertView alloc] initWithTitle:@""
                                   message:NSLocalizedString(@"ERROR_PLEASE_ENTER_VALID_USER_ID", @"")
                                  delegate:nil
                         cancelButtonTitle:@"Close"
                         otherButtonTitles:nil, nil];
        [alert show];
        return;
    }
    
    if (![self isValidEmail:self.txtIdentity.text]) {
        UIAlertView* alert = [[UIAlertView alloc]
                              initWithTitle:@""
                              message:NSLocalizedString(@"ERROR_PLEASE_ENTER_VALID_EMAIL", @"")
                              delegate:nil
                              cancelButtonTitle:@"Close"
                              otherButtonTitles:nil, nil];
        [alert show];
        return;
    }
    
    if([MPin isDeviceName]) {
        ConfigurationManager *cfm = [ConfigurationManager sharedManager];
        [cfm setDeviceName:self.txtDevName.text];
    }
    
    [self startLoading];
    [sdk RegisterNewUser:self.txtIdentity.text devName:self.txtDevName.text];
}

- (void)OnRegisterNewUserCompleted:(id)sender user:(const id<IUser>)user {
    [self stopLoading];
    switch ([user getState]) {
        case STARTED_REGISTRATION: {
            UIStoryboard* storyboard =
            [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
            ConfirmEmailViewController* cevc = (ConfirmEmailViewController*)
            [storyboard instantiateViewControllerWithIdentifier:
             @"ConfirmEmailViewController"];
            cevc.iuser = user;
            [self.navigationController pushViewController:cevc animated:YES];
        } break;
        case ACTIVATED:
            [self startLoading];
            currentUser = user;
            [sdk FinishRegistration:user];
            break;
        default:
            [self
             showError:[user getIdentity]
             desc:[NSString stringWithFormat:NSLocalizedString(@"ERROR_UNEXPECTED_USER_STATE", @""),
                   [user getState]]];
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
    [self stopLoading];
    MpinStatus* mpinStatus = [error.userInfo objectForKey:kMPinSatus];
    [self showError:[mpinStatus getStatusCodeAsString]
               desc:mpinStatus.errorMessage];
}

- (void)OnFinishRegistrationCompleted:(id)sender user:(const id<IUser>)user {
    [self stopLoading];
    UIStoryboard* storyboard =
    [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    IdentityCreatedViewController* vcIDCreated = (IdentityCreatedViewController*)
    [storyboard instantiateViewControllerWithIdentifier:
     @"IdentityCreatedViewController"];
    vcIDCreated.strEmail = [user getIdentity];
    vcIDCreated.user = user;
    [self.navigationController pushViewController:vcIDCreated animated:YES];
}
- (void)OnFinishRegistrationError:(id)sender error:(NSError*)error {
    [self stopLoading];
    if (error.code == IDENTITY_NOT_VERIFIED) {
        UIStoryboard* storyboard =
        [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
        ConfirmEmailViewController* cevc = (ConfirmEmailViewController*)[storyboard
                                                                         instantiateViewControllerWithIdentifier:@"ConfirmEmailViewController"];
        cevc.iuser = [error.userInfo objectForKey:kUSER];
        [self.navigationController pushViewController:cevc animated:YES];
    } else {
        // TODO:
    }
}

- (BOOL)textFieldShouldReturn:(UITextField*)textField {
    [textField resignFirstResponder];
    return YES;
}
- (BOOL)textFieldShouldBeginEditing:(UITextField*)textField {
    return YES;
}

- (BOOL)isValidEmail:(NSString*)emailString {
    if ([emailString length] == 0 ||
        [emailString rangeOfString:@" "].location != NSNotFound) {
        return NO;
    }
    
    NSString* regExPattern = @"[A-Z0-9a-z._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}";
    
    NSRegularExpression* regEx = [[NSRegularExpression alloc]
                                  initWithPattern:regExPattern
                                  options:NSRegularExpressionCaseInsensitive
                                  error:nil];
    NSUInteger regExMatches =
    [regEx numberOfMatchesInString:emailString
                           options:0
                             range:NSMakeRange(0, [emailString length])];
    
    if (regExMatches == 0) {
        return NO;
    } else {
        return YES;
    }
}

- (IBAction)back:(id)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

/// TODO :: to be removed when new design is ready
- (void)alertView:(UIAlertView*)alertView
clickedButtonAtIndex:(NSInteger)buttonIndex {
    [self.navigationController popToRootViewControllerAnimated:YES];
}

@end
