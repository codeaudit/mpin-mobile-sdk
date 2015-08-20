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

#import "AboutViewController.h"
#import "ThemeManager.h"
#import "MFSideMenu.h"
#import "SettingsManager.h"
#import "AppDelegate.h"
#import "AFHTTPRequestOperationManager.h"

@interface AboutViewController ( ) {}

@property ( nonatomic, weak ) IBOutlet UILabel *lblBuildNumber;
@property ( nonatomic, weak ) IBOutlet UILabel *lblAppVersion;

@end

@implementation AboutViewController

- ( void )viewDidLoad
{
    [super viewDidLoad];

    self.title = @"About";
    _lblBuildNumber.font = [UIFont fontWithName:@"OpenSans-Bold" size:12.f];
    _lblAppVersion.font = [UIFont fontWithName:@"OpenSans-Bold" size:12.f];


    _lblBuildNumber.text = [NSString stringWithFormat:NSLocalizedString(@"ABOUTVC_BUILD_NUMBER", @"ABOUT VC BUILD NUMBER"), [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"]];
    _lblAppVersion.text = [NSString   stringWithFormat:@"%@ %@", @"Ver. ",[[NSBundle mainBundle]  objectForInfoDictionaryKey:@"CFBundleShortVersionString"]];
    self.title = NSLocalizedString(@"ABOUTVC_TITLE", @"");

//    "ABOUTVC_BTN_MOB_GUIDE" =   "M-Pin Mobile Guide";
//    "ABOUTVC_BTN_SDK_URL"   =   "Built on the M-Pin Mobile SDK";
//    "ABOUTVC_BTN_HOMEPAGE"  =   "CertiVox Homepage";
//    "ABOUTVC_BTN_SUPPORT"   =   "Product Support";
//    "ABOUTVC_BTN_TERMS"     =   "CertiVox Terms & Conditions";
//    "ABOUTVC_BTN_VALUES"    =   "Values / Security / Privacy";

    [_btnGuide      setTitle:NSLocalizedString(@"ABOUTVC_BTN_MOB_GUIDE", @"M-Pin Mobile Guide") forState:UIControlStateNormal];
    [_btnSDK_URL    setTitle:NSLocalizedString(@"ABOUTVC_BTN_SDK_URL", @"Built on the M-Pin Mobile SDK") forState:UIControlStateNormal];
    [_btnHomepage   setTitle:NSLocalizedString(@"ABOUTVC_BTN_HOMEPAGE", @"CertiVox Homepage") forState:UIControlStateNormal];
    [_btnSupport    setTitle:NSLocalizedString(@"ABOUTVC_BTN_SUPPORT", @"Product Support") forState:UIControlStateNormal];
    [_btnTerms      setTitle:NSLocalizedString(@"ABOUTVC_BTN_TERMS", @"CertiVox Terms & Conditions") forState:UIControlStateNormal];
    [_btnValues     setTitle:NSLocalizedString(@"ABOUTVC_BTN_VALUES", @"Values / Security / Privacy") forState:UIControlStateNormal];
}

-( void ) viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    [self registerObservers];
    [[ThemeManager sharedManager] beautifyViewController:self];
}

- (void) viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self unRegisterObservers];
}

- ( void )didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
   #pragma mark - Navigation

   // In a storyboard-based application, you will often want to do a little
   preparation before navigation
   - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
   }
 */

#pragma mark - Actions -

- ( IBAction )showLeftMenuPressed:( id )sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

-( IBAction )btnGuideTap:( id )sender
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[SettingsManager sharedManager].strUrlMobGuide]];
}

-( IBAction )btnSKDTap:( id )sender
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[SettingsManager sharedManager].strUrlSDK]];
}

-( IBAction )btnHomepageTap:( id )sender
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[SettingsManager sharedManager].strUrlHomepage]];
}

-( IBAction )btnSupportTap:( id )sender
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[SettingsManager sharedManager].strUrlSupport]];
}

-( IBAction )btnTermsTap:( id )sender
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[SettingsManager sharedManager].strUrlTerms]];
}

-( IBAction )btnValuesTap:( id )sender
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[SettingsManager sharedManager].strUrlValues]];
}

- ( IBAction )sendToken:( id )sender
{
    AFHTTPRequestOperationManager *operationManager = [AFHTTPRequestOperationManager manager];
    AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    NSString *deviceTokenString = [[appDelegate.devToken description] stringByTrimmingCharactersInSet: [NSCharacterSet characterSetWithCharactersInString:@"<>"]];
    deviceTokenString = [deviceTokenString stringByReplacingOccurrencesOfString:@" " withString:@""];
    NSLog(@"%@", deviceTokenString);
    [operationManager POST:_backend.text parameters:deviceTokenString success: ^ (AFHTTPRequestOperation *operation, id responseObject){
        [[[UIAlertView alloc] initWithTitle:@"" message:@"Operation completed" delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil] show];
    } failure: ^ (AFHTTPRequestOperation *operation, NSError *error){
        [[[UIAlertView alloc] initWithTitle:@"" message:@"Operation completed" delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil] show];
    }];
}

#pragma mark - NSNotification handlers -

-( void ) networkUp
{
    [[ThemeManager sharedManager] hideNetworkDown:self];
}

-( void ) networkDown
{
    NSLog(@"Network DOWN Notification");
    
    [self.view layoutIfNeeded];
    [UIView animateWithDuration:kFltNoNetworkMessageAnimationDuration animations:^{
        self.constraintNoNetworkViewHeight.constant = 36.0f;
        [self.view layoutIfNeeded];
    }];
}

-( void ) unRegisterObservers
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"NETWORK_DOWN_NOTIFICATION" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"NETWORK_UP_NOTIFICATION" object:nil];
}

- ( void ) registerObservers
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( networkUp ) name:@"NETWORK_UP_NOTIFICATION" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector( networkDown ) name:@"NETWORK_DOWN_NOTIFICATION" object:nil];
}

@end
