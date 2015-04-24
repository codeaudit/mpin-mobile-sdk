//
//  BaseViewController.h
//  MPinApp
//
//  Created by Georgi Georgiev on 12/1/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MpinStatus.h"
#import "MPin.h"
#import "MFSideMenu.h"
#import "SettingsManager.h"
#import "ATMHud.h"


#define SUCCESSFUL_AUTHENTICATION 200
#define USER_ACTIVATION_REQUIRED 403

#define GOTO_IDENTITY_LIST 0
#define ICONFIRM 1
#define RESEND 2


@interface BaseViewController : UIViewController <UIAlertViewDelegate> {
    ATMHud* hud;
    id<IUser> currentUser;
    NSString* pinpadHeaderTitle;
    NSString *strBuildNumber;
    NSString *strAppVersion;
    NSString *strSDKVersion;
}

- (void)showPinPad;
- (void)startLoading;
- (void)stopLoading;

- (void) showConfirmationScreen:(id<IUser>) iuser;
- (void)finishRegistration:(id<IUser>)iuser;
- (void)onFinishRegistration:(MpinStatus*)mpinStatus;
- (IBAction)showLeftMenuPressed:(id)sender;

@end
