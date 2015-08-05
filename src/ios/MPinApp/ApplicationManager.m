//
//  ConnectionManager.m
//  MPinApp
//
//  Created by Georgi Georgiev on 5/14/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "ApplicationManager.h"
#import "ConfigurationManager.h"
#import "AppDelegate.h"
#import "MFSideMenuContainerViewController.h"
#import "UserListViewController.h"

static NSString *constStrConnectionTimeoutNotification = @"ConnectionTimeoutNotification";

@interface ApplicationManager ( ) {
    MPin *sdk;
    AppDelegate *appdelegate;
}

@end


@implementation ApplicationManager

+ ( ApplicationManager * )sharedManager
{
    static ApplicationManager *sharedManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^ {
        sharedManager = [[self alloc] init];
    });

    return sharedManager;
}

- ( instancetype ) init
{
    self = [super init];
    if ( self )
    {
        sdk = [[MPin alloc] init];
        sdk.delegate = self;
        appdelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    }

    return self;
}

/// TODO :: Move any MSGs to Localization File

- ( void ) OnSetBackendCompleted:( id ) sender
{
    MFSideMenuContainerViewController *container = (MFSideMenuContainerViewController *)appdelegate.window.rootViewController;
    if ( [( (UINavigationController *)container.centerViewController ).topViewController isMemberOfClass:[UserListViewController class]] )
    {
        [(UserListViewController *)( ( (UINavigationController *)container.centerViewController ).topViewController )invalidate];
    }
}

- ( void ) OnSetBackendError:( id ) sender error:( NSError * ) error;
{
    [[NSNotificationCenter defaultCenter] postNotificationName:constStrConnectionTimeoutNotification object:nil];

    //FIXME: Remove alert
    MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:[mpinStatus getStatusCodeAsString] message:mpinStatus.errorMessage delegate:nil cancelButtonTitle:@"Close" otherButtonTitles:nil];
    [alert show];
}

-( void ) setBackend
{
    [sdk SetBackend:[[ConfigurationManager sharedManager] getSelectedConfiguration]];
}

@end




