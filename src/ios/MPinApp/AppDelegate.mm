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
#import "Mint.h"
#import <SplunkMint-iOS/SplunkMint-iOS.h>
#import "OTPViewController.h"
#import "AFNetworkReachabilityManager.h"
#import "ApplicationManager.h"


@interface AppDelegate ()
@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	[[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleDefault];

    [[Mint sharedInstance] initAndStartSession:@"a61632de"];
    
	UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone"
	                            bundle:[NSBundle mainBundle]];

	MFSideMenuContainerViewController *container = (MFSideMenuContainerViewController *)self.window.rootViewController;
	_vcUserList = [storyboard
	               instantiateViewControllerWithIdentifier:@"UserListViewController"];

	UIViewController *leftSideMenuViewController = [storyboard instantiateViewControllerWithIdentifier:@"MenuViewController"];

	[container setLeftMenuViewController:leftSideMenuViewController];
	[container setCenterViewController:[[UINavigationController alloc] initWithRootViewController:_vcUserList]];

	self.window.rootViewController = container;
    
    [ApplicationManager sharedManager];
    
	return YES;
}

/// http://192.168.98.109:8005
/// https://mpindemo-qa-v3.certivox.org
//// @"http://ec2-54-77-232-113.eu-west-1.compute.amazonaws.com"
/// @"http://risso.certivox.org"

- (void)applicationWillResignActive:(UIApplication *)application
{
	// Sent when the application is about to move from active to inactive state.
	// This can occur for certain types of temporary interruptions (such as an
	// incoming phone call or SMS message) or when the user quits the application
	// and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and throttle down
	// OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    MFSideMenuContainerViewController *container = (MFSideMenuContainerViewController *)self.window.rootViewController;
    if ([((UINavigationController *)container.centerViewController).topViewController  isMemberOfClass:[OTPViewController class]]){
        [((UINavigationController *)container.centerViewController) popToRootViewControllerAnimated:NO];
    }
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	// Restart any tasks that were paused (or not yet started) while the
	// application was inactive. If the application was previously in the
	// background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	// Called when the application is about to terminate. Save data if
	// appropriate. See also applicationDidEnterBackground:.
}

@end
