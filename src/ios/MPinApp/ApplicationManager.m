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

#import "ApplicationManager.h"
#import "AFNetworkReachabilityManager.h"
#import "ConfigurationManager.h"
#import "AppDelegate.h"
#import "MFSideMenuContainerViewController.h"
#import "UserListViewController.h"

static NSString *constStrConnectionTimeoutNotification = @"ConnectionTimeoutNotification";

@interface ApplicationManager ( ) {
    MPin *sdk;
    AppDelegate *appdelegate;
}
- ( void ) runNetowrkMonitoring;
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
        [self runNetowrkMonitoring];
    }

    return self;
}

/// TODO :: Move any MSGs to Localization File
- ( void ) runNetowrkMonitoring
{
    [[AFNetworkReachabilityManager sharedManager] setReachabilityStatusChangeBlock: ^ (AFNetworkReachabilityStatus status) {
        switch ( status )
        {
        case AFNetworkReachabilityStatusReachableViaWiFi:
        case AFNetworkReachabilityStatusReachableViaWWAN:
            if ( ![MPin isConfigLoadSuccessfully] )
            {
                [sdk SetBackend:[[ConfigurationManager sharedManager] getSelectedConfiguration]];
            }
                [[ErrorHandler sharedManager] hideMessage];
            break;

        default:
            {
                //TODO: show netowrk indicator
                
                UIWindow *window = [[UIApplication sharedApplication] keyWindow];

                
                [[ErrorHandler sharedManager] presentMessageInViewController:window.rootViewController errorString:NSLocalizedString(@"ERROR_NO_INTERNET_CONNECTION", @"No Internet Connection!") addActivityIndicator:YES minShowTime:0];
            }
            break;
        }
    }];
    // and now activate monitoring
    [[AFNetworkReachabilityManager sharedManager] startMonitoring];
}

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
    if ( ![MPin isConfigLoadSuccessfully] )
    {
        [sdk SetBackend:[[ConfigurationManager sharedManager] getSelectedConfiguration]];
    }
}

@end




