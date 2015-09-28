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

#import <HockeySDK/HockeySDK.h>
#import "AppDelegate.h"
#import "Constants.h"
#import "MFSideMenuContainerViewController.h"
#import "ApplicationManager.h"
#import "NetworkMonitor.h"
#import "NetworkDownViewController.h"
#import "AFNetworkReachabilityManager.h"
#import "ANAuthenticationSuccessful.h"
#import "Utilities.h"
#import "HelpViewController.h"
#import "ConfigurationManager.h"
#import "IUser.h"

@interface AppDelegate ()
{
    MFSideMenuContainerViewController *container;
    NetworkDownViewController *vcNetworkDown;
    HelpViewController *vcHelp;
    BOOL boolRestartFlow;
    
    MPin *sdk;
}

@property (nonatomic, retain) NSString* strDeviceToken;

- ( void ) setDeviceTokenString:(NSData *) dToken;
- ( void ) showPinPad:(id<IUser>) user;

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    boolRestartFlow = NO;
    [[AFNetworkReachabilityManager sharedManager] startMonitoring];
    [[UIApplication sharedApplication] setApplicationIconBadgeNumber:0];
    UIUserNotificationType types = UIUserNotificationTypeBadge |
    UIUserNotificationTypeSound | UIUserNotificationTypeAlert;
    
    // By request of TDL
    // If there is environment variable called PUSH_NOTIFICATIONS and the value of this variable is STOP, this will prevent registration for 
    NSString *strRegisterNotifications = [[[NSProcessInfo processInfo] environment] objectForKey:@"PUSH_NOTIFICATIONS"];
    if (![strRegisterNotifications isEqualToString:@"STOP"])
    {
        UIUserNotificationSettings *mySettings = [UIUserNotificationSettings settingsForTypes:types categories:nil];
        
        [[UIApplication sharedApplication] registerUserNotificationSettings:mySettings];
        [application registerForRemoteNotifications];
        
    }

	[[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleDefault];

    [[BITHockeyManager sharedHockeyManager] configureWithIdentifier:[SettingsManager sharedManager].strHockeyAppID];
    [[BITHockeyManager sharedHockeyManager].crashManager setCrashManagerStatus:BITCrashManagerStatusAutoSend];
    [[BITHockeyManager sharedHockeyManager] startManager];
    [[BITHockeyManager sharedHockeyManager].authenticator authenticateInstallation];
    
	container = (MFSideMenuContainerViewController *)self.window.rootViewController;
    
	UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:[NSBundle mainBundle]];
    _vcUserList = [storyboard instantiateViewControllerWithIdentifier:@"UserListViewController"];
    
    vcNetworkDown = [storyboard instantiateViewControllerWithIdentifier:@"NetworkDownViewController"];
    vcHelp = [storyboard instantiateViewControllerWithIdentifier:@"HelpViewController"];
    
	UIViewController *leftSideMenuViewController = [storyboard instantiateViewControllerWithIdentifier:@"MenuViewController"];

    [container setCenterViewController:[[UINavigationController alloc] initWithRootViewController:_vcUserList]];
    
	[container setLeftMenuViewController:leftSideMenuViewController];

	self.window.rootViewController = container;
    
    [ApplicationManager sharedManager];
    [NetworkMonitor sharedManager];
    
    if (![NetworkMonitor isNetworkAvailable])
    {
        [container setCenterViewController:[[UINavigationController alloc] initWithRootViewController:vcNetworkDown]];
        container.panMode = MFSideMenuPanModeNone;
    }
    else if ([[ConfigurationManager sharedManager] isFirstTimeLaunch])
    {
        [self firstTimeLaunch];
    }
    
    [ApplicationManager sharedManager];
    [NetworkMonitor sharedManager];
    
    
    sdk  = [[MPin alloc] init];
    sdk.delegate = self;

	return YES;
}

- (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)devToken
{
    self.devToken = devToken;
    [self setDeviceTokenString:devToken];
    NSLog(@"%@", devToken.description);
}

- (void)application:(UIApplication *)app didFailToRegisterForRemoteNotificationsWithError:(NSError *)err {
    NSLog(@"Error in registration. Error: %@", err);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult result))handler
{
    /// NSLog(@"%@", userInfo[@"aps"][@"token"]);
    NSLog(@"%@", userInfo[@"aps"][@"hash_user_id"]);
    NSLog(@"%@", userInfo[@"aps"][@"notification_message"]);
    NSLog(@"%@", userInfo[@"aps"][@"mobileToken"]);
    
    /// TODO ::
}
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    boolRestartFlow = NO;
    MFSideMenuContainerViewController *c = (MFSideMenuContainerViewController *)self.window.rootViewController;
    
    UINavigationController *centerNavigationController = c.centerViewController;
    if ([centerNavigationController.topViewController isKindOfClass:[UserListViewController class]]
        || [centerNavigationController.topViewController isKindOfClass:[ANAuthenticationSuccessful class]])
    {
        [c.centerViewController popToRootViewControllerAnimated:NO];
        boolRestartFlow = YES;
    }

}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    
    NSDictionary * urlParams = [Utilities urlQueryParamsToDictianary:[url query]];
    NSString * mpinId = [urlParams objectForKey:@"mpinId"];
    NSString * activateKey = [urlParams objectForKey:@"activateKey"];
    NSString * hash_user_id = [urlParams objectForKey:@"hash_user_id"];
    
    /// TODO :: store hash value mapped to mpinID
    
    NSString * fromHexMpinId = [Utilities stringFromHexString:mpinId];
    NSError *error = nil;
    NSDictionary *mpinIdJSON = [NSJSONSerialization JSONObjectWithData:[fromHexMpinId dataUsingEncoding:NSUTF8StringEncoding] options:kNilOptions error:&error];
    if(error != nil) {
        [[ErrorHandler sharedManager] presentMessageInViewController:((UINavigationController *)container.centerViewController).topViewController
                                                         errorString:[NSString stringWithFormat:@"Failed to parse mpinId json: %@", mpinId]
                                                addActivityIndicator:YES
                                                         minShowTime:0];
        
        return YES;
    }

    NSString * userID = mpinIdJSON[@"userID"];
    id<IUser> user = [MPin MakeNewUser:userID];
    [sdk VerifyUser:user mpinId:mpinId activationKey:activateKey];
    return YES;
}


- ( void ) OnVerifyUserompleted:( id ) sender user:( const id<IUser>) user {
    
    [[NSNotificationCenter defaultCenter] removeObserver:nil name:kShowPinPadNotification object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( showPinPad: ) name:kShowPinPadNotification object:nil];
    [sdk FinishRegistration:user pushNotificationIdentifier:self.strDeviceToken];
}
- ( void ) OnVerifyUserError:( id ) sender error:( NSError * ) error {
    
    MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
    MFSideMenuContainerViewController *container = (MFSideMenuContainerViewController *)self.window.rootViewController;
    [[ErrorHandler sharedManager] presentMessageInViewController:((UINavigationController *)container.centerViewController).topViewController
                                                     errorString:mpinStatus.errorMessage
                                            addActivityIndicator:YES
                                                     minShowTime:0];

}


- ( void )OnFinishRegistrationCompleted:( id )sender user:( const id<IUser>)user
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];
}

- ( void )OnFinishRegistrationError:( id )sender error:( NSError * )error
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:kShowPinPadNotification object:nil];

    MpinStatus *mpinStatus = ( error.userInfo ) [kMPinSatus];
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
    [((UINavigationController *)container.centerViewController).topViewController.navigationController pushViewController:pinpadViewController animated:YES];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
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



- (void)applicationDidBecomeActive:(UIApplication *)application
{
    if (![NetworkMonitor isNetworkAvailable])
    {
        [self connectionDown];
    }
    if (boolRestartFlow && [NetworkMonitor isNetworkAvailable])
    {
        boolRestartFlow = !boolRestartFlow;
        [container setCenterViewController:[[UINavigationController alloc] initWithRootViewController:_vcUserList]];
        [_vcUserList invalidate];
    }

    UIView *colourView = [self.window viewWithTag:1234];
    [UIView animateWithDuration:0.5 animations:^{
        colourView.alpha = 0;
    } completion:^(BOOL finished) {
        [colourView removeFromSuperview];
    }];
}


-( void) firstTimeLaunch
{
    NSLog(@"Appdelegate : First time");
    
    [container setCenterViewController:[[UINavigationController alloc] initWithRootViewController:vcHelp]];
    vcHelp.helpMode = HELP_QUICK_START;
    container.panMode = MFSideMenuPanModeNone;
}

-( void) connectionDown
{
    NSLog(@"Appdelegate : Connection Down");
    [container setCenterViewController:[[UINavigationController alloc] initWithRootViewController:vcNetworkDown]];
    container.panMode = MFSideMenuPanModeNone;
}

-( void) connectionUp
{
    NSLog(@"Appdelegate : Connection Up");
    if ([[ConfigurationManager sharedManager] isFirstTimeLaunch])
    {
        [self firstTimeLaunch];
    }
    else
    {
        [container setCenterViewController:[[UINavigationController alloc] initWithRootViewController:_vcUserList]];
        [_vcUserList setBackend];
        container.panMode = MFSideMenuPanModeDefault;
    }
    
}

- ( void ) setDeviceTokenString:(NSData *) dToken {
    self.strDeviceToken = [[dToken description] stringByTrimmingCharactersInSet: [NSCharacterSet characterSetWithCharactersInString:@"<>"]];
    self.strDeviceToken  = [ self.strDeviceToken  stringByReplacingOccurrencesOfString:@" " withString:@""];
}

@end
