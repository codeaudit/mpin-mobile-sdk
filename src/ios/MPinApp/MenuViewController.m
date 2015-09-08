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

#import "MenuViewController.h"
#import "MFSideMenu.h"
#import "AboutViewController.h"
#import "SettingsViewController.h"
#import "UserListViewController.h"
#import "AppDelegate.h"
#import "MenuTableViewCell.h"
#import "ThemeManager.h"
#import "ConfigurationManager.h"
#import "AFHTTPRequestOperationManager.h"
#import "HelpViewController.h"


@interface MenuViewController ( ) {
    AboutViewController     *vcAbout;
    SettingsViewController  *vcSettings;
    UserListViewController  *vcUserList;
    HelpViewController      *vcHelp;
    enum MENU_OPTIONS
    {
        USER_LIST = 0,
        SETTINGS = 1,
        QUICK_START = 2,
        GET_SERVER = 3,
        ABOUT = 4
    };
}

@end

@implementation MenuViewController

- ( void )viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    vcAbout = [self.storyboard instantiateViewControllerWithIdentifier:@"AboutViewController"];
    AppDelegate *appDelegate = (AppDelegate *)[UIApplication sharedApplication].delegate;
    vcUserList  = appDelegate.vcUserList;
    vcAbout     = [self.storyboard instantiateViewControllerWithIdentifier:@"AboutViewController"];
    vcSettings  = [self.storyboard instantiateViewControllerWithIdentifier:@"SettingsViewController"];
    vcHelp      = [self.storyboard instantiateViewControllerWithIdentifier:@"HelpViewController"];
    [[ThemeManager sharedManager] beautifyViewController:self];
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    [self setConfiguration];
}

-( void ) viewDidAppear:( BOOL )animated
{
    [super viewDidAppear:animated];
}

- ( void )didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - UITableViewDataSource -

- ( CGFloat )tableView:( UITableView * )tableView heightForRowAtIndexPath:( NSIndexPath * )indexPath
{
    return 60.f;
}

- ( NSInteger )numberOfSectionsInTableView:( UITableView * )tableView
{
    return 1;
}

- ( NSInteger )tableView:( UITableView * )tableView numberOfRowsInSection:( NSInteger )section
{
    return 5;
}

- ( UITableViewCell * )tableView:( UITableView * )tableView cellForRowAtIndexPath:( NSIndexPath * )indexPath
{
    static NSString *userListTableIdentifier = @"MenuTableViewCell";
    MenuTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:userListTableIdentifier];
    if ( cell == nil )
        cell = [[MenuTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:userListTableIdentifier];
    [[ThemeManager sharedManager] customiseMenuCell:cell];

    return cell;
}

- ( void )tableView:( UITableView * )tableView willDisplayCell:( UITableViewCell * )cell forRowAtIndexPath:( NSIndexPath * )indexPath
{
    switch ( indexPath.row )
    {
    case USER_LIST:
        ( (MenuTableViewCell *)cell ).lblMenuID.text = NSLocalizedString(@"MENUVC_OPTION_0",@"");
        break;

    case SETTINGS:
        ( (MenuTableViewCell *)cell ).lblMenuID.text = NSLocalizedString(@"MENUVC_OPTION_1",@"");
        break;

    case QUICK_START:
        ( (MenuTableViewCell *)cell ).lblMenuID.text = NSLocalizedString(@"MENUVC_OPTION_2",@"");
        break;

    case GET_SERVER:
        ( (MenuTableViewCell *)cell ).lblMenuID.text = NSLocalizedString(@"MENUVC_OPTION_3",@"");
        break;

    case ABOUT:
        ( (MenuTableViewCell *)cell ).lblMenuID.text = NSLocalizedString(@"MENUVC_OPTION_4",@"");
        break;
    }
}

- ( void )tableView:( UITableView * )tableView didSelectRowAtIndexPath:( NSIndexPath * )indexPath
{
    UIViewController *vc;

    [tableView deselectRowAtIndexPath:indexPath animated:YES];

    switch ( indexPath.row )
    {
    case USER_LIST:
        vc = vcUserList;
        break;

    case SETTINGS:
        vc = vcSettings;
        break;

    case QUICK_START:
        vc = vcHelp;
        break;

    case GET_SERVER:
        vc = vcHelp;
        break;

    case ABOUT:
        vc = vcAbout;
        break;

    default:
        vc = vcUserList;
    }

    [self setCenter:vc];
    
    
}

- ( void )setCenterWithID:( int )vcId
{
    UIViewController *vc = vcUserList;
    switch ( vcId )
    {
    case USER_LIST:
        vc = vcUserList;
        break;

    case SETTINGS:
        vc = vcSettings;
        break;

    case ABOUT:
        vc = vcAbout;
        break;

    case QUICK_START:
        vc = vcHelp;
        break;

    case GET_SERVER:
        vc = vcHelp;
        break;
    }

    [self setCenter:vc];
}

- (void) setCenter:(UIViewController *)vc
{
    UINavigationController *navigationController = self.menuContainerViewController.centerViewController;
    [navigationController setNavigationBarHidden:NO animated:NO];
    NSArray *controllers = @ [vc];
    navigationController.viewControllers = controllers;
    [self.menuContainerViewController setMenuState:MFSideMenuStateClosed];
}

- ( void )setConfiguration
{
    if ( ![[ConfigurationManager sharedManager] isEmpty] )
    {
        NSArray *settings = [[NSUserDefaults standardUserDefaults] objectForKey:@"settings"];
        NSInteger intSelectedConfiguration = [[NSUserDefaults standardUserDefaults] integerForKey:@"currentSelectionIndex"];
        NSDictionary *dictConfiguration = settings [intSelectedConfiguration];
        _lblConfigurationName.text = dictConfiguration [@"CONFIG_NAME"];
        _lblConfigurationURL.text = dictConfiguration [@"backend"];
    }
}

@end
