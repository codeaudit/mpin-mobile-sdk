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

@interface AppDelegate ()
@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	[[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleDefault];
    
    [[BITHockeyManager sharedHockeyManager] configureWithIdentifier:[SettingsManager sharedManager].strHockeyAppID];
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
@end
