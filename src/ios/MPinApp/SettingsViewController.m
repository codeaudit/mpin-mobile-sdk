//
//  SettingsViewController.m
//  MPinApp
//
//  Created by Georgi Georgiev on 1/19/15.
//  Copyright (c) 2015 Certivox. All rights reserved.
//

#import "SettingsViewController.h"
#import "ConfigListTableViewCell.h"
#import "AddSettingViewController.h"
#import "AppDelegate.h"
#import "Constants.h"
#import "ATMHud.h"
#import "UIViewController+Helper.h"
#import "MenuViewController.h"
#import "ConfigurationManager.h"
#import "MFSideMenu.h"
#import "ThemeManager.h"

#define NONE 0
#define OTP 1
#define AN 2

@interface SettingsViewController () {
    ATMHud* hud;
    MPin* sdk;
}
- (IBAction)gotoIdentityList:(id)sender;

@end

@implementation SettingsViewController

#pragma mark - UIViewController -

- (void)viewDidLoad
{
    [super viewDidLoad];

    hud = [[ATMHud alloc] initWithDelegate:self];
    sdk = [[MPin alloc] init];
    sdk.delegate = self;
    [[ThemeManager sharedManager] beautifyViewController:self];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.tableView reloadData];
    [(MenuViewController*)self.menuContainerViewController.leftMenuViewController setConfiguration];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

#pragma mark - Table view datasource & delegate -

- (NSInteger)tableView:(UITableView*)tableView numberOfRowsInSection:(NSInteger)section
{
    return [[ConfigurationManager sharedManager] getConfigurationsCount];
}

- (CGFloat)tableView:(UITableView*)tableView heightForRowAtIndexPath:(NSIndexPath*)indexPath
{
    return 60.f;
}

- (UITableViewCell*)tableView:(UITableView*)tableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
    static NSString* SettingsTableIdentifier = @"ConfigListTableViewCell";
    ConfigListTableViewCell* cell = [tableView dequeueReusableCellWithIdentifier:SettingsTableIdentifier];
    if (cell == nil)
        cell = [[ConfigListTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:SettingsTableIdentifier];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    cell.lblConfigurationName.font = [UIFont fontWithName:@"OpenSans" size:16.f];
    cell.lblConfigurationType.font = [UIFont fontWithName:@"OpenSans" size:14.f];
    [cell setIsSelectedImage:([[ConfigurationManager sharedManager] getSelectedConfigurationIndex] != indexPath.row)];
    return cell;
}

- (void)tableView:(UITableView*)tableView willDisplayCell:(UITableViewCell*)cell forRowAtIndexPath:(NSIndexPath*)indexPath
{
    ConfigListTableViewCell* customCell = (ConfigListTableViewCell*)cell;
    customCell.lblConfigurationName.text = [[ConfigurationManager sharedManager] getNameAtIndex:indexPath.row];
    NSInteger service = [[ConfigurationManager sharedManager] getConfigurationTypeAtIndex:indexPath.row];
    switch (indexPath.row) {
    case NONE:
        customCell.lblConfigurationType.text = NSLocalizedString(@"LOGIN_MOBILE_APP", @"");
        break;
    case OTP:
        customCell.lblConfigurationType.text = NSLocalizedString(@"LOGIN_OTP", @"");
        break;
    case AN:
        customCell.lblConfigurationType.text = NSLocalizedString(@"LOGIN_ONLINE_SESSION", @"");
        break;
    default:
        switch (service) {
        case LOGIN_ON_MOBILE:
            customCell.lblConfigurationType.text = NSLocalizedString(@"LOGIN_MOBILE_APP", @"");;
            break;
        case LOGIN_ONLINE:
            customCell.lblConfigurationType.text = NSLocalizedString(@"LOGIN_ONLINE_SESSION", @"");
            break;
        case LOGIN_WITH_OTP:
            customCell.lblConfigurationType.text = NSLocalizedString(@"LOGIN_OTP", @"");
            break;
        }
        break;
    }
    [customCell setIsSelectedImage:([[ConfigurationManager sharedManager] getSelectedConfigurationIndex] == indexPath.row)];
    [[ThemeManager sharedManager] customiseConfigurationListCell:customCell];
}

- (void)tableView:(UITableView*)tableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{

    if ([[ConfigurationManager sharedManager] getSelectedConfigurationIndex] == indexPath.row) {
        return;
    }
    NSInteger intSelectedConfiguration = indexPath.row;
    hud.minShowTime = 1.0;
    [hud setCaption:NSLocalizedString(@"HUD_CHANGE_CONFIGURATION", @"")];
    [hud setActivity:YES];
    [hud showInView:self.view];

    NSString* rpsPrefix = [[ConfigurationManager sharedManager] getPrefixAtIndex:intSelectedConfiguration];
    NSString* url = [[ConfigurationManager sharedManager] getURLAtIndex:intSelectedConfiguration];
    if ([rpsPrefix isEqualToString:@""]) {
        rpsPrefix = nil;
    }

    [sdk SetBackend:url rpsPrefix:rpsPrefix];

    [[ConfigurationManager sharedManager] setSelectedConfiguration:indexPath.row];
    [tableView reloadData];
    [(MenuViewController*)self.menuContainerViewController.leftMenuViewController setConfiguration];
}

- (void)OnSetBackendCompleted:(id)sender
{
    [hud hide];
}

- (void)OnSetBackendError:(id)sender error:(NSError*)error
{
    [hud hide];
    MpinStatus* status = (error.userInfo)[kMPinSatus];
    [self showError:[status getStatusCodeAsString] desc:status.errorMessage];
}

- (BOOL)tableView:(UITableView*)tableView canEditRowAtIndexPath:(NSIndexPath*)indexPath
{
    return NO;
}

#pragma mark - Custom actions -
- (IBAction)add:(id)sender
{
    UIStoryboard* storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
    AddSettingViewController* addViewController = [storyboard instantiateViewControllerWithIdentifier:@"AddConfig"];
    addViewController.isEdit = NO;
    [self.navigationController pushViewController:addViewController animated:YES];
}

- (IBAction)edit:(id)sender
{
    if ([[ConfigurationManager sharedManager] getSelectedConfigurationIndex] > 2) {
        UIStoryboard* storyboard = [UIStoryboard storyboardWithName:@"Main_iPhone" bundle:nil];
        AddSettingViewController* addViewController = [storyboard instantiateViewControllerWithIdentifier:@"AddConfig"];
        addViewController.isEdit = YES;
        //FIXME
        addViewController.selectedIndex = [[ConfigurationManager sharedManager] getSelectedConfigurationIndex];
        [self.navigationController pushViewController:addViewController animated:YES];
    }
    else {
        hud.minShowTime = 2.0;
        [hud setCaption:NSLocalizedString(@"WARNING_CANNOT_EDIT_PREDEFINED_CONFIG", @"")];
        [hud setActivity:NO];
        [hud showInView:self.view];
        [hud hide];
    }
}

- (IBAction)deleteConfiguration:(id)sender
{
    if ([[ConfigurationManager sharedManager] getSelectedConfigurationIndex] > 2) {
        UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"KEY_WARNING", @"")
                                                            message:NSLocalizedString(@"WARNING_THIS_WILL_DELETE_ALL_IDS", @"")
                                                           delegate:self
                                                  cancelButtonTitle:NSLocalizedString(@"KEY_CANCEL", @"")
                                                  otherButtonTitles:NSLocalizedString(@"KEY_OKBTN", @""), nil];
        [alertView show];
    }
    else {
        hud.minShowTime = 2.0;
        [hud setCaption:NSLocalizedString(@"WARNING_CANNOT_DELETE_PREDEFINED_CONFIG", @"")];
        [hud setActivity:NO];
        [hud showInView:self.view];
        [hud hide];
    }
}

- (IBAction)gotoIdentityList:(id)sender
{
    MenuViewController* menuVC = (MenuViewController*)self.menuContainerViewController.leftMenuViewController;
    [menuVC setCenterWithID:0];
}

- (IBAction)showLeftMenuPressed:(id)sender
{
    [self.menuContainerViewController toggleLeftSideMenuCompletion:nil];
}

#pragma mark - Alert view delegate -

- (void)alertView:(UIAlertView*)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    NSInteger intSelectedIndex = [[ConfigurationManager sharedManager] getSelectedConfigurationIndex];

    switch (buttonIndex) {
    case 1:
        [[ConfigurationManager sharedManager] deleteConfigurationAtIndex:intSelectedIndex];
        [[ConfigurationManager sharedManager] setSelectedConfiguration:0];
        [self.tableView reloadData];

        break;
    }
}

@end
