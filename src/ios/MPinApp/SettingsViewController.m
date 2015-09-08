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

#import "SettingsViewController.h"
#import "ConfigListTableViewCell.h"
#import "AddSettingViewController.h"
#import "AppDelegate.h"
#import "Constants.h"
#import "UIViewController+Helper.h"
#import "MenuViewController.h"
#import "ConfigurationManager.h"
#import "MFSideMenu.h"
#import "ThemeManager.h"
#import "ErrorHandler.h"
#import "HelpViewController.h"


#define NONE 0
#define OTP 1
#define AN 2

@interface SettingsViewController ( ) {
    MPin *sdk;
    NSUInteger intNextConfiguration;
}
- ( IBAction )gotoIdentityList:( id )sender;
- ( IBAction )addQR:( id )sender;

@end

@implementation SettingsViewController

#pragma mark - UIViewController -

- ( void )viewDidLoad
{
    [super viewDidLoad];
    UIBarButtonItem *addItem = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"QR"] style:UIBarButtonItemStylePlain target:self action:@selector( addQR: )];
    UIBarButtonItem *qrItem = [[UIBarButtonItem alloc] initWithImage:[UIImage imageNamed:@"plus-white"] style:UIBarButtonItemStylePlain target:self action:@selector( add: )];
    self.navigationItem.rightBarButtonItems = [NSArray arrayWithObjects:addItem,qrItem, nil];

    if ( [[ConfigurationManager sharedManager] isFirstTimeServerSettings] )
    {
        NSString *filePath = [[NSBundle mainBundle] pathForResource:kHelpFile ofType:@"plist"];
        NSDictionary *menuData = [[NSDictionary alloc] initWithContentsOfFile:filePath];
        HelpViewController *helpControler  = [self.storyboard instantiateViewControllerWithIdentifier:@"HelpViewController"];

        [self presentViewController:helpControler animated:NO completion:nil];
    }
}

- ( void )viewWillAppear:( BOOL )animated
{
    [super viewWillAppear:animated];
    [[ThemeManager sharedManager] beautifyViewController:self];
    [self registerObservers];
    sdk = [[MPin alloc] init];
    sdk.delegate = self;

    [self.tableView reloadData];
    [(MenuViewController *)self.menuContainerViewController.leftMenuViewController setConfiguration];
}

- ( void )viewWillDisappear:( BOOL )animated
{
    [super viewWillDisappear:animated];
    [self unRegisterObservers];
}

#pragma mark - Table view datasource & delegate -

- ( NSInteger )tableView:( UITableView * )tableView numberOfRowsInSection:( NSInteger )section
{
    return [[ConfigurationManager sharedManager] getConfigurationsCount];
}

- ( CGFloat )tableView:( UITableView * )tableView heightForRowAtIndexPath:( NSIndexPath * )indexPath
{
    return 60.f;
}

- ( UITableViewCell * )tableView:( UITableView * )tableView cellForRowAtIndexPath:( NSIndexPath * )indexPath
{
    static NSString *SettingsTableIdentifier = @"ConfigListTableViewCell";
    ConfigListTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:SettingsTableIdentifier];
    if ( cell == nil )
        cell = [[ConfigListTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:SettingsTableIdentifier];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    cell.lblConfigurationName.font = [UIFont fontWithName:@"OpenSans" size:16.f];
    cell.lblConfigurationType.font = [UIFont fontWithName:@"OpenSans" size:14.f];
    [cell setIsSelectedImage:( [[ConfigurationManager sharedManager] getSelectedConfigurationIndex] != indexPath.row )];

    return cell;
}

- ( void )tableView:( UITableView * )tableView willDisplayCell:( UITableViewCell * )cell forRowAtIndexPath:( NSIndexPath * )indexPath
{
    ConfigListTableViewCell *customCell = (ConfigListTableViewCell *)cell;
    customCell.lblConfigurationName.text = [[ConfigurationManager sharedManager] getNameAtIndex:indexPath.row];
    NSInteger service = [[ConfigurationManager sharedManager] getConfigurationTypeAtIndex:indexPath.row];
    switch ( service )
    {
    case LOGIN_ON_MOBILE:
        customCell.lblConfigurationType.text = NSLocalizedString(@"LOGIN_MOBILE_APP", @"");;
        break;

    case LOGIN_ONLINE:
        customCell.lblConfigurationType.text = NSLocalizedString(@"LOGIN_ONLINE_SESSION", @"");
        break;

    case LOGIN_WITH_OTP:
        customCell.lblConfigurationType.text = NSLocalizedString(@"LOGIN_OTP", @"");
        break;

    default:
        break;
    }
    if ( [[ConfigurationManager sharedManager] getSelectedConfigurationIndex] == indexPath.row )
    {
        [customCell setIsSelectedImage:YES];
    }
    else
    {
        [customCell setIsSelectedImage:NO];
    }

    [[ThemeManager sharedManager] customiseConfigurationListCell:customCell];
}

- ( void )tableView:( UITableView * )tableView didSelectRowAtIndexPath:( NSIndexPath * )indexPath
{
    ConfigListTableViewCell *curentCell = (ConfigListTableViewCell *)[tableView cellForRowAtIndexPath:indexPath];

    if ( [[ConfigurationManager sharedManager] getSelectedConfigurationIndex] == indexPath.row )
    {
        return;
    }
    else
    {
        if ( curentCell )
        {
            curentCell.imgViewSelected.image = [UIImage imageNamed:@"pin-dot-incorrect"];
        }
        intNextConfiguration = indexPath.row;
        [self changeConfiguration:indexPath.row];
    }
}

- ( BOOL )tableView:( UITableView * )tableView canEditRowAtIndexPath:( NSIndexPath * )indexPath
{
    return NO;
}

- ( void )OnSetBackendCompleted:( id )sender
{
    [[ConfigurationManager sharedManager] setSelectedConfiguration:intNextConfiguration];
    [(MenuViewController *)self.menuContainerViewController.leftMenuViewController setConfiguration];
    [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(@"CONFIGURATIONS_CONFIG_CHANGED",@"Configuration changed")
     addActivityIndicator:NO
     hideAfter:6];
    [_tableView reloadData];
}

- ( void )OnSetBackendError:( id )sender error:( NSError * )error
{
    MpinStatus *status = ( error.userInfo ) [kMPinSatus];
    [[ErrorHandler sharedManager] updateMessage:NSLocalizedString(status.statusCodeAsString, @"UNKNOWN ERROR") addActivityIndicator:NO hideAfter:6];
    [_tableView reloadData];
}

#pragma mark - Custom actions -

-( void ) changeConfiguration: ( NSUInteger ) index
{
    [[ErrorHandler sharedManager] presentMessageInViewController:self
     errorString:NSLocalizedString(@"HUD_CHANGE_CONFIGURATION", @"")
     addActivityIndicator:YES
     minShowTime:0];

    NSString *rpsPrefix = [[ConfigurationManager sharedManager] getPrefixAtIndex:index];
    NSString *url = [[ConfigurationManager sharedManager] getURLAtIndex:index];
    if ( [rpsPrefix isEqualToString:@""] )
    {
        rpsPrefix = nil;
    }

    [sdk SetBackend:url rpsPrefix:rpsPrefix];
}

- ( IBAction )add:( id )sender
{
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    AddSettingViewController *addViewController = [storyboard instantiateViewControllerWithIdentifier:@"AddConfig"];
    addViewController.isEdit = NO;
    [self.navigationController pushViewController:addViewController animated:YES];
}

- ( IBAction )addQR:( id )sender
{
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    UIViewController *vcQR = [storyboard instantiateViewControllerWithIdentifier:@"QRController"];
    [self.navigationController pushViewController:vcQR animated:YES];
}

- ( IBAction )edit:( id )sender
{
    if ( [[ConfigurationManager sharedManager] isEmpty] )
        return;

    if ( [[ConfigurationManager sharedManager] getSelectedConfigurationIndex] > ( [[ConfigurationManager sharedManager] defaultConfigCount] - 1 ) )
    {
        UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
        AddSettingViewController *addViewController = [storyboard instantiateViewControllerWithIdentifier:@"AddConfig"];
        addViewController.isEdit = YES;
        //FIXME
        addViewController.selectedIndex = [[ConfigurationManager sharedManager] getSelectedConfigurationIndex];
        [self.navigationController pushViewController:addViewController animated:YES];
    }
    else
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"WARNING_CANNOT_EDIT_PREDEFINED_CONFIG", @"")
         addActivityIndicator:NO
         minShowTime:3];
    }
}

- ( IBAction )deleteConfiguration:( id )sender
{
    if ( [[ConfigurationManager sharedManager] isEmpty] )
        return;

    if ( [[ConfigurationManager sharedManager] getSelectedConfigurationIndex] > ( [[ConfigurationManager sharedManager] defaultConfigCount] - 1 ) )
    {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"KEY_WARNING", @"")
                                  message:NSLocalizedString(@"WARNING_THIS_WILL_DELETE_ALL_IDS", @"")
                                  delegate:self
                                  cancelButtonTitle:NSLocalizedString(@"KEY_CANCEL", @"")
                                  otherButtonTitles:NSLocalizedString(@"KEY_OKBTN", @""), nil];
        [alertView show];
    }
    else
    {
        [[ErrorHandler sharedManager] presentMessageInViewController:self
         errorString:NSLocalizedString(@"WARNING_CANNOT_DELETE_PREDEFINED_CONFIG", @"")
         addActivityIndicator:NO
         minShowTime:3];
    }
}

- ( IBAction )gotoIdentityList:( id )sender
{
    MenuViewController *menuVC = (MenuViewController *)self.menuContainerViewController.leftMenuViewController;
    [menuVC setCenterWithID:0];
}

- ( IBAction )showLeftMenuPressed:( id )sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

#pragma mark - Alert view delegate -

- ( void )alertView:( UIAlertView * )alertView clickedButtonAtIndex:( NSInteger )buttonIndex
{
    NSInteger intSelectedIndex = [[ConfigurationManager sharedManager] getSelectedConfigurationIndex];

    switch ( buttonIndex )
    {
    case 1:
        [[ConfigurationManager sharedManager] deleteConfigurationAtIndex:intSelectedIndex];
        [[ConfigurationManager sharedManager] setSelectedConfiguration:0];
        [self.tableView reloadData];
        [sdk SetBackend:[[ConfigurationManager sharedManager] getSelectedConfiguration]];
        break;
    }
}

#pragma mark - NSNotification handlers -

-( void ) networkUp
{
    [[ThemeManager sharedManager] hideNetworkDown:self];
}

-( void ) networkDown
{
    NSLog(@"Network DOWN Notification");
    [[ErrorHandler sharedManager] hideMessage];
    [self.view layoutIfNeeded];
    [UIView animateWithDuration:kFltNoNetworkMessageAnimationDuration animations: ^ {
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
