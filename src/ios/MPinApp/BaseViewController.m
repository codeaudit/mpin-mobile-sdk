//
//  BaseViewController.m
//  MPinApp
//
//  Created by Georgi Georgiev on 12/1/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import "BaseViewController.h"
#import "PinPadViewController.h"
#import "ConfirmEmailViewController.h"
#import "Constants.h"
#import "IdentityCreatedViewController.h"

static NSString *const kConfirm = @"I confirmed my email";
static NSString *const kResend = @"Resend confirmation e-mail";
static NSString *const kGotoIdList = @"Go to the identities list";

@interface BaseViewController (){
}
@property (nonatomic, weak) IBOutlet UIButton *btnAdd;

@end

@implementation BaseViewController

- (void)viewDidLoad
{
	[super viewDidLoad];

	self.automaticallyAdjustsScrollViewInsets = NO;
	pinpadHeaderTitle = kSetupPin;
	hud = [[ATMHud alloc] initWithDelegate:self];
	

	strBuildNumber = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"];
	strAppVersion = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
	strSDKVersion = @"";
}

- (void)startLoading
{
	[hud showInView:self.view];
}
- (void)stopLoading
{
	[hud hide];
}

- (void)finishRegistration:(id<IUser>)iuser
{
	[self startLoading];

	currentUser = iuser;
	dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{

        MpinStatus *mpinStatus = [MPin FinishRegistration:iuser];

		dispatch_async(dispatch_get_main_queue(), ^(void) {
			[self onFinishRegistration:mpinStatus ];
		});
	});
}

- (void)onFinishRegistration:(MpinStatus *)mpinStatus
{

	if (mpinStatus.status == IDENTITY_NOT_VERIFIED) {
		[self showConfirmationScreen:currentUser];
	}
	else {
		UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
		IdentityCreatedViewController *vcIDCreated = (IdentityCreatedViewController *)[storyboard instantiateViewControllerWithIdentifier:@"IdentityCreatedViewController"];
		vcIDCreated.strEmail = [currentUser getIdentity];
		[self stopLoading];
		[self.navigationController pushViewController:vcIDCreated animated:YES];
	}
	[self stopLoading];
}

/// TODO to be removed later when we have new design
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
}

- (void)showConfirmationScreen:(id<IUser>)iuser
{
	UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
	ConfirmEmailViewController *cevc = (ConfirmEmailViewController *)[storyboard instantiateViewControllerWithIdentifier:@"ConfirmEmailViewController"];
	cevc.iuser = iuser;
	[self stopLoading];
	[self.navigationController pushViewController:cevc animated:YES];
}

- (void)showPinPad
{
	UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
	PinPadViewController *pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
	pinpadViewController.userId = [currentUser getIdentity];
	if ([pinpadHeaderTitle isEqualToString:kSetupPin]) {
		pinpadViewController.boolShouldShowBackButton = NO;
	}
	else {
		pinpadViewController.boolShouldShowBackButton = YES;
	}
	pinpadViewController.title = pinpadHeaderTitle;
	[self.navigationController pushViewController:pinpadViewController animated:YES];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showPinPad) name:kShowPinPadNotification object:nil];
}
- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
	[[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}
- (IBAction)showLeftMenuPressed:(id)sender
{
	[self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}
@end
