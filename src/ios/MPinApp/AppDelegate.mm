//
//  AppDelegate.m
//  MPinApp
//
//  Created by Georgi Georgiev on 11/17/14.
//  Copyright (c) 2014 Certivox. All rights reserved.
//

#import "AppDelegate.h"
#import "Constants.h"
#import "MFSideMenuContainerViewController.h"
#import "SettingsManager.h"
#import "OTPViewController.h"
#import "AFNetworkReachabilityManager.h"
#import "ApplicationManager.h"
#import <HockeySDK/HockeySDK.h>
#import "NetworkMonitor.h"

@interface AppDelegate ()
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
    [NetworkMonitor sharedManager];
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
    return NO;
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
