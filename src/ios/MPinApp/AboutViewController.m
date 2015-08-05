//
//  AboutViewController.m
//  MPinApp
//
//  Created by Tihomir Ganev on 6.февр..15.
//  Copyright (c) 2015 г. Certivox. All rights reserved.
//

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

    [[ThemeManager sharedManager] beautifyViewController:self];
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

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    [[ThemeManager sharedManager] beautifyViewController:self];
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
    [operationManager POST:_backend.text parameters:deviceTokenString success:^(AFHTTPRequestOperation *operation, id responseObject){
        
        [[[UIAlertView alloc] initWithTitle:@"" message:@"Operation completed" delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil] show];
        
    }failure:^(AFHTTPRequestOperation *operation, NSError *error){
        
        [[[UIAlertView alloc] initWithTitle:@"" message:@"Operation completed" delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil] show];
        
        
    }];
    
}

@end
