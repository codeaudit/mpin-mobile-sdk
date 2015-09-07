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
#import "MenuItem.h"


@interface MenuViewController ( ) {
    NSMutableArray * menu;
}
- (void) initMenuItems;
@end

@implementation MenuViewController

- (void) initMenuItems {
    menu = [NSMutableArray array];
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:kHelpFile ofType:@"plist"];
    NSDictionary *menuData = [[NSDictionary alloc] initWithContentsOfFile:filePath];
    
    [menu addObject:[[MenuItem alloc] initWith:NSLocalizedString(@"MENUVC_OPTION_0",@"")
                                    controller:((AppDelegate *)[UIApplication sharedApplication].delegate).vcUserList]];
    
    [menu addObject:[[MenuItem alloc] initWith:NSLocalizedString(@"MENUVC_OPTION_1",@"")
                                    controller:[self.storyboard instantiateViewControllerWithIdentifier:@"SettingsViewController"]]];
    
    HelpViewController * helpControler  = [self.storyboard instantiateViewControllerWithIdentifier:@"helpcontroller"];
    helpControler.dataSource = [menuData objectForKey:kQuickStartGuide];
    
    [menu addObject:[[MenuItem alloc] initWith:NSLocalizedString(@"MENUVC_OPTION_2",@"")
                     controller:helpControler]];
    
    helpControler  = [self.storyboard instantiateViewControllerWithIdentifier:@"helpcontroller"];
    helpControler.dataSource = [menuData objectForKey:kMpinServerGuide];
    
    [menu addObject:[[MenuItem alloc] initWith:NSLocalizedString(@"MENUVC_OPTION_3",@"")
                                    controller:helpControler]];
    
    [menu addObject:[[MenuItem alloc] initWith:NSLocalizedString(@"MENUVC_OPTION_4",@"")
                     controller:[self.storyboard instantiateViewControllerWithIdentifier:@"AboutViewController"]]];

}



- ( void )viewDidLoad
{
    [super viewDidLoad];
    [self initMenuItems];
    [[ThemeManager sharedManager] beautifyViewController:self];
    
    if ([[ConfigurationManager sharedManager] isFirstTimeLaunch]) {
        NSString *filePath = [[NSBundle mainBundle] pathForResource:kHelpFile ofType:@"plist"];
        NSDictionary *menuData = [[NSDictionary alloc] initWithContentsOfFile:filePath];
        HelpViewController * helpControler  = [self.storyboard instantiateViewControllerWithIdentifier:@"helpcontroller"];
        helpControler.dataSource = [menuData objectForKey:kFirstTimeLaunch];
        [self presentViewController:helpControler animated:NO completion:nil];
    }
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    [self setConfiguration];
   
    
    
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
    return [menu count];
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
    ( (MenuTableViewCell *)cell ).lblMenuID.text = ((MenuItem *)[menu objectAtIndex:indexPath.row]).title;
}

- ( void )tableView:( UITableView * )tableView didSelectRowAtIndexPath:( NSIndexPath * )indexPath
{
    UIViewController *vc = nil;

    [tableView deselectRowAtIndexPath:indexPath animated:YES];

    vc = ((MenuItem *)[menu objectAtIndex:indexPath.row]).controller;
   
    UINavigationController *navigationController = self.menuContainerViewController.centerViewController;
    NSArray *controllers = @ [vc];
    navigationController.viewControllers = controllers;
    [self.menuContainerViewController setMenuState:MFSideMenuStateClosed];
}

- ( void )setCenterWithID:( int )vcId
{
    UIViewController *vc = nil;
  
    vc = ((MenuItem *)[menu objectAtIndex:vcId]).controller;
    
    UINavigationController *navigationController = self.menuContainerViewController.centerViewController;
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
