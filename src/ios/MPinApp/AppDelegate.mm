//
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


#import "AppDelegate.h"
#import "Constants.h"
#import "MFSideMenuContainerViewController.h"
#import "SettingsManager.h"
#import "OTPViewController.h"
#import "AFNetworkReachabilityManager.h"
#import "ApplicationManager.h"
#import <HockeySDK/HockeySDK.h>
#import "Utilities.h"

@interface AppDelegate ()
- ( void )showPinPad:(id<IUser>) user;
@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
    UIUserNotificationType types = UIUserNotificationTypeBadge |
    UIUserNotificationTypeSound | UIUserNotificationTypeAlert;
    
    UIUserNotificationSettings *mySettings = [UIUserNotificationSettings settingsForTypes:types categories:nil];
    
    [[UIApplication sharedApplication] registerUserNotificationSettings:mySettings];
    [application registerForRemoteNotifications];

    
	[[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleDefault];

    [[BITHockeyManager sharedHockeyManager] configureWithIdentifier:[SettingsManager sharedManager].strHockeyAppID];
    [[BITHockeyManager sharedHockeyManager].crashManager setCrashManagerStatus:BITCrashManagerStatusAutoSend];
    [[BITHockeyManager sharedHockeyManager] startManager];
    [[BITHockeyManager sharedHockeyManager].authenticator authenticateInstallation];
    
	UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:[NSBundle mainBundle]];

	MFSideMenuContainerViewController *container = (MFSideMenuContainerViewController *)self.window.rootViewController;
	_vcUserList = [storyboard instantiateViewControllerWithIdentifier:@"UserListViewController"];

	UIViewController *leftSideMenuViewController = [storyboard instantiateViewControllerWithIdentifier:@"MenuViewController"];

	[container setLeftMenuViewController:leftSideMenuViewController];
	[container setCenterViewController:[[UINavigationController alloc] initWithRootViewController:_vcUserList]];

	self.window.rootViewController = container;
    
    [ApplicationManager sharedManager];
    
	return YES;
}

- (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)devToken
{
    self.devToken = devToken;
    NSLog(@"%@", devToken.description);
}

- (void)application:(UIApplication *)app didFailToRegisterForRemoteNotificationsWithError:(NSError *)err {
    NSLog(@"Error in registration. Error: %@", err);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult result))handler
{
    NSLog(@"%@", userInfo[@"aps"][@"token"]);
}
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    MFSideMenuContainerViewController *container = (MFSideMenuContainerViewController *)self.window.rootViewController;
    if ([((UINavigationController *)container.centerViewController).topViewController  isMemberOfClass:[OTPViewController class]]){
        [((UINavigationController *)container.centerViewController) popToRootViewControllerAnimated:NO];
    }
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    NSDictionary * urlParams = [Utilities urlQueryParamsToDictianary:[url query]];
    MPin *sdk  = [[MPin alloc] init];
    sdk.delegate = self;
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( showPinPad: ) name:kShowPinPadNotification object:nil];
    [sdk ActivateUserRegisteredBySMS:[urlParams objectForKey:@"mpinId"] activationKey:[urlParams objectForKey:@"activateKey"]];
    return YES;
}

- ( void ) OnActivateUserRegisteredBySMSCompleted:( id ) sender {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}
- ( void ) OnActivateUserRegisteredBySMSError:( id ) sender error:( NSError * ) error {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
    
    MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
    MFSideMenuContainerViewController *container = (MFSideMenuContainerViewController *)self.window.rootViewController;
    [[ErrorHandler sharedManager] presentMessageInViewController:((UINavigationController *)container.centerViewController).topViewController
                                                     errorString:mpinStatus.errorMessage
                                            addActivityIndicator:YES
                                                     minShowTime:0];
}

- ( void )showPinPad:(NSNotification *)notification  {
    UIStoryboard * storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    PinPadViewController *pinpadViewController = [storyboard instantiateViewControllerWithIdentifier:@"pinpad"];
    pinpadViewController.boolShouldShowBackButton = YES;
    pinpadViewController.boolIsSMS = YES;
    pinpadViewController.title = kEnterPin;
    pinpadViewController.currentUser = [notification.userInfo objectForKey:kUser];
    pinpadViewController.boolSetupPin = YES;
    MFSideMenuContainerViewController *container = (MFSideMenuContainerViewController *)self.window.rootViewController;
    [((UINavigationController *)container.centerViewController).topViewController.navigationController pushViewController:pinpadViewController animated:YES];
}

- (void)applicationWillResignActive:(UIApplication *)application {
    
    // fill screen with our own colour
    UIView *protectionView = [[UIView alloc]initWithFrame:self.window.frame];
    protectionView.backgroundColor = [UIColor whiteColor];
    protectionView.tag = 1234;
    protectionView.alpha = 0;
    [self.window addSubview:protectionView];
    UIImageView *imgView = [[UIImageView alloc] initWithFrame:self.window.frame];
    [protectionView addSubview:imgView];
    [imgView setImage:[UIImage imageNamed:@"CVXLogo"]];
    imgView.contentMode = UIViewContentModeCenter;
    imgView.backgroundColor = [[SettingsManager sharedManager] color10];
    [self.window bringSubviewToFront:protectionView];
    
    // fade in the view
    [UIView animateWithDuration:0.5 animations:^{
        protectionView.alpha = 1;
    }];
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    
    // grab a reference to our coloured view
    UIView *colourView = [self.window viewWithTag:1234];
    
    // fade away colour view from main view
    [UIView animateWithDuration:0.5 animations:^{
        colourView.alpha = 0;
    } completion:^(BOOL finished) {
        // remove when finished fading
        [colourView removeFromSuperview];
    }];
}

@end
